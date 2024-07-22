*Open source version (published by EWL)
$onlisting

*$LOG Creating "Environment Report"
*$SHOW
*$LOG
*$LOG Starting JMM-Model, MG: %mg_cur%
*$LOG Paths:
*$LOG   code_path: %code_path%
*$LOG   data_path_in: %data_path_in%
*$LOG   data_path_out: %data_path_out%
*$LOG
*$LOG debug mode: %debug%
*end version

* Preamble *{
* File JMM.gms

* Dynamic partial equilibrium model for the electricity and CHP sector
* Based on rolling planning approach

* Efforts have been made to make a good model and to improve it continously.
* However, the model may still be incomplete and subject to errors.

* Contributions by (incomplete list):
* Peter Meibom, Heike Brand, Christoph Weber, Ruediger Barth, Juergen Apfelbeck, Kerstin Daechert, Bjoern Felten, Francesco Hipp, Thomas Kallabis, Julian Radek, Marco S. Breder

$TITLE JMM2.1 (Mar 2024)

* Allows empty data statements for set, parameter or table data
$ONEMPTY

* -------------------------------------------------------------------------------
* Set path variables
* -------------------------------------------------------------------------------
*VersID_TSOs
*start version
$IFTHEN NOT SET code_path
$  SETGLOBAL code_path ../
$  LOG Option 'code_path' not set. Now set to default value '../' to mimic old TSO behaviour
$  ABORT code_path not set
$ENDIF

$IFTHEN NOT SET data_path_in
$  SETGLOBAL data_path_in ./
$  LOG Option 'data_path_in' not set. Now set to default value './' to mimic old TSO behaviour
$  ABORT data_path_in not set
$ENDIF

$IFTHEN NOT SET data_path_out
$  SETGLOBAL data_path_out ./
$  LOG Option 'data_path_out' not set. Now set to default value '../' to mimic old TSO behaviour
$  ABORT data_path_out not set
$ENDIF

$SETGLOBAL code_path_addons %code_path%model\AddOns\
$SETGLOBAL code_path_model %code_path%Model\
$SETGLOBAL code_path_printinc %code_path%model\Printinc\
*end version

*-------------------------------------------------------------------------------
* Choice.gms contains choices regarding the model run
*-------------------------------------------------------------------------------

$INCLUDE  '%data_path_in%Choice.gms'


*VersID_TSOs
*start version
$LOG   data_source: %data_source%

* -------------------------------------------------------------------------------
* Adjustments to parameters / sets depending on the data source
* -------------------------------------------------------------------------------
$SET data_source_valid YES
$LOG data_source: %data_source%
$LOG data_source_valid: %data_source_valid%
$IFI NOT %data_source% == EMMDa SET data_source_valid NO
$IFI NOT %data_source% == Access SET data_source_valid NO
$IFI %data_source_valid% == NO $ABORT "The variable data_source has to be either to 'EMMDa' or 'Access'"

$ifi %data_source% == Access $SETGLOBAL Sonstige_NEE Sonstige_NEE
$ifi %data_source% == EMMDa $SETGLOBAL Sonstige_NEE KWK_Z

* -------------------------------------------------------------------------------
* Initialize parameters and sets for generating the cplex.opt files
* -------------------------------------------------------------------------------
*if set to NA (not avaiable)
* --> variable Type: double dash variable
* --> scope type: scoped (not available in parent files)
* Dropping the double dash variable allows to refine it as type "global" (using setglobal).

$ifthen %opt_threads% == NA
$  drop opt_threads
$endif

$ifthen %opt_tilim% == NA
$  drop opt_tilim
$endif

$INCLUDE "%code_path_addons%/CplexOpt/cplex_opt_init.gms"

* -------------------------------------------------------------------------------
* create out dir and gdx_files if neccessary (and copy code
* -------------------------------------------------------------------------------
$LOG "Creating out directories:"
$LOG "%gams.sysdir%gbin\mkdir.exe -p %data_path_out%GDX_files"
$CALL '="%gams.sysdir%gbin\mkdir.exe" -p %data_path_out%GDX_files'

$INCLUDE "%code_path_addons%/CopyCode/copyCode.gms"

*

* -------------------------------------------------------------------------------
* Declaration of parameters and sets for input data consistency checks
*-------------------------------------------------------------------------------
set
IDCT 'input data check type' /MAXCHP, MINCHP, MINCHP_GKDERATE/
;

parameter
IINPUTDATA_ERRORCOUNT(IDCT)
;

IINPUTDATA_ERRORCOUNT(IDCT) =0;
*end version

*-------------------------------------------------------------------------------
* GAMS options are $included from the file WilmarGams.opt.
* Observe that in order to make them apply, the option $ONGLOBAL must first be set:
* The dollar control options set in the lower level include file are passed on to the parent file only if the $onglobal option is set

$ONGLOBAL

$INCLUDE '%code_path_model%/WilmarGams.opt'

*}

* Sets (Part I) *{

* Sets (external, included from data files) *{

$offlisting
$INCLUDE '%code_path_model%inc_structure_det\sets.inc';
$onlisting
*{ This file contains:

* sets related to geography
* SET C                            'Countries in simulation'
* SET RRR                          'Regions in simulation'
* SET TSO_RRR                      'Regions of TSOs in simulation'
* SET AAA                          'All areas'
* CHP: SET AAA_CHP_Exo(AAA)             'Areas for which CHP heat extraction or resulting el. restrictions are given exogeneously'
* SET CCCRRR(C,RRR)                'Regions in countries' (Assignments of regions to countries)
* SET TSO_RRRRRR(TSO_RRR,RRR)      'Regions in TSO-regions' (Assignments of regions to zones)
* SET RRRAAA(RRR,AAA)              'Areas in region' (Assignments of areas to regions)
* SET AAA_HYDRORES_STEP(AAA)       'Reservoir areas with hydro supply function'

* sets related to Group NTCs and Flow-Based Market Coupling (LFB)
* SET INTC(RRR)                    'Regions with NTC restriction on ingoing and outgoing lines'
* SET INTCLINES(INTC,RRR)          'Lines with NTC restriction (listing only in outgoing direction)'
* LFB: SET TRL                          'Transmission Lines'
* LFB: SET FLG(TRL)                     'Flowgates (for load flow based calculations)'
* LFB: SET NO_FLG(TRL)                  'All Transmission Lines not being Flowgates'
* LFB: SET RRR_FLBMC(RRR)               'Regions with Flow-Based Market Coupling'
* LFB: SET RRR_RRR_FLBMC(RRR,RRR)       'Regions between which exchange is possible through Flow-Based Market Coupling'

* sets related to time
* SET YYY                          'Total number of years in model (2000*2070)'
* SET TTT                          'Total number of time periods (All time periods within one optimisation loop: TM24*TM01, T00*T36)'
* SET BASETIME                     'Base time for loops (Hourly time step, i.e. 8760 time steps for one year, in the format Year-Month-Day-Hour, e.g. 2012-01-01-00)'
* SET INFOTIME(BASETIME)           'Start time of one rolling planning run (e.g. 2012-01-01-00, 2012-01-01-12, 2012-01-02-00...)'
* SET BASEDAY                      'DAYS'
* SET DAY                          'Days in loop'
* SET WEEKS                        'Week numbers for one year'
* SET BASETIME_WEEK(BASETIME,WEEKS)        'Connection between BASETIME og WEEKS'
* SET BASETIME_BASEDAY(BASETIME,BASEDAY)   'Connection between BASETIME and BASEDAY'
* SET Y(YYY)                       'Years in the optimization'
* SET T(TTT)                       'Time periods in the optimization, T00*T36'
* SET T_WITH_HIST(TTT)             'Time periods in the optimization, TM24*TM01 and T00*T36'
* SET DYN_DAY_AHEAD(T)                     'Time steps used to define IT_SPOTPERIOD (day-ahead bidding)
* SET DYN_DAY_AHEAD_FIRST_SOLVE(T) 'Time steps used to define IT_SPOTPERIOD (day-ahead bidding) for first loop
* SET T_INTRADAY(T)                                'Time steps for intraday modelling (not used in weekly looping)

* sets related to technologies/data
* SET G                            'Generation technologies in simulation'
* SET Bottleneck_G(RRR,RRR,G)      'Link bottlenecks (between Region 1 and Region 2) and Units'
* SET GDATASET                     'Generation technology data' ($ifi %UnitCramp%)
* SET GSTARTSET                    'Generation technology data'
* SET FFF                          'Fuels'
* SET FUELDATASET                  'Characteristics of fuels'
* SET DEF                          'Steps in elastic electricity demand'
* SET DEF_D(DEF)                   'Downwards steps in elastic electricity demand'
* SET DEF_U(DEF)                   'Upwards steps in elastic electricity demand'
* SET MPOLSET                      'Emission and other policy data'
* SET GONLINE(G)                   'Technologies with costs associated with bringing and having capacity online'
* SET GALWAYSRUNNING(G)            'Technologies with available capacity being always online'
* SET GSPINNING(G)                 'Technologies offering primary (spinning) reserves' ($if %PlugIn%)
* SET GINFLEXIBLE(G)               'Technologies with online capacity being independant of the wind power scenario' ($if %PlugIn%)
* SET GSLOPES(G)                   'Technologies with more than one incremental heat rate slope'
* SET GONESLOPE(G)                 'Technologies with one incremental heat rate slope'
* SET GSPECIALRESERVE(G)           'Technologies that can provide spinning reserve above its maximum power'
* SET IHRS                         'set of incremental heat rate slopes'
* SET PS                           'Plant station'
* SET STACONF(AAA,PS,G)            'Units in a plant station located in an area'

*Set CCGT_Group 'CCGT_groups that consists of/connect GT and ST'
*Set CCGT_GT(G) 'Gasturbines that are part of CCGT'
*Set CCGT_ST(G)'Steamturbines that are part of CCGT'
*Set CCGT_DB(G)'Duct burner that are part of CCGT'
*Set CCGT_Group_G(CCGT_Group, G) 'assignment of Units to CCGT_Groups'


*}
*}

* Sets (internal) *{
*------------------------------------------------------------------------------
* Declaration of internal sets:
*------------------------------------------------------------------------------

* Internal sets in relation to geography:
SET IR(RRR)                 'Regions in the simulation'
SET IA(AAA)                 'Areas in the simulation'
SET ICA(C,AAA)              'Assignment of areas to countries'
SET TSO_RA(TSO_RRR,AAA)     'Assignment of areas to TSO-regions'
;

* By definition in the database the Sets AAA and Sets RRR only include the areas which belong to the simulated countries.
* an area is assigned to a country if there is a region in the country to which the area is assigned

*------------------------------------------------------------------------------
* Definition of internal sets:
*------------------------------------------------------------------------------
IR(RRR)      = YES;
IA(AAA)      = YES;
ICA(C,AAA)   = YES$(SUM(RRR$ (RRRAAA(RRR,AAA) AND CCCRRR(C,RRR)),1) GT 0);
TSO_RA(TSO_RRR,AAA)   = YES$(SUM(RRR$ (RRRAAA(RRR,AAA) AND TSO_RRRRRR(TSO_RRR,RRR)),1) GT 0);


*------------------------------------------------------------------------------
* Declaration of internal sets:
*------------------------------------------------------------------------------
* The following sets are defined dependent on years in the outer loop
SET IAHYDRO(AAA)            'Areas containing hydropower with reservoirs'
SET IAELECSTO(AAA)          'Areas containing electricity storage'

*SET IAHYDRO_ELECSTO(AAA)    'Areas containing hydropower with reservoirs or electricity storage'

SET IAHYDRO_HYDRORES(AAA)        'Reservoir areas without hydro supply function'
$ifi '%HydroSupplyCurves%' == YES  SET IAHYDRO_HYDRORES_STEP(AAA)   'Reservoir areas with hydro supply function'
$ifi '%HydroSupplyCurves%' == YES  SET IAHYDROGK_Y(AAA,G)

SET IAGK_Y(AAA,G)           'Area, technology with positive capacity current simulation year';
;

SET IACHP_EXO(AAA)             'Areas for which the heat extraction or el. restrictions of CHP plants are calculated exogenously'
SET IACHP_ENDO(AAA)            'Areas for which the CHP operation is calculated endogenously within GAMS'
;

SET IRWINDFORECAST(RRR)                 'Regions for which forecasts are provided'
SET IRNOWINDFORECAST(RRR)               'Regions for which wind_bid is wind_realised'

;

* Sets of Regions using or not using wind forecasts
$ifi                    '%UseWindForecast%'==YES IRWINDFORECAST(RRR) = RRR_WindForecast(RRR);
$ifi Not        '%UseWindForecast%'==YES IRWINDFORECAST(RRR) = NO;
IRNOWINDFORECAST(RRR) = IR(RRR) - IRWINDFORECAST(RRR);

$ifi '%HydroSupplyCurves%' == YES  IAHYDRO_HYDRORES_STEP(AAA) = AAA_HYDRORES_STEP(AAA);

$ifi     '%CHP%'==YES IACHP_EXO(AAA) = AAA_CHP_Exo(AAA);
$ifi Not '%CHP%'==YES IACHP_EXO(AAA) = NO;


*Steps of the HydroPenalty=Yes slack variable
SET IHydFlxStep 'Steps of the HydroPenalty=Yes slack variable' /HS01*HS25/;

* transmission
SET IEXPORT_Y(RRR,RRR)              'Lines with positive transmission capacity';
SET IEXPORT_NTC_Y(RRR,RRR)          'Aggregation of transmission lines with positive transmission capacity for which NTC based MC applies'
SET IEXPORT_FLBMC_Y(RRR,RRR)        'Regions where flow-based market coupling is executed and where there is transmission capacity through Flowgates'
SET IEXPORT_FLBMC_DC_Y(RRR,RRR)     'Regions where flow-based market coupling is executed and where there is transmission capacity through Non-Flowgates (DC-lines)'

SET I_AC_Line_Y(RRR,RRR)            'SET of AC TRANSMISSION LINES';
SET I_DC_LINE_Y(RRR,RRR)            'SET of DC TRANSMISSION LINES';

* IEXPORT_Y / IEXPORT_Y_Day_ahead should contain the ATC for
* the applicable grid load case (this is valid for ATC based MC and flowbased MC).
* Whereas the ATC for ATC based MC can be aggregated per import and export region,
* the ATC for flow based MC must be per line

$ifi '%LFB%' ==  Yes SET ISFD_Y_FLG(FLG)      'Flowgates with remaining available margin in Standard Flow Direction (lines used for LFB Calculations)'
$ifi '%LFB%' ==  Yes SET INSFD_Y_FLG(FLG)     'Flowgates with remaining available margin in Non-Standard Flow Direction (lines used for LFB Calculations)'

* sets used for printout
$ifi '%LFB%' ==  Yes SET IRRRFLG_SFD(RRR,RRR,FLG)
$ifi '%LFB%' ==  Yes SET IRRRFLG_NSFD(RRR,RRR,FLG)


* time related
SET T(TTT)                               'Timesteps'
SET IT_SPOTPERIOD(T)                     'Timesteps for spot market optimisation' //;
SET IENDTIME(T)                          'Last timestep of each optimisation period, as rolling planning periods ends alway at the end of the day'  //;
SET IENDTIME_HYDISDP(T)                  'Last timestep in optimization period for select hydro equations (equal to iendtime for daily looping, +12h else)' //;
SET IENDTIME_SHADOWPRICE(T)              'Timestep when shadowprice is determined'  //;
SET IT_OPT(T)                            'Timesteps for optimisation (varies with DA- or ID-loop)' //;
SET IT_OPT_ALL(T)                        'Timesteps for optimisation additionally contains T00' //;

*for QGONLOP2
SET DTIME 'Set containing a number of time periods before current element of T' /DT1*DT12/;
SET UTIME 'Set containing a number of time periods after current element of T' /UT1*UT5/;
SET TWRITEOUT(BASETIME,TTT);

*------------------------------------------------------------------------------
* Declaration of internal sets (most of them related to technologies)
*------------------------------------------------------------------------------

* Definitions of the sets relative to technologies:
* Internal subsets of generation technologies:
SETS
IGCONDENSING(G)             'Condensing technologies'
IGBACKPR(G)                 'Back pressure technologies'
IGEXTRACTION(G)             'Extraction technologies'
IGHEATBOILER(G)             'Heat-only boilers'
IGHEATPUMP(G)               'Electric heaters or heatpumps'
IGHEATSTORAGE(G)            'Heat storage technologies'
IGELECSTORAGE(G)            'Electricity storage technologies'
IGELECSTOACAES(G)           'adiabate Compressed Air Energy (CAES) storage technologies'
IGELECSTODCAES(G)           'Diabate Compressed Air Energy (CAES) storage technologies'
IGELECSTOEV(G)              'Electric Vehicle storage technologies'
IGELECSTODSM(G)             'Industrial Demand Side Management'
IGHYDRORES(G)               'Hydropower with reservoir'
IGRUNOFRIVER(G)             'Hydropower run-of-river no reservoir'
IGWIND(G)                   'Wind power technologies'
IGSOLAR(G)                  'Solar power technologies'
IGTIDALSTREAM(G)            'Tidal stream technologies'
IGWAVE(G)                   'Wave power technologies'
IGDISGEN(G)                 'Distributed Generation'
IGGEOTHHEAT(G)              'Geothermal power technologies'
IGBiomass(G)                'Biomass power plants'
IGOthRes(G)                 'Other Res units'
IGSOLARTH(G)                'Solar thermal power'
IGDUCTBURNER(G)             'Duct burner of CCGT units (CCGT_Imp)'
IGPTG(G)                    'Power-to-Gas'
IGLOADSHED(G)               'Interruptible load'

* (Temporary) subsets of generation technologies:
ITMPGCONDENSING(G)          'Condensing technologies'
ITMPGBACKPR(G)              'Back pressure technologies'
ITMPGEXTRACTION(G)          'Extraction technologies'
ITMPGHEATBOILER(G)          'Heat-only boilers'
ITMPGHEATPUMP(G)            'Electric heaters or heatpumps'
ITMPGHEATSTORAGE(G)         'Heat storage technologies'
ITMPGELECSTORAGE(G)         'Electricity storage technologies'
ITMPGELECSTOACAES(G)        'adiabate Compressed Air Energy (CAES) storage technologies'
ITMPGELECSTODCAES(G)        'Diabate Compressed Air Energy (CAES) storage technologies'
ITMPGELECSTOEV(G)           'Electric Vehicle storage technologies'
ITMPGELECSTODSM(G)          'Industrial Demand Side Management'
$ifi '%Code_version%' == EWL ITMPGDSMSTORAGE(G)'Industrial Demand Side Management'
ITMPGHYDRORES(G)            'Hydropower with reservoir'
ITMPGRUNOFRIVER(G)          'Hydropower run-of-river no reservoir'
ITMPGWIND(G)                'Wind power technologies'
ITMPGSOLAR(G)               'Solar power technologies'
ITMPGTIDALSTREAM(G)         'Tidal stream technologies'
ITMPGWAVE(G)                'Wave power technologies'
ITMPGDISGEN(G)              'Distributed Generation'
ITMPGGEOTHHEAT(G)           'Geothermal power technologies'
ITMPGSOLARTH(G)             'Solarthermal power technologies'
ITMPBiomass(G)              'Biomass power plants'
ITMPOthRes(G)               'Oth RES power plants'
ITMPDUCTBURNER(G)           'Duct burner of CCGT units (CCGT_Imp)'
ITMPGPTG(G)                 'Power-to-Gas'
ITMPGLOADSHED(G)            'Interruptible load'
* internal sets read in from sets.inc
IGALWAYSRUNNING(G)          'Technologies with large lead times'
IGSPINNING(G)               'Technologies providing ancilliary services'
IGINFLEXIBLE(G)             'Technologies with medium lead times'
IGSLOPES(G)                 'Set of power plants with more than one incremental heat rate slope'
IGONESLOPE(G)               'Set of power plants with one incremental heat rate slope'

* internal sets for CHP
IGFIXHEAT(G)                'CHP units with fixed heat production'
IGFIXHEAT_EXT(G)            'Extraction-condensing CHP units with fixed heat production'
IGFLEXHEAT(G)               'Units whose heat production (if any) is determined endogenously'
IGCHPBOUNDE(G)              'CHP units with upper / lower bound of el. generation (due to operation restrictions from heat supply)'
IGCHP_EXO(G)                'CHP units with exogeneous heat demand (IGFIXHEAT+IGCHPBOUNDE)'

* Further sets that are determined wrt external parameters
IGNATGAS(G)                 'Set of natural gas fired power plants'
IGMINOPERATION(G)           'Set for those plants that have minimum operation time >1'
IGRAMP(G)                   'Set for those plants that have ramp up rates above 1 hour (i.e. GDRAMPUP <1)'
IGLEADTIME(G)               'Set of those plants that have lead times >1 hour'

* and their respective temporary sets
ITMPNATGAS(G)               'Natural Gas Technologies'
ITMPGRAMP(G)                            'Ramp up rates above 1 hour'
ITMPGMINOPERATION(G)        'Minimum operation time >1'
ITMPGLEADTIME(G)                        'Lead times >1 hour'

* internal sets that are defined as combinations from basic generation technologies
IGUSINGFUEL(G)              'Technologies using fuel (not included water, solar)'
IGELECONLY(G)               'Technologies generating electricity only'
IGHEATONLY(G)               'All technologies generating heat-only'
IGELECANDHEAT(G)            'Technologies generating electricity and heat'
IGELEC(G)                   'Technologies generating electricity'
IGHEAT(G)                   'Technologies generating heat'
IGELECTHERMAL(G)            'Thermal electricity producing untis (condensing + extraction + backpressure)'
IGDISPATCH(G)               'Technologies that can be dispatched'
IGSTORAGE(G)                'Electricity and heat storage technologies'
IGESTORAGE_DSM(G)           'Units consisting of electricity storages and units using electricity like heat pumps and units for demand side management'

IGFLEXDEMAND(G)             'Units using electricity like heat pumps and units for demand side management'

IGCON_HYRES_ELSTO(G)        'Condensing technologies and hydropower with reservoir technologies and electricity storage'
IGHYRES_ELSTO(G)            'Set of hydroreservoirs and elecstorages'

IGHYDRO(G)                  'Hydropower'
IGELECFLUCTUATION(G)        'Technologies that cannot be scheduled e.g. wind, solar, hydro run-of-river'
IGONL_CONDENSING(G)         'Condensing technologies with part load efficency lower than maximum efficiency'
IGUC(G)                     'Technologies with unit commitment restrictions (e.g. minimum power)'
IGNOUCSCC(G)                'Set of power plants without outtime dependant start-up fuel consumption but with start-up costs'
IGSTARTUPRAMP(G)            'Set of power plants with a start-up ramp limit'

IGDISPATCH_NOUC(G)          'Set of dispatchable power plants not part IGUC (either must-run or without unit commitment restrictions)'
IGDISPATCH_NOELECSTO(G)     'Set of dispatchable power plants except electricity storage (pumped hydro)'
IGHEAT_NO_STORAGE(G)        'All technologies generating heat without storages'
IGELECPARTLOAD(G)           'Technologies using fuel where partload efficiency is different from fullload efficiency'
IGELECNOPARTLOAD(G)         'Technologies using fuel where Partload efficiency is same as fullload efficiency'

IGFASTUNITS(G)              'Set of those plants that have start-up time <= 1'
ITMPGFASTUNITS(G)           ' Temporary Set start-up time < 1'

IGLEADTIME_NOALWAYS(G)      'Set of those plants that have lead time >1 and is not always online'
IGUC_NOALWAYS(G)            'Set of those plants that are part of IGUC and is not always online'
IGONLYDAYAHEAD(G)           'Unit groups offering all their production capacity to the day-ahead market, and only the capacity not sold on day-ahead on the intraday market'
IGDISPATCH_NOONLYDAYAHEAD(G) 'Unit groups that can be scheduled and which are not part of IGONLYDAYAHEAD'
IGMINOPERATION_NOALWAYS(G) 'Set of those plants that have minimum operation time >1 and is not always online'
IGELEC_UNP_OUTAGE(AAA,G)    'Set of those plants, that have an unplanned outage in the actual loop'

;


*------------------------------------------------------------------------------
* Duplicates of certain external and internal sets:
*------------------------------------------------------------------------------
* Duplications of sets: alias(ORIGSET, SET1, SET2, ...) means that SET1, SET2 are all copies of ORIGSET (and will always contain the same elements as the original set!)
ALIAS (RRR, IRRRE, IRRRI, IRE, IRI, IRALIAS)
ALIAS (T,ITALIAS)
ALIAS (T_WITH_HIST,ITALIAS_WITH_HIST)
;

ALIAS (IHYDFLXSTEP, IHYDFLXSTEPALIAS);
ALIAS (IGHYDRORES, IGHYDRORESALIAS);

* Sign of slack variables
SET IPLUSMINUS   'Violation of equation'  /IPLUS Violation of equation upwards, IMINUS  Violation of equation downwards/;

*}

*}

* Parameters (Part I, external) *{

*---- Technology data: --------------------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\tech.inc';
$onlisting
*{ This file contains:
* PARAMETER GDATA(AAA,G,GDATASET)            'Technologies characteristics'
* where GDATASET consists of
* GDTYPE                      'Generation type'
* GDCV                        'Cv-value (CHP-Ext)' (Backpressure wird separat eingelesen)
* GDCB                        'Cb-value (CHP)'
* GDFUEL                      'Fuel type'
* GDFE_SECTION                'Fuel efficiency : section of line of fuel consumption function' (All except BP)
* GDFE_SLOPE                  'Fuel efficiency : slope of line of fuel consumption function'
* GDFUELTAX                   'Tax on fuel usage for producing power and heat (condensing and chp) in Germany (Euro/GJ fuel)'
* GDFUELTAXRED                'Tax reduction on fuel usage for producing power and heat in chp in Germany (Euro/GJ fuel)'
* GDLOADEFF                   'Effiency of storage when loading'
* GDOMVCOST                   'Variable operating and maintenance costs (Euro/MWh)'
* GDSTARTUPCOST               'Variable start-up costs (EUR/MW)'
* GDSTARTUPFUEL               'Start up fuel type'
* GDMINLOADFACTOR             'Minimum load factor (representing = maximum capacity/ minimum power'
* GDMINOPERATION              'Minimum operation time'
* GDMINSHUTDOWN               'Minimum shutdown time'
* GDLEADTIMEHOT               'Lead time for hot start'
* GDLEADTIMEWARM              'Lead time for warm start'
* GDLEADTIMECOLD              'Lead time for cold start'
* GDPARTLOAD                  'Part load efficiency'
* GDFULLLOAD                  'full load efficiency'
* GDRAMPUP                    'Maximum ramp rate up per hour relatively to the operating range (max power minus min power) of the default technology [1/h]'
* GDRAMPDOWN                  'Maximum ramp rate down per hour relatively to the operating range (max power minus min power) of the default technology [1/h]'
* GDSTARTUPRAMP               'Maximum start-up ramp rate per hour relatively to the output capacity of the default technology [1/h]'
* GDDESO2                     'Degree of desulphoring'
* GDUCSCCC                    'Unit commitment: time to cool from shut-down to cold (hours)'
* GDHOTWARM                   'Time to cool from shut-down to warm (hours)'
* GDWARMCOLD                  'Time to cool from warm to cold (hours)'
* GDMINONL                    'Minimum capacity online'
* GDMaxSpinResPri             'Maximum provision of primary spinning reserve'
* GDMaxSpinResSec             'Maximum provision of secondary spinning reserve'
* GDStartUpFuelConsHot        'Startup fuel consumption for hot starts'
* GDCHP_Subsidy1         'German CHP-Subsidy for loads above 2 MW (modelled for capacities above 2 MW)'
* GDCHP_Subsidy2         'German additional CHP-Subsidy for loads between 0.25-2 MW (modelled for capacities above 0.25-2 MW)'
* GDCHP_Subsidy3         'German additional CHP-Subsidy for loads between 0.05-0.25 MW (modelled for capacities below 0.25 MW)'

*CCGT_Flag_GT 'expected 1, if CCGT Gas Turbine'
*CCGT_Flag_ST 'expected 1, if CCGT Steam Turbine'
*CCGT_Flag_DB 'expected 1, if CCGT duct burner'

*}

*---- Fuel data: --------------------------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\fuel.inc';
$onlisting
*{ This file contains:
* PARAMETER FDATA(FFF,FUELDATASET)              'Fuel specific values'
*}

*---- Geographically specific values: -----------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\geogr.inc';
$onlisting
*{ This file contains:
* PARAMETER DISLOSS_E(RRR)                   'Loss in electricity distribution'
* PARAMETER DISLOSS_H(AAA)                   'Loss in heat distribution'
* PARAMETER DISCOST_E(RRR)                   'Cost of electricity distribution'
* PARAMETER DISCOST_H(AAA)                   'Cost of heat distribution'
* PARAMETER TAX_HEATPUMP(C)                  'Tax on electricity used in heat pumps'
* PARAMETER ELEC_SUBSIDY(C,FFF)              'Subsidy on electricity from biomass or waste'
* PARAMETER TAX_DE(C)                        'Consumers tax on electricity consumption'
* PARAMETER TAX_DH(C)                        'Consumers tax on heat consumption'
* PARAMETER M_POL(MPOLSET,C,WEEKS)           'Emission policy data'
* PARAMETER DEFP_BASE(RRR)                   'Nominal annual average consumer electricity price'
* PARAMETER DHFP_BASE(AAA)                   'Nominal annual average consumer heat price'
*}

*---- Annually specified generation capacities: -------------------------------
*#UeNB version 20.08.2019
*In EWL-Version values of GDFE_Section, GDFE_Slope, GDCB and GDCV are overwritten for FixQ Units in AAA_CHP_Exo areas
*overwriting in gkfx_all.inc file
*correction of Unit parameters should not be made within JMM Code --> Corrected parameters should be transfered to Database

$offlisting
$INCLUDE '%code_path_model%inc_structure_det\gkfx_all.inc';
$onlisting
*{ This file contains:
* PARAMETER GKFXELEC(YYY,AAA,G)                'Power Capacity of generation technologies'
* PARAMETER GKFXHEAT(YYY,AAA,G)                'Capacity of heat production'
* $ifi '%CCGT_Imp%' == Yes PARAMETER GKFXSTEAM(YYY,AAA,G)               'Capacity of steam production (duct burner of CCGT units)'
* PARAMETER GKFX_CONTENTSTORAGE(YYY,AAA,G)     'Storage capacity of heat and power storages in MWh'
* PARAMETER GKFX_MINCONTENTSTORAGE(YYY,AAA,G)  'Minimum storage content of heat and power storages in MWh'
* PARAMETER GKFX_CHARGINGSTORAGE(YYY,AAA,G)    'Capacity of charging (loading) the heat or power storage'
* PARAMETER GKFX_MINCHARGINGSTORAGE(YYY,AAA,G) 'Minimum capacity of charging (loading) the heat or power storage'
* PARAMETER GKMAX_CONTENTHYDRORES(YYY,AAA,G)   'Hydro reservoir capacity in MWh'
* PARAMETER GKMIN_CONTENTHYDRORES(YYY,AAA,G)   'Minimum hydro reservoir content of in MWh'
*}

*---- Transmission data: ------------------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\trans.inc';
$onlisting
*{ This file contains:
* PARAMETER XCAPACITY(RRR,RRR,BASETIME)            'Transmission capacity between regions'
* PARAMETER XCAPACITY_Day_ahead(RRR,RRR,BASETIME)  'Transmission capacity day-ahead between regions'
* PARAMETER XCOST(IRRRE,IRRRI)                     'Transmission cost between regions'
* PARAMETER XLOSS(RRR,RRR)                         'Transmission loss between regions'
*}

*---- Annually specified fuel prices: ---------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\fuelp.inc';
$onlisting
*{ This file contains:
* PARAMETER FUELPRICE_PER_GJ(AAA,FFF,WEEKS)  'Fuel prices (Euro/GJ)'
* PARAMETER FUELPRICE(AAA,FFF,WEEKS)         'Fuel prices (Euro/MWh)';
*}

*---- Price-elastic demand: -------------------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\var.inc';
$onlisting
*{ This file contains:
* PARAMETER DEF_STEPS_QUANT(RRR,DEF)         'Quanties of elastic electricity demands'
* PARAMETER DEF_STEPS_PRICE(RRR,DEF)         'Prices of elastic electricity demands'
*}

*---- Daily variations for whole year: ------------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\basevar.inc';
$onlisting
*{ This file contains:
* PARAMETER BASE_WEEKDAY_VAR_INFOTIME(INFOTIME)      'Weekday of the infotime (assignments of infotime to weekdays: 1 = monday, 2 = tuesday,..., 7 = sunday'
* PARAMETER BASE_DE_VAR_T(RRR,BASETIME)              'Variation in electricity demand', 'contains the hourly, time series for the electricity demand in absolute, scaled values'
* PARAMETER BASE_DEMAND_NONSPIN_ANCPOS_N_1(TSO_RRR)  'Demand of positive minute reserve due to N-1 criteria'
* PARAMETER BASE_DEMAND_NONSPIN_ANCNEG_N_1(TSO_RRR)  'Demand of negative minute reserve due to N-1 criteria'
* PARAMETER BASE_DH_VAR_T(AAA,BASETIME)              'Variation in heat demand'
* PARAMETER BASE_DH_FRAC_Natgas_Min(AAA)             'Minimum thermal load fraction of NATGAS heta producers'
* PARAMETER BASE_INFLOWHYDRORES_VAR_T(AAA,BASETIME)  'Variation of the water inflow to reservoirs'
* PARAMETER BASE_RUNRIVER_VAR_T(RRR,BASETIME)        'Variation of generation of run-of-river'
* PARAMETER BASE_RESLEVELS_T(AAA,BASETIME)           'Historical average reservoir levels for hydro power'
* PARAMETER BASE_SOLAR_VAR_T(RRR,BASETIME)           'Variation of the solar generation'
* PARAMETER BASE_GEOTHERMAL_VAR_T(RRR,BASETIME)      'Variation of the solar generation'
* PARAMETER BASE_TIDALSTREAM_VAR_T(RRR,BASETIME)     'Variation of the tidal stream generation'
* PARAMETER BASE_WAVE_VAR_T(RRR,BASETIME)            'Variation of the wave power generation'
* PARAMETER BASE_X3FX_VAR_T(RRR,BASETIME)            'Variation in exchange with 3. region'

* PARAMETER BASE_DE_ANCPOS_VAR_D(TSO_RRR,BASEDAY)    'Demand for positive secondary reserve'
* PARAMETER BASE_DE_ANCNEG_VAR_D(TSO_RRR,BASEDAY)    'Demand for negative secondary reserve'
* PARAMETER BASE_DE_ANCPOS_PRI_VAR_D(TSO_RRR,BASEDAY) 'Demand for positive primary reserve'
* PARAMETER BASE_DE_ANCNEG_PRI_VAR_D(TSO_RRR,BASEDAY) 'Demand for negative primary reserve'

* PARAMETER BASE_WIND_VAR_Det(RRR,BASETIME)          'Deterministic wind power production'
* PARAMETER BASE_GKDERATE_VAR_WEEK (AAA,G,WEEKS)     'availability factor due to planned revisions of units'
* PARAMETER BASE_UNP_OUTAGE_VAR_T (AAA,G,BASETIME)   'availability factor due to unplanned outages of units'
* PARAMETER BASE_DISGEN_VAR_T(RRR,BASETIME)          'fixed distributed generation'
* PARAMETER BASE_FIXQ(AAA,G,BASETIME)                'fixed external heat generation'
*}

PARAMETER IHELP_ANY_OUTAGE  'parameter that indicates if any outage occurs'  /1/;
IF (SUM((AAA,G,BASETIME), BASE_UNP_OUTAGE_VAR_T(AAA,G,BASETIME)) LT 1,
  IHELP_ANY_OUTAGE = 0;
);

IACHP_Endo(AAA)=IA(AAA)$(sum(BASETIME,BASE_DH_VAR_T(AAA,BASETIME))>0)-IACHP_Exo(AAA);

*---- Start value : ---------------------------------------------------------------
$offlisting
PARAMETER GSTARTVALUEDATA(AAA,G,GSTARTSET)  'start values for first run'
/
$INCLUDE "%data_path_in%inc_database\O Par GSTARTVALUEDATA.inc";
/;
$onlisting
* $INCLUDE '%code_path_model%inc_structure_det\startvalues.inc';
*{ This file contains:
* PARAMETER GSTARTVALUEDATA(AAA,G,GSTARTSET)  'Start values for first run'
*}

*---- Parameters related to rolling planning : ------------------------------------
$ifi NOT '%Looping%' == week PARAMETER NO_ROLL_PERIODS_WITHIN_DAY 'Number of rolling planning periods within a day' /2/;
$ifi '%Looping%' == week     PARAMETER NO_ROLL_PERIODS_WITHIN_DAY 'Number of rolling planning periods within a day' /1/;
$ifi '%Looping%' == 3hours NO_ROLL_PERIODS_WITHIN_DAY=8;
$ifi not '%Looping%' == week PARAMETER NO_HOURS_OVERLAP   'Number of hours for which rolling planning is shifted'  /12/;
$ifi '%Looping%' == week     PARAMETER NO_HOURS_OVERLAP   'Number of hours for which rolling planning is shifted'  /168/;
$ifi '%Looping%' == 3hours NO_HOURS_OVERLAP=3;

PARAMETER LAG_TIME_BIDDING  /12/;

*---- Piecewise linear fuel consumption curve:-------------------------------------
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\APPROX_ExternalParameters.inc'
$onlisting
*{ This file contains:
* PARAMETER FE_SLOPE_APPROX(AAA,G,IHRS)       'different slopes of the piecewise linear approximated fuel consumption function'
* PARAMETER FE_RIGHTBORDER_APPROX(AAA,G,IHRS) 'the right border of the intervall in relation to the total capacity , e.g. the last right border has to have the value 1'
*}

*---- MIP: -------------------------------------------------------------------
$ifi '%UnitCmin%'==Yes $INCLUDE '%code_path_addons%/Mip/MIP_ExternalParameters.inc'
*{ This file contains:
* $ifi %UnitCscc%==yes   SET i  'Set larger than largest value of GDUCSCCC'  /1*30/;
* $ifi %UnitCscc%==yes   PARAMETER UCSCCCOST(G,i) 'Startup costs when unit has been offline for i hours'
* $ifi %UnitCscc%==yes   SET GSTARTCOSTOUTTIME(G) 'Technologies with outtime dependant start-up costs'
*}


*----- End of include files ---------------------------------------------------
*}

* Sets (Part II, dependent on parameters, e.g. GDTYPE) *{


*------------------------------------------------------------------------------
* Definition of internal sets (most of them related to technologies)
*------------------------------------------------------------------------------

IGCONDENSING(G)    = NO;
IGBACKPR(G)        = NO;
IGEXTRACTION(G)    = NO;
IGHEATBOILER(G)    = NO;
IGHEATPUMP(G)      = NO;
IGHEATSTORAGE(G)   = NO;
IGELECSTORAGE(G)   = NO;
IGELECSTOACAES(G)  = NO;
IGELECSTODCAES(G)  = NO;
IGELECSTOEV(G)     = NO;
IGELECSTODSM(G)    = NO;
IGHYDRORES(G)      = NO;
IGRUNOFRIVER(G)    = NO;
IGWIND(G)          = NO;
IGSOLAR(G)         = NO;
IGTIDALSTREAM(G)   = NO;
IGWAVE(G)          = NO;
IGDISGEN(G)        = NO;
IGGEOTHHEAT(G)     = NO;
IGSOLARTH(G)       = NO;
IGBiomass(G)       = NO;
IGOthRes(G)        = NO;
IGDUCTBURNER(G)    = NO;
IGPTG(G)           = NO;
IGLOADSHED(G)      = NO;

IGNATGAS(G)        = NO;
IGMINOPERATION(G)  = NO;
IGRAMP(G)          = NO;
IGLEADTIME(G)      = NO;

IGFASTUNITS(G)     = NO;
IGELEC_UNP_OUTAGE(IA,G) = NO;


LOOP (IA,
*  Parameters for generation types (15)
   ITMPGCONDENSING(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ  1);
   ITMPGBACKPR(G)       = YES$(GDATA(IA,G,'GDTYPE') EQ  2);
   ITMPGEXTRACTION(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ  3);
   ITMPGHEATBOILER(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ  4);
   ITMPGHEATPUMP(G)     = YES$(GDATA(IA,G,'GDTYPE') EQ  5);
   ITMPGHEATSTORAGE(G)  = YES$(GDATA(IA,G,'GDTYPE') EQ  6);
   ITMPGELECSTORAGE(G)  = YES$(GDATA(IA,G,'GDTYPE') EQ  7);
   ITMPGHYDRORES(G)     = YES$(GDATA(IA,G,'GDTYPE') EQ  8);
   ITMPGRUNOFRIVER(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ  9);
   ITMPGWIND(G)         = YES$(GDATA(IA,G,'GDTYPE') EQ 10);
   ITMPGSOLAR(G)        = YES$(GDATA(IA,G,'GDTYPE') EQ 11);
   ITMPGTIDALSTREAM(G)  = YES$(GDATA(IA,G,'GDTYPE') EQ 14);
   ITMPGWAVE(G)         = YES$(GDATA(IA,G,'GDTYPE') EQ 15);
   ITMPGDISGEN(G)       = YES$(GDATA(IA,G,'GDTYPE') EQ 16);
$ifi '%Code_version%' == EWL ITMPGDSMSTORAGE(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ 19);
   ITMPGGEOTHHEAT(G)    = YES$(GDATA(IA,G,'GDTYPE') EQ 17);
   ITMPBiomass(G)       = YES$(GDATA(IA,G,'GDTYPE') EQ 20);
   ITMPGSOLARTH(G)      = YES$(GDATA(IA,G,'GDTYPE') EQ 21);
   ITMPOthRes(G)        = YES$(GDATA(IA,G,'GDTYPE') EQ 22);
   ITMPGELECSTOACAES(G) = YES$(GDATA(IA,G,'GDTYPE') EQ 23);
   ITMPGELECSTODCAES(G) = YES$(GDATA(IA,G,'GDTYPE') EQ 24);
   ITMPGELECSTODSM(G)   = YES$(GDATA(IA,G,'GDTYPE') EQ 25);
   ITMPGELECSTOEV(G)    = YES$(GDATA(IA,G,'GDTYPE') EQ 26);
   ITMPDUCTBURNER(G)    = YES$(GDATA(IA,G,'GDTYPE') EQ 27);
   ITMPGPTG(G)          = YES$(GDATA(IA,G,'GDTYPE') EQ 28);
   ITMPGLOADSHED(G)     = YES$(GDATA(IA,G,'GDTYPE') EQ 29);

* Particular parameters for generation
   ITMPNATGAS(G)        = YES$(GDATA(IA,G,'GDFUEL') EQ 2);
   ITMPGRAMP(G)         = YES$((GDATA(IA,G,'GDRAMPUP')<1) AND (GDATA(IA,G,'GDRAMPUP')>0));
   ITMPGLEADTIME(G)     = YES$(GDATA(IA,G,'GDLEADTIMEHOT') > 1);

   IGCONDENSING(G)      = IGCONDENSING(G)   + ITMPGCONDENSING(G);
   IGBACKPR(G)          = IGBACKPR(G)       + ITMPGBACKPR(G) ;
   IGEXTRACTION(G)      = IGEXTRACTION(G)   + ITMPGEXTRACTION(G) ;
   IGHEATBOILER(G)      = IGHEATBOILER(G)   + ITMPGHEATBOILER(G);
   IGHEATPUMP(G)        = IGHEATPUMP(G)     + ITMPGHEATPUMP(G) ;
   IGHEATSTORAGE(G)     = IGHEATSTORAGE(G)  + ITMPGHEATSTORAGE(G);
   IGELECSTOACAES(G)    = IGELECSTOACAES(G) + ITMPGELECSTOACAES(G) ;
   IGELECSTODCAES(G)    = IGELECSTODCAES(G) + ITMPGELECSTODCAES(G) ;
   IGELECSTOEV(G)       = IGELECSTOEV(G)    + ITMPGELECSTOEV(G) ;

   IGELECSTODSM(G)      = IGELECSTODSM(G)   + ITMPGELECSTODSM(G)
$ifi '%Code_version%' == EWL +ITMPGDSMSTORAGE(G)
   ;
 IGPTG(G)              = IGPTG(G)          + ITMPGPTG(G);

   IGELECSTORAGE(G)     = IGELECSTORAGE(G)  + ITMPGELECSTORAGE(G) + IGELECSTOACAES(G) + IGELECSTODCAES(G);
   IGHYDRORES(G)        = IGHYDRORES(G)     + ITMPGHYDRORES(G);
   IGRUNOFRIVER(G)      = IGRUNOFRIVER(G)   + ITMPGRUNOFRIVER(G) ;
   IGWIND(G)            = IGWIND(G)         + ITMPGWIND(G) ;
   IGSOLAR(G)           = IGSOLAR(G)        + ITMPGSOLAR(G);
   IGTIDALSTREAM(G)     = IGTIDALSTREAM(G)  + ITMPGTIDALSTREAM(G);
   IGWAVE(G)            = IGWAVE(G)         + ITMPGWAVE(G);
   IGGEOTHHEAT(G)       = IGGEOTHHEAT(G)    + ITMPGGEOTHHEAT(G);
   IGSOLARTH(G)         = IGSOLARTH(G)      + ITMPGSOLARTH(G);
   IGBIOMASS(G)         = IGBIOMASS(G)      + ITMPBiomass(G);
   IGOthRes(G)          = IGOthRes(G)       + ITMPOthRes(G);
   IGDISGEN(G)          = IGDISGEN(G)       + ITMPGDISGEN(G);
   IGNATGAS(G)          = IGNATGAS(G)       + ITMPNATGAS(G);
   IGRAMP(G)            = IGRAMP(G)         + ITMPGRAMP(G);
   IGLEADTIME(G)        = IGLEADTIME(G)     + ITMPGLEADTIME(G);
   IGDUCTBURNER(G)      = IGDUCTBURNER(G)   + ITMPDUCTBURNER(G);
   IGLOADSHED(G)        = IGLOADSHED(G)     + ITMPGLOADSHED(G);
   
);

* ---- Reading in further time series (which require the above-defined subsets ----
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\further_time_series.inc';
$onlisting

*{ This file modifies:
* GSTARTVALUEDATA
* GDATA
*}

* sets built from 14 sets (IGCONDENSING - IGGEOTHHEAT) above:
   IGUSINGFUEL(G)         = IGCONDENSING(G)   + IGBACKPR(G)        + IGEXTRACTION(G) + IGHEATBOILER(G) + IGHEATPUMP(G) + IGELECSTODCAES(G) + IGDUCTBURNER(G) ;

   IGELECONLY(G)          = IGCONDENSING(G)   + IGELECSTORAGE(G)    + IGHYDRORES(G)     + IGRUNOFRIVER(G) + IGWIND(G) + IGSOLAR(G) + IGTIDALSTREAM(G) + IGWAVE(G) + IGGEOTHHEAT(G) + IGSOLARTH(G) + IGBiomass(G) + IGOthRes(G) + IGELECSTODSM(G) + IGELECSTOEV(G)+IGLOADSHED(G);
   IGHEATONLY(G)          = IGHEATPUMP(G)     + IGHEATSTORAGE(G)   + IGHEATBOILER(G);

   IGELECANDHEAT(G)       = IGBACKPR(G)       + IGEXTRACTION(G)    ;

   IGELEC(G)                                = IGELECONLY(G)     + IGELECANDHEAT(G)   ;
   IGHEAT(G)                                = IGHEATONLY(G)     + IGELECANDHEAT(G);
   IGHEAT_NO_STORAGE(G)                     = IGHEATPUMP(G)     + IGHEATBOILER(G)    + IGBACKPR(G)     + IGEXTRACTION(G);
   IGELECTHERMAL(G)                         = IGCONDENSING(G)   + IGBACKPR(G)        + IGEXTRACTION(G) + IGELECSTODCAES(G);
   IGDISPATCH(G)                            = IGCONDENSING(G)   + IGBACKPR(G)        + IGEXTRACTION(G) + IGELECSTORAGE(G) + IGHYDRORES(G) + IGELECSTODSM(G) + IGELECSTOEV(G)+IGLOADSHED(G)+IGPTG(G);

   IGESTORAGE_DSM(G)                        = IGHEATPUMP(G)     + IGELECSTORAGE(G)   + IGELECSTODSM(G) + IGELECSTOEV(G) + IGPTG(G);

   IGFLEXDEMAND(G)                          = IGELECSTODSM(G)   + IGHEATPUMP(G)      + IGELECSTOEV(G) + IGPTG(G);

   IGSTORAGE(G)                             = IGHEATSTORAGE(G)  + IGESTORAGE_DSM(G);
   IGCON_HYRES_ELSTO(G)                     = IGCONDENSING(G)   + IGELECSTORAGE(G)   + IGHYDRORES(G) ;
   IGHYRES_ELSTO(G) = IGHYDRORES(G) + IGELECSTORAGE(G);

* internal sets based on sets read in from sets.inc above
   IGUC(G)                                  = GONLINE(G);
   IGALWAYSRUNNING(G)                       = GALWAYSRUNNING(G);
   IGSPINNING(G)                            = GSPINNING(G);
* the following two sets are (probably) complementary
   IGSLOPES(G)                              = GSLOPES(G);
   IGONESLOPE(G)                            = GONESLOPE(G);

   IGFIXHEAT(G)                             = NO;
   IGCHPBOUNDE(G)                           = NO;
$ifi '%CHP%'==YES   IGFIXHEAT(G)                             = GCHP_FixQ(G);
   IGFIXHEAT_EXT(G)                         = NO;
$ifi '%CHP%'==YES   IGFIXHEAT_EXT(G)                         = IGFIXHEAT(G)$(IGEXTRACTION(G));
$ifi '%CHP%'==YES   IGCHPBOUNDE(G)                           = GCHP_BoundE(G);
   IGFLEXHEAT(G)                            = IGHEAT(G) - IGFIXHEAT(G) - IGCHPBOUNDE(G);
   IGCHP_EXO(G)                             = IGFIXHEAT(G) + IGCHPBOUNDE(G);



* ---- Default values (within this file there exists the possibility to change values which are missing in the database) ----
$offlisting
$INCLUDE '%code_path_model%inc_structure_det\default_values.inc';
$onlisting


* -----------------------------------
* set default value for minimum operation and shut down time for all backpressure units with fixed heat

LOOP ((AAA,G) $((GDATA(AAA,G,'GDTYPE') eq  2) AND IGFIXHEAT(G)),
      GDATA(AAA,G,'GDMINOPERATION')=0;
      GDATA(AAA,G,'GDMINSHUTDOWN')=0;
);


LOOP (IA,
   ITMPGMINOPERATION(G) = YES$(GDATA(IA,G,'GDMINOPERATION') > 1);
   IGMINOPERATION(G)    = IGMINOPERATION(G) + ITMPGMINOPERATION(G);
);
*------------------------------------


* internal sets built from sets above

   IGNOUCSCC(G)                             = GONLINE(G);
$ifi %UnitCscc%==yes    IGNOUCSCC(IGUC) = YES$(Not GSTARTCOSTOUTTIME(IGUC));
   IGDISPATCH_NOUC(G)                       = IGDISPATCH(G)-IGUC(G);

*  if the current version is shared by everybody simplifications are possible in the following lines
*  IGONLYDAYAHEAD(G)                        = IGALWAYSRUNNING(G) + IGINFLEXIBLE(G);
   IGONLYDAYAHEAD(G)                        = No;
   IGDISPATCH_NOONLYDAYAHEAD(IGDISPATCH)    = YES$(Not IGONLYDAYAHEAD(IGDISPATCH));

   IGUC_NOALWAYS(IGUC)                      = YES $ (Not IGALWAYSRUNNING(IGUC));
   IGMINOPERATION_NOALWAYS(IGMINOPERATION)  = YES $ (Not IGALWAYSRUNNING(IGMINOPERATION));
   IGELECPARTLOAD(IGELECTHERMAL)            = YES $(SUM(IA,GDATA(IA,IGELECTHERMAL,'GDFE_SECTION'))) ;
   IGELECNOPARTLOAD(IGELECTHERMAL)          = YES $(Not IGELECPARTLOAD(IGELECTHERMAL));
   IGFASTUNITS(IGDISPATCH)                  = YES$((NOT SUM(IA,GDATA(IA,IGDISPATCH,'GDLEADTIMEHOT')))
                                                                                                        AND (SUM(IA,GDATA(IA,IGDISPATCH,'GDMINOPERATION')) LE 1)

                                                                                                       AND (SUM(IA,GDATA(IA,IGDISPATCH,'GDMINSHUTDOWN'))  LE 1) ) ;

*parameters and dynamic sets regarding to CCGT
$ifi '%CCGT_Imp%' == Yes  CCGT_GT(G)$(sum(IA,GDATA(IA,G,'CCGT_Flag_GT')) EQ 1) = YES;
$ifi '%CCGT_Imp%' == Yes  CCGT_ST(G)$(sum(IA,GDATA(IA,G,'CCGT_Flag_ST')) EQ 1) = YES;
$ifi '%CCGT_Imp%' == Yes  CCGT_DB(G)$(sum(IA,GDATA(IA,G,'CCGT_Flag_DB')) EQ 1) = YES;

*IG_Bypass: default set to Zero. If needed, new inc-file recommended
$ifi '%CCGT_Imp%' == Yes  PARAMETER IG_Bypass (CCGT_Group) 'Bypass per CCGT_Group, expected 1 or 0';
$ifi '%CCGT_Imp%' == Yes  IG_Bypass (CCGT_Group)= 0;
$ifi '%CCGT_Imp%' == Yes  Scalar CCGT_etaSteamProd /0.9/;

*parameter for adjustment of CCGT-power plant-parameters and modelling of CCGT
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_eta_fulload(CCGT_Group) 'Parameter that saves the efficiency of an CCGT_Group for further calculations';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_eta_partload(CCGT_Group) 'Parameter that saves the efficiency of an CCGT_Group for further calculations';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_STEAM_full(CCGT_Group) 'Steam Production of CCGT-Group in fulload';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_STEAM_min(CCGT_Group)  'Steam production of CCGT-Group in partload';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_slope_STEAM(AAA,G) 'slope of steam-usage-function of steam turbine';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_section_STEAM(AAA,G) 'section of steam-usage-function of steam turbine';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_sum_full(CCGT_Group) 'fulload capacity of CCGT-Group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_GT_full(CCGT_Group) 'fulload capacity of gas turbines of CCGT-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_ST_full(CCGT_Group) 'fulload capacity of steam turbines of CCGT-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_sum_min(CCGT_Group) 'partload capacity of CCGT-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_GT_min(CCGT_Group) 'partload capacity of gas turbines of CCGT-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_Capacity_ST_min(CCGT_Group) 'partload capacity of steam turbines of CCGT-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_GT_GDFULLLOAD_adj(CCGT_Group) 'adjusted fulload efficiency (GDFULLOAD) of gas turbines of CCGR-group';
$ifi '%CCGT_Imp%' == Yes  PARAMETER CCGT_GT_GDPARTLOAD_adj(CCGT_Group) 'adjusted partload efficiency (GDFULLOAD) of gas turbines of CCGR-group';

$ifi '%CCGT_Imp%' == Yes  Parameter HELPVAR_CCGT_Adj 'Binary parameter that enables/disables CCGT_Adjustment loop';
$ifi '%CCGT_Imp%' == Yes  $ifi '%CCGT_Adjustments%' == YES HELPVAR_CCGT_Adj=1;

*}

* Parameters (Part II) *{
*------------------------------------------------------------------------------
* Internal parameters and settings
*------------------------------------------------------------------------------

SCALAR CONVERT_KG_PER_GJ_T_PER_MWH             'Convert kg/GJ in ton/MWh' /0.0036/;
* 1kg/GJ=0.001t/GJ=0.001 t/GJ*3.6 GJ/MWh=0.0036 t/MWh
$ifi Not %UnitCmin% ==yes SCALAR Epsilon 'Small number to take care of numerical instabilities' /0.000001/;
$ifi     %UnitCmin% ==yes SCALAR Epsilon 'Small number to take care of numerical instabilities' /0.00001/;

*------------- Parameter to save  timeseries variations  -----------------------
PARAMETER IRUNRIVER_VAR_T(RRR,TTT)             'Variation of generation of run-of-river in regions' ;
PARAMETER IWAVE_VAR_T(RRR,TTT)                 'Variation of the wave power generation' ;
PARAMETER ISOLAR_VAR_T(RRR,TTT)                'Variation of the solar generation' ;
PARAMETER IGEOTHHEAT_VAR_T(RRR,TTT)            'Variation of the geothermal generation' ;
PARAMETER ITIDALSTREAM_VAR_T(RRR,TTT)          'Variation of the tidal stream generation';
PARAMETER IRESLEVELS_T(AAA,TTT)                'Variation in historical reservoir levels';
PARAMETER IDISGEN_VAR_T(RRR,TTT)               'Variation in distributed generation';

PARAMETER IDISGEN_VAR_T2(RRR,TTT)              'Variation in distributed generation for printout';
PARAMETER ITIDALSTREAM_VAR_T2(G,RRR,TTT)       'Variation of the tidal stream generation for printout';
PARAMETER IBIOMASS_VAR_T(RRR,TTT)              'Variation in Biomass';
PARAMETER IOthRes_Var_T(RRR,TTT)               'Injection of Other RES';
PARAMETER ISOLARTH_VAR_T(RRR,TTT)              'Variation of the solar generation' ;
*Aisle lines for departing and arriving EVs
PARAMETER IEV_Leave_Var_T(AAA,G,TTT)                'Variation of leaving electric vehicles';
PARAMETER IEV_Arrive_Var_T(AAA,G,TTT)               'Variation of arriving electric vehicles';

PARAMETER IEV_SOC_Leave_T(AAA,G,TTT)            'Time-variable state-of-charge of leaving vehicles';
PARAMETER IEV_SOC_Arrive_T(AAA,G,TTT)           'Time-variable state-of-charge of arriving vehicles';

*Flow Based Market Coupling
$ifi '%LFB%'==YES PARAMETER IFLBMC_CNEC_RAM_USED(TRL,T);
$ifi '%LFB%'==YES PARAMETER IFLBMC_CNEC_RAM_UNUSED(TRL,T);
* Shadow prices
PARAMETER ISDP_HYDRORES(AAA,TTT)               'Price profil for hydroreservoirs in regions' ;
PARAMETER ISDP_ONLINE(AAA,G)                   'Shadow price for unit being online' ;

$ifi '%HydroSupplyCurves%' == YES  PARAMETER ISDP_HYDRORES_STEP(AAA,G,TTT)        'Price profil (stepfunction) for hydroreservoirs in regions' ;
$ifi '%HydroSupplyCurves%' == YES  PARAMETER IMEDRESLEVELS_T(AAA,TTT)             'Long-term median filling level variation for supply function of hydro reservoirs'

$offlisting
PARAMETER ISDP_REFPRICE(AAA)                   'Area specific water value (shadow price)'
/
$INCLUDE '%data_path_in%inc_database\O Par ISDP_Refprice.inc'
/;
$onlisting

*------------- Set default values for hydro reservoir refprice -------------
Loop (AAA$(ISDP_REFPRICE(AAA) eq 0),
$ifi '%Emission_taxes_Daily%' == YES  ISDP_REFPRICE(AAA) = 3*sum((C,Baseday)$(ord(Baseday)=1 and ord(C)=1), M_POL('Tax_CO2',C,BASEDAY) );
$ifi not '%Emission_taxes_Daily%' == YES  ISDP_REFPRICE(AAA) = 3*sum((C,WEEKS)$(ord(WEEKS)=1 and ord(C)=1), M_POL('Tax_CO2',C,WEEKS) );
);

PARAMETER IHLPSDPONLINE_WEEKEND(AAA,G)    'Shadow price for unit being online' ;
PARAMETER IHLPSDPONLINE_WEEKDAY(AAA,G)     ;
          IHLPSDPONLINE_WEEKEND(AAA,G) = 0 ;
          IHLPSDPONLINE_WEEKDAY(AAA,G) = 0 ;

PARAMETER ISDP_STORAGE(AAA,G)             'Shadow price for storage content' ;
          ISDP_STORAGE(AAA,G)=0;

PARAMETER IHLPSDPSTORAGE_WEEKEND(AAA,G)   'Shadow price for storage content' ;
PARAMETER IHLPSDPSTORAGE_WEEKDAY(AAA,G) ;
          IHLPSDPSTORAGE_WEEKEND(AAA,G) = 0 ;
          IHLPSDPSTORAGE_WEEKDAY(AAA,G) = 0;

* Demand
PARAMETER IDEMANDELEC_VAR_T(RRR,TTT)     'Electricity demand';
PARAMETER IDEMANDELEC_BID_IR(RRR,TTT)    'Expected electricity demand forecasted at 12.00 for the next day';
PARAMETER IDEMANDHEAT(AAA,TTT)           'Heat demand (MW)';
PARAMETER IFRACNATGAS_MIN(AAA)           'Minimum thermal load fraction (-)';
IFRACNATGAS_MIN(IACHP_ENDO)                      = BASE_DH_FRAC_Natgas_Min(IACHP_ENDO);

$ifi %CHP%==YES PARAMETER IHEATEXT_EXO(AAA,G,TTT)              'The exogenously-given heat extraction by CHP units (MW)';
$ifi '%Code_version%' == EWL $ifi %CHP%==YES PARAMETER IBOUNDE_MIN(AAA,G,TTT)              'Exogeneously-given minimum electricity generation due to heat extraction of CHP units (MW)';
$ifi '%Code_version%' == EWL $ifi %CHP%==YES PARAMETER IBOUNDE_MAX(AAA,G,TTT)              'Exogeneously-given available electric capacity (reduced due to heat extraction of CHP units (MW)';

PARAMETER IDEMAND_ANCNEG_SEC(TSO_RRR,T)          'Demand of negative secondary reserve';
PARAMETER IDEMAND_ANCPOS_SEC(TSO_RRR,T)                                  'Demand of positive secondary reserve';
PARAMETER IDEMAND_ANCNEG_PRI(TSO_RRR,T)                                  'Demand of negative primary reserve';
PARAMETER IDEMAND_ANCPOS_PRI(TSO_RRR,T)                                  'Demand of positive primary reserve';
PARAMETER IDEMAND_ANCPOS(TSO_RRR,T)              'Demand of positive ancilliary reserve PRI+SEC';
PARAMETER IDEMAND_ANCNEG(TSO_RRR,T)              'Demand of negative ancilliary reserve PRI+SEC';
PARAMETER IDEMAND_NONSPIN_ANCPOS(TSO_RRR,T)        'Demand of positive tertiary reserve (MW)';
PARAMETER IDEMAND_NONSPIN_ANCNEG(TSO_RRR,T)        'Demand of negative tertiary reserve (MW)';

* Parameter for flexible demand function.
* PARAMETER IDEFLEXIBLEPRICE_T holds the price levels of individual steps
* in the electricity demand function, transformed to be comparable with
* production costs (including fuel taxes) by subtraction of taxes
* and distribution costs.
* Unit: Money/MWh.
PARAMETER IDEFLEXIBLEPRICE_T(RRR,DEF)          'Prices on elastic electricity demand steps';
PARAMETER IDEF_STEPS_QUANT(RRR,DEF)            'Quantity of each step in the electricity demand curve' ;

* Parameters for price-elastic demand
IDEFLEXIBLEPRICE_T(IR,DEF_D) = DEF_STEPS_PRICE(IR,DEF_D);
IDEF_STEPS_QUANT(IR,DEF_D)   = DEF_STEPS_QUANT(IR,DEF_D);
IDEFLEXIBLEPRICE_T(IR,DEF_U) = DEF_STEPS_PRICE(IR,DEF_U) * DEFP_BASE(IR) - SUM(C$CCCRRR(C,IR),TAX_DE(C)) - DISCOST_E(IR);
IDEF_STEPS_QUANT(IR,DEF_U)   = DEF_STEPS_QUANT(IR,DEF_U);

PARAMETER IGKDERATE(AAA,G,TTT)                 'Technical availability'  ;
PARAMETER IG_UNP_OUTAGE(AAA,G,TTT)             'Binary Parameter, if unit has an unplanned outage   1= unplanned outage'  ;

Parameter IMINCHPFACTOR(AAA,G,T)               'Minimum electricity generation by CHP units relative to the installed el. capacity (unit-less)' ;
Parameter IMAXCHPFACTOR(AAA,G,T)               'Maximum electricity generation by CHP units relative to the installed el. capacity (unit-less)' ;

Parameter IMaxGen(AAA,G,T);
Parameter IMinGen(AAA,G,T);
Parameter IMINVGONLINE(AAA,G,T)                'Minimum electricity generation';
Parameter IMINVGONLINE_CHP(AAA,G,T)            'Minimum electricity generation due to heat extraction';

Parameter IStartRamp_CHP(AAA,G,T);

PARAMETER IVGELEC_OUTAGE_T(RRR,TTT)            ' Parameter, that determines the energy that has to be replaced within one region, when an outage occurs';
PARAMETER IVGELEC_AND_RESERVES_OUTAGE_T(RRR,TTT) 'Parameter, that determines the energy that has to be replaced within one region, when a new outage occurs';
PARAMETER IVGELEC_PREVLOOP(AAA,G,TTT)          'Electricty commited to spot market, as planned in the previous loop';
PARAMETER IVGELEC_PREVDAYAHEADLOOP(AAA,G,TTT)  'Electricty production commited to spot market, as planned in the previous loop with day-ahead scheduling';
PARAMETER IVGELEC_CONSUMED_PREVDAYAHEAD(AAA,G,TTT) 'Electricty loading of electricity storage commited to spot market, as planned in the previous loop with day-ahead scheduling';
PARAMETER IVGELEC_DNEG_PREVLOOP(AAA,G,TTT);
PARAMETER IVGELEC_DPOS_PREVLOOP(AAA,G,TTT);
PARAMETER IVGONLINE_PREVLOOP(AAA,G,TTT);
PARAMETER IVGE_ANCPOS_PREVLOOP(AAA,G,TTT);

IG_UNP_OUTAGE(AAA,G,TTT)                 =0;
IVGELEC_PREVLOOP(AAA,G,TTT)              =0;
IVGELEC_PREVDAYAHEADLOOP(AAA,G,TTT)      =0;
IVGELEC_CONSUMED_PREVDAYAHEAD(AAA,G,TTT) =0;
IVGELEC_DNEG_PREVLOOP(AAA,G,TTT)         =0;
IVGELEC_DPOS_PREVLOOP(AAA,G,TTT)         =0;
IVGONLINE_PREVLOOP(AAA,G,TTT)            =0;
IVGE_ANCPOS_PREVLOOP(AAA,G,TTT)          =0;


PARAMETER IWIND_REALISED_IR(RRR,T)  'Realized wind infeed';
PARAMETER IWIND_BID_IR(RRR,T)       'Expected average production from wind power forecasted at 12.00 for the next day';

* Capacity of each unit
PARAMETER IGELECCAPACITY_Y(AAA,G)   'Nominal (net) electrical capacity of units' ;
PARAMETER IGHEATCAPACITY_Y(AAA,G)   'Nominal (net) thermal capacity of units' ;
$ifi '%CCGT_Imp%' == Yes PARAMETER IGSTEAMCAPACITY_Y (AAA,G) 'Nominal (net) steam procution capacity of units' ;
PARAMETER IGELECCAPEFF_CHP(AAA,G,TTT)  'Available el. capacity of CHP units (not considering unplanned outages)' ;

PARAMETER IMax_Delta_VGELEC_CHP(AAA,G,TTT) 'Maximal Delta in CHP production';

* Capability for storages
PARAMETER IGSTOLOADCAPACITY_Y(AAA,G);
PARAMETER IGSTOLOADCAPACITYMIN_Y(AAA,G);
PARAMETER IGSTOCONTENTCAPACITY_Y(AAA,G);
PARAMETER IGSTOMINCONTENT_Y(AAA,G);
PARAMETER IGHYDRORESCONTENTCAPACITY_Y(AAA,G);

* For hydroreservoir minimum content is required
PARAMETER IGHYDRORESMINCONTENT_Y(AAA,G);

PARAMETER IGResLOADCAPACITY_Y(AAA,G);
Parameter IPMPMAX(AAA,G,T);
Parameter IHYRESMAXGEN(AAA,G,T);
$ifi '%Looping%' == week Parameter IPMPMAX_E(AAA,G,T);
$ifi '%Looping%' == week Parameter IPMPMIN_E(AAA,G,T);

* Transmission capacity (MW) between regions RI and RE at the beginning of current simulation year:
PARAMETER IXCAPACITY_Y(IRRRE,IRRRI,T);
PARAMETER IXCAPACITY_Y_Day_ahead(IRRRE,IRRRI,T);
$ifi '%LFB%' ==  Yes PARAMETER IXCAPACITY_NO_FLG(IRRRE,IRRRI,T);

PARAMETER IXTYPE_Y(IRRRE,IRRRI);
PARAMETER PTDF(IRRRE,IRRRI,IRALIAS);

$ifi '%LFB%' ==  Yes PARAMETER IXCAPACITY_FLG_SFD(TRL,T);
$ifi '%LFB%' ==  Yes PARAMETER IXCAPACITY_FLG_NSFD(TRL,T);
$ifi '%LFB%' ==  Yes PARAMETER IPTDF_FLG(RRR,TRL,T);

* Fixed exchange with third countries (MW) current simulation year:
PARAMETER IX3COUNTRY_T_Y(RRR,T);

*VersID_TSOs
*start version
*Energiedumping durch Export
SET SHEDDING_FORBIDDEN_DUE_TO_IMPORT(RRR,T);
SHEDDING_FORBIDDEN_DUE_TO_IMPORT(RRR,T) = no;
* SET marks indices for which shedding for wind or pv is not
*   allowed because at this time there was no shedding in the
*   ENTSO-run
SET SHEDDING_FORBIDDEN_DUE_TO_ENTSO(RRR,T);
SHEDDING_FORBIDDEN_DUE_TO_ENTSO(RRR,T) = no;

SET SHEDDING_FORBIDDEN_TOTAL(RRR,T);
SHEDDING_FORBIDDEN_TOTAL(RRR,T) = no;

* Control which kind of shedding is used
SCALAR SHEDDING_FORBIDDEN_OPT;
$IFTHENI %SHEDDING_OPT% == 'NoConstraint'
  SHEDDING_FORBIDDEN_OPT =0;
$ELSEIFI %SHEDDING_OPT% == 'OnlyImport'
  SHEDDING_FORBIDDEN_OPT =1;
$ELSEIFI %SHEDDING_OPT% == 'ImportAndENTSOE'
  SHEDDING_FORBIDDEN_OPT = 2;
$ELSE
$ ABORT SHEDDING_OPT has to be either 'ImportAndENTSOE', 'OnlyImport', or 'NoConstraint'.
$ENDIF
*end version

* Fuel price for current simulation period:
PARAMETER IFUELPRICE_Y(AAA,FFF,T);

*Brennstofftransportkosten
PARAMETER IFUELTRANS_Y(AAA,G,T);


* Emission Taxes for current simulation period:
PARAMETER IM_POL(MPOLSET,C,T);

*Use value for PtG:
$ifi '%PtG%' ==  Yes  PARAMETER IUSEVALUE(AAA,G);

* Inflow in hydroreservoirs :
PARAMETER IHYDROINFLOW_AAA_T(AAA,T);

* Run of river distributed on areas:
PARAMETER IRUNRIVER_AAA_T(AAA,T);

* Parameters for consumption function of used fuel
PARAMETER IFUELUSAGE_SLOPE(AAA,G);
PARAMETER IFUELUSAGE_SECTION(AAA,G);

* Parameter for loops (considered time steps of BASETIME)
PARAMETER IBASESTARTTIME   'Real start time for optimization (value between 1 and 8760)';
PARAMETER IBASEFINALTIME   'Real final time for optimisation (value between 1 and 8760)';

PARAMETER INO_HOURS_OPT_PERIOD        'Number of hours of optimisation period';
PARAMETER INO_HOURS_WITH_FIXED_VGELEC 'Number of hours when vgelec is fixed before the problem is solved';

* parameter used for output files
PARAMETER ISTATUSSOLV             'For the solution status';
PARAMETER ISTATUSMOD              'For saving the solution status of the model' ;
PARAMETER IBASEFINALTIMEWRITEOUT  'Time up to which we write out the variables of the current loop';

* parameter used for splitting tidal generation on several plants of type Tidal
PARAMETER Totcap_Tidal_Nres(RRR);
PARAMETER Share_Tidal_Nres(G,RRR);
loop(y,
 Totcap_Tidal_NRES(RRR) = sum((AAA,IGTIDALSTREAM),GKFXELEC(Y,AAA,IGTIDALSTREAM)$RRRAAA(RRR,AAA));
);
loop(y,
Share_Tidal_Nres(IGTIDALSTREAM,RRR)$(Totcap_Tidal_NRES(RRR)>0) = sum(AAA$RRRAAA(RRR,AAA),GKFXELEC(Y,AAA,IGTIDALSTREAM)/Totcap_Tidal_NRES(RRR));
);


* ------Parameters for rolling planning --------------
PARAMETER IFLEXIBLE_DEF_YES       'Binary parameter for model with flexible electricity demand' ;
PARAMETER IUPDATE_SHADOWPRICE_YES 'Binary parameter for update shadow prices';
PARAMETER ICALC_SHADOWPRICE_YES   'Binary parameter for calculation of shadow price';
PARAMETER IFIXSPINNING_UNITS_YES  'Binary parameter for indication those rolling planning periods, when demand is fixed' /0/;

PARAMETER IBID_DAYAHEADMARKET_YES 'BINARY Parameter for indicating if spotmarket is optimised';
PARAMETER INO_SOLVE               'Parameter counting the number of runs';

PARAMETER ILOOPINDICATOR                 'Parameter that identifies day-ahead or intraday loop';
*for 24/36-looping: day-ahead-loop: 0; intraday-loop: 1
*for weekly-looping: not used
*for 3hours-looping: day-ahead-loop: 0; intraday-loops: 1-7

PARAMETER ISTOP_LOOP                    ' Parameter to exit loop structure'   /0/;
PARAMETER IKNOWN_OUTAGE(AAA,G)          ' Binary Parameter, to determine if the outage started in the previous loop';
PARAMETER INO_TIMESTEPSTRANSFER_OUTAGE  ' Parameter to determine information about unplanned outages';

IKNOWN_OUTAGE(AAA,G) = 0;

PARAMETER IGELECCAPEFF(AAA,G,TTT);

* Parameter holding the start-up time of a unit at the beginning of a planning loop
PARAMETER IGSTARTUPTIME(AAA,G);

Scalar Starttimecounter  /0/;
Parameter Start_OnOff(AAA,G);

$ifi %UnitCmin%==yes $INCLUDE '%code_path_addons%/Mip/MIP_InternalParameters.inc'

*{ This file contains:
* PARAMETERS
*   UCINI(AAA,G)  'Number of hours the unit has been up (>0) or down (<0) at the beginning of a new planning loop'
*   IUCMININIU(AAA,G) 'Unit commitment, UnitCmin: internal parameter, initial remaining uptime'
*   IUCMININID(AAA,G) 'Unit commitment, UnitCmin: internal parameter, initial remaining downtime'
*   VGONLINE_SUM2(AAA,G)
*   VGONLINE_SUM3(AAA,G)
* $ifi %UnitCramp%==yes   UCRAMPINI(AAA,G) 'Unit commitment: UnitCramp: Initial state'
* $ifi %UnitCscc%==yes   Set IGUCSCC(G)  'Set of power plants with outtime dependant start-up times';
*}


*Set reserve capacity to relative values for LP runs
*$ifi '%UnitCmin%' == NO GDATA(IA,IGDISPATCH,'GDMAXSpinResPri') = GDATA(IA,IGDISPATCH,'GDMAXSpinResPri') / SUM(Y, GKFXELEC(Y,IA,IGDISPATCH) + 1$(GKFXELEC(Y,IA,IGDISPATCH) = 0));
*$ifi '%UnitCmin%' == NO GDATA(IA,IGDISPATCH,'GDMAXSpinResSec') = GDATA(IA,IGDISPATCH,'GDMAXSpinResSec') / SUM(Y, GKFXELEC(Y,IA,IGDISPATCH) + 1$(GKFXELEC(Y,IA,IGDISPATCH) = 0));

$ifi '%Code_version%' == EWL GDATA(IA,IGDISPATCH,'GDMAXSpinResPri') = GDATA(IA,IGDISPATCH,'GDMAXSpinResPri') * SUM(Y, GKFXELEC(Y,IA,IGDISPATCH));
$ifi '%Code_version%' == EWL GDATA(IA,IGDISPATCH,'GDMAXSpinResSec') = GDATA(IA,IGDISPATCH,'GDMAXSpinResSec') * SUM(Y, GKFXELEC(Y,IA,IGDISPATCH));

*----- End of internal sets and parameters ------------------------------------
*}

* Output Files *{
* Prepare intermediate values and output file possibilities:

* The following file contains definitions of logical file names
* and the associated names of files
* that may be used for printout of simulation results.
* It also contains various definitions that are useful
* for the production and layout of the output.

$offlisting
$INCLUDE '%code_path_printinc%print_file_definition.inc';
$onlisting
*}

* Variables *{
*------------------------------------------------------------------------------
*  VARIABLES
*------------------------------------------------------------------------------

* The following variables are found by optimisation in each optimisation run (possibly incomplete list)
*VARIABLES
*   VOBJ                                  'Objective function value (MMoney)'
*   VGELEC_T(T,AAA,G)                     'Electricity generation (MW) fixed on day-ahead market'
*   VGE_ANCPOS(DAY,AAA,G)                 'Spinning capacity reserved for providing positive primary reserve (day-ahead market) (MW)'
*   VGE_ANCNEG(DAY,AAA,G)                 'Spinning capacity reserved for providing negative primary reserve (day-ahead market) (MW)'
*   VGELEC_CONSUMED_T(T,AAA,G)            'Loading of electricity storage or heat pump fixed on day-ahead market (MW)'
*   VWINDSHEDDING_DAY_AHEAD(T,RRR)        'Windshedding day-ahead';
*   VSOLARSHEDDING_DAY_AHEAD(T,RRR)       'Solarshedding day-ahead'
*   VXELEC_T(T,IRRRE,IRRRI)               'Electricity export from region IRRRE to IRRRI (MW)'
*   VGELEC_DPOS_T(T,AAA,G)                'Up regulation (Positive deviation) of electricity generation (MW)'
*   VGELEC_DNEG_T(T,AAA,G)                'Down regulation (Negative deviation) of electricity generation (MW)'
*   VGELEC_CONSUMED_DPOS_T(T,AAA,G)       'Increased loading of electricity storage or heat pump (MW)'
*   VGELEC_CONSUMED_DNEG_T(T,AAA,G)       'Decreased loading of electricity storage or heat pump (MW)'
*   VXELEC_DPOS_T(T,IRRRE,IRRRI)          'Electricity export of up regulation'
*   VXELEC_DNEG_T(T,IRRRE,IRRRI)          'Electricity export of down regulation'
*   VWINDCUR_ANCPOS(T,RRR)                'Curtailment of wind power in order to provide positive spinning reserve'
*   VGE_SPIN_ANCPOS(T,AAA,G)              'Spinning capacity reserved for providing positive non-spinning reserve (MW)'
*   VGE_NONSPIN_ANCPOS(T,AAA,G)           'Non-spinning capacity reserved for providing positive non-spinning reserve (MW)'

*   VGHEAT_T(T,AAA,G)                     'Heat generation (MW'
*   VGOUTPUT_T(T,AAA,G)                   'Output of the unit per h in MW
*   VGOUTPUT_APPROX_T(T,AAA,G,IHRS)       'Output of the unit per h in MW in block IHRS of the piecewise fuel use function
*   VGFUELUSAGE_T(T,AAA,G)                'Fuel usage per h in (MW)'

*   VGONLINE_T(TTT,AAA,G)                 'Capacity online (MW)'
*   VGONLCHARGE(TTT,AAA,G)                'Pumped hydro storage operating in pumping mode (individual pump)'
*   VGSTARTUP_T(T,AAA,G)                  'Capacity started up from time step T-1 to T (MW), or (binary) start-up process started at beginning of this hour'
*   VGSTARTUPFUELCONS(T,AAA,G)            'Start-up fuel consumption of power plant G and hour T'

*   VXE_NONSPIN_ANCPOS(T,IRRRE,IRRRI)     'Electricity export of positive non-spinning reserve'
*   VDEMANDELECFLEXIBLE_T(T,RRR,DEF)      'Flexible electricity demands (MW)'

*   VCONTENTSTORAGE_T(T,AAA,G)           'Content of electricity storage at time t (MWh)'
*   VCONTENTHYDRORES_T(T,AAA)          'Content of hydro reservoir (MWh)'

*   VGELEC_CONSUMED_ANCPOS(AAA,G,DAY)     'Reservation of decreased loading capacity of electricity storage or heat pump for providing positive primary reserve (MW)'
*   VGELEC_CONSUMED_ANCNEG(AAA,G,DAY)     'Reservation of increased loading capacity of electricity storage or heat pump for providing negative primary reserve (MW)'

*   VGHEAT_CONSUMED_T(T,AAA,G)            'Loading of heat storage (MW)'
*   VGELEC_CONSUMED_NONSP_ANCPOS(AAA,G,T) 'Reservation of decreased loading capacity of electricity storage or heat pump for providing positive non-spinning reserve (MW)'

*   VHYDROSPILLAGE(T,AAA)                 'Spillage of hydropower due to overflow in reservoirs'

*   VQDAYAHEAD(T,RRR,IPLUSMINUS)          'Feasibility of QEEQDAY'
*   VQINTRADAY(T,RRR,IPLUSMINUS)          'Feasibility of QEEQINT'
*   VQHEQ(T,AAA,IPLUSMINUS)               'Feasibility of QHEQ'
*   VQNONSP_ANCPOSEQ(T,RRR,IPLUSMINUS)    'Feasibility of QANC_NONSP_POSEQ'
*;


FREE     VARIABLE VOBJ;

POSITIVE VARIABLE VGELEC_T(T,AAA,G) ;
POSITIVE VARIABLE VGELEC_DPOS_T(T,AAA,G);
POSITIVE VARIABLE VGELEC_DNEG_T(T,AAA,G);
POSITIVE VARIABLE VGE_NONSPIN_ANCPOS(T,AAA,G);
POSITIVE VARIABLE VGE_ANCPOS(T,AAA,G);
POSITIVE VARIABLE VGE_ANCNEG(T,AAA,G);
POSITIVE VARIABLE VGE_ANCPOS_PRI(T,AAA,G);
POSITIVE VARIABLE VGE_ANCPOS_SEC(T,AAA,G);
POSITIVE VARIABLE VGE_ANCNEG_PRI(T,AAA,G);
POSITIVE VARIABLE VGE_ANCNEG_SEC(T,AAA,G);
POSITIVE VARIABLE VGE_SPIN_ANCNEG(T,AAA,G);
POSITIVE VARIABLE VGE_SPIN_ANCPOS(T,AAA,G);

POSITIVE VARIABLE VGHEAT_T(T,AAA,G) ;
$ifi '%CCGT_Imp%' == Yes POSITIVE VARIABLE VGSTEAM_T(T,AAA,G);
POSITIVE VARIABLE VGFUELUSAGE_T(T,AAA,G);

$ifi '%CCGT_Imp%' == Yes  POSITIVE VARIABLE CCGT_STEAM(T,CCGT_Group);

$ifi not '%UnitCmin%'  == Yes  POSITIVE VARIABLE VGSTARTUP_T(TTT,AAA,G);
$ifi     '%UnitCmin%'  == Yes  BINARY  VARIABLE VGSTARTUP_T(TTT,AAA,G);

POSITIVE VARIABLE VGOUTPUT_APPROX_T(T,AAA,G,IHRS);
POSITIVE VARIABLE VGSTARTUPFUELCONS(T,AAA,G);
POSITIVE VARIABLE VGOUTPUT_T(T,AAA,G);
$ifi not '%UnitCmin%'  == Yes  POSITIVE VARIABLE VGONLINE_T(TTT,AAA,G)  ;
$ifi     '%UnitCmin%'  == Yes  BINARY  VARIABLE VGONLINE_T(TTT,AAA,G)  ;
$ifi     '%UnitCmin%'  == Yes  BINARY  VARIABLE VGONLCHARGE(TTT,AAA,G)  ;
$ifi     '%UnitCmin%'  == Yes POSITIVE VARIABLE VQESTOLOAD1(T,AAA,G,IPLUSMINUS);

*------------- Variables for different transport representation -------------
POSITIVE VARIABLE VXELEC_T(T,IRRRE,IRRRI);
POSITIVE VARIABLE VXELEC_DPOS_T(T,IRRRE,IRRRI);
POSITIVE VARIABLE VXELEC_DNEG_T(T,IRRRE,IRRRI);
POSITIVE VARIABLE VXE_NONSPIN_ANCPOS(T,IRRRE,IRRRI);
POSITIVE VARIABLE VXE_NONSPIN_ANCNEG(T,IRRRE,IRRRI);

$ifi '%LFB%' == Yes POSITIVE VARIABLE VXELEC_DC_T(T,IRRRE,IRRRI);
$ifi '%LFB%' == Yes POSITIVE VARIABLE VXELEC_DC_DPOS_T(T,IRRRE,IRRRI);
$ifi '%LFB%' == Yes POSITIVE VARIABLE VXELEC_DC_DNEG_T(T,IRRRE,IRRRI);
$ifi '%LFB%' == Yes FREE VARIABLE VXELEC_EXPORT_T(T,RRR)
$ifi '%LFB%' == Yes POSITIVE VARIABLE VXELEC_EXPORT_DPOS_T(T,RRR)
$ifi '%LFB%' == Yes POSITIVE VARIABLE VXELEC_EXPORT_DNEG_T(T,RRR)

* -------------------------------------------------------------------------------------------
POSITIVE VARIABLE VDEMANDELECFLEXIBLE_T(T,RRR,DEF);
POSITIVE VARIABLE VGELEC_CONSUMED_T(T,AAA,G);
POSITIVE VARIABLE VGELEC_CONSUMED_DPOS_T(T,AAA,G);
POSITIVE VARIABLE VGELEC_CONSUMED_DNEG_T(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_ANCPOS(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_ANCNEG(T,AAA,G);

POSITIVE VARIABLE VGE_CONSUMED_ANCPOS_PRI(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_ANCPOS_SEC(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_ANCNEG_PRI(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_ANCNEG_SEC(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_NONSP_ANCNEG(T,AAA,G);
POSITIVE VARIABLE VGE_CONSUMED_NONSP_ANCPOS(T,AAA,G);

$ifi     '%UnitCmin%'==Yes BINARY   VARIABLE VGTURBINE_ON(T,AAA,G);
$ifi Not '%UnitCmin%'==Yes POSITIVE VARIABLE VGTURBINE_ON(T,AAA,G) ;

POSITIVE VARIABLE VGHEAT_CONSUMED_T(T,AAA,G) ;
Positive VARIABLE VCONTENTSTORAGE_T(T,AAA,G);
* for DSM, storage content may get negative, therefore redefined as free variable through following statement
VCONTENTSTORAGE_T.lo(T,AAA,IGELECSTODSM)=-INF;

POSITIVE VARIABLE VCONTENTHYDRORES_T(T,AAA);
* New variables that allow deviations from the hydro reference reservoi level within the choice HydroPenalty=Yes
$ifi '%HydroPenalty%' == YES POSITIVE VARIABLE VQRESLEVELUP(T,AAA,IHYDFLXSTEP)   'Allows deviation from the hydro reference reservoir level';
$ifi '%HydroPenalty%' == YES POSITIVE VARIABLE VQRESLEVELDOWN(T,AAA,IHYDFLXSTEP) 'Allows deviation from the hydro reference reservoir level';

POSITIVE VARIABLE VWINDSHEDDING_DAY_AHEAD(T,RRR);
POSITIVE VARIABLE VSOLARSHEDDING_DAY_AHEAD(T,RRR);
$ifi '%Renewable_Spinning%' == Yes POSITIVE VARIABLE VWINDCUR_ANCPOS(T,RRR);
POSITIVE VARIABLE VHYDROSPILLAGE(T,AAA);
POSITIVE VARIABLE VHYDROSPILLAGE_NEG(T,AAA);

POSITIVE VARIABLE VGRAMP_UP(TTT,AAA,G);
POSITIVE VARIABLE VGRAMP_DOWN(TTT,AAA,G);

POSITIVE VARIABLE VQDAYAHEAD(T,RRR,IPLUSMINUS);
POSITIVE VARIABLE VQINTRADAY(T,RRR,IPLUSMINUS);
*POSITIVE VARIABLE VQUC4(T,AAA,G,IPLUSMINUS);
POSITIVE VARIABLE VQMAXCAP(T,AAA,G,IPLUSMINUS);
POSITIVE VARIABLE VQHEQ(T,AAA,IPLUSMINUS);
POSITIVE VARIABLE VQNONSP_ANCPOSEQ(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQNONSP_ANCNEGEQ(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCPOSEQ(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCNEGEQ(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCPOSEQ_PRI(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCNEGEQ_PRI(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCPOSEQ_SEC(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQANCNEGEQ_SEC(T,TSO_RRR,IPLUSMINUS);
POSITIVE VARIABLE VQONLOP(TTT,AAA,G,IPLUSMINUS);
POSITIVE VARIABLE VQHEQNATGAS(T,AAA,IPLUSMINUS);
POSITIVE Variable VQBOUNDEMIN(T,AAA,G,IPLUSMINUS);
POSITIVE Variable VQBOUNDEMAX(T,AAA,G,IPLUSMINUS);
$ifi '%LFB%' == Yes POSITIVE VARIABLE PTDF_SLACK_FLG(TRL,T,IPLUSMINUS);

POSITIVE VARIABLE VQMINCAP(T,AAA,G,IPLUSMINUS);

$ifi '%CCGT_Imp%' == Yes  POSITIVE VARIABLE VQCCGT (T,CCGT_Group,IPLUSMINUS);

VDEMANDELECFLEXIBLE_T.UP(T,IR,DEF_D) = IDEF_STEPS_QUANT(IR,DEF_D);

$offlisting
$ifi '%FLEX_addon%' == YES               $INCLUDE '%code_path_addons%/FLEX/FLEX_addon_core.inc';
$onlisting

*----- End of variables -------------------------------------------------------
*}

* Equations *{
* Declarations *{

*------------------------------------------------------------------------------
*  EQUATIONLIST1
*------------------------------------------------------------------------------

EQUATIONS

   QOBJ                            'Objective function'

   QEEQDAY(T,RRR)                  'Day-ahead market: Electricity generation equals demand (IT_SPOTPERIOD)'
   QEEQINT(T,RRR)                  'Intraday market: Electricity generation equals demand'
   QHEQ(T,AAA)                     'Heat generation equals demand'

   QHEQNATGAS(T,AAA)               'Heat generation from natural gas driven heat boilers and natural gas driven CHP plants covers demand'
$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ(T,TSO_RRR)            'Primary+secondary reserve covers demand for positive reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ(T,TSO_RRR)            'Primary+secondary reserve covers demand for negative reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ_PRI(T,TSO_RRR)        'Primary reserve covers demand for positive primary reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ_SEC(T,TSO_RRR)        'Secondary reserve covers demand for positive secondary reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ_PRI(T,TSO_RRR)        'Primary reserve covers demand for negative primary reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ_SEC(T,TSO_RRR)        'Secondary reserve covers demand for negative secondary reserve (IT_SPOTPERIOD)'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_PEN(T,TSO_RRR,IPLUSMINUS) 'Penalty for primary and secondary reserve fits penalty on spinning reserve'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_PEN(T,TSO_RRR,IPLUSMINUS) 'Penalty for primary and secondary reserve fits penalty on spinning reserve'
$ifi not %NoReserveRestrictions%==YES    QNONSP_ANCNEGEQ(T,TSO_RRR)      'Spinning and non-spinning reserve covers demand for non-spinning negative reserve '
$ifi not %NoReserveRestrictions%==YES    QNONSP_ANCPOSEQ(T,TSO_RRR)      'Spinning and non-spinning reserve covers demand for non-spinning positive reserve '

   QGMAXCAPDISPATCH(T,AAA,G)             'Limit for electric generation for units having unit commitment restrictions (capacity online )'
   QGMAXCAPDISPATCH_CHP(T,AAA,G)         'Limit for electric generation for units having unit commitment restrictions (capacity online ) only for CHP'
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes   QGMAXCAP_CHP(T,AAA,G)                 'Upper Limit for electric generation for CHP units'
   QGMAXCAPDISPATCHDAYAHEAD(T,AAA,G)      'Maximum capacity for production sold on the day-ahead market'
   QGMINCAPDISPATCH(T,AAA,G)             'Limit for electric generation for units having unit commitment restrictions (Minimum production)(capacity online )'
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes   QGMINCAP_CHP(T,AAA,G)             'Lower Limit for electric generation for CHP units'
   QGMinGen(T,AAA,G)                                 'Minimum Generation of Units'
   QGMINCAPDISPATCH_CHP(T,AAA,G)         'Limit for electric generation for units having unit commitment restrictions (Minimum production)(capacity online ) only CHP'

   QGCBGEXT(T,AAA,G)          'CHP generation (extraction) limited by Cb-line'
   QGCBGBPR(T,AAA,G)          'CHP generation (back pressure) limited by Cb-line'
   QGCAPHEAT(T,AAA,G)         'Maximum capacity on heat production'
$ifi '%CCGT_Imp%' == Yes QGCAPSTEAM(T,AAA,G)      'Maximum capacity on steam production'

   QGONLCAPMAX(T,AAA,G)       'Online capacity lower than available capacity'
   QGONLCAPMIN(T,AAA,G)       'Online capacity greater than must-run value (all units) - must-run other than from heat extraction'

   QGONLCAPMIN_CHP(T,AAA,G)   'Online capacity greater than must-run value from heat extraction'

$ifi '%CCGT_Imp%' == Yes   QGCCGT_STEAM(T,CCGT_Group) 'Determination of Steam Production of CCGT'
$ifi '%CCGT_Imp%' == Yes   QGCCGT_ST(T,CCGT_Group) 'Production of Steamturbine uses complete Steam Production of CCGT'

   QWINDSHED(T,RRR)          'Wind Shedding planned on day-ahead market lower than expected wind power production'
   QSOLARSHED(T,RRR)         'Solar Shedding planned on day-ahead market lower than expected solar power production'

* Energiedumping durch Export
   QDumpLimDAY(T,RRR)        'Dump energy is smaller than generation, no export of dump energy'
   QDump_SHEDDING(T,RRR)     'Conditional shadding, see also SHEDDING_OPT in choices.gms'

   QGCAPELEC_Wind(T,RRR)     'Wind shedding lower than wind power production forecast'
   QGCAPELEC_Solar(T,RRR)    'Solar shedding lower than solar production'

   QGNEGDEV(T,AAA,G)          'Down-regulation limited by production sold on day-ahead market'
   QXNEGDEV(T,RRR,RRR)        'Reduction lower than transmission'

$ifi '%LFB%' == Yes   QXNEGDEVDC(T,RRR,RRR)      'Reduction lower than transmission (DC lines in Flow-based market coupling'

   QGCAPELEC4(T,AAA,G)             'Maximum capacity for power to electricity storage bought on the day-ahead market'

   QGCAPELEC4Hyres(T,AAA,G)  'Maximum capacity for power to reservoir bought on the day-ahead market'

$ifi not %NoReserveRestrictions%==YES    QGCAPNON_SPIN(T,AAA,G)      'Non-spinning positive reserve lower than available capacity when unit is off-line'

$ifi not %NoReserveRestrictions%==YES    QGANCPOS_PRI(T,AAA,G)        'Provision of primary positive spinning reserve restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SEC(T,AAA,G)        'Provision of secondary positive spinning reserve restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_PRI(T,AAA,G)        'Provision of primary negative spinning reserve restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SEC(T,AAA,G)        'Provision of secondary negative spinning reserve restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_CONS_PRI(T,AAA,G)   'Provision of primary positive spinning reserve though IGELECSTORAGE consuming restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_CONS_SEC(T,AAA,G)   'Provision of secondary positive spinning reserve though IGELECSTORAGE consuming restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_CONS_PRI(T,AAA,G)   'Provision of primary negative spinning reserve though IGELECSTORAGE consuming restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_CONS_SEC(T,AAA,G)   'Provision of secondary negative spinning reserve though IGELECSTORAGE consuming restricted'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SUM(T,AAA,G)      'Sum of primary reserve and secondary reserve is spinning reserve'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SUM(T,AAA,G)      'Sum of primary reserve and secondary reserve is spinning reserve'
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SUM_STO(T,AAA,G)  'Sum of primary reserve and secondary reserve is spinning reserve'
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SUM_STO(T,AAA,G)  'Sum of primary reserve and secondary reserve is spinning reserve'

   QGOUTPUTUSINGFUEL(T,AAA,G)        'Fuel usage - calculation of output needed for calculation of fuel usage'
   QGFUELUSENONLINEAR(T,AAA,G)        'Fuel usage for power plants with more than one incremental heat rate slope'
   QGFUELUSELINEAR(T,AAA,G)        'Fuel usage for power plants with only one incremental heat rate slope'
   QUC07(T,AAA,G)             'Prod equal to the sum of prod in each segment of the piecewise linear fuel consumption curve'
   QGOUTPUTNONLINEAR1(T,AAA,G)             'Rightborder of the first segment of the piecewise linear fuel consumption curve'
   QGOUTPUTNONLINEAR2(T,AAA,G,IHRS)        'Rightborder of each segment (except the first) of the piecewise linear fuel consumption curve'

$ifi not '%NoStartUpCosts%' == Yes    QGONLSTART(T,AAA,G)        'Capacity started up(t) > capacity online (t)-capacity online(t-1)'
$ifi not '%NoOpTimeRestrictions%' ==  Yes QGONLOP(T,T_WITH_Hist,AAA,G) 'Minimum operation time '
$ifi not '%NoOpTimeRestrictions%' ==  Yes QGONLOP2(T,T_WITH_Hist,UTIME,AAA,G) 'Minimum operation time (for LP)'
$ifi not '%NoOpTimeRestrictions%' ==  Yes QGONLSD(T,T_WITH_HIST,AAA,G) 'Minimum shut down time '
$ifi not '%NoOpTimeRestrictions%' ==  Yes  QGONLSD2(T,T_WITH_HIST,DTIME,AAA,G) 'Minimum shut down time (for LP)'
   QGONLSLOW(T,AAA,G)         'Vgonline same for all slow units'

   QGRAMP(T,AAA,G)            'Ramping production upwards is restricted'

   QGSTARTRAMP(T,AAA,G)          'Start-up ramp upwards'
   QGSTARTRAMP_CHP(T,AAA,G)      'Start-up ramp upwards'

   QGGETOH(T,AAA,G)           'Electric heat pump'

   QHYRSSEQ(T,AAA)            'Hydropower reservoir dynamic equation'
   QHYRSMAXCON(T,AAA)         'Hydropower with reservoir max content'
   QHYRSMINCON(T,AAA)         'Hydropower with reservoir min content'

   QHYRSMAXPROD(T,AAA)        'Sum of run of river production and regulated production lower than capacity'


$ifi '%Looping%' == week   QHSTOEnd_2(T,AAA,G)    'Storage level at the end of the week <= 80%'
$ifi '%Looping%' == week   QHSTOEndSmall(T,AAA,G) 'Small hydro storages (capacity for 24 hours or less): storage level at the end of the week > 20%'
$ifi '%Looping%' == week   QHSTOEndLarge(T,AAA,G) 'Large hydro storages (capacity for more than 24hours): storage level at the end of the week = 50%'

*Maximum and Minimum hydroreservoir level at endtime(T)
$ifi '%HydroPenalty%' == YES   QHYRSEndUpper(T,AAA)       'Maximum hydroreservoir level at end of loop'
$ifi '%HydroPenalty%' == YES   QHYRSEndLower(T,AAA)       'Minimum hydroreservoir level at end of loop'

   QHYRESPUMPCHLIM_Pos(T,AAA) 'Limitation of VGELEC_consumed_dpos for Hydroreservoirs'
   QHYRESPUMPCHLIM(T,AAA) 'Limitation of VGELEC_consumed_dneg for Hydroreservoirs'
   QHYPmpMax(T,AAA) 'Additional limitation for pumping of Hydroreservoirs'
   QHYGenMax(T,AAA) 'Additional limitation for generation of Hydroreservoirs'
*---TYNDP specific Hydro equations to meet weekly pumping restrictions--------*
$ifi '%Looping%' == week   QHYPmpMax_E(AAA,G)
$ifi '%Looping%' == week   QHYPmpMIN_E(AAA,G)

   QESTOVOLT(T,AAA,G)         'Electricty storage dynamic equation (MWh)'

   QESTOLOADC(T,AAA,G)        'Loading capacity > realised loading + loading reserved'
   QESTOLOADA(T,AAA,G)         'Decreased loading + decreased loading reserved < day-ahead loading + increased loading'

   QESTOLOADA_HyRes(T,AAA,G)   'Hydroreservoirs: Decreased loading + decreased loading reserved < day-ahead loading + increased loading'

   QESTOMAXCO(T,AAA,G)        'Maximum content of electricity storage'
   QESTOMINCO(T,AAA,G)        'Energy content in storage enough to cover 1 hour delivery of primary and secondary reserve'

   QDSMVOLT(T,AAA,G)         'Dynamic quation (MWh) for industrial DSM, heatpumps and electric vehicles'
   QDSMLOADC(T,AAA,G)        'DSM: Loading capacity > realised loading + loading reserved'
   QDSMLOADA(T,AAA,G)        'DSM: Decreased loading + decreased loading reserved < day-ahead loading + increased loading'
   QDSMMAXCO(T,AAA,G)        'DSM: Maximum content of storage'
   QDSMMINCO(T,AAA,G)        'DSM: Energy content enough to cover 1 hour delivery of primary and secondary reserve and energy content in EV-storage should be enough, that all starting vehicles could leave with a predefined SOC'
   QGCAPELECDSM4(T,AAA,G)    'DSM: Maximum capacity for power bought on the day-ahead market'

   QHSTOVOLT(T,AAA,G)         'Heat storage dynamic equation (MWh)'
   QHSTOLOADC(T,AAA,G)        'Capacity for loading process of heat storage(MW)'
   QHSTOMAXCO(T,AAA,G)        'Maximum content of heat storage'

   QESTONOMIX1(T,AAA,G)       'Avoid mixed use of pump and turbine of a storage '
   QESTONOMIX2(T,AAA,G)       'Avoid mixed use of pump and turbine of a storage '
   QESTONOMIX3(T,AAA,G)       'Avoid mixed use of pump and turbine of a storage '

   QGRAMP_UP(T,AAA,G)         'Ramping production upwards is penalized'
   QGRAMP_DOWN(T,AAA,G)       'Ramping production downwards is penalized'

;

* "Dec" for declaration
$INCLUDE '%code_path_addons%/LOAD_Flow/EQUDec_GroupNTC.inc';
$ifi '%No_Load_Flow%' == Yes            $INCLUDE '%code_path_addons%/LOAD_FLOW/EQUDec_No_Load_Flow.inc';
$ifi '%LFB_NE%'       == YES            $INCLUDE '%code_path_addons%/LOAD_Flow/EQUDec_LFB_netexport.inc';
$ifi '%LFB_BEX%'      == YES            $INCLUDE '%code_path_addons%/LOAD_Flow/EQUDec_LFB_BEX.inc';
*}

* Definitions *{
*------------------------------------------------------------------------------
*  DEFINITION OF EQUATIONS
*------------------------------------------------------------------------------

* ---- QOBJ: Objective function (Variables:
* VGFUELUSAGE_T, VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T, VGHEAT_T,
* VGSTARTUPFUELCONS, VGSTARTUP_T, VGONLINE_T,
* VCONTENTSTORAGE_T, VCONTENTHYDRORES_T, VWINDCUR_ANCPOS,
* VXELEC_T, VXELEC_DPOS_T, VXELEC_DNEG_T,
* VGELEC_CONSUMED_T, VGELEC_CONSUMED_DPOS_T, VGELEC_CONSUMED_DNEG_T,
* VDEMANDELECFLEXIBLE_T,
* VQINTRADAY, VQMAXCAP, VQONLOP, VQDAYAHEAD, VQANCPOSEQ, VQANCNEGEQ, VQHEQ, VQHEQNATGAS, VQNONSP_ANCPOSEQ, VQESTOLOAD1) *{

QOBJ ..

   VOBJ =E=

* #1 (VGFUELUSAGE_T)
* Cost of fuel consumption during optimisation period:

   SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$( GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')),
      SUM( T $IT_OPT(T),
        (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUSINGFUEL,T)) *
          VGFUELUSAGE_T(T,IA,IGUSINGFUEL)
        )
      )

* #2 (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Operation and maintainance cost:
* attention: seperate calculation of maintentance costs as not for all units if IGELEC vgELEC_DNEG is defined
* ensure that we have one unit for IGRUNOFRIVER per area and one unit IGSOLAR, IGTIDALSTREAM, IGWAVE in each region

  + SUM(IAGK_Y(IA,IGDISPATCH),
      SUM(T $IT_OPT(T),
         GDATA(IA,IGDISPATCH,'GDOMVCOST')*
          (VGELEC_T(T,IA,IGDISPATCH) + VGELEC_DPOS_T(T,IA,IGDISPATCH) - VGELEC_DNEG_T(T,IA,IGDISPATCH))
      )
    )


* #3+4 (VGHEAT_T)
* relevant for IACHP_ENDO

  + SUM(IAGK_Y(IA,IGHEAT),
      SUM(T $IT_OPT(T),
       (GDATA(IA,IGHEAT,'GDOMVCOST') * GDATA(IA,IGHEAT,'GDCV') * VGHEAT_T(T,IA,IGHEAT))$IGELECANDHEAT(IGHEAT)
       + (GDATA(IA,IGHEAT,'GDOMVCOST') * VGHEAT_T(T,IA,IGHEAT))$IGHEATONLY(IGHEAT)
      )
    )


* #5 (VGSTARTUP_T) Start-up variable costs:
$ifi not '%NoStartUpCosts%' == Yes  + SUM(IAGK_Y(IA,IGUC),
$ifi not '%NoStartUpCosts%' == Yes      SUM(T $IT_OPT(T),
$ifi not '%NoStartUpCosts%' == Yes        GDATA(IA,IGUC,'GDSTARTUPCOST')*
$ifi not '%NoStartUpCosts%' == Yes  IGELECCAPEFF(IA,IGUC,T)*
$ifi not '%NoStartUpCosts%' == Yes        VGSTARTUP_T(T,IA,IGUC)
$ifi not '%NoStartUpCosts%' == Yes      )
$ifi not '%NoStartUpCosts%' == Yes    )


* #6 (modified by ITRANSMISSION_INTRADAY_YES)  (VXELEC_T, VXELEC_DPOS_T, VXELEC_DNEG_T)
* Transmission cost:

  + SUM(IEXPORT_Y(IRE,IRI),
      SUM(T $IT_OPT(T), XCOST(IRE,IRI)* (
                                                                         0
$ifi '%No_Load_Flow%' ==   Yes    +  VXELEC_T(T,IRE,IRI)    + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T(T,IRE,IRI)    -VXELEC_DNEG_T(T,IRE,IRI))
$ifi '%LFB_BEX%'      ==   Yes    +  VXELEC_T(T,IRE,IRI)    + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T(T,IRE,IRI)    -VXELEC_DNEG_T(T,IRE,IRI))
$ifi '%LFB_BEX%'      ==   Yes    +  VXELEC_DC_T(T,IRE,IRI) + ITRANSMISSION_INTRADAY_YES *(VXELEC_DC_DPOS_T(T,IRE,IRI) -VXELEC_DC_DNEG_T(T,IRE,IRI))
$ifi '%LFB_NE%'       ==   Yes    +  0
                                                                 )
                 )
    )

* #7 (VGFUELUSAGE_T)
* Tax on fuel usage for producing power and heat (condensing and chp) in Germany.
* Used fuel * fuel tax * 3.6, which is a multiplier from MWh to GJ:
* relevant for IACHP_ENDO

  +  SUM(sameas(C,'DE'),
       SUM(IAGK_Y(IA,IGUSINGFUEL)$(IGELECTHERMAL(IGUSINGFUEL) AND ICA(C,IA)),
           SUM( T $IT_OPT(T),
               GDATA(IA,IGUSINGFUEL,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T(T,IA,IGUSINGFUEL)
           )
        )
   )


* #8 (VGELEC_CONSUMED_T, VGELEC_CONSUMED_DPOS_T, VGELEC_CONSUMED_DNEG_T)
* Electricity tariff on electricity used for heating (heat pumps and electrical boilers):

   +  SUM(C,
         SUM(IAGK_Y(IA,IGHEATPUMP)$ICA(C,IA),
            SUM( T $IT_OPT(T),TAX_HEATPUMP(C)*
               (VGELEC_CONSUMED_T(T,IA,IGHEATPUMP) + VGELEC_CONSUMED_DPOS_T(T,IA,IGHEATPUMP)- VGELEC_CONSUMED_DNEG_T(T,IA,IGHEATPUMP))
            )
         )
      )


* #9 (VGFUELUSAGE_T)
* Emission taxes CO2, SO2:

   +  SUM(C,
         SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$( (GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA)),
            SUM( T $IT_OPT(T),
               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2') * IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGUSINGFUEL,'GDDESO2'))) * CONVERT_KG_PER_GJ_T_PER_MWH *
               VGFUELUSAGE_T(T,IA,IGUSINGFUEL)
            )
         )
      )


* #10 (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Subsidy on electricity produced using biomass or waste: (values of ELEC_SUBSIDY <0)

   +  SUM(C,
         SUM(FFF,
            SUM(IAGK_Y(IA,IGUSINGFUEL)$( (GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND IGELEC(IGUSINGFUEL)),
               SUM( T $IT_OPT(T), ELEC_SUBSIDY(C,FFF)*
                 (VGELEC_T(T,IA,IGUSINGFUEL) + VGELEC_DPOS_T(T,IA,IGUSINGFUEL) - VGELEC_DNEG_T(T,IA,IGUSINGFUEL))
               )
            )
         )
      )


*Currently not in use - Revenue for unit being online at the end of optimization horizon:
*$ontext
  - SUM( (IAGK_Y(IA,IGUC),FFF)$( GDATA(IA,IGUC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')),
     SUM(T $IENDTIME(T), GDATA(IA,IGUC,'GDSTARTUPFUELCONSHOT') * (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUC,T)) *
     IGELECCAPEFF(IA,IGUC,T) *
         VGONLINE_T(T,IA,IGUC)
     )
   )
*$offtext

* Revenue for hydroreservoir
  - SUM(IAHYDRO_HYDRORES,
        SUM(T $IENDTIME_HYDISDP(T),  ISDP_HYDRORES(IAHYDRO_HYDRORES,T)
                                    *  (VCONTENTHYDRORES_T(T,IAHYDRO_HYDRORES) - VCONTENTHYDRORES_T('T00',IAHYDRO_HYDRORES))
       )
      )

$ifi '%HydroSupplyCurves%' == YES    + sum (IAHYDROGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES),
$ifi '%HydroSupplyCurves%' == YES           SUM(T $IT_OPT(T),
$ifi '%HydroSupplyCurves%' == YES                   ISDP_HYDRORES_STEP(IAHYDRO_HYDRORES_STEP,IGHYDRORES,T)*(VGELEC_T(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES) + VGELEC_DPOS_T(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES) - VGELEC_DNEG_T(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES))
$ifi '%HydroSupplyCurves%' == YES           )
$ifi '%HydroSupplyCurves%' == YES   )


* Penalty for deviation from reservoir reference level:
* 0-1% =  0.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 1-2% =  1.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 2-3% =  2.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* ...
* 9-10% = 9.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* --> penalty increases (100% -->300%) as storage capacity decreases (1000 flh --> 333 flh), allowing "weekly storages" (low storage capacity) a less flexible operation than "annual storages" (high storage capacity)
$ifi '%HydroPenalty%' == YES   + SUM((IAHYDRO,T)$IENDTIME(T),
$ifi '%HydroPenalty%' == YES         sum(IHYDFLXSTEP, VQRESLEVELUP(T,IAHYDRO,IHYDFLXSTEP)  *(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES       + sum(IHYDFLXSTEP, VQRESLEVELDOWN(T,IAHYDRO,IHYDFLXSTEP)*(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES   )

* # 13+14 (VCONTENTSTORAGE_T)

  - SUM(IAGK_Y(IA,IGSTORAGE),
      SUM(T $IENDTIME_HYDISDP(T),
$ifi '%Code_version%'==EWL     (ISDP_STORAGE(IA,IGSTORAGE)*
$ifi '%Code_version%'==UENB        ((   ISDP_STORAGE(IA,IGSTORAGE)$(ISDP_STORAGE(IA,IGSTORAGE) GT 0)
$ifi '%Code_version%'==UENB           +          ISDP_REFPRICE(IA)$(ISDP_STORAGE(IA,IGSTORAGE) LE 0)) *
           VCONTENTSTORAGE_T(T,IA,IGSTORAGE))
      )
    )

* Changes in consumers' utility relative to electricity consumption:

  + IFLEXIBLE_DEF_YES *
    (
      SUM(IR,
        SUM(T $IT_SPOTPERIOD(T),
          SUM(DEF_D, VDEMANDELECFLEXIBLE_T(T,IR,DEF_D)* IDEFLEXIBLEPRICE_T(IR,DEF_D))
          - SUM(DEF_U, VDEMANDELECFLEXIBLE_T(T,IR,DEF_U)* IDEFLEXIBLEPRICE_T(IR,DEF_U))
        )
      )
    )

* #17a Operation and maintainance costs of renewables
  + SUM(IR,
      SUM(T $IT_OPT(T),
         (
          ITIDALSTREAM_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGTIDALSTREAM) $RRRAAA(IR,IA), GDATA(IA,IGTIDALSTREAM,'GDOMVCOST'))
          + IWAVE_VAR_T(IR,T)      * SUM(IAGK_Y(IA,IGWAVE) $RRRAAA(IR,IA), GDATA(IA,IGWAVE,'GDOMVCOST'))
          + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), GDATA(IA,IGDISGEN,'GDOMVCOST'))
          + IBIOMASS_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + IOthRes_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + ISOLARTH_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGSOLARTH) $RRRAAA(IR,IA), GDATA(IA,IGSOLARTH,'GDOMVCOST'))
          + SUM(IAGK_Y(IA,IGRUNOFRIVER) $RRRAAA(IR,IA), GDATA(IA,IGRUNOFRIVER,'GDOMVCOST') * IRUNRIVER_AAA_T(IA,T))
          + ISOLAR_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGSOLAR) $RRRAAA(IR,IA), GDATA(IA,IGSOLAR,'GDOMVCOST'))
          + IGEOTHHEAT_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGGEOTHHEAT) $RRRAAA(IR,IA), GDATA(IA,IGGEOTHHEAT,'GDOMVCOST'))
$ifi '%Code_version%'==UENB  + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'%Sonstige_NEE%',T)+IFUELTRANS_Y(IA,IGDISGEN,T))  )
$ifi '%Code_version%'==EWL   + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'DISGEN',T) +IFUELTRANS_Y(IA,IGDISGEN,T)) )
        )
      )
    )


* #17b (VWINDCUR_ANCPOS, VGELEC_DNEG_T): operation and maintainance costs of production from wind
   + SUM(IR,
       SUM(T $IT_OPT(T),
         SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA), GDATA(IA,IGWIND,'GDOMVCOST')*
           (IWIND_REALISED_IR(IR,T)
$ifi '%Renewable_Spinning%' == Yes           - VWINDCUR_ANCPOS(T,IR)
           - VGELEC_DNEG_T(T,IA,IGWIND))
        )
      )
    )

* #18a (VGSTARTUPFUELCONS and VGSTARTUP_T, resp.)
* Start-up fuel costs: for power plants without outtime dependant start-up fuel consumption
* Set IGNOUCSCC = IGUC if UnitCscc=No. IGNOUCSCC=IGUC-IGUCSCC if UnitCscc=Yes

$ifi not '%NoStartUpCosts%' == Yes   + SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$( GDATA(IA,IGNOUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')),
$ifi not '%NoStartUpCosts%' == Yes      SUM( T $IT_OPT(T), (IFUELPRICE_Y(IA,FFF,T) + IFUELTRANS_Y(IA,IGNOUCSCC,T)) *
$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes        VGSTARTUPFUELCONS(T,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes        GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T(T,IA,IGNOUCSCC) * IGELECCAPEFF(IA,IGNOUCSCC,T)
$ifi not '%NoStartUpCosts%' == Yes      )
$ifi not '%NoStartUpCosts%' == Yes   )

* #18b (VGSTARTUPFUELCONS)
* Start-up fuel costs: for power plants with outtime dependant start-up fuel consumption

$ifi not '%NoStartUpCosts%' == Yes $ifi %UnitCscc%==yes   + SUM( (IAGK_Y(IA,IGUCSCC),FFF)$( GDATA(IA,IGUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')),
$ifi not '%NoStartUpCosts%' == Yes $ifi %UnitCscc%==yes      SUM( T $IT_OPT(T),  (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUCSCC,T))*
$ifi not '%NoStartUpCosts%' == Yes $ifi %UnitCscc%==yes         VGSTARTUPFUELCONS(T,IA,IGUCSCC)))

* #19a (VGSTARTUP_T bzw. VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (see #9):
$ifi not '%NoStartUpCosts%' == Yes    +  SUM(C,
$ifi not '%NoStartUpCosts%' == Yes          SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$( (GDATA(IA,IGNOUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA)),
$ifi not '%NoStartUpCosts%' == Yes             SUM( T $IT_OPT(T),
$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes  GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T(T,IA,IGNOUCSCC)*IGELECCAPEFF(IA,IGNOUCSCC,T)*
$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes  VGSTARTUPFUELCONS(T,IA,IGNOUCSCC)*
$ifi not '%NoStartUpCosts%' == Yes                (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGNOUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi not '%NoStartUpCosts%' == Yes             )
$ifi not '%NoStartUpCosts%' == Yes          )
$ifi not '%NoStartUpCosts%' == Yes       )

* #19b (VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (out-time dependant start-up):
$ifi %UnitCscc%==yes   +  SUM(C,
$ifi %UnitCscc%==yes         SUM( (IAGK_Y(IA,IGUCSCC),FFF)$( (GDATA(IA,IGUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA)),
$ifi %UnitCscc%==yes            SUM( T $IT_OPT(T), VGSTARTUPFUELCONS(T,IA,IGUCSCC)*
$ifi %UnitCscc%==yes               (FDATA(FFF,'FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi %UnitCscc%==yes            )
$ifi %UnitCscc%==yes         )
$ifi %UnitCscc%==yes      )


* #19c (constant)
* emmission taxed CO2, SO2 for distributed generation
  +    SUM(C,
          SUM( (IAGK_Y(IA,IGDISGEN))$(ICA(C,IA)),
             SUM( T $IT_OPT(T),   SUM(IR $RRRAAA(IR,IA),IDISGEN_VAR_T(IR,T))*(1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) *
$ifi '%Code_version%'==EWL      (FDATA('DISGEN','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('DISGEN','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi '%Code_version%'==UENB     (FDATA('%Sonstige_NEE%','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('%Sonstige_NEE%','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
             )
          )
       )

* Cost components from FLEX addons
$ifi '%FLEX_addon%' == Yes   + SUM(T$IT_OPT(T), VCOST_FLEX(T))

* #20 (VGFUELUSAGE_T)
* Tax reduction on fuel usage for producing power and heat in chp (applied to Germany).
* Fuel use * fuel tax * 3.6, which is a multiplier from GJ to MWH:
  -  SUM(sameas(C,'DE'),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA)),
           SUM( T $IT_OPT(T),
              GDATA(IA,IGELECANDHEAT,'GDFUELTAXRED')* 3.6 * VGFUELUSAGE_T(T,IA,IGELECANDHEAT)
           )
        )
    )

* #21 (VGHEAT_T)
* Tax on fuel usage for producing heat in chp production.
* Produced heat * fuel tax (conversion efficiency from fuel to heat included)
* * 3.6, which is a multiplier from GJ to MWH. Applies not for Germany:
  +  SUM(C $(Not sameas(C,'DE')),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA)),
           SUM( T $IT_OPT(T),
              GDATA(IA,IGELECANDHEAT,'GDFUELTAX') * 3.6 * VGHEAT_T(T,IA,IGELECANDHEAT)
           )
        )
     )

* #22 (VGHEAT_T)
* Subsidy for CHP Plants (KWK-Zuschlag), only for Germany.
* Produced heat * CHP coefficient * CHP-subsidy
  -  SUM(sameas(C,'DE'),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA)),
            SUM( T $IT_OPT(T),
               GDATA(IA,IGELECANDHEAT,'GDCB') *
               (GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY1') + GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY2') + GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY3')) *
               VGHEAT_T(T,IA,IGELECANDHEAT)
            )
        )
     )

* #23 Tax on fuels of heatboilers (VGFUELUSAGE_T)
* Used fuel * fuel tax * 3.6, which is a multiplier from GJ to MWH:
  +  SUM(C,
        SUM(IAGK_Y(IA,IGHEATBOILER)$(ICA(C,IA)),
           SUM( T $IT_OPT(T),
             GDATA(IA,IGHEATBOILER,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T(T,IA,IGHEATBOILER)
           )
        )
     )

* #24: Revenues for PtG
$ifi '%PtG%' ==  Yes  - SUM(IAGK_Y(IA,IGPTG),
$ifi '%PtG%' ==  Yes     SUM(T $IT_OPT(T),
$ifi '%PtG%' ==  Yes         IUSEVALUE(IA,IGPTG)*(VGELEC_T(T,IA,IGPTG) + VGELEC_DPOS_T(T,IA,IGPTG) - VGELEC_DNEG_T(T,IA,IGPTG))/GDATA(IA,IGPTG,'GDLOADEFF')
$ifi '%PtG%' ==  Yes      )
$ifi '%PtG%' ==  Yes    )



* Infeasibility penalties / slacks / VQ variables:
* (VQDAYAHEAD)
   + SUM(IR $(IBID_DAYAHEADMARKET_YES EQ 1),
        SUM(T $IT_SPOTPERIOD(T),
                (IPENALTY_ENS * VQDAYAHEAD(T,IR,'IMINUS') + IPENALTY_DUM * VQDAYAHEAD(T,IR,'IPLUS'))
           )
      )

* (VQINTRADAY)
   + SUM(IR,
        SUM(T $IT_OPT(T),
            IPENALTY * (VQINTRADAY(T,IR,'IMINUS') + VQINTRADAY(T,IR,'IPLUS'))
           )
      )

* (VQMAXCAP, VQMINCAP)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        SUM(T $IT_OPT(T),
              IPENALTYCAP * VQMAXCAP(T,IA,IGDISPATCH,'IPLUS')
            + IPENALTYCAP * VQMINCAP(T,IA,IGDISPATCH,'IMINUS')
           )
      )

* (VQBOUNDEMAX, VQBOUNDEMIN)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        SUM(T $IT_OPT(T),
              IPENALTYBOUNDE * VQBOUNDEMAX(T,IA,IGDISPATCH,'IPLUS')
            + IPENALTYBOUNDE * VQBOUNDEMIN(T,IA,IGDISPATCH,'IMINUS')
           )
      )

* (VQONLOP)
  +  SUM(IAGK_Y(IA,IGMINOPERATION_NOALWAYS),
        SUM(T_WITH_HIST $(ORD(T_WITH_HIST) LE (24 + INO_HOURS_OPT_PERIOD + 1)),
            IPENALTY *
IGELECCAPACITY_Y(IA,IGMINOPERATION_NOALWAYS)*
            (VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IPLUS') + VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IMINUS'))
            )
      )

* (VQANCPOSEQ, VQANCNEGEQ)
    + SUM((TSO_RRR,T),
        IPENALTY_SpinRes * (VQANCPOSEQ(T,TSO_RRR,'IPLUS') + VQANCPOSEQ(T,TSO_RRR,'IMINUS'))
      + IPENALTY_SpinRes * (VQANCNEGEQ(T,TSO_RRR,'IPLUS') + VQANCNEGEQ(T,TSO_RRR,'IMINUS'))
     )

* (PTDF_SLACK)

$ifi '%LFB%' ==  Yes   +  Sum(TRL,
$ifi '%LFB%' ==  Yes        SUM(T,IPENALTYPTDF * (PTDF_SLACK_FLG(TRL,T,'IPLUS') + PTDF_SLACK_FLG(TRL,T,'IMINUS'))
$ifi '%LFB%' ==  Yes        )
$ifi '%LFB%' ==  Yes      )

* (VQHEQ)
  +  SUM(IA,
        SUM(T $IT_OPT(T),
           IPENALTYHEAT * (VQHEQ(T,IA,'IPLUS')+ VQHEQ(T,IA,'IMINUS'))
        )
      )

* (VQHEQNATGAS)
  +  SUM(IA,
        SUM(T $IT_OPT(T),
           IPENALTYHEAT * VQHEQNATGAS(T,IA,'IMINUS')
        )
     )

* (VQNONSP_ANCPOSEQ)
   +  SUM(TSO_RRR,
        SUM(T $IT_OPT(T),
           IPENALTY1 * (VQNONSP_ANCPOSEQ(T,TSO_RRR,'IPLUS')+VQNONSP_ANCPOSEQ(T,TSO_RRR,'IMINUS'))
           )
      )

* (VQNONSP_ANCNEGEQ)
   +  SUM(TSO_RRR,
        SUM(T $IT_OPT(T),
           IPENALTY1 * (VQNONSP_ANCNEGEQ(T,TSO_RRR,'IPLUS')+VQNONSP_ANCNEGEQ(T,TSO_RRR,'IMINUS'))
           )
      )

* (VQESTOLOAD1)
$ifi '%UnitCmin%'==Yes  + SUM(IAGK_Y(IA,IGELECSTORAGE),
$ifi '%UnitCmin%'==Yes        SUM(T $IT_OPT(T),
$ifi '%UnitCmin%'==Yes               IPENALTY * (VQESTOLOAD1(T,IA,IGELECSTORAGE,'IMINUS') + VQESTOLOAD1(T,IA,IGELECSTORAGE,'IPLUS'))
$ifi '%UnitCmin%'==Yes        )
$ifi '%UnitCmin%'==Yes    )

* Penalty for wind shedding
  + SUM(IR,
      SUM(T $IT_OPT_ALL(T),  IPENALTYWINDCURT *
            (
$ifi '%Renewable_Spinning%' == Yes           VWINDCUR_ANCPOS(T,IR) +
            VWINDSHEDDING_DAY_AHEAD(T,IR))
      )
    )

* Penalty for solar shedding
   + SUM(IR,
      SUM(T $IT_OPT_ALL(T),  IPENALTY_SOLAR *
            VSOLARSHEDDING_DAY_AHEAD(T,IR)
      )
    )

* Penalty for large ramps
  + SUM(IAGK_Y(IA,IGDISPATCH),
      SUM( T $IT_OPT(T), IPENALTYRAMP *
        (VGRAMP_UP(T,IA,IGDISPATCH) + VGRAMP_DOWN(T,IA,IGDISPATCH))
      )
    )

*Penalty for Spillage
   + SUM(IA,
        SUM(T $IT_OPT(T),
            IHydro_Fill*VHYDROSPILLAGE_NEG(T,IA))
           )

    + SUM(IA,
        SUM(T $IT_OPT(T),
            IHydro_Dum*VHYDROSPILLAGE(T,IA))
           )

*(VQCCGT) (Penalty for waste of heat in Case of no Bypass)
$ifi '%CCGT_Imp%' == Yes    + Sum(CCGT_Group,
$ifi '%CCGT_Imp%' == Yes      Sum(T $IT_OPT(T),
$ifi '%CCGT_Imp%' == Yes        (1-IG_Bypass(CCGT_Group)) * IPENALTY_CCGT_STEAM * VQCCGT (T,CCGT_Group,'IMINUS')
$ifi '%CCGT_Imp%' == Yes        +(IPENALTY_CCGT_STEAM * VQCCGT(T,CCGT_Group,'IPLUS'))
$ifi '%CCGT_Imp%' == Yes      )
$ifi '%CCGT_Imp%' == Yes    )


;

*}
*}

*------------------------ Balance equations -------------------------------------------*{

* ---- QEEQDAY ---- Balance equation for electricity on the day-ahead market ---- *{
* dispatchable production (including delivery from electricity storages)
* + average fluctuating production + expected wind - wind shedding + import_region
* = fixed export third country  + consumption for heatpumps + loading electricity storages + demand + export_region

QEEQDAY(T,IR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

   SUM(IAGK_Y(IA,IGDISPATCH)$(RRRAAA(IR,IA)AND NOT IGPTG(IGDISPATCH)),
     VGELEC_T(T,IA,IGDISPATCH))
   + ITIDALSTREAM_VAR_T(IR,T) + IWAVE_VAR_T(IR,T) + IRUNRIVER_VAR_T(IR,T) + ISOLAR_VAR_T(IR,T) + IGEOTHHEAT_VAR_T(IR,T) + IDISGEN_VAR_T(IR,T)
   + ISOLARTH_VAR_T(IR,T) + IBiomass_VAR_T(IR,T) + IOthRes_Var_T(IR,T)
   + IWIND_BID_IR(IR,T)

   - IVWINDSHEDDING_DAYAHEAD_YES * (VWINDSHEDDING_DAY_AHEAD(T,IR)
$ifi '%Renewable_Spinning%' == Yes   +VWINDCUR_ANCPOS(T,IR)
   )
   - IVSOLARSHEDDING_DAYAHEAD_YES * VSOLARSHEDDING_DAY_AHEAD(T,IR)

$ifi     '%No_Load_Flow%' == Yes   + SUM(IEXPORT_NTC_Y(IRE,IR),  VXELEC_T(T,IRE,IR)*(1-XLOSS(IRE,IR)))
$ifi Not '%No_Load_Flow%' == Yes   + SUM(IEXPORT_NTC_Y(IRE,IR),  VXELEC_T(T,IRE,IR)*(1-XLOSS(IRE,IR)))
$ifi     '%LFB_BEX%'      == Yes   + SUM(IEXPORT_FLBMC_Y(IRE,IR),VXELEC_T(T,IRE,IR))
$ifi     '%LFB%'          == Yes   + SUM(IEXPORT_FLBMC_DC_Y(IRE,IR), VXELEC_DC_T(T,IRE,IR))

     =E=

   IX3COUNTRY_T_Y(IR,T)

   + SUM(IAGK_Y(IA,IGESTORAGE_DSM)$RRRAAA(IR,IA),
       VGELEC_CONSUMED_T(T,IA,IGESTORAGE_DSM)
       )

   + SUM(IAGK_Y(IA,IGHydrores)$RRRAAA(IR,IA),
       VGELEC_CONSUMED_T(T,IA,IGHydrores)
       )

   + (IDEMANDELEC_BID_IR(IR,T)
       + IFLEXIBLE_DEF_YES * (SUM(DEF_U,VDEMANDELECFLEXIBLE_T(T,IR,DEF_U))
                             - SUM(DEF_D,VDEMANDELECFLEXIBLE_T(T,IR,DEF_D)))
      )/(1-DISLOSS_E(IR))

$ifi     '%No_Load_Flow%' ==  Yes  + SUM(IEXPORT_NTC_Y(IR,IRI),VXELEC_T(T,IR,IRI))
$ifi Not '%No_Load_Flow%' ==  Yes  + SUM(IEXPORT_NTC_Y(IR,IRI),VXELEC_T(T,IR,IRI))
$ifi     '%LFB%'          ==  Yes  + SUM(IEXPORT_FLBMC_DC_Y(IR,IRI), VXELEC_DC_T(T,IR,IRI))
$ifi     '%LFB_NE%'       ==  Yes  + VXELEC_EXPORT_T(T,IR)$(RRR_FLBMC(IR))
$ifi     '%LFB_BEX%'      ==  Yes  + SUM(IEXPORT_FLBMC_Y(IR,IRI),VXELEC_T(T,IR,IRI))

$ifi '%FLEX_addon%' == Yes + VDEMAND_FLEX(IR,T)

   + VQDAYAHEAD(T,IR,'IPLUS')
   - VQDAYAHEAD(T,IR,'IMINUS')
;
*}

* ---- QEEQINT ---- Balance equation for regulation on the intraday market ---- *{
* Regulation dispatchable units - Down regulation non-dipatchable units +
* Import regulation + Regulation heat pumps and electricity storages =
* Demand regulation + Export regulation

QEEQINT(T,IR) $(IT_OPT(T) AND (NOT(IBID_DAYAHEADMARKET_YES EQ 1)OR NOT IT_SPOTPERIOD(T)))..

   SUM(IAGK_Y(IA,IGDISPATCH_NOONLYDAYAHEAD)$(RRRAAA(IR,IA)AND NOT IGPTG(IGDISPATCH_NOONLYDAYAHEAD)),
      (  VGELEC_DPOS_T(T,IA,IGDISPATCH_NOONLYDAYAHEAD)
       - VGELEC_DNEG_T(T,IA,IGDISPATCH_NOONLYDAYAHEAD))
       )

   - SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),
         VGELEC_DNEG_T(T,IA,IGWIND) * IVWINDSHEDDING_Intraday_YES)

   - SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),
         VGELEC_DNEG_T(T,IA,IGSOLAR)* IVSOLARSHEDDING_Intrady_YES)

   + SUM(IAGK_Y(IA,IGHydrores)$RRRAAA(IR,IA),
      (  VGELEC_CONSUMED_DNEG_T(T,IA,IGHydrores)
      -  VGELEC_CONSUMED_DPOS_T(T,IA,IGHydrores))
       )

$ifi     '%No_Load_Flow%' == Yes  + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_NTC_Y(IRE,IR),(VXELEC_DPOS_T(T,IRE,IR)-VXELEC_DNEG_T(T,IRE,IR))*(1-XLOSS(IRE,IR)))
$ifi Not '%No_Load_Flow%' == Yes  + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_NTC_Y(IRE,IR),(VXELEC_DPOS_T(T,IRE,IR)-VXELEC_DNEG_T(T,IRE,IR))*(1-XLOSS(IRE,IR)))
$ifi     '%LFB%' == Yes           + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_FLBMC_DC_Y(IRE,IR),(VXELEC_DC_DPOS_T(T,IRE,IR)-VXELEC_DC_DNEG_T(T,IRE,IR)))
$ifi     '%LFB_BEX%' == Yes       + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_FLBMC_Y(IRE,IR),(VXELEC_DC_DPOS_T(T,IRE,IR)-VXELEC_DC_DNEG_T(T,IRE,IR)))

    =E=

  IWIND_BID_IR(IR,T) - IWIND_REALISED_IR(IR,T)

  + (IDEMANDELEC_VAR_T(IR,T) - IDEMANDELEC_BID_IR(IR,T))/(1-DISLOSS_E(IR))

  + IVGELEC_OUTAGE_T(IR,T)

$ifi     '%No_Load_Flow%' == Yes  + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_NTC_Y(IR,IRI), (VXELEC_DPOS_T(T,IR,IRI) - VXELEC_DNEG_T(T,IR,IRI)))
$ifi Not '%No_Load_Flow%' == Yes  + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_NTC_Y(IR,IRI), (VXELEC_DPOS_T(T,IR,IRI) - VXELEC_DNEG_T(T,IR,IRI)))
$ifi     '%LFB%' == Yes                   + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_FLBMC_DC_Y(IR,IRI), (VXELEC_DC_DPOS_T(T,IR,IRI) - VXELEC_DC_DNEG_T(T,IR,IRI)))
$ifi     '%LFB_NE%' == Yes        + ITRANSMISSION_INTRADAY_YES * (VXELEC_EXPORT_DPOS_T(T,IR) - VXELEC_EXPORT_DNEG_T(T,IR))$(RRR_FLBMC(IR))
$ifi     '%LFB_BEX%' == Yes       + ITRANSMISSION_INTRADAY_YES * SUM(IEXPORT_FLBMC_Y(IR,IRI),(VXELEC_DC_DPOS_T(T,IR,IRI)-VXELEC_DC_DNEG_T(T,IR,IRI)))

  + VQINTRADAY(T,IR,'IPLUS')
  - VQINTRADAY(T,IR,'IMINUS')
;
*}

* ---- QHEQ ---- Balance equations for heat areas ---- *{
* production backpressure + extraction + heat boilers + heat pumps + heat storages
* = Consumption of heat storages + heat demand +/- slacks

QHEQ(T,IACHP_ENDO) $IT_OPT(T)..

    SUM(IGHEAT $IAGK_Y(IACHP_ENDO,IGHEAT), VGHEAT_T(T,IACHP_ENDO,IGHEAT))

=E=

     SUM(IGHEATSTORAGE $IAGK_Y(IACHP_ENDO,IGHEATSTORAGE), VGHEAT_CONSUMED_T(T,IACHP_ENDO,IGHEATSTORAGE))
     + ( IDEMANDHEAT(IACHP_ENDO,T)) /(1-DISLOSS_H(IACHP_ENDO))
     + VQHEQ(T,IACHP_ENDO,'IPLUS')
     - VQHEQ(T,IACHP_ENDO,'IMINUS')
;
*}
*}

* --- QHEQNATGAS ---- Min restriction for heat areas (minimum operation of gas driven heat generators) ---- *{
* production backpressure (NAT_GAS) + production extraction (NAT_GAS) + production boilers (NAT_GAS)
* >=  minimum operation factor * th. capacities [backpressure (NAT_GAS) + extraction (NAT_GAS) + boilers (NAT_GAS)]/th. capacities [total] * heatdemand
QHEQNATGAS(T,IACHP_ENDO) $IT_OPT(T)..

         SUM(IGNATGAS $IAGK_Y(IACHP_ENDO,IGNATGAS),VGHEAT_T(T,IACHP_ENDO,IGNATGAS))
* If the secondary condition for NATGAS-CHPs is to be formulated more restrictively, remove the asterisk from the following line of code
*     - SUM(IGHEATBOILER $IAGK_Y(IA,IGHEATBOILER), VGHEAT_T(T,IA,IGHEATBOILER))
     =G=
     IDEMANDHEAT(IACHP_ENDO,T) * IFRACNATGAS_MIN(IACHP_ENDO) *
         ((
         SUM(IGNATGAS $IAGK_Y(IACHP_ENDO,IGNATGAS), IGHEATCAPACITY_Y(IACHP_ENDO,IGNATGAS))
* If the secondary condition for NATGAS-CHPs is to be formulated more restrictively, remove the asterisk from the following line of code
*         - SUM(IGHEATBOILER $IAGK_Y(IA,IGHEATBOILER), IGHEATCAPACITY_Y(IA,IGHEATBOILER))
         )/
         (

         SUM(IGHEAT $IAGK_Y(IACHP_ENDO,IGHEAT), IGHEATCAPACITY_Y(IACHP_ENDO,IGHEAT))

* If the secondary condition for NATGAS-CHPs is to be formulated more restrictively, remove the asterisk from the following line of code
*         - SUM(IGHEATBOILER $IAGK_Y(IA,IGHEATBOILER), IGHEATCAPACITY_Y(IA,IGHEATBOILER))
         + 1))
     - VQHEQNATGAS(T,IACHP_ENDO,'IMINUS')
         ;
*}

* ----------- Demand secondary reserve and primary reserve ---------------------
* ---- QANCPOSEQ positive demand for spinning reserve ---- *{
* Positive reserve spinning units (including positive reserve produced by
* planning to reduce the loading of electricity storage) + positive reserve delivered by reducing
* the consumption of heat pumps and the loading of electricity storages
* = Demand positive reserve

$ifi not %NoReserveRestrictions%==YES QANCPOSEQ(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES      SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGELEC)), VGE_ANCPOS(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES    + SUM(IAGK_Y(IA,IGESTORAGE_DSM) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCPOS(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES    + SUM(IAGK_Y(IA,IGHYDRORES) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCPOS(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES     IDEMAND_ANCPOS(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES     - VQANCPOSEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QANCPOSEQ_PRI positive demand for spinning reserve ---- *{
* Positive reserve spinning units (including positive reserve produced by
* planning to reduce the loading of electricity storage) + positive reserve delivered by reducing
* the consumption of heat pumps and the loading of electricity storages
* = Demand positive reserve PRIMARY

$ifi not %NoReserveRestrictions%==YES QANCPOSEQ_PRI(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES       SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGELEC)), VGE_ANCPOS_PRI(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGESTORAGE_DSM) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCPOS_PRI(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGHYDRORES) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCPOS_PRI(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES     IDEMAND_ANCPOS_PRI(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES     - VQANCPOSEQ_PRI(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QANCPOSEQ_SEC positive demand for spinning reserve ---- *{
* Positive reserve spinning units (including positive reserve produced by
* planning to reduce the loading of electricity storage) + positive reserve delivered by reducing
* the consumption of heat pumps and the loading of electricity storages
* = Demand positive reserve SECONDARY

$ifi not %NoReserveRestrictions%==YES QANCPOSEQ_SEC(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES       SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGELEC)), VGE_ANCPOS_SEC(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGESTORAGE_DSM) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCPOS_SEC(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGHYDRORES) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCPOS_SEC(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES     IDEMAND_ANCPOS_SEC(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES     - VQANCPOSEQ_SEC(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QANCNEGEQ Negative demand for spinning reserve ---- *{
* Negative reserve spinning units (including negative reserve produced by
* unloading of electricity storage) + negative reserve delivered by increasing
* the consumption of heat pumps and the loading of electricity storages
* = Demand negative reserve

$ifi not %NoReserveRestrictions%==YES QANCNEGEQ(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA)$IGSPINNING(IGELEC)), VGE_ANCNEG(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES  + SUM(IAGK_Y(IA,IGESTORAGE_DSM)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCNEG(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES  + SUM(IAGK_Y(IA,IGHYDRORES)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCNEG(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES   IDEMAND_ANCNEG(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES   -  VQANCNEGEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QANCNEGEQ_PRI negative demand for spinning reserve ---- *{
* Negative reserve spinning units (including negative reserve produced by
* unloading of electricity storage) + negative reserve delivered by increasing
* the consumption of heat pumps and the loading of electricity storages
* = Demand negative reserve PRIMARY

$ifi not %NoReserveRestrictions%==YES QANCNEGEQ_PRI(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES       SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGELEC)), VGE_ANCNEG_PRI(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGESTORAGE_DSM) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCNEG_PRI(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGHYDRORES) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCNEG_PRI(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES     IDEMAND_ANCNEG_PRI(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES     - VQANCNEGEQ_PRI(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QANCNEGEQ_SEC negative demand for spinning reserve ---- *{
* Negative reserve spinning units (including negative reserve produced by
* unloading of electricity storage) + negative reserve delivered by increasing
* the consumption of heat pumps and the loading of electricity storages
* = Demand negative reserve SECONDARY

$ifi not %NoReserveRestrictions%==YES QANCNEGEQ_SEC(T,TSO_RRR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES       SUM(IAGK_Y(IA,IGELEC)$(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGELEC)), VGE_ANCNEG_SEC(T,IA,IGELEC))
$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGESTORAGE_DSM) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGESTORAGE_DSM)), VGE_CONSUMED_ANCNEG_SEC(T,IA,IGESTORAGE_DSM))
$ifi not %NoReserveRestrictions%==YES         + SUM(IAGK_Y(IA,IGHYDRORES) $(TSO_RA(TSO_RRR,IA) $IGSPINNING(IGHYDRORES)), VGE_CONSUMED_ANCNEG_SEC(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES     IDEMAND_ANCNEG_SEC(TSO_RRR,T)
$ifi not %NoReserveRestrictions%==YES     - VQANCNEGEQ_SEC(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QNONSP_ANCPOSEQ Positive demand for non-spinning reserve (VGE_SPIN_ANCPOS, VGE_NONSPIN_ANCPOS, VXE_NONSPIN_ANCPOS, VQNONSP_ANCPOSEQ )---- *{

$ifi not %NoReserveRestrictions%==YES QNONSP_ANCPOSEQ(T,TSO_RRR) $((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    SUM(IAGK_Y(IA,IGSPINNING)$(TSO_RA(TSO_RRR,IA)), VGE_SPIN_ANCPOS(T,IA,IGSPINNING))

$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGDISPATCH) $(IGFASTUNITS(IGDISPATCH) AND (TSO_RA(TSO_RRR,IA))), VGE_NONSPIN_ANCPOS(T,IA,IGDISPATCH))

$ifi not %NoReserveRestrictions%==YES         + SUM(IAGK_Y(IA,IGESTORAGE_DSM)$(TSO_RA(TSO_RRR,IA)), VGE_CONSUMED_NONSP_ANCPOS(T,IA,IGESTORAGE_DSM))

$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGHYDRORES)$(TSO_RA(TSO_RRR,IA)), VGE_CONSUMED_NONSP_ANCPOS(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES     + ITRANSMISSION_NONSP_YES * SUM(IEXPORT_Y(IRE,IR)$(TSO_RRRRRR(TSO_RRR,IR) AND Not(TSO_RRRRRR(TSO_RRR,IRE))), (1-XLOSS(IRE,IR)) * VXE_NONSPIN_ANCPOS(T,IRE,IR))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES    IDEMAND_NONSPIN_ANCPOS(TSO_RRR,T)

$ifi not %NoReserveRestrictions%==YES    + ITRANSMISSION_NONSP_YES * SUM(IEXPORT_Y(IR,IRI)$(TSO_RRRRRR(TSO_RRR,IR) AND Not(TSO_RRRRRR(TSO_RRR,IRI))),(1-XLOSS(IR,IRI)) * VXE_NONSPIN_ANCPOS(T,IR,IRI))
$ifi not %NoReserveRestrictions%==YES    - VQNONSP_ANCPOSEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QNONSP_ANCNEGEQ Negative demand for non-spinning reserve (VGE_SPIN_ANCNEG, VGE_NONSPIN_ANC, VXE_NONSPIN_ANCNEG, VQNONSP_ANCNEGEQ )---- *{

$ifi not %NoReserveRestrictions%==YES QNONSP_ANCNEGEQ(T,TSO_RRR) $((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    SUM(IAGK_Y(IA,IGSPINNING)$(TSO_RA(TSO_RRR,IA)), VGE_SPIN_ANCNEG(T,IA,IGSPINNING))

$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGESTORAGE_DSM)$(TSO_RA(TSO_RRR,IA)), VGE_CONSUMED_NONSP_ANCNEG(T,IA,IGESTORAGE_DSM))

$ifi not %NoReserveRestrictions%==YES     + SUM(IAGK_Y(IA,IGHYDRORES)$(TSO_RA(TSO_RRR,IA)), VGE_CONSUMED_NONSP_ANCNEG(T,IA,IGHYDRORES))

$ifi not %NoReserveRestrictions%==YES     + ITRANSMISSION_NONSP_YES * SUM(IEXPORT_Y(IRE,IR)$(TSO_RRRRRR(TSO_RRR,IR) AND Not(TSO_RRRRRR(TSO_RRR,IRE))), (1-XLOSS(IRE,IR)) * VXE_NONSPIN_ANCNEG(T,IRE,IR))

$ifi not %NoReserveRestrictions%==YES   =E=

$ifi not %NoReserveRestrictions%==YES    IDEMAND_NONSPIN_ANCNEG(TSO_RRR,T)

$ifi not %NoReserveRestrictions%==YES    + ITRANSMISSION_NONSP_YES * SUM(IEXPORT_Y(IR,IRI)$(TSO_RRRRRR(TSO_RRR,IR) AND Not(TSO_RRRRRR(TSO_RRR,IRI))),(1-XLOSS(IR,IRI)) * VXE_NONSPIN_ANCNEG(T,IR,IRI))

$ifi not %NoReserveRestrictions%==YES    - VQNONSP_ANCNEGEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

*------ Generation technologies' electricity/heat operating area shape: -------

* --- Capacities *{

* ---- QGMAXCAPDISPATCH (formerly QGUC4), QGMAXCAPDISPATCH_CHP Production including reserves bounded by capacity/VGONLINE_T ---- *{
* Realised production + Tertiary positive reserve + non-spinning reserve (provided by spinning units) < maximum available capacity

QGMAXCAPDISPATCH(T,IAGK_Y(IA,IGDISPATCH)) $IT_OPT(T)..

    VGELEC_T(T,IA,IGDISPATCH)+ VGELEC_DPOS_T(T,IA,IGDISPATCH)- VGELEC_DNEG_T(T,IA,IGDISPATCH)
     + VGE_ANCPOS(T,IA,IGDISPATCH) $IGSPINNING(IGDISPATCH)
     + VGE_SPIN_ANCPOS(T,IA,IGDISPATCH)
     + VGE_NONSPIN_ANCPOS(T,IA,IGDISPATCH)$IGDISPATCH_NOUC(IGDISPATCH)

  =L=

(IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDispatch,T)* VGONLINE_T(T,IA,IGDISPATCH))$(IGUC(IGDISPATCH))
   + (IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDispatch,T))$(IGDISPATCH_NOUC(IGDISPATCH))
$ifi '%Code_version%'==UENB   - (VGHEAT_T(T,IA,IGDISPATCH) * GDATA(IA,IGDISPATCH,'GDCV'))$IGEXTRACTION(IGDISPATCH)
$ifi '%Code_version%'==EWL    - (VGHEAT_T(T,IA,IGDISPATCH) * GDATA(IA,IGDISPATCH,'GDCV'))$(IGEXTRACTION(IGDISPATCH) AND NOT IGFIXHEAT_EXT(IGDISPATCH))
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes - (IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDispatch,T)- IBOUNDE_MAX(IA,IGDISPATCH,T)- VQBOUNDEMAX(T,IA,IGDISPATCH,'IPLUS'))$IGFIXHEAT_EXT(IGDISPATCH)
   +  VQMAXCAP(T,IA,IGDISPATCH,'IPLUS')
;


$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes QGMAXCAP_CHP(T,AG_CHP_FixQ_Y(IA,IGDISPATCH)) $IT_OPT(T)..

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes    VGELEC_T(T,IA,IGDISPATCH)+ VGELEC_DPOS_T(T,IA,IGDISPATCH)- VGELEC_DNEG_T(T,IA,IGDISPATCH)
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes     + VGE_ANCPOS(T,IA,IGDISPATCH) $IGSPINNING(IGDISPATCH)
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes     + VGE_SPIN_ANCPOS(T,IA,IGDISPATCH)
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes     + VGE_NONSPIN_ANCPOS(T,IA,IGDISPATCH)$IGDISPATCH_NOUC(IGDISPATCH)

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes  =L=

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes IBOUNDE_MAX(IA,IGDISPATCH,T) + VQBOUNDEMAX(T,IA,IGDISPATCH,'IPLUS')
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes ;

QGMAXCAPDISPATCH_CHP(T,IAGK_Y(IA,IGDISPATCH)) $(IT_OPT(T) and IGCHPBOUNDE(IGDISPATCH))..

    VGELEC_T(T,IA,IGDISPATCH)+ VGELEC_DPOS_T(T,IA,IGDISPATCH)- VGELEC_DNEG_T(T,IA,IGDISPATCH)
     + VGE_ANCPOS(T,IA,IGDISPATCH) $IGSPINNING(IGDISPATCH)
     + VGE_SPIN_ANCPOS(T,IA,IGDISPATCH)
     + VGE_NONSPIN_ANCPOS(T,IA,IGDISPATCH)$IGDISPATCH_NOUC(IGDISPATCH)

  =L=

 IGELECCAPEFF_CHP(IA,IGDISPATCH,T)
;

* ---- QGMINCAPDISPATCH (formerly QGUC2), QGMINCAPDISPATCH_CHP: Production including reserves bounded by capacity/VGONLINE_T ---- *{
* Realised production - Primary negative reserve > Vgonline * Minimum load factor

QGMINCAPDISPATCH(T,IAGK_Y(IA,IGDISPATCH)) $(IT_OPT(T))..

    VGELEC_T(T,IA,IGDISPATCH) + VGELEC_DPOS_T(T,IA,IGDISPATCH)- VGELEC_DNEG_T(T,IA,IGDISPATCH)
    - VGE_ANCNEG(T,IA,IGDISPATCH) $IGSPINNING(IGDISPATCH)
    - VGE_SPIN_ANCNEG(T,IA,IGDISPATCH)

   =G=

    GDATA(IA,IGDISPATCH,'GDMINLOADFACTOR')*(
(IGELECCAPEFF(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH))$(IGUC(IGDISPATCH))
   )

*   + (GDATA(IA,IGDISPATCH,'GDMINLOADFACTOR')* VGTURBINE_ON(T,IA,IGDISPATCH) * IGELECCAPEFF(IA,IGDISPATCH,T))$IGHYRES_ELSTO(IGDISPATCH)
   - VQMINCAP(T,IA,IGDISPATCH,'IMINUS')
   - (VGHEAT_T(T,IA,IGDISPATCH) * GDATA(IA,IGDISPATCH,'GDCV'))$(IGEXTRACTION(IGDISPATCH))
;

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes QGMINCAP_CHP(T,AG_CHP_FixQ_Y(IA,IGDISPATCH)) $(IT_OPT(T))..

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes    VGELEC_T(T,IA,IGDISPATCH) + VGELEC_DPOS_T(T,IA,IGDISPATCH)- VGELEC_DNEG_T(T,IA,IGDISPATCH)
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes    - VGE_ANCNEG(T,IA,IGDISPATCH) $IGSPINNING(IGDISPATCH)

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes   =G=

$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes IBOUNDE_MIN(IA,IGDISPATCH,T) - VQBOUNDEMIN(T,IA,IGDISPATCH,'IMINUS')
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes ;


QGMINCAPDISPATCH_CHP(T,IAGK_Y(IA,IGCHPBOUNDE(IGDISPATCH))) $(IT_OPT(T))..

    VGELEC_T(T,IA,IGCHPBOUNDE) + VGELEC_DPOS_T(T,IA,IGCHPBOUNDE)- VGELEC_DNEG_T(T,IA,IGCHPBOUNDE)
    - VGE_ANCNEG(T,IA,IGCHPBOUNDE) $IGSPINNING(IGDISPATCH)
    - VGE_SPIN_ANCNEG(T,IA,IGCHPBOUNDE)

    =G=

    IMINCHPFACTOR(IA,IGCHPBOUNDE,T) * IGELECCAPACITY_Y(IA,IGCHPBOUNDE)
;

* ---- QGMinGen: additional minimum generation constraints, e.g. for swell water (not included in chp time series)

QGMinGen(T,IAGK_Y(IA,IGDISPATCH))$(IMinGen(IA,IGDISPATCH,T)>0)..

     VGELEC_T(T,IA,IGDISPATCH)
   + VGELEC_DPOS_T(T,IA,IGDISPATCH)
   - VGELEC_DNEG_T(T,IA,IGDISPATCH)

   =G=

$ifi     '%UnitCmin%' == Yes   IMinGen(IA,IGDISPATCH,T)*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGELECCAPEFF(IA,IGDISPATCH,T)*IGKDERATE(IA,IGDISPATCH,T)
$ifi not '%UnitCmin%' == Yes   IMinGen(IA,IGDISPATCH,T)*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGELECCAPEFF(IA,IGDISPATCH,T)
;

* ---- QGMAXCAPDISPATCHDAYAHEAD (formerly QGCAPELEC3): Day-ahead production bounded by capacity ---- *{

QGMAXCAPDISPATCHDAYAHEAD(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VGELEC_T(T,IA,IGDISPATCH)

  =L=

  IGELECCAPEFF(IA,IGDISPATCH,T)$(NOT(IGCHPBOUNDE(IGDISPATCH)))
  + IGELECCAPEFF_CHP(IA,IGDISPATCH,T)$(IGCHPBOUNDE(IGDISPATCH))
;
*}


* ---- QGONLCAPMAX Online capacity lower than available capacity ---- *{
* Capacity online < Maximum capacity

QGONLCAPMAX(T,IAGK_Y(IA,IGUC_NOALWAYS)) $IT_OPT(T)..

  VGONLINE_T(T,IA,IGUC_NOALWAYS)

  =L=
  
(CEIL(IGKDERATE(IA,IGUC_NOALWAYS,T) * (1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T))))$(NOT (IGCHPBOUNDE(IGUC_NOALWAYS)))
+ (CEIL(IGKDERATE(IA,IGUC_NOALWAYS,T) * (1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T))))$(IGCHPBOUNDE(IGUC_NOALWAYS))
;
*}

* ---- QGONLCAPMIN Online capacity greater than must-run value (all units) - must-run other than from heat extraction ---- *{
* Capacity online > Must-run value

QGONLCAPMIN(T,IAGK_Y(IA,IGUC_NOALWAYS)) $IT_OPT(T)..

  VGONLINE_T(T,IA,IGUC_NOALWAYS)

  =G=

  IMINVGONLINE(IA,IGUC_NOALWAYS,T)
;
*}

* ---- QGONLCAPMIN_CHP Online capacity greater than must-run value from heat extraction ---- *{
* Capacity online > Must-run value

QGONLCAPMIN_CHP(T,IAGK_Y(IA,IGUC_NOALWAYS)) $(IT_OPT(T) AND IACHP_Exo(IA) AND IGCHP_EXO(IGUC_NOALWAYS))..

  VGONLINE_T(T,IA,IGUC_NOALWAYS)$(IACHP_Exo(IA) AND IGCHP_EXO(IGUC_NOALWAYS))

  =G=

$ifi '%Code_version%'==UENB   + IMINCHPFACTOR(IA,IGUC_NOALWAYS,T)$(IGCHPBOUNDE(IGUC_NOALWAYS))
$ifi '%Code_version%'==UENB $ifi     '%CHP%' == Yes + ((IMINVGONLINE_CHP(IA,IGUC_NOALWAYS,T) - VGHEAT_T(T,IA,IGUC_NOALWAYS) * GDATA(IA,IGUC_NOALWAYS,'GDCV'))*0.999*(1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T)))$(IGFIXHEAT(IGUC_NOALWAYS))
$ifi '%Code_version%'==UENB   /max(IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)$(IGFIXHEAT(IGUC_NOALWAYS)),0.001)
$ifi '%Code_version%'==EWL  0
;
*}

* ---- QGCCGT_STEAM STEAMPRODUCTION in CCGT ---- *{
*Steam-Production equals the sum of produced Steam of all GT of CCGT_Group (conciders CCGT_etaSteamProd)
* - wasted heat (in case of no Bypass wasted heat is penalised)

$ifi '%CCGT_Imp%' == Yes  QGCCGT_STEAM(T,CCGT_Group) $IT_OPT(T)..

$ifi '%CCGT_Imp%' == Yes  CCGT_STEAM(T,CCGT_Group)
$ifi '%CCGT_Imp%' == Yes  =E=
$ifi '%CCGT_Imp%' == Yes  Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)) , (VGFUELUSAGE_T(T,IA,G) - VGOUTPUT_T(T,IA,G)))*CCGT_etaSteamProd
$ifi '%CCGT_Imp%' == Yes  + Sum(IAGK_Y(IA,G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_DB(G)), VGSTEAM_T(T,IA,G))
$ifi '%CCGT_Imp%' == Yes  -VQCCGT(T,CCGT_Group,'IMINUS')
$ifi '%CCGT_Imp%' == Yes  ;
*}

* ---- QGCCGT_ST Production of ST in CCGT ---- *{
*Production of ST equals availavle STEAM
$ifi '%CCGT_Imp%' == Yes  QGCCGT_ST(T,CCGT_Group) $IT_OPT(T)..

$ifi '%CCGT_Imp%' == Yes   Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)) , (VGOUTPUT_T(T,IA,G)*CCGT_slope_STEAM(IA,G)+CCGT_section_STEAM(IA,G)*VGONLINE_T(T,IA,G)))
$ifi '%CCGT_Imp%' == Yes  =E=
$ifi '%CCGT_Imp%' == Yes  CCGT_STEAM(T,CCGT_Group)+VQCCGT(T,CCGT_Group,'IPLUS')
$ifi '%CCGT_Imp%' == Yes  ;
*}

* ---- QGCAPSTEAM max capacity on steam (duct burner) ---- *{
*steam production < steam capacity

$ifi '%CCGT_Imp%' == Yes  QGCAPSTEAM(T,IAGK_Y(IA,IGDUCTBURNER)) $(IT_OPT(T))..

$ifi '%CCGT_Imp%' == Yes  VGSTEAM_T(T,IA,IGDUCTBURNER)

$ifi '%CCGT_Imp%' == Yes  =L=

$ifi '%CCGT_Imp%' == Yes  IGSTEAMCAPACITY_Y(IA,IGDUCTBURNER) * IGKDERATE(IA,IGDUCTBURNER,T) * (1-IG_UNP_OUTAGE(IA,IGDUCTBURNER,T))
$ifi '%CCGT_Imp%' == Yes  ;
*}

* ---- QGCBGEXT Extraction ---- *{
* Production day-ahead + Up regulation - Down regulation - Primary negative reserve
* > Heat production * CB-factor

$ifi '%Code_version%'==UENB QGCBGEXT(T,IAGK_Y(IA,IGEXTRACTION)) $((IT_OPT(T)) AND NOT(IGCHPBOUNDE(IGEXTRACTION)))..
$ifi '%Code_version%'==EWL QGCBGEXT(T,IAGK_Y(IA,IGEXTRACTION)) $((IT_OPT(T)) AND NOT(IGCHPBOUNDE(IGEXTRACTION))AND NOT (AG_CHP_FixQ_Y(IA,IGEXTRACTION)))..

   VGELEC_T(T,IA,IGEXTRACTION) + VGELEC_DPOS_T(T,IA,IGEXTRACTION) - VGELEC_DNEG_T(T,IA,IGEXTRACTION)
   - VGE_ANCNEG(T,IA,IGEXTRACTION) $IGSPINNING(IGEXTRACTION)

  =G=

   VGHEAT_T(T,IA,IGEXTRACTION) * GDATA(IA,IGEXTRACTION,'GDCB') * (1-IG_UNP_OUTAGE(IA,IGEXTRACTION,T))
;
*}

* ---- QGCBGBPR Backpressure ---- *{
* Production day-ahead + Up regulation - Down regulation = Heat production * CB-factor

$ifi '%Code_version%'==UENB QGCBGBPR(T,IAGK_Y(IA,IGBACKPR)) $((IT_OPT(T)) AND NOT(IGCHPBOUNDE(IGBACKPR)))..
$ifi '%Code_version%'==EWL QGCBGBPR(T,IAGK_Y(IA,IGBACKPR)) $((IT_OPT(T)) AND NOT(IGCHPBOUNDE(IGBACKPR))AND NOT (AG_CHP_FixQ_Y(IA,IGBACKPR)))..

    VGELEC_T(T,IA,IGBACKPR) + VGELEC_DPOS_T(T,IA,IGBACKPR) - VGELEC_DNEG_T(T,IA,IGBACKPR)

  =E=

    VGHEAT_T(T,IA,IGBACKPR) * GDATA(IA,IGBACKPR,'GDCB')* (1-IG_UNP_OUTAGE(IA,IGBACKPR,T))
;
*}

* ---- QGCAPHEAT Max capacity on heat ---- *{
* Heat production < heat capacity

QGCAPHEAT(T,IAGK_Y(IA,IGHEAT)) $(IT_OPT(T) AND IGFLEXHEAT(IGHEAT) AND IACHP_ENDO(IA))..

  VGHEAT_T(T,IA,IGHEAT)

  =L=

  IGHEATCAPACITY_Y(IA,IGHEAT) * IGKDERATE(IA,IGHEAT,T) * (1-IG_UNP_OUTAGE(IA,IGHEAT,T))
;
*}

*----------- Transmission (MW):------------------------------------------------
$ifi '%No_Load_Flow%' == Yes            $INCLUDE '%code_path_addons%/LOAD_FLOW/EQUDef_No_Load_Flow.inc';
$ifi '%LFB_NE%'       == YES            $INCLUDE '%code_path_addons%/LOAD_FLOW/EQUDEF_LFB_netexport.inc';
$ifi '%LFB_BEX%'      == YES            $INCLUDE '%code_path_addons%/LOAD_Flow/EQUDEF_LFB_BEX.inc';
$INCLUDE '%code_path_addons%/LOAD_FLOW/EQUDef_GroupNTC.inc';
*------------------------------------------------------------------------------

*{ These files contain:
* EQUDef_No_Load_Flow.inc: QXK1A, QXK2A
* EQUDef_GroupNTC.inc: QXK3A, QXK3B, QXK4A, QXK4B
* EQUDEF_LFB.inc: QXKPTDF10Plus, QXKPTDF10Minus
*}

* ---- QGANCPOS- / -NEG_PRI / _SEC: Capability of unit in providing positive / negative primary / secondary operating margin ---- *{

$ifi not %NoReserveRestrictions%==YES QGANCPOS_PRI(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES    VGE_ANCPOS_PRI(T,IA,IGDISPATCH)

$ifi not %NoReserveRestrictions%==YES    =L=

$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes    (GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH))$(not IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGTURBINE_ON(T,IA,IGDISPATCH))$(IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   (GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH) )$(IGUC(IGDISPATCH) and not IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH))*(IMAXCHPFACTOR(IA,IGDISPATCH,T)-IMINCHPFACTOR(IA,IGDISPATCH,T)) )$(IGUC(IGDISPATCH) and IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)*VGTURBINE_ON(T,IA,IGDISPATCH))$IGHYRES_ELSTO(IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCPOS_SEC(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES    VGE_ANCPOS_SEC(T,IA,IGDISPATCH)

$ifi not %NoReserveRestrictions%==YES    =L=

$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes     (GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH))$(not IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes   + (GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGTURBINE_ON(T,IA,IGDISPATCH))$(IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   (GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')* VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH) )$(IGUC(IGDISPATCH) and not IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH))*(IMAXCHPFACTOR(IA,IGDISPATCH,T)-IMINCHPFACTOR(IA,IGDISPATCH,T)) )$(IGUC(IGDISPATCH) and IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)*VGTURBINE_ON(T,IA,IGDISPATCH))$IGHYRES_ELSTO(IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_PRI(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES    VGE_ANCNEG_PRI(T,IA,IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES    =L=
$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes    (GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH))$(not IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGTURBINE_ON(T,IA,IGDISPATCH))$(IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   (GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH) )$(IGUC(IGDISPATCH) and not IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH))*(IMAXCHPFACTOR(IA,IGDISPATCH,T)-IMINCHPFACTOR(IA,IGDISPATCH,T)) )$(IGUC(IGDISPATCH) and IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)*VGTURBINE_ON(T,IA,IGDISPATCH))$IGHYRES_ELSTO(IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_SEC(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)) ..

$ifi not %NoReserveRestrictions%==YES    VGE_ANCNEG_SEC(T,IA,IGDISPATCH)

$ifi not %NoReserveRestrictions%==YES    =L=

$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes    (GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH))$(not IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi     '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)* VGTURBINE_ON(T,IA,IGDISPATCH))$(IGHYRES_ELSTO(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   (GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH) )$(IGUC(IGDISPATCH) and not IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(VGONLINE_T(T,IA,IGDISPATCH)*IGELECCAPEFF (IA,IGDISPATCH,T)/IGELECCAPACITY_Y(IA,IGDISPATCH))*(IMAXCHPFACTOR(IA,IGDISPATCH,T)-IMINCHPFACTOR(IA,IGDISPATCH,T)) )$(IGUC(IGDISPATCH) and IGCHPBOUNDE(IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES $ifi Not '%UnitCmin%' == Yes   +(GDATA(IA,IGDISPATCH,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))*IGKDERATE(IA,IGDISPATCH,T)*VGTURBINE_ON(T,IA,IGDISPATCH))$IGHYRES_ELSTO(IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QGANCPOS- / -NEG_CONS_PRI / _SEC: Capability of storage unit in providing primary / secondary operating margin from charging ---- *{

$ifi not %NoReserveRestrictions%==YES QGANCPOS_CONS_PRI(T,IAGK_Y(IA,IGHYRES_ELSTO))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    VGE_Consumed_ANCPOS_PRI(T,IA,IGHYRES_ELSTO)

$ifi not %NoReserveRestrictions%==YES   =L=

$ifi not %NoReserveRestrictions%==YES GDATA(IA,IGHYRES_ELSTO,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*(1-VGTURBINE_ON(T,IA,IGHYRES_ELSTO))
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCPOS_CONS_SEC(T,IAGK_Y(IA,IGHYRES_ELSTO))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    VGE_Consumed_ANCPOS_SEC(T,IA,IGHYRES_ELSTO)

$ifi not %NoReserveRestrictions%==YES   =L=

$ifi not %NoReserveRestrictions%==YES GDATA(IA,IGHYRES_ELSTO,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*(1-VGTURBINE_ON(T,IA,IGHYRES_ELSTO))
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_CONS_PRI(T,IAGK_Y(IA,IGHYRES_ELSTO))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    VGE_Consumed_ANCNEG_PRI(T,IA,IGHYRES_ELSTO)

$ifi not %NoReserveRestrictions%==YES   =L=

$ifi not %NoReserveRestrictions%==YES GDATA(IA,IGHYRES_ELSTO,'GDMAXSpinResPri')*(1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*(1-VGTURBINE_ON(T,IA,IGHYRES_ELSTO))
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_CONS_SEC(T,IAGK_Y(IA,IGHYRES_ELSTO))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES   VGE_Consumed_ANCNEG_SEC(T,IA,IGHYRES_ELSTO)

$ifi not %NoReserveRestrictions%==YES    =L=

$ifi not %NoReserveRestrictions%==YES GDATA(IA,IGHYRES_ELSTO,'GDMAXSpinResSec')*(1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*(1-VGTURBINE_ON(T,IA,IGHYRES_ELSTO))
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QGANCPOS- / -NEG_SUM / _STO  ----  Summing up primary and secondary reserve into spinning for legacy reasons *{

$ifi not %NoReserveRestrictions%==YES QGANCPOS_SUM(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

$ifi not %NoReserveRestrictions%==YES    (VGE_ANCPOS_PRI(T,IA,IGDISPATCH)   + VGE_ANCPOS_SEC(T,IA,IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VGE_ANCPOS(T,IA,IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_SUM(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..
$ifi not %NoReserveRestrictions%==YES    (VGE_ANCNEG_PRI(T,IA,IGDISPATCH)   + VGE_ANCNEG_SEC(T,IA,IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VGE_ANCNEG(T,IA,IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCPOS_SUM_STO(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..
$ifi not %NoReserveRestrictions%==YES    (VGE_CONSUMED_ANCPOS_PRI(T,IA,IGDISPATCH)    + VGE_CONSUMED_ANCPOS_SEC(T,IA,IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VGE_CONSUMED_ANCPOS(T,IA,IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_SUM_STO(T,IAGK_Y(IA,IGDISPATCH))$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..
$ifi not %NoReserveRestrictions%==YES    (VGE_CONSUMED_ANCNEG_PRI(T,IA,IGDISPATCH)    + VGE_CONSUMED_ANCNEG_SEC(T,IA,IGDISPATCH))
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VGE_CONSUMED_ANCNEG(T,IA,IGDISPATCH)
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QGANCPOS- / -NEG_PEN  ----  Summing up primary and secondary reserve slack variables into spinning for legacy reasons *{

$ifi not %NoReserveRestrictions%==YES QGANCPOS_PEN(T,TSO_RRR,'IMINUS')$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..
$ifi not %NoReserveRestrictions%==YES    VQANCPOSEQ_PRI(T,TSO_RRR,'IMINUS')    + VQANCPOSEQ_SEC(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VQANCPOSEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;

$ifi not %NoReserveRestrictions%==YES QGANCNEG_PEN(T,TSO_RRR,'IMINUS')$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..
$ifi not %NoReserveRestrictions%==YES    VQANCNEGEQ_PRI(T,TSO_RRR,'IMINUS')    + VQANCNEGEQ_SEC(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES   =E=
$ifi not %NoReserveRestrictions%==YES    VQANCNEGEQ(T,TSO_RRR,'IMINUS')
$ifi not %NoReserveRestrictions%==YES ;
*}

* ---- QWINDSHED Wind shedding planned on day-ahead market lower than expected wind power production when bidding ---- *{
* + VWINDCUR_ANCPOS(T,IR_STOC) -> Include in reserve equations

QWINDSHED(T,IR)$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VWINDSHEDDING_DAY_AHEAD(T,IR)
$ifi '%Renewable_Spinning%' == Yes  + VWINDCUR_ANCPOS(T,IR)
  =L=
  IWIND_BID_IR(IR,T)
;
*}

* ---- QSOLARSHED Solar shedding planned on day-ahead market lower than expected solar power production when bidding ---- *{

QSOLARSHED(T,IR)$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VSOLARSHEDDING_DAY_AHEAD(T,IR)
  =L=
  ISOLAR_VAR_T(IR,T)
;
*}

* ---- QDumpLimDAY Dump energy is smaller that generation (limitation of export for d) ---- *{
QDumpLimDAY(T,IR)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T))..

 VQDAYAHEAD(T,IR,'IPLUS')
 =L=
 + 0.0*ITIDALSTREAM_VAR_T(IR,T)
 + 1.0*IWAVE_VAR_T(IR,T)
 + 0.5*IRUNRIVER_VAR_T(IR,T)
 + 0.0*IGEOTHHEAT_VAR_T(IR,T)
 + 0.0*ISOLARTH_VAR_T(IR,T)
 + 0.5*IBiomass_VAR_T(IR,T)
 + 0.5*IOthRes_VAR_T(IR,T)
 + 0.0*IDISGEN_VAR_T(IR,T)
;
*}

* ---- QDump_SHEDDING Restriction to ensure that the production sold on the day-ahead market is not  unreasonable big ---- *{
QDump_SHEDDING(T,IR)$((IBID_DAYAHEADMARKET_YES EQ 1)$IT_SPOTPERIOD(T) AND SHEDDING_FORBIDDEN_TOTAL(IR, T) ) ..
  VQDAYAHEAD(T,IR,'IPLUS')
  + VSOLARSHEDDING_DAY_AHEAD(T,IR)
  + VWINDSHEDDING_DAY_AHEAD(T,IR)
 =E= 0
;
*}

* ---- QGCAPNON_SPIN: Provision of non-spinning reserve from off-line units lower than available capacity
* Without MIP: provison lower than capacity still off-line (equal to Available Capacity minus vgonline) *{

$ifi not %NoReserveRestrictions%==YES QGCAPNON_SPIN(T,IAGK_Y(IA,IGFASTUNITS)) $(IT_OPT(T) AND IGUC(IGFASTUNITS)) ..

$ifi not %NoReserveRestrictions%==YES    VGE_NONSPIN_ANCPOS(T,IA,IGFASTUNITS)
$ifi not %NoReserveRestrictions%==YES    =L=
$ifi not %NoReserveRestrictions%==YES IGELECCAPEFF(IA,IGFASTUNITS,T)*(1-VGONLINE_T(T,IA,IGFASTUNITS))
$ifi not %NoReserveRestrictions%==YES ;
*}
*}


* --- Restrictions for down regulation *{

* ---- QGCAPELEC_Wind (9.2.4): realised wind shedding ---- *{
* Restriction on the load shedding capacity of fluctuating units
* the contribution to down regulation is lower than the actual production

QGCAPELEC_Wind(T,IR) $IT_OPT(T) ..

  SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),
   VGELEC_DNEG_T(T,IA,IGWIND))
$ifi '%Renewable_Spinning%' == Yes + VWINDCUR_ANCPOS(T,IR)
   =L=
   (IWIND_REALISED_IR(IR,T)- VWINDSHEDDING_DAY_AHEAD(T,IR))*IVWINDSHEDDING_Intraday_YES
;
*}

* ---- QGCAPELEC_Solar ---- *{
QGCAPELEC_Solar(T,IR) $IT_OPT(T) ..

  SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),
   VGELEC_DNEG_T(T,IA,IGSOLAR))
   =L=
   (ISOLAR_VAR_T(IR,T)- VSOLARSHEDDING_DAY_AHEAD(T,IR))*IVSOLARSHEDDING_Intrady_YES
;
*}

* ---- QGNEGDEV capacity restrictions negative intraday electricity production ---- *{
* Down regulation + Primary negative reserve < Production day-ahead + Up regulation

QGNEGDEV(T,IAGK_Y(IA,IGCON_HYRES_ELSTO)) $IT_OPT(T)..

  VGE_ANCNEG(T,IA,IGCON_HYRES_ELSTO) $IGSPINNING(IGCON_HYRES_ELSTO)
  =L=
  VGELEC_T(T,IA,IGCON_HYRES_ELSTO) + VGELEC_DPOS_T(T,IA,IGCON_HYRES_ELSTO) - VGELEC_DNEG_T(T,IA,IGCON_HYRES_ELSTO)
;
*}

* ---- QXNEGDEV capacity restrictions negative intraday electricity transmission ---- *{
* decrease in export < export fixed at day-ahead market

QXNEGDEV(T,IEXPORT_NTC_Y(IRE,IRI)) $IT_OPT(T)..
   VXELEC_DNEG_T(T,IRE,IRI)
   =L=
   VXELEC_T(T,IRE,IRI)
;

$ifi '%LFB%' == Yes QXNEGDEVDC(T,IEXPORT_FLBMC_DC_Y(IRE,IRI)) $IT_OPT(T)..
$ifi '%LFB%' == Yes    VXELEC_DC_DNEG_T(T,IRE,IRI)
$ifi '%LFB%' == Yes    =L=
$ifi '%LFB%' == Yes    VXELEC_DC_T(T,IRE,IRI)
$ifi '%LFB%' == Yes ;

*}

*}

* ---- QGOUTPUTUSINGFUEL (before: QGFUELUSE1), QGFUELUSELINEAR (before QGFUELUSE3): Fuel consumption dependant on power and heat generation ---- *{

QGOUTPUTUSINGFUEL(T,IAGK_Y(IA,IGUSINGFUEL)) $IT_OPT(T)..

   VGOUTPUT_T(T,IA,IGUSINGFUEL)
   =E=
    (VGELEC_T(T,IA,IGUSINGFUEL) + VGELEC_DPOS_T(T,IA,IGUSINGFUEL) - VGELEC_DNEG_T(T,IA,IGUSINGFUEL))$IGELEC(IGUSINGFUEL)
    + (GDATA(IA,IGUSINGFUEL,'GDCV') * VGHEAT_T(T,IA,IGUSINGFUEL))$IGELECANDHEAT(IGUSINGFUEL)
    + VGHEAT_T(T,IA,IGUSINGFUEL)$IGHEATONLY(IGUSINGFUEL)
$ifi '%CCGT_Imp%' == Yes    +VGSTEAM_T(T,IA,IGUSINGFUEL)$IGDUCTBURNER(IGUSINGFUEL)
;


QGFUELUSELINEAR(T,IAGK_Y(IA,IGONESLOPE)) $IT_OPT(T)..

   VGFUELUSAGE_T(T,IA,IGONESLOPE)
   =E=
   IFUELUSAGE_SECTION(IA,IGONESLOPE) * VGONLINE_T(T,IA,IGONESLOPE) * IGELECCAPEFF(IA,IGONESLOPE,T)
   + IFUELUSAGE_SLOPE(IA,IGONESLOPE) * VGOUTPUT_T(T,IA,IGONESLOPE)
;
*}


* ---- QUC07: Electricity production equal to the sum of electricity production in each segment of the piecewise linear fuel consumption curve *{

QUC07(T,IAGK_Y(IA,IGSLOPES)) $IT_OPT(T)..

  VGOUTPUT_T(T,IA,IGSLOPES)
  =E=
  SUM(IHRS $(ORD(IHRS) LE GDATA(IA,IGSLOPES,'GDFE_NSLOPES')), VGOUTPUT_APPROX_T(T,IA,IGSLOPES,IHRS))

   + GDATA(IA,IGSLOPES,'GDMINLOADFACTOR') * VGONLINE_T(T,IA,IGSLOPES) * IGELECCAPEFF(IA,IGSLOPES,T)
;
*}

* ---- QGFUELUSENONLINEAR ---- *{
QGFUELUSENONLINEAR(T,IAGK_Y(IA,IGSLOPES)) $IT_OPT(T)..

   VGFUELUSAGE_T(T,IA,IGSLOPES)
   =E=
   (IFUELUSAGE_SECTION(IA,IGSLOPES) + FE_SLOPE_APPROX(IA,IGSLOPES,'IHRS01') * GDATA(IA,IGSLOPES,'GDMINLOADFACTOR')) * IGELECCAPEFF(IA,IGSLOPES,T)

    * VGONLINE_T(T,IA,IGSLOPES)

 + SUM (IHRS $(ORD(IHRS) LE GDATA(IA,IGSLOPES,'GDFE_NSLOPES')), FE_SLOPE_APPROX(IA,IGSLOPES,IHRS) * VGOUTPUT_APPROX_T(T,IA,IGSLOPES,IHRS))
;
*}

* ---- QGOUTPUTNONLINEAR1 (before: QUC08): Rightborder of the first segment of the piecewise linear fuel consumption curve ----

QGOUTPUTNONLINEAR1(T,IAGK_Y(IA,IGSLOPES)) $IT_OPT(T)..

  VGOUTPUT_APPROX_T(T,IA,IGSLOPES,'IHRS01')
  =L=
  (FE_RIGHTBORDER_APPROX(IA,IGSLOPES,'IHRS01') - GDATA(IA,IGSLOPES,'GDMINLOADFACTOR')) * IGELECCAPEFF(IA,IGSLOPES,T)
;

* ---- QGOUTPUTNONLINEAR2 (before: QUC09): Rightborder of each segment (except the first) of the piecewise linear fuel consumption curve ---- *{

QGOUTPUTNONLINEAR2(T,IAGK_Y(IA,IGSLOPES),IHRS) $((ORD(IHRS) GT 1 ) AND (ORD(IHRS) LE GDATA(IA,IGSLOPES,'GDFE_NSLOPES')) AND IT_OPT(T) )..

   VGOUTPUT_APPROX_T(T,IA,IGSLOPES,IHRS)
  =L=
  (FE_RIGHTBORDER_APPROX(IA,IGSLOPES,IHRS) - FE_RIGHTBORDER_APPROX(IA,IGSLOPES,IHRS-1)) * IGELECCAPEFF(IA,IGSLOPES,T)
;
*}
*}
*}

* ---- QGONLSTART Start-up ---- *{

$ifi not '%NoStartUpCosts%' == Yes QGONLSTART(T,IAGK_Y(IA,IGUC)) $IT_OPT(T)..
$ifi not '%NoStartUpCosts%' == Yes    VGSTARTUP_T(T,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T)
$ifi not '%NoStartUpCosts%' == Yes    =G=
$ifi not '%NoStartUpCosts%' == Yes    (1-IG_UNP_OUTAGE(IA,IGUC,T)) * (VGONLINE_T(T,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T) - VGONLINE_T(T-1,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T-1))
$ifi not '%NoStartUpCosts%' == Yes  ;
*}

* ---- QGONLOP: minimum operation time (approximation) ---- *{
* If the online capacity is increased at timestep t by an amount of delta_P then the
* unit has to be online (with at least this amount delta_P) for a certain number of time steps

$ifi not '%NoOpTimeRestrictions%' ==  Yes QGONLOP2(T,T_WITH_Hist,UTIME,IAGK_Y(IA,IGMINOPERATION_NOALWAYS)) $(IT_OPT(T) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes        (ORD(T_WITH_HIST) GE (ORD(T) + 24 - GDATA(IA,IGMINOPERATION_NOALWAYS,'GDMINOPERATION'))) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes        (ORD(T_WITH_HIST) < (ORD(T) + 24 - (ORD(UTIME)-1))) AND (ORD(T) > ORD(UTIME)) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes        (GDATA(IA,IGMINOPERATION_NOALWAYS,'GDMINOPERATION') > ORD(UTIME))
$ifi not '%NoOpTimeRestrictions%' ==  Yes        )..

$ifi not '%NoOpTimeRestrictions%' ==  Yes    VGONLINE_T(T,IA,IGMINOPERATION_NOALWAYS)*IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T)
$ifi not '%NoOpTimeRestrictions%' ==  Yes    =G=
$ifi not '%NoOpTimeRestrictions%' ==  Yes    VGONLINE_T(T-ORD(UTIME),IA,IGMINOPERATION_NOALWAYS)*IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T-ORD(UTIME)) - VGONLINE_T(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS)*IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T_WITH_HIST)
$ifi not '%NoOpTimeRestrictions%' ==  Yes    - VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IPLUS')
$ifi not '%NoOpTimeRestrictions%' ==  Yes ;

$ontext
* EWL version formulation (not restrictive enough for LP configuration)
QGONLOP(T,T_WITH_Hist,IAGK_Y(IA,IGMINOPERATION_NOALWAYS)) $(IT_OPT(T) AND
       (ORD(T_WITH_HIST) GE (ORD(T) + 24 - GDATA(IA,IGMINOPERATION_NOALWAYS,'GDMINOPERATION'))) AND
       (ORD(T_WITH_HIST) < (ORD(T) + 24))
       )..

   VGONLINE_T(T,IA,IGMINOPERATION_NOALWAYS)
   =G=
   VGONLINE_T(T-1,IA,IGMINOPERATION_NOALWAYS) - VGONLINE_T(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS)
   - VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IPLUS')
;
$offtext

*}

* ---- QGONLSD (9.2.7): minimum shut down time (approximation) ---- *{

$ifi not '%NoOpTimeRestrictions%' ==  Yes QGONLSD2(T,T_WITH_HIST,DTIME,IAGK_Y(IA,IGUC_NOALWAYS)) $(IT_OPT(T) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes         (ORD(T_WITH_HIST) GE (ORD(T) + 24 - GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN'))) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes         (ORD(T_WITH_HIST) < (ORD(T) + 24 - (Ord(DTIME)-1))) AND (ORD(T) > ORD(DTIME)) AND
$ifi not '%NoOpTimeRestrictions%' ==  Yes         (GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') > 1)
$ifi not '%NoOpTimeRestrictions%' ==  Yes         )..
$ifi not '%NoOpTimeRestrictions%' ==  Yes    VGONLINE_T(T,IA,IGUC_NOALWAYS)*IGELECCAPEFF(IA,IGUC_NOALWAYS,T) - VGONLINE_T(T-Ord(DTime),IA,IGUC_NOALWAYS)*IGELECCAPEFF(IA,IGUC_NOALWAYS,T-Ord(DTime))
$ifi not '%NoOpTimeRestrictions%' ==  Yes     =L=
$ifi not '%NoOpTimeRestrictions%' ==  Yes   (1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T)) *
$ifi not '%NoOpTimeRestrictions%' ==  Yes   (IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)
$ifi not '%NoOpTimeRestrictions%' ==  Yes    - VGONLINE_T(T_WITH_HIST,IA,IGUC_NOALWAYS)*IGELECCAPEFF(IA,IGUC_NOALWAYS,T_WITH_HIST)

*addition of Epsilon due to numerical problems that lead to infeasibility
$ifi not '%NoOpTimeRestrictions%' ==  Yes +30*Epsilon
$ifi not '%NoOpTimeRestrictions%' ==  Yes   )
$ifi not '%NoOpTimeRestrictions%' ==  Yes ;

$ontext
* EWL version formulation (not restrictive enough for LP configuration)
QGONLSD(T,T_WITH_HIST,IAGK_Y(IA,IGUC_NOALWAYS)) $(IT_OPT(T) AND
        (ORD(T_WITH_HIST) GE (ORD(T) + 24 - GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN'))) AND
        (ORD(T_WITH_HIST) < (ORD(T) + 24)) AND
        (GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') > 1)
        )..

   VGONLINE_T(T,IA,IGUC_NOALWAYS)
    =L=
  (1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T)) *
  (IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)
   - ( VGONLINE_T(T_WITH_HIST,IA,IGUC_NOALWAYS) - VGONLINE_T(T-1,IA,IGUC_NOALWAYS))
  )
;
$offtext
*}

* ---- QGONLSLOW (9.2.7): Slow units are always set on maximum capacity --- *{
* Capacity online = Maximum capacity

QGONLSLOW (T,IAGK_Y(IA,IGALWAYSRUNNING)) $IT_OPT(T)..

 VGONLINE_T(T,IA,IGALWAYSRUNNING)
 =E=
CEIL(IGKDERATE(IA,IGALWAYSRUNNING,T) * (1-IG_UNP_OUTAGE(IA,IGALWAYSRUNNING,T)))
;
*}

* ---- QGRAMP: Restriction on ramping production upwards.
* Realised production (t) - Realised production (t-1) < Capacity online (t)*NODE,T *{

QGRAMP(T,IAGK_Y(IA,IGRAMP)) $IT_OPT(T) ..

   VGELEC_T(T,IA,IGRAMP) + VGELEC_DPOS_T(T,IA,IGRAMP) - VGELEC_DNEG_T(T,IA,IGRAMP)
   - (VGELEC_T(T-1,IA,IGRAMP) + VGELEC_DPOS_T(T-1,IA,IGRAMP) - VGELEC_DNEG_T(T-1,IA,IGRAMP))

   =L=

   GDATA(IA,IGRAMP,'GDRAMPUP') * VGONLINE_T(T,IA,IGRAMP) * IGELECCAPEFF(IA,IGRAMP,T)
;

* ---- QGSTARTRAMP / _CHP: Restriction on ramping at start time. *{
$ifi not %MinGen_Manipulation%==yes QGSTARTRAMP(T,IAGK_Y(IA,IGUC))$(IT_OPT(T) and (GDATA(IA,IGUC,'GDTYPE') eq  1)) ..
$ifi     %MinGen_Manipulation%==yes QGSTARTRAMP(T,IAGK_Y(IA,IGUC))$(IT_OPT(T) and (GDATA(IA,IGUC,'GDTYPE') eq  1) and not (GDATA(IA,IGUC,'GDFUEL') eq 203 or (GDATA(IA,IGUC,'GDFUEL') eq 10)))..


   VGELEC_T(T,IA,IGUC) + VGELEC_DPOS_T(T,IA,IGUC) - VGELEC_DNEG_T(T,IA,IGUC) + VGE_ANCPOS(T,IA,IGUC)
   =L=
   VGONLINE_T(T-1,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T-1)
   + ISTARTRAMP_CHP(IA,IGUC,T) * (VGONLINE_T(T,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T) -  VGONLINE_T(T-1,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T-1))
;

QGSTARTRAMP_CHP(T,IAGK_Y(IA,IGCHPBOUNDE))$IT_OPT(T) ..

   VGELEC_T(T,IA,IGCHPBOUNDE) + VGELEC_DPOS_T(T,IA,IGCHPBOUNDE) - VGELEC_DNEG_T(T,IA,IGCHPBOUNDE)
   + VGE_ANCPOS(T,IA,IGCHPBOUNDE)
    =L=
   IMax_Delta_VGELEC_CHP(IA,IGCHPBOUNDE,T) * IGELECCAPACITY_Y(IA,IGCHPBOUNDE)
;
*}

* ---- QGRAMP_UP: Ramping production upwards is penalized, currently not in documentation or MODEL statement *{

QGRAMP_UP(T,IAGK_Y(IA,IGDISPATCH))$IT_OPT(T)..

   VGELEC_T(T,IA,IGDISPATCH) + VGELEC_DPOS_T(T,IA,IGDISPATCH) - VGELEC_DNEG_T(T,IA,IGDISPATCH)-
   ( VGELEC_T(T-1,IA,IGDISPATCH) + VGELEC_DPOS_T(T-1,IA,IGDISPATCH) - VGELEC_DNEG_T(T-1,IA,IGDISPATCH)
    )
- 0.1 * IGELECCAPEFF(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH)
   =L=
   VGRAMP_UP(T,IA,IGDISPATCH);
;
*}

* ---- QGRAMP_DOWN: Ramping production downwards is penalized, currently not in documentation or MODEL statement *{

QGRAMP_DOWN(T,IAGK_Y(IA,IGDISPATCH))$IT_OPT(T)..

   -( VGELEC_T(T,IA,IGDISPATCH) + VGELEC_DPOS_T(T,IA,IGDISPATCH) - VGELEC_DNEG_T(T,IA,IGDISPATCH)-
      (VGELEC_T(T-1,IA,IGDISPATCH) + VGELEC_DPOS_T(T-1,IA,IGDISPATCH) - VGELEC_DNEG_T(T-1,IA,IGDISPATCH))
     )
- 0.1 * IGELECCAPEFF(IA,IGDISPATCH,T)* VGONLINE_T(T,IA,IGDISPATCH)
   =L=
   VGRAMP_DOWN(T,IA,IGDISPATCH);
;
*}

* ---- QGGETOH (9.2.9): Electric heat pumps ---- *{
* Electric heat pumps:
* Electricity consumption day-ahead + Increased electricity consumption -
* Decreased electricity consumption = Heat production/Efficiency

QGGETOH(T,IAGK_Y(IA,IGHEATPUMP)) $IT_OPT(T) ..

 VGELEC_T(T,IA,IGHEATPUMP) + VGELEC_DPOS_T(T,IA,IGHEATPUMP) - VGELEC_DNEG_T(T,IA,IGHEATPUMP)
 =E=
 VGHEAT_T(T,IA,IGHEATPUMP)/(GDATA(IA,IGHEATPUMP,'GDFULLLOAD'))
;
*}


* -------------- Hydropower with reservoirs: ----------------------------------- *{
* ---- QHYRSSEQ: Hydro reservoir content - dynamic equation ---- *{
* Content reservoir(t) = Content reservoir(t-1) - Hydropower production day-ahead - Up regulation
* + Down regulation + Water inflow

QHYRSSEQ(T,IAHYDRO) $IT_OPT(T)..

   VCONTENTHYDRORES_T(T-1,IAHYDRO)
   - Sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES), VGELEC_T(T,IAHYDRO,IGHYDRORES) + VGELEC_DPOS_T(T,IAHYDRO,IGHYDRORES) - VGELEC_DNEG_T(T,IAHYDRO,IGHYDRORES))
   + Sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),
                                                   GDATA(IAHYDRO,IGHYDRORES,'GDLOADEFF')*(VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES)
                                                                                                 + VGELEC_Consumed_DPOS_T(T,IAHYDRO,IGHYDRORES)
                                                                                                 - VGELEC_Consumed_DNEG_T(T,IAHYDRO,IGHYDRORES)) )
   + IHYDROINFLOW_AAA_T(IAHYDRO,T)
   - VHYDROSPILLAGE(T,IAHYDRO)
   + VHYDROSPILLAGE_Neg(T,IAHYDRO)

   =E=

   VCONTENTHYDRORES_T(T,IAHYDRO)
;
*}

* ---- QHYRSMAXCON: Maximum content ---- *{
* Content reservoir + increased production reserved for providing spinning reserve
* + increased production reserved for providing replacement reserve < Maximum capacity reservoir

QHYRSMAXCON(T,IAHYDRO) $IT_OPT(T)..

   VCONTENTHYDRORES_T(T,IAHYDRO)
   + Sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),
             VGE_ANCNEG(T,IAHYDRO,IGHYDRORES) $IGSPINNING(IGHYDRORES)
             + VGE_SPIN_ANCNEG(T,IAHYDRO,IGHYDRORES)
                         + VGE_CONSUMED_ANCNEG(T,IAHYDRO,IGHYDRORES)
                         + VGE_CONSUMED_NONSP_ANCNEG(T,IAHYDRO,IGHYDRORES)
          )

   =L=

   Sum(IGHYDRORES, IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))
;
*}

* ---- QHYRSMINCON: Minimum content ---- *{
* Content reservoir - decreased production reserved for providing negative spinning
* reserve > Minimum capacity reservoir

QHYRSMINCON(T,IAHYDRO) $IT_OPT(T)..

  VCONTENTHYDRORES_T(T,IAHYDRO)
  - Sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),
      VGE_ANCPOS(T,IAHYDRO,IGHYDRORES) $IGSPINNING(IGHYDRORES)
      + VGE_SPIN_ANCPOS(T,IAHYDRO,IGHYDRORES)
      + VGE_NONSPIN_ANCPOS(T,IAHYDRO,IGHYDRORES)
          + VGE_CONSUMED_ANCPOS(T,IAHYDRO,IGHYDRORES)
          + VGE_CONSUMED_NONSP_ANCPOS(T,IAHYDRO,IGHYDRORES)
    )

  =G=

  Sum(IGHYDRORES,IGHYDRORESMINCONTENT_Y(IAHYDRO,IGHYDRORES))
;
*}

* ---- QHYRSMAXPROD: Regulated production + Non spinning reserve + Primary reserve + Run of river production
* < Available hydro capacity *{

QHYRSMAXPROD(T,IAHYDRO) $IT_OPT(T)..

   SUM(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),
      VGELEC_T(T,IAHYDRO,IGHYDRORES) + VGELEC_DPOS_T(T,IAHYDRO,IGHYDRORES) - VGELEC_DNEG_T(T,IAHYDRO,IGHYDRORES)
      + VGE_ANCPOS(T,IAHYDRO,IGHYDRORES) $IGSPINNING(IGHYDRORES)
      + VGE_SPIN_ANCPOS(T,IAHYDRO,IGHYDRORES)
      + VGE_NONSPIN_ANCPOS(T,IAHYDRO,IGHYDRORES)
      )
   + IRUNRIVER_AAA_T(IAHYDRO,T)

  =L=

   Sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),
      IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES) * IGKDERATE(IAHYDRO,IGHYDRORES,T) * (1-IG_UNP_OUTAGE(IAHYDRO,IGHYDRORES,T))
      )
   + Sum(IGRUNOFRIVER $ IAGK_Y(IAHYDRO,IGRUNOFRIVER),
      IGELECCAPACITY_Y(IAHYDRO,IGRUNOFRIVER) * IGKDERATE(IAHYDRO,IGRUNOFRIVER,T) * (1-IG_UNP_OUTAGE(IAHYDRO,IGRUNOFRIVER,T))
      )
;
*}

* ---- QHSTOEnd_2, QHSTOEndSmall, QHSTOEndLarge: Restriction on the end filling level dependent on reservoir size

* ---- QHSTOEnd_2: storage level at the end of the week <= 80% (avoids completely filled storages in case of high shadow prices) *{
$ifi '%Looping%' == week QHSTOEnd_2(T,IAELECSTO,IGELECSTORAGE)$(IENDTIME(T) AND (SUM(IR,IDEMANDELEC_BID_IR(IR,T))>0) AND IGELECCAPACITY_Y(IAELECSTO,IGELECSTORAGE) gt 0)..
$ifi '%Looping%' == week    VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE) $IENDTIME(T) =l= 0.8*IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)
$ifi '%Looping%' == week ;
*}
* ---- QHSTOEndSmall: small hydro storages (capacity for 24 hours or less): storage level at the end of the week > 20% *{
$ifi '%Looping%' == week QHSTOEndSmall(T,IAELECSTO,IGELECSTORAGE)$(IENDTIME(T) AND (SUM(IR,IDEMANDELEC_BID_IR(IR,T))>0) AND IGELECCAPACITY_Y(IAELECSTO,IGELECSTORAGE) gt 0 AND (IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)/IGELECCAPACITY_Y(IAELECSTO,IGELECSTORAGE) le 24))..
$ifi '%Looping%' == week    VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE) $IENDTIME(T) =g=  0.2*IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)
$ifi '%Looping%' == week ;
*}
* ---- QHSTOEndLarge: large hydro storages (capacity for more than 24hours): storage level at the end of the week = 50% (same value as the initialization value in "default_values.inc") *{
* (technology "hydro storages" includes reservoir storages with pump turbines and low water inflows)
$ifi '%Looping%' == week QHSTOEndLarge(T,IAELECSTO,IGELECSTORAGE)$((IENDTIME(T)) AND (sum(ir,IDEMANDELEC_BID_IR(IR,T))>0) and IGELECCAPACITY_Y(IAELECSTO,IGELECSTORAGE) gt 0 AND (IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)/IGELECCAPACITY_Y(IAELECSTO,IGELECSTORAGE) gt 24))..
$ifi '%Looping%' == week    VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE) =E=  0.5*IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)
$ifi '%Looping%' == week ;
*end change
*}
*}

* ---- QHYRSENDUPPER, QHYRSENDLOWER: Calculate deviations from filling level for objective function penalty terms *{
*QHYRSENDUPPER: VContent <= Reference reservoirlevel + upper deviation from reference reservoirlevel
*QHYRSENDLOWER: VContent >= Reference reservoirlevel - lower deviation from reference reservoirlevel

$ifi '%HydroPenalty%' == YES QHYRSEndUpper(T,IAHYDRO)$(IENDTIME(T) OR IENDTIME_HYDISDP(T))..

$ifi '%HydroPenalty%' == YES    VCONTENTHYDRORES_T(T,IAHYDRO)
$ifi '%HydroPenalty%' == YES  =L=
$ifi '%HydroPenalty%' == YES    IRESLEVELS_T(IAHYDRO,T)/100 *Sum(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES)) + sum(IHYDFLXSTEP, VQRESLEVELUP(T,IAHYDRO,IHYDFLXSTEP))
$ifi '%HydroPenalty%' == YES ;

$ifi '%HydroPenalty%' == YES QHYRSEndLower(T,IAHYDRO)$(IENDTIME(T) OR IENDTIME_HYDISDP(T))..

$ifi '%HydroPenalty%' == YES    VCONTENTHYDRORES_T(T,IAHYDRO)
$ifi '%HydroPenalty%' == YES   =G=
$ifi '%HydroPenalty%' == YES    IRESLEVELS_T(IAHYDRO,T)/100*Sum(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES)) - sum(IHYDFLXSTEP, VQRESLEVELDOWN(T,IAHYDRO,IHYDFLXSTEP))
$ifi '%HydroPenalty%' == YES ;
*}

* ---- QHYRESPUMPCHLIM, QHYRESPUMPCHLIM_POS, QHYPmpMax, QHYGenMax: additional constraints on the operation of pumped-storage hydro plants *{
QHYRESPUMPCHLIM(T,IAHydro)$(ord(t) GT 1)..

   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),VGELEC_Consumed_DNEG_T(T,IAHYDRO,IGHYDRORES))
  =L=
   sum(IGHYDRORES $ IAGK_Y(IAHYDRO,IGHYDRORES),VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES));

QHYRESPUMPCHLIM_POS(T,IAHydro)$(ord(t) GT 1)..

   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),(VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES) + VGELEC_Consumed_DPOS_T(T,IAHYDRO,IGHYDRORES)) )
  =L=
   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),(1-VGTURBINE_ON(T,IAHYDRO,IGHydroRes))* (1-IG_UNP_OUTAGE(IAHYDRO,IGHydroRes,T))* IGKDERATE(IAHYDRO,IGHydroRes,T)*IGResLOADCAPACITY_Y(IAHYDRO,IGHydroRes));

QHYPmpMax(T,IAHydro)$(ord(t) GT 1)..

   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),(VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES) + VGELEC_Consumed_DPOS_T(T,IAHYDRO,IGHYDRORES)) )
  =L=
   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),(1-VGTURBINE_ON(T,IAHYDRO,IGHydroRes))*IGResLOADCAPACITY_Y(IAHYDRO,IGHydroRes)*IPmPMax(IAHydro,IGHYDRORES,T));

QHYGenMax(T,IAHydro)$(ord(t) GT 1)..

   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),(VGELEC_T(T,IAHYDRO,IGHYDRORES) + VGELEC_DPOS_T(T,IAHYDRO,IGHYDRORES)) )
  =L=
   sum(IGHYDRORES$IAGK_Y(IAHYDRO,IGHYDRORES),VGTURBINE_ON(T,IAHYDRO,IGHydroRes) * IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES)*IHYRESMAXGEN(IAHydro,IGHYDRORES,T));
*}

*---TYNDP specific Hydro equations to meet weekly pumping restrictions--------*{
$ifi '%Looping%' == week QHYPmpMax_E(IAHydro,IGHYDRORES)$(IAGK_Y(IAHYDRO,IGHYDRORES) and (sum(T,IPmPMax_E(IAHydro,IGHYDRORES,T)))>0)..
$ifi '%Looping%' == week  sum(T, VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES)$IAGK_Y(IAHYDRO,IGHYDRORES))=L=
$ifi '%Looping%' == week  Min( Sum(T, IPmPMax_E(IAHydro,IGHYDRORES,T)$IAGK_Y(IAHYDRO,IGHYDRORES)*1000/ 168),IGResLOADCAPACITY_Y(IAHYDRO,IGHydroRes)$IAGK_Y(IAHYDRO,IGHYDRORES)*168 );

$ifi '%Looping%' == week QHYPmpMIN_E(IAHydro,IGHYDRORES)$IAGK_Y(IAHYDRO,IGHYDRORES)..
$ifi '%Looping%' == week  sum(T, VGELEC_Consumed_T(T,IAHYDRO,IGHYDRORES)$IAGK_Y(IAHYDRO,IGHYDRORES)) =G=
$ifi '%Looping%' == week  sum(T,IPmPMIN_E(IAHydro,IGHYDRORES,T)*1000)/168;
*}
*------------ Heat and electricity storages:------------------------------------ *{

* ---- QESTOVOLT:
* Content storage(t) = Content storage(t-1) + Loading efficiency * Loading storage - Unloading  *{

$ontext
QESTOVOLT(T,IAHYDRO_ELECSTO)$IT_OPT(T)..

Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),VCONTENTSTORAGE_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE))$IAELECSTO(IAHYDRO_ELECSTO)
+ VCONTENTHYDRORES_T(T,IAHYDRO_ELECSTO)$IAHYDRO(IAHYDRO_ELECSTO)

=E=
Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),((1-GDATA(IAHYDRO_ELECSTO,IGELECSTORAGE,'GDSTOLOSS'))*VCONTENTSTORAGE_T(T-1,IAHYDRO_ELECSTO,IGELECSTORAGE)))$IAELECSTO(IAHYDRO_ELECSTO)
+ VCONTENTHYDRORES_T(T-1,IAHYDRO_ELECSTO)$IAHYDRO(IAHYDRO_ELECSTO)

+ Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),(GDATA(IAHYDRO_ELECSTO,IGELECSTORAGE,'GDLOADEFF')*(VGELEC_CONSUMED_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE)+VGELEC_CONSUMED_DPOS_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE)-VGELEC_CONSUMED_DNEG_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE))))$IAELECSTO(IAHYDRO_ELECSTO)
+ Sum(IGHYDRORES$IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES),GDATA(IAHYDRO_ELECSTO,IGHYDRORES,'GDLOADEFF')*(VGELEC_Consumed_T(T,IAHYDRO_ELECSTO,IGHYDRORES)+VGELEC_Consumed_DPOS_T(T,IAHYDRO_ELECSTO,IGHYDRORES)-VGELEC_Consumed_DNEG_T(T,IAHYDRO_ELECSTO,IGHYDRORES)))$IAHYDRO(IAHYDRO_ELECSTO)

- Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),((1/GDATA(IAHYDRO_ELECSTO,IGELECSTORAGE,'GDUNLOADEFF'))*(VGELEC_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE)+VGELEC_DPOS_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE)-VGELEC_DNEG_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE))))$IAELECSTO(IAHYDRO_ELECSTO)
- Sum(IGHYDRORES$IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES),VGELEC_T(T,IAHYDRO_ELECSTO,IGHYDRORES)+VGELEC_DPOS_T(T,IAHYDRO_ELECSTO,IGHYDRORES)-VGELEC_DNEG_T(T,IAHYDRO_ELECSTO,IGHYDRORES))$IAHYDRO(IAHYDRO_ELECSTO)

+( IHYDROINFLOW_AAA_T(IAHYDRO_ELECSTO,T)
  -VHYDROSPILLAGE(T,IAHYDRO_ELECSTO)
  +VHYDROSPILLAGE_Neg(T,IAHYDRO_ELECSTO))$IAHYDRO(IAHYDRO_ELECSTO)
;
$offtext

QESTOVOLT(T,IAGK_Y(IAELECSTO,IGELECSTORAGE))$IT_OPT(T)..

VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE)
=E=
(1-GDATA(IAELECSTO,IGELECSTORAGE,'GDSTOLOSS'))*VCONTENTSTORAGE_T(T-1,IAELECSTO,IGELECSTORAGE)
+ GDATA(IAELECSTO,IGELECSTORAGE,'GDLOADEFF')*(VGELEC_CONSUMED_T(T,IAELECSTO,IGELECSTORAGE)+VGELEC_CONSUMED_DPOS_T(T,IAELECSTO,IGELECSTORAGE)-VGELEC_CONSUMED_DNEG_T(T,IAELECSTO,IGELECSTORAGE))
- (1/GDATA(IAELECSTO,IGELECSTORAGE,'GDUNLOADEFF'))*(VGELEC_T(T,IAELECSTO,IGELECSTORAGE)+VGELEC_DPOS_T(T,IAELECSTO,IGELECSTORAGE)-VGELEC_DNEG_T(T,IAELECSTO,IGELECSTORAGE))
;


*}

QDSMVOLT(T,IAGK_Y(IA,IGFLEXDEMAND))$IT_OPT(T)..
 VCONTENTSTORAGE_T(T,IA,IGFLEXDEMAND)
   =E=
   (1-GDATA(IA,IGFLEXDEMAND,'GDSTOLOSS')) * VCONTENTSTORAGE_T(T-1,IA,IGFLEXDEMAND)
   + GDATA(IA,IGFLEXDEMAND,'GDLOADEFF') *
     (VGELEC_CONSUMED_T(T,IA,IGFLEXDEMAND) + VGELEC_CONSUMED_DPOS_T(T,IA,IGFLEXDEMAND) - VGELEC_CONSUMED_DNEG_T(T,IA,IGFLEXDEMAND))
   - (1/ GDATA(IA,IGFLEXDEMAND,'GDUNLOADEFF')) *
     (VGELEC_T(T,IA,IGFLEXDEMAND) + VGELEC_DPOS_T(T,IA,IGFLEXDEMAND) - VGELEC_DNEG_T(T,IA,IGFLEXDEMAND))
   - (IEV_Leave_Var_T(IA,IGFLEXDEMAND,T)* IGSTOCONTENTCAPACITY_Y(IA,IGFLEXDEMAND) * IEV_SOC_Leave_T(IA,IGFLEXDEMAND,T))$IGELECSTOEV(IGFLEXDEMAND)
   + (IEV_Arrive_Var_T(IA,IGFLEXDEMAND,T)* IGSTOCONTENTCAPACITY_Y(IA,IGFLEXDEMAND) * IEV_SOC_Arrive_T(IA,IGFLEXDEMAND,T))$IGELECSTOEV(IGFLEXDEMAND)
;

* ---- QESTOLOADC: Load lower than given capacity  ---- *{
* Loading (day-ahead + up regulation - down regulation) + Loading capacity reserved for providing negative primary reserve
* <= Capacity loading. The equation applies for both electricity storages and heat pumps. *{

QESTOLOADC(T,IAGK_Y(IA,IGELECSTORAGE)) $IT_OPT(T)..

   VGELEC_CONSUMED_T(T,IA, IGELECSTORAGE) + VGELEC_CONSUMED_DPOS_T(T,IA, IGELECSTORAGE) - VGELEC_CONSUMED_DNEG_T(T,IA, IGELECSTORAGE)
   + VGE_CONSUMED_ANCNEG(T,IA, IGELECSTORAGE) $IGSPINNING(IGELECSTORAGE)
   + VGE_CONSUMED_NONSP_ANCNEG(T,IA, IGELECSTORAGE)
   =L=
   (1-IG_UNP_OUTAGE(IA, IGELECSTORAGE,T)) * IGKDERATE(IA, IGELECSTORAGE,T) *
  IGSTOLOADCAPACITY_Y(IA, IGELECSTORAGE)
;

QDSMLOADC(T,IAGK_Y(IA,IGFLEXDEMAND)) $IT_OPT(T)..

   VGELEC_CONSUMED_T(T,IA,IGFLEXDEMAND) + VGELEC_CONSUMED_DPOS_T(T,IA,IGFLEXDEMAND) - VGELEC_CONSUMED_DNEG_T(T,IA,IGFLEXDEMAND)
   + VGE_CONSUMED_ANCNEG(T,IA,IGFLEXDEMAND) $IGSPINNING(IGFLEXDEMAND)
   + VGE_CONSUMED_NONSP_ANCNEG(T,IA,IGFLEXDEMAND)
   =L=
   (1-IG_UNP_OUTAGE(IA,IGFLEXDEMAND,T)) * IGKDERATE(IA,IGFLEXDEMAND,T) *
   (
   IGSTOLOADCAPACITY_Y(IA,IGFLEXDEMAND)$(Not IGHEATPUMP(IGFLEXDEMAND))
    + (IGHEATCAPACITY_Y(IA,IGFLEXDEMAND)/(GDATA(IA,IGFLEXDEMAND,'GDFULLLOAD')))$IGHEATPUMP(IGFLEXDEMAND)
   )
   ;

*}
* ---- QGCAPELEC4: Restriction to ensure that the consumption for electricity storage and hydro reservoirs
* on the day-ahead market is not unreasonable big ---- *{

$ontext
QGCAPELEC4(T,IAGK_Y(IA,IGHYRES_ELSTO))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VGELEC_CONSUMED_T(T,IA, IGHYRES_ELSTO)$IGELECSTORAGE(IGHYRES_ELSTO)
+ VGELEC_CONSUMED_T(T,IA, IGHYRES_ELSTO)$IGHYDRORES(IGHYRES_ELSTO)

+(VGE_CONSUMED_ANCNEG(T,IA,IGHyRES_ELSTO)$IGSPINNING(IGHYRES_ELSTO)+ VGE_CONSUMED_NONSP_ANCNEG(T,IA,IGHYRES_ELSTO))$IGHYDRORES(IGHYRES_ELSTO)

  =L=

 ((1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*IGSTOLOADCAPACITY_Y(IA,IGHYRES_ELSTO))$IGELECSTORAGE(IGHYRES_ELSTO)
+((1-IG_UNP_OUTAGE(IA,IGHYRES_ELSTO,T))*IGKDERATE(IA,IGHYRES_ELSTO,T)*(IGResLOADCAPACITY_Y(IA,IGHYRES_ELSTO)))$IGHYDRORES(IGHYRES_ELSTO)
;
$offtext

QGCAPELEC4(T,IAGK_Y(IA,IGELECSTORAGE))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VGELEC_CONSUMED_T(T,IA, IGELECSTORAGE)

  =L=

 (1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))*IGKDERATE(IA,IGELECSTORAGE,T)*IGSTOLOADCAPACITY_Y(IA,IGELECSTORAGE)
;
*enc change
*}

QGCAPELECDSM4(T,IAGK_Y(IA,IGFLEXDEMAND))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VGELEC_CONSUMED_T(T,IA,IGFLEXDEMAND)

  =L=

  (1-IG_UNP_OUTAGE(IA,IGFLEXDEMAND,T)) * IGKDERATE(IA,IGFLEXDEMAND,T) *
  (
     IGSTOLOADCAPACITY_Y(IA,IGFLEXDEMAND)$(Not IGHEATPUMP(IGFLEXDEMAND))
     + (IGHEATCAPACITY_Y(IA,IGFLEXDEMAND)/(GDATA(IA,IGFLEXDEMAND,'GDFULLLOAD')))$IGHEATPUMP(IGFLEXDEMAND)
  )
;

QGCAPELEC4Hyres(T,IAGK_Y(IA,IGHydroRes))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

  VGELEC_CONSUMED_T(T,IA,IGHydroRes)
   + VGE_CONSUMED_ANCNEG(T,IA,IGHydroRes) $IGSPINNING(IGHydroRes)
   + VGE_CONSUMED_NONSP_ANCNEG(T,IA,IGHydroRes)

  =L=

  (1-IG_UNP_OUTAGE(IA,IGHydroRes,T))*IGKDERATE(IA,IGHydroRes,T)*IGResLOADCAPACITY_Y(IA,IGHydroRes)
;

* ---- QESTOLOADA: Load greater or equal to zero
* Loading (day-ahead + up regulation - down regulation) - Loading capacity reserved for providing positive primary reserve >= 0
* The equation applies for both electricity storages and heat pumps *{

QESTOLOADA(T,IAGK_Y(IA,IGELECSTORAGE))$IT_OPT(T)..

     VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE) + VGELEC_CONSUMED_DPOS_T(T,IA, IGELECSTORAGE) - VGELEC_CONSUMED_DNEG_T(T,IA, IGELECSTORAGE)
     - VGE_CONSUMED_NONSP_ANCPOS(T,IA, IGELECSTORAGE)
     - VGE_CONSUMED_ANCPOS(T,IA, IGELECSTORAGE)
  =G=
     0;

QESTOLOADA_HyRes(T,IAGK_Y(IA,IGHYDRORES))$IT_OPT(T)..

     VGELEC_CONSUMED_T(T,IA,IGHYDRORES) + VGELEC_CONSUMED_DPOS_T(T,IA,IGHYDRORES) - VGELEC_CONSUMED_DNEG_T(T,IA,IGHYDRORES)
     - VGE_CONSUMED_NONSP_ANCPOS(T,IA,IGHYDRORES)
     - VGE_CONSUMED_ANCPOS(T,IA,IGHYDRORES)
  =G=
     0
;

QDSMLOADA(T,IAGK_Y(IA,IGFLEXDEMAND))$IT_OPT(T)..

     VGELEC_CONSUMED_T(T,IA,IGFLEXDEMAND) + VGELEC_CONSUMED_DPOS_T(T,IA,IGFLEXDEMAND) - VGELEC_CONSUMED_DNEG_T(T,IA,IGFLEXDEMAND)
     - VGE_CONSUMED_NONSP_ANCPOS(T,IA,IGFLEXDEMAND)
     - VGE_CONSUMED_ANCPOS(T,IA,IGFLEXDEMAND)
  =G=
     0
;

*}

* ---- QESTOMAXCO (9.2.11), QESTOMAXCO_DSM: Storage contents does not exceed upper limit (MWh): *{
* Content storage + Increased loading capacity reserved for providing negative primary reserve < Maximum capacity storage
$ontext
QESTOMAXCO(T,IAHYDRO_ELECSTO)$IT_OPT(T)..

Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE), VCONTENTSTORAGE_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE))$IAELECSTO(IAHYDRO_ELECSTO)
+ VCONTENTHYDRORES_T(T,IAHYDRO_ELECSTO)$IAHYDRO(IAHYDRO_ELECSTO)

 + (Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),
    GDATA(IAHYDRO_ELECSTO,IGELECSTORAGE,'GDLOADEFF') *
  ( VGE_CONSUMED_ANCNEG(T,IAHYDRO_ELECSTO,IGELECSTORAGE)$IGSPINNING(IGELECSTORAGE)
    + VGE_SPIN_ANCNEG(T,IAHYDRO_ELECSTO,IGELECSTORAGE)
    + VGE_ANCNEG(T,IAHYDRO_ELECSTO,IGELECSTORAGE)$IGSPINNING(IGELECSTORAGE))))$IAELECSTO(IAHYDRO_ELECSTO)

+  (Sum(IGHYDRORES $ IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES),
             VGE_ANCNEG(T,IAHYDRO_ELECSTO,IGHYDRORES) $IGSPINNING(IGHYDRORES)
             + VGE_SPIN_ANCNEG(T,IAHYDRO_ELECSTO,IGHYDRORES)
                         + VGE_CONSUMED_ANCNEG(T,IAHYDRO_ELECSTO,IGHYDRORES)
                         + VGE_CONSUMED_NONSP_ANCNEG(T,IAHYDRO_ELECSTO,IGHYDRORES)
          ))$IAHYDRO(IAHYDRO_ELECSTO)

  =L=

  Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),IGSTOCONTENTCAPACITY_Y(IAHYDRO_ELECSTO,IGELECSTORAGE))$IAELECSTO(IAHYDRO_ELECSTO)
+ Sum(IGHYDRORES$IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES), IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO_ELECSTO,IGHYDRORES))$IAHYDRO(IAHYDRO_ELECSTO)

;
$offtext

QESTOMAXCO(T,IAGK_Y(IAELECSTO,IGELECSTORAGE))$IT_OPT(T)..

VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE)

 + GDATA(IAELECSTO,IGELECSTORAGE,'GDLOADEFF') *
  ( VGE_CONSUMED_ANCNEG(T,IAELECSTO,IGELECSTORAGE)$IGSPINNING(IGELECSTORAGE)
    + VGE_SPIN_ANCNEG(T,IAELECSTO,IGELECSTORAGE)
    + VGE_ANCNEG(T,IAELECSTO,IGELECSTORAGE)$IGSPINNING(IGELECSTORAGE))

  =L=

  IGSTOCONTENTCAPACITY_Y(IAELECSTO,IGELECSTORAGE)
;

QDSMMAXCO(T,IAGK_Y(IA,IGFLEXDEMAND))$IT_OPT(T)..

  VCONTENTSTORAGE_T(T,IA,IGFLEXDEMAND)
  + GDATA(IA,IGFLEXDEMAND,'GDLOADEFF') *
  ( VGE_CONSUMED_ANCNEG(T,IA,IGFLEXDEMAND) $IGSPINNING(IGFLEXDEMAND)
  + VGE_SPIN_ANCNEG(T,IA,IGFLEXDEMAND)
  + VGE_ANCNEG(T,IA,IGFLEXDEMAND) $IGSPINNING(IGFLEXDEMAND))
  =L=
  IGSTOCONTENTCAPACITY_Y(IA,IGFLEXDEMAND)$(not IGELECSTOEV(IGFLEXDEMAND))
  + (IGSTOCONTENTCAPACITY_Y(IA,IGFLEXDEMAND)*IGKDERATE(IA,IGFLEXDEMAND,T))$IGELECSTOEV(IGFLEXDEMAND)
;

*}

* ---- QESTOMINCO, QESTOMINCO_DSM: Storage content should be enough to deliver primary and secondary positive
* reserve for one hour: *{
* Storage content - Positive primary reserve - Positive secondary reserve

$ontext
QESTOMINCO(T,IAHYDRO_ELECSTO)$IT_OPT(T)..

Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),VCONTENTSTORAGE_T(T,IAHYDRO_ELECSTO,IGELECSTORAGE))$IAELECSTO(IAHYDRO_ELECSTO)
+VCONTENTHYDRORES_T(T,IAHYDRO_ELECSTO)$IAHYDRO(IAHYDRO_ELECSTO)

+ (Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),(1/ GDATA(IAHYDRO_ELECSTO,IGELECSTORAGE,'GDUNLOADEFF')) *
 (- VGE_SPIN_ANCPOS(T,IAHYDRO_ELECSTO,IGELECSTORAGE)
  - VGE_NONSPIN_ANCPOS(T,IAHYDRO_ELECSTO,IGELECSTORAGE)
  - (VGE_ANCPOS(T,IAHYDRO_ELECSTO,IGELECSTORAGE))$IGSPINNING(IGELECSTORAGE)
  - (VGE_CONSUMED_ANCPOS(T,IAHYDRO_ELECSTO,IGELECSTORAGE))$IGSPINNING(IGELECSTORAGE))))$IAELECSTO(IAHYDRO_ELECSTO)

- (Sum(IGHYDRORES $ IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES),
              (VGE_ANCPOS(T,IAHYDRO_ELECSTO,IGHYDRORES))$IGSPINNING(IGHYDRORES)
              + VGE_SPIN_ANCPOS(T,IAHYDRO_ELECSTO,IGHYDRORES)
              + VGE_NONSPIN_ANCPOS(T,IAHYDRO_ELECSTO,IGHYDRORES)
              + VGE_CONSUMED_ANCPOS(T,IAHYDRO_ELECSTO,IGHYDRORES)
              + VGE_CONSUMED_NONSP_ANCPOS(T,IAHYDRO_ELECSTO,IGHYDRORES)))$IAHYDRO(IAHYDRO_ELECSTO)


  =G=
   Sum(IGELECSTORAGE$IAGK_Y(IAHYDRO_ELECSTO,IGELECSTORAGE),IGSTOMINCONTENT_Y(IAHYDRO_ELECSTO,IGELECSTORAGE))$IAELECSTO(IAHYDRO_ELECSTO)
  +Sum(IGHYDRORES$IAGK_Y(IAHYDRO_ELECSTO,IGHYDRORES),IGHYDRORESMINCONTENT_Y(IAHYDRO_ELECSTO,IGHYDRORES))$IAHYDRO(IAHYDRO_ELECSTO)
;
$offtext

QESTOMINCO(T,IAGK_Y(IAELECSTO,IGELECSTORAGE))$IT_OPT(T)..

VCONTENTSTORAGE_T(T,IAELECSTO,IGELECSTORAGE)

+ (1/ GDATA(IAELECSTO,IGELECSTORAGE,'GDUNLOADEFF')) *
 (- VGE_SPIN_ANCPOS(T,IAELECSTO,IGELECSTORAGE)
  - VGE_NONSPIN_ANCPOS(T,IAELECSTO,IGELECSTORAGE)
  - (VGE_ANCPOS(T,IAELECSTO,IGELECSTORAGE))$IGSPINNING(IGELECSTORAGE)
  - (VGE_CONSUMED_ANCPOS(T,IAELECSTO,IGELECSTORAGE))$IGSPINNING(IGELECSTORAGE))

  =G=
   IGSTOMINCONTENT_Y(IAELECSTO,IGELECSTORAGE)
;

QDSMMINCO(T,IAGK_Y(IA,IGFLEXDEMAND))$IT_OPT(T)..

  VCONTENTSTORAGE_T(T,IA,IGFLEXDEMAND)
  + (1/ GDATA(IA,IGFLEXDEMAND,'GDUNLOADEFF')) *
 (- VGE_SPIN_ANCPOS(T,IA,IGFLEXDEMAND)
  - VGE_NONSPIN_ANCPOS(T,IA,IGFLEXDEMAND)
  - VGE_ANCPOS(T,IA,IGFLEXDEMAND) $IGSPINNING(IGFLEXDEMAND)
  - VGE_CONSUMED_ANCPOS(T,IA,IGFLEXDEMAND) $IGSPINNING(IGFLEXDEMAND))
  =G=
  IGSTOMINCONTENT_Y(IA,IGFLEXDEMAND)

;
*}

* ---- QHSTOVOLT: Heat storage dynamic equation ---- *{
* Content storage(t) = Content storage(t-1) * Loss storage + Loading storage - Unloading storage

QHSTOVOLT(T,IAGK_Y(IA,IGHEATSTORAGE))$IT_OPT(T)..

   VCONTENTSTORAGE_T(T,IA,IGHEATSTORAGE)
   =E=
   GDATA(IA,IGHEATSTORAGE,'GDLOADEFF') * VCONTENTSTORAGE_T(T-1,IA,IGHEATSTORAGE)
   + VGHEAT_CONSUMED_T(T,IA,IGHEATSTORAGE)
   - VGHEAT_T(T,IA,IGHEATSTORAGE)
;
*}

* ---- QHSTOLOADC: Max capacity for loading process ---- *{
* Loading storage < Maximum loading capacity

QHSTOLOADC(T,IAGK_Y(IA,IGHEATSTORAGE)) $IT_OPT(T)..

  VGHEAT_CONSUMED_T(T,IA,IGHEATSTORAGE)
  =L=
  IGSTOLOADCAPACITY_Y(IA,IGHEATSTORAGE)
;
*}


* ---- QHSTOMAXCO (9.2.11): Storage contents does not exceed upper limit (MWh) ---- *{
* Content storage < Maximum capacity storage

QHSTOMAXCO(T,IAGK_Y(IA,IGHEATSTORAGE)) $IT_OPT(T) ..

  VCONTENTSTORAGE_T(T,IA,IGHEATSTORAGE)
  =L=
  IGSTOCONTENTCAPACITY_Y(IA,IGHEATSTORAGE)
;
*}

* ---- QESTONOMIX1, QESTONOMIX2, QESTONOMIX3: Equations that restrict the use of storages to destroy energy (by using lossy charge-discharge operations simultaneously) *{
QESTONOMIX1(T,IAGK_Y(IA,IGELECSTORAGE))$(ORD(T) GT 1) ..

  VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE)
  + VGELEC_CONSUMED_DPOS_T(T,IA,IGELECSTORAGE)
  - VGELEC_CONSUMED_DNEG_T(T,IA,IGELECSTORAGE)

  =L=

  (1-VGTURBINE_ON(T,IA,IGELECSTORAGE))*
          IGSTOLOADCAPACITY_Y(IA,IGELECSTORAGE)*
          IGKDERATE(IA,IGELECSTORAGE,T)*
          (1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
;

QESTONOMIX2(T,IAGK_Y(IA,IGELECSTORAGE))$(ORD(T) GT 1) ..

  VGELEC_T(T,IA,IGELECSTORAGE)
  + VGELEC_DPOS_T(T,IA,IGELECSTORAGE)
  - VGELEC_DNEG_T(T,IA,IGELECSTORAGE)
  =L=
  VGTURBINE_ON(T,IA,IGELECSTORAGE)* IGELECCAPEFF(IA,IGELECSTORAGE,T)

;
QESTONOMIX3(T,IAGK_Y(IA,IGHYRES_ELSTO))$(ORD(T) GT 1) ..

  VGTURBINE_ON(T,IA,IGHYRES_ELSTO)
  =L= 1;
*}
*}

* ---- MIP
$ifi %UnitCmin%==yes $INCLUDE '%code_path_addons%/Mip/MIP_Equations_3.inc'

*----- End of equations -------------------------------------------------------
*------------------------------------------------------------------------------
*}
*}

* Model *{
*------------------------------------------------------------------------------
* DEFINE THE MODELS:
*------------------------------------------------------------------------------
* EQUATIONLIST2
*------------------------------------------------------------------------------
MODEL WILMARBASE1 /
   QOBJ
   QEEQDAY
   QEEQINT
   QHEQ

* activation of QHEQNATGAS is useful in case of runs without CHP-Tool (especially FixQ) to ensure that NATGAS-units produce heat on a reasonable level
*   QHEQNATGAS

$ifi not %NoReserveRestrictions%==YES    QNONSP_ANCPOSEQ

$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ
$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ_PRI
$ifi not %NoReserveRestrictions%==YES    QANCPOSEQ_SEC
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ_PRI
$ifi not %NoReserveRestrictions%==YES    QANCNEGEQ_SEC
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_PRI
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SEC
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_PRI
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SEC
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_CONS_PRI
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_CONS_SEC
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_CONS_PRI
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_CONS_SEC
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SUM
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SUM
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_SUM_STO
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_SUM_STO
$ifi not %NoReserveRestrictions%==YES    QGANCPOS_PEN
$ifi not %NoReserveRestrictions%==YES    QGANCNEG_PEN
$ifi not %NoReserveRestrictions%==YES    QNONSP_ANCNEGEQ

*----------------------------------------------------
$ifi Not '%LFB%' == YES QXCAP
$ifi Not '%LFB%' == YES QXCAPDAYAHEAD
$ifi     '%LFB%' == YES QXCAPNTC
$ifi     '%LFB%' == YES QXCAPDC

*   QXK3A
*   QXK4A
*   QXK3B
*   QXK4B
$ifi '%LFB%' == YES   QXCAPFBMCPlus
$ifi '%LFB%' == YES   QXCAPFBMCMinus
$ifi '%LFB_NE%' == YES   QXEXP
$ifi '%LFB_NE%' == YES QXDCEFB
*----------------------------------------------------
QXNEGDEV
*TKMK$ifi '%LFB_NE%' == YES   QXNEGDEVDC
$ifi '%LFB_NE%' == YES   QXNEGDEVDC

   QGCAPELEC_Wind
   QGCAPELEC_Solar
   QGMAXCAPDISPATCHDAYAHEAD
   QGCAPELEC4
   QGCAPHEAT
$ifi '%CCGT_Imp%' == Yes QGCAPSTEAM
   QGCAPELEC4Hyres
   QGNEGDEV
   QWINDSHED
   QSOLARSHED
   QDumpLimDAY
   QDump_SHEDDING
   QGOUTPUTUSINGFUEL
   QGFUELUSENONLINEAR
   QGFUELUSELINEAR
*   QUC07
*   QGOUTPUTNONLINEAR1
*   QGOUTPUTNONLINEAR2
   QGMinGen
   QGMINCAPDISPATCH
   QGMINCAPDISPATCH_CHP
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes   QGMINCAP_CHP
*   QGMINCAPDISPATCH2
   QGMAXCAPDISPATCH
   QGMAXCAPDISPATCH_CHP
$ifi '%Code_version%'==EWL $ifi '%CHP%' == Yes   QGMAXCAP_CHP
$ifi not %NoReserveRestrictions%==YES    QGCAPNON_SPIN
QGCBGBPR
QGCBGEXT

$ifi not '%NoStartUpCosts%' == Yes   QGONLSTART
*TKMK$ifi not '%UnitCmin%' == Yes   QGONLOP
$ifi not '%NoOpTimeRestrictions%' ==  Yes $ifi not '%UnitCmin%' == Yes   QGONLOP2
*TKMK$ifi not '%UnitCmin%' == Yes   QGONLSD
$ifi not '%NoOpTimeRestrictions%' ==  Yes $ifi not '%UnitCmin%' == Yes   QGONLSD2

   QGONLCAPMAX
   QGONLCAPMIN
   QGONLCAPMIN_CHP
$ifi '%CCGT_Imp%' == Yes  QGCCGT_STEAM
$ifi '%CCGT_Imp%' == Yes  QGCCGT_ST
   QGONLSLOW
   QGGETOH
   QHYRSSEQ
   QHYRSMAXCON
   QHYRSMINCON
$ifi '%Looping%' == week   QHSTOEnd_2
$ifi '%Looping%' == week   QHSTOEndLarge
$ifi '%Looping%' == week   QHSTOEndSmall
$ifi '%HydroPenalty%' == YES   QHYRSEndUpper
$ifi '%HydroPenalty%' == YES   QHYRSEndLower
   QHYRESPUMPCHLIM_Pos
   QHYRESPUMPCHLIM
   QHYPmpMax
   QHYGenMax
$ifi '%Looping%' == week   QHYPmpMax_E
$ifi '%Looping%' == week   QHYPmpMin_E
   QHSTOVOLT
   QHSTOLOADC
   QHSTOMAXCO
   QESTOLOADC
   QESTOLOADA
   QESTOLOADA_HyRes
   QESTOVOLT
   QESTOMAXCO
   QESTOMINCO
   QESTONOMIX1
   QESTONOMIX2
   QESTONOMIX3
   QDSMVOLT
   QDSMLOADC
   QDSMLOADA
   QGCAPELECDSM4
   QDSMMAXCO
   QDSMMINCO

*   QGRAMP
*   QGRAMP_UP
*   QGRAMP_DOWN


$ifi '%StartRamp%' == Yes $ifi not '%UnitCmin%' == Yes QGSTARTRAMP
$ifi '%StartRamp%' == Yes $ifi not '%UnitCmin%' == Yes QGSTARTRAMP_CHP


$ifi %UnitCmin%==yes $INCLUDE '%code_path_addons%\Mip\MIP_Model_2.inc'

$ifi '%FLEX_addon%' == yes FLEX_model
/;
*----- End of definition of the model -----------------------------------------
*}



*VersID_TSOs
*start version
*------------------------------------------------------------------------------
* Generate the cplex.opt file(s)
*   for LPs only cplex.opt
*   for MIPs cplex.opt and cplex.op2 are written
*------------------------------------------------------------------------------
$SETGLOBAL opt_cplexfile "cplex.opt"

*------------------------------
* LP solver options
$IFTHENI.cplex not '%UnitCmin%' == YES

$LOG "Writing cplex.opt for LP"

$ifi '%Code_version%'==UENB cplex_values_do_write('UENBValues') = 1;
$ifi '%Code_version%'==EWL cplex_values_do_write('EWLValues') = 1;

$ifi '%Code_version%'==UENB cplex_values('threads')$(not opt_cplex_external_value('threads')) = 8;
* LP: setting threads to > 1 may result in CPLEX perturbing the problem
* --> CPLEX will need additional time to remove the perturbation after
*     solving the perturbed problem
* The reason which causes CPLEX to perturb the problem might be
* degeneracy of the (primal) problem.

* cplex_values('tilim') = 10* X *cplex_values('iatriggertime') (DE: 10 * X = 10 * 3 = 30, DKW: 10 * X = 10 * 0,5 = 5)
$ifi '%Code_version%'==UENB cplex_values('tilim') = MAX (120,CEIL(CARD(G)*2)) *10 * MAX(0.5,CARD(G)/250);

$BATINCLUDE %code_path_addons%/CplexOpt/write_cplex_opt.gms 1

$ENDIF.cplex

*------------------------------
* MIP solver options
* '%UnitCmin%'  == Yes
$IFTHENI.cplex '%UnitCmin%' == YES

$LOG "Writing cplex.opt for MIP"

***

$ifi '%Code_version%'==UENB cplex_values_do_write('UENBValues') = 1;
$ifi '%Code_version%'==EWL cplex_values_do_write('EWLValues') = 1;

cplex_values('threads')$(not opt_cplex_external_value('threads')) = MIN(CEIL(CARD(G)/500),4);

cplex_values('epgap')$(not opt_cplex_external_value('epgap')) = 0.001;
$ifi '%Code_version%'==UENB cplex_values_do_write('epgap') = 1;
$ifi '%Code_version%'==EWL cplex_values_do_write('epgap') = 1;

* set polishtime to 0.1s per generator
cplex_values('polishtime')$(not opt_cplex_external_value('polishtime')) = MAX (20,CEIL(CARD(G)*0.1));
$ifi '%Code_version%'==UENB cplex_values_do_write('polishing') = 1;

cplex_values('iatriggertime') = MAX (120,CEIL(CARD(G)*2));
$ifi '%Code_version%'==UENB cplex_values_do_write('min_run_time') = 1;
$ifi '%Code_version%'==EWL cplex_values_do_write('min_run_time') = 1;

* cplex_values('tilim') = 10* X *cplex_values('iatriggertime') (DE: 10 * X = 10 * 3 = 30, DKW: 10 * X = 10 * 0,5 = 5)
$ifi '%Code_version%'==UENB cplex_values('tilim') = cplex_values('iatriggertime') *10 * MAX(0.5,CARD(G)/250);
*$ifi '%Code_version%'==EWL cplex_values('tilim') = cplex_values('iatriggertime') *10 * MAX(0.5,CARD(G)/250);


$BATINCLUDE %code_path_addons%/CplexOpt/write_cplex_opt.gms 1

*** write cplex.op2 that is only needed for MIP
* don't write min_run_time options
cplex_values_do_write('min_run_time') = 0;

$SETGLOBAL opt_cplexfile "cplex.op2"

cplex_values('epgap')$(not opt_cplex_external_value('epgap')) = 0.01;

* set polishtime to 0.3s per generator
$ifi '%Code_version%'==UENB cplex_values('polishtime')$(not opt_cplex_external_value('polishtime')) = MAX(60, CEIL(CARD(G)*0.3));

$BATINCLUDE %code_path_addons%/CplexOpt/write_cplex_opt.gms 2

$ENDIF.cplex
*end version

*------------------------------------------------------------------------------
*------------------------------------------------------------------------------
*------------------------------------------------------------------------------
* Loops
* Loops *{


* Outer LOOP (over all years) *{


LOOP(Y,

* ---- Update sets, parameters and bounds and set start values for current simulation year *{

* ---- Transmission ---- *{
* ---- Update sets with positive transmission capacities: ----

IEXPORT_Y(IR,IRALIAS)           = NO;
IEXPORT_Y(IR,IRALIAS)           = YES$(Sum(BASETIME,XCAPACITY(IR,IRALIAS,BASETIME))>0);

$ifi Not '%No_Load_Flow%' == Yes  IXTYPE_Y(IR,IRALIAS)    = XTYPE(IR,IRALIAS);
$ifi Not '%No_Load_Flow%' == Yes  I_AC_Line_Y(IR,IRALIAS) = NO;
$ifi Not '%No_Load_Flow%' == Yes  I_AC_Line_y(IR,IRALIAS) = Yes$(IXTYPE_Y(IR,IRALIAS) = 1);
$ifi Not '%No_Load_Flow%' == Yes  I_DC_Line_y(IR,IRALIAS) = NO;
$ifi Not '%No_Load_Flow%' == Yes  I_DC_Line_y(IR,IRALIAS) = Yes$(IXTYPE_Y(IR,IRALIAS) = 2);

$ifi '%LFB%' == Yes ISFD_Y_FLG(FLG)            = NO;
$ifi '%LFB%' == Yes ISFD_Y_FLG(FLG)            = YES$(sum(GRIDLOADCASE, XCAPACITY_FLG_SFD(FLG,GRIDLOADCASE))>0);

$ifi '%LFB%' == Yes INSFD_Y_FLG(FLG)           = NO;
$ifi '%LFB%' == Yes INSFD_Y_FLG(FLG)           = YES$(sum(GRIDLOADCASE, XCAPACITY_FLG_NSFD(FLG,GRIDLOADCASE))<0);

$ifi '%LFB%' == Yes IEXPORT_FLBMC_Y(IR,IRI)      = NO;


*$ifi '%LFB_NE%' == Yes IEXPORT_FLBMC_Y(IRE,IRI)     = RRR_RRR_FLBMC(IRE,IRI)$(SUM(GRIDLOADCASE,SUM(FLG, XCAPACITY_FLG_SFD_R(IRE,IRI,FLG,GRIDLOADCASE) - XCAPACITY_FLG_NSFD_R(IRE,IRI,FLG,GRIDLOADCASE)))>0);
$ifi '%LFB%' == Yes IEXPORT_FLBMC_Y(IRE,IRI)     = RRR_RRR_FLBMC(IRE,IRI);


* Save assignment flowgate to FBMC connection
*$ifi '%LFB_NE%' == Yes IRRRFLG_SFD(IR,IRI,FLG)= NO;
*$ifi '%LFB_NE%' == Yes IRRRFLG_NSFD(IR,IRI,FLG)= NO;
*$ifi '%LFB_NE%' == Yes IRRRFLG_SFD(IR,IRI,FLG)= YES $(SUM(GRIDLOADCASE, XCAPACITY_FLG_SFD_R(IR,IRI,FLG,GRIDLOADCASE))>0);
*$ifi '%LFB_NE%' == Yes IRRRFLG_NSFD(IRI,IR,FLG)= YES $(SUM(GRIDLOADCASE, XCAPACITY_FLG_SFD_R(IR,IRI,FLG,GRIDLOADCASE))>0);

$ifi '%LFB%' == Yes IEXPORT_FLBMC_DC_Y(IR,IRALIAS)      = NO;
$ifi '%Code_version%'==UENB $ifi '%LFB%' == Yes IEXPORT_FLBMC_DC_Y(IRE,IRI)         = YES$(sum(BASETIME,sum(NO_FLG,XCAPACITY_NO_FLG(IRE,IRI,BASETIME)))>0);
$ifi '%Code_version%'==EWL $ifi '%LFB%' == Yes IEXPORT_FLBMC_DC_Y(IRE,IRI)         = YES$(sum(NO_FLG,XCAPACITY_NO_FLG(IRE,IRI))>0);

* All connections between regions are NTC which are not FB-connections (including the DC connections)
IEXPORT_NTC_Y(IR,IRALIAS)            = NO;
$ifi     '%LFB%' == Yes IEXPORT_NTC_Y(IRE,IRI) = IEXPORT_Y(IRE,IRI) -IEXPORT_FLBMC_Y(IRE,IRI) -IEXPORT_FLBMC_DC_Y(IRE,IRI);
$ifi Not '%LFB%' == Yes IEXPORT_NTC_Y(IRE,IRI) = IEXPORT_Y(IRE,IRI);

$ifi  '%Kupferplatte%' ==  Yes          IXCAPACITY_Y(IR,IRALIAS,T)             = 200000;
$ifi  '%Kupferplatte%' ==  Yes          IXCAPACITY_Y_Day_ahead(IR,IRALIAS,T)   = 200000;

$ifi '%LFB%' ==  Yes  $ifi '%Kupferplatte%' ==  Yes     IXCAPACITY_NO_FLG(IR,IRALIAS,T)        = 200000;
$ifi '%LFB%' ==  Yes  $ifi '%Kupferplatte%' ==  Yes     IXCAPACITY_FLG_SFD(TRL,T)              = 200000;
$ifi '%LFB%' ==  Yes  $ifi '%Kupferplatte%' ==  Yes     IXCAPACITY_FLG_NSFD(TRL,T)             = -200000;

*}

* ---- Generation technologies ---- *{

* ---- Update generation capacities (parameters):
  IGELECCAPACITY_Y(IA,G)   = GKFXELEC(Y,IA,G);
$ifi '%EMOB_V2G%' == NO IGELECCAPACITY_Y(IA,IGELECSTOEV) = 0;

  IGHEATCAPACITY_Y(IA,G)   = GKFXHEAT(Y,IA,G);
$ifi '%CCGT_Imp%' == Yes IGSTEAMCAPACITY_Y(IA,G) = GKFXSTEAM(Y,IA,G);
$ifi '%FLEX_addon%' == Yes IGDSMCAPACITY_Y(IA,G) = GKFXDSM(Y,IA,G);

* Correction for extraction turbines to save solving time
  IGHEATCAPACITY_Y(IA,IGEXTRACTION) $(GKFXELEC(Y,IA,IGEXTRACTION) >0) =
                  MIN(GKFXHEAT(Y,IA,IGEXTRACTION),(IGELECCAPACITY_Y(IA,IGEXTRACTION)
                                                  /(GDATA(IA,IGEXTRACTION,'GDCV')+ GDATA(IA,IGEXTRACTION,'GDCB')+0.01) ));

* ---- Sets of generation technologies with positive capacity:
  IAGK_Y(IA,G) = NO;
  IAGK_Y(IA,G) = YES$((IGELECCAPACITY_Y(IA,G) >0) OR (IGHEATCAPACITY_Y(IA,G) >0) OR (GKFX_CONTENTSTORAGE(Y,IA,G)>0));
$ifi '%FLEX_addon%' == Yes IAGK_Y(IA,IGDSM) = YES$(IGDSMCAPACITY_Y(IA,IGDSM) > 0);
$ifi '%CCGT_Imp%' == Yes   IAGK_Y(IA,IGDUCTBURNER) = YES$(IGSTEAMCAPACITY_Y(IA,IGDUCTBURNER) > 0);

  IAHYDRO(IA)  = NO;
  IAHYDRO(IA)  = YES$ (Sum(IGHYDRORES,IGELECCAPACITY_Y(IA,IGHYDRORES))>0);
  
$ifi '%HydroSupplyCurves%' ==  Yes    IAHYDRO_HYDRORES(AAA) = IAHYDRO(AAA) - IAHYDRO_HYDRORES_STEP(AAA);
$ifi '%HydroSupplyCurves%' ==  NO     IAHYDRO_HYDRORES(AAA) = IAHYDRO(AAA);

$ifi '%HydroSupplyCurves%' ==  Yes    IAHYDROGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES)=NO;
$ifi '%HydroSupplyCurves%' ==  Yes    IAHYDROGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES)=YES$(IGELECCAPACITY_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES) >0);
  

  IAELECSTO(IA)  = NO;
  IAELECSTO(IA)  = YES$(Sum(IGELECSTORAGE,IGELECCAPACITY_Y(IA,IGELECSTORAGE))>0);

*IAHYDRO_ELECSTO(IA) = IAHYDRO(IA) + IAELECSTO(IA);


*------adjustment of Powerplant-Parameter for CCGT Units
$ifi '%CCGT_Imp%' == Yes  $INCLUDE '%code_path_addons%/CCGT/CCGT_Para.inc'

* ---- Parameters related to fuel use
  LOOP (IAGK_Y(IA,IGONESLOPE),
     IFUELUSAGE_SLOPE(IA,IGONESLOPE)        = GDATA(IA,IGONESLOPE,'GDFE_SLOPE');
  );

  LOOP (IAGK_Y(IA,IGELECPARTLOAD),
     IFUELUSAGE_SECTION(IA,IGELECPARTLOAD)  = GDATA(IA,IGELECPARTLOAD,'GDFE_SECTION');
  );

  LOOP (IAGK_Y(IA,IGELECNOPARTLOAD),
    IFUELUSAGE_SLOPE(IA,IGELECNOPARTLOAD)   = 1/(GDATA(IA,IGELECNOPARTLOAD,'GDFULLLOAD'));
    IFUELUSAGE_SECTION(IA,IGELECNOPARTLOAD) = 0;
  );

  LOOP (IAGK_Y(IA,IGHEATONLY),
    IFUELUSAGE_SLOPE(IA,IGHEATONLY)         = 1/(GDATA(IA,IGHEATONLY,'GDFULLLOAD'));
    IFUELUSAGE_SECTION(IA,IGHEATONLY)       = 0;
  );

  LOOP (IAGK_Y(IA,IGDUCTBURNER),
    IFUELUSAGE_SLOPE(IA,IGDUCTBURNER)         = 1/(GDATA(IA,IGDUCTBURNER,'GDFULLLOAD'));
    IFUELUSAGE_SECTION(IA,IGDUCTBURNER)       = 0;
  );

* ---- Capacity of load process and storage content
  LOOP (IAGK_Y(IA,IGSTORAGE),
    IGSTOLOADCAPACITY_Y(IA,IGSTORAGE)       = GKFX_CHARGINGSTORAGE(Y,IA,IGSTORAGE);
    IGSTOLOADCAPACITYMIN_Y(IA,IGSTORAGE)    = GKFX_MINCHARGINGSTORAGE(Y,IA,IGSTORAGE);
    IGSTOCONTENTCAPACITY_Y(IA,IGSTORAGE)    = GKFX_CONTENTSTORAGE(Y,IA,IGSTORAGE);
    IGSTOMINCONTENT_Y(IA,IGSTORAGE)         = GKFX_MINCONTENTSTORAGE(Y,IA,IGSTORAGE);
  );

*---- Minimum and Maximum content hydro reservoir
  LOOP (IAGK_Y(IAHYDRO,IGHYDRORES),
    IGHYDRORESMINCONTENT_Y(IAHYDRO,IGHYDRORES)      = GKMIN_CONTENTHYDRORES(Y,IAHYDRO,IGHYDRORES);
    IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES) = GKMAX_CONTENTHYDRORES(Y,IAHYDRO,IGHYDRORES);
  );

 LOOP (IAGK_Y(IAHYDRO,IGHYDRORES),
  IGResLoadCapacity_Y(IAHYDRO,IGHYDRORES) = GKFX_ChargingRes(Y,IAHYDRO,IGHYDRORES);
 );

* ---- Initialise start-up time ----------------------------------------------
  IGSTARTUPTIME(IAGK_Y(IA,IGUC)) = GDATA(IA,IGUC,'GDLEADTIMEHOT');

*---- Start values: -------------------------------------------------------
*Start conditions for IGUC:
*units with CHP-restriction
*Determine first value of MinCHPFactor depending on StartLoop
$ifi '%Looping%' ==  3hours Starttimecounter = (3*STARTLOOP)  + 1;
$ifi '%Looping%' == 12hours Starttimecounter = (12*STARTLOOP) + 1;
$ifi '%Looping%' == 24hours Starttimecounter = (24*STARTLOOP) + 1;
$ifi '%Looping%' == week    Starttimecounter = (168*STARTLOOP)+ 1;

*By default units are on in order to avoid ramping issues in the first hour
 Start_OnOff(IAGK_Y(IA,IGUC)) = 1;
 LOOP(IAGK_Y(IA,IGCHPBOUNDE),
  LOOP(BASETIME$(ORD(BASETIME)=Starttimecounter),
   Start_OnOff(IA,IGCHPBOUNDE)$(MAXCHPFACTOR(IA,IGCHPBOUNDE,BASETIME) = 0) = 0;
   Start_OnOff(IA,IGCHPBOUNDE)$(MinCHPFACTOR(IA,IGCHPBOUNDE,BASETIME) > 0) = 1;
  );
 );
*}

* ---- Initial values variables: ---- *{
* ---- Units producing electricity
  LOOP( IAGK_Y(IA,IGELEC),
    VGELEC_T.FX('T00',IA,IGELEC)            = GSTARTVALUEDATA(IA,IGELEC,'GSELEC')*IGELECCAPACITY_Y(IA,IGELEC);
    VGELEC_DPOS_T.FX('T00',IA,IGELEC)       = 0;
    VGELEC_DNEG_T.FX('T00',IA,IGELEC)       = 0;
  );

   LOOP((IA,IGUC)$IAGK_Y(IA,IGUC),
       LOOP (T_WITH_HIST $(ORD(T_WITH_HIST)<=25),

      );
   );

* ---- Units producing heat
  LOOP( IAGK_Y(IA,IGHEAT),
    VGHEAT_T.FX('T00',IA,IGHEAT)             = GSTARTVALUEDATA(IA,IGHEAT,'GSHEAT')*IGHEATCAPACITY_Y(IA,IGHEAT);
  );

* ---- Storages



$ifi '%Looping%' == week                        LOOP (BASETIME$(ord(BASETIME)=(1+((STARTLOOP-1)*168)) ),
$ifi not '%Looping%' == week  LOOP (BASETIME$(ord(BASETIME)=(1+((STARTLOOP-1)*12))  ),
                               LOOP ( IAHYDRO,
                                VCONTENTHYDRORES_T.FX('T00',IAHYDRO)=
                                 BASE_RESLEVELS_T(IAHYDRO,BASETIME)  / 100 *SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))
                                                                                                                                                );
                                                                                                                                                );


*this way to determine the reservoir starting level might be good for future years. Make sure the startvalue is a historical value, not random
*  LOOP ( IAHYDRO,
*    VCONTENTHYDRORES_T.FX('T00',IAHYDRO)     = Sum(IGHYDRORES,GSTARTVALUEDATA(IAHYDRO,IGHYDRORES,'GSCONTENTHYDRORES'));
*  );

  LOOP ( IAGK_Y(IA,IGSTORAGE),
    VCONTENTSTORAGE_T.FX('T00',IA,IGSTORAGE) = GSTARTVALUEDATA(IA,IGSTORAGE,'GSCONTENTSTORAGE') * IGSTOCONTENTCAPACITY_Y(IA,IGSTORAGE);
  );

* --- Availabilities
* If no availability due to planned revisions is given, set availability per default to 1:
$IFI %data_source% == Access  LOOP ( IAGK_Y(IA,IGHYRES_ELSTO),
$IFI %data_source% == Access                IF(SUM(BASETIME, BASE_GKDERATE_VAR_T(IA,IGHYRES_ELSTO,BASETIME))=0,
$IFI %data_source% == Access                BASE_GKDERATE_VAR_T(IA,IGHYRES_ELSTO,BASETIME) = 1;
$IFI %data_source% == Access         );
$IFI %data_source% == Access  );

*}
*}

* Counter for solve-loops
  INO_SOLVE=0;

* #Ensure that we always start with a day-ahead loop
$ifi NOT '%Looping%' == 3hours ILOOPINDICATOR =  MOD(STARTLOOP+1, NO_ROLL_PERIODS_WITHIN_DAY);
$ifi NOT '%Looping%' == 3hours IF(ILOOPINDICATOR NE 0,
$ifi NOT '%Looping%' == 3hours    STARTLOOP = STARTLOOP+1;
$ifi NOT '%Looping%' == 3hours );
$ifi  '%Looping%' == 3hours    ILOOPINDICATOR =  MOD(STARTLOOP+7, NO_ROLL_PERIODS_WITHIN_DAY);
$ifi  '%Looping%' == 3hours    WHILE(ILOOPINDICATOR NE 0,
$ifi  '%Looping%' == 3hours         STARTLOOP = STARTLOOP+1;
$ifi  '%Looping%' == 3hours         ILOOPINDICATOR =  MOD(STARTLOOP+7, NO_ROLL_PERIODS_WITHIN_DAY);
$ifi  '%Looping%' == 3hours    );
*}

*-------------------------------------------------------------------------------
* Input data consistency checks
*-------------------------------------------------------------------------------

* ----- MAXCHPFACTOR / MINCHPFACTOR

*The following line tests if MAXCHPFACTOR can be used.
$ifi     '%UnitCmin%' == yes                                                                    loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(MAXCHPFACTOR(IAGK_Y,BASETIME) > 0  AND MAXCHPFACTOR(IAGK_Y,BASETIME) < GDATA(IAGK_Y,'GDMINLOADFACTOR')                     -0.001),
$ifi     '%UnitCmin%' == yes                                    PUT ISOLVESTATUS_EXTENDED_OUT;
$ifi     '%UnitCmin%' == yes                                    PUT  / 'Error: MAXCHPFACTOR >0 and smaller than GDMINLOADFACTOR. Unit: ' IGCHPBOUNDE.TL:0 ' TIME: ' BASETIME.TL:0 /;
$ifi     '%UnitCmin%' == yes                                    IINPUTDATA_ERRORCOUNT('MAXCHP') = IINPUTDATA_ERRORCOUNT('MAXCHP') + 1;
$ifi     '%UnitCmin%' == yes                                                                    );

*The following lines test if MINCHPFACTOR can be used.
                                                                loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(MINCHPFACTOR(IAGK_Y,BASETIME) > MAXCHPFACTOR(IAGK_Y,BASETIME)+0.001),
                                                                PUT ISOLVESTATUS_EXTENDED_OUT;
                                                                PUT  / 'Error: MINCHPFACTOR larger than MAXCHPFACTOR. Unit: ' IGCHPBOUNDE.TL:0 ' TIME: ' BASETIME.TL:0 /;
                                                                IINPUTDATA_ERRORCOUNT('MINCHP') = IINPUTDATA_ERRORCOUNT('MINCHP') + 1;
                                                                      ;)

                                                                                                                                loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(MINCHPFACTOR(IAGK_Y,BASETIME) > BASE_GKDERATE_VAR_T(IAGK_Y,BASETIME)*(1-BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME))+0.001),
                                                                                                                                PUT ISOLVESTATUS_EXTENDED_OUT;
                                                                                                                                PUT  / 'Error: MINCHPFACTOR larger than GKDERATE. Unit: ' IGCHPBOUNDE.TL:0 ' TIME: ' BASETIME.TL:0 /;
                                                                                                                                IINPUTDATA_ERRORCOUNT('MINCHP_GKDERATE') = IINPUTDATA_ERRORCOUNT('MINCHP_GKDERATE') + 1;
                                                                                                                                                                                                                                                                                                                                                                                                ;)

* -- ABORT WITH ERROR MESSAGE

If (IINPUTDATA_ERRORCOUNT('MAXCHP') ge 1 AND IINPUTDATA_ERRORCOUNT('MINCHP') ge 1,
abort  'Error: 1) MAXCHPFACTOR >0 and smaller than GDMINLOADFACTOR 2) MINCHPFACTOR larger than MAXCHPFACTOR. Check ISOLVESTATUS_EXTENDED_OUT for further information.'
);

If (IINPUTDATA_ERRORCOUNT('MAXCHP') ge 1,
abort  'Error: MAXCHPFACTOR >0 and smaller than GDMINLOADFACTOR. Check ISOLVESTATUS_EXTENDED_OUT for further information.'
);

If (IINPUTDATA_ERRORCOUNT('MINCHP') ge 1,
abort  'Error: MINCHPFACTOR larger than MAXCHPFACTOR. Check ISOLVESTATUS_EXTENDED_OUT for further information.'
);

If (IINPUTDATA_ERRORCOUNT('MINCHP_GKDERATE') ge 1,
abort  'Error: MINCHPFACTOR larger than GKDERATE. Check ISOLVESTATUS_EXTENDED_OUT for further information.'
);

* -- CORRECTING NUMERICAL ERRORS

*MAXCHPFACTOR
$ifi     '%UnitCmin%' == yes            loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(     MAXCHPFACTOR(IAGK_Y,BASETIME) > 0
$ifi     '%UnitCmin%' == yes                                                             AND MAXCHPFACTOR(IAGK_Y,BASETIME) > GDATA(IAGK_Y,'GDMINLOADFACTOR')-0.001
$ifi     '%UnitCmin%' == yes                                                                                             AND MAXCHPFACTOR(IAGK_Y,BASETIME) < GDATA(IAGK_Y,'GDMINLOADFACTOR')  ),
$ifi     '%UnitCmin%' == yes                                                                                            MAXCHPFACTOR(IAGK_Y,BASETIME) = GDATA(IAGK_Y,'GDMINLOADFACTOR');
$ifi     '%UnitCmin%' == yes                 );

*MINCHPFACTOR
                                        loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(MINCHPFACTOR(IAGK_Y,BASETIME) > MAXCHPFACTOR(IAGK_Y,BASETIME)),
                                                                                                                                                                MINCHPFACTOR(IAGK_Y,BASETIME) = MAXCHPFACTOR(IAGK_Y,BASETIME);
                                                                                        );

                                                                                loop((IAGK_Y(IA,IGCHPBOUNDE),BASETIME)$(MINCHPFACTOR(IAGK_Y,BASETIME) > BASE_GKDERATE_VAR_T(IAGK_Y,BASETIME)*(1-BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME))),
                                                                                                                                                                MINCHPFACTOR(IAGK_Y,BASETIME) = BASE_GKDERATE_VAR_T(IAGK_Y,BASETIME)*(1-BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME));
                                                                                        );
* Nested inner LOOP (INFOTIME) *{
*-------------------------------------------------------------------------------
* LOOP over single periods.
* Every loopstep the time is shifted by NO_HOURS_OVERLAP time steps
* All start times of rolling planning runs, STARTLOOP and LOOPRUNS specified in Choice-File
* INFOTIME = All start times of rolling planning runs (currently two per day over one year, i.e. 732 loops in total)
* STARTLOOP = 1 -> model will start at 12:00 of the first day (Choice-File)

  LOOP (INFOTIME $( (ORD(INFOTIME) GE STARTLOOP) AND (ORD(INFOTIME) LT (LOOPRUNS+STARTLOOP))),

* ---- Preprocessing ---- *{
* ---- Dynamic sets/parameters depending on day-ahead or intraday ---- *{

    IT_SPOTPERIOD(IT_SPOTPERIOD)    = NO;
    IT_OPT(IT_OPT)                  = NO;
    IT_OPT_ALL(IT_OPT_ALL)          = NO;
    IT_OPT_ALL('T00')               = YES;
    IENDTIME(IENDTIME)              = NO;
    IENDTIME_HYDISDP(IENDTIME_HYDISDP)          = NO;
        IENDTIME_SHADOWPRICE(IENDTIME_SHADOWPRICE)  = NO;

* ILOOPINDICATOR indicates whether the current rolling planning loop is day-ahead (ILOOPINDICATOR=0) or intra-day (ILOOPINDICATOR=1)
$ifi NOT '%Looping%' == 3hours    ILOOPINDICATOR =  MOD((ORD(INFOTIME)), NO_ROLL_PERIODS_WITHIN_DAY);
$ifi     '%Looping%' == 3hours    ILOOPINDICATOR =  MOD((ORD(INFOTIME)+3), NO_ROLL_PERIODS_WITHIN_DAY);

$ifi '%Looping%' == week $goto loopcaseweek

* "Day-ahead loop"
    If (ILOOPINDICATOR EQ 0 OR INO_SOLVE=0,
        IBID_DAYAHEADMARKET_YES = 1;
        IT_SPOTPERIOD(DYN_DAY_AHEAD)  = YES;
        IF (INO_SOLVE EQ 0,
          IT_SPOTPERIOD(DYN_DAY_AHEAD_FIRST_SOLVE)=YES;
        );
* "Intra-day loop"
    ELSE
        IBID_DAYAHEADMARKET_YES       = 0;
    );

$ifi '%Looping%' == 3hours $goto loopcase3hours

* "Day-ahead loop"
    If (ILOOPINDICATOR EQ 0,
        IENDTIME('T36')                  = YES;
        IENDTIME_HYDISDP('T36')          = YES;
                IENDTIME_SHADOWPRICE('T12')      = YES;
        IT_OPT(DYN_DAY_AHEAD_FIRST_SOLVE)=YES;
        IT_OPT_ALL(DYN_DAY_AHEAD_FIRST_SOLVE)=YES;

        INO_HOURS_OPT_PERIOD          = 36;
        INO_HOURS_WITH_FIXED_VGELEC   = 12;
        IFIXSPINNING_UNITS_YES        = 1;
* IHLP_FLEXIBLE_DEF_YES defined in Choice-File (currently =0)
        IFLEXIBLE_DEF_YES             = IHLP_FLEXIBLE_DEF_YES;
        ICALC_SHADOWPRICE_YES         = 1;
        IUPDATE_SHADOWPRICE_YES       = 1;

* "Intra-day loop"
    ELSE
        IENDTIME('T24')                  = YES;
        IENDTIME_HYDISDP('T24')          = YES;
                IENDTIME_SHADOWPRICE('T12')      = YES;
        INO_HOURS_OPT_PERIOD          = 24;
        INO_HOURS_WITH_FIXED_VGELEC   = 24;
        IT_OPT(T_INTRADAY)=YES;
        IT_OPT_ALL(T_INTRADAY)=YES;

        IFIXSPINNING_UNITS_YES        = 0;
        IFLEXIBLE_DEF_YES             = 0;
        ICALC_SHADOWPRICE_YES         = 0;
        IUPDATE_SHADOWPRICE_YES       = 0;
    );
*}

$ifi NOT '%Looping%' == 3hours $goto loopcasedaily
$label loopcase3hours
* -- Loop case: assign values for 3hours looping
* "Day-ahead loop" (ILOOPINDICATOR = 0)
    If (ILOOPINDICATOR EQ 0,
        IENDTIME('T36')                  = YES;
        IENDTIME_HYDISDP('T36')          = YES;
                IENDTIME_SHADOWPRICE('T12')      = YES;
        IT_OPT(DYN_DAY_AHEAD_FIRST_SOLVE)=YES;
        IT_OPT_ALL(DYN_DAY_AHEAD_FIRST_SOLVE)=YES;

        INO_HOURS_OPT_PERIOD          = 36;
        INO_HOURS_WITH_FIXED_VGELEC   = 12;
        IFIXSPINNING_UNITS_YES        = 1;
* IHLP_FLEXIBLE_DEF_YES defined in Choice-File (currently =0)
        IFLEXIBLE_DEF_YES             = IHLP_FLEXIBLE_DEF_YES;
        ICALC_SHADOWPRICE_YES         = 1;
        IUPDATE_SHADOWPRICE_YES       = 1;

* "Intra-day loops" (ILOOPINDICATOR = 1-7)
    ELSE
        INO_HOURS_OPT_PERIOD                                                                                                    = 36-3*ILOOPINDICATOR;
        INO_HOURS_WITH_FIXED_VGELEC                                                                                             = 36-3*ILOOPINDICATOR;
        IENDTIME(T)$(ORD(T) EQ INO_HOURS_OPT_PERIOD+1)                          =YES;
        IENDTIME_HYDISDP(T)$(ORD(T) EQ INO_HOURS_OPT_PERIOD+1)= YES;
                IENDTIME_SHADOWPRICE('T12')      = YES;
        IT_OPT(T)$(ORD(T)>1 and ORD(T) <= INO_HOURS_OPT_PERIOD+1)                 =YES;
        IT_OPT_ALL(T)$(ORD(T)>1 and ORD(T) <= INO_HOURS_OPT_PERIOD+1)   =YES;

        IFIXSPINNING_UNITS_YES        = 0;
        IFLEXIBLE_DEF_YES             = 0;
        ICALC_SHADOWPRICE_YES         = 0;
        IUPDATE_SHADOWPRICE_YES       = 0;
    );

$ifi NOT '%Looping%' == week $goto loopcasedaily
$label loopcaseweek

$ifi '%FLEX_addon%' == YES               $INCLUDE '%code_path_addons%/FLEX/FLEX_addon_loops.inc';

* -- Loop cases: all weeks except first and last week of the year {
if((ORD(INFOTIME)) ne 53,

        IBID_DAYAHEADMARKET_YES = 1;
        IT_SPOTPERIOD(DYN_DAY_AHEAD)  = YES;

* not last loop and not last week of a year: WITH 12h forecast  (inputdata for timesteps of forecast is avaiable - guaranteed)
if(INO_SOLVE <> LOOPRUNS,

        IENDTIME(T)$(            ord(T) eq  NO_HOURS_OVERLAP+1)                 = YES;
        IENDTIME_HYDISDP(T)$(    ord(T) eq  card(DYN_DAY_AHEAD)+1)              = YES;
        IENDTIME_SHADOWPRICE(T)$(ord(T) eq  NO_HOURS_OVERLAP+1)                 = YES;

        IT_OPT(DYN_DAY_AHEAD)             =YES;
        IT_OPT_ALL(DYN_DAY_AHEAD)         =YES;

        INO_HOURS_OPT_PERIOD              = card(DYN_DAY_AHEAD);

* last loop but not last week of a year: WITHOUT 12h forecast   (inputdata for timesteps of forecast might not be avaiable, e.g. in case of a "Grenzsituation" which typically consists of 4 weeks)

else
        IENDTIME(T)$(            ord(T) eq  NO_HOURS_OVERLAP+1)                 = YES;
        IENDTIME_HYDISDP(T)$(    ord(T) eq  NO_HOURS_OVERLAP+1)                 = YES;
        IENDTIME_SHADOWPRICE(T)$(ord(T) eq  NO_HOURS_OVERLAP+1)                 = YES;

        IT_OPT(T)$(                             ord(T) gt 1 AND ord(T) le  NO_HOURS_OVERLAP+1)  =YES;
        IT_OPT_ALL(T)$(                         ord(T) le  NO_HOURS_OVERLAP+1)  =YES;

        INO_HOURS_OPT_PERIOD         = NO_HOURS_OVERLAP;
);
);
*}

* -- Loop cases: only last week of the year {
if( (ORD(INFOTIME)) eq 53,

        IBID_DAYAHEADMARKET_YES = 1;
        IT_SPOTPERIOD(DYN_DAY_AHEAD_LAST_SOLVE)  = YES;

        IENDTIME(T)$(ord(T)              eq  card(DYN_DAY_AHEAD_LAST_SOLVE)+1) = YES;
        IENDTIME_HYDISDP(T)$(ord(T)      eq  card(DYN_DAY_AHEAD_LAST_SOLVE)+1) = YES;
        IENDTIME_SHADOWPRICE(T)$(ord(T)  eq  card(DYN_DAY_AHEAD_LAST_SOLVE)+1) = YES;

        IT_OPT(DYN_DAY_AHEAD_LAST_SOLVE)=YES;
        IT_OPT_ALL(DYN_DAY_AHEAD_LAST_SOLVE)=YES;

        INO_HOURS_OPT_PERIOD  = card(DYN_DAY_AHEAD_LAST_SOLVE);

);
*}

        INO_HOURS_WITH_FIXED_VGELEC   = 0;
        IFIXSPINNING_UNITS_YES        = 1;
        IFLEXIBLE_DEF_YES             = IHLP_FLEXIBLE_DEF_YES;
        ICALC_SHADOWPRICE_YES         = 1;
        IUPDATE_SHADOWPRICE_YES       = 1;

$label loopcasedaily

* ---- Start and end time of this loop ------------------- *{

* First (DA-)loop:  IBASESTARTTIME=13, IBASEFINALTIMEWRITEOUT=24, IBASEFINALTIME=48
* Second (ID-)loop: IBASESTARTTIME=25, IBASEFINALTIMEWRITEOUT=36, IBASEFINALTIME=48
* IBASEFINALTIMEWRITEOUT indicates up to which time the results are written out to files

    IBASESTARTTIME = (ORD(INFOTIME)-1) * NO_HOURS_OVERLAP + 1;
    IBASEFINALTIMEWRITEOUT = IBASESTARTTIME + NO_HOURS_OVERLAP -1;
    If (ILOOPINDICATOR EQ 0,
       IBASEFINALTIME = IBASESTARTTIME + 3 * NO_HOURS_OVERLAP -1;
    ELSE
       IBASEFINALTIME = IBASESTARTTIME + 2 * NO_HOURS_OVERLAP -1;
    );

*Set elements for 3hours looping - overwrites previous seven lines if switch set to 3hours
$ifi '%Looping%' == 3hours              IBASESTARTTIME = (ORD(INFOTIME)-1) * NO_HOURS_OVERLAP + 1;
$ifi '%Looping%' == 3hours              IBASEFINALTIMEWRITEOUT = IBASESTARTTIME + NO_HOURS_OVERLAP -1;
$ifi '%Looping%' == 3hours              IBASEFINALTIME = IBASESTARTTIME + 12 + (8-ILOOPINDICATOR) * NO_HOURS_OVERLAP -1;

*Set elements for weekly looping - overwrites previous seven lines if switch set to week
$ifi '%Looping%' == week IBASESTARTTIME = (ORD(INFOTIME)-1) * 168 + 1;
$ifi '%Looping%' == week IBASEFINALTIME = IBASESTARTTIME +179;
$ifi '%Looping%' == week IBASEFINALTIMEWRITEOUT = IBASESTARTTIME +167;
*}

* ---- Outages (Part I): Obtain information on unplanned outages ---- *{

IF(IHELP_ANY_OUTAGE,

    LOOP (IAGK_Y,
      IKNOWN_OUTAGE(IAGK_Y) = 0;
      LOOP (BASETIME $( ORD(BASETIME) eq ( IBASESTARTTIME-1 )),
        IF ((BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME) eq 1) and (BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME+1) eq 1 ),
          IKNOWN_OUTAGE(IAGK_Y) = 1
        );
      );

      INO_TIMESTEPSTRANSFER_OUTAGE(IAGK_Y) = NO_HOURS_OVERLAP;
      ISTOP_LOOP = 0;

      LOOP (BASETIME $(( ORD(BASETIME) >= IBASESTARTTIME + NO_HOURS_OVERLAP ) AND (ORD(BASETIME) <= IBASEFINALTIME)
                        AND ISTOP_LOOP EQ 0 ),
        IF (BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME) eq 1,
          INO_TIMESTEPSTRANSFER_OUTAGE(IAGK_Y) = INO_TIMESTEPSTRANSFER_OUTAGE(IAGK_Y)+1;
        ELSE
          ISTOP_LOOP = 1;
        );
      );

      LOOP (BASETIME $(( ORD(BASETIME) >= IBASESTARTTIME) AND (ORD(BASETIME) <= IBASEFINALTIME)),
        LOOP ( T $(ORD(T) = (ORD(BASETIME) - IBASESTARTTIME + 2)),
          IF ( ORD(T) LE INO_TIMESTEPSTRANSFER_OUTAGE(IAGK_Y) + 1,
            IG_UNP_OUTAGE(IAGK_Y,T) = BASE_UNP_OUTAGE_VAR_T(IAGK_Y,BASETIME);
          ELSE
            IG_UNP_OUTAGE(IAGK_Y,T) = 0;
          );
        );
      );
    );
* end loop IAGK_Y

    IGELEC_UNP_OUTAGE(IGELEC_UNP_OUTAGE) = NO;
    LOOP(IAGK_Y  $(SUM(T $((ORD(T)> 1) and (ORD(T) LE (NO_HOURS_OVERLAP+1))), IG_UNP_OUTAGE(IAGK_Y,T)) GE 1),
      IGELEC_UNP_OUTAGE(IAGK_Y)=YES ;
    );
    );
*}

* ---- Parameters: Copy input values (assigned in basevar.inc) to internal parameters for time steps T01*T36 for DA-loop and T01*T24 for ID-loop *{

    LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME ) AND (ORD(BASETIME) <= IBASEFINALTIME) ),
      LOOP ( T $(ORD(T)= (ORD(BASETIME) - IBASESTARTTIME + 2)),
          IDEMANDHEAT(IACHP_ENDO,T)                 = BASE_DH_VAR_T(IACHP_ENDO,BASETIME);

$ifi Not '%Kupferplatte%' ==  Yes          IXCAPACITY_Y(IR,IRALIAS,T)             = XCAPACITY(IR,IRALIAS,BASETIME);
$ifi Not '%Kupferplatte%' ==  Yes          IXCAPACITY_Y_Day_ahead(IR,IRALIAS,T)   = XCAPACITY_Day_ahead(IR,IRALIAS,BASETIME);
$ifi '%Code_version%'==UENB $ifi '%LFB%' ==  Yes  $ifi Not '%Kupferplatte%' ==  Yes   IXCAPACITY_NO_FLG(IR,IRALIAS,T)        = XCAPACITY_NO_FLG(IR,IRALIAS,BASETIME);
$ifi '%Code_version%'==EWL $ifi '%LFB%' ==  Yes  $ifi Not '%Kupferplatte%' ==  Yes   IXCAPACITY_NO_FLG(IR,IRALIAS,T)        = XCAPACITY_NO_FLG(IR,IRALIAS);
$ifi '%LFB%' ==  Yes  $ifi Not '%Kupferplatte%' ==  Yes   IXCAPACITY_FLG_SFD(TRL,T)              = SUM(GRIDLOADCASE $BASETIME_GRIDLOADCASE(BASETIME,GRIDLOADCASE),XCAPACITY_FLG_SFD(TRL,GRIDLOADCASE));
$ifi '%LFB%' ==  Yes  $ifi Not '%Kupferplatte%' ==  Yes   IXCAPACITY_FLG_NSFD(TRL,T)             = SUM(GRIDLOADCASE $BASETIME_GRIDLOADCASE(BASETIME,GRIDLOADCASE),XCAPACITY_FLG_NSFD(TRL,GRIDLOADCASE));

$ifi '%LFB%' ==  Yes     IPTDF_FLG(RRR,TRL,T)                   = SUM(GRIDLOADCASE $BASETIME_GRIDLOADCASE(BASETIME,GRIDLOADCASE),PTDF_FLG(RRR,TRL,GRIDLOADCASE));

                                                                   IGKDERATE(IAGK_Y,T)               = BASE_GKDERATE_VAR_T(IAGK_Y,BASETIME);

$ifi '%FLEX_addon%' == YES      IGKDerate_DSM(IAGK_Y,T)           = BASE_GKDERATE_DSM_VAR_T(IAGK_Y,BASETIME);

                                IGKDERATE(IAGK_Y(IA,IGHEATBOILER),T)                 = 1;
                                IGKDERATE(IAGK_Y(IA,IGHEATSTORAGE),T)                 = 1;
                                IGKDERATE(IAGK_Y(IA,IGHEATPUMP),T)                 = 1;
                                IGKDERATE(IAGK_Y(IA,IGWIND),T)                 = 1;
                                IGKDERATE(IAGK_Y(IA,IGsolar),T)                = 1;
                                IGKDERATE(IAGK_Y(IA,IGWave),T)                 = 1;
                                IGKDERATE(IAGK_Y(IA,IGTidalStream),T)          = 1;
                                IGKDERATE(IAGK_Y(IA,IGDisgen),T)               = 1;
                                IGKDERATE(IAGK_Y(IA,IGBiomass),T)              = 1;
                                IGKDERATE(IAGK_Y(IA,IGsolarTH),T)              = 1;
                                IGKDERATE(IAGK_Y(IA,IGRUNOFRIVER),T)           = 1;
                                IGKDerate(IAGK_Y(IA,IGOthRes),T)               = 1;
if (sum(IAGK_Y(IA,IGELECSTODSM), IGKDERATE(IA,IGELECSTODSM,T)) = 0,
                                IGKDERATE(IAGK_Y(IA,IGELECSTODSM),T)          = 1; );
if (sum(IAGK_Y(IA,IGELECSTOEV), IGKDERATE(IA,IGELECSTOEV,T)) = 0,
                                IGKDERATE(IAGK_Y(IA,IGELECSTOEV),T)          = 1; );



                                                                                                 IGKDERATE(IAGK_Y(IA,IGELECSTORAGE),T)  = 1;
                                 IGKDERATE(IAGK_Y(IA,IGHYDRORES),T)     = 0.95;


*---- MAXCHPFACTOR
*values assigned to internal parameters
        IMAXCHPFACTOR(IAGK_Y,T) = MAXCHPFACTOR(IAGK_Y,BASETIME);
        IMAXCHPFACTOR(IAGK_Y,T)$(IMAXCHPFACTOR(IAGK_Y,T) > IGKDERATE(IAGK_Y,T)*(1-IG_UNP_OUTAGE(IAGK_Y,T))) = IGKDERATE(IAGK_Y,T)*(1-IG_UNP_OUTAGE(IAGK_Y,T));



*Testing IMAXCHPFACTOR
* --> see "Input data consistency checks"
*Correcting numerical errors.

*---- MINCHPFACTOR
*values assigned to internal parameters
        IMINCHPFACTOR(IAGK_Y,T) = MINCHPFACTOR(IAGK_Y,BASETIME);

*Testing IMINCHPFACTOR
* --> see "Input data consistency checks"
*Correcting numerical errors.

*statement S. Spieker (18.5.2017): IMINVGONLINE should be considered
        IMINVGONLINE(IA,IGUC_NOALWAYS,T)= Min(SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),BASE_MinVGOnline_VAR_T(IA,IGUC_NOALWAYS,WEEKS)),IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)*IGKDerate(IA,IGUC_NOALWAYS,T), IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)*IMAXCHPFACTOR(IA, IGUC_NOALWAYS,T));
        IMINVGONLINE_CHP(IA,IGFIXHEAT,T)= BASE_MinVGOnline_CHP_VAR_T(IA,IGFIXHEAT,BASETIME);

*---- IMinGen
        IMinGen(IA,IGDISPATCH,T) = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),Base_MinGeneration_Var_T(IA,IGDISPATCH,WEEKS));

          ISTARTRAMP_CHP(IAGK_Y,T) = StartRamp_CHP(IAGK_Y,Basetime);
          IMax_Delta_VGELEC_CHP(IAGK_Y,T)=Max_Delta_VGELEC_CHP(IAGK_Y,BASETIME);

$ifi NOT '%UnitCmin%' == yes    IMaxGen(IAGK_Y,T)=1;
$ifi     '%UnitCmin%' == yes    IMaxGen(IAGK_Y,T)=IGKDERATE(IAGK_Y,T);
*         IMaxGen(IAGK_Y,T)=IGKDERATE(IAGK_Y,T);

          IDEMANDELEC_VAR_T(IR,T)           = BASE_DE_VAR_T(IR,BASETIME);
          IDEMAND_NONSPIN_ANCPOS(TSO_RRR,T)   = BASE_DEMAND_NONSPIN_ANCPOS_N_1(TSO_RRR);
          IDEMAND_NONSPIN_ANCNEG(TSO_RRR,T)   = BASE_DEMAND_NONSPIN_ANCNEG_N_1(TSO_RRR);

* Update wind infeed only in DA-loops for time steps in which bidding is possible (T13*T36 and T01*T36 in first loop)
* In all other time steps (ID-loop, DA-loop T01*T12), the values are shifted at the end of the preceding loop
          IDEMANDELEC_BID_IR(IR,T)$IT_SPOTPERIOD(T) = IDEMANDELEC_VAR_T(IR,T) ;

*          IWIND_BID_IR(IR,T)$IT_SPOTPERIOD(T)       = IWIND_REALISED_IR(IR,T);

* Assigning Wind Forecasts for both regions that use / don't use forecasts below
* S P O T P E R I O D
* for Regions NOT using forecasts => IWIND_BID is set to Realisation
                                        IWIND_BID_IR(IRNOWINDFORECAST,T)$IT_SPOTPERIOD(T) = BASE_WIND_Var_Det(IRNOWINDFORECAST,BASETIME);

* for Regions using forecasts              => IWIND_BID is set to corresponding DA-Forecast
                                        IWIND_BID_IR(IRWINDFORECAST,T)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)) = BASE_WIND_VAR_NT(INFOTIME,IRWINDFORECAST,T);


* I N T R A D A Y - P E R I O D
* for Regions NOT using forecasts
                                        IWIND_REALISED_IR(IRNOWINDFORECAST,T) = BASE_WIND_VAR_Det(IRNOWINDFORECAST,BASETIME);

* for Regions using forecasts
* here we should use for IWIND_REALISED_IR the intraday forecasts for all periods except the first hours.
                                        IWIND_REALISED_IR(IRWINDFORECAST,T) $(ORD(T) > (NO_HOURS_OVERLAP + 1))  = BASE_WIND_VAR_NT(INFOTIME,IRWINDFORECAST,T);
* for the first hours (those that are not recomputed in the following loop, we use the realised values as forecasts.
                                        IWIND_REALISED_IR(IRWINDFORECAST,T) $(ORD(T) <= (NO_HOURS_OVERLAP + 1)) = BASE_WIND_VAR_Det(IRWINDFORECAST,BASETIME);


          IX3COUNTRY_T_Y(IR,T)           = BASE_X3FX_VAR_T(IR,BASETIME);
*          IIMPORTFLAG_T(IR,T)            = BASE_IMPORT_FLAG(IR,BASETIME);
                  SHEDDING_FORBIDDEN_DUE_TO_IMPORT(IR, T) = yes$(BASE_IMPORT_FLAG(IR,BASETIME) eq 1);
                  SHEDDING_FORBIDDEN_DUE_TO_ENTSO(IR, T) = yes$( BASE_NoDumpInENTSOFlag(IR,BASETIME) eq 1 );
                  SHEDDING_FORBIDDEN_TOTAL(IR, T) = yes$(
                    (SHEDDING_FORBIDDEN_DUE_TO_ENTSO(IR, T) AND (SHEDDING_FORBIDDEN_OPT>=2))
                        OR
                        (SHEDDING_FORBIDDEN_DUE_TO_IMPORT(IR, T) AND (SHEDDING_FORBIDDEN_OPT>=1)) );

          IRUNRIVER_AAA_T(IA,T)          = BASE_RUNRIVER_VAR_T(IA,BASETIME);
          IRUNRIVER_VAR_T(IR,T)          = SUM(IA $RRRAAA(IR,IA), IRUNRIVER_AAA_T(IA,T));

          ITIDALSTREAM_VAR_T(IR,T)       = BASE_TIDALSTREAM_VAR_T(IR,BASETIME);
                  ITIDALSTREAM_VAR_T2(IGTIDALSTREAM,IR,T)$(Totcap_Tidal_NRES(IR)>0) = ITIDALSTREAM_VAR_T(IR,T)*Share_Tidal_Nres(IGTIDALSTREAM,IR);
          IWAVE_VAR_T(IR,T)              = BASE_WAVE_VAR_T(IR,BASETIME);
          IHYDROINFLOW_AAA_T(IAHYDRO,T)  = BASE_INFLOWHYDRORES_VAR_T(IAHYDRO,BASETIME);

          IGEOTHHEAT_VAR_T(IR,T)         = BASE_GEOTHERMAL_VAR_T(IR,BASETIME);
$ifi '%CHP%'==Yes                IHEATEXT_EXO(IA,G,T)           = BASE_FIXQ(IA,G,BASETIME);
$ifi '%Code_version%'==EWL $ifi '%CHP%'==Yes               IBOUNDE_MIN(IA,G,T)            = BASE_BOUNDE_MIN(IA,G,BASETIME);
$ifi '%Code_version%'==EWL $ifi '%CHP%'==Yes               IBOUNDE_MAX(IA,G,T)            = BASE_BOUNDE_MAX(IA,G,BASETIME);


          IRESLEVELS_T(IAHYDRO,T)        = BASE_RESLEVELS_T(IAHYDRO,BASETIME);
          ISOLAR_VAR_T(IR,T)             = BASE_SOLAR_VAR_T(IR,BASETIME);
          IDISGEN_VAR_T(IR,T)            = BASE_DISGEN_VAR_T(IR,BASETIME);
                        if(Card(IGDISGEN)>0,
              IDISGEN_VAR_T2(IR,T)           = IDISGEN_VAR_T(IR,T)/(Card(IGDISGEN));
              else
              IDISGEN_Var_T2(IR,T)           = 0;
              );

    ISOLARTH_VAR_T(IR,T)           = BASE_SOLARTH_VAR_T(IR,BASETIME);
    IBIOMASS_VAR_T(IR,T)           = BASE_BIOMASS_VAR_T(IR,BASETIME);
    IOthRes_VAR_T(IR,T)            = Base_OthRes_VAR_T(IR,Basetime);

    IEV_Leave_Var_T(IA,G,T)   = BASE_EV_Leave_VAR_T(IA,G,BASETIME);
    IEV_Arrive_Var_T(IA,G,T)  = BASE_EV_Arrive_VAR_T(IA,G,BASETIME);
    IEV_SOC_Leave_T(IA,G,T)   = BASE_EV_SOC_Leave_T(IA,G,BASETIME);
    IEV_SOC_Arrive_T(IA,G,T)  = BASE_EV_SOC_Arrive_T(IA,G,BASETIME);


          IFUELPRICE_Y(IA,FFF,T)         = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),FUELPRICE(IA,FFF,WEEKS));
          IFUELTRANS_Y(IA,IGUSINGFUEL,T) = GDFUELTRANSCOST(IA,IGUSINGFUEL) ;
          IM_POL(MPOLSET,C,T)            = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),M_POL(MPOLSET,C,WEEKS));

$ifi '%HydroSupplyCurves%' ==  Yes       IMEDRESLEVELS_T(IAHYDRO_HYDRORES_STEP,T) =  BASE_MEDIAN_RESLEVEL_ISDPHYDRORES (IAHYDRO_HYDRORES_STEP,BASETIME);
*PtG Use Value

*$ifi '%PtG%' ==  Yes LOOP(C,
*$ifi '%PtG%' ==  Yes IUSEVALUE(IAGK_Y(IA,IGPTG))$ICA(C,IA) =
*$ifi '%PtG%' ==  Yes (IFUELPRICE_Y(IA,'NAT_GAS','T01') + IM_POL('TAX_CO2',C,'T01')*FDATA('NAT_GAS','FDCO2')*CONVERT_KG_PER_GJ_T_PER_MWH)*GDATA(IA,IGPTG,'GDLOADEFF');
*$ifi '%PtG%' ==  Yes )


$ifi '%PtG%' ==  Yes     LOOP(IAGK_Y(IA,IGPTG),
*$ifi '%PtG%' ==  Yes     IF ((GUSEVALUE(IA,IGPTG) GT 0),
$ifi '%PtG%' ==  Yes     IUSEVALUE(IA,IGPTG) = GUSEVALUE(IA,IGPTG))
*)
;


* --- Water value / price profile for hydro reservoirs in areas with hydro supply function
$ifi '%HydroSupplyCurves%' ==  Yes   LOOP(C,
$ifi '%HydroSupplyCurves%' ==  Yes   ISDP_HYDRORES_STEP(IAHYDROGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES),T)$(IT_OPT(T)AND ICA(C,IAHYDRO_HYDRORES_STEP)) =

$ifi '%HydroSupplyCurves%' ==  Yes              Max(1,
$ifi '%HydroSupplyCurves%' ==  Yes              BASE_COEFF_REF_ISDPHYDRORES(IAHYDRO_HYDRORES_STEP,IGHYDRORES)
$ifi '%HydroSupplyCurves%' ==  Yes            + BASE_COEFF_COAL_ISDPHYDRORES(IAHYDRO_HYDRORES_STEP,IGHYDRORES)*IFUELPRICE_Y(IAHYDRO_HYDRORES_STEP,'COAL',T)
$ifi '%HydroSupplyCurves%' ==  Yes            + BASE_COEFF_GAS_ISDPHYDRORES(IAHYDRO_HYDRORES_STEP,IGHYDRORES) *IFUELPRICE_Y(IAHYDRO_HYDRORES_STEP,'NAT_GAS',T)
$ifi '%HydroSupplyCurves%' ==  Yes            + BASE_COEFF_CO2_ISDPHYDRORES(IAHYDRO_HYDRORES_STEP,IGHYDRORES) *IM_POL('TAX_CO2',C,T)
$ifi '%HydroSupplyCurves%' ==  Yes            + BASE_COEFF_RESLEVEL_ISDPHYDRORES(IAHYDRO_HYDRORES_STEP,IGHYDRORES)*(IMEDRESLEVELS_T(IAHYDRO_HYDRORES_STEP,'T01') - 100 * VCONTENTHYDRORES_T.l('T00',IAHYDRO_HYDRORES_STEP)/SUM(IGHYDRORESALIAS,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORESALIAS))));
$ifi '%HydroSupplyCurves%' ==  Yes      );


*---- Shadrow price for hydro reservoir in areas without hydro supply function
*---- Shadow price for hydro reservoirs increases if water level goes below what's normal for the time of the year ---

$ifi '%HydroPenalty%' == NO  IF ((IBID_DAYAHEADMARKET_YES EQ 1),
$ifi '%HydroPenalty%' == NO          ISDP_HYDRORES(IAHYDRO,T) $IT_SPOTPERIOD(T) = MAX(1,
$ifi '%HydroPenalty%' == NO                 ISDP_REFPRICE(IAHYDRO) + WEIGHT_ISDP_HYDRORES *(IRESLEVELS_T(IAHYDRO,'T01')
$ifi '%HydroPenalty%' == NO                 - 100 * VCONTENTHYDRORES_T.l('T00',IAHYDRO)/SUM(IGHYDRORES, IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))));
$ifi '%HydroPenalty%' == NO  );


*$ifi '%HydroPenalty%' == YES If(INO_SOLVE EQ 0,
$ifi '%HydroPenalty%' == YES ISDP_HYDRORES(IAHYDRO,IENDTIME_HYDISDP) = MAX(1,ISDP_REFPRICE(IAHydro) + (min(100, 10 + SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))/SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*25/1000))*(IRESLEVELS_T(IAHYDRO,'T01')/100
$ifi '%HydroPenalty%' == YES                       - VCONTENTHYDRORES_T.l('T00',IAHYDRO)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES)) ));
*$ifi '%HydroPenalty%' == YES  );


    IPMPMAX(IAHYDRO,IGHYDRORES,T) = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),Base_PmPMax_Var_T(IAHYDRO,IGHYDRORES,WEEKS));
    IHYRESMAXGEN(IAHYDRO,IGHYDRORES,T) = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),Base_HYRESMAXGEN_Var_T(IAHYDRO,IGHYDRORES,WEEKS));
$ifi '%Looping%' == week    IPMPMAX_E(IAHYDRO,IGHYDRORES,T) = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),Base_HYRESMAXPMP_E_Var_T(IAHYDRO,IGHYDRORES,WEEKS));
$ifi '%Looping%' == week    IPMPMIN_E(IAHYDRO,IGHYDRORES,T) = SUM(WEEKS$BASETIME_WEEK(BASETIME,WEEKS),Base_HYRESMINPMP_E_Var_T(IAHYDRO,IGHYDRORES,WEEKS));

$ifi '%FLEX_addon%' == YES   IDemand_EV(IA,T) = Demand_EV(IA,BASETIME);

* --- Demand for positive and negative primary reserve (once per day)
 IDEMAND_ANCNEG_SEC(TSO_RRR,T)= sum(BASEDAY$BASETIME_BASEDAY(BASETIME,BASEDAY), BASE_DE_ANCNEG_VAR_D(TSO_RRR,BASEDAY));
 IDEMAND_ANCPOS_SEC(TSO_RRR,T)= sum(BASEDAY$BASETIME_BASEDAY(BASETIME,BASEDAY), BASE_DE_ANCPOS_VAR_D(TSO_RRR,BASEDAY));
 IDEMAND_ANCNEG_PRI(TSO_RRR,T)= sum(BASEDAY$BASETIME_BASEDAY(BASETIME,BASEDAY), BASE_DE_ANCNEG_PRI_VAR_D(TSO_RRR,BASEDAY));
 IDEMAND_ANCPOS_PRI(TSO_RRR,T)= sum(BASEDAY$BASETIME_BASEDAY(BASETIME,BASEDAY), BASE_DE_ANCPOS_PRI_VAR_D(TSO_RRR,BASEDAY));
 IDEMAND_ANCNEG(TSO_RRR,T)= IDEMAND_ANCNEG_PRI(TSO_RRR,T) + IDEMAND_ANCNEG_SEC(TSO_RRR,T) ;
 IDEMAND_ANCPOS(TSO_RRR,T)= IDEMAND_ANCPOS_PRI(TSO_RRR,T) + IDEMAND_ANCPOS_SEC(TSO_RRR,T);

      );
    );

$ifi '%HydroPenalty%' == YES VQRESLEVELUP.up(T,IAHYDRO,IHYDFLXSTEP)$(IENDTIME(T)) = ((ORD(IHYDFLXSTEP)**0.33)/SUM(IHYDFLXSTEPALIAS,(ORD(IHYDFLXSTEPALIAS)**0.33)))*SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))*(1-IRESLEVELS_T(IAHYDRO,T)/100);
$ifi '%HydroPenalty%' == YES VQRESLEVELDOWN.up(T,IAHYDRO,IHYDFLXSTEP)$(IENDTIME(T)) = ((ORD(IHYDFLXSTEP)**0.33)/SUM(IHYDFLXSTEPALIAS,(ORD(IHYDFLXSTEPALIAS)**0.33)))*(SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))*(IRESLEVELS_T(IAHYDRO,T)/100)-SUM(IGHYDRORES,IGHYDRORESMINCONTENT_Y(IAHYDRO,IGHYDRORES)));

*$ifi '%HydroPenalty%' == YES VQRESLEVELUP.up(T,IAHydro,IHYDFLXSTEP)$(IENDTIME(t))  =0.0025*ORD(IHYDFLXSTEP)**0.33*Sum(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES));
*$ifi '%HydroPenalty%' == YES VQRESLEVELDOWN.up(T,IAHydro,IHYDFLXSTEP)$(IENDTIME(t))=0.0025*ORD(IHYDFLXSTEP)**0.33*Sum(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES));

*$ifi '%HydroPenalty%' == NO  VQRESLEVELUP.up(T,IAHydro,IHYDFLXSTEP)$(IENDTIME(t))  =0;
*$ifi '%HydroPenalty%' == NO  VQRESLEVELDOWN.up(T,IAHydro,IHYDFLXSTEP)$(IENDTIME(t))=0;

* Calculate expected, average value of fluctuating production bid into the day-ahead market
    IF (INO_SOLVE EQ 0,
      IWIND_BID_IR(IR,T)       = IWIND_REALISED_IR(IR,T);
      IDEMANDELEC_BID_IR(IR,T) = IDEMANDELEC_VAR_T(IR,T);
    );

* ---- calculate parameter available capacity ---------------------------------
$ifi Not '%UnitCmin%' == Yes     IGELECCAPEFF(IA,G,T)$(ORD(T)>1) = IGELECCAPACITY_Y(IA,G) * IGKDERATE(IA,G,T)      * (1-IG_UNP_OUTAGE(IA,G,T));
$ifi     '%UnitCmin%' == Yes     IGELECCAPEFF(IA,G,T)$(ORD(T)>1) = IGELECCAPACITY_Y(IA,G) * CEIL(IGKDERATE(IA,G,T) * (1-IG_UNP_OUTAGE(IA,G,T)));

*For first loop: assign values for TM24-T00
IF (INO_SOLVE EQ 0,
   LOOP((IA,IGUC)$IAGK_Y(IA,IGUC),
       LOOP (T_WITH_HIST $(ORD(T_WITH_HIST)<=25),
                                                        IGELECCAPEFF(IA,G,T_WITH_HIST)=IGELECCAPEFF(IA,G,'T01')
      );
   );
   );

    IGELECCAPEFF_CHP(IA,G,T)=IGELECCAPACITY_Y(IA,G)*IMAXCHPFACTOR(IA,G,T);
* --- End Update of parameter values
*}

* ---- Variables: fix/unfix *{

  IF (IBID_DAYAHEADMARKET_YES EQ 1,

* ---- remove fixing of VGELEC_T and the spinning reserve variables for those timesteps where day-ahead market bidding is allowed (#KD: T13*T36, in first iteration T01*T36)

      LOOP ((IAGK_Y(IA,IGELEC),T)  $IT_SPOTPERIOD(T),
        VGELEC_T.UP(T,IA,IGELEC) = INF;
        VGELEC_T.LO(T,IA,IGELEC) = 0;
        VGE_ANCPOS.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCPOS.LO(T,IA,IGSPINNING) = 0;
        VGE_ANCNEG.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCNEG.LO(T,IA,IGSPINNING) = 0;
        VGE_NONSPIN_ANCPOS.UP(T,IA,IGSPINNING) = INF;
        VGE_NONSPIN_ANCPOS.LO(T,IA,IGSPINNING) = 0;
        VGE_SPIN_ANCPOS.UP(T,IA,IGSPINNING) = INF;
        VGE_SPIN_ANCPOS.LO(T,IA,IGSPINNING) = 0;

        VGE_ANCPOS_PRI.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCPOS_PRI.LO(T,IA,IGSPINNING) = 0;
        VGE_ANCPOS_SEC.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCPOS_SEC.LO(T,IA,IGSPINNING) = 0;
        VGE_ANCNEG_PRI.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCNEG_PRI.LO(T,IA,IGSPINNING) = 0;
        VGE_ANCNEG_SEC.UP(T,IA,IGSPINNING) = INF;
        VGE_ANCNEG_SEC.LO(T,IA,IGSPINNING) = 0;
        VGE_SPIN_ANCNEG.UP(T,IA,IGSPINNING) = INF;
        VGE_SPIN_ANCNEG.LO(T,IA,IGSPINNING) = 0;
      );

      LOOP ((IAGK_Y(IA,IGESTORAGE_DSM),T)  $IT_SPOTPERIOD(T),
        VGELEC_CONSUMED_T.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGELEC_CONSUMED_T.LO(T,IA,IGESTORAGE_DSM) = 0;

        VGELEC_T.UP(T,IA,IGHEATPUMP) = INF;
        VGELEC_T.LO(T,IA,IGHEATPUMP) = 0;

        VGE_CONSUMED_ANCPOS.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCPOS.LO(T,IA,IGESTORAGE_DSM) = 0;
        VGE_CONSUMED_ANCNEG.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCNEG.LO(T,IA,IGESTORAGE_DSM) = 0;
        VGE_CONSUMED_NONSP_ANCPOS.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_NONSP_ANCPOS.LO(T,IA,IGESTORAGE_DSM) = 0;

        VGE_CONSUMED_ANCPOS_PRI.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCPOS_PRI.LO(T,IA,IGESTORAGE_DSM) = 0;
        VGE_CONSUMED_ANCPOS_SEC.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCPOS_SEC.LO(T,IA,IGESTORAGE_DSM) = 0;
        VGE_CONSUMED_ANCNEG_PRI.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCNEG_PRI.LO(T,IA,IGESTORAGE_DSM) = 0;
        VGE_CONSUMED_ANCNEG_SEC.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_ANCNEG_SEC.LO(T,IA,IGESTORAGE_DSM) = 0;

        VGE_CONSUMED_NONSP_ANCNEG.UP(T,IA,IGESTORAGE_DSM) = INF;
        VGE_CONSUMED_NONSP_ANCNEG.LO(T,IA,IGESTORAGE_DSM) = 0;
      );


   LOOP ((IAGK_Y(IAHydro,IGHydroRes),T)  $IT_SPOTPERIOD(T),
    VGELEC_CONSUMED_T.UP(T,IAHYDRO,IGHydroRes) = INF;
    VGELEC_CONSUMED_T.LO(T,IAHYDRO,IGHydroRes) = 0;
   );

      IF (IVWINDSHEDDING_DAYAHEAD_YES EQ 1,
          VWINDSHEDDING_DAY_AHEAD.UP(T,IR)$(IT_SPOTPERIOD(T)) = INF;
          VWINDSHEDDING_DAY_AHEAD.LO(T,IR)$(IT_SPOTPERIOD(T)) = 0;
        );

$ifi '%Renewable_Spinning%' == Yes     VWINDCUR_ANCPOS.UP(T,IR)$(IT_SPOTPERIOD(T)) = INF;
$ifi '%Renewable_Spinning%' == Yes     VWINDCUR_ANCPOS.LO(T,IR)$(IT_SPOTPERIOD(T)) = 0;

                   IF (IVSOLARSHEDDING_DAYAHEAD_YES EQ 1,
                    VSOLARSHEDDING_DAY_AHEAD.UP(T,IR)$(IT_SPOTPERIOD(T)) = INF;
                    VSOLARSHEDDING_DAY_AHEAD.LO(T,IR)$(IT_SPOTPERIOD(T)) = 0;
   );

*      VXELEC_T.UP(T,IEXPORT_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  INF;
*      VXELEC_T.LO(T,IEXPORT_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  0;

      VXELEC_T.UP(T,IEXPORT_NTC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  INF;
      VXELEC_T.LO(T,IEXPORT_NTC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  0;
$ifi '%LFB%' == Yes         VXELEC_DC_T.UP(T,IEXPORT_FLBMC_DC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  INF;
$ifi '%LFB%' == Yes         VXELEC_DC_T.LO(T,IEXPORT_FLBMC_DC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) =  0;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_T.UP(T,RRR_FLBMC(IRE))$(IT_SPOTPERIOD(T)) =  INF;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_T.LO(T,RRR_FLBMC(IRE))$(IT_SPOTPERIOD(T)) =  -INF;

* ---- fix variables (lower-bound=upper-bound) of variables for up/down regulation, in-/decreased loading of electricity storage or heat pump, up/down regulation of electricity export

       VGELEC_DPOS_T.up(T,IAGK_Y(IA,IGELEC))$(IT_SPOTPERIOD(T))              = 0;
       VGELEC_DNEG_T.up(T,IAGK_Y(IA,IGELEC))$(IT_SPOTPERIOD(T))              = 0;
       VGELEC_DPOS_T.up(T,IAGK_Y(IA,IGHEATPUMP))$(IT_SPOTPERIOD(T))              = 0;
       VGELEC_DNEG_T.up(T,IAGK_Y(IA,IGHEATPUMP))$(IT_SPOTPERIOD(T))              = 0;
       VGELEC_CONSUMED_DPOS_T.up(T,IAGK_Y(IA,IGESTORAGE_DSM))$(IT_SPOTPERIOD(T))     = 0;
       VGELEC_CONSUMED_DNEG_T.up(T,IAGK_Y(IA,IGESTORAGE_DSM))$(IT_SPOTPERIOD(T))     = 0;
       VGELEC_CONSUMED_DPOS_T.up(T,IAHYDRO,IGHYDRORES)$(IT_SPOTPERIOD(T))     = 0;
       VGELEC_CONSUMED_DNEG_T.up(T,IAHYDRO,IGHYDRORES)$(IT_SPOTPERIOD(T))     = 0;

*       VXELEC_DPOS_T.up(T,IEXPORT_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
*       VXELEC_DNEG_T.up(T,IEXPORT_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;

       VXELEC_DPOS_T.up(T,IEXPORT_NTC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
       VXELEC_DNEG_T.up(T,IEXPORT_NTC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB%' == Yes         VXELEC_DC_DPOS_T.up(T,IEXPORT_FLBMC_DC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB%' == Yes         VXELEC_DC_DNEG_T.up(T,IEXPORT_FLBMC_DC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_DPOS_T.up(T,RRR_FLBMC(IRE))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_DNEG_T.up(T,RRR_FLBMC(IRE))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB_BEX%' == Yes     VXELEC_DPOS_T.up(T,IEXPORT_FLBMC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;
$ifi '%LFB_BEX%' == Yes     VXELEC_DNEG_T.up(T,IEXPORT_FLBMC_Y(IRE,IRI))$(IT_SPOTPERIOD(T)) = 0;

* Fix the value of VGE_NONSPIN_ANCPOS for those units not included in IGFASTUNITS(IGDISPATCH).
        VGE_NONSPIN_ANCPOS.UP(T,IAGK_Y(IA,IGDISPATCH))$(NOT IGFASTUNITS(IGDISPATCH) AND (ORD(T) LE (INO_HOURS_OPT_PERIOD+1)))= 0;

);


$ifi '%CHP%' == Yes       LOOP(IAGK_Y(IA,IGFIXHEAT),
$ifi '%CHP%' == Yes              LOOP(T$(ORD(T)>1),

$ifi '%CHP%' == Yes    VGHEAT_T.FX(T,IA,IGFIXHEAT)= IHEATEXT_EXO(IA,IGFIXHEAT,T);
$ifi '%CHP%' == Yes    VGE_ANCPOS.FX(T,IA,IGFIXHEAT)$IGBACKPR(IGFIXHEAT) = 0;
$ifi '%CHP%' == Yes    VGE_SPIN_ANCPOS.FX(T,IA,IGFIXHEAT)$IGBACKPR(IGFIXHEAT) = 0;
$ifi '%CHP%' == Yes    VGE_NONSPIN_ANCPOS.FX(T,IA,IGFIXHEAT)$IGBACKPR(IGFIXHEAT) = 0;
$ifi '%CHP%' == Yes    VGE_ANCNEG.FX(T,IA,IGFIXHEAT)$IGBACKPR(IGFIXHEAT) = 0;
$ifi '%CHP%' == Yes    VGE_SPIN_ANCNEG.FX(T,IA,IGFIXHEAT)$IGBACKPR(IGFIXHEAT) = 0;

$ifi '%CHP%' == Yes       );
$ifi '%CHP%' == Yes       );
*}

VCONTENTSTORAGE_T.LO(IENDTIME,IAGK_Y(IA,IGELECSTODSM)) = 0;

* ---- Outages (Part II): Update associated parameters and upper bounds of related variables) ----------------------------------------------- *{

   IF (INO_SOLVE GT 0,

*--- determine the value of replaced energy IVGELEC_OUTAGE_T needed in QEEQINT
     LOOP ((IR,T) $(ORD(T) > 1),
       IVGELEC_OUTAGE_T(IR,T) = 0;
       LOOP( IA $RRRAAA(IR,IA),
         LOOP (IGELEC $IGELEC_UNP_OUTAGE(IA,IGELEC),

           IF (((ORD(T)-1 )  <= INO_HOURS_WITH_FIXED_VGELEC),
               IVGELEC_OUTAGE_T(IR,T) = IVGELEC_OUTAGE_T(IR,T)
                                        + (IVGELEC_PREVDAYAHEADLOOP(IA,IGELEC,T) - IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGELEC,T)) * IG_UNP_OUTAGE(IA,IGELEC,T);
           );
         );
       );
     );

* Determine the value of those units, where the outage occurs in the actual loop and which were planned to be online in the previous loop
* --- Compute IVGELEC_AND_RESERVES_OUTAGE_T which is used in QNONSP_ANCPOSEQ (zero in first loop)

     LOOP ((IR,T) $( (ORD(T) > 1) AND ((ORD(T)-1)<= NO_HOURS_OVERLAP)) ,
       IVGELEC_AND_RESERVES_OUTAGE_T(IR,T)=0;

       LOOP( IA $RRRAAA(IR,IA),
         LOOP (IGELEC $IGELEC_UNP_OUTAGE(IA,IGELEC),

* IKNOWN_OUTAGE(IA,IGELEC) = 1 means that the outage started in the previous loop and still occurs in the first timestep of the current loop
           IF (IKNOWN_OUTAGE(IA,IGELEC) EQ 0,

* get the values for the unit commitment of the previous loop (shift by number of timesteps of the first stage)
             LOOP (ITALIAS $(ORD(ITALIAS) EQ (ORD(T) + NO_HOURS_OVERLAP)),

* determine maximum value of capacity online in the last loop

               IVGELEC_AND_RESERVES_OUTAGE_T(IR,T) = IVGELEC_AND_RESERVES_OUTAGE_T(IR,T) +
*$ifi  NOT  '%UnitCmin%' == Yes   IVGONLINE_PREVLOOP(IA,IGELEC,ITALIAS)
*$ifi  '%UnitCmin%' == Yes        (IVGELEC_DPOS_PREVLOOP(IA,IGELEC,ITALIAS) - IVGELEC_DNEG_PREVLOOP(IA,IGELEC,ITALIAS) + IVGE_ANCPOS_PREVLOOP(IA,IGELEC,ITALIAS))
*$ifi  '%UnitCmin%' == Yes         +   IVGELEC_PREVLOOP(IA,IGELEC,ITALIAS)
        (IVGELEC_DPOS_PREVLOOP(IA,IGELEC,ITALIAS) - IVGELEC_DNEG_PREVLOOP(IA,IGELEC,ITALIAS) + IVGE_ANCPOS_PREVLOOP(IA,IGELEC,ITALIAS))
         +   IVGELEC_PREVLOOP(IA,IGELEC,ITALIAS)
;

               IVGELEC_AND_RESERVES_OUTAGE_T(IR,T) = max(0, IVGELEC_AND_RESERVES_OUTAGE_T(IR,T) * IG_UNP_OUTAGE(IA,IGELEC,T));
             );
           );
         );
       );
     );


* ---- adjust upper bound of VGELEC_T according to availability (continuous values between 0 and 1 possible, otherwise shorter formulation!)
     LOOP ((IA,IGELEC,T) $(IAGK_Y(IA,IGELEC)
                          $(IGELEC_UNP_OUTAGE(IA,IGELEC)
                           $( (ORD(T)-1 ) <= INO_HOURS_WITH_FIXED_VGELEC))) ,
       if ((VGELEC_T.UP(T,IA,IGELEC) LT INF),
         VGELEC_T.UP(T,IA,IGELEC) = VGELEC_T.UP(T,IA,IGELEC) * (1-IG_UNP_OUTAGE(IA,IGELEC,T))  ;
       else
         if  (IG_UNP_OUTAGE(IA,IGELEC,T) eq 1,
           VGELEC_T.UP(T,IA,IGELEC) = 0 ;
         );
       );
     );


* ---- adjust upper bound of variable VGELEC_CONSUMED_T according to availability (between 0 and one, otherwise much shorter formulation possible!)
     LOOP ((IA,IGESTORAGE_DSM,T) $(IAGK_Y(IA,IGESTORAGE_DSM)
                          $(IGELEC_UNP_OUTAGE(IA,IGESTORAGE_DSM)
                           $( (ORD(T)-1 ) <= INO_HOURS_WITH_FIXED_VGELEC))) ,

       if ((VGELEC_CONSUMED_T.UP(T,IA,IGESTORAGE_DSM) LT INF),
         VGELEC_CONSUMED_T.UP(T,IA,IGESTORAGE_DSM) = VGELEC_CONSUMED_T.UP(T,IA,IGESTORAGE_DSM)* (1-IG_UNP_OUTAGE(IA,IGESTORAGE_DSM,T));

       else
         if  (IG_UNP_OUTAGE(IA,IGESTORAGE_DSM,T) eq 1,
           VGELEC_CONSUMED_T.UP(T,IA,IGESTORAGE_DSM) = 0;
         );
       );
     );


    LOOP ((IAHYDRO,IGHydroRes,T) $(IAGK_Y(IAHYDRO,IGHydroRes)
                                    $(IGELEC_UNP_OUTAGE(IAHYDRO,IGHydroRes)
                                    $( (ORD(T)-1 ) <= INO_HOURS_WITH_FIXED_VGELEC))) ,

    if ((VGELEC_CONSUMED_T.UP(T,IAHYDRO,IGHydroRes) LT INF),
         VGELEC_CONSUMED_T.UP(T,IAHYDRO,IGHydroRes) = vgelec_consumed_t.up(T,IAHYDRO,IGHydroRes)* (1-IG_UNP_OUTAGE(IAHYDRO,IGHydroRes,T));
         VGELEC_CONSUMED_T.LO(T,IAHYDRO,IGHydroRes) = VGELEC_CONSUMED_T.LO(T,IAHYDRO,IGHydroRes)* (1-IG_UNP_OUTAGE(IAHYDRO,IGHydroRes,T));
    else
     if  (IG_UNP_OUTAGE(IAHYDRO,IGHydroRes,T) eq 1,
          VGELEC_CONSUMED_T.UP(T,IAHYDRO,IGHydroRes) = 0;
          VGELEC_CONSUMED_T.LO(T,IAHYDRO,IGHydroRes) = 0;
     else
          VGELEC_CONSUMED_T.UP(T,IAHYDRO,IGHydroRes) = INF;
     );

    );

* enf of    LOOP ((IA,IGHydroRes,T)
   );

* end of IF (INO_SOLVE Gt 0,
  );

*}

* ---- Shadow prices depending on the level of the stochastic demand (ISDP_STORAGE used in objective function) ---- *{

    IF ((IUPDATE_SHADOWPRICE_YES EQ 1) AND (INO_SOLVE >= 1),
      IF ((BASE_WEEKDAY_VAR_INFOTIME(INFOTIME) EQ 4) OR (BASE_WEEKDAY_VAR_INFOTIME(INFOTIME) EQ 5),

        ISDP_ONLINE(IAGK_Y(IA,IGUC))          = 0;
*        ISDP_ONLINE(IAGK_Y(IA,IGUC))          = IHLPSDPONLINE_WEEKEND(IA,IGUC);
        ISDP_STORAGE(IAGK_Y(IA,IGSTORAGE))    = IHLPSDPSTORAGE_WEEKEND(IA,IGSTORAGE);

      ELSE

        ISDP_ONLINE(IAGK_Y(IA,IGUC))          = 0;
*        ISDP_ONLINE(IAGK_Y(IA,IGUC))          = IHLPSDPONLINE_WEEKDAY(IA,IGUC);
        ISDP_STORAGE(IAGK_Y(IA,IGSTORAGE))    = IHLPSDPSTORAGE_WEEKDAY(IA,IGSTORAGE);
        );
    );

*}


$ifi %UnitCmin%==yes $INCLUDE '%code_path_addons%/Mip/MIP_BeforeSolve.inc'

*}
*}

* ---- Solve ---- *{
*---- Options ---- *{
*------------------------------------------------------------------------------
* Include cplex-optfile
* EWL version:
$ontext
    WILMARBASE1.OPTFILE = 1;

* Treat fixed variables as constants (default 0)
    WILMARBASE1.HOLDFIXED=1;

    OPTION LP=CPLEX;
$offtext
*TSO Current version
*------------------------------------------------------------------------------
* MinRuntime == NO  --> Use CPLEX.OPT
* MinRuntime == YES --> Use CPLEX.OP2 and CPLEX.OP3
*                       1) 0 - 600s: CPLEX.OP2 (OPTCR/EPGAP = 0.001) is used
*                       2) > 600s:   ONLY if CPLEX.OP2 is used, the file "readop3" will trigger GAMS to use CPLEX.op3 (OPTCR7EPGAP = 0.05)
* Further information avaiable at: https://support.gams.com/solver:terminate_the_optimization_after_gams_cplex_reaches_a_certain_value_of_the_objective

$ifi '%UnitCmin%'  == Yes  EXECUTE_UNLOAD   "readop2.gdx" INO_SOLVE;

*$ifi not '%UnitCmin%'  == Yes    WILMARBASE1.OPTFILE = 1;
*$ifi     '%UnitCmin%'  == Yes    WILMARBASE1.OPTFILE = 2;
WILMARBASE1.OPTFILE = 1;

  WILMARBASE1.HOLDFIXED=1;
*    OPTION Savepoint=2;
  OPTION LP=Cplex;
*   OPTION LP=GAMSCHK;
  OPTION RESLIM= 10000000;

*-------------------------------------------------------------------------------
*}



*---- Solve ---- *{
$ifi Not '%UnitCmin%'==Yes  SOLVE Wilmarbase1 USING LP MINIMIZING VOBJ;
$ifi     '%UnitCmin%'==Yes  SOLVE WILMARBASE1 USING MIP MINIMIZING VOBJ;
*}
*}

* ---- Postprocessing *{

* ---- Write out results to external files ---- *{

* Text File with Information
    ISTATUSSOLV$(WILMARBASE1.solvestat<>1)=2;
    ISTATUSSOLV$(WILMARBASE1.solvestat=1)=1;
    ISTATUSSOLV$(WILMARBASE1.solvestat=3)=3;

    ISTATUSMOD$(WILMARBASE1.modelstat<>1)=2;
    ISTATUSMOD$(WILMARBASE1.modelstat=1)=1;
    ISTATUSMOD$(WILMARBASE1.modelstat=4)=4;
    ISTATUSMOD$(WILMARBASE1.modelstat=8)=8;


$INCLUDE '%code_path_printinc%print-isolvestatus.inc';
$INCLUDE '%code_path_printinc%print-isolvestatus_12hours.inc';

$INCLUDE '%code_path_printinc%print-isolvestatus_compact.inc';
$INCLUDE '%code_path_printinc%print-isolvestatus_compact_focustime.inc';
$INCLUDE '%code_path_printinc%print-isolvestatus_extended.inc';

    INO_SOLVE=INO_SOLVE + 1;

* Output of results
$INCLUDE '%code_path_printinc%print_results.inc';

* Gdx File
*if (INO_SOLVE < INO_SOLVE_Gdx_Lim,
if (((INO_SOLVE GE INO_write_Gdx_start) AND (INO_SOLVE LE INO_write_Gdx_end) OR mod(INO_SOLVE, INO_write_Gdx_interval) = 0),
   EXECUTE_UNLOAD  '%data_path_out%GDX_files\wilmar.gdx';
   bat.PW = 3000;
   PUT bat, 'move %data_path_out%';
   PUT bat, 'GDX_files\wilmar.gdx ';
   PUT bat, '%data_path_out%';
   PUT bat, 'GDX_files\',INFOTIME.tl:13,'.gdx';
   PUTCLOSE bat
   EXECUTE 'renam.bat';
);

*}

* -----------------------------------------------------------------------------
* Shadow price for hydro reservoirs increases if water level goes below what's normal for the time of the year.
* Factor depends on capacity-to-power-ratio of the hydro reservoir level, to allow some deviation
* for units with small capacity as well as for units with large capacity,
* capacity-to-power-ratio = 500 --> 10% deviation cause a change of +/- 2.25 EUR/MWh (= (10 EUR/MWh + 500/1000 x 25 EUR/MWh) x +/- 0.10) of the shadow price.
* capacity-to-power-ratio =1000 --> 10% deviation cause a change of +/- 3.50 EUR/MWh (= (10 EUR/MWh +1000/1000 x 25 EUR/MWh) x +/- 0.10) of the shadow price.
* capacity-to-power-ratio =2000 --> 10% deviation cause a change of +/- 6.00 EUR/MWh (= (10 EUR/MWh +2000/1000 x 25 EUR/MWh) x +/- 0.10) of the shadow price.
* capacity-to-power-ratio =3000 --> 10% deviation cause a change of +/- 8.50 EUR/MWh (= (10 EUR/MWh +3000/1000 x 25 EUR/MWh) x +/- 0.10) of the shadow price.
* (The maximum allowed deviation is set in equ. QHYRSEndUpper and QHYRSEndLower.)
$ifi '%Looping%' == week    ISDP_HYDRORES(IAHYDRO,IENDTIME_HYDISDP) = MAX(1,ISDP_REFPRICE(IAHydro) + (min(100, 10 + SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))/SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*25/1000))*(IRESLEVELS_T(IAHYDRO,IENDTIME_HYDISDP)/100
$ifi '%Looping%' == week                                                                                                        - VCONTENTHYDRORES_T.l(IENDTIME_HYDISDP,IAHYDRO)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES)) ));

* Updating the reference price (Makes the model more robust against bad initial values and reduces long term deviation of the hydro reservoir level.).
$ifi '%Looping%' == week Loop(T$(IENDTIME_HYDISDP(T)),
$ifi '%Looping%' == week    ISDP_REFPRICE(IAHydro) = ISDP_REFPRICE(IAHydro) + (0.2*(ISDP_HYDRORES(IAHYDRO,T) - ISDP_REFPRICE(IAHydro)));
$ifi '%Looping%' == week    );


* ---- Save shadow prices for storages (electric and heat) and for unit being online  ---- *{
* Compute IHLPDEMANDLEVEL_WEEKEND, IHLPSDPONLINE_WEEKEND, IHLPSDPSTORAGE_WEEKEND
* ICALC_SHADOWPRICE_YES=1 in DA loop

    IF ((ICALC_SHADOWPRICE_YES EQ 1),

* Save shadow price of each node and save demand at that node

      IF ((BASE_WEEKDAY_VAR_INFOTIME(INFOTIME) EQ 5) OR (BASE_WEEKDAY_VAR_INFOTIME(INFOTIME) EQ 6),
         LOOP (T $IENDTIME_SHADOWPRICE(T),
           LOOP (IAGK_Y(IA,IGNOUCSCC),
             IHLPSDPONLINE_WEEKEND(IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes  $ifi Not '%UnitCmin%' == Yes    =  QGONLSTART.M(T,IA,IGNOUCSCC);
$ifi     '%NoStartUpCosts%' == Yes  $ifi Not '%UnitCmin%' == Yes    =  0;
$ifi not '%NoStartUpCosts%' == Yes  $ifi     '%UnitCmin%' == Yes    =  QGSTARTFUEL.M(T,IA,IGNOUCSCC);
$ifi     '%NoStartUpCosts%' == Yes  $ifi     '%UnitCmin%' == Yes    =  0;
           );

$ifi %UnitCscc%==yes           LOOP (IAGK_Y(IA,IGUCSCC),
$ifi %UnitCscc%==yes                IHLPSDPONLINE_WEEKEND(IA,IGUCSCC)
$ifi %UnitCscc%==yes                = SMAX(T_WITH_HIST$(ORD(T_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')), QUC12.M(T,T_WITH_HIST,IA,IGUCSCC));
$ifi %UnitCscc%==yes           );


           LOOP (IAGK_Y(IAELECSTO,IGELECSTORAGE),
*Reason for EWL-version (=0): reference price is set for midnight. In the night prices are lower so that the value of water for these hours is low, too.
*            IHLPSDPSTORAGE_WEEKEND(IAELECSTO,IGELECSTORAGE) = -QESTOVOLT.M(T,IAELECSTO,IGELECSTORAGE);
             IHLPSDPSTORAGE_WEEKEND(IAELECSTO,IGELECSTORAGE) = 0;

           );
           LOOP (IAGK_Y(IA,IGFLEXDEMAND),
*             IHLPSDPSTORAGE_WEEKEND(IA,IGFLEXDEMAND) = -QDSMVOLT.M(T,IA,IGFLEXDEMAND);
             IHLPSDPSTORAGE_WEEKEND(IA,IGFLEXDEMAND) = 0;
           );

           LOOP (IAGK_Y(IA,IGHEATSTORAGE),
             IHLPSDPSTORAGE_WEEKEND(IA,IGHEATSTORAGE) = QHSTOVOLT.M(T,IA,IGHEATSTORAGE);
           );
         );

* Compute IHLPDEMANDLEVEL_WEEKDAY, IHLPSDPONLINE_WEEKDAY, IHLPSDPSTORAGE_WEEKDAY
        ELSE
         LOOP (T $IENDTIME_SHADOWPRICE(T) ,

         LOOP (IAGK_Y(IA,IGNOUCSCC),
             IHLPSDPONLINE_WEEKDAY(IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes   = QGONLSTART.M(T,IA,IGNOUCSCC);
$ifi     '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes   = 0;
$ifi not '%NoStartUpCosts%' == Yes  $ifi     '%UnitCmin%' == Yes    =  QGSTARTFUEL.M(T,IA,IGNOUCSCC);
$ifi     '%NoStartUpCosts%' == Yes  $ifi     '%UnitCmin%' == Yes    =  0;
          );

$ifi %UnitCscc%==yes           LOOP (IAGK_Y(IA,IGUCSCC),
$ifi %UnitCscc%==yes                IHLPSDPONLINE_WEEKDAY(IA,IGUCSCC)
$ifi %UnitCscc%==yes                =
$ifi %UnitCscc%==yes                SMAX(T_WITH_HIST$(ORD(T_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')),QUC12.M(T,T_WITH_HIST,IA,IGUCSCC));
$ifi %UnitCscc%==yes           );

           LOOP( IAGK_Y(IAELECSTO,IGELECSTORAGE),
*              IHLPSDPSTORAGE_WEEKDAY(IAELECSTO,IGELECSTORAGE) = -QESTOVOLT.M(T,IAELECSTO, IGELECSTORAGE);
               IHLPSDPSTORAGE_WEEKDAY(IAELECSTO,IGELECSTORAGE) = 0;
           );

           LOOP( IAGK_Y(IA,IGFLEXDEMAND),
*              IHLPSDPSTORAGE_WEEKDAY(IA,IGFLEXDEMAND) = -QDSMVOLT.M(T,IA,IGFLEXDEMAND);
                                                         IHLPSDPSTORAGE_WEEKDAY(IA,IGFLEXDEMAND) = 0;
           );
           LOOP( IAGK_Y(IA,IGHEATSTORAGE),
             IHLPSDPSTORAGE_WEEKDAY(IA,IGHEATSTORAGE) = QHSTOVOLT.M(T,IA,IGHEATSTORAGE);
           );
         );
       );
    );
*}

* ---- Variable VGONLINE and IGELECCAPEFF: Save the history of vgonline and igeleccapeff, needed for minimum operation times and shut down times *{

IGELECCAPEFF(IA,IGUC,T_WITH_HIST)$(ORD(T_WITH_HIST)<=25) = IGELECCAPEFF(IA,IGUC,T_WITH_HIST + no_hours_overlap);

VGONLINE_T.FX(T_WITH_HIST,IA,IGUC)$(ORD(T_WITH_HIST)<=25) = VGONLINE_T.L(T_WITH_HIST + no_hours_overlap,IA,IGUC);

* VGSTARTUP_T not currently fixed in TSO version
* EWL version             VGSTARTUP_T.FX(T_WITH_HIST,IA,IGUC)$(ORD(T_WITH_HIST)<=25) = VGSTARTUP_T.L(T_WITH_HIST + no_hours_overlap,IA,IGUC);
* EWL alternative version VGSTARTUP_T.FX(T_WITH_HIST,IA,IGUC)$(ORD(T_WITH_HIST)<=24) = VGSTARTUP_T.L(T_WITH_HIST + no_hours_overlap,IA,IGUC);

* ---- Fix and shift the value of vgonline for those units, where IGSTARTUPTIME(AAA,G) > 1. (VGONLINE_T.UP)

* EWL version of fixing VGONLINE
$ontext
   LOOP(IAGK_Y(IA,IGUC_NOALWAYS) $(IGSTARTUPTIME(IA,IGUC_NOALWAYS)>1),
     LOOP (T $( IT_OPT(T) AND (ORD(T) <= IGSTARTUPTIME(IA,IGUC_NOALWAYS)+1)),
        LOOP(ITALIAS $(ORD(ITALIAS) EQ ORD(T)+no_hours_overlap),

* to avoid numerical problems
$ifi Not '%UnitCmin%' == Yes     VGONLINE_T.UP(T,IA,IGUC_NOALWAYS)= MAX(0,VGONLINE_T.L(ITALIAS,IA,IGUC_NOALWAYS));
$ifi '%UnitCmin%' == Yes         VGONLINE_T.UP(T,IA,IGUC_NOALWAYS)= CEIL(VGONLINE_T.L(ITALIAS,IA,IGUC_NOALWAYS));
         );
     );
   );
$offtext
*}

*---- Update the number of hours that the unit has been online/offline
$ifi %UnitCmin%==yes $INCLUDE '%code_path_addons%/Mip/MIP_AfterSolve.inc'

* ---- Variables: Set initial values of certain, not all, variables (copy current value from time step T12 to T00) *{
    LOOP(T$(ORD(T) EQ (1+ NO_HOURS_OVERLAP)),

      VGELEC_T.FX('T00',IAGK_Y(IA,IGELEC))             = VGELEC_T.L(T,IA,IGELEC);
      VGELEC_DPOS_T.FX('T00',IAGK_Y(IA,IGELEC))        = VGELEC_DPOS_T.L(T,IA,IGELEC);
      VGELEC_DNEG_T.FX('T00',IAGK_Y(IA,IGELEC))        = VGELEC_DNEG_T.L(T,IA,IGELEC);
      VGHEAT_T.FX('T00',IAGK_Y(IA,IGHEAT))             = VGHEAT_T.L(T,IA,IGHEAT);
      VCONTENTHYDRORES_T.FX('T00',IAHYDRO)             = VCONTENTHYDRORES_T.L(T,IAHYDRO);
      VCONTENTSTORAGE_T.FX('T00',IAGK_Y(IA,IGSTORAGE)) = VCONTENTSTORAGE_T.L(T,IA,IGSTORAGE);
      VCONTENTSTORAGE_T.LO(IENDTIME,IAGK_Y(IA,IGELECSTODSM)) = -INF;
    );
*}

* ---- Parameters: Update after spot optimisation: 1) fix demand for hours 00:00 -24:00, 2) shift to left *{

* Update values of IDEMANDELEC_BID_IR, IVGELEC_PREVDAYAHEADLOOP, IVGELEC_CONSUMED_PREVDAYAHEAD (for T13,..,T36)

    If ((IBID_DAYAHEADMARKET_YES  EQ 1),

          IDEMANDELEC_BID_IR(IR,T) $IT_SPOTPERIOD(T) = IDEMANDELEC_BID_IR(IR,T) + IFLEXIBLE_DEF_YES *
                  (SUM(DEF_U, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_U)) - SUM(DEF_D, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_D)));

          IVGELEC_PREVDAYAHEADLOOP(IA,IGELEC,T) $IT_SPOTPERIOD(T)             = VGELEC_T.L(T,IA,IGELEC);
          IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGELECSTORAGE,T) $IT_SPOTPERIOD(T) = VGELEC_CONSUMED_T.L(T,IA,IGELECSTORAGE);
          IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGHydroRes,T) $IT_SPOTPERIOD(T)= VGELEC_CONSUMED_T.L(T,IA,IGHydroRes);
    );

*---- Shift parameters: In DA-loop T13*T36 -> T01*T24, in ID-loop T13-T24 -> T01*T12

    LOOP (T $( (ORD(T) > (NO_HOURS_OVERLAP + 1)) AND (ORD(T) <= (INO_HOURS_OPT_PERIOD + 1))),
      LOOP(ITALIAS  $(ORD(ITALIAS) EQ (ORD(T)-NO_HOURS_OVERLAP)),

          IDEMANDELEC_BID_IR(IR,ITALIAS)                    = IDEMANDELEC_BID_IR(IR,T);
          IWIND_BID_IR(IR,ITALIAS)                          = IWIND_BID_IR(IR,T);

          IVGELEC_PREVDAYAHEADLOOP(IA,IGELEC,ITALIAS)             = IVGELEC_PREVDAYAHEADLOOP(IA,IGELEC,T);
          IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGELECSTORAGE,ITALIAS) = IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGELECSTORAGE,T);
          IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGHydroRes,ITALIAS)    = IVGELEC_CONSUMED_PREVDAYAHEAD(IA,IGHydroRes,T);

          ISDP_HYDRORES(IAHYDRO,ITALIAS) = ISDP_HYDRORES(IAHYDRO,T);

      );
    );

* IVGONLINE_PREVLOOP needed for IVGELEC_AND_RESERVES_OUTAGE_T,
* IVGELEC_PREVLOOP, IVGELEC_DPOS_PREVLOOP, IVGELEC_DNEG_PREVLOOP, IVGE_ANCPOS_PREVLOOP only needed for MIP?

    LOOP (T $( ((ORD(T)>1) AND ( ORD(T) <= (INO_HOURS_OPT_PERIOD+1)))),
      IVGELEC_PREVLOOP(IA,IGELEC,T) = VGELEC_T.L(T,IA,IGELEC);

        LOOP (IAGK_Y(IA,IGELEC),
          IVGONLINE_PREVLOOP(IA,IGELEC,T)    = VGONLINE_T.L(T,IA,IGELEC);
$ifi %UnitCmin%==yes          IVGONLINE_PREVLOOP(IA,IGELEC,T)    = ROUND(IVGONLINE_PREVLOOP(IA,IGELEC,T));
          IVGELEC_DPOS_PREVLOOP(IA,IGELEC,T) = VGELEC_DPOS_T.L(T,IA,IGELEC);
          IVGELEC_DNEG_PREVLOOP(IA,IGELEC,T) = VGELEC_DNEG_T.L(T,IA,IGELEC);
          IVGE_ANCPOS_PREVLOOP(IA,IGELEC,T)  = VGE_ANCPOS.L(T,IA,IGELEC);
        );
    );
*}

* ---- Variables *{
*---- Fixing: For the next loop, fix values of certain variables (IVWINDSHEDDING_DAYAHEAD_YES set in Choice-File, currently =1):
*---- shift values by NO_HOURS_OVERLAP time steps, i.e. T13->T01, ... , T36->T24

LOOP (T $( (ORD(T) > (NO_HOURS_OVERLAP + 1)) AND (ORD(T) <= (INO_HOURS_OPT_PERIOD + 1))),
      LOOP(ITALIAS  $(ORD(ITALIAS) EQ (ORD(T)-NO_HOURS_OVERLAP)),

          VGELEC_T.FX(ITALIAS,IAGK_Y(IA,IGELEC))                                    = VGELEC_T.L(T,IA,IGELEC);
          VGELEC_T.FX(ITALIAS,IAGK_Y(IA,IGHEATPUMP))                                = VGELEC_T.L(T,IA,IGHEATPUMP);
          VGELEC_CONSUMED_T.FX(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))                   = VGELEC_CONSUMED_T.L(T,IA,IGESTORAGE_DSM);
          VGELEC_CONSUMED_T.FX(ITALIAS,IAGK_Y(IAHYDRO,IGHydroRes))                        = VGELEC_CONSUMED_T.L(T,IAHYDRO,IGHydroRes);

          VWINDSHEDDING_DAY_AHEAD.FX(ITALIAS,IR)$(IVWINDSHEDDING_DAYAHEAD_YES EQ 1) = VWINDSHEDDING_DAY_AHEAD.L(T,IR);
          VSOLARSHEDDING_DAY_AHEAD.FX(ITALIAS,IR)$(IVSOLARSHEDDING_DAYAHEAD_YES EQ 1) = VSOLARSHEDDING_DAY_AHEAD.L(T,IR);

$ifi '%Renewable_Spinning%' == Yes  VWINDCUR_ANCPOS.FX(ITALIAS,IR)                 = VWINDCUR_ANCPOS.L(T,IR);

          VGE_ANCPOS.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                              = VGE_ANCPOS.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCPOS.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                              = VGE_ANCPOS.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_ANCNEG.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                              = VGE_ANCNEG.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCNEG.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                              = VGE_ANCNEG.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_SPIN_ANCPOS.up(ITALIAS,IAGK_Y(IA,IGDISPATCH))                         = VGE_SPIN_ANCPOS.L(T,IA,IGDISPATCH)+ 10*epsilon;
          VGE_SPIN_ANCPOS.lo(ITALIAS,IAGK_Y(IA,IGDISPATCH))                         = VGE_SPIN_ANCPOS.L(T,IA,IGDISPATCH)- 10*epsilon;
          VGE_NONSPIN_ANCPOS.up(ITALIAS,IAGK_Y(IA,IGFASTUNITS))                     = VGE_NONSPIN_ANCPOS.L(T,IA,IGFASTUNITS)+ 10*epsilon;
          VGE_NONSPIN_ANCPOS.lo(ITALIAS,IAGK_Y(IA,IGFASTUNITS))                     = VGE_NONSPIN_ANCPOS.L(T,IA,IGFASTUNITS)- 10*epsilon;
          VGE_CONSUMED_ANCPOS.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))            = VGE_CONSUMED_ANCPOS.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCPOS.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))            = VGE_CONSUMED_ANCPOS.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_ANCNEG.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))            = VGE_CONSUMED_ANCNEG.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCNEG.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))            = VGE_CONSUMED_ANCNEG.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_NONSP_ANCPOS.fx(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))      = VGE_CONSUMED_NONSP_ANCPOS.L(T,IA,IGESTORAGE_DSM);

          VGE_ANCPOS_PRI.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCPOS_PRI.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCPOS_PRI.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCPOS_PRI.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_ANCPOS_SEC.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCPOS_SEC.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCPOS_SEC.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCPOS_SEC.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_ANCNEG_PRI.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCNEG_PRI.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCNEG_PRI.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCNEG_PRI.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_ANCNEG_SEC.up(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCNEG_SEC.L(T,IA,IGSPINNING)+ 10*epsilon;
          VGE_ANCNEG_SEC.lo(ITALIAS,IAGK_Y(IA,IGSPINNING))                          = VGE_ANCNEG_SEC.L(T,IA,IGSPINNING)- 10*epsilon;
          VGE_SPIN_ANCNEG.up(ITALIAS,IAGK_Y(IA,IGDISPATCH))                         = VGE_SPIN_ANCNEG.L(T,IA,IGDISPATCH)+ 10*epsilon;
          VGE_SPIN_ANCNEG.lo(ITALIAS,IAGK_Y(IA,IGDISPATCH))                         = VGE_SPIN_ANCNEG.L(T,IA,IGDISPATCH)- 10*epsilon;
          VGE_CONSUMED_ANCPOS_PRI.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCPOS_PRI.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCPOS_PRI.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCPOS_PRI.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_ANCPOS_SEC.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCPOS_SEC.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCPOS_SEC.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCPOS_SEC.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_ANCNEG_PRI.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCNEG_PRI.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCNEG_PRI.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCNEG_PRI.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_ANCNEG_SEC.up(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCNEG_SEC.L(T,IA,IGESTORAGE_DSM)+ 10*epsilon;
          VGE_CONSUMED_ANCNEG_SEC.lo(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))             = VGE_CONSUMED_ANCNEG_SEC.L(T,IA,IGESTORAGE_DSM)- 10*epsilon;
          VGE_CONSUMED_NONSP_ANCNEG.fx(ITALIAS,IAGK_Y(IA,IGESTORAGE_DSM))           = VGE_CONSUMED_NONSP_ANCNEG.L(T,IA,IGESTORAGE_DSM);


*          VXELEC_T.UP(ITALIAS,IRE,IRI)                                              = VXELEC_T.L(T,IRE,IRI)+Epsilon;
*          VXELEC_T.LO(ITALIAS,IRE,IRI)                                              = VXELEC_T.L(T,IRE,IRI)-Epsilon;

          VXELEC_T.UP(ITALIAS,IEXPORT_NTC_Y(IRE,IRI))                       = VXELEC_T.L(T,IRE,IRI)+Epsilon;
          VXELEC_T.LO(ITALIAS,IEXPORT_NTC_Y(IRE,IRI))                       = VXELEC_T.L(T,IRE,IRI)-Epsilon;
$ifi '%LFB%' == Yes     VXELEC_DC_T.UP(ITALIAS,IEXPORT_FLBMC_DC_Y(IRE,IRI))         = VXELEC_DC_T.L(T,IRE,IRI)+Epsilon;
$ifi '%LFB%' == Yes     VXELEC_DC_T.LO(ITALIAS,IEXPORT_FLBMC_DC_Y(IRE,IRI))         = VXELEC_DC_T.L(T,IRE,IRI)-Epsilon;
$ifi '%LFB_NE%' == Yes  VXELEC_EXPORT_T.UP(ITALIAS,RRR_FLBMC(IRE))                  = VXELEC_EXPORT_T.L(T,IRE)+Epsilon;
$ifi '%LFB_NE%' == Yes  VXELEC_EXPORT_T.LO(ITALIAS,RRR_FLBMC(IRE))                  = VXELEC_EXPORT_T.L(T,IRE)-Epsilon;

      );
   );

*---- remove fixing: after a day-ahead loop, remove fixed bound on the variables of up and down regulation
  VGELEC_DPOS_T.UP(T,IAGK_Y(IA,IGELEC))$IT_OPT(T)               = INF;
  VGELEC_DNEG_T.UP(T,IAGK_Y(IA,IGELEC))$IT_OPT(T)               = INF;
  VGELEC_CONSUMED_DPOS_T.UP(T,IAGK_Y(IA,IGESTORAGE_DSM))$IT_OPT(T)      = INF;
  VGELEC_CONSUMED_DNEG_T.UP(T,IAGK_Y(IA,IGESTORAGE_DSM))$IT_OPT(T)      = INF;

  VGELEC_CONSUMED_DPOS_T.UP(T,IAGK_Y(IA,IGHYDRORES))$IT_OPT(T)      = INF;
  VGELEC_CONSUMED_DNEG_T.UP(T,IAGK_Y(IA,IGHYDRORES))$IT_OPT(T)      = INF;
  VXELEC_DPOS_T.UP(T,IEXPORT_NTC_Y(IRE,IRI)) $IT_OPT(T) = INF;
  VXELEC_DNEG_T.UP(T,IEXPORT_NTC_Y(IRE,IRI)) $IT_OPT(T) = INF;

$ifi '%LFB%' == Yes         VXELEC_DC_DPOS_T.UP(T,IEXPORT_FLBMC_DC_Y(IRE,IRI)) $IT_OPT(T) = INF;
$ifi '%LFB%' == Yes         VXELEC_DC_DNEG_T.UP(T,IEXPORT_FLBMC_DC_Y(IRE,IRI)) $IT_OPT(T) = INF;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_DPOS_T.UP(T,RRR_FLBMC(IRE)) $IT_OPT(T) = INF;
$ifi '%LFB_NE%' == Yes      VXELEC_EXPORT_DNEG_T.UP(T,RRR_FLBMC(IRE)) $IT_OPT(T) = INF;
$ifi '%LFB_BEX%' == Yes     VXELEC_DPOS_T.up(T,IEXPORT_FLBMC_Y(IRE,IRI))$IT_OPT(T)  = INF;
$ifi '%LFB_BEX%' == Yes     VXELEC_DNEG_T.up(T,IEXPORT_FLBMC_Y(IRE,IRI))$IT_OPT(T)  = INF;

*}
*}
*}

);
* end LOOP INFOTIME
*}

*------------------------------------------------------------------------------
*                    END OF SIMULATION FOR CURRENT YEAR
*------------------------------------------------------------------------------

);
* End of major loop over all years
*}
*}

$LOG Creating "Environment Report"
$SHOW

* ---- End of WILMAR Model ----
