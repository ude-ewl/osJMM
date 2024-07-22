* File: print-OUT_IGELECCAPEFF.inc

PUT OUT_IGELECCAPEFF_OUT;
OUT_IGELECCAPEFF_OUT.PW = 32767;
OUT_IGELECCAPEFF_OUT.PS = 1;
OUT_IGELECCAPEFF_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
LOOP(CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                    AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
         LOOP ((IGELEC,IA)$IGELECCAPEFF(IA,IGELEC,T),
            LOOP (G_GID(IGELEC,GId),
               PUT CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",IGELECCAPEFF(IA,IGELEC,T):0:4/;
            );
         );
      );
   );
);

