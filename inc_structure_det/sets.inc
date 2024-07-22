* File Sets.inc
*------------------------------------------------------------------------------

* This file contains all the sets that have their members explicitly declared.
* (Other sects are declared in the *.gms file.)

*------------------------------------------------------------------------------
* Part 1: Base sets.
*------------------------------------------------------------------------------

* Sets related to geography:

* Countries:
SET C  'Countries in simulation'
/
$INCLUDE "%data_path_in%inc_database\O SET C.inc";
/;

* Regions:
SET RRR  'Regions in simulation'
/
$INCLUDE "%data_path_in%inc_database\O SET R.inc";
/;

* Regions (zones) of TSOs (in German: Regelzone): note that TSO_RRR is not a subset of RRR
SET TSO_RRR  'regions of TSOs in simulation'
/
$INCLUDE "%data_path_in%inc_database\O SET ResR.inc";
/;

Set R_ANC_INC(TSO_RRR);
$ifi '%FlexCorrecEU%' == NO   R_ANC_INC(TSO_RRR)=NO;
$ifi '%FlexCorrecEU%' == YES  R_ANC_INC(TSO_RRR)=YES;

* Regions for which Wind Forecasts are provided in O Par Base Wind_Var_NT.inc

SET RRR_WindForecast(RRR) 'Regions for which wind forecast (updates) are provided'
/
$ifi '%UseWindForecast%' == Yes $INCLUDE "inc_database/O Set R_WindForecast.inc";
/;

SET INTC(RRR) 'Regions with NTC restriction on ingoing and outgoing lines'
/
$ifi '%GroupNTC%' == Yes $INCLUDE "%data_path_in%inc_database\O Set R_NTC.inc";
/;

SET INTCLINES(INTC,RRR) 'Lines with NTC restriction (listing only in outgoing direction)'
/
$ifi '%GroupNTC%' == Yes $INCLUDE "%data_path_in%inc_database\O Set RX_NTC.inc";
/;

* Areas:
SET AAA  'All areas'
/
$INCLUDE "%data_path_in%inc_database\O SET A.inc";
/;

$ifi '%LFB%' ==  Yes SET TRL  'Transmission Lines'
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Set Transmission Lines.csv";
$ifi '%LFB%' ==  Yes /;

$ifi '%LFB%' ==  Yes SET FLG(TRL)  'Flowgates (for flow-based market coupling)'
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Set Flowgates.csv";
$ifi '%LFB%' ==  Yes /;


$ifi '%LFB%' ==  Yes SET NO_FLG(TRL)  'All Transmission Lines not being Flowgates'
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Set No_Flowgates.csv";
$ifi '%LFB%' ==  Yes /;


* Areas for which CHP heat extraction or resultant el. restrictions are given exogeneously:
SET AAA_CHP_Exo(AAA)  'Areas for which CHP heat extraction or resultant el. restrictions are given exogeneously'
/
$ifi '%CHP%'==YES $INCLUDE "%data_path_in%inc_database\O SET A_CHP_Exo.inc";
/
;

* Regions with Flow-Based Market Coupling:
SET RRR_FLBMC(RRR)  'Regions with Flow-Based Market Coupling'
/
$ifi '%LFB%' ==  Yes  $INCLUDE "%data_path_in%inc_database\O SET Regions_FLBMC.inc";
/;

* Regions between which exchange is possible through Flow-Based Market Coupling:
SET RRR_RRR_FLBMC(RRR,RRR)        'Regions between which exchange is possible through Flow-Based Market Coupling'
/
$ifi '%LFB%' ==  Yes  $INCLUDE "%data_path_in%inc_database\O SET RRR_RRR_FLBMC.inc";
/;

$ifi '%LFB%' ==  Yes SET GRIDLOADCASE
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Set GRIDLOADCASE.csv";
$ifi '%LFB%' ==  Yes /;

* DC Lines considered in evolved Flow-Based Market Coupling:
$ifi '%LFB%' ==  Yes SET DCEFB  'DC line that is part of PTDF (evolved FBMC)'
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes  $INCLUDE "%data_path_in%inc_database\O Set DCEFB.csv";
$ifi '%LFB%' ==  Yes /;

* DC Line - Region connerction for DC Lines considered in evolved Flow-Based Market Coupling:
$ifi '%LFB%' ==  Yes SET DCEFB_R(DCEFB,RRR)  'Virtual Regions that belong to one DC line that is part of PTDF (evolved FBMC)'
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $ondelim
$ifi '%LFB%' ==  Yes  $INCLUDE "%data_path_in%inc_database\O Set DCEFB_R.csv";
$ifi '%LFB%' ==  Yes $offdelim
$ifi '%LFB%' ==  Yes /;

* Assignments of regions to countries:
SET CCCRRR(C,RRR)  'Regions in countries'
/
$INCLUDE "%data_path_in%inc_database\O SET CR.inc";
/;

* Assignments of regions to zones:
SET TSO_RRRRRR(TSO_RRR,RRR)  'Regions in TSO-regions'
/
$INCLUDE "%data_path_in%inc_database\O Set ResRR.inc";
/;

* Assignments of areas to regions:
SET RRRAAA(RRR,AAA)  'Areas in region'
/
$INCLUDE "%data_path_in%inc_database\O SET RA.inc";
/;

*------------------------------------------------------------------------------
* Sets related to time:

SET YYY  'All years'
/2000 *
2070
/;

SET TTT  'All time periods within one optimisation loop'
/
TM24
TM23
TM22
TM21
TM20
TM19
TM18
TM17
TM16
TM15
TM14
TM13
TM12
TM11
TM10
TM09
TM08
TM07
TM06
TM05
TM04
TM03
TM02
TM01
T00*T36
$ifi '%Looping%' == week T37*T180
/;

SET BASETIME   'Hourly time step'
/
$INCLUDE "%data_path_in%inc_database\O Set BASETIME.inc";
/;

SET INFOTIME(BASETIME)       'Start time of one rolling planning run'
/
$ifi '%Code_version%'==UENB $INCLUDE "%data_path_in%inc_database\O Set INFOTIME.inc";

$ifi '%Looping%' == week $goto InfotimeWeek
$ifi '%Looping%' == 3hours $goto Infotime3hours
$ifi '%Code_version%'==EWL $INCLUDE "%data_path_in%inc_database\O Set INFOTIME.inc";
$goto InfotimeEnd

$label InfotimeWeek
$ifi '%Code_version%'==EWL $INCLUDE "%data_path_in%inc_database\O Set InfoTime 168h.inc";
$goto InfotimeEnd

$label Infotime3hours
$ifi '%Code_version%'==EWL $INCLUDE "%data_path_in%inc_database\O Set InfoTime 3h.inc";

$label InfotimeEnd
/;


SET BASEDAY       'DAYS'
/
$INCLUDE "%data_path_in%inc_database\O Set BASEDATE.inc";
/;


SET DAY
/
DAY1
DAY2
/;

SET WEEKS    'Week numbers for one year'
/ W01 * W53
/;

SET BASETIME_WEEK(BASETIME,WEEKS)   'Connection between BASETIME og WEEKS'
/
$INCLUDE "%data_path_in%inc_database\O SET BaseTime_WWW.inc";
/;

SET BASETIME_BASEDAY(BASETIME,BASEDAY)   'Connection between BASETIME and BASEDAY'
/
$INCLUDE "%data_path_in%inc_database\O SET BaseTime_DDD.inc";
/;

SET Y(YYY)  'Years in the simulation'
/
$INCLUDE "%data_path_in%inc_database\O SET Y.inc";
/;

SET T(TTT)
/
T00*T36
$ifi '%Looping%' == week T37*T180
/;


SET T_WITH_HIST(TTT)
/
TM24
TM23
TM22
TM21
TM20
TM19
TM18
TM17
TM16
TM15
TM14
TM13
TM12
TM11
TM10
TM09
TM08
TM07
TM06
TM05
TM04
TM03
TM02
TM01
T00*T36
$ifi '%Looping%' == week T37*T180
/;

* sets of time steps used to define IT_SPOTPERIOD for day-ahead bidding
SET DYN_DAY_AHEAD(T)
/
$ifi NOT '%Looping%' == week T13*T36
$ifi '%Looping%' == week T01*T180
/;

SET DYN_DAY_AHEAD_FIRST_SOLVE(T)
/
T01*T36
$ifi '%Looping%' == week T37*T180
/;

$ifi '%Looping%' == week SET DYN_DAY_AHEAD_LAST_SOLVE(T)
$ifi '%Looping%' == week /
$ifi '%Looping%' == week T01*T48
$ifi '%Looping%' == week /;

SET T_INTRADAY(T)
/
$ifi NOT '%Looping%' == week T01*T24
/;

*------------------------------------------------------------------------------
* Sets related to technology:

SET G  'Generation technologies in simulation'
/
$INCLUDE "%data_path_in%inc_database\O SET G.inc";
/;

* Note that there are still some files in inc_database which are unused (unchecked):
* GDMaxSpinResTer, GDSTARTUPFUELCONSwarm, GDSTARTUPFUELCONScold
* GDMaxSpinDown, GDMaxSpinResPri_MIP, GDMaxSpinUp

SET GDATASET  'Generation technology data'
/ GDTYPE            'Generation type'
  GDCV              'Cb-value (CHP-Ext) '
  GDCB              'Cb-value (CHP-Ext) '
  GDFUEL            'Fuel type'
  GDFE_SECTION      'Fuel efficiency : section of line of fuel consumption function'
  GDFE_SLOPE        'Fuel efficiency : slope of line of fuel consumption function'
  GDFUELTAX         'Tax for fuels used in either heat and/or elec production'
  GDFUELTAXRED      'Tax reduction for fuels used in either heat and/or elec production'
  GDLOADEFF         'Efficiency of storage when loading'
  GDUNLOADEFF       'Efficiency of storage when unloading'
  GDSTOLOSS         'Losses of storages'
  GDOMVCOST         'Variable operating and maintenance costs (Euro/MWh)(default value)'
  GDSTARTUPCOST     'Variable start-up costs excluding fuel costs (EUR/MWh)(default value)'
  GDSTARTUPFUEL     'Start up fuel type'
  GDMINLOADFACTOR   'Minimum load factor (represeting = maximum capacity/ minimum power'
  GDMINOPERATION    'Minimum operation time'
  GDMINSHUTDOWN     'Minimum shutdown time'
  GDLEADTIMEHOT     'start-up time from hot state'
  GDLEADTIMEWARM    'start-up time from warm state'
  GDLEADTIMECOLD    'start-up time from cold state'
  GDPARTLOAD        'Part load efficiency'
  GDFULLLOAD        'full load efficiency'
  GDRAMPUP          'Maximum ramp rate up per hour relatively to the output capacity of the default technology [1/h]'
  GDSTARTUPRAMP     'Maximum start-up ramp rate per hour relatively to the output capacity of the default technology [1/h]'
  GDDESO2           'Degree of desulphoring'

*$ifi %UnitCmin%==yes   GDSHUTDOWNCOST    'Variable shut-down costs (EUR/MW)'
$ifi %UnitCramp%==yes  GDUCDURD      'Unit commitment: duration of shut down process (hours)'
$ifi %UnitCramp%==yes  GDUCDURU      'Unit commitment: duration of start up process (hours)'
$ifi %UnitCramp%==yes  GDUCRAMPU     'Unit commitment: ramp-up limit (MW/h)'
$ifi %UnitCramp%==yes  GDUCRAMPD     'Unit commitment: ramp-down limit (MW/h)'

  GDUCSCCC      'Time to cool from shut-down to cold/warm (hours)'
  GDHOTWARM     'Time to cool from shut-down to warm (hours)'
  GDWARMCOLD    'Time to cool from warm to cold (hours)'
  GDFE_NSLOPES  'Linear Approximation of fuel function : number of incremental heat rate slopes'
  GDMINONL      'The contribution to the minimum number of online units constraint'
  GDRampDown
  GDMaxSpinResPri
  GDMaxSpinResSec
  GDSTARTUPFUELCONSHot  'Consumption of start up fuel (MWh fuel/MW started up)'
  GDCHP_SUBSIDY1        'German CHP-Subsidy for loads above 2 MW (modelled for capacities above 2 MW)'
  GDCHP_SUBSIDY2        'German additional CHP-Subsidy for loads between 0.25-2 MW (modelled for capacities above 0.25-2 MW)'
  GDCHP_SUBSIDY3        'German additional CHP-Subsidy for loads between 0.05-0.25 MW (modelled for capacities below 0.25 MW)'
$ifi '%FLEX_addon%' == YES GDShiftMinusDSM  'Max length of DSM-shift to the past'
$ifi '%FLEX_addon%' == YES GDShiftPlusDSM   'Max length of DSM-shift to the future'
$ifi '%FLEX_addon%' == YES GDRepeatDSM      'Max repeating time of DSM'
$ifi '%FLEX_addon%' == YES GDOffDSM         'Min idle time of DSM'
$ifi '%CCGT_Imp%' == Yes  CCGT_Flag_GT
$ifi '%CCGT_Imp%' == Yes  CCGT_Flag_ST
$ifi '%CCGT_Imp%' == Yes  CCGT_Flag_DB
/;



SET GSTARTSET  'Generation technology data'
/
GSELEC            'electricity production'
GSHEAT            'heat production'
GSONLINE          'online capacity'
GSOPTIME          'Operation time'
GSSDTIME          'Shut down time'
GSCONTENTSTORAGE  'Content of storages'
GSCONTENTHYDRORES 'Content of hydro reservoir'
GSSHADOWONLINE    'Shadow price of unit being online'
GSSHADOWCONTENT   'Shadow price of content of electricity and heat storages'
/;

*------------------------------------------------------------------------------
* Sets related to fuels:

* The set of all fuels:

SET FFF  'Fuels'
/
$INCLUDE "%data_path_in%inc_database\O SET FFF.inc";
/;

* The set of characteristics of fuels:

SET FUELDATASET  'Characteristics of fuels '
/ FDNB      'Fuel number for internal use'
  FDCO2     'CO2 emission of fuel kg/GJ'
  FDSO2     'SO2 emission of fuel kg/GJ'
/;

* Set matching basetime to gridloadcase (part of trans / LFB)
$ifi '%LFB%' ==  Yes SET BASETIME_GRIDLOADCASE(BASETIME,GRIDLOADCASE)
$ifi '%LFB%' ==  Yes /
$ifi '%LFB%' ==  Yes $ondelim
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Set BASETIME_GRIDLOADCASE.csv";
$ifi '%LFB%' ==  Yes $offdelim
$ifi '%LFB%' ==  Yes /;

*------------------------------------------------------------------------------
* Sets related to elastic demands:


* The elasticities are specified as steps relating quantities and prices:

* Definition of individual steps in the electricity demand function:

SET DEF  'Steps in elastic electricity demand'
/ DEF_DINF,DEF_D9,DEF_D8,DEF_D7,DEF_D6,DEF_D5,DEF_D4,DEF_D3,DEF_D2,DEF_D1
  DEF_U1,DEF_U2,DEF_U3,DEF_U4,DEF_U5,DEF_U6,DEF_U7,DEF_U8,DEF_U9,DEF_UINF
/;

* The elements  DEF_DINF and DEF_UINF  must be included in the sets.

* Definition of subsets of steps for regulation downwards (decreased demand)
* and upwards (increased demands) of electricity demand:

SET DEF_D(DEF)  'Downwards steps in elastic electricity demand'
/
$INCLUDE "%data_path_in%inc_database\O Set DEF_D.inc";
/;

SET DEF_U(DEF)  'Upwards steps in elastic electricity demand'
/
$INCLUDE "%data_path_in%inc_database\O Set DEF_U.inc";
/;

* If the above two sets are defined as
*   DEF_D(DEF)  / DEF_DINF /
*   DEF_U(DEF)  / DEF_UINF /
* then the intention is that demand is essentially inelastic.

*------------------------------------------------------------------------------
* Sets related to emission policies:

SET MPOLSET  'Emission and other policy data'
/ TAX_CO2  'CO2 emission tax (Euro/t CO2)'
  TAX_SO2  'SO2 emission tax (Euro/t SO2)'
  TAX_NOx  'NOx emission tax (Euro/kg NOx)'
/;

SET GONLINE(G) 'Technologies with costs associated with bringing and having capacity online'
/
$INCLUDE "%data_path_in%inc_database\O SET GONLINE.inc";
/;


SET GALWAYSRUNNING(G)  'Technologies with available capacity being always online'
/
$INCLUDE "%data_path_in%inc_database\O Set GALWAYSRUNNING.inc";
/;


SET GSPINNING(G)    'Technologies offering primary (spinning) reserves'
/
$INCLUDE "%data_path_in%inc_database\O SET GSPINNING.inc";
/;


SET GINFLEXIBLE(G)  'Technologies with online capacity being independant of the wind power scenario'
/
$INCLUDE "%data_path_in%inc_database\O SET GINFLEXIBLE.inc"
/;


SET GSLOPES(G)  'Technologies with more than one incremental heat rate slope'
/
$INCLUDE "%data_path_in%inc_database\O SET GSLOPES.inc"
/;


SET GONESLOPE(G)  'Technologies with one incremental heat rate slope'
/
$INCLUDE "%data_path_in%inc_database\O SET GONESLOPE.inc"
/;

$ifi '%CHP%' == Yes SET GCHP_FixQ(G)  'CHP units with fixed heat (calculated in another model)'
$ifi '%CHP%' == Yes /
$ifi '%CHP%' == Yes $INCLUDE "%data_path_in%inc_database\O SET GCHP_FixQ.inc";
$ifi '%CHP%' == Yes /;

$ifi '%CHP%' == Yes SET GONESLOPES_CHPTool(G)  'Technologies with more than one incremental heat rate slope';
$ifi '%CHP%' == Yes GONESLOPES_CHPTool(G) = (GONESLOPE(G))$GCHP_FixQ(G);

SET AG_CHP_fixQ_Y(AAA,G) 'Set of areas and technologies with fixed heat extraction';

$ifi '%CHP%' == Yes SET GCHP_BoundE(G)  'CHP units with upper and lower bound of electricity generation (calculated in another model)'
$ifi '%CHP%' == Yes /
$ifi '%CHP%' == Yes $INCLUDE "%data_path_in%inc_database\O SET GCHP_BoundE.inc";
$ifi '%CHP%' == Yes $ifi '%data_source%'==  Access $INCLUDE "%data_path_in%inc_database\O SET GCHP_BoundE2.inc";
$ifi '%CHP%' == Yes /;

* Set used to approximate the non linear fuel function, stands for the different intervalls

SET IHRS 'set of incremental heat rate slopes'
/IHRS01*IHRS10 /;

*Sets related to CCGT
$ifi '%CCGT_Imp%' == Yes  SET CCGT_Group 'CCGT_groups that consists of/connect GT and ST'
$ifi '%CCGT_Imp%' == Yes  /
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%data_path_in%inc_database\O SET CCGT_Group.inc"
$ifi '%CCGT_Imp%' == Yes  /;

$ifi '%CCGT_Imp%' == Yes  Set CCGT_Group_G(CCGT_Group, G) 'assignment of Units to CCGT_Groups'
$ifi '%CCGT_Imp%' == Yes  /
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%data_path_in%inc_database\O Set CCGT_Group_G.inc"
$ifi '%CCGT_Imp%' == Yes  /;

$ifi '%CCGT_Imp%' == Yes  Set CCGT_GT(G) 'Gasturbines that are part of CCGT';
$ifi '%CCGT_Imp%' == Yes  Set CCGT_ST(G)'Steamturbines that are part of CCGT';
$ifi '%CCGT_Imp%' == Yes  Set CCGT_DB(G)'Duct burner that are part of CCGT';


* Sets for areas with hydro supply function
$ifi '%HydroSupplyCurves%' == Yes  SET AAA_HYDRORES_STEP(AAA)  'Reservoir areas with hydro supply function'
$ifi '%HydroSupplyCurves%' == Yes  /
$ifi '%HydroSupplyCurves%' == Yes  $INCLUDE "inc_database/O SET A_HYDRORES_STEP.inc";
$ifi '%HydroSupplyCurves%' == Yes  /