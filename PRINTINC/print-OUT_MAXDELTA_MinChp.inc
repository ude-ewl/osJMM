* File: print-OUT_MAXDELTA_MinChp.inc

* This file will print the changes to MAXDELTA_MinChp

PUT OUT_MAXDELTA_MinChp_OUT;
OUT_MAXDELTA_MinChp_Out.PW = 32000;
OUT_MAXDELTA_MinChp_Out.PS = 1;
OUT_MAXDELTA_MinChp_Out.nJ = 2;

if (INO_SOLVE EQ 1,
        PUT "CaseId, Area, UnitGroup, Value" /;
    );

LOOP(CaseId,
        LOOP(IAGK_Y(AAA,G)$(MAXDELTA_MinChp(AAA,G) NE 0),
                        put AAA.tl:0"."G.tl:0"="MAXDELTA_MinChp(AAA,G):0:4/ ;
                        );
    );

