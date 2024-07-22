* File: print-OUT_VXELEC_T_Realised_PhysicalFlows.inc

* This file will print the value of VXELEC_T_Realised
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.

PUT OUT_VXELEC_T_Realised_PhysicalFlows_OUT;
OUT_VXELEC_T_Realised_PhysicalFlows_OUT.PW = 1255;
OUT_VXELEC_T_Realised_PhysicalFlows_OUT.PS = 1;
OUT_VXELEC_T_Realised_PhysicalFlows_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,FLG,BaseTime,Value" /;
);


* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP(BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
          LOOP(FLG, 
              IPHYSICALFLOW_TMP(FLG,T) = SUM(RRR_FLBMC, 
                           IPTDF_FLG(RRR_FLBMC,FLG,T) *
                           (VXELEC_EXPORT_T.L(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T.L(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T.L(T,RRR_FLBMC))
                      );

               IF(abs(IPHYSICALFLOW_TMP(FLG,T)) > EPSILON ,
                  Put CaseId.TL:0,",";
                  Put FLG.TL:0,",";
                  Put BASETIME.TL:0,",",IPHYSICALFLOW_TMP(FLG,T):0:4/ ;
               );
        );
      );
   );
);





