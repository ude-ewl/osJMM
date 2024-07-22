* File: print-juha-OUT_UNITGROUPS_IN_CASE.inc

PUT UNITGROUPS_IN_CASE_OUT;
UNITGROUPS_IN_CASE_OUT.PW = 32767;
UNITGROUPS_IN_CASE_OUT.PS = 1;
UNITGROUPS_IN_CASE_OUT.nJ = 2;

*-------------------------------------------------------------------------------

if (INO_SOLVE EQ 1,
   PUT "UnitGroup,CaseName" /;

   LOOP (G,
     LOOP (CaseName,
          Put G.TL:0,",",CaseName.TL:0/ ;
      );
   );
);

*-------------------------------------------------------------------------------