* File: print-OUT_VOBJ_R_T_CO2Cost.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_CO2Cost_OUT;
OUT_VOBJ_R_T_CO2Cost_OUT.PW = 32767;
OUT_VOBJ_R_T_CO2Cost_OUT.PS = 1;
OUT_VOBJ_R_T_CO2Cost_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_CO2Cost_out(IR,t)

 =

* #9 (VGFUELUSAGE_T)
* Emission taxes CO2, SO2:

   +  SUM(C,
         SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$( (GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),
               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2') * IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGUSINGFUEL,'GDDESO2'))) * CONVERT_KG_PER_GJ_T_PER_MWH *
               VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)
         )
      )
;
* end loop (IR,t
);




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
                  VOBJ_CO2Cost_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
