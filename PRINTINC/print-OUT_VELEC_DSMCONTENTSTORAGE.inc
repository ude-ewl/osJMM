* File: print-OUT_VELEC_CONTENTSTORAGE.inc

PUT VELEC_DSMCONTENTSTORAGE_OUT;
VELEC_DSMCONTENTSTORAGE_OUT.PW = 32767;
VELEC_DSMCONTENTSTORAGE_OUT.PS = 1;
VELEC_DSMCONTENTSTORAGE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
          LOOP (IA,
             LOOP(A_AId(IA,AId),
                if ((((Card(IGELECSTODSM)=0)) and ((ORD(T)-1)=1)),
                   Put CaseId.TL:0,",","3",",",BASETIME.TL:0,",",
                   "0"/ ;
                );
             );
          );
          LOOP (IAGK_Y(IA,IGELECSTODSM),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGELECSTODSM,GId),
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VCONTENTSTORAGE_T.l(T,IA,IGELECSTODSM) :0:4/ ;
               );
            );
         );
     );
   );

);