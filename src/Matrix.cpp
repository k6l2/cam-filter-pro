#include "Matrix.h"
#include <cassert>
#include <queue>
#include <iostream>
#include <sstream>
#include "Z85.h"
const unsigned Matrix::CHARACTER_W = 16;
const unsigned Matrix::CHARACTER_H = 16;
const char* const Matrix::FONT_DATA = "0096101F#.00rk9pG5]4F{qi/002U]0000]v@kT[{M+dn002u}0006qv%h@ps&p6O000bl0000Kw]Iy1Gepwu000b10000kxkz<9QQ5X(000cM0026mxK>$88F<jD002Bl000jVxK$Q./3Vz3002V<0000SxLp%=1Z}E#002WI0000AxL?<o!9l]$002nL0004Uy&r/(01hEs002i&0004YzddWd0nEOZ002W}0000wzEEx[K?[8d0002=0008jAbP/t2%!)!002si0002.Ab]JeCebb2000b/0000.0000l0rJl3000000001p05<(T000000096b0ic200000000icg0j@jh0000000ri?0qVV}0000000Aoe0l$MA0000000Ju[0B=3e0000000SAg0J#F7000000000T0000100000009660hPU@0000000ic90jh.b0000000riB0n4nL0000000Ao90lCiw0000000JuF0x{F:0000000SAb0JDb4009ie0001p05<(W009ie0096b0ic23009ie00icg0j@jk009ie00ri?0qVV$009ie00Aoe0l$MD009ie00Ju[0B=3h009ie00SAg0K1jrixnVDfFCp9fcCK.yh1/eB7/o8z//[ufcz@9kXa}Gg+JoRz6gFOCxXl-fF>y%A4V6Hx8<T^irh5tiuO>kg5%0dfAr)eaoj4{aolV!BPudRx-GmQzVB?UxAh-Dz2!IO@@u%qaoj4{aoj7}gYP=vwb{VJx8<:*e=YwYzVBROaoj4{aolP=Ck#*!e=WN9gYPcdy5&hMe=Y!&ASx-ag?5c{05ayf0aC{Z05EI:xlw30ASw#qwDluyBPujTve$s4g?5c{05ayf0aC{Z05EtTv@=H8wN]H-avZo!Bz>/jvq])>AV^g>f?JMxg?5c{08iD40aM1H0b?<O0a=dv0bhBw03znh0b?<P%nJ7q0a#pL0at>D0b8vz0ce5Z05KWd05jEC03zm}05KW+05ayeo<}KLz/65@x<&bNz/fVnxl4B1xK#ibg+HHPattP7gCgBu08iD40aM1H0b?<O0a=dv0bhBw03znh0b?<P%nJ7q0a#pL0at>D0b8vz0ce5Z05KWd05jEC03zm}05BQc05BQc05sKifF)k)gxo9egYQ$+fAr)ekMH<oC10KT10{wXvq3G5xKBjW@@@D60RuCk0ds>z0ds>z0E1M*t4XDL^ZT3e{:PZE4NxEgFi25TQk5ZH01w]dkN!2a2NyIm0S&x60004VXb(R-mqb7TxHCp5l}7=Smqb7TxHGfg1v-wwVJ*X704UqS1vV#(U)7z3xHz?200ic+0005d0}33<00-H8kP*7k2TwW$2lUeg00SPf0%eSe1][(j1{1=a0@@u507Ib+hY8JVjte/d@x7U&jom8$@x8.}@x76mfAB:UXC#Qf2UwYrUS#8*i1X9W00@Rui9w3HaRu1R5HfHvCMbUV}u39-5<DCq00ic20001E0=Xhx00-H0kPfUe2TwW@2l>tm0%eG60%eVg1owGc1{1-]B$U=>053C/jte?}jom5!e?O+ee?NbUfAB:UXC#Tg2UwYrUS#8*i1X9W00@Rui9u[t5I>dD6/gn^EslIBASg/yEe<lG0004z0E1P+00ria08t^00se@@2)yIp0@@Me1oFPe1{j@e1Q1&d0096?B$U=>052cJfbD.KffGg}e?O+e05lednOCTm000o@xKObgqDL)<h[n[]%g*P%5I(o?7C^][Esp%uEf1M*}Uyf[000000=XoY02>[W0h&7V0u-HIaPJt}6Cz2*3jc[u0S:kU85Ke95FbD.4gAQK2)ZRm0rshY6+*/P1oY+^6a$JN1%1W484D/V1%j)93>0js2NpFm0#qQ:0$+LvmS{Bz7YVi9jtdHkfbD+H7D.ZK7D?Tjjt5lse?O+ee?O+ee?O+ee?O+ee=WHRe?O+ee?O+ee?O>j0zc$n015YGnOzLpkVgYz5Jvl2aPGkpsPHx8h6S?Xh6S?X6/cNH6/cNXh8t$6h8v9mh3Uu-gxIMAEslJ(EslJ(EslJ(EslIACys)/EslJ(EslJ(EslJ(Ee<lG0094K0RuCk01e=w0eS%50tVCo7ypMX3KM$q6+?q]5FbD.4gAQK2)Z+u1Q1]e0rrAi1P[(i1@4ZQ0%O@A5!LiO0rrVw0r>@ofe<ZFffy)IfbD+H7D-oc7D.ZJe?O+ee?O+ee?O+ee?O+ee?O+ee=WHRe?O+ef!-S1xT[9D02wdtvx-0{su3)KXD0ABZ=kD$bnzBrbnA0(gC{xT6/cNXh8v9m6-&JbEsp$*EG291Esp}[AEAm-Esq9KEsp%uEe<lJ000000=XoY00ria01e=q022u.kSO]W6HixC6a+V:4gAQK2l>?O0S-=u1{k2j3Ku?p01PFB0%]by5c&pu1Q1&d01]>I0%!#g0S-r>B$U=>06-O07D:t!jom8KjPc6UfbD+H7D:r5e?NbRe?O+ee?O+ee?]3XnOCTm0rr*9xKObgqDL)<h[n]5%g*P%6c!kFb{KcX0w=OTi@}h-7zO.-EslIACys]sIg6/2Esp%u0=-Tp}U(uFEFOwu00ic20007u0=Xhx03qiokWjsjav47M6+?G36Cy-S0%X/e1S!W583QlF1rOQ50%xwO5!LP.3jc+u0r-@A3<c>T92PO50rruk1%j(]3i{Um2l(a=7zWI+4H9ET5^5ZS0TPbo0%wS93R2p)hY8JV7D:]AkrGIQ7D:t!jPc6U0wtlK@tdNH7D?Tjjp}Dcjoc#^f!-S1xT[9J03B<Dvx-0{su3)KXD0=LZ=kEybnA0Jbq*XXbq*Y6gC{ymgC{%6bnA0HgC{%6bnA0nCL.gjEsp%uEslLu}UCR*Esp{rETMS)EslJ(EslJ(Esh7@0004z0ds]*00ri:kOj211v9a<0%eG60rri60rCVkfe<ZFffy)I0wk#Se=WQW0zc$n00965nOzLpkVgYz5Jvl21PMm%sRA1{gKTX10=!5p00ri30002}0=Xhx00-Gi0a4]m0sPx44fc.w2l>tm3KNgC2l>tm0%eG60rrGk0%eJ81X9U*hY8JVjte/Ijom5!e?O+ee?O+ee?NbRe?O+ee?]3XnOCTm1PO%5xKObgqDL)<h[n[%%g*P%6c!kFd/*kr7D0AL}H9^uEslJ(0=-U/Ig6/200ri30002}0=Xhx00-Gi0a4]m0sPx44fM$y2l>tm3KNgC2l>tm0%eG60@@Yo0S-u607Ib+hY8JVjte/Ijom5!e?O+ee?O+ee?NbRe?O+ee?]3XnOCTm000B0xKObgqDL)<h[n[%%g*P%5FW8V7B6IFd/*wvEslJ(EslIBAEAm-@!V4600Ju50qV.u0=Xhx00-Gi01P5y0dm3[0tDkk6:L0E5FbD.2)ZRm4gAEC0r&Da6a$VW1Qb2w5E]lP1Q22f3jc+o0tu1P0$kLy0%nJ80rL-lfe<ZFknMBn7D:t!jp}Dcjom8KjPc6Ue?O+ee?O+e04](UfAB:UXC#Wh6Ih$DUS#8*i1X9W02kEGi9u[Lbnz1LbnAc{bnyCvbnzvVbnCSP}Hei*EshcuEf5$*EslJ(EslLuEslJ(009610ds((0Ruv109}&k0sPx44fD$B2)ZXq0%e&q2)Z+u1Q1]e0rrue0rrcbmS{Bz7YViKffxm/e?O+ee?O+ee?O:Re?O"
"+ee?O+ef!-S1xT[9H01qCjvx-0{su3)KXD06rZ=kEQgC{%Cbq*XXbnA1.EslJ(EslJ(EslJ(00ic2%apsF0ds?H00-H0kPfUe2TwW%0rrue1Q1]e0rri61Q25m0rAr)B$U=>06-O0jte!$e?O+ee?O+e04](UfAB:UXC#:j2UwYrUS#8*i1X9W00@Rui9wrzh6StZh6WJ$EslJ(EFOu(009610qV.u0E1IC06JNV0r-J<1P!=c0rri40S-r4mS{Bz7YViKffxm/e?O:Rf!-S1xT[9y00C(bvx-0{su3)KXC#^jZ=kEe6^kBnCl8]oEe<lF000000ds&}00ri:kOj211v9a>0%eG60%eG60rtPjfe<ZFffy)I0wk#Se=WQW0zc$n00004nOzLpkVgYz5Jvl21PMm%sURY&byK#*00Ju50007u0=Xhx00-Gi01P5y0dEf*0tDkk6:tr:4?o3S3KNgC0%eG66a$(*4?o3S3KNgC2l>tm0%eG62lURI1{C5e5jQ*@hY8JVjte/Ijom5!e?O+ee?O+ee?O+ee?O+ee?NbRe?O+ee?O+ee?O+ee?]3XnOCTm5c9zoxKObgqDL)<h[n]5%g*P%0w=OThX!ba0w=OTi@}h-12z!V0E5%uEsq86EslJ(EFOwu0E5%uEsq86Esq9KEe<lG000000RuCk00rii0cgs.0t2/c5EP6T3jc.p00AYr4*Syy0%F-e1{C5e1{+UD1Q1]h00S]z4GM>w0$bCv0%O/c1X9U*hY8JV7D:]AkrGIQ7D-ocjom8$@tdNHjPc6U05lednOCTm1PP99xKObgqDL)<h[n]1%g*P%b[Rvl6^BqDh8t$65He2n0Eax*EFOwu}HeD/@!*^TCys]sIg2v8000000qV+p00Ju(kO:w820-y]1Q1]e0rri60rroa0S-x)B$U=>06-O0jte!$e?O+ee?NbUfAB:UXC#Zi21-GpUS#8*i1X9W00-Fsi9wrz5He2DEsp%u0=-R*00Ao40005(0=Xhx00-Gi01/iSkR&AN5<N9l2l>+K4?o3S3KNgC2l>tm0%eG63Ku[r1o[(s5deHC1}IgC01xtr0rJo4mS{Bz7YV=.ffy)IffGgHjp}Dcjom5!e?O+ee?O+ee?O+ee?O+e04](UfAB:UXC#Qf5>M+BUS#8*i1X9W022sEi9u?daVf#<h6StvgC]gRh6B?40E5%u}UHhF0RDwFEfazpEFOwuEsq9KEsh80000000RuCk00ria01e=q022u:kSw=U6Hixp2l>?O5FbD.4gAQK2)Z+u1Q1]e0rrGk1P[(i2[vyK0%]eB5dX<K0@@@C0S-u607Ib+hY8JVjte/IjtdHk@tdNc@tdM%e?O+ee?O+ee?O+ee?O+ee?O+e04](UfAB:UXC#Qf6Ih$DUS#8*i1X9W02kEGi9u?daVf#<h6StvgC]$abnzvpaVfpAEsp$*Esq9K0E1P+Esp%uEslLuEslJ(00ic20005(0=Xhx01/iUkSX#V5<N9m2l>+K3KM$t0T5/n1P[(r4GMUi1pU+H4fDXj1}H[w0S@Dc3>0sy2m}yE3jc.B1oOVkmS{Bz7YV=.krId)fbD+H7D-oc7D.ZKjPc6UfbD+H7D:r505lednOCTm3&*]ixKObgqDL)<h[n]3%g*P%6brqXhXv?XbnzBXbq?nmgC%PeEslJ(}H9+)C6[Ym@!V5KEFOwu00ri30005(0=XhB01e=q0cgsT0t2/c5DSEM4gAQK2)Z+u1Q1]e0rru81{22o1{k2k4H&^F3iQCw3<3Ch0r+(nfe<ZFkrHOYffy)7jPc6U5DaY(e?O+ee?O+ee?O+ee?O+e05lednOCTm1oo08xKObgqDL)<h[n]1%g*P%6eZ{<aRt>P7B6UJ2q)UtEf1M>3&$N2EslLyI2?*GEG27KEslJ(Ee<lG000000RuCk00rim0d^y20tDkk6+pVV5!LP.3jc.r00A&z6atlO2lUbl0T5Yh4gABw4/#+l4H94F5cSgs1PO-t6a$rL2M>qo0S-D]B$U=>06-Ogju>}t@tdNc@tdNc@x79ljPc6UffGgHjoc#^f!-S1xT[9D02e1rvx-0{su3)KXD0uzZ=kEyh6Taxbm#Z?gzIbHgzIXXbnzB7CLV-o}UH5pEFOztDJsd>EslJ(Esh7$000000RuCk00Jue01w{AkR8{C4NpKc4gAQK2)Z+u1Q1]e0rrGm2)gCr0sYCu0%n-d1PO=i0S-u607Ib+hY8JVjte/Ijp}Dcjp}Dcjom5!e?O+ee?O+ee?O+e05lednOCTm000H2xKObgqDL)<h[n[#%g*P%5FVD7gC]g?h6Uf-h6ScCEG27KEslJ(0=-Tp}U=GKEe<lH000000RuCk00ria02>{dkVN[996+IT8w%q95!LoI0sn-e7-9>*6b}F[3>0sC1{kek1p+>F0%]2f1}RN^3Kc+r3i*+J6B+cA1Q>OB85JU=2l+km8xfN}5Fb3T5!t6O0S-P%B$U=>06.nZkrId)5D9zc5D9zc5D9zc5DaY(fbD+H7D:t!jPc6U05lednOCTm2(>@pxKObgqDL)<h[n]d%g*P%0w#8p7D0AT0v6DTh6T$mh8t$66^BCH6^BCr0Eazp}H9^uEFOwuEsp})CZB/lDWt^wEslJ(EslJ(00ic20005(0=Xhx022u!kTLL^6Hixz5!LP.1{C8l0T65B6atlO2lU5j00ASn0@@Jl2l>wk2)7bf2NgRF5cSds1}?NZ0sYCt4?n%H3iykmmS{Bz7YV=pju>})jp}Dcjp}Dcjp}DN5DaY(ffGgHjte?}7D.WLfAB:UXC#{o6Ih$DUS#8*i1X9W02kEGi9u[fgC]m<h6T$6h8v9C6/cBD6^Fe6}UH5pEFOwu}t{hmD&w[JEslJ(@!V4600ic20ds&}0Ruu]00-H0kPfUe2TwW%2l>tm0%eG60rrre1{j]g1owG(B$U=>053C/jte?}jom5!e?O+ee?NbUfAB:UXC#Qf2UwYrUS#8*i1X9W00@Rui9u?Hh6SX?h6WJ$EshcuEG27K00ic20001E0Ruu]00-H0kPfUe2TwW@2l>tm0%eG61Q22i0S-oa1{1-]B$U=>053C/jte?}jom5!e?O+ee?NbUfAB:UXC#Tg2UwYrUS#8*i1X9W00@Rui9u[t6/cNH6/gn^EslIA]Nhe:00Ju50004z0=Xhx00-Gi01P5y0eiV#0tDkk6:kiZ6a$(*4?o3S3KNgC2l>tm0%eG60%e=e4?5:I4H&ZD2NgUz0rSrb1}}KK1oFPfmS{Bz7YV=.ffy)IfbD+H7D-ocjp}DN0wk#Se?O+ee?O+ee?O+ee?O+ee=WHRf!-S1xT[9G02e1rvx-0{su3)KXD0uzZ=kEggC{ys0w#87gC{x[h6S?Lh6S&:EslJ(EFOwu}Hei*0E5%u@!V71EslJ(Ee<lG0001E0E1Op00ria08t=$0se@@2)7to1Q1]e0rrf41{Unk00SPc07Ib+hY8JFjte/IjPdwye?O+ee?O+e05lednOCTm000o@xKObgqDL)<h[n[]%g*P%5FVDD1R&gr0EaK10E1OpEsp%uEe<lJ000000E1P+00ria01e=q022u:kSnYT6Hixx6a$JM6a$(*4?o3S3KNgC2l>tm0%eG62)Z?A4gAQD3i?1J1{k2h01/+z0rJo4mS{Bz7YV=.ffy)IfbD+H7D-ocjom5!e?O+ee?O+ee?O+ee?O+ee?NbRe?O+hfAB:UXC#Qf6Ih$DUS#8*i1X9W02kEGi9u?dgC{fNgC]m<h6Uf-h6ShVh6WJ$EslJ(Esp%uEfazpEslJ(@!V5K@!V4600Ao40005(0=Xhx00-Gi01P6IkRr6G5i{*h2l>XG4gAQK2)Z+u1Q1]e0rrGk1P[(i0sxbq4?nWw4gAps0S-o<B$U=>06-O0jte/IjPdwN@tdM%e?O+ee?O+ee?O+ee?O+e04](UfAB:UXC#Qf5j{LzUS#8*i1X9W01/gCi9u?daVf#<h6StvgC]m<h6ScCEsp$*}H9+[AEAoiEslJ(@!V4600ri30007u0=Xhx00@Su0e:3g0tVCo7yy/*4?n^E1Q2zG2nE{Z3<^4q2lj(m3iHtj2Npzi1{>Ls1o]IJ4?5*J1{k8g1}qmW0$C+v6+R8X3iyktmS{Bz7YV=pju>})jp}Dcjp}DNffGg}0wuOljL-c#7D-n+jPc6U05lednOCTm5c9FqxKObgqDL)<h[n]7%g*P%b@D[Z6cN@zh8vf6bnzvVbm#Z?aR+4IEsq9KEFOwu@!V5K0E60uD&Xx?I$}/2Ig6^MCys(t00ic20005(0=XhF01P6IkS5MN5i{*g4gAEC0%F-e1Q2tC2)ZLn00AVp4?5TF2m.gE0sxbq0%FYb0rSYj0%nJ80szqtfe<ZFknMBDju<SA5DaZAjPdwyffGgHjte?}7D.WLfAB:UXC#%p5j{LzUS#8*i1X9W01/gCi9u[LgzIXTbq*XXbm$8pbnCU6Esp%u0Eawv]Nhe!41iK8].OyV00ri30005(0=Xhx00-Gq0eJ)d0tDkk6:LA:4H&:K2lU8m0r:5E4*97w1P[&b1pLUu0To5D1Q1]u1{+Iv2lLem3k0ps0$UUD4?nTI3<c>A0rAM$B$U=>06-Ogju>}t@tdNc@tdNc@tdNc@x79ljPc6UfbD+H7D.WLfAB:UXD00q6Ih$DUS#8*i1X9W02kEGi9u[Jh6S[Z6/cTJ6^jej6/cNH6/gpm}UH6+Esq9K0E5%uEf1Ot}G&l>0=-TpEe<lH000000E1P+00ria01e^kkQ3jq3{Um43jc}w1Q2hu2)Z+u1Q1]e0rrf43Ku?m0S-D]B$U=>06-O0jte!$e?O+ee?O+ee?O+e04](Re?O+hfAB:UXC#^k3}UbvUS#8*i1X9W01w[yi9untbnz{T5I>jXbnyl6}H9!*}t{kpEG27KEsh7%000000RuCk00rie0a[FE0sPx44g9yF2lU5h00ASn0%ePf1P!Ye1}7+u0@@Jd2)ZOq1{k2i0rAu[B$U=>06-Ogju>}t@tdNc@x79ljPdw}jPc6U05lednOCTm1PO%5xKObgqDL)<h[n[%%g*P%b[Rvl7A<wl6/cBD0Eazp@!QV1EslLBB]EqF0=-U/I2WIR000000E1P+01e^okQuBt3{Um43KNgC2l>tm0%eG61oO&f0r@Yi2mp+B00&/h0szqtfe<ZFkrHOYffx*Ajte?}jom5!e?O+ee?O+ee?NbUfAB:UXC#%p3}UbvUS#8*i1X9W01w[yi9vyZbq?nmbq?nmaRs:0}UH6+@!V5KEslLxJ@PdX000000E1P+00@TdkP*7m3p1$10rrAi2l>tm0%eG60%eVf2l>Fu1Q1(c2s->&hY8JVjte/Ijp}DN0wk#Se?O+ee?O:Re?O>j0zc$n00-GhnOzLpkVgYz5Jvl23JE[3sVnb]bq?nmaRuIU}UH6/IgbkGEsq6P00ri30005(0=Xhx00-Gm0cZW:0t2/c5Dz$q3jc}w1Q1]e4H&Nw3>0gG2MU2c3KNav1{Ltx3<EjL0S-Pk0rSr60r>@ofe<ZFkrId)ffy)I@x7U&7D.ZK7D?TU5DaZze?O+ee?O:Re?O>j0zc$n00JulnOzLpkVgYz5Jvl25DxK9sPHx66!{{Bgx!{VgC{ymgxEdEIg6^MCM9D}EFK0pEsq9KEslJ(009610005(0=XhF0a4]q0sPx44g9mz0rrDk1oP2q2lK@f1{C5g0sGkp0%e=m1P[(d14EC!hY8JF7D:t!jom8KjPc6Ue?O+e04](Re?]3XnOCTm0@@:3xKObgqDL)<h[n[%%g*P%0v6DD5I(p26/cBD0Eax*EFOwu0EazsKnwXOAr2$t000000E1P+01e^kkQcpr3{Um53KNay3KNgC2l>tm0%eG60%e=m1Q1&d00if6mS{Bz7YV=pjte/Ijom5!e?O+ee?O+ee?NbRe?O+hfAB:UXC#Zi3}UbvUS#8*i1X9W01w[yi9u[Jh6AX<5He2lh6.%C@!V5K0E5})]Nhe!Ig6^L00ri30005(0=Xhz00@Sm0b1LC0s/P84*isG2)Z+u1oO(k1Q1&a4gAsv0Te@s2)gCp01okp0rJoamS{Bz7YV=.ffy)I0wtlK@tdNGe?O+ee=WHRe?O+ee?O>j0zc$n00SAknOzLpkVgYz5Jvl24/:s7sRzJa5I>116/dY/0w=RfEsq86EFK1+EG267].%X}EslJ(00Ju50005(0=XhB01e=q022tG0fxDo0t(Us83}5[4?or*3jc+m6a$/:1{Cek02Dy.5dPcS2m7Fr1oO-a6a+DM5!LoH1o]qz2lLer3ipke7zD@N0TPbl0rCVkfe<ZFkrHOnjtdHkfbD+H7D-ocjom8KjPc6U5D9zce?O+ee?NbRe?O+hfAB:UXC#Tg7!FyHUS#8*i1X9W02U:Ki9u[t5I(p2h6U4oh6StZh6S+(bnz1LbnCSPEslLuEslJ(EslLuEfaA!KnsoMEFOx>Ig6/2Esh7@000000E1P+00Ju>kOTq720-y[0S-Dc0%eG61oO"
"/c0r+(nfe<ZFffy)I0wk#Se?O:Re?O>j0zc$n00AoanOzLpkVgYz5Jvl22lhE#sS]TE5Hd>*CLV-o0RDtK00ri30007u0=XhB01e=u0e9Q10tDkk6+H@^4gAQK2)Z+u1PPeE0%eSe0rSr81}7Xw0%O(m4H&:E2)p[M4*9jN5=@do5Fb6E0%5D)B$U=>052cJfbD.KfbD+H7D-oc7D-oc7D:r5e?NbRe?O+ee?O+ee?O+hfAB:UXC#Wh6Ih$DUS#8*i1X9W02kEGi9u[Lbm#ZzgC][6h6U486/cBT5HhZLEslJ(}H9^uEf5$*@F{#6EFOwuEG268Ar2$u000000RuCk00-Gm0c7mU0t2/c5DSsE2)Z+u1PP2w0%eSe0rSr81{$Uw0%O(s3>0dy4gAvw0$L>y0%5AimS{Bz7YVi9jtdHk0wtlK@tdNc@tdNGe?O:Re?O+ee?O+ef!-S1xT[9L01..nvx-0{su3)KXD0ivZ=kEy6/cBD6^BCvgC{ymbm#ZjCys)/EslJ(EFS^wAR+9iEsq6P00ic20005(0=Xhx01P6IkSeSO5i{*n4H&:K0r&Da3>0NK2lUbj0T5Yh0S-Gh4gAyy0%FVe4I63I2)ZRv3i{Uq0S-D]B$U=>06.nZkrId)5D9zc5DaY(ffGgHjte?}7D.WLfAB:UXC#^k5j{LzUS#8*i1X9W01/gCi9vKtbm$Un5I(p26/cBD6^A:6}H9!*Esp%v0D[y10rEy)EG27K00ic20005(0=XhD01w{AkRJiG4NpK94gipw1Q28o3>0vA0%FVc0S-Vh0S@=l1Q2ny1{+Iv0%Y2h0rrf42s->&hY8JVju>})jp}DNffGgHjom8$@tdNHjPc6U05lednOCTm2lkl9xKObgqDL)<h[n[#%g*P%0w+Dn6^BCr5G}ljh6ScCEsp%u0Eazp}t{l+Eshfp@!*EJ00ic2%apvA0=Xhx022uYkT2h.6Hixv5!LP.3jc.r00A&z3<c>J2NpFm1P!/h1P!Ye2N}gC1oO/c1}IgQ5ebVS2)ZRv3i{Uq0S-D]B$U=>06.nZkrId)ffy)I@tdNc@x79ljPc6UfbD+H7D.WLfAB:UXC#^k6Ih$DUS#8*i1X9W02kEGi9vKtbm$Un5I(p26/cBD6/cNHh6ScC}H9!*EslJ(}HeD/@!*^TCys]sIg6/2009610005(0=XhL0d3)(0tl2g695NO3>0sC0Tw-e5E]lL0162n0%nVg1Rh7J0$2kA5cA1o2)Z.t4gApy1{k2i0rSr50r>@ofe<ZFknMBDju>}t@x8.HjPc6U0wuOl7D:t!jPc6U05lednOCTm1PPfbxKObgqDL)<h[n]3%g*P%aTn&?6^Bql6^BCHh8t%(6/b).EFOu)CL.e:EslJ(0=-R:}tbHt00Ju50005(0=Xhx00-Gi01P5y0dWr[0tDkk6:tr:5FbD.4gAQK2)Z+u1Q1]e0rrDk0rSra0%e@w3J*Us6a+VR3<M[F5E5yu0rtPjfe<ZFkrHOYffy)7jPc6UffGgHjom5!e?O+ee?O+ee?O+ee?O+ee?NbUfAB:UXC#Qf6Ih$DUS#8*i1X9W02kEGi9u?Hh6U3{aVfuVbnywtbnzvpaVjW@Ef1Lu}t>!*@!QV10E5%uEsq9KEslJ(EslIz009610004z0=XhB08t=#0se@@2)gzp1Q1]e0rroa0rrue0%nPa0r>@ofe<ZFknMBnjte!$e?O+ee?O+e05lednOCTm1PO*1xKObgqDL)<h[n[]%g*P%i@]{Hh6AX<EslLyIg2A%Esh7$000000RuCk00ria01e^kkQuBt3{Um53Ku$w0rrrc0S-Vo0%nJb2)Z+r1o!]g0%e/o0r>@ofe<ZFffy)7jom8KjPc6Ue?O+e04](Re?O+hfAB:UXC#^k3}UbvUS#8*i1X9W01w[yi9vK-5I0[{gCGR2h34]HEFOwu}H5zp@Tsj600Ao40005(0=Xhx00-Gi01P6HkRJiI5i{*m4?n{M2l>tm0rrGm0@@Sk1{k2j0s/Lz0%w-q3<3Ii3Ku?p0U2Ls07Ib+hY8JFjtdHk0wtlK@tdNc@tdNGe?O:Re?O+ee?O+ee?O>j0zc$n0000gnOzLpkVgYz5Jvl25DxK9sV4#Ebnh<<br3t[h32G<byL1pEslJ(@!V5KEf60t4eP?%}U=F600Ju50007u0=Xhx00-Gi01P5y0e9Q10tDkk6+Rb>4?n%O2l>tm0%fbG0S-Vm0%nJa1RzpQ0$two4?5WB1oY5r1{k2x5ch>k4gAKB2MT@}B$U=>052cJfbD.KfbD+H7D-oc7D-oc7D:r5e?NbRe?O+ee?O+ee?O+hfAB:UXC#Tg6Ih$DUS#8*i1X9W02kEGi9u[t5I>7B5I<UFh6S[[h6S+Dgzb4-EFOwu}H9!*EslJ(0=!hF0EaK1EslJ(0RDv10Ruu}000000RuCk00ria01e=q022uRkSF&V6Hixo5!LP.2lU8e6a+DQ1{Cel4?5*M0%FVr5!blS1QCqt2)QUp1P!&g0rSrl5dX<M0rCVkfe<ZFfbD-77D:t!jPc6U0wtlK@tdNGe?O+ee?O:Rf!-S1xT[9z02e1rvx-0{su3)KXD0uzZ=kEgbnA0XbnA0th6S?<bnAowh6WJ$Ef1LuEsp%uEFOwu0=!8k}H9^u}Uyf]000000RuCk00rie01P6BkRAcH5i{*l1Q1]e3jc+m4?n%M2l+kl4gABA0%FVf1{LOD2)gCv3ipke4H&BtmS{Bz7YViKfbD-77D?TU0wtlK@tdNGe?O:Re?O+ef!-S1xT[9z01..nvx-0{su3)KXD0ivZ=kEgbnAovbnA0Hgz7&l6/dZ}Esp$*}H9!*}t$WkEshd*@Tsj600ri30004z0=Xhz00@So0cgsT0t2/c5DAsK4gAQK2)Z+u1Q1]e0rrue1P[(i0UbUC0%/8t3JQkr3iHth0rCVkfe<ZFkrHOYffy)I@tdNc@tdM%e?O+ee?O+ee?O+ee?O+e05lednOCTm0rrW5xKObgqDL)<h[n]1%g*P%0u&gl6/duv6/cT-bq*XX0EaK1}H9^uEsp%u0=-TpEFOwu}tZG@000000qV+p00-H0kPfUe2TwW$1oO(k1Q1]e0rru80S-Ge0S-o<B$U=>06-O0jte!$e?O+ee?O+e04](UfAB:UXC#Qf2UwYrUS#8*i1X9W00@Rui9va1gC{x?gD22SEfaA!K9}4MEe<lJ000000=XoY00ria01e=q022u?kTCF=6HixA1{Cek0S@Da2l>ev5EOWD1qqsS3<)7y1p+>y0@@=s3J*.u4?nQx022$K0%/8x5Fa%F1oO-c0S-G{B$U=>06-Ogju>}t@tdNH7D?Tjjom8KjPc6U?8$SLjp{sh@tdM@e?O>j0zc$n00SAqnOzLpkVgYz5Jvl26-U%dsRA2qgBA[Zh31{VgB&F-gzp#<gX#JFEshd*EslJ(EFOwu0E5%uEsq86Esq9KEe<lF000000qV+p00-H0kPfUe2TwW@1{Cko1Q1]e0rrf42lj(e0S-u(B$U=>06-O0jte!$e?O+ee?O+e04](UfAB:UXC#Wh2UwYrUS#8*i1X9W00@Rui9w3HbnA0DbnH7KEshfpEG27K00ri30E1M*0=Xhx00-Gi0ad#r0sPx44g9yH2l>Iw2lUzu0%nPc1{C2f00ric2N7qk0S-u61v^L/hY8JVjtdHkffxm*7D?Tjjt5lse=WHRe?O>j0zc$n00AognOzLpkVgYz5Jvl24faa5sRzJah3U]{bnSc[byL1pEsp%uEfazpEFOwuEe<lF000000Ruwu00ri:kOj211v9a(0%eG60@@x60rtPjfe<ZFffy)I0wk#Se=WQW0zc$n00004nOzLpkVgYz5Jvl21PMm%sV3{qh39t$Esh7%0004z0qV+p00ria08t=$0se@@2)yLr2l>tm0%eG60@@Gc0S-u607Ib+hY8JVjte/Ijom5!e?O+ee?NbRe?]3XnOCTm000o@xKObgqDL)<h[n[]%g*P%5FW8V6/dY(EslIBAEAoiEe<lG000000RuA+00rie0a!zD0sPx44f=>m1p2bq2N7tk1POVk3ju$s0%eVg3jc.n00SVl2lt2j0S-S$B$U=>06-Ogjp}Dcjp}Dcjom8$@tdNHjPdwxf!-S1xT[9I01qCjvx-0{su3)KXD06rZ=kEQgC{yu0u&3#h6T#b}Uykp@!V5L{Yk*x41m%L00ic20005(0=XhD01w{CkR-uI4NpK94givy2lUkm3>0vA0%FVc0S-Vh0S@=l1pUwi0$kwt2)ZOq1{k2i0rAu[B$U=>06-Ogju>}t@tdNc@tdM%ffGgHjte?}7D.WLfAB:UXC#^k4OptxUS#8*i1X9W01P4Ai9unr6^BCp5I>dT12z!V0E5%u@!V71}UH3?CLVZ?AEE?z{Y7o@000000E1Op00ria01e^kkQlvs3{Um43KNgC2l>tm0%eG63jc}t1{Uqm0%nJ80sGel1X9U*hY8JFjte/I7D?Tjjom5!e?O+ee?O+ee?NbUfAB:UXC#^k3}UbvUS#8*i1X9W01w[yi9untbnz{T6/cT-bnyl6}H9!*}t>!*EFOwuEe<lG000000RuCk00rig0b+4S0s/P84*SyA0S@Da2)Z[C1Q1]h00A-t4fM+n0Tn/p3JQtf1{>Ry0se]j1{C5e3}ty[hY8JVju>})jp}Dcjp}Dcjom8$@tdNHjPc6U05lednOCTm3&*=exKObgqDL)<h[n[#%g*P%h-0rojPNt@gz7=/bq<]C}UCTtIg6<F0EYy{Ka3<800ic20005(0Ruu]01/iZkTbnZ5<N9s4gAyu5FbrL3iZFw4GMUi1Qkns1Q1]h0T5-j4G=Xk4f^jG2M+2g2m}yI0%F(i3jc+C1owJcmS{Bz7YV=.krGIQ7D:t!jPc6U0wuOl7D-oc7D-oc7D-ocjod8&0zc$n00SAonOzLpkVgYz5Jvl2692:bsVnh.9rw2xh8v9C6/cNH6/cQ3EslLuEFOx?DWCTr}H9^uEsh7%000000E1P+00rii0c7mQ0t2/c5DAsI3Ku$u4?o3S3KNgC2l>tm0%eG64H&QA1Qk2e0162m0S&xbmS{Bz7YV=.ffy)I5D9zc0wk#Se?O+ee?O+ee?O+ee=WHRe?O+ef!-S1xT[9E01..nvx-0{su3)KXD0ivZ=kD$bnA0V6^BCH6/dZ6h6T%VCys]oEslJ(EfazpEsp$*Esh7%009360RuA+00rik0dv9}0tl2g6aCG-1{C5i0r-(w5EY3M2lUbl00ASn3KM$r0To5t2MU2c1{LOF1P[(i5d]DN4gApy1{1-hmS{Bz7YV=.krId)5D9zc5D9zc5D9zc0wuOl7D:uk@tdM@f!-S1xT[9H01}>pvx-0{su3)KXD0oxZ=kEQgC{x?gC{ymbq?nmaRt}lbyPB+}UCR*0E5$*E!-)KEsp%u@F#vb00ic20005(0=XhB01e^kkQuBt3{Umd3KNay1PO(o0%e&o2lLel1Q1&d00ifc2M$ki0rL-lfe<ZFfbD-7jom8KjPc6Ue?O+e04](Re?O+hfAB:UXC#Wh3}UbvUS#8*i1X9W01w[yi9u[Lbm#ZzgC]+25HhZLEFOwu0Ry*sJ@:DM@Tn*K00ic20001E0=Xhx00-H3kPx!g2TwW@2l>tm0%eG61Q22h0rrue0%eJ80y?k=hY8JVjte/Ijp}DN0wk#Se?O+ee=WQW0zc$n00969nOzLpkVgYz5Jvl22(?X1sRA1{gzIbDgKTX1EshfpEsq9K00ri3@@@G10=Xhx00-Gi0awbr0sPx44fcXu2l>Ly2)Z+u1Q1]e0rroa1{a/93KM$q0S-P%B$U=>06-O0jte/d@x79ke?O+ee?O+ee?O:Re?O+ef!-S1xT[9H01qCjvx-0{su3)KXD06rZ=kEgbnA0Hbm#ZZ0tNA!EslJ(EslIBAEAom41e9T000000RuCk00@Sm01/iQkSeSQ5<N9q3KM$q4?o3Q1{COI3>0vB00JDa0r-Vk1oO-n3&{Cg2m}yL0%/5k5E]3F1{1(%B$U=>052cJffy)7jPc6U0wtlK@x7U&7D:r5e?O+e04](Re?]3XnOCTm1PPfbxKObgqDL)<h[n]3%g*P%0v5s75Hd(zh8t)<6/cBVbnyl6EslJ(}H9^uEf1Lu0EaH6Ef1Lu@!V5K009610001E0=Xhx06JNV0r-J<1POSa0rrc20%eG7mS{Bz7YViKffxm/e?O:Rf!-S1xT[9z00C(bvx-0{su3)KXC#^jZ=kEgbm#.WEshfp@F{{W000000=Xnk00@Sm0a!zA0s/P84*@*P2NpFm3>07o1Q1@g0Tw]j4giyz2NHFs3i*Un1PXS7mS{Bz7YVi9jte!$fbD+Hjte?}jt5lse=WHRe?O+ef!-S1xT[9y01IOlvx-0{su3)KXD0ctZ=kEm0w+rz5I>1Ph4.RRbyK#*EslIA].]m1CM9B1CyxD00E1IB000000RuA+00ric09huc0sxf03JQzl0%e-k0S-Ge0%nJb2)pFq2)ZLl009r"
"]B$U=>052cJffxm*jPc6Ue?O+e04](Re?]3XnOCTm2lk95xKObgqDL)<h[n[}%g*P%aU%YX0w=OR5I<DiEsp$*@!V47CyxD000ic20005(0Ruu]01P6FkS5MN5i{*n4H&:K0r&Da3>0NK2lUbj0T5Yh3KNay0TePe2N}gC1oO/c1}zdN0so2k0r>@ofe<ZFkrId)5D9zc5D9zc0wuOl7D:uk@tdM@f!-S1xT[9D01..nvx-0{su3)KXD0ivZ=kEyh6Taxbq?nmgzIXXbnzB7CLV-o}H9!*E!-Ws}H9!*Ee<lG009360RuA+00@Sm0b+4S0s/P84/)aD1{k2i2Np+C2)ptl00Au73J*Cj3i*Rx0rrig1{+Iv0Tw]f1Q1&b2s->&hY8JVju>})jp}Dcjp}Dcjom8$@tdNHjPc6U05lednOCTm2lkl9xKObgqDL)<h[n[#%g*P%0w+Dn6^BCr5G}ljh6ScCEsp%u0Eazp}t>*pEFK3k@!*EJ00ic2@@@I@0Ruu]01w{CkR-uI4NpKe2)ZOm0r-Si3KN4u0%FVc4gAWG0%nPe1Q>+t0@@Ji3jc.s2lj@i0S-Z0B$U=>06-Ogju>}t@tdNc@tdM%ffGgHjte?}7D.WLfAB:UXD03r4OptxUS#8*i1X9W01P4Ai9wf@h6Ul+aRt}Rbq*Y6EFS^vCyxsoE!S}(@sX%P}tZG%000000E1Op00rie0a4]p0sPx44fcRq1Q1]e3jc!m1{C5g0r-=o2lL8n2MU2c3KM[vmS{Bz7YViKfbD.KfbD+H7D:r5e?NbRe?O+ee?]3XnOCTm2(>x9xKObgqDL)<h[n[%%g*P%0v5sn2qXjZh6SX?0E5%u}H9^uEf5$*EslLy3&*ne000000RuA+00-Gm0c7mS0t2/c5E5:Q4gAQK2)Z+u1Q1]e0rrrc0S@Dc0$C+y0%n-r3>0dy2lk8q0rL-lfe<ZFffy)IfbD+H7D-oc7D.ZJe?O+ee?O+ee?O+ee?O:Rf!-S1xT[9A01..nvx-0{su3)KXD0ivZ=kEg6^BCHh8v8<6^kB<h6S&:EFS*t3&$N2}t{kpEslLyIg6/2Ee<lG000000E1P+01e=q0b+4M0t2/c5Ew*L0%e#y4gAQK2)Z+u1Q1]e0rrAi1P[(j4H&TB1{1Ym3&{D1B$U=>053C/jtdHk@tdM%e?O+ee?O+ee?O+ee?O+e04](Re?]3XnOCTm0@@)7xKObgqDL)<h[n]1%g*P%6c!kp6/cNXh8v9m6e.0H}H9^uEslJ(EfazpEFS!*@Tsj600ic20005(0Ruu}00@TakP*7m3p1$32)ZLi2NpFm1POYj1{a(c2l>tl1oOYa2s->&hY8JFjtdHk0wtlK@x8X+e?NbRe?O+hfAB:UXC#<m3q1]tUS#8*i1X9W01e+wi9vKL6^jet0UEi?CLRt:EFOwuEshd*@!QQT000000RuA+00ric01w]u0cpyX0tl2g69eWR4gAQK2Npwg2l>kg5dYiL3iQFu3J*Fh1RzjJ2N7wm1{C8h0Uk+w21A+?hY8JFjtdHk0wtlK@tdNc@tdNGe?O:Re?O+ee?O+ee?O>j0zc$n00SAonOzLpkVgYz5Jvl2692:bsTt<Ybl{7J5I(oXgC{xZgC{xTCys]oEslJ(EslJ(}UH6+@!V5KEFOu(00Ju50007u0Ruu]00-Gi01P5y0d)D#0tDkk6:kl-4?n%O1Q1]e1{C5e4?5Kx00APv5Eeyv1pbUL3J*Uq4H&:E2)*>x1{1(c5FbxN3&{w#B$U=>052cJfbD.KfbD+H7D-oc7D-oc7D:r5e?NbRe?O+ee?O+ee?]3XnOCTm0rr*9xKObgqDL)<h[n]5%g*P%6c!kFaU$#a0U)G@gC{x[gC{z9EshcuEslJ(EFOwu}H9+)].OBX}UH6+00Ju50004z0Ruu]00-Gi01P5y0cZW*0tDkk6:b9W4H&TC0rr=C3Ku}u1Q2tA3jc!r0TPbq1{a@e2ndQT3>0jC1Q1]f022[O0$CtimS{Bz7YVi9jtdHkfbD+H7D.ZK7D?Tjjt5lse?O+ee=WQW0zc$n0000knOzLpkVgYz5Jvl26-U%dsRhx8h8uD(gB&FvgB&Fvi@}h-6[b}bEslJ(EslLuEf5$*Esp%uEsh7$009360RuA+00rie01P6BkRAcH5i{*i3jc!o2l>kg4gAQK0%FVd4?n)K1Qb2p1o]wB1{k2m2ls]b4H&BtmS{Bz7YViKfbD-77D?TU0wtlK@tdNGe?O:Re?O+ef!-S1xT[9z01..nvx-0{su3)KXD0ivZ=kEgbm#ZF6/cBD5Hep$gC%PeEFOwu}Uyl+@!V5K0=!i@EslJ(00ic20005(0Ruu%01P6HkRAcH5i{*i4?o3S3KNgC2l>tm0%eG64gAKB2Ngww4G(>o1{Lkj0%5Sb3KM[mmS{Bz7YViKffy)7jPc6U5D9zc0wk#Se?O+ee?O+ee?O+ee=WQW0zc$n0000gnOzLpkVgYz5Jvl25DxK9sRhx86/dZ6h31{paVgG4bL(?p@!V47].J$p]NhdoEG27KEsh7$000000E1P+00Jue01P6IkRAcH5i{*g4?o3S3KNgC2l>tm0%eG64gAKE2)ZOm0r%5A2lLel01fbp0%eGcmS{Bz7YV=.ffy)IfbD+H7D-ocjom5!e?O+ee?O+ee?O+ee?NbUfAB:UXC#*l5j{LzUS#8*i1X9W01/gCi9u[fgC{%C0w=OT6eIG<h6WJ$EG27KEsp%uEfaA+Esq9KEG27KEe<lF000000ds]*00ri:kOj211v9a>0%eG60%eG60rtPjfe<ZFffy)I0wk#Se=WQW0zc$n00004nOzLpkVgYz5Jvl21PMm%sURYAbyK$wAr2$v000000E1P+00Jug01P6IkRAcH5i{*p4?o3S3KNgC2l>tm0%eG62l>Fw4?n:B1o]qz3J*.p00@]k0S&x6mS{Bz7YV=.ffy)IfbD+H7D-ocjom5!e?O+ee?O+ee?O+ee?NbUfAB:UXC#Tg5j{LzUS#8*i1X9W01/gCi9u[t6/cNX2o%%th6Uf-h6.$]EslJ(}H9^uEfazpEFOx*Ee<lI0004z0RuCk00ria01e=q0cgsX0t2/c5Ew%T2l>Iw2lUIC3J*Uq0UkXw0%ePf1PXY93KM$r01xtu0%w]o0S@D80r+(nfe<ZFknMBn7D:t!jom8KjPc6U5D9zce?O+e04](Re?]3XnOCTm1oo08xKObgqDL)<h[n]1%g*P%6brqX2qXjZaU$#22qXjZEsp%uEf1LuEslJ(0=-Tp}H9!*Ee<lE000000001D0001D0001D0001D0002!0004a000770009F000b/000eY000fZ000hv000j1000ln000m*000oc000pd000qa000sC000uA000vN000x+000A8000Ci000Em000GE000Iu000L9000Nz000O.000Q4000SA000T^000Wa000Yc000.S000:Q000=#000!S000*A000&6000<L000(R000[l000]>000}Y000$r000#E0011!0013:0015.0017Q0019*001b)001el001fK001he001jg001lQ001n=001pW001rW001t0001vC001w-001yz001zy001AZ001CB001Et001F#001H&001J#001L]001O5001PY001R6001S-001U(001V)001XW001Ze001-6001:$001=?001!F001*D001&v001<&001(}001]l001}x001%p001#l0021n0022m0024k0026m0026m0RuvR000000Ruu(0Ruu(0qVV}0Ruu(0}33*0=Xhu0}33*0}33*0qVV}0E1Iz0E1Iz0Ruu(0Ruu(0E1Iz0Ruu(0qVV}0}33*0=Xhu0E1Iz0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0qVV}0qVV}0Ruu(0Ruu(0Ruu(0=Xhu0}33*0=Xhu0=Xhu0Ruu(0=Xhu0Ruu(0Ruu(0=Xhu0=Xhu0Ruu(0=Xhu0=Xhu0Ruu(0}33*0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0=Xhu0Ruu(0=Xhu0=Xhu0}33*0=Xhu0=Xhu0Ruu(0E1Iz0}33*0E1Iz0Ruu(0=Xhu0E1Iz0=Xhu0=Xhu0Ruu(0=Xhu0=Xhu0Ruu(0=Xhu0=Xhu0qVV}0E1Iz0=Xhu0qVV}0}33*0=Xhu0=Xhu0=Xhu0=Xhu0Ruu(0=Xhu0Ruu(0=Xhu0=Xhu0}33*0Ruu(0=Xhu0=Xhu0Ruu(0qVV}0Ruu(0=Xhu0Ruu(00ic200000%9B.W00000000000000000000000000aM0e0096300ri700Jub00-Gf00@Sj01e=n01w]r01P5v01/hz022tD02kFH02CRL02U+P02>[T0384X03qg-03Is^03.E?03}Q(04d:{04v)$04O4204!g6051sa05jEe05BQi05T:m05<)q0673u06pfy06HrC06ZDG06{PK07c-O07u(S07N2W07^e.080q=08iC*08AO>08S.]08&>%0962109oe509Gq909YCd09]Oh0ab.l0at(e00003000000003B009610000s00ri40003B000370003100000000340000200000000000000000001000971Qkhq2(>tw4gS)W5Ft->6=4P585:Cl9uDpBaTecRb{<#/djM&0eInXgf!$Kwh8WxMixxk:jW87}k%+MtmmG^rnLhSHo?[FXqbQs(rArg6sZ23mu0Z(Cve{oc0000000000000000000000000000000000000000000000000a00000000000000000000000000000^0000000000000000000000000000000000000000000000000000000000000000000000003Ed0000000Aq]0000800&Ma0001F0h7CC%nJ6003zomaq1OM000000000100&Oy0l1{q00ri700Jub00-Gf00@Sj01e=n01w]r01P5v01/hz022tD02kFH02CRL02U+P02>[T0384X03qg-03Is^03.E?03}Q(04d:{04v)$04O4204!g6051sa05jEe05BQi05T:m05<)q0673u06pfy06HrC06ZDG06{PK07c-O07u(S07N2W07^e.080q=08iC*08AO>08S.]08&>%0962109oe509Gq909YCd09]Oh0ab.l0at>p01Ybg0000g0001j2)QRl1Qa@h2l>zr0S@Jd1P[]f2l>nm1{Lno1{Lnk0Te-h1{Utp1Qkbl1{Lkn1{Cko1{Lno1{Chn2l>wo0%X]i1{k5l1Qkem1{Lbh1{b2l1{Lnn1{Chn2l+qo1P!&i1POJ53j3!n1Qa#i2NgLu0%w-g1Q1#h2Ngzo1{Lno1{Lnl0%F&i1{+zq1Qkbl1{Lkn1{Cnp1{Lno1{Chn2NgFp1pbbl1{tbm1Qkem1{Lej1{kbn1{Lnn1{Chn2N7zp1P[]j1POJ53Ku}p1{Lel2NpRv0%w-h1{tbj2NpFq2l$Fs2l$Fo0%O@l2m7Lt1{Utp2l$Cr2l>Cs2l$Fs2l>zr2NpOs1pbbm2lUnp1{Uwq2l$tl2lLkp2l$Fr2l>zr2NgIs1{k5m1][S63<^ar1{Lel2)Z?z0%F&j1{Chk2)ZXu2NyXw2NyXr0%O@l2NH+x1{+zq2NyRu2NgOv2NyXw2NgLu2)Z!v1QLtp2N7Ft1{+Fs2NyIo2M>ws2NyXu2NgLu2)HUv1{k5n1][S64gisu2l$to3j3$C0%F&k2l+tm3j3!v2NyXw2NyXr0%Y5o2NQ?y2m7Lt2NyUv2NpXx2NyXw2NpRv3j3[x1QUzr2N7Ft2m7Ou2NyIo2M>zt2NyXv2NpRv3i{?x2lLhp2lj-74HSHw2l$wp3KEgG1o!@l2l+tn3KD$y2)*[A2)*[v1p2ep2[44C2mgRu2)*?y2)Q?A2)*[A2)Q+y3KEaA1Q+Fs2)yRw2mgXw2)*.r2)pRx2)*[y2)Q+y3Kl$A2lUnr2lj-74*@Ty2NyLs3KEgG1o]5o2NgLq3KE1z2)*[A2)*[v1pbns2[44C2NH+x2)*>z2)Z[B2)*[A2)Z?z3KEaB1}7Rv2)HXx2NH!y2)*.s2)pRx2)*[z2)Z?z3Kv4B2M$zt2MK&85dw*A2NyLs3<)yK1o]5o2NgLq3<)gC3jmaE3jmay1pbns3jEmG2NQ?y3jm4C3j44E3jmaE3j3$C3<)sE1}gXw3i*?A2NQ[A3jl[u3iQ+A3jmaC3j3$C3<WgE2M$zu2MK&85E/3D2)*.v4grQO1o]5p2)HXs4grvF3KWsI3KWsB1pkwv3K)EK2[44C3KWmG3KEmI3KWsI3KEgG4grKI1}p+y3Kc$D2[4aE3KW7w3J{[D3KWsG3KEgG4g9yI2)pLx2(<@95!kiF2)*+w4HS:R1Qtns2)Q+u4HSHH3KWsI3KWsC1QLFw3L0KL2[44C3KWmG3KEpJ3KWsI3KEgG4HSTJ2mZ$B3Km4E2[4aE3KWay3K41F3KWsG3"
"KEgG4HAHJ2)yRy2(<@96aLuH3jl}z4H-*S1Qtnt3i{[w4H-NJ3>9KM3>9KF1QUOz3>rWO3jEmG3>9EK3<)EM3>9KM3<)yK4H-:M2mZ$C3<NgH3jEsI3>9pA3<vaH3>9KK3<)yK4HJQM3iZ+B3ig5a6B$JJ3jl}z4?f3W1QCwv3j3$x4?e^N4gJ:Q4gJ:I1QUOz4g-)S3jNsH4gJTN4giQP4gJ:Q4giKN4?e%P2OdgF4g0yL3jNBK4gJED4fWmK4gJ:N4giKN4*&:P3iZ+C3ig5a6+y-M3KWaC5dGfZ1QCww3Kvaz5dF)O4gJ:Q4gJ:I1Q+XC4g&%T3K)EK4gJWO4grZR4gJ:Q4grQO5dG6R2OmmH4g0yL3K)KM4gJED4fWpL4gJ:O4grQO5dn{R3K3[E3JHeb77*]O3KWdD5E]x+1{+Fx3KvaA5E]6R4H@%U4H@%M1}7!D4IofX3L0KL4H@<R4HS<U4H@%U4HS:R5E]oU2OvsI4HrKO3L0TO4H@WG4H9HP4H@%R4HS:R5EP6U3Kc$G3JHeb7zd5Q3>9sG5E]x+1{>OA3<^sD5E]9S4H@%U4H@%M1}g[G4IofX3>rWO4H@)S4H-{V4H@%U4H-*S5E]oV2[WEL4HAQP3>r:Q4H@WH4H9HP4H@%S4H-*S5EYcV3<EaI3&*nc7.NkS3>9sG5!tP/1{>OA3<^sD5!toV4?xfY4?xfP1}g[G4?Yx-3>A:P4?x6V4?66Y4?xfY4?5%V5!tGY2[^KM4*-:S3>A<S4?w<J4*ATS4?xfV4?5%V5!2oY3<EaJ3&*nc0006E0GWo+009el0/u%T0fmh]0/u%T0Mt3]0rSr4000c40000000000000000000000000002r3BAmBE03Ax}0}c6)0008*0qVV}0096100961qU)<AuK]o(000b+00000X#6-J00000X#6-J009360=Xqf0000300ic300000009610}c6)0008*0000.0=Xhv0000000000000000001e009610aM0K00Ju50000200&M&015Ya0ak/800962";
bool Matrix::GlyphData::operator<(const GlyphData& other) const
{
    return intensity > other.intensity;
}
Matrix::Matrix(sf::RenderWindow& rw)
    :renderWindow(rw)
    ,theGrid()
    ,glyphCodex()
    ,glyphSprite()
    ,codexColumns(0)
    ,random()
    ,orderedGlyphIndices()
    ,orderedGlyphIndicesTex()
    ,minIntensity(0)
    ,maxIntensity(-1)
    ,m_shader()
    ,minMaxIntensity()
    ,minMaxShader()
    ,matrixIntensity()
    ,intensityShader()
    ,digitalRaindrops()
    ,digitalRaindropsPrev()
    ,digitalRaindropsShader()
    ,matrixRain()
    ,matrixRainPrev()
    ,matrixRainShader()
    ,texNoise()
    ,font()
    ,txtDisplay("", font)
    ,clock()
    ,avgFPS(60)
    ,seconds(0)
    ,debugMode(0)
    ,showFPS(false)
    ,m_appFocus(true)
{
    random.seed();
	//const size_t fontDataSize = Z85::decodedFileSize(24366);
	const size_t fontDataSize = Z85::decodedFileSize(strlen(FONT_DATA));
	char* fontData = new char[fontDataSize];
	Z85::decode((char*)FONT_DATA, fontData);
    //if(!font.loadFromFile("assets/Tiny_04b.ttf"))
    if(!font.loadFromMemory(fontData, fontDataSize))
    {
		std::cerr << "ERROR: failed to load font!\n";
        assert(false);
    }
	//const size_t glyphDataSize = Z85::decodedFileSize()
    //if(!glyphCodex.loadFromFile("assets/ascii.png"))
    if(!glyphCodex.loadFromFile("assets/ascii.png") &&
	   !glyphCodex.loadFromFile("../../assets/ascii.png"))
    {
		std::cerr << "ERROR: failed to load ascii.png!\n";
        assert(false);
    }
    glyphSprite.setTexture(glyphCodex);
    glyphSprite.setTextureRect(sf::IntRect(0,0,CHARACTER_W,CHARACTER_H));
    codexColumns = glyphCodex.getSize().x/CHARACTER_W;
    unsigned codexRows = glyphCodex.getSize().y/CHARACTER_H;
    sf::Image glyphCodexData = glyphCodex.copyToImage();
    std::priority_queue<GlyphData> gDataQ;
    for(unsigned r = 0; r < codexRows; r++)
    {
        for(unsigned c = 0; c < codexColumns; c++)
        {
            unsigned long long glyphIntensity = 0;
            for(unsigned px = c*CHARACTER_W; px < c*CHARACTER_W + CHARACTER_W; px++)
            {
                for(unsigned py = r*CHARACTER_H; py < r*CHARACTER_H + CHARACTER_H; py++)
                {
                    sf::Color glyphPixel = glyphCodexData.getPixel(px, py);
                    unsigned pixelIntensity = (unsigned(glyphPixel.r) + unsigned(glyphPixel.g) + unsigned(glyphPixel.b))*unsigned(glyphPixel.a);
                    glyphIntensity += pixelIntensity;
                }
            }
            uint8_t glyphIndex = r*codexColumns + c;
            gDataQ.push({glyphIndex, glyphIntensity});
        }
    }
    // Initialize orderedGlyphIndicesTex //
    sf::Image orderedGlyphIndicesImage;
    orderedGlyphIndicesImage.create(16,16, sf::Color(0,0,0,0));
    while(!gDataQ.empty())
    {
        const GlyphData& leastIntense = gDataQ.top();
        unsigned texRow = orderedGlyphIndices.size()/16;
        unsigned texCol = orderedGlyphIndices.size()%16;
        orderedGlyphIndicesImage.setPixel(texCol, texRow, sf::Color(leastIntense.index,0,0,0));
        orderedGlyphIndices.push_back(leastIntense.index);
        gDataQ.pop();
    }
    orderedGlyphIndicesTex.loadFromImage(orderedGlyphIndicesImage);
    // Generate texNoise //
    texNoise.create(32, 32);
    texNoise.setRepeated(true);
    for(unsigned x = 0; x < texNoise.getSize().x; x++)
    {
        for(unsigned y = 0; y < texNoise.getSize().y; y++)
        {
            sf::RectangleShape rs({1,1});
            rs.setPosition(x,y);
            rs.setFillColor(sf::Color(random.range(0,255), random.range(0,255), random.range(0,255), random.range(0,255)));
            texNoise.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
        }
    }
    texNoise.display();
    // Initialize matrixIntensity //
    matrixIntensity.create(1920/CHARACTER_W + 1, 1080/CHARACTER_H + 1);
    if(!intensityShader.loadFromFile("assets/calculate-intensities.frag", sf::Shader::Fragment) &&
	   !intensityShader.loadFromFile("../../assets/calculate-intensities.frag", sf::Shader::Fragment))
    {
		std::cerr << "ERROR: failed to load calculate-intensities.frag!\n";
        assert(false);
    }
	intensityShader.setUniform("characterSize", sf::Glsl::Vec2{ float(CHARACTER_W), float(CHARACTER_H) });
	intensityShader.setUniform("matrixSize", sf::Glsl::Vec2{ float(matrixIntensity.getSize().x), float(matrixIntensity.getSize().y) });
    // Initialize minMaxIntensity //
    minMaxIntensity.create(2,1);
    if(!minMaxShader.loadFromFile("assets/calculate-min-max-intensity.frag", sf::Shader::Fragment) &&
	   !minMaxShader.loadFromFile("../../assets/calculate-min-max-intensity.frag", sf::Shader::Fragment))
    {
		std::cerr << "ERROR: failed to load calculate-min-max-intensity.frag!\n";
        assert(false);
    }
    // Initialize digitalRaindrops //
    digitalRaindrops.create(matrixIntensity.getSize().x, 3);
    digitalRaindrops.clear(sf::Color(0,0,0,255));
    sf::RectangleShape rs({1, 2});
    for(unsigned x = 0; x < matrixIntensity.getSize().x; x++)
    {
        int randTime = random.range(0x100,0x2A00);
        rs.setPosition(x,0);
//        rs.setFillColor(sf::Color(0, 0, randTime&0xFF, (randTime>>8)&0xFF));//time & timer  (fixed float)
        rs.setFillColor(sf::Color(0, 0, 200, 0));//time & timer  (fixed float)
//        rs.setFillColor(sf::Color(0, 0, (x+1)%255, (x+1)%255));//time & timer  (fixed float)
        digitalRaindrops.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
    }
    digitalRaindrops.display();
    digitalRaindropsPrev.create(matrixIntensity.getSize().x, 3);
    if(!digitalRaindropsShader.loadFromFile("assets/process-digital-rain.frag", sf::Shader::Fragment) &&
	   !digitalRaindropsShader.loadFromFile("../../assets/process-digital-rain.frag", sf::Shader::Fragment))
    {
		std::cerr << "ERROR: failed to load process-digital-rain.frag!\n";
        assert(false);
    }
    digitalRaindropsShader.setUniform("digitalRaindropsPrev", digitalRaindropsPrev.getTexture());
    digitalRaindropsShader.setUniform("matrixIntensity", matrixIntensity.getTexture());
    digitalRaindropsShader.setUniform("noise", texNoise.getTexture());
    // Initialize matrixRain //
    matrixRain.create(matrixIntensity.getSize().x, matrixIntensity.getSize().y*3);
    matrixRain.clear(sf::Color(0,0,0,0));
    rs.setSize({float(matrixIntensity.getSize().x), float(matrixIntensity.getSize().y)});
    rs.setPosition(0,matrixIntensity.getSize().y*2);
    rs.setFillColor(sf::Color(255,255,255,255));
    matrixRain.draw(rs, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
    matrixRain.display();
    matrixRainPrev.create(matrixIntensity.getSize().x, matrixIntensity.getSize().y*3);
    if(!matrixRainShader.loadFromFile("assets/process-matrix-rain.frag", sf::Shader::Fragment) && 
	   !matrixRainShader.loadFromFile("../../assets/process-matrix-rain.frag", sf::Shader::Fragment))
    {
		std::cerr << "ERROR: failed to load process-matrix-rain.frag!\n";
        assert(false);
    }
    matrixRainShader.setUniform("digitalRaindrops", digitalRaindrops.getTexture());
    matrixRainShader.setUniform("matrixIntensity", matrixIntensity.getTexture());
    matrixRainShader.setUniform("matrixRainPrev", matrixRainPrev.getTexture());
    // Initialize matrix shader //
    if(!m_shader.loadFromFile("assets/matrix.frag", sf::Shader::Fragment) &&
	   !m_shader.loadFromFile("../../assets/matrix.frag", sf::Shader::Fragment))
    {
		std::cerr << "ERROR: failed to load matrix.frag!\n";
        assert(false);
    }
    m_shader.setUniform("glyphCodex", glyphCodex);
	m_shader.setUniform("characterSize", sf::Glsl::Vec2{ float(CHARACTER_W), float(CHARACTER_H) });
    m_shader.setUniform("codexColumns", float(codexColumns));
    m_shader.setUniform("orderedGlyphIndices", orderedGlyphIndicesTex);
    m_shader.setUniform("matrixIntensity", matrixIntensity.getTexture());
    m_shader.setUniform("minMaxIntensity", minMaxIntensity.getTexture());
    m_shader.setUniform("matrixRain", matrixRain.getTexture());
}
Matrix::~Matrix()
{
}
void Matrix::step(float deltaSeconds, const sf::Image& camImage)
{
    // (percentage of the screen that one character takes up) * (width of the camera) = pixels of the image get read by each character
    float pixelsPerCharacterW = float(CHARACTER_W)/renderWindow.getSize().x*camImage.getSize().x;
    float pixelsPerCharacterH = float(CHARACTER_H)/renderWindow.getSize().y*camImage.getSize().y;
    unsigned long long newMinIntensity = -1;
    unsigned long long newMaxIntensity = 0;
    unsigned long long minMaxRange = maxIntensity - minIntensity;
    for(unsigned r = 0; r < theGrid.size(); r++)
    {
        for(unsigned c = 0; c < theGrid[r].size(); c++)
        {
            //find the camera intensity for this character:
            theGrid[r][c].cameraIntensity = 0;
            for(unsigned x = c*pixelsPerCharacterW; x < c*pixelsPerCharacterW + pixelsPerCharacterW; x++)
            {
                for(unsigned y = r*pixelsPerCharacterH; y < r*pixelsPerCharacterH + pixelsPerCharacterH; y++)
                {
                    sf::Color cameraPixel = camImage.getPixel(std::min(x,camImage.getSize().x - 1),
                                                              std::min(y,camImage.getSize().y - 1));
                    unsigned pixelIntensity = unsigned(cameraPixel.r) + unsigned(cameraPixel.g) + unsigned(cameraPixel.b);
                    theGrid[r][c].cameraIntensity += pixelIntensity;
                }
            }
            if(theGrid[r][c].cameraIntensity < newMinIntensity)
            {
                newMinIntensity = theGrid[r][c].cameraIntensity;
            }
            if(theGrid[r][c].cameraIntensity > newMaxIntensity)
            {
                newMaxIntensity = theGrid[r][c].cameraIntensity;
            }
            if(minMaxRange <= 0)
            {
                theGrid[r][c].spriteId = orderedGlyphIndices[0];
            }
            else
            {
                unsigned index = float(theGrid[r][c].cameraIntensity - minIntensity)/minMaxRange*orderedGlyphIndices.size();
                index = std::min(std::max(index, unsigned(0)), unsigned(orderedGlyphIndices.size() - 1));
                theGrid[r][c].spriteId = orderedGlyphIndices[index];
            }
            glyphSprite.setPosition(c*CHARACTER_W, r*CHARACTER_H);
            unsigned glyphRow = theGrid[r][c].spriteId/codexColumns;
            unsigned glyphCol = theGrid[r][c].spriteId%codexColumns;
            glyphSprite.setTextureRect(sf::IntRect(glyphCol*CHARACTER_W,glyphRow*CHARACTER_H,
                                                   CHARACTER_W,CHARACTER_H));
            renderWindow.draw(glyphSprite);
        }
    }
    minIntensity = newMinIntensity;
    maxIntensity = newMaxIntensity;
}
void Matrix::stepShader(float deltaSeconds, const sf::Sprite& camSprite)
{
	if (debugMode == 5)
	{
		renderWindow.draw(camSprite);
	}
	else
	{
		// calculate fps //
		sf::Time timePassedSinceLastFrame = clock.getElapsedTime();
		clock.restart();
		float fps = 1/(timePassedSinceLastFrame.asSeconds() > 0 ? timePassedSinceLastFrame.asSeconds() : 1/16.f);
		avgFPS = 0.9*avgFPS + 0.1*fps;
		seconds += timePassedSinceLastFrame.asSeconds();
		// process digital raindrops //
		sf::Sprite sprDigitalRaindrops(digitalRaindrops.getTexture());
		digitalRaindropsPrev.draw(sprDigitalRaindrops, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
		digitalRaindropsPrev.display();
		digitalRaindropsShader.setUniform("deltaSeconds", timePassedSinceLastFrame.asSeconds());
		digitalRaindropsShader.setUniform("time", seconds);
		sf::RenderStates statesDigitalRain(&digitalRaindropsShader);
		statesDigitalRain.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero);
		digitalRaindrops.draw(sprDigitalRaindrops, statesDigitalRain);
		digitalRaindrops.display();
		// process matrix rain //
		sf::Sprite sprMatrixRain(matrixRain.getTexture());
		matrixRainPrev.draw(sprMatrixRain, sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
		matrixRainPrev.display();
		matrixRainShader.setUniform("deltaSeconds", timePassedSinceLastFrame.asSeconds());
		sf::RenderStates statesMatrixRain(&matrixRainShader);
		statesMatrixRain.blendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero);
		matrixRain.draw(sprMatrixRain, statesMatrixRain);
		matrixRain.display();
		// build intensity matrix //
		matrixIntensity.draw(camSprite, &intensityShader);
		matrixIntensity.display();
		// calculate min/max intensity //
		sf::Sprite sprIntensity(matrixIntensity.getTexture());
		minMaxIntensity.draw(sprIntensity, &minMaxShader);
		minMaxIntensity.display();
		// draw The Matrix //
		switch (debugMode)
		{
		case 0:
			renderWindow.draw(camSprite, &m_shader);
			break;
		case 1:
			sprIntensity.setScale(2.5, 2.5);
			renderWindow.draw(sprIntensity);
			break;
		case 2:
		{
			sf::Sprite sprMinMax(minMaxIntensity.getTexture());
			sprMinMax.setScale(100, 100);
			renderWindow.draw(sprMinMax);
		}
		break;
		case 3:
			sprDigitalRaindrops.setScale(7, 100);
			renderWindow.draw(sprDigitalRaindrops);
			break;
		case 4:
			sprMatrixRain.setScale(2.5, 2.5);
			renderWindow.draw(sprMatrixRain);
			break;
		}
	}
    // draw FPS //
    if(showFPS)
    {
        std::stringstream ss;
        ss << "FPS:" << int(avgFPS);
        txtDisplay.setString(ss.str());
        txtDisplay.setPosition(renderWindow.getView().getSize().x - txtDisplay.getLocalBounds().width, 0);
        sf::RectangleShape rs({txtDisplay.getGlobalBounds().width, txtDisplay.getGlobalBounds().height*2});
        rs.setPosition(renderWindow.getView().getSize().x - txtDisplay.getLocalBounds().width, 0);
        rs.setFillColor(sf::Color::Black);
        renderWindow.draw(rs);
        renderWindow.draw(txtDisplay);
    }
    /// DEBUG ///
    if(m_appFocus)
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
        {
            debugMode = 0;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
        {
            debugMode = 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F3))
        {
            debugMode = 2;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
        {
            debugMode = 3;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
        {
            debugMode = 4;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
        {
            debugMode = 5;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F7))
        {
            showFPS = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F8))
        {
            showFPS = false;
        }
    }
//    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
//    {
//        sf::Vector2i pos = sf::Mouse::getPosition(renderWindow);
//        sf::Image img = digitalRaindrops.getTexture().copyToImage();
//        sf::Vector2i imgPos(pos.x/7, pos.y/100);
//        if(imgPos.x < img.getSize().x && imgPos.y < img.getSize().y)
//        {
//            sf::Color pixel = img.getPixel(imgPos.x, imgPos.y);
//            std::cout<<int(pixel.r)<<","<<int(pixel.g)<<","<<int(pixel.b)<<","<<int(pixel.a)<<"\n";
//        }
//    }
}
void Matrix::onResize()
{
    std::cout<<"renderwindow.size=("<<renderWindow.getSize().x<<","<<renderWindow.getSize().y<<")\n";
    sf::Vector2u gridSize(std::max(renderWindow.getSize().x/CHARACTER_W, unsigned(1)) + 1,
                          std::max(renderWindow.getSize().y/CHARACTER_H, unsigned(1)) + 1);
    std::cout<<"gridSize=("<<gridSize.x<<","<<gridSize.y<<")\n";
    unsigned curr = 0;
    theGrid.resize(gridSize.y);
    for(unsigned r = 0; r < theGrid.size(); r++)
    {
        theGrid[r].resize(gridSize.x);
        for(unsigned c = 0; c < theGrid[r].size(); c++)
        {
//            theGrid[r][c].spriteId = abs(random.rInt())%256;
            theGrid[r][c].spriteId = orderedGlyphIndices[(curr++)%orderedGlyphIndices.size()];
        }
    }
}
void Matrix::appFocus(bool val)
{
    m_appFocus = val;
}
