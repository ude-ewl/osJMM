* File: print-OUT_VGSTEAM.inc

PUT VGSTEAM_OUT;
VGSTEAM_OUT.PW = 32767;
VGSTEAM_OUT.PS = 1;
VGSTEAM_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGDUCTBURNER),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGDUCTBURNER,GId),
                  if (VGSTEAM_T.l(T,IA,IGDUCTBURNER)>0,
                     Put CaseId.TL:0,",", GId.TL:0,",",BASETIME.TL:0,",",
                     VGSTEAM_T.l(T,IA,IGDUCTBURNER) :0:4/ ;
                  );
               );
            );
         );
     );
   );

);