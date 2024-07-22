* File: print-OUT_QGONLSTART_M.inc

* This file will print the marginal value of equation QGONLSTART, which is
* interpreted as the marginal value of increasing the online capacity.
* QGONLSTART_M depends on the sets AAA, GGG, Node and T

PUT OUT_QGONLSTART_M_OUT;
OUT_QGONLSTART_M_OUT.PW = 32767;
OUT_QGONLSTART_M_OUT.PS = 1;
OUT_QGONLSTART_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGUC),
$ontext
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIME('STAGE1')),
         LOOP (T  $((ORD(T)-1) = IDURATION_STAGE('STAGE1')),
$offtext
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
          LOOP (T  $IENDTIME_SHADOWPRICE(T),
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGUC,GId),
                  Put CaseId.TL:0,",",AId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                  QGONLSTART.M(T,IA,IGUC):0:4/ ;
               );
            );
         );
      );
   );
);