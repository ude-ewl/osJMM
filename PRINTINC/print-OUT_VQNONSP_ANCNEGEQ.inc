* File: print-OUT_VQNONSP_ANCNEGEQ.inc

PUT OUT_VQNONSP_ANCNEGEQ_OUT;
OUT_VQNONSP_ANCNEGEQ_OUT.PW = 32767;
OUT_VQNONSP_ANCNEGEQ_OUT.PS = 1;
OUT_VQNONSP_ANCNEGEQ_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP (TSO_RRR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (TSO_RRR_TSO_RRRId(TSO_RRR,TSO_RRRId) $VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IMINUS'),
              Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",BASETIME.TL:0,",",
              VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IMINUS'):0:4/ ;
           );
        );
     );
  );
);