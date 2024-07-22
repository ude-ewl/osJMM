* File: print-OUT_VOBJ_R_T_Trans.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_Trans_OUT;
OUT_VOBJ_R_T_Trans_OUT.PW = 32767;
OUT_VOBJ_R_T_Trans_OUT.PS = 1;
OUT_VOBJ_R_T_Trans_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_Trans_out(IR,t)

 =
* Transmission cost:

  + SUM(IRE $IEXPORT_Y(IRE,IR),
         XCOST(IRE,IR)*
$ifi '%No_Load_Flow%' ==   Yes   (VXELEC_T.L(T,IRE,IR) + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T.L(T,IRE,IR) -VXELEC_DNEG_T.L(T,IRE,IR)) )
$ifi '%LFB_Intraday%' ==   Yes   (VXELEC_T.L(T,IRE,IR) + VXELEC_DPOS_T.L(T,IRE,IR) + VXELEC_DNEG_T.L(T,IRE,IR))
$ifi '%LFB_DayAhead%' ==   Yes   0
$ifi '%LFB_NE%' ==         Yes   0
$ifi '%LFB_BEX%' ==        Yes   (VXELEC_T.L(T,IRE,IR) + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T.L(T,IRE,IR) -VXELEC_DNEG_T.L(T,IRE,IR)) )
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
                  VOBJ_Trans_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
