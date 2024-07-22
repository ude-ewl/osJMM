* File: print-OUT_ISDP_HYDRORES_STEP.inc

* This file will print the water value for areas with hydro supply function from the Long-term model
* ISDP_HYDRORES_STEP depends on the sets AAA, G and T

PUT OUT_ISDP_HYDRORES_STEP_OUT;
OUT_ISDP_HYDRORES_STEP_OUT.PW = 32767;
OUT_ISDP_HYDRORES_STEP_OUT.PS = 1;
OUT_ISDP_HYDRORES_STEP_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Area,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAHYDRO_HYDRORES_STEP,
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
         LOOP (T $((ORD(T)-1) = INO_HOURS_OPT_PERIOD),
            LOOP (A_AId(IAHYDRO_HYDRORES_STEP,AId),
                 LOOP(G_GId(IGHYDRORES,GId) $IAGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES),
               Put CaseId.TL:0,",",AId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",ISDP_HYDRORES_STEP(IAHYDRO_HYDRORES_STEP,IGHYDRORES,T):0:4/;
               );
            );
         );
      );
  );
);


