* File: print-OUT_BASETIME.inc


* This file will print the BASETIME used in the case

PUT BASETIME_OUT;
BASETIME_OUT.PW = 32767;
BASETIME_OUT.PS = 1;
BASETIME_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,BaseTime,Week" /;
);

* -----------------------------------------------------------------------------
LOOP(CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                    AND (ORD(BASETIME) <=  IBASEFINALTIMEWRITEOUT ) ),
      LOOP(weeks$BASETIME_WEEK(BASETIME,WEEKS),
         PUT CaseId.TL:0,",",BASETIME.TL:0,",",WEEKS.TL:0/;
      );
   );
);