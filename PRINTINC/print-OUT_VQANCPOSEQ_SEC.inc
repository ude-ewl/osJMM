* File: print-OUT_VQANCPOSEQ_SEC.inc

* This file will print the marginal value of equation VQNONSP_ANCPOSEQ_SEC, which is
* a slack variable for the not-fullfillment of secondary positive spinning reserve requirements. 
* OUT_VQANCPOSEQ_SEC depends on the sets ZZZ, Node and T

PUT OUT_VQANCPOSEQ_SEC_OUT;
OUT_VQANCPOSEQ_SEC_OUT.PW = 32767;
OUT_VQANCPOSEQ_SEC_OUT.PS = 1;
OUT_VQANCPOSEQ_SEC_OUT.nJ = 2;


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
           LOOP (TSO_RRR_TSO_RRRId(TSO_RRR,TSO_RRRId)$VQANCPOSEQ_SEC.L(T,TSO_RRR,'IMINUS'),
              Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",BASETIME.TL:0,",",
              VQANCPOSEQ_SEC.L(T,TSO_RRR,'IMINUS'):0:4/ ;
           );
        );
     );
  );
);
