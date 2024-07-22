* File: print-OUT_VXELEC_T_Realised_woSaldo.inc

* This file will print the value of VXELEC_T_Realised
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.

PUT OUT_VXELEC_T_Realised_woSaldo_OUT;
OUT_VXELEC_T_Realised_woSaldo_OUT.PW = 32767;
OUT_VXELEC_T_Realised_woSaldo_OUT.PS = 1;
OUT_VXELEC_T_Realised_woSaldo_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region1,ID_Region2,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP(BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
          LOOP(IR,
            LOOP(IRE,
               IVXELEC_TMP(T,IR,IRE) = SUM(IEXPORT_NTC_Y(IR,IRE),
                                              VXELEC_T.l(T,IR,IRE) + VXELEC_DPOS_T.l(T,IR,IRE) - VXELEC_DNEG_T.l(T,IR,IRE)

											)
$ifi '%LFB%' == Yes                       + SUM(IEXPORT_FLBMC_DC_Y(IR,IRE),
$ifi '%LFB%' == Yes                               VXELEC_DC_T.L(T,IR,IRE) + VXELEC_DC_DPOS_T.L(T,IR,IRE) - VXELEC_DC_DNEG_T.L(T,IR,IRE)
*$ifi '%LFB%' == Yes                              - (VXELEC_DC_T.L(T,IRE,IR) + VXELEC_DC_DPOS_T.L(T,IRE,IR) - VXELEC_DC_DNEG_T.L(T,IRE,IR))
$ifi '%LFB%' == Yes                             )
$ifi '%LFB_NE%' == Yes                    + MAX(0, SUM(RRR_FLBMC,
$ifi '%LFB_NE%' == Yes                            (VXELEC_EXPORT_T.L(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T.L(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T.L(T,RRR_FLBMC)) 


$ifi '%LFB_NE%' == Yes                                 )
$ifi '%LFB_NE%' == Yes                 )
									   ;

               IF(IVXELEC_TMP(T,IR,IRE) > EPSILON ,
                  Put CaseId.TL:0,",";
                  LOOP(R_RId(IR,RId),
                     Put RId.TL:0,",";
                  );
                  LOOP(R_RId(IRE,RId),
                     Put RId.TL:0,",",BASETIME.TL:0,",",IVXELEC_TMP(T,IR,IRE):0:4/ ;
                  );
               );
            );
        );
      );
   );
);





