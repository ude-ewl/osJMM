* File basevar.inc
*------------------------------------------------------------------------------

PARAMETER BASE_GKDERATE_VAR_WEEK (AAA,G,WEEKS) 'availability factor due to planned revisions of units by week'
/
$INCLUDE "%data_path_in%inc_database\O Par GKDERATE Week.inc";
/;

*1) availability is transformed from weekly resolution to hourly
PARAMETER BASE_GKDERATE_VAR_PSEUDO_T(AAA,G,BASETIME) 'pseudo-hourly availability factor due to planned revisions of units';
BASE_GKDERATE_VAR_PSEUDO_T(AAA,G,BASETIME)              = SUM(Weeks$BASETIME_WEEK(BASETIME,WEEKS), BASE_GKDERATE_VAR_WEEK(AAA,G,Weeks));

*2) availability already provided with real hourly resolution
PARAMETER BASE_GKDERATE_VAR_REAL_T (AAA,G,BASETIME) 'hourly availability factor due to planned revisions of units (from inc-file)'
/
*German units
$INCLUDE "%data_path_in%inc_database\O Par GKDERATE.inc";
*units outside Germany
$ifi '%data_source%'=='Access' $INCLUDE "%data_path_in%inc_database\O Par GKDERATE2.inc";
/;

*3) merge both availability data: most common application will be one or the other. In case of inconsistencies, the real horuly data prevails
PARAMETER BASE_GKDERATE_VAR_T (AAA,G,BASETIME) 'hourly availability factor due to planned revisions of units';
BASE_GKDERATE_VAR_T (AAA,G,BASETIME) =  BASE_GKDERATE_VAR_PSEUDO_T(AAA,G,BASETIME) + BASE_GKDERATE_VAR_REAL_T(AAA,G,BASETIME);

*Default GKDERATE values added in main code JMM.gms

*5) read in O Par MinVGONLINE
Parameter BASE_MinVGOnline_VAR_T(AAA,G,WEEKS)
/
$INCLUDE "%data_path_in%inc_database\O Par MinVGONLINE.inc";
/;

*6) read in O Par MinGeneration_Hydro
Parameter BASE_MinGeneration_Var_T(AAA,G,WEEKS) 'Minimal Generation Requirement per Week'
/
$include "%data_path_in%inc_database/O Par MinGeneration_Hydro.inc";
/;