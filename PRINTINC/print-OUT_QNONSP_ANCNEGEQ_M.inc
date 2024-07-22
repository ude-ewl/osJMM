* File: print-OUT_QNONSP_ANCNEGEQ_M.inc

* This file will print the marginal value of equation QNONSP_ANCNEGEQ, which is
* interpreted as the price of providing additional tertiary reserve.
* QNONSP_ANCNEGEQ_M depends on the sets RRR, Node and T

PUT OUT_QNONSP_ANCNEGEQ_M_OUT;
OUT_QNONSP_ANCNEGEQ_M_OUT.PW = 32767;
OUT_QNONSP_ANCNEGEQ_M_OUT.PS = 1;
OUT_QNONSP_ANCNEGEQ_M_OUT.nJ = 2;



if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_TSO_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (TSO_RRR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP (TSO_RRR_TSO_RRRId(TSO_RRR, TSO_RRRId),
               Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",BASETIME.TL:0,",",
               QNONSP_ANCNEGEQ.M(T,TSO_RRR):0:4/ ;
            );
         );
      );
   );
);