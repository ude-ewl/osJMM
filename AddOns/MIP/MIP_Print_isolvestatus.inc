*--------------------------------------------------------------------------
* print error message if infeasibility in QESTOLOAD1

LOOP (IAGK_Y(IA,IGELECSTORAGE),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

$ontext
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE1') )
                        AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE3')) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME('STAGE1')+1)),
         LOOP(NODE $(INT(NODE,T)),
$offtext
            if(VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IMINUS')>0,
                 Put "Error QESTOLOAD1 is infeasible ";
                 Put "VQESTOLOAD1_IMINUS: ", VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECSTORAGE.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IPLUS')>0,
                 Put "Error QESTOLOAD1 is infeasible ";
                 Put "VQESTOLOAD1_IPLUS: ", VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECSTORAGE.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
*         );
      );
   );
);
