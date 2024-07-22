* File: print-OUT_TechnologyData.inc

* This file will print the values of technology data for each unit group, such as
* the fuel type, the technology type, the variable costs
* Technology Data depends on the sets AAA and GGG

PUT OUT_TechnologyData_OUT;
OUT_TechnologyData_OUT.PW = 32767;
OUT_TechnologyData_OUT.PS = 1;
OUT_TechnologyData_OUT.nJ = 2;



if (INO_SOLVE EQ 1,
   PUT "CaseName,Area,UnitGroup,Parameter,Value" /;
);

* -----------------------------------------------------------------------------
if (INO_SOLVE EQ 1,
   LOOP(CaseName,
      LOOP ((IA,G,GDATASET)$GDATA(IA,G,GDATASET),
          PUT CaseName.TL:0,",",IA.TL:0,",",G.TL:0,",",GDATASET.TL:0,",",GDATA(IA,G,GDATASET):0:4/;
      );
   );
);