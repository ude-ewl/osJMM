* File: print-OUT_vgheat.inc

PUT vgheat_OUT;
vgheat_OUT.PW = 32767;
vgheat_OUT.PS = 1;
vgheat_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGHEAT),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGHEAT,GId),
                  if (vgheat_t.l(T,IA,IGHEAT)>0,
                     Put CaseId.TL:0,",", GId.TL:0,",",BASETIME.TL:0,",",
                     vgheat_t.l(T,IA,IGHEAT) :0:4/ ;
                  );
               );
            );
         );
     );
   );

);