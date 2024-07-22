* File: print-OUT_QEEQDAY_M_ct.inc

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* QEEQDAY_M depends on the sets RRR and T

PUT OUT_QEEQDAY_M_OUT_ct;
OUT_QEEQDAY_M_OUT_ct.PW = 32767;
OUT_QEEQDAY_M_OUT_ct.PS = 1;
OUT_QEEQDAY_M_OUT_ct.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "BaseTime";
   LOOP (IR, PUT ",",IR.TL:0);
);


* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),

         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME
$ifi NOT '%Looping%' == week				     + NO_HOURS_OVERLAP         
         )
$ifi NOT '%Looping%' == week   AND (ORD(BASETIME) <= IBASEFINALTIME) ),      
$ifi '%Looping%' == week       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
                                                PUT / BASETIME.TL:0;
         LOOP (IR,
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               LOOP (R_RId(IR, RId),
                  PUT ",",QEEQDAY.M(T,IR):0:2;
               );
            );
         );
      );
   );
