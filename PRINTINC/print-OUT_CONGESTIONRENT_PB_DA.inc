* File: print-OUT_CONGESTIONRENT_DA.inc

* This file will print the value of VXELEC_T_Realised
* multiplied with the price difference of the 2 different Regions

* VXELEC_T_Realised depends on the sets RRR, RRR, and T.
* QEEQINT depends on the sets RRR and T.

PUT OUT_CONGESTIONRENT_PB_DA;
OUT_CONGESTIONRENT_PB_DA.PW = 32767;
OUT_CONGESTIONRENT_PB_DA.PS = 1;
OUT_CONGESTIONRENT_PB_DA.nJ = 2;


IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
   LOOP ((IR,IRE,T)$(((ORD(T)>NO_HOURS_OVERLAP +1) AND (ORD(T)<= 3*NO_HOURS_OVERLAP +1))),
      Congestionrent_PB_DA(IR,IRE,T) = (-QEEQDAY.M(T,IR) + (1-XLOSS(IR,IRE))*QEEQDAY.M(T,IRE)) * (
                        SUM(IEXPORT_NTC_Y(IR,IRE), VXELEC_T.l(T,IR,IRE)) 
$ifi '%LFB_NE%' == Yes   + SUM(IEXPORT_FLBMC_DC_Y(IRE,IR), VXELEC_DC_T.L(T,IRE,IR))

$ifi '%LFB_NE%' == Yes   + SUM(RRR_FLBMC(IR), 
$ifi '%LFB_NE%' == Yes        SUM(ISFD_Y_FLG(FLG), IPTDF_FLG(IR,FLG,T)) * VXELEC_EXPORT_T.L(T,IR)  
$ifi '%LFB_NE%' == Yes   );        
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
*            LOOP (IEXPORT_Y(IR,IRE),
             LOOP ((IR,IRE),
               if ( Congestionrent_PB_DA(IR,IRE,T) > EPSILON,
                  Put CaseId.TL:0,",";
                  LOOP(R_RId(IR,RId),
                     Put RId.TL:0,",";
                  );
                  LOOP(R_RId(IRE,RId),
                     Put RId.TL:0,",",BASETIME.TL:0,",", Congestionrent_PB_DA(IR,IRE,T):0:4/ ;
                  );
               );
            );
         );
      );
   );
);



