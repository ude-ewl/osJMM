* File: print-OUT_VGSTARTUP.inc

PUT VGSTARTUP_OUT;
VGSTARTUP_OUT.PW = 32767;
VGSTARTUP_OUT.PS = 1;
VGSTARTUP_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGUC),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGUC,GId),
                  if (vgstartup_t.l(T,IA,IGUC)>0,
                     Put CaseId.TL:0,",", GId.TL:0,",",BASETIME.TL:0,",",
                     vgstartup_t.l(T,IA,IGUC) :0:4/ ;
                  );
               );
            );
         );
     );
   );
);
