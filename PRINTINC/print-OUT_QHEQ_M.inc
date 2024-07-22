* File: print-OUT_QHEQ_M.inc

* This file will print the marginal values of the equations QHEQ,
* which is understood as the heat price
* QHEQ.M depends on the sets AAA, GGG, Node and T

PUT OUT_QHEQ_M_OUT;
OUT_QHEQ_M_OUT.PW = 32767;
OUT_QHEQ_M_OUT.PS = 1;
OUT_QHEQ_M_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
$ontext
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE1') )
                    AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE1')) ),
      LOOP (T  $( ((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1)) AND
                    ((ORD(T)-1) <= IDURATION_STAGE('STAGE1'))  ),
$offtext
         LOOP (IA,
            LOOP (A_AId(IA,AId),
               Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",",
               QHEQ.M(T,IA):0:4/;
            );
         );
      );
   );
);
