* File: print-OUT_QANCNEGEQ_SEC_M.inc

* This file will print the marginal value of the equation QANCNEGEQ_SEC,
* which is understood as the price (per hour) of reserving capacity for providing
* negative secondary reserve
* QANCNEGEQ_SEC.M depends on the sets RRR, Node and Day

PUT OUT_QANCNEGEQ_SEC_M_OUT;
OUT_QANCNEGEQ_SEC_M_OUT.PW = 32767;
OUT_QANCNEGEQ_SEC_M_OUT.PS = 1;
OUT_QANCNEGEQ_SEC_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_TSO_Region,BaseDate,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
if ((IFIXSPINNING_UNITS_YES=0),
   LOOP (CaseId,
      LOOP (TSO_RRR,
          LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
                        AND (ORD(BASETIME) <= (IBASEFINALTIME-1)) ),
                LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
                    LOOP (TSO_RRR_TSO_RRRId(TSO_RRR, TSO_RRRId),
                        Put CaseId.TL:0,",",TSO_RRRId.TL:0,",",T.TL:0,",",
                        QANCNEGEQ_SEC.M(T,TSO_RRR) :0:4/;
                    );
                );
          );
      );
   );
);

