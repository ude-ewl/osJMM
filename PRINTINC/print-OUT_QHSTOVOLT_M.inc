* File: print-OUT_QHSTOVOLT_M.inc

* This file will print the marginal value of equation QHSTOVOLT, which is
* interpreted as marginal value of increasing the stored energy in the heat storage.
* QHSTOVOLT_M depends on the sets AAA, GGG and T

PUT OUT_QHSTOVOLT_M_OUT;
OUT_QHSTOVOLT_M_OUT.PW = 32767;
OUT_QHSTOVOLT_M_OUT.PS = 1;
OUT_QHSTOVOLT_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGHEATSTORAGE),
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
*         LOOP (T  $((ORD(T)-1) = IDURATION_STAGE('STAGE1')),
          LOOP (T  $IENDTIME_SHADOWPRICE(T),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGHEATSTORAGE,GId),
                  Put CaseId.TL:0,",",AId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                  QHSTOVOLT.M(T,IA,IGHEATSTORAGE):0:4/ ;
               );
            );
         );
      );
   );
);

