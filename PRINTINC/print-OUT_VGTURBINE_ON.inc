* File: print-OUT_VGTURBINE_ON.inc

PUT VGTURBINE_ON_OUT;
VGTURBINE_ON_OUT.PW = 32767;
VGTURBINE_ON_OUT.PS = 1;
VGTURBINE_ON_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed


LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGELECSTORAGE),
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
         LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGELECSTORAGE,GId),
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VGTURBINE_ON.l(T,IA,IGELECSTORAGE) :0:4/ ;
                   );
            );
         );
      );
   );
);
