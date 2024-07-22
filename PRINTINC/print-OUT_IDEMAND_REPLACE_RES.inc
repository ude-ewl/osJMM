* File: print-OUT_IDEMAND_REPLACE_RES.inc

* IDEMAND_REPLACE_RES depends on the sets RRR, Node and T

PUT OUT_IDEMAND_REPLACE_RES_OUT;
OUT_IDEMAND_REPLACE_RES_OUT.PW = 32767;
OUT_IDEMAND_REPLACE_RES_OUT.PS = 1;
OUT_IDEMAND_REPLACE_RES_OUT.nJ = 2;


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
               IDEMAND_NONSPIN_ANCPOS(TSO_RRR):0:4/ ;
            );
         );
      );
   );
);
