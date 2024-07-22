* File: print-OUT_VOBJ_R_T_Revenue.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_Revenue_OUT;
OUT_VOBJ_R_T_Revenue_OUT.PW = 32767;
OUT_VOBJ_R_T_Revenue_OUT.PS = 1;
OUT_VOBJ_R_T_Revenue_OUT.nJ = 2;


   VOBJ_Revenue_out(IR,t)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1)

 =
$ontext
*  Revenue for unit being online:

  - SUM( (IAGK_Y(IA,IGUC),FFF)$( GDATA(IA,IGUC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB') AND RRRAAA(IR,IA)),
     GDATA(IA,IGUC,'GDSTARTUPFUELCONSHOT') * (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUC,T)) *
$ifi     '%UnitCmin%' == Yes        IGELECCAPEFF(IA,IGUC,T) *
         VGONLINE_T.L(T,IA,IGUC)
     )
$offtext



* #12 (VCONTENTHYDRORES_T)
* Revenue for hydroreservoir
  - SUM(IAHYDRO$(RRRAAA(IR,IAHYDRO)),
       ISDP_HYDRORES(IAHYDRO,T)
                                    *  (VCONTENTHYDRORES_T.L(T,IAHYDRO) - VCONTENTHYDRORES_T.L('T00',IAHYDRO)
       )

$ifi     '%DeRD%' == Yes              ( ISDP_HYDRORES(IAHYDRO,T) - ISDP_HYDRORES(IAHYDRO,ITALIAS)) * VCONTENTHYDRORES_T.L(T,IAHYDRO)

      )


* Penalty for deviation from reservoir reference level:
* 0-1% =  0.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 1-2% =  1.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 2-3% =  2.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* ...
* 9-10% = 9.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* --> penalty increases (100% -->300%) as storage capacity decreases (1000 flh --> 333 flh), allowing "weekly storages" (low storage capacity) a less flexible operation than "annual storages" (high storage capacity)
$ifi '%HydroPenalty%' == YES   + SUM(IAHYDRO$(RRRAAA(IR,IAHYDRO)),
$ifi '%HydroPenalty%' == YES         sum(IHYDFLXSTEP, VQRESLEVELUP.L(T,IAHYDRO,IHYDFLXSTEP)  *(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES       + sum(IHYDFLXSTEP, VQRESLEVELDOWN.L(T,IAHYDRO,IHYDFLXSTEP)*(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES   )





* # 13+14 (VCONTENTSTORAGE_T)

  - SUM(IAGK_Y(IA,IGSTORAGE)$(RRRAAA(IR,IA)),
$ifi NOT '%Looping%' == week     (ISDP_STORAGE(IA,IGSTORAGE) *
$ifi     '%Looping%' == week     ((   ISDP_STORAGE(IA,IGSTORAGE)$(ISDP_STORAGE(IA,IGSTORAGE) NE 0)
$ifi     '%Looping%' == week     + ISDP_REFPRICE(IA)$(ISDP_STORAGE(IA,IGSTORAGE) EQ 0)) *
           VCONTENTSTORAGE_T.L(T,IA,IGSTORAGE))
      )


* #24: Revenues for PtG
$ifi '%PtG%' ==  Yes  - SUM(IAGK_Y(IA,IGPTG),
$ifi '%PtG%' ==  Yes     
$ifi '%PtG%' ==  Yes         IUSEVALUE(IA,IGPTG)*(VGELEC_T.l(T,IA,IGPTG) + VGELEC_DPOS_T.l(T,IA,IGPTG) - VGELEC_DNEG_T.l(T,IA,IGPTG))/GDATA(IA,IGPTG,'GDLOADEFF')
$ifi '%PtG%' ==  Yes      
$ifi '%PtG%' ==  Yes    )
;

* end loop (IR,t
*);




if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP(IR,
            LOOP (R_RId(IR,RId),
               Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                  VOBJ_Revenue_out(IR,T):0:4/ ;
            );
         );
      );
   );
);