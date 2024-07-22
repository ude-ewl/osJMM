* File basevar.inc
*------------------------------------------------------------------------------

* The time series are expressed as absolute scaled values, i.e. no scaling
* of these values take place in the Joint Market model.

*------------------------------------------------------------------------------
* Parameter  BASE_WEEKDAY_VAR_INFOTIME(INFOTIME) stands for the weekday of the infotime
* infotime is the time the the rolling planning is started
* 1 stands for monday, 2 for tuesday,..., 6 for saturday  7 for sunday

PARAMETER BASE_WEEKDAY_VAR_INFOTIME(INFOTIME)
/
$ifi not '%Looping%' == week $INCLUDE "%data_path_in%inc_database\O Par INFOTIME_WEEKDAY.inc";
/;

* PARAMETER BASE_DE_VAR_T contains the hourly, time series for the electricity
* demand in absolute, scaled values
* Units: MWh

PARAMETER BASE_DE_VAR_T(RRR,BASETIME)  'Electricity demand'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DE_VAR Det.inc";
/;

PARAMETER BASE_DE_VAR_EV_T(RRR,BASETIME)  'Electricity demand only for dumb charging of electric vehicles'
/
$ifi %EWL_EMOB% == YES $INCLUDE "%data_path_in%inc_database\O Par BASE_DE_VAR_EV Det.inc";
/;

BASE_DE_VAR_T(RRR,BASETIME)= BASE_DE_VAR_T(RRR,BASETIME) + BASE_DE_VAR_EV_T(RRR,BASETIME);

* PARAMETER Demand_EV contains the hourly, time series of the electric vehicle demand.
* Units: MW.

$ifi '%FLEX_addon%' == YES                PARAMETER Demand_EV(AAA,BASETIME)  'Demand of electric vehicles per hour'
$ifi '%FLEX_addon%' == YES                /
$ifi '%FLEX_addon%' == YES                $include "%data_path_in%inc_database\O Par Base_DE_Var_Demand_EV.inc";
$ifi '%FLEX_addon%' == YES                /;

* Demand for positive minute reserve due to outages of power plants.
* Used in deterministic runs
PARAMETER BASE_DEMAND_NONSPIN_ANCPOS_N_1(TSO_RRR) 'Demand of positive minute reserve due to N-1 criteria'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DEMAND_NONSPIN_ANCPOS_N_1.inc";
/;

* Demand for negative minute reserve
* Used in deterministic runs
PARAMETER BASE_DEMAND_NONSPIN_ANCNEG_N_1(TSO_RRR) 'Demand of negative minute reserve due to N-1 criteria'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DEMAND_NONSPIN_ANCNEG_N_1.inc";
/;


* PARAMETER BASE_DH_VAR_T contains the hourly, time series of the heat demand.
* Units: MW.
PARAMETER  BASE_DH_VAR_T(AAA,BASETIME)  'Variation in heat demand'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DH_VAR_T 1.inc";
$INCLUDE "%data_path_in%inc_database\O Par BASE_DH_VAR_T 2.inc";
/;

* PARAMETER BASE_DH_FRAC_Natgas_Min contains the minimum thermal load fraction of the aggregation gas driven heat producers (at peak heat demand) in each area.
* Units: -.
PARAMETER  BASE_DH_FRAC_Natgas_Min(AAA)  'Minimum thermal load fraction of NATGAS heat producers'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DH_FRAC_Natgas_min.inc";
/;

* PARAMETER BASE_INFLOWHYDRORES_VAR_T contains the hourly time series of
* water inflow to the reservoirs. For Germany the hourly values are constructed
* from daily data. For the Nordic countries the hourly values are constructed by
* distributing weekly values evenly on all hours within the week.
* Unit: (MWh)
PARAMETER BASE_INFLOWHYDRORES_VAR_T(AAA,BASETIME)  'Variation of the water inflow to reservoirs'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_INFLOWHYDRORES_VAR_T.inc";
/
;

* PARAMETER BASE_RUNRIVER_VAR_T contains the hourly time series of
* the hydro generation from hydro run-of-river generation technologies for each region.
* For Germany the hourly values are constructed
* from daily data. For the Nordic countries the hourly values are constructed by
* distributing weekly values evenly on all hours within the week.
* Units: (MWh).

PARAMETER BASE_RUNRIVER_VAR_T(AAA,BASETIME)    'Variation of generation of run-of-river'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_RUNRIVER_VAR_T.inc";
$INCLUDE "%data_path_in%inc_database\O Par BASE_RUNRIVER_VAR_T_1.inc";
/;

PARAMETER BASE_RESLEVELS_T(AAA,BASETIME)      'Historical average reservoir levels for hydro power'

$ifi '%Code_version%'==EWL  /
$ifi '%Code_version%'==EWL $INCLUDE "%data_path_in%inc_database\O Par BASE_RESLEVELS_T.inc";
$ifi '%Code_version%'==EWL  /;

$ifi '%Code_version%'==UENB $ifi not '%Looping%' == week /
$ifi '%Code_version%'==UENB $ifi not '%Looping%' == week $INCLUDE "%data_path_in%inc_database\O Par BASE_RESLEVELS_T.inc";
$ifi '%Code_version%'==UENB $ifi not '%Looping%' == week /;

$ifi '%Code_version%'==UENB $ifi '%Looping%' == week PARAMETER BASE_RESLEVELS_T2(AAA,BASETIME)      'Historical average reservoir levels for hydro power'
$ifi '%Code_version%'==UENB $ifi '%Looping%' == week /
$ifi '%Code_version%'==UENB $ifi '%Looping%' == week $INCLUDE "%data_path_in%inc_database\O Par BASE_RESLEVELS_T2.inc";
$ifi '%Code_version%'==UENB $ifi '%Looping%' == week /;

$ifi '%Code_version%'==UENB $ifi '%Looping%' == week Base_Reslevels_T(AAA,Basetime)$(ord(Basetime) ne Card(Basetime)) = Base_Reslevels_T2(AAA,Basetime+1);
$ifi '%Code_version%'==UENB $ifi '%Looping%' == week Base_Reslevels_T(AAA,Basetime)$(ord(Basetime) eq Card(Basetime)) = Base_Reslevels_T2(AAA,Basetime);

* Fix TSO input data to match EWL code
$ifi '%Code_version%'==UENB Base_Reslevels_T2(AAA,Basetime) = Base_Reslevels_T2(AAA,Basetime) * 100;
$ifi '%Code_version%'==UENB Base_Reslevels_T(AAA,Basetime)  = Base_Reslevels_T(AAA,Basetime)  * 100;

* PARAMETER BASE_SOLAR_VAR_T contains the description of  variation
* of the solar power production.
* Units: MWh

PARAMETER BASE_SOLAR_VAR_T(RRR,BASETIME)  'Variation of the solar generation'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_SOLAR_VAR_T.inc";
/;


* PARAMETER BASE_GEOTHERMAL_VAR_T contains the description of  variation
* of the solar power production.
* Units: MWh

PARAMETER BASE_GEOTHERMAL_VAR_T(RRR,BASETIME)  'Variation of the solar generation'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_GEOTHERMAL_VAR_T.inc";
/;


* PARAMETER BASE_TIDALSTREAM_VAR_T contains the description of  variation
* of the tidal stream power production.
* Units: MWh

PARAMETER BASE_TIDALSTREAM_VAR_T(RRR,BASETIME)  'Variation of the tidal stream generation'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_TIDAL_VAR_T.inc";
/;

* PARAMETER BASE_WAVE_VAR_T contains the description of  variation
* of the wave power production.
* Units: MWh

PARAMETER BASE_WAVE_VAR_T(RRR,BASETIME)  'Variation of the wave power generation'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_WAVE_VAR_T.inc";
/;

PARAMETER BASE_SOLARTH_VAR_T(RRR,BASETIME)  'Variation of the solar generation'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_SOLARTH_VAR_T.inc";
/;

PARAMETER BASE_Biomass_VAR_T(RRR,BASETIME)  'Biomass injection'
/
$INCLUDE "%data_path_in%inc_database\O Par Base_Biomass_Var_T.inc";
/;

PARAMETER BASE_OthRes_VAR_T(RRR,BASETIME)  'OthRes injection'
/
$INCLUDE "%data_path_in%inc_database\O Par Base_OthRes_Var_T.inc";
/

* PARAMETER BASE_X3FX_VAR_T contains the hourly time series for the fixed
* exchange with third regions.
* Units: MWh

* Data source:

* A positive number indicates net export from the region in the set IR,
* a negative number indicates net import to the region in the set IR
* for the given time segment BASETIME.
* The values are seen from the country in the set IC,
* i.e., any losses are disregarded.

PARAMETER BASE_X3FX_VAR_T(RRR,BASETIME)  'Variation in exchange with 3. region'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_X3FX_VAR_T.inc";
$INCLUDE "%data_path_in%inc_database\O Par Base_Export_Bal_NotOptimised_Var_T.inc";
/;

PARAMETER BASE_IMPORT_FLAG(RRR,BASETIME)
/
$INCLUDE "%data_path_in%inc_database\O Par BASE IMPORT_FLAG.inc";
/;
$CALL printf "" >> "%data_path_in%inc_database\O Par BASE NoDumpInENTSO_FLAG.inc"
PARAMETER BASE_NoDumpInENTSOFlag(RRR,BASETIME)
/
$INCLUDE "%data_path_in%inc_database\O Par BASE NoDumpInENTSO_FLAG.inc";
/;

PARAMETER BASE_DE_ANCPOS_VAR_D(TSO_RRR,BASEDAY) 'demand for positive secondary reserve'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DE_ANC_POS_VAR_D.inc";
/;

PARAMETER BASE_DE_ANCNEG_VAR_D(TSO_RRR,BASEDAY) 'demand for negative secondary reserve'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_DE_ANC_NEG_VAR_D.inc";
/;

PARAMETER BASE_DE_ANCPOS_PRI_VAR_D(TSO_RRR,BASEDAY) 'demand for positive primary reserve'
/
$INCLUDE "%data_path_in%inc_database\O Par Base_DE_Anc_Pos_PRI_Var_D.inc";
/;

PARAMETER BASE_DE_ANCNEG_PRI_VAR_D(TSO_RRR,BASEDAY) 'demand for negative primary reserve'
/
$INCLUDE "%data_path_in%inc_database\O Par Base_DE_Anc_Neg_PRI_Var_D.inc";
/;

$ifi '%FlexCorrecEU%' == YES  BASE_DE_ANCPOS_VAR_D(R_ANC_INC,BASEDAY) = BASE_DE_ANCPOS_VAR_D(R_ANC_INC,BASEDAY)*1.1;
$ifi '%FlexCorrecEU%' == YES  BASE_DE_ANCNEG_VAR_D(R_ANC_INC,BASEDAY) = BASE_DE_ANCNEG_VAR_D(R_ANC_INC,BASEDAY)*1.1;
$ifi '%FlexCorrecEU%' == YES  BASE_DE_ANCPOS_PRI_VAR_D(R_ANC_INC,BASEDAY) = BASE_DE_ANCPOS_PRI_VAR_D(R_ANC_INC,BASEDAY)*1.1;
$ifi '%FlexCorrecEU%' == YES  BASE_DE_ANCNEG_PRI_VAR_D(R_ANC_INC,BASEDAY) = BASE_DE_ANCNEG_PRI_VAR_D(R_ANC_INC,BASEDAY)*1.1;

* PARAMETER  BASE_WIND_VAR_Det contains a deterministic version of the wind power
* scenario tree.

PARAMETER  BASE_WIND_VAR_Det(RRR,BASETIME)    'Deterministic wind power production'
/
$INCLUDE "%data_path_in%inc_database\O Par BASE_WIND_VAR Det.inc";
/;

PARAMETER BASE_WIND_VAR_NT(INFOTIME,RRR,TTT) 'Deterministic wind power forecast'
/
$ifi '%UseWindForecast%' == Yes $INCLUDE "inc_database/O Par Base_Wind_Var_NT.inc";
/;

* If unit 'G' in area 'A' has an unplanned outage  in time period 'T1' then let 'G','A','T1' BASE_UNP_OUTAGE_VAR_T ('G','A','T1')=1
* else  BASE_UNP_OUTAGE_VAR_T ('G','A','T1') =0;
PARAMETER BASE_UNP_OUTAGE_VAR_T (AAA,G,BASETIME) 'availability factor due to unplanned outages of units'
/
$INCLUDE "%data_path_in%inc_database\O Par Base_ForcedOutage_Var_T.inc";
/;

* In order to take into account Engpert Pelgrums Distributed Generation
PARAMETER BASE_DISGEN_VAR_T(RRR,BASETIME) 'fixed distributed generation'
/

$INCLUDE "%data_path_in%inc_database\O Par DisGen.inc";
/;

PARAMETER BASE_FIXQ(AAA,G,BASETIME)  'fixed heat generation (determined externally)'
/
$ifi '%CHP%' == Yes $INCLUDE "%data_path_in%inc_database\O Par FixQ.inc";
/;

$ifi '%Code_version%' == EWL PARAMETER BASE_BOUNDE_MAX(AAA,G,BASETIME)  '(reduced) available electric capacity (due to heat extraction from CHP)'
$ifi '%Code_version%' == EWL /
$ifi '%Code_version%' == EWL $ifi '%CHP%' == Yes $INCLUDE "inc_database/O Par BOUNDE_MAX.inc";
$ifi '%Code_version%' == EWL /;

$ifi '%Code_version%' == EWL PARAMETER BASE_BOUNDE_MIN(AAA,G,BASETIME)  'minimum electricity generation (due to heat extraction from CHP)'
$ifi '%Code_version%' == EWL /
$ifi '%Code_version%' == EWL $ifi '%CHP%' == Yes $INCLUDE "inc_database/O Par BOUNDE_MIN.inc";
$ifi '%Code_version%' == EWL /;

PARAMETER BASE_MinVGOnline_CHP_VAR_T(AAA,G,BASETIME)  'minimum capacity online due to heat extraction (determined externally)'
/
$ifi '%CHP%' == Yes $INCLUDE "%data_path_in%inc_database\O Par Min_CHP_VGONLINE.inc";
/;

*Data input from time series model (electrical min/max boundaries based on the EWL CHP tool
Parameter MINCHPFACTOR(AAA,G,BASETIME) 'Minimum electricity generation factor of CHP units (unit-less, fraction of nominal capacity)'
*this may include unit groups which represent a mix of condensing and CHP units (at least one CHP unit)

/
*for German CHP unit groups (without suffix)
$INCLUDE "%data_path_in%inc_database\O PAR GDATA MINCHPFACTOR.inc";

*for units outside of Germany
$ifi '%data_source%'=='Access' $INCLUDE "%data_path_in%inc_database\O PAR GDATA MINCHPFACTOR2.inc";
/;

Parameter MAXCHPFACTOR(AAA,G,BASETIME) 'Maximum electricity generation factor of CHP units (unit-less, fraction of nominal capacity)'
*this may include unit groups which represent a mix of condensing and CHP units (at least one CHP unit)
/

*for German CHP unit groups (without suffix)
$INCLUDE "%data_path_in%inc_database\O PAR GDATA MAXCHPFACTOR.inc";

*for units outside of Germany
$ifi '%data_source%'=='Access' $INCLUDE "%data_path_in%inc_database\O PAR GDATA MAXCHPFACTOR2.inc";
/;

Parameter MINCHPFACTOR_MinLoadCorr(AAA,G,BASETIME);
MINCHPFACTOR_MinLoadCorr(AAA,G,BASETIME) = MINCHPFACTOR(AAA,G,BASETIME);

Parameter Change_minchp(aAA,G,BASETIME);
Change_minchp(AAA,G,BASETIME) = MinchpFactor(AAA,G,BASETIME)-MinCHPFACTOR_MinLoadCorr(AAA,G,BASETIME);

Parameter MaxDelta_MinChp_RAMP(AAA,G,Basetime);
MaxDelta_MinChp_RAMP(AAA,G,Basetime)=0;
LOOP( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1) ),
 LOOP(  BASETIME$(ord(BASETIME) > 1),
   IF ( (MINCHPFACTOR(AAA,G,BASETIME) > 0) AND (MINCHPFACTOR(AAA,G,BASETIME-1)=0),
    MaxDelta_MinChp_RAMP(AAA,G,Basetime) = Max(MaxDelta_MinChp_RAMP(AAA,G,Basetime),(MINCHPFACTOR(AAA,G,BASETIME)-MINCHPFACTOR(AAA,G,BASETIME-1)) );
   );
 );
);


Parameter MaxDelta_MinChp(AAA,G);
MaxDelta_MinChp(AAA,G)=0;
LOOP( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1) ),
 LOOP(  BASETIME$(ord(BASETIME) > 1),
    MaxDelta_MinChp(AAA,G) = Max(MaxDelta_MinChp(AAA,G),(MINCHPFACTOR(AAA,G,BASETIME)-MINCHPFACTOR(AAA,G,BASETIME-1)) );
 );
);

Parameter MaxDelta_MinChp_T(AAA,G,BASETIME);
MaxDelta_MinChp_T(AAA,G,BASETIME)=0;
LOOP( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1) ),
 LOOP(  BASETIME$(ord(BASETIME) > 1),
    MaxDelta_MinChp_T(AAA,G,BASETIME) = Max(0,(MINCHPFACTOR(AAA,G,BASETIME)-MINCHPFACTOR(AAA,G,BASETIME-1)) );
 );
);

Parameter Base_PmPMax_Var_T(AAA,G,WEEKS) 'Maximal pumping'
/
$INCLUDE "%data_path_in%inc_database\O Par MaxPmP_Hydro.inc";
/;
LOOP ( (AAA,G)$( GDATA(AAA,G,'GDTYPE')eq 8) ,
 Loop(weeks,
  IF (Base_PmPMax_Var_T(AAA,G,WEEKS) eq 0,
      Base_PmPMax_Var_T(AAA,G,WEEKS) = 1;
  );
 );
);

Parameter Base_HYRESMAXGEN_Var_T(AAA,G,WEEKS) 'Maximal Generation of Hydroreservoirs'
/
$INCLUDE "%data_path_in%inc_database\O Par MaxGen_Hydro.inc";
/;
LOOP ((AAA,G) $( GDATA(AAA,G,'GDTYPE') eq  8) ,
 Loop(WEEKS,
  IF (Base_HYRESMAXGEN_Var_T(AAA,G,WEEKS) eq 0,
      Base_HYRESMAXGEN_Var_T(AAA,G,WEEKS) = 1;
  );
 );
);
*---TYNDP specific Hydro equations to meet weekly pumping restrictions--------*
$ifi '%looping%'=='week' Parameter Base_HYRESMAXPMP_E_Var_T(AAA,G,WEEKS) 'Maximal Generation of Hydroreservoirs'
$ifi '%looping%'=='week' /
$ifi '%looping%'=='week' $include "%data_path_in%inc_database\O Par MaxPMP_E_Hydro.inc";
$ifi '%looping%'=='week' /;
$ifi '%looping%'=='week' LOOP ((AAA,G) $( GDATA(AAA,G,'GDTYPE') eq  8) ,
$ifi '%looping%'=='week'  Loop(weeks,
$ifi '%looping%'=='week'   IF (Base_HYRESMAXPMP_E_Var_T(AAA,G,WEEKS) eq 0,
$ifi '%looping%'=='week'      Base_HYRESMAXPMP_E_Var_T(AAA,G,WEEKS) = 99999;
$ifi '%looping%'=='week'   );
$ifi '%looping%'=='week'  );
$ifi '%looping%'=='week' );

$ifi '%looping%'=='week' Parameter Base_HYRESMINPMP_E_Var_T(AAA,G,WEEKS) 'Maximal Generation of Hydroreservoirs'
$ifi '%looping%'=='week' /
$ifi '%looping%'=='week' $include "%data_path_in%inc_database\O Par MINPMP_E_Hydro.inc";
$ifi '%looping%'=='week' /;

* Required feasible Startup-ramp plus RampRate -------------------------------------------------
PARAMETER CHP_GKDERATE_VAR_T (AAA,G,BASETIME) 'availability factor due to planned revisions of units'
/
$INCLUDE "%data_path_in%inc_database\O Par GKDERATE.inc";
$ifi '%data_source%'=='Access' $INCLUDE "%data_path_in%inc_database\O Par GKDERATE2.inc";
/;

Parameter GKDERATERAMP(AAA,G,BASETIME);
GKDERATERAMP(AAA,G,BASETIME) = 0;
LOOP( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1) ),
 LOOP(  BASETIME$(ord(BASETIME) > 1),
    GKDERATERAMP(AAA,G,BASETIME) = Max(0,(CHP_GKDERATE_VAR_T(AAA,G,BASETIME)-CHP_GKDERATE_VAR_T(AAA,G,BASETIME-1)) );
 );
);

*STARTUP-RAMP
Parameter Startup_RAMP_CHP_T(AAA,G,BASETIME);
$ifi '%UnitCmin%' == YES Startup_RAMP_CHP_T(AAA,G,BASETIME)     = ( MAXDELTA_minchp_RAMP(AAA,G,BASETIME))$( (GKDERATERAMP(AAA,G,BASETIME)=1) OR (MAXCHPFACTOR(AAA,G,BASETIME-1)=0) );
$ifi not '%UnitCmin%' == YES Startup_RAMP_CHP_T(AAA,G,BASETIME) = Max( ((MinCHPFActor(AAA,G,BASETIME) -  CHP_GKDERATE_VAR_T(AAA,G,BASETIME-1))/GKDERATERAMP(AAA,G,BASETIME))$(GKDERATERAMP(AAA,G,BASETIME)>0),((MinCHPFActor(AAA,G,BASETIME) -  Maxchpfactor(AAA,G,BASETIME-1))/CHP_GKDERATE_VAR_T(AAA,G,BASETIME))$(CHP_GKDERATE_VAR_T(AAA,G,BASETIME)>0) );

Parameter RAMP_CHP_T(AAA,G,BASETIME);
RAMP_CHP_T(AAA,G,BASETIME)=(MAXDELTA_minchp_T(AAA,G,BASETIME))$(GKDERATERAMP(AAA,G,BASETIME)=0);

Parameter RAMP_CHP(AAA,G);
Parameter Startup_RAMP_CHP(AAA,G);
RAMP_CHP(AAA,G)=0;
Startup_RAMP_CHP(AAA,G)=0;
LOOP( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1) ),
 LOOP(  BASETIME$(ord(BASETIME) > 1),
    RAMP_CHP(AAA,G) = Max(RAMP_CHP(AAA,G),RAMP_CHP_T(AAA,G,BASETIME ));
    Startup_RAMP_CHP(AAA,G) = Max(Startup_RAMP_CHP(AAA,G),Startup_RAMP_CHP_T(AAA,G,BASETIME ));
  );
);

Parameter StartRamp_CHP(AAA,G,Basetime);
StartRamp_CHP(AAA,G,Basetime) = Max(GDATA(AAA,G,'GDSTARTUPRAMP'),Startup_RAMP_CHP_T(AAA,G,BASETIME));

$ifi '%UnitCmin%' == YES GDATA(AAA,G,'GDSTARTUPRAMP')= Max(GDATA(AAA,G,'GDSTARTUPRAMP'),Startup_RAMP_CHP(AAA,G));
$ifi '%UnitCmin%' == YES GDATA(AAA,G,'GDRAMPUP')     = Max(GDATA(AAA,G,'GDRAMPUP'),RAMP_CHP(AAA,G));

$ifi '%UnitCmin%' == YES GDATA(AAA,G,'GDSTARTUPRAMP')= (round( GDATA(AAA,G,'GDSTARTUPRAMP'),4) + 0.0001 )$(GDATA(AAA,G,'GDSTARTUPRAMP')>0);

Parameter Max_Delta_VGELEC_CHP(AAA,G,BASETIME);
Max_Delta_VGELEC_CHP(AAA,G,BASETIME) = CHP_GKDERATE_VAR_T(AAA,G,BASETIME);



$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_MEDIAN_RESLEVEL_ISDPHYDRORES(AAA,BASETIME) 'Long-term median filling level variation of hydro reservoir for OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par MEDIAN_RESLEVEL_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;


* Formerly PlugInStart(AAA,BASETIME) = BASE_EV_Leave_VAR_T(AAA,G,BASETIME)
PARAMETER BASE_EV_Leave_VAR_T(AAA,G,BASETIME)  'variation of electric vehicles which are leaving the charging system'
/
$ifi %EWL_EMOB% == YES $include "%data_path_in%inc_database\O Par Base_DE_VAR EV_Leave.inc";
/;
* Formerly Demand_EV(AAA,BASETIME) = BASE_EV_Arrive_VAR_T(AAA,G,BASETIME)
PARAMETER BASE_EV_Arrive_VAR_T(AAA,G,BASETIME)  'variation of electric vehicles which are arriving at the charging system'
/
$ifi %EWL_EMOB% == YES $include "%data_path_in%inc_database\O Par Base_DE_VAR EV_Arrive.inc";
/;

*Formerly PlugIn_SOC_ONEEV, only one parameter for SOC in leaving and arriving EVs
Parameter BASE_EV_SOC_Leave_T(AAA,G,BASETIME) 'state of charge (SOC) of the battery of one electric vehicle which leaves the charging system (in percentage)'
/
$ifi %EWL_EMOB% == YES $include "%data_path_in%inc_database\O Par BASE_EV_SOC_Leave_T.inc";
/;

*only one parameter for SOC in leaving and arriving EVs
Parameter BASE_EV_SOC_Arrive_T(AAA,G,BASETIME) 'state of charge (SOC) of the battery of one electric vehicle which arrives at the charging system (in percentage)'
/
$ifi %EWL_EMOB% == YES $include "%data_path_in%inc_database\O Par BASE_EV_SOC_Arrive_T.inc";
/;