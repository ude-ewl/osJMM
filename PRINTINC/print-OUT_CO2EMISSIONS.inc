* File: print-OUT_CO2Emissions.inc

* This file will print the CO2 emissions

PUT CO2Emissions_OUT;
CO2Emissions_OUT.PW = 1255;
CO2Emissions_OUT.PS = 1;
CO2Emissions_OUT.nJ = 2;

* Headline: Regions
if (INO_SOLVE EQ 1,
  Put "BaseTime";
   LOOP(IR, Put ",",IR.TL:0;);
);

   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
               Put BASETIME.TL:0;
* Standard fuel using technologies
          LOOP (IR,
                      Put ",",
                      SUM((IAGK_Y(IA,IGUSINGFUEL),FFF)$((GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
                          VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)*FDATA(FFF,'FDCO2')*CONVERT_KG_PER_GJ_T_PER_MWH
              ) :0:2;
          );

                );
      );
