* File: print-OUT_VOBJ_R_T_FlexDemand.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_FlexDemand_OUT;
OUT_VOBJ_R_T_FlexDemand_OUT.PW = 32767;
OUT_VOBJ_R_T_FlexDemand_OUT.PS = 1;
OUT_VOBJ_R_T_FlexDemand_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_FlexDemand_out(IR,t)

 =

* # 15+16 (VDEMANDELECFLEXIBLE_T)
* Changes in consumers utility relative to electricity consumption:

  + IFLEXIBLE_DEF_YES *
    (
          SUM(DEF_D, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_D)* IDEFLEXIBLEPRICE_T(IR,DEF_D))
          - SUM(DEF_U, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_U)* IDEFLEXIBLEPRICE_T(IR,DEF_U))
    )
*end change

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
                  VOBJ_FlexDemand_out(IR,T):0:4/ ;
            );
         );
      );
   );
);