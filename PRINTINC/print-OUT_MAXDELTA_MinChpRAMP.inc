* File: print-OUT_MAXDELTA_MinChpRAMP.inc

* This file will print the changes to MaxDelta_MinChp_RAMP

PUT OUT_MAXDELTA_MinChpRAMP_OUT;
OUT_MAXDELTA_MinChpRAMP_Out.PW = 32000;
OUT_MAXDELTA_MinChpRAMP_Out.PS = 1;
OUT_MAXDELTA_MinChpRAMP_Out.nJ = 2;

if (INO_SOLVE EQ 1,
        PUT "CaseId, Area, UnitGroup, Basetime, Value" /;
    );

LOOP(CaseId,

        LOOP(IAGK_Y(AAA,G),
                        LOOP(BASETIME$(MaxDelta_MinChp_RAMP(AAA,G,BASETIME) NE 0),
                                put AAA.tl:0"."G.tl:0"."BASETIME.tl:0"="MaxDelta_MinChp_RAMP(AAA,G,BASETIME):0:4/ ;
                                );
                        );
);
