* File: print-OUT_IGELECCAPACITY_Y.inc

PUT OUT_IGELECCAPACITY_Y_OUT;
OUT_IGELECCAPACITY_Y_OUT.PW = 32767;
OUT_IGELECCAPACITY_Y_OUT.PS = 1;
OUT_IGELECCAPACITY_Y_OUT.nJ = 2;



if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
if (IBID_DAYAHEADMARKET_YES EQ 1,
   LOOP(CaseId,
      LOOP ((IGELEC,IA)$IGELECCAPACITY_Y(IA,IGELEC),
         LOOP(BASETIME  $(ORD(BASETIME) = IBASESTARTTIME ),
            LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
               LOOP (G_GID(IGELEC,GId),
                  HELPVAR1= IGKDERATE(IA,IGELEC,T)*IGELECCAPACITY_Y(IA,IGELEC);
                  PUT CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",HELPVAR1:0:4/;
               );
            );
         );
      );
   );
);

