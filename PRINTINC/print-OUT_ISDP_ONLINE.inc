* File: print-OUT_ISDP_ONLINE.inc

PUT ISDP_ONLINE_OUT;
ISDP_ONLINE_OUT.PW = 32767;
ISDP_ONLINE_OUT.PS = 1;
ISDP_ONLINE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, and Node. Only root node values are printed


LOOP (CaseId,
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
         LOOP (T $((ORD(T)-1) = NO_HOURS_OVERLAP),
          LOOP (IAGK_Y(IA,IGUC),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGUC,GId),
                  if(ISDP_ONLINE(IA,IGUC)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     ISDP_ONLINE(IA,IGUC) :0:4/ ;
                  );
               );
            );
         );
      );
   );
);