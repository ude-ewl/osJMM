* File: print-OUT_QHYRSSEQ_M.inc

* This file will print the marginal value of equation QESTOVOLT, which is
* interpreted as marginal value of increasing the stored energy in the electricity storage.
* QHYRSSEQ_M depends on the sets AAA, GGG, Node and T

PUT OUT_QHYRSSEQ_M_OUT;
OUT_QHYRSSEQ_M_OUT.PW = 32767;
OUT_QHYRSSEQ_M_OUT.PS = 1;
OUT_QHYRSSEQ_M_OUT.nJ = 2;


*all hours are printed
if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAHYDRO,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
                  Put CaseId.TL:0,",",IAHYDRO.TL:0,",",BASETIME.TL:0,",",
                  QHYRSSEQ.M(T,IAHYDRO):0:4/ ;
               );
      );
   );
);