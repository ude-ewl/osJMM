* File: print-OUT_VQANCNEGEQ.inc

* This file will print the marginal value of equation VQANCNEGEQ, which is
* a slack variable for the not-fullfillment of negative spinning reserve requirements.
* OUT_VQANCNEGEQ depends on the sets ZZZ, Node and T

PUT OUT_VQANCNEGEQ_OUT;
OUT_VQANCNEGEQ_OUT.PW = 32767;
OUT_VQANCNEGEQ_OUT.PS = 1;
OUT_VQANCNEGEQ_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Controlzone,BaseTime,IMINUS,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP (TSO_RRR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (TSO_RRR_TSO_RRRId(TSO_RRR,TSO_RRRId)$VQANCNEGEQ.L(T,TSO_RRR,'IMINUS'),
              Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",BASETIME.TL:0,",",
              VQANCNEGEQ.L(T,TSO_RRR,'IMINUS'):0:4/ ;
           );
        );
     );
  );
);
