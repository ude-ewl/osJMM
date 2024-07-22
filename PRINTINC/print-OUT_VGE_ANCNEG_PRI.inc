* File: print-OUT_VGE_ANCNEG_PRI.inc

PUT VGE_ANCNEG_PRI_OUT;
VGE_ANCNEG_PRI_OUT.PW = 32767;
VGE_ANCNEG_PRI_OUT.PS = 1;
VGE_ANCNEG_PRI_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* Variables depending on AAA, GGG and Day
LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGSPINNING),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
               LOOP(A_AId(IA,AId),
                  LOOP(G_GId(IGSPINNING,GId),
                     if(VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)>0,
                        Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",VGE_ANCNEG_PRI.l(T,IA,IGSPINNING):0:4/ ;
                     );
                  );
               );
         );
      );
   );
);
