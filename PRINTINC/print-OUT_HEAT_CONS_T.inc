* File: print-OUT_HEAT_CONS_T.inc

* This file will print the realised consumption of electricity.
* HEAT_CONS_T depends on the sets RRR and T

PUT OUT_HEAT_CONS_T_OUT;
OUT_HEAT_CONS_T_OUT.PW = 32767;
OUT_HEAT_CONS_T_OUT.PS = 1;
OUT_HEAT_CONS_T_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaID,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP(IA,
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
         LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
           LOOP (A_AId(IA,AId),
            HELPVAR1= IDEMANDHEAT(IA,T)/(1-DISLOSS_H(IA));
            Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",",HELPVAR1:0:4/ ;
           );
         );
     );
   );
);


