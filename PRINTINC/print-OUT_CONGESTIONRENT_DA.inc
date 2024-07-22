* File: print-OUT_CONGESTIONRENT_DA.inc

* This file will print the value of VXELEC_T_Realised
* multiplied with the price difference of the 2 different Regions

* VXELEC_T_Realised depends on the sets RRR, RRR, and T.
* QEEQINT depends on the sets RRR and T.


PUT OUT_CONGESTIONRENT_DA;
OUT_CONGESTIONRENT_DA.PW = 32767;
OUT_CONGESTIONRENT_DA.PS = 1;
OUT_CONGESTIONRENT_DA.nJ = 2;

$ontext
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE2') )
                      AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE3')) ),
         LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1))  ),
$offtext

IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
   LOOP (CaseId,
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME + NO_HOURS_OVERLAP)
                      AND (ORD(BASETIME) <= IBASEFINALTIME) ),
         LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
           LOOP(IR,
                   Congestionrent_DA(IR,T) = SUM(IRE,                               
                                               ((-VXELEC_T.l(T,IR,IRE)*QEEQDAY.M(T,IR) + VXELEC_T.l(T,IR,IRE)*(1-XLOSS(IR,IRE))*QEEQDAY.M(T,IRE)
                                                 )/2)$IEXPORT_NTC_Y(IR,IRE) +
                                               ((-VXELEC_T.l(T,IRE,IR)*QEEQDAY.M(T,IRE)+ VXELEC_T.l(T,IRE,IR)*(1-XLOSS(IRE,IR))*QEEQDAY.M(T,IR)
                                                 )/2)$IEXPORT_NTC_Y(IRE,IR)
                                                 
$ifi '%LFB%' == Yes         + ((-VXELEC_DC_T.l(T,IR,IRE)*QEEQDAY.M(T,IR) + VXELEC_DC_T.l(T,IR,IRE)*QEEQDAY.M(T,IRE)
$ifi '%LFB%' == Yes          )/2)$IEXPORT_FLBMC_DC_Y(IR,IRE)  +
$ifi '%LFB%' == Yes         ((-VXELEC_DC_T.l(T,IRE,IR)*QEEQDAY.M(T,IRE)+ VXELEC_DC_T.l(T,IRE,IR)*QEEQDAY.M(T,IR)
$ifi '%LFB%' == Yes          )/2)$IEXPORT_FLBMC_DC_Y(IR,IRE)
                             ) 

$ifi '%LFB_NE%' == Yes      - SUM(RRR_FLBMC(IR), 
$ifi '%LFB_NE%' == Yes           VXELEC_EXPORT_T.L(T,IR) * QEEQINT.M(T,IR)
$ifi '%LFB_NE%' == Yes        )
                );
             );
         );
     );
   );


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
   LOOP (CaseId,
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME + NO_HOURS_OVERLAP)
                      AND (ORD(BASETIME) <= IBASEFINALTIME) ),
         LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
           LOOP(IR,
               LOOP (R_RId(IR, RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                     Congestionrent_DA(IR,T):0:4/;
               );
            );
         );
     );
   );
);



