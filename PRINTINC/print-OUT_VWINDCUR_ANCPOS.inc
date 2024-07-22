* File: print-OUT_VWINDCUR_ANCPOS.inc

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* VWINDCUR_ANCPOS depends on the sets RRR and T

PUT VWINDCUR_ANCPOS_OUT;
VWINDCUR_ANCPOS_OUT.PW = 32767;
VWINDCUR_ANCPOS_OUT.PS = 1;
VWINDCUR_ANCPOS_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP (R_RId(IR, RId),
               IF(VWINDCUR_ANCPOS.L(T,IR)>0,
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",VWINDCUR_ANCPOS.L(T,IR):0:4/;
               );
            );
         );
     );
   );
);
