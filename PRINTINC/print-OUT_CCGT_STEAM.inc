* File: print-OUT_CCGT_STEAM.inc

PUT CCGT_STEAM_OUT;
CCGT_STEAM_OUT.PW = 32767;
CCGT_STEAM_OUT.PS = 1;
CCGT_STEAM_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,CCGT_Group,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
               LOOP(CCGT_Group,
                  if (CCGT_STEAM.l(T,CCGT_Group)>0,
                     Put CaseId.TL:0,",", CCGT_Group.TL:0,",",BASETIME.TL:0,",",
                     CCGT_STEAM.l(T,CCGT_Group) :0:4/ ;
                  );
               );
           );
     );
);