* File: print-OUT_VGELEC_CONSUMED_DNEG_3h.inc

PUT VGELEC_CONSUMED_DNEG_3h_OUT;
VGELEC_CONSUMED_DNEG_3h_OUT.PW = 32767;
VGELEC_CONSUMED_DNEG_3h_OUT.PS = 1;
VGELEC_CONSUMED_DNEG_3h_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,Infotime,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
          LOOP (IA,
             LOOP(A_AId(IA,AId),
                if (((Card(IGESTORAGE_DSM)=0) and ((ORD(T)-1)=1)),
                   Put CaseId.TL:0,",","1",",",INFOTIME.TL:0,",",BASETIME.TL:0,",",
                   "0"/ ;
                );
             );
          );
          LOOP (IAGK_Y(IA,IGESTORAGE_DSM),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGESTORAGE_DSM,GId),
                  if(VGELEC_CONSUMED_DNEG_T.l(T,IA,IGESTORAGE_DSM)>0.0001,
                     Put CaseId.TL:0,",",GId.TL:0,",",INFOTIME.TL:0,",",BASETIME.TL:0,",",
                     VGELEC_CONSUMED_DNEG_T.l(T,IA,IGESTORAGE_DSM) :0:4/ ;
                  );
               );
            );
         );

          LOOP (IAGK_Y(IA,IGHydroRes),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGHydroRes,GId),
                  if(VGELEC_CONSUMED_DNEG_T.l(T,IA,IGHydroRes)>0.0001,
                     Put CaseId.TL:0,",",GId.TL:0,",",INFOTIME.TL:0,",",BASETIME.TL:0,",",
                     VGELEC_CONSUMED_DNEG_T.l(T,IA,IGHydroRes) :0:4/ ;
                  );
               );
            );
         );

     );
   );

);