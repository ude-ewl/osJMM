* File: print-OUT_ISDP_HYDRORES.inc

* This file will print the water value from the Long-term model
* ISDP_HYDRORES depends on the sets RRR and T

PUT OUT_ISDP_HYDRORES_OUT;
OUT_ISDP_HYDRORES_OUT.PW = 32767;
OUT_ISDP_HYDRORES_OUT.PS = 1;
OUT_ISDP_HYDRORES_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Area,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IAHYDRO,
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
         LOOP (T $((ORD(T)-1) = INO_HOURS_OPT_PERIOD),
            LOOP (A_AId(IAHYDRO,AId),
               Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",",ISDP_HYDRORES(IAHYDRO,T):0:4/;
            );
         );
      );
  );
);