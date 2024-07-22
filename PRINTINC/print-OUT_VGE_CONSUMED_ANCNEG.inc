* File: print-OUT_VGE_CONSUMED_ANCNEG.inc

PUT VGE_CONSUMED_ANCNEG_OUT;
VGE_CONSUMED_ANCNEG_OUT.PW = 32767;
VGE_CONSUMED_ANCNEG_OUT.PS = 1;
VGE_CONSUMED_ANCNEG_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* Variables depending on AAA, GGG and Day
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
         LOOP (IA,
            LOOP(A_AId(IA,AId),
               if (((Card(IGESTORAGE_DSM)=0) and ((ORD(T)-1)=1)),
                  Put CaseId.TL:0,",1,",BASETIME.TL:0,",0"/ ;
              );
            );
         );
         LOOP (IAGK_Y(IA,IGSPINNING)$(IGSPINNING(IGSPINNING) OR IGHYDRORES(IGSPINNING)),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGSPINNING,GId),
                  if(VGE_CONSUMED_ANCNEG.l(T,IA,IGSPINNING)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VGE_CONSUMED_ANCNEG.l(T,IA,IGSPINNING) :0:4/ ;
                  );
               );
            );
         );
     );
   );
);
