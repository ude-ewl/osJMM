* File: print-OUT_VGONLINE.inc

PUT VGONLINE_OUT;
VGONLINE_OUT.PW = 32767;
VGONLINE_OUT.PS = 1;
VGONLINE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

*-------------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

* set vgonline for hydropower regulating equal to the available capacity of hydropower

LOOP (CaseId,
$ontext
   LOOP (IAGK_Y(IA,IGHYDRORES),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGHYDRORES,GId),
                  Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                  (IGELECCAPACITY_Y(IA,IGHYDRORES) * IGKDERATE(IA,IGHYDRORES,T) * (1-IG_UNP_OUTAGE(IA,IGHYDRORES,T))):0:4/ ;
               );
            );
        );
      );
   );
$offtext

   LOOP (IAGK_Y(IA,IGUC),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGUC,GId),
                  if (vgonline_t.l(T,IA,IGUC)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     vgonline_t.l(T,IA,IGUC) :0:4/ ;
                  );
               );
           );
        );
      );
   );

   LOOP (IAGK_Y(IA,IGALWAYSRUNNING)$ (Not IGUC(IGALWAYSRUNNING)),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGALWAYSRUNNING,GId),
                  if (vgonline_t.l(T,IA,IGALWAYSRUNNING)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     vgonline_t.l(T,IA,IGALWAYSRUNNING) :0:4/ ;
                  );
               );
           );
        );
      );
   );


);