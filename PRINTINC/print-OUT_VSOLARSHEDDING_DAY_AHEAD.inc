* File: print-OUT_VWINDSHEDDING_DAY_AHEAD.inc

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* VWINDSHEDDING_DAY_AHEAD depends on the sets RRR and T

PUT OUT_VSOLARSHEDDING_DAY_AHEAD_OUT;
OUT_VSOLARSHEDDING_DAY_AHEAD_OUT.PW = 32767;
OUT_VSOLARSHEDDING_DAY_AHEAD_OUT.PS = 1;
OUT_VSOLARSHEDDING_DAY_AHEAD_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               LOOP (R_RId(IR, RId),
                  If(VSOLARSHEDDING_DAY_AHEAD.L(T,IR),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",VSOLARSHEDDING_DAY_AHEAD.L(T,IR):0:4/;
                  );
               );
            );
         );
     );
   );
);

