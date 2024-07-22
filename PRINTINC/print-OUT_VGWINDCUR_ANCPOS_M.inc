* File: print-OUT_QEEQDAY_M.inc

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* QEEQDAY_M depends on the sets RRR and T

PUT OUT_QEEQDAY_M_OUT;
OUT_QEEQDAY_M_OUT.PW = 32767;
OUT_QEEQDAY_M_OUT.PS = 1;
OUT_QEEQDAY_M_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
      LOOP (IR,
         LOOP(BASETIME  $( ( ORD(BASETIME) >= (IBASESTARTTIME + 2*NO_HOURS_OVERLAP) )
                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               LOOP (R_RId(IR, RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",QEEQDAY.M(T,IR):0:4/;
               );
            );
         );
     );
   );
