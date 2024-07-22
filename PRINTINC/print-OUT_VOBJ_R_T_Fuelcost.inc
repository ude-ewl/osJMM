* File: print-OUT_VOBJ_R_T_Fuelcost.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_Fuelcost_OUT;
OUT_VOBJ_R_T_Fuelcost_OUT.PW = 32767;
OUT_VOBJ_R_T_Fuelcost_OUT.PS = 1;
OUT_VOBJ_R_T_Fuelcost_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_Fuelcost_out(IR,t)

 =

* #1 (VGFUELUSAGE_T)
* Cost of fuel consumption during optimisation period:

   SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$(( GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
        (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUSINGFUEL,T)) * VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)
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
                  VOBJ_Fuelcost_out(IR,T):0:4/ ;
            );
         );
      );
   );
);