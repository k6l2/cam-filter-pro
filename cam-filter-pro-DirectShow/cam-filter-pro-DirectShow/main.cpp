#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <dshow.h>
#include <cassert>
#include "../../src/Matrix.h"
// maybe research this wrapper shit again? http://www.codeguru.com/cpp/g-m/directx/directshow/article.php/c9551/DirectShow-SingleFrame-Capture-Class-Without-MFC.htm
/// WTF? https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/ac877e2d-80a7-47b6-b315-5e3160b8b219/alternative-for-isamplegrabber?forum=windowsdirectshowdevelopment ///
#pragma comment(lib,"Strmiids.lib")
const sf::String STR_WINDOW_TITLE = "Cam Filter Pro - DirectShow";
interface ISampleGrabberCB : public IUnknown
{
	virtual STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample) = 0;
	virtual STDMETHODIMP BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen) = 0;
};
interface ISampleGrabber : public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetOneShot(BOOL OneShot) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType(const AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(BOOL BufferThem) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(long *pBufferSize, long *pBuffer) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(IMediaSample **ppSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback(ISampleGrabberCB *pCallback, long WhichMethodToCallback) = 0;
};
static const IID IID_ISampleGrabber = { 0x6B652FFF, 0x11FE, 0x4fce,{ 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F } };
static const IID IID_ISampleGrabberCB = { 0x0579154A, 0x2B53, 0x4994,{ 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85 } };
static const CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3,{ 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };
/// The following is literally garbage?? ///
//#define DsHook(a,b,c) if (!c##_) { INT_PTR* p=b+*(INT_PTR**)a;   VirtualProtect(&c##_,4,PAGE_EXECUTE_READWRITE,&no);\
//                                          *(INT_PTR*)&c##_=*p;   VirtualProtect(p,    4,PAGE_EXECUTE_READWRITE,&no);\
//										  *p=(INT_PTR)c; }
//// Here you get image video data in buf / len. Process it before calling Receive_ because renderer dealocates it.
//HRESULT ( __stdcall * Receive_ ) ( void* inst, IMediaSample *smp ) ; 
//HRESULT   __stdcall   Receive    ( void* inst, IMediaSample *smp ) {     
//    BYTE*     buf;    smp->GetPointer(&buf); DWORD len = smp->GetActualDataLength();
//    HRESULT   ret  =  Receive_   ( inst, smp );   
//    return    ret; 
//}
/// Most of this shit is from the following old as fuck M$ page: https://msdn.microsoft.com/en-us/library/ms783323(VS.85).aspx
///		(tutorials -> Using the Sample Grabber)
///		which doesn't seem to be on their new spiffy looking site?? https://msdn.microsoft.com/en-us/library/windows/desktop/dd375454(v=vs.85).aspx
HRESULT GetUnconnectedPin(
    IBaseFilter *pFilter,   // Pointer to the filter.
    PIN_DIRECTION PinDir,   // Direction of the pin to find.
    IPin **ppPin)           // Receives a pointer to the pin.
{
    *ppPin = 0;
    IEnumPins *pEnum = 0;
    IPin *pPin = 0;
    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr))
    {
        return hr;
    }
    while (pEnum->Next(1, &pPin, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PinDir)
        {
            IPin *pTmp = 0;
            hr = pPin->ConnectedTo(&pTmp);
            if (SUCCEEDED(hr))  // Already connected, not the pin we want.
            {
                pTmp->Release();
            }
            else  // Unconnected, this is the pin we want.
            {
                pEnum->Release();
                *ppPin = pPin;
                return S_OK;
            }
        }
        pPin->Release();
    }
    pEnum->Release();
    // Did not find a matching pin.
    return E_FAIL;
}
HRESULT ConnectFilters(
	IGraphBuilder *pGraph, // Filter Graph Manager.
	IPin *pOut,            // Output pin on the upstream filter.
	IBaseFilter *pDest)    // Downstream filter.
{
	if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL))
	{
		return E_POINTER;
	}
#ifdef debug
	PIN_DIRECTION PinDir;
	pOut->QueryDirection(&PinDir);
	_ASSERTE(PinDir == PINDIR_OUTPUT);
#endif
	// Find an input pin on the downstream filter.
	IPin *pIn = 0;
	HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
	if (FAILED(hr))
	{
		return hr;
	}
	// Try to connect them.
	hr = pGraph->Connect(pOut, pIn);
	pIn->Release();
	return hr;
}
HRESULT ConnectFilters(
    IGraphBuilder *pGraph, 
    IBaseFilter *pSrc, 
    IBaseFilter *pDest)
{
    if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL))
    {
        return E_POINTER;
    }
    // Find an output pin on the first filter.
    IPin *pOut = 0;
    HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
    if (FAILED(hr)) 
    {
        return hr;
    }
    hr = ConnectFilters(pGraph, pOut, pDest);
    pOut->Release();
    return hr;
}
// returns the # of actual devices there are, which is <= numDevicesToEnumerate
ULONG enumerateVideoInputDevices()
{
	HRESULT hr;
	ICreateDevEnum* cDevEnum = nullptr;
	IEnumMoniker* enumMoniker = nullptr;
	IMoniker* moniker = nullptr;
	ULONG numMonikersRetrieved = 0;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&cDevEnum);
	hr = cDevEnum ? cDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumMoniker, 0) : 0;
	while (enumMoniker->Next(1, &moniker, NULL) == S_OK)
	{
		IPropertyBag *pPropBag;
		hr = moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPropBag);
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"FriendlyName", &varName, NULL);
		//printf("Device[%d]=\"%ls\"\n", numMonikersRetrieved++, varName.bstrVal);
		std::wcout << "Device[" << numMonikersRetrieved++ << "]=\"" << varName.bstrVal << "\"\n";
		VariantClear(&varName);
		pPropBag->Release();
		moniker->Release();
	}
	enumMoniker->Release();
	cDevEnum->Release();
	return numMonikersRetrieved;
}
int main(int argc, char** argv)
{
	const bool useProgramArgs = argc == 3;
	int paramDevice;
	int paramFormat;
	if (useProgramArgs)
	{
		paramDevice = atoi(argv[1]);
		paramFormat = atoi(argv[2]);
	}
	else
	{
		std::cout << "NOTE: if you don't want the program to prompt for device/format, use the following.\n";
		std::cout << "Usage: cam-filter-pro-DirectShow.exe [device] [format]\n";
	}
	// Reference: //
	// http://www.codeproject.com/Articles/12869/Real-time-video-image-processing-frame-grabber-usi //
	HRESULT hr = CoInitialize(nullptr);
	const ULONG numVideoInputDevices = enumerateVideoInputDevices();
	ULONG selectedDevice;
	if (useProgramArgs)
	{
		selectedDevice = paramDevice;
	}
	else
	{
		std::cout << "Select a device:";
		std::cin >> selectedDevice;
	}
	IGraphBuilder* graph = nullptr;
	IMediaControl* mControl = nullptr;
	ICreateDevEnum* cDevEnum = nullptr;
	IEnumMoniker* eMoniker = nullptr;
	ULONG numMonikersRetrieved;
	IMoniker** monikers = new IMoniker*[numVideoInputDevices];
	IBaseFilter* camFilter = nullptr;
	IBaseFilter* sGrabberFilter = nullptr;
	ISampleGrabber* sGrabber = nullptr;
	IMediaFilter* mFilter = nullptr;
	IEnumPins* camOutputPins = nullptr;
	IPin* camOutputPin = nullptr;
	IAMStreamConfig* camConfig = nullptr;
	int numFormatCapabilities;
	int configStructureSize;//in BYTES
	VIDEO_STREAM_CONFIG_CAPS videoCapabilities;
	AM_MEDIA_TYPE mt;
	AM_MEDIA_TYPE* mtPointer;
	VIDEOINFOHEADER* vih;
	// Setup the graph, media filter, and media control //
	hr = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graph);
	hr = graph->QueryInterface(IID_IMediaFilter, (void**)&mFilter);
	/// no idea what's actually going on here... is the filter even necessary??
	mFilter->SetSyncSource(nullptr);
	hr = graph->QueryInterface(IID_IMediaControl, (void**)&mControl);
	// Now actually get the video devices here: //
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&cDevEnum);
	hr = cDevEnum ? cDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &eMoniker, 0) : 0;
	hr = eMoniker->Next(numVideoInputDevices, monikers, &numMonikersRetrieved);
	assert(numVideoInputDevices == numMonikersRetrieved);
	assert(selectedDevice < numVideoInputDevices);
	// get selected capture device
	hr = monikers[selectedDevice]->BindToObject(nullptr, nullptr, IID_IBaseFilter, (void**)&camFilter);
	hr = graph->AddFilter(camFilter, L"Camera Capture Filter");
	//Get camera configuration shit now: //
	hr = camFilter->EnumPins(&camOutputPins);
	hr = camOutputPins->Next(1, &camOutputPin, nullptr);
	hr = camOutputPin->QueryInterface(IID_IAMStreamConfig, (void**)&camConfig);
	hr = camConfig->GetNumberOfCapabilities(&numFormatCapabilities, &configStructureSize);
	for (unsigned c = 0; c < unsigned(numFormatCapabilities); c++)
	{
		hr = camConfig->GetStreamCaps(c, &mtPointer, (BYTE*)&videoCapabilities);
		std::cout << "format#"<<c<<
			" minFrameInterval=" << videoCapabilities.MinFrameInterval <<
			" maxFrameInterval=" << videoCapabilities.MaxFrameInterval<<
			" maxOutputSize="<<videoCapabilities.MaxOutputSize.cx<<","<<videoCapabilities.MaxOutputSize.cy<<
			" minOutputSize="<<videoCapabilities.MinOutputSize.cx<<","<<videoCapabilities.MinOutputSize.cy
			<<std::endl;
		if (mtPointer->formattype == FORMAT_VideoInfo)
		{
			vih = (VIDEOINFOHEADER*)mtPointer->pbFormat;
			std::cout << "\tAvgTimePerFrame=" << vih->AvgTimePerFrame<<std::endl;
		}
	}
	int selectedFormatNum;
	if (useProgramArgs)
	{
		selectedFormatNum = paramFormat;
	}
	else
	{
		std::cout << "Select a format#:";
		std::cin >> selectedFormatNum;
	}
	hr = camConfig->GetStreamCaps(selectedFormatNum, &mtPointer, (BYTE*)&videoCapabilities);
	camConfig->SetFormat(mtPointer);
	std::cout << "program full path=" << argv[0] << std::endl;
	////////////////////////////////////////
	hr = CoCreateInstance(CLSID_SampleGrabber, nullptr, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&sGrabberFilter);
	hr = graph->AddFilter(sGrabberFilter, L"Sample Grabber");
	sGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&sGrabber);
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;
	sGrabber->SetMediaType(&mt);
	hr = ConnectFilters(graph, camFilter, sGrabberFilter);
	hr = sGrabber->SetBufferSamples(true);
	hr = mControl->Run();
	// Getting the format of the camera video stream: //
	hr = sGrabber->GetConnectedMediaType(&mt);
	//FreeMediaType(mt); ///TODO: figure out wtf to do with this shit?
	vih = (VIDEOINFOHEADER*)mt.pbFormat;
	std::cout << "biWidth=" << vih->bmiHeader.biWidth << " biHeight=" << vih->bmiHeader.biHeight << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window{ sf::VideoMode(vih->bmiHeader.biWidth, vih->bmiHeader.biHeight), STR_WINDOW_TITLE };
	Matrix theMatrix(window);
	sf::Image cameraImage;
	cameraImage.create(vih->bmiHeader.biWidth, vih->bmiHeader.biHeight, sf::Color(255, 0, 0));
	sf::Texture cameraTex;
	cameraTex.loadFromImage(cameraImage);
	sf::Sprite cameraSprite(cameraTex);
	const float camImageAspectRatio = cameraImage.getSize().x / float(cameraImage.getSize().y);
	long bufferSize = 0;
	char* cameraBuffer = nullptr;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (!window.hasFocus())
				{
					break;
				}
				switch (event.key.code)
				{
				case sf::Keyboard::F9:
				{
					sf::Vector2u newSize = window.getSize();
					newSize.x = unsigned(newSize.y*camImageAspectRatio);
					window.setSize(newSize);
				}
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				}
				break;
			}
		}
		window.clear();
		hr = sGrabber->GetCurrentBuffer(&bufferSize, nullptr);
		if (bufferSize > 0)
		{
			cameraBuffer = new char[bufferSize];
			hr = sGrabber->GetCurrentBuffer(&bufferSize, (long*)cameraBuffer);
			//std::cout << "bufferSize=" << bufferSize << std::endl;
			//cameraImage.loadFromMemory(cameraBuffer, bufferSize);
			//memcpy((void*)cameraImage.getPixelsPtr(), cameraBuffer, bufferSize);
			for (long p = 0; p < bufferSize; p += 3)
			{
				unsigned pixelIndex = p / 3;
				unsigned row = pixelIndex / vih->bmiHeader.biWidth;
				unsigned col = pixelIndex % vih->bmiHeader.biWidth;
				cameraImage.setPixel(col, row, sf::Color(cameraBuffer[p + 2], cameraBuffer[p + 1], cameraBuffer[p]));
			}
			cameraImage.flipVertically();
			delete cameraBuffer;
		}
		cameraTex.loadFromImage(cameraImage);
		//window.draw(cameraSprite);
		theMatrix.appFocus(window.hasFocus());
		theMatrix.stepShader(1 / 60.f, cameraSprite);
		window.display();
	}
	return EXIT_SUCCESS;
}
