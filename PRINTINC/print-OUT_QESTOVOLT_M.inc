* File: print-OUT_QESTOVOLT_M.inc

* This file will print the marginal value of equation QESTOVOLT, which is
* interpreted as marginal value of increasing the stored energy in the electricity storage.
* QESTOVOLT_M depends on the sets T,AAA,GGG

PUT OUT_QESTOVOLT_M_OUT;
OUT_QESTOVOLT_M_OUT.PW = 32767;
OUT_QESTOVOLT_M_OUT.PS = 1;
OUT_QESTOVOLT_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,UnitGroupId,BaseTime,Value" /;
);

$ontext
* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAHYDRO_ELECSTO,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
                  Put CaseId.TL:0,",",IAHYDRO_ELECSTO.TL:0,",",BASETIME.TL:0,",",
                  QESTOVOLT.M(T,IAHYDRO_ELECSTO):0:4/ ;
               );
      );
   );
);
$offtext



* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGELECSTORAGE),
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
*         LOOP (T  $((ORD(T)-1) = IDURATION_STAGE),
          LOOP (T  $IENDTIME_SHADOWPRICE(T),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGELECSTORAGE,GId),
                  Put CaseId.TL:0,",",AId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                  QESTOVOLT.M(T,IA,IGELECSTORAGE):0:4/ ;
               );
            );
         );
      );
   );
);