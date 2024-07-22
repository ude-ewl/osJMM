
*----------------------------------------------------------------------------------
* Choice file structure
*----------------------------------------------------------------------------------
*#1 Looping, Runtime and gdx settings
*#2 penalties
*#3 Renewable shedding and penalties
*#4 Transmission and loadflow settings
*#5 simplification settings
*#6 special features
*   - renewable spinning
*   - CHP
*   - CCGT moddeling
*   - Manipulation of MinGen parameters for WASTE and BFG (noCHP)
*   - StartRamp Equation
*#7 other settings
*#8 Test routines and automated settings
*    - UnitCmin, UnitCramp, UnitCscc
*    - FlexCorrecEU
*    - LFB
*    - Load flow test routine


*----------------------------------------------------------------------------------
*#1 Looping, Runtime and gdx settings
*----------------------------------------------------------------------------------

* Choice of looping structure / optimization horizon. Allows the following options:
* '$SetGlobal Looping' for 36/24-hour day-ahead / intraday modeling (standard)
* '$SetGlobal Looping week' for 168-hour weekly looping with 12 hour overlap
* '$SetGlobal Looping 3hours' for 3-hour dayahead (1)/intraday(7) modelling
* Possible choices: ''/week/3hours
$SetGlobal Looping week

* The number of infotimes that are skipped
* before solving the model. Should be equal
* to 2+4*N with N being a positive integer.
* default: 1
SCALAR STARTLOOP /1/;

* Number of loops included in one run.
* EWL-default: week-->53; 12h-looping-->730; 3h-looping-->2920
SCALAR LOOPRUNS /53/;

* Number of loops for which GDX - files should be created
* GDX - files are created
* if     loop number >= INO_write_Gdx_start AND <= INO_write_Gdx_end
*     OR loop number = X * INO_write_Gdx_interval
SCALAR INO_write_Gdx_start                'first loop to write gdx'              /1/;
SCALAR INO_write_Gdx_end          'last loop to write gdx'               /4/;
SCALAR INO_write_Gdx_interval     'interval to write gdx'                /53/;

*----------------------------------------------------------------------------------
*#2 penalties
*----------------------------------------------------------------------------------
SCALAR IPENALTYPTDF               'Penalty on violation of PTDF-restriction on day ahead' /300000/;
SCALAR IPENALTY1                  'Penalty on violation of replacement reserve equation' /50000/;
SCALAR IPENALTY_SpinRes           'Penalty on violation of spinning reserve equations'   /2000/;
SCALAR IPENALTY_DUM               'Penalty on IPLUS violation of day-ahead balance equation'  /2000/;
SCALAR IPENALTY_ENS               'Penalty on IMINUS violation of day-ahead balance equation'  /2000/;
SCALAR IPENALTYBOUNDE             'Penalty on violation of BOUNDEMIN or BOUNDEMAX of CHP units' /50000/;
SCALAR IPENALTY                   'Penalty on violation of intra-day balance equation'  /50000/;
SCALAR IPENALTYCAP                'Penalty on violation of capacity restrictions'   /50000/;
SCALAR IPENALTYHEAT               'Penalty on violation of heat equation'   /1000/;
SCALAR IPENALTYVQONLOP            'Penalty on violation of QGONLOP - ONLY in Priontinc for VOBJ_R_T_INCLSLACK' /0/;
SCALAR IPENALTYRAMP               'Penalty on violation of QGRAMP or variants'   /0/;
SCALAR IPENALTYWINDCURT           'Penalty wind curtailment';
SCALAR IPENALTY_Solar 'absolute value of the negative price at which PV panelsturn off';
SCALAR IHydro_DUM                 'Penalty on Spillage of Hydro'                         /60/;
SCALAR IHydro_Fill                'Penalty on Spillage of Hydro'                         /10000/;
SCALAR IPENALTY_CCGT_STEAM        'Penalty on waste of CCGT_HEAT without Bypass or aditional production of STEAM'    /5000/;

* Choice for deviation penalty-based hydro modeling
* Possible choices: YES/NO
* default:NO
$SetGlobal HydroPenalty NO

*----------------------------------------------------------------------------------
*#3 Renewable shedding and penalties
*----------------------------------------------------------------------------------

* IWINDSHEDDING_DAYAHEAD_YES =1  -> It is allowed to bid less than the expected
*                                   wind power production on the day-ahead market.
* IWINDSHEDDING_DAYAHEAD_YES =0  -> The expected wind power production is always
*                                   bid into the day-ahead market.
* Possible choices: 1/0
* default: 1
PARAMETER IVWINDSHEDDING_DAYAHEAD_YES 'Wind shedding on the day-ahead market' /1/;

* Penalties set based on previous settings
If (IVWINDSHEDDING_DAYAHEAD_YES = 1,
 IPENALTYWINDCURT = 50;
Else
 IPENALTYWINDCURT = 500;
)

PARAMETER IVSOLARSHEDDING_DAYAHEAD_YES 'Solar shedding on the day-ahead market' /1/;
* default: 1
If (IVSOLARSHEDDING_DAYAHEAD_YES = 1,
 IPENALTY_SOLAR = 50;
Else
 IPENALTY_SOLAR = 500;
)

* Possible choices: 1/0
* default: 1
PARAMETER IVWINDSHEDDING_Intraday_YES 'Wind shedding on the intraday market'  /1/;

* Possible choices: 1/0
* default: 1
PARAMETER IVSOLARSHEDDING_Intrady_YES 'Solar shedding on the intraday market' /1/;

* SHEDDING-Parameter
* Controls the way generation in solar, wind and VQDAYAHEAD(...,'IPLUS') is
* allowed to be shedded in national runs. Options:
*   'NoConstraint': No additional constraint on the shedding
*   'OnlyImport': Shedding is only allowed when there is no import
*   'ImportAndENTSOE': 'OnlyImport' and shedding is only allowed if shedding
*     takes place in ENTSO run.
* Possible choices: NoConstraint/OnlyImport/ImportAndENTSOE
* default: NoConstraint
$SETGLOBAL SHEDDING_OPT NoConstraint

*----------------------------------------------------------------------------------
*#4 Transmission and loadflow settings
*----------------------------------------------------------------------------------
* Choice of representation of exchanges/load flows
* One (and only one) of the following four options must be chosen:
* Market exchanges without load flow:                            No_Load_Flow       --> YES
* Version 1 flow-based market coupling (based on NetExport)      LFB_NE             --> YES
* Version 2 flow-based market coupling (based on Bilateral)      LFB_BEX            --> YES
* default: No_Load_flow for NTC Or LFB_NE for FBMC

$SetGlobal No_Load_Flow YES
$setGlobal LFB_NE
$setGlobal LFB_BEX

* ITRANSMISSION_INTRADAY__YES =1 -> Transmission capacity can be scheduled on both
*                                   the intraday and the day-ahead market
* ITRANSMISSION_INTRADAY__YES =0 -> All transmission capacity is available for the
*                                   day-ahead market and none for the intra-day market.
*                                   After the day-ahead market has been cleared the
*                                   transmission between regions can not be changed.
* This option has to be set to 1, if load flow option lfb_intraday is chosen
* Possible choices: 1/0
* default: 1
PARAMETER ITRANSMISSION_INTRADAY_YES 'Transmission scheduled on both the intraday and day-ahead market' /1/;


* Production capacity is reserved in the model for providing
* positive secondary (minute) reserve.
* ITRANSMISSION_NONSP__YES =1    -> It is possible to exchange positive
*                                   secondary reserve between regions.
* ITRANSMISSION_NONSP__YES =0    -> The positive secondary reserve has
*                                   to be provided within each region.
* Possible choices: 1/0
* default: 0
PARAMETER ITRANSMISSION_NONSP_YES 'Transmission of non-spinning secondary reserve' /0/;

*----------------------------------------------------------------------------------
*#5 simplification settings
*----------------------------------------------------------------------------------
* Choices for speed-up model through simplification

* Choice for neglection minimum operation time and minimum down-time (if yes, equations are commented out)
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal NoOpTimeRestrictions

* Choice for neglection of startup costs (if yes, equations are commented out)
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal NoStartUpCosts

* Choice for neglection of reserve equations (if yes, equations are commented out)
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal NoReserveRestrictions NO

*----------------------------------------------------------------------------------
*#6 special features
*----------------------------------------------------------------------------------

*---------- Electric vehicles modeling
* Choice for EV modeling: if YES input needed (e.g. SOC_leave, SOC_arrive)
* Possible choices: YES/''(NO)
* default: depends
$SetGlobal EWL_EMOB  YES
$SetGlobal EMOB_V2G  NO



*---------- Power-to-Gas
* Choice for PtG modeling: if YES the inc-file O PAR GUSEVALUE.inc must exist
* Possible choices: YES/''(NO)
* default: depends
$SetGlobal PtG YES


*---------- Renewable spinning
* This option allowes to use Windenergy for positive spinning
* Possible choices: YES/''(NO)
* default: YES
$SetGlobal Renewable_Spinning YES

*----------- CHP
* Choice for CHP modeling: if YES the model uses input from CHP tool
* Possible choices: YES/''(NO)
* default: depends
$SetGlobal CHP YES

*----------- CCGT moddeling
* Choice for improved CCGT modelling
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal CCGT_Imp

* Choice for CCGT_Adjustment in dependance of data-structure
* Possible choices: YES/''(NO)
* default: ''
$SETGLOBAL CCGT_Adjustments

*----------- WindForecast
* Choice for Windforecast-Update usualy used in combination with 3 hour looping
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal UseWindForecast ''

*----------- MinGen_Manipulation
* Choice for Manipulation of MinGen parameters for WASTE and BFG (noCHP)
* reflect production restriction due to obligation of using input fuel (WASTE or BFG)
* also manipulation of MaxPower to avoid over production
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal MinGen_Manipulation YES



*----------- StartRamp Equation
* Choice for usage of StartRamp equation
* Possible choices: YES/''(NO)
* default: ''
$SetGlobal StartRamp YES


*----------- Hydro Supply Curves
* Choice for usage of Hydro Supply Curves
* Possible choices: YES/''(NO)
* default: ''
* if YES input is needed (e.g. BASE_COEFF_REF_ISDPHYDRORES)
$SetGlobal HydroSupplyCurves NO



*----------------------------------------------------------------------------------
*#7 other settings
*----------------------------------------------------------------------------------

*--------------ISDP_HYDRORES
* Weight used to compute ISDP_HYDRORES
* Possible choices: 0 <= WEIGHT_ISDP_HYDRORES <= 1
* default: 1
PARAMETER WEIGHT_ISDP_HYDRORES 'Weight used to compute ISDP_HYDRORES' /1/;


*-------------- FLEX_addon
*PNA DSM flexibility-features
*EWL: unused feature
$SetGlobal FLEX_addon


*--------------- flexible demand
* IHLP_FLEXIBLE_DEF_YES =1 -> Price flexible demand on the day-ahead market.
* IHLP_FLEXIBLE_DEF_YES =0 -> Inelastic demand on the day-ahead market.
* Possible choices: 1/0
* default: 0
PARAMETER IHLP_FLEXIBLE_DEF_YES 'Flexible electricity demand' /0/;

*---------------data source
* Option data_source
* data_source: EMMDa or Access
* Possible choices: EMMDa/Access
* default: Access
$SETGLOBAL data_source Access

*----------------------------------------------------------------------------------
*#8 Test routines and automated settings
*----------------------------------------------------------------------------------

*----------------UnitCmin, UnitCramp, UnitCscc
* Choice of using CMIN MIP version of JMM (minimum stable generation limit, minimum up and down times)
* Possible choices: YES/NO
* default: depends on %DeMIP%
$ifi %DeMIP%==YES $SetGlobal UnitCmin YES
$ifi NOT %DeMIP%==YES $SetGlobal UnitCmin NO

* Choice of using CRAMP MIP version of JMM (start-up and shut-down ramp rates) in addition to CMIN
* Possible choices: YES/'' (NO)
* default: ''
$SetGlobal UnitCramp

* Choice of using CSCC MIP version of JMM (outtime dependant start-up costs) in addition to CMIN
* Possible choices: YES/NO
* default: NO
$SetGlobal UnitCscc

$ifi '%Code_version%' == UENB $ifi %UnitCmin%==YES $SetGlobal UnitCramp YES
$ifi '%Code_version%' == EWL $ifi %UnitCmin%==YES $SetGlobal UnitCramp NO
$ifi %UnitCmin%==YES $SetGlobal UnitCscc NO

*---------------FlexCorrecEU
*Option to have a reserve margin for reserve for national model runs
* Flexibility correction for two stage runs
* with first stage (EU) = LP and second stage (DE, FR,..) = MIP
*(compensates overestimation in power plant flexibility in first stage LP-runs and thus helps to meet flexibility demand in second stage MIP-runs)
* Possible choices: YES/NO
* default: NO
$ifi '%UnitCmin%' == YES $SetGlobal FlexCorrecEU NO
$ifi '%Code_version%' == UENB $ifi '%UnitCmin%' == NO $SetGlobal FlexCorrecEU YES
$ifi '%Code_version%' == EWL $ifi '%UnitCmin%' == NO $SetGlobal FlexCorrecEU NO

*----------------LFB
* The LFB flag %LFB% is set to "YES" in case a flow based option is active.
$ifi '%No_Load_Flow%' == Yes  $setGlobal LFB NO
$ifi '%LFB_NE%'      == Yes  $setGlobal LFB YES
$ifi '%LFB_BEX%'     == Yes  $setGlobal LFB YES

*----------------Load flow test routine
* Test routines for Load Flow option check (whether exactly one of the three options is active)
*$INCLUDE '%code_path_addons%/LOAD_FLOW/Load_Flow_Option_Check.inc';
Parameter testnwr;
testnwr = 0;
$ifi '%No_Load_Flow%'     == Yes           testnwr = testnwr + 1;
$ifi '%LFB_NE%'           == Yes           testnwr = testnwr + 1;
$ifi '%LFB_BEX%'          == Yes           testnwr = testnwr + 1;
if (testnwr = 1,
    display "Network representation check passed";
 else
 abort "Only one option for network representation allowed";
);