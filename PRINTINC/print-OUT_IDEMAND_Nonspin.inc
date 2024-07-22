* File: print-OUT_IDEMAND_Nonspin.inc

PUT OUT_IDEMAND_Nonspin_OUT;
OUT_IDEMAND_Nonspin_OUT.PW = 32767;
OUT_IDEMAND_Nonspin_OUT.PS = 1;
OUT_IDEMAND_Nonspin_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,TSO-Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (TSO_RRR,
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
         LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
                         Put CaseId.TL:0,",",TSO_RRR.TL:0,",",BASETIME.TL:0,",",
                         IDEMAND_NONSPIN_ANCPOS(TSO_RRR,T):0:4/ ;
               );
            );
         );
      );

