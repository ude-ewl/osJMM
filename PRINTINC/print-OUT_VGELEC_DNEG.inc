* File: print-OUT_VGELEC_DNEG.inc

PUT VGELEC_DNEG_OUT;
VGELEC_DNEG_OUT.PW = 32767;
VGELEC_DNEG_OUT.PS = 1;
VGELEC_DNEG_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed


LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGDISPATCH)$(NOT IGPTG(IGDISPATCH)),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGELEC,GId),
                  if(VGELEC_DNEG_T.l(T,IA,IGELEC)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VGELEC_DNEG_T.l(T,IA,IGELEC) :0:4/ ;
                  );
               );
            );
         );
      );
   );
);
