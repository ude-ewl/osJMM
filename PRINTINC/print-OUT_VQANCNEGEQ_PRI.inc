* File: print-OUT_VQANCNEGEQ_PRI.inc

* This file will print the marginal value of equation VQANCNEGEQ_PRI, which is
* a slack variable for the not-fullfillment of positive negative spinning reserve requirements.
* OUT_VQANCNEGEQ_PRI depends on the sets ZZZ, Node and T

PUT OUT_VQANCNEGEQ_PRI_OUT;
OUT_VQANCNEGEQ_PRI_OUT.PW = 32767;
OUT_VQANCNEGEQ_PRI_OUT.PS = 1;
OUT_VQANCNEGEQ_PRI_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Controlzone,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP (TSO_RRR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (TSO_RRR_TSO_RRRId(TSO_RRR,TSO_RRRId)$VQANCNEGEQ_PRI.L(T,TSO_RRR,'IMINUS'),
              Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",BASETIME.TL:0,",",
              VQANCNEGEQ_PRI.L(T,TSO_RRR,'IMINUS'):0:4/ ;
           );
        );
     );
  );
);
