* File: print-OUT_VDEMANDELECFLEXIBLE.inc

* This file will print the realised consumption of electricity.
* ELEC_CONS_T depends on the sets RRR and T

PUT OUT_VDEMANDELECFLEXIBLE_OUT;
OUT_VDEMANDELECFLEXIBLE_OUT.PW = 32767;
OUT_VDEMANDELECFLEXIBLE_OUT.PS = 1;
OUT_VDEMANDELECFLEXIBLE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

* print out flexible electricity demand activated
* The variable VDEMANDELECFLEXIBLE_T can only used in those loops where the
* day-ahead market is optimised and then only for the timesteps for the day ahead market

LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
        LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
           LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
               LOOP (R_RId(IR,RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",((SUM(DEF_U,VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_U))
                                 -SUM(DEF_D,VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_D)))/(1-DISLOSS_E(IR))):0:4/;
               );
            );
         );
     );
   );
);



$ontext
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE3') )
                         AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE3')) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1))  ),
               LOOP (R_RId(IR,RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",((SUM(DEF_U,VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_U))
                                 -SUM(DEF_D,VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_D)))/(1-DISLOSS_E(IR))):0:4/;
               );
            );
         );
     );
   );
);
$offtext
