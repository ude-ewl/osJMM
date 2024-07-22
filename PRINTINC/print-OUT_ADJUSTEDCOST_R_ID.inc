* File: print-OUT_ADJUSTEDCOST_R_ID.inc

* This file will print the value of VXELEC_T_Realised
* multiplied with the price difference of the 2 different Regions

* VXELEC_T_Realised depends on the sets RRR, RRR, and T.
* QEEQINT depends on the sets RRR and T.


  PUT OUT_ADJUSTEDCOST_R_ID;
  OUT_ADJUSTEDCOST_R_ID.PW = 32000;
  OUT_ADJUSTEDCOST_R_ID.PS = 1;
  OUT_ADJUSTEDCOST_R_ID.nJ = 2;




*LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),

      ADJUSTEDCOST_R_ID(IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1) = SUM(IRE,
                                  -((VXELEC_T.l(T,IR,IRE) + VXELEC_DPOS_T.l(T,IR,IRE) - VXELEC_DNEG_T.l(T,IR,IRE)) *                                                    
                                     (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_NTC_Y(IR,IRE) 
                                  +((VXELEC_T.l(T,IRE,IR) + VXELEC_DPOS_T.l(T,IRE,IR)- VXELEC_DNEG_T.l(T,IRE,IR)) *                                                   
                                     (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_NTC_Y(IRE,IR)
                            
$ifi '%LFB%' == Yes         -(( VXELEC_DC_T.l(T,IR,IRE) + VXELEC_DC_DPOS_T.l(T,IR,IRE) - VXELEC_DC_DNEG_T.l(T,IR,IRE)) *                         
$ifi '%LFB%' == Yes                                (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_FLBMC_DC_Y(IR,IRE) 
$ifi '%LFB%' == Yes         +(( VXELEC_DC_T.l(T,IRE,IR) + VXELEC_DC_DPOS_T.l(T,IRE,IR) - VXELEC_DC_DNEG_T.l(T,IRE,IR)) *
$ifi '%LFB%' == Yes                                (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_FLBMC_DC_Y(IRE,IR)

*$ifi '%LFB_NE%' == Yes     -(( VXELEC_T.l(T,IR,IRE) + VXELEC_DPOS_T.l(T,IR,IRE) - VXELEC_DNEG_T.l(T,IR,IRE)) *                         
*$ifi '%LFB_NE%' == Yes                            (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_FLBMC_Y(IR,IRE) 
*$ifi '%LFB_NE%' == Yes     +(( VXELEC_T.l(T,IRE,IR) + VXELEC_DPOS_T.l(T,IRE,IR) - VXELEC_DNEG_T.l(T,IRE,IR)) *
*$ifi '%LFB_NE%' == Yes                            (QEEQINT.M(T,IRE)+QEEQINT.M(T,IR))/2)$IEXPORT_FLBMC_Y(IRE,IR)


$ifi '%LFB_NE%' == Yes     - SUM(RRR_FLBMC(IR), 
$ifi '%LFB_NE%' == Yes           (VXELEC_EXPORT_T.L(T,IR) + VXELEC_EXPORT_DPOS_T.L(T,IR) - VXELEC_EXPORT_DNEG_T.L(T,IR)) * QEEQINT.M(T,IR)
$ifi '%LFB_NE%' == Yes          )
                 )
                                 + VOBJ_ALL_out(IR,T);

*     );






if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

if (INO_SOLVE ge 1,
  LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP(IR,
              LOOP (R_RId(IR,RId),
                 Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                    ADJUSTEDCOST_R_ID(IR,T):0:4/ ;
              );
           );
        );
     );
  );
);
* -----------------------------------------------------------------------------
