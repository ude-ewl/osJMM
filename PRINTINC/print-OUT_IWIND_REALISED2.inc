* File: print-OUT_IWIND_REALISED2.inc

* This file will print the root node value of the unregulated production
* equal to the wind power
* IWIND_REALISED2_IR depends on the sets RRR, Node and T

PUT OUT_IWIND_REALISED2_OUT;
OUT_IWIND_REALISED2_OUT.PW = 32767;
OUT_IWIND_REALISED2_OUT.PS = 1;
OUT_IWIND_REALISED2_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
         LOOP (IR,
              LOOP (R_RId(IR,RId),
                Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
               (IWIND_REALISED_IR(IR,T)):0:4/ ;
              );
         );

$ontext
         LOOP (IR_DET,
            LOOP (R_RId(IR_DET,RId),
                Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                IWIND_BID_IR(IR_DET,T):0:4/ ;
            );
         );
$offtext
      );
   );
);
