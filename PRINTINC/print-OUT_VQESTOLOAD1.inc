* File: print-OUT_VQESTOLOAD1.inc

PUT VQESTOLOAD1_OUT;
VQESTOLOAD1_OUT.PW = 32767;
VQESTOLOAD1_OUT.PS = 1;
VQESTOLOAD1_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,IPLUSMINUS,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

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
             LOOP(A_AId(IA,AId),
                if (((Card(IGELECSTORAGE)=0) and ((ORD(T)-1)=1)),
                   Put CaseId.TL:0,",","1",",",BASETIME.TL:0,"IPLUS,",
                   "0"/ ;
                );
             );
          );
          LOOP (IAGK_Y(IA,IGELECSTORAGE),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGELECSTORAGE,GId),
                  LOOP(IPLUSMINUS$VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'NODE_000',IPLUSMINUS),
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     IPLUSMINUS.TL:0,",",VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'NODE_000',IPLUSMINUS) :0:4/ ;
                  );
               );
            );
         );

     );
   );

);