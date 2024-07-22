* File: print-OUT_Change_MinCHP.inc

* This file will print the changes to MinCHP

PUT OUT_Change_Minchp_OUT;
OUT_Change_Minchp_Out.PW = 32000;
OUT_Change_Minchp_Out.PS = 1;
OUT_Change_Minchp_Out.nJ = 2;

if (INO_SOLVE EQ 1,
	PUT "CaseId, Area, UnitGroup, Basetime, Value" /;
    );

LOOP(CaseId,
*	LOOP(AAA,
* 		LOOP(G,
*  			LOOP(BASETIME,
*   				If(Change_minchp(AAA,G,BASETIME) NE 0,
        LOOP(IAGK_Y(AAA,G),
  			LOOP(BASETIME$(Change_minchp(AAA,G,BASETIME) NE 0),
    				put AAA.tl:0"."G.tl:0"."BASETIME.tl:0"="Change_minchp(AAA,G,BASETIME):0:4/ ;
   				);
  			);
*		);
*    );
);