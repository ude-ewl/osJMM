* File: print-OUT_VXELEC_T_Realised_onlyNTCDC.inc

* This file will print the value of VXELEC_T_Realised
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.

PUT OUT_VXELEC_T_Realised_onlyNTCDC_OUT;
OUT_VXELEC_T_Realised_onlyNTCDC_OUT.PW = 1255;
OUT_VXELEC_T_Realised_onlyNTCDC_OUT.PS = 1;
OUT_VXELEC_T_Realised_onlyNTCDC_OUT.nJ = 2;

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
                      - (VXELEC_T.l(T,IRE,IR) + VXELEC_DPOS_T.l(T,IRE,IR) - VXELEC_DNEG_T.l(T,IRE,IR))
                        )
$ifi '%LFB%' == Yes   + SUM(IEXPORT_FLBMC_DC_Y(IR,IRE), VXELEC_DC_T.L(T,IR,IRE) + VXELEC_DC_DPOS_T.L(T,IR,IRE) - VXELEC_DC_DNEG_T.L(T,IR,IRE)
$ifi '%LFB%' == Yes                               - (VXELEC_DC_T.L(T,IRE,IR) + VXELEC_DC_DPOS_T.L(T,IRE,IR) - VXELEC_DC_DNEG_T.L(T,IRE,IR)) 
$ifi '%LFB%' == Yes    )
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





