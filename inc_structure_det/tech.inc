* File Tech.inc
*------------------------------------------------------------------------------


* PARAMETER GDATA contains information about the technologies.
* For further description see bottom of the file or declaration in inc_structure/sets.inc

PARAMETER GDATA(AAA,G,GDATASET)  'Technologies characteristics'
/
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDTYPE.inc";

*read in data rendered/ output produced by CHP-Tool

*power loss coefficient for non-FixQ areas (from data base)
$ifi '%Code_version%' == UENB $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCV1.inc";
$ifi '%Code_version%' == EWL $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCV.inc";
*power loss coefficient for FixQ areas (from CHP tool)
$ifi '%Code_version%' == UENB $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCV2.inc";

*power-to-heat ratio for non-FixQ areas (from data base)
*start change
$ifi '%Code_version%' == UENB $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCB1.inc";
$ifi '%Code_version%' == EWL $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCB.inc";
*end change
*power-to-heat ratio for FixQ areas (from CHP tool)
$ifi '%Code_version%' == UENB $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCB2.inc";

$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFUEL.inc";
$INCLUDE "%data_path_in%inc_database\O Par GDATA GDFE_SECTION All except BP.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFE_SECTION BP.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFE_SLOPE.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFUELTAX.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFUELTAXRED.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDLOADEFF.inc";

$ifi %EWL_EMOB% == YES $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDUNLOADEFF.inc";
$ifi %EWL_EMOB% == YES $INCLUDE "%data_path_in%inc_database\O PAR GDATA GDSTOLOSS.inc";

$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDRESLOADEFF.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDOMVCOST.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDSTARTUPCOST.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDSTARTUPFUEL.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDMINLOADFACTOR.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDMINOPERATION.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDMINSHUTDOWN.inc";
*$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDLEADTIMEHOT.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDLEADTIMEWARM.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDLEADTIMECOLD.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDPARTLOAD.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDFULLLOAD.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDRAMPUP.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDSTARTUPRAMP.inc";
$INCLUDE "%data_path_in%inc_database\O Par GDATA GDDESO2.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDUCSCCC.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDHOTWARM.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDWARMCOLD.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDMINONL.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDRAMPDOWN.inc";

$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDMaxSpinResPri.inc";
$INCLUDE "%data_path_in%inc_database\O Par GData GDMaxSpinResSec.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDStartUpFuelConsHot.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCHP_Subsidy1.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCHP_Subsidy2.inc";
$INCLUDE "%data_path_in%inc_database\O PAR GDATA GDCHP_Subsidy3.inc";
$ifi '%FLEX_addon%' == YES                $include "%data_path_in%inc_database\O PAR GDATA DSM_MaxShiftMinus.inc";
$ifi '%FLEX_addon%' == YES                $include "%data_path_in%inc_database\O PAR GDATA DSM_MaxShiftPlus.inc";
$ifi '%FLEX_addon%' == YES                $include "%data_path_in%inc_database\O PAR GDATA DSM_MaxActiveTime.inc";
$ifi '%FLEX_addon%' == YES                $include "%data_path_in%inc_database\O PAR GDATA DSM_MinDownTime.inc";

$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%data_path_in%inc_database\O Par GDATA CCGT_Flag_GT.inc";
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%data_path_in%inc_database\O Par GDATA CCGT_Flag_ST.inc";
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%data_path_in%inc_database\O Par GDATA CCGT_Flag_DB.inc";
/

$ifi '%CHP%'==YES PARAMETER GDATA_CHPTool(AAA_CHP_Exo,GCHP_FixQ,GDATASET) 'Technology characteristics improved by CHP Tool'
$ifi '%CHP%'==YES /
$ifi '%CHP%'==YES $include "inc_database/O PAR GDATA GDFE_SLOPE_CHPTool.inc";
$ifi '%CHP%'==YES $include "inc_database/O PAR GDATA GDFE_SECTION_CHPTool.inc";
$ifi '%CHP%'==YES $ifi '%Code_version%' == EWL $include "inc_database/O PAR GDATA GDCB_CHPTool.inc";
$ifi '%CHP%'==YES $ifi '%Code_version%' == EWL $include "inc_database/O PAR GDATA GDCV_CHPTool.inc";
$ifi '%CHP%'==YES /


*In EWL-Version values of GDFE_Section, GDFE_Slope, GDCB and GDCV are overwritten for FixQ Units in AAA_CHP_Exo areas
*overwriting in gkfx_all.inc file  

Parameter GDFUELTRANSCOST(AAA,G)
/
$INCLUDE "%data_path_in%inc_database\O Par Gdata Fueltrans.inc";
/;

* Set default value for rampup
* Added here and not in default_values since the latter file is read in after additional parameters are assigned
LOOP ( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1)),
  IF (GDATA(AAA,G,'GDSTARTUPRAMP') eq 0,
      GDATA(AAA,G,'GDSTARTUPRAMP') = GDATA(AAA,G,'GDminloadfactor');
  );
);

LOOP ((AAA,G) $(GDATA(AAA,G,'GDTYPE') eq  1),
  IF (GDATA(AAA,G,'GDRAMPUP') eq 0,
      GDATA(AAA,G,'GDRAMPUP')=(1);
  );
);





PARAMETER GUSEVALUE(AAA,G)
/
$ifi %PtG% == YES $INCLUDE "%data_path_in%inc_database\O PAR GUSEVALUE.inc";
/;


*Coefficients of the OLS estimation for the supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas

$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_COEFF_REF_ISDPHYDRORES(AAA,G) 'Coefficients of the OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par COEFF_REF_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;

$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_COEFF_COAL_ISDPHYDRORES(AAA,G) 'Coefficients (for hard coal price) of the OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par COEFF_COAL_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;

$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_COEFF_GAS_ISDPHYDRORES(AAA,G) 'Coefficients (for natural gas price) of the OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par COEFF_GAS_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;

$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_COEFF_CO2_ISDPHYDRORES(AAA,G) 'Coefficients (for CO2 price) of the OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par COEFF_CO2_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;

$ifi '%HydroSupplyCurves%' == YES PARAMETER BASE_COEFF_RESLEVEL_ISDPHYDRORES(AAA,G) 'Coefficients (for filling level deviation) of the OLS estimation for supply function of hydro reservoirs in IAHYDRO_HYDRORES_STEP-Areas'
$ifi '%HydroSupplyCurves%' == YES /
$ifi '%HydroSupplyCurves%' == YES $INCLUDE "inc_database/O Par COEFF_RESLEVEL_ISDPHYDRORES.inc";
$ifi '%HydroSupplyCurves%' == YES /;

* GDTYPE:
* See main code JMM.gms und comment line *  Parameters for generation types (15)

* GDFUEL:
* With respect to GDFUEL see the file fuel.inc

* GDCV:
* GDCV is relevant only for extraction units (GDTYPE 3),
* it should probably be in the range 0 to 0.3.
* The GDCV value is relevant only for extraction units (GDTYPE 3)
* and should not be filled in for other units
* (it is however set to 1 for back pressure units such that the fuel consumption
* for these units can be found using the same formula as for extraction units).

* GDCB
* GDCB is relevant only for backpressure and extraction units (GDTYPE 2 and 3),
* it should probably be in the range 0.1 to 1.

* GDFULLOAD:
* GDFULLLOAD: Electricity efficiency for units which only produce electricity
* (GDTYPE 1) and for extraction units (GDTYPE 3); total efficiency for units
* which only produce heat (GDTYPE 4) and for back pressure units (GDTYPE 2).
* For condensing units and extraction units (GDTYPE 1 and 3) the value should
* probably be in the range 0.4 - 0.6.
* For extraction units (GDTYPE 3) the connection between GDFE and the GDCV value
* shall be checked so the total efficiency does not exceed 100%.
* For back pressure units (GDTYPE 2) the value should probably be
* in the range 0.7 - 0.95
* For heat only units (GDTYPE 4) the value should probably be in
* the range 0.7 - 0.95
* For heatpumps (GDTYPE 5) the value should probably be between 0.9 and 4.
* For hydro (GDTYPE 6, 7, 8), wind (GDTYPE 9), solar (GDTYPE 10): insert 1.00
* For hydro (GDTYPE  8), wind (GDTYPE 9), solar (GDTYPE 10): insert 1.00

* GDMINLOADFACTOR
* Factor for those units which requiere a minimum steam production
* approximatedly this factor is defined , as
* GDMINLOADFACTOR =(minimum power production/maximum power production)

* GDMINOPERATION
* Parameter for the minimum operation times of a unit

* GDMINSHUTDOWN
* Parameter for the minimum shut down times

* GDLEADTIME
* Parameter for the start up time of a unit



