* File print_file_definition.inc.

* This file is part of the WIlmar model

* -----------------------------------------------------------------------------
* Contents:

* This file contains definitions of logical file names
* and the associated names of files

parameter HELPVAR6(RRR,TTT);
parameter HELPVAR1;


PARAMETER ADJUSTEDCOST_R_DA(RRR,TTT);
PARAMETER ADJUSTEDCOST_R_DA_ID(RRR,TTT);
PARAMETER ADJUSTEDCOST_R_ID(RRR,TTT);
PARAMETER ADJUSTEDCOST_R_Y(RRR);
PARAMETER CONGESTIONRENT_DA(RRR,TTT);
PARAMETER Congestionrent_PB_DA(RRR,RRR,TTT);
PARAMETER CONGESTIONRENT_DA_ID(RRR,TTT);
PARAMETER CONGESTIONRENT_ID(RRR,TTT);
PARAMETER CONGESTIONRENT_Y(RRR);
PARAMETER CONSUMERRENT_DA(RRR,TTT);
PARAMETER CONSUMERRENT_ID(RRR,TTT);
PARAMETER CONSUMERRENT_Y(RRR);
PARAMETER Producerrent_DA(RRR,TTT);
PARAMETER Producerrent_DA_ID(RRR,TTT);
PARAMETER Producerrent_ID(RRR,TTT);
PARAMETER Producerrent_Y(RRR);
PARAMETER VOBJ_ALL_inclSlack_DA(RRR,TTT);
PARAMETER VOBJ_ALL_inclSlack_ID(RRR,TTT);
PARAMETER VOBJ_ALL_inclSlack_Y(RRR);
PARAMETER VOBJ_ALL_PRODCOST_DA(RRR,TTT);
PARAMETER VOBJ_ALL_PRODCOST_ID(RRR,TTT);
PARAMETER VOBJ_ALL_PRODCOST_Y(RRR);


PARAMETER OPMARG_GDISPATCH(AAA,G,TTT);


PARAMETER IVXELEC_TMP(TTT,RRR,RRR);

$ifi '%LFB%' == Yes PARAMETER IPHYSICALFLOW_TMP(FLG,TTT);

Parameter RES_SUM(RRR,T);
Parameter GENERSUM(RRR,T);
Parameter IVXELEC_Phys_T(IRE,IRI,T);
Parameter VQDAYAHEAD_T2(T,RRR);

SCALAR OCASEID "WILMAR 1.10" ;
SCALAR EndTime "End of Gams run"
       hour "end hour of Gams run"
       minute "end minute of Gams run"
       second "end second of Gams run";

* Define scalars containing respectively the number of errors in the model run
* and the number of warnings

SCALAR ERRORCOUNT /0/;
SCALAR WARNINGCOUNT /0/;

* Define sets used to write some of the output using IDs in stead of names

Set CaseName     'Name of run case'
/
$INCLUDE "%data_path_in%\inc_database\O Set CaseName.inc";
/;

Set CaseId       'ID of run case'
/
$INCLUDE "%data_path_in%\inc_database\O Set CaseId.inc";
/;

*Set WindCaseName     'Name of wind case used in run case'
*/
*$INCLUDE "%data_path_in%\inc_database\O Set WindCaseName.inc";
*/;

*Set WindCaseId       'ID of wind case used in run case'
*/
*$INCLUDE "%data_path_in%\inc_database\O Set WindCaseId.inc";
*/;

Set GId     'ID of generation technologies'
/
$INCLUDE "%data_path_in%\inc_database\O Set GId.inc";
/;

Set G_GId(G,GId)     'Connection between G and GId of generation technologies'
/
$INCLUDE "%data_path_in%\inc_database\O Set G_GId.inc";
/;

Set AId     'ID of Areas'
/
$INCLUDE "%data_path_in%\inc_database\O Set AId.inc";
/;

Set A_AId(AAA,AId)     'Connection between Areas and ID of Areas'
/
$INCLUDE "%data_path_in%\inc_database\O Set A_AId.inc";
/;

Set RId     'ID of Regions'
/
$INCLUDE "%data_path_in%\inc_database\O Set RId.inc";
/;

Set R_RId(RRR,RId)     'Connection between Regions and ID of Regions'
/
$INCLUDE "%data_path_in%\inc_database\O Set R_RId.inc";
/;

Set TSO_RRRId     'ID of Regions'
/
$INCLUDE "%data_path_in%\inc_database\O Set ResRId.inc";
/;

Set TSO_RRR_TSO_RRRId(TSO_RRR,TSO_RRRId)     'Connection between Regions and ID of Regions'
/
$INCLUDE "%data_path_in%\inc_database\O Set ResR_ResRId.inc";
/;



* Define parameter containing production and transmission costs

Parameter vobj_all_out(RRR,TTT);
Parameter vobj_CO2Cost_out(RRR,TTT);
Parameter vobj_CO2Cost_Start_out(RRR,TTT);
Parameter vobj_FuelCost_out(RRR,TTT);
$ifi not '%NoStartUpCosts%' == Yes Parameter vobj_StartCost_out(RRR,TTT);
Parameter vobj_OMVCost_out(RRR,TTT);
Parameter vobj_Trans_out(RRR,TTT);
Parameter vobj_Revenue_out(RRR,TTT);
Parameter vobj_TaxSub_out(RRR,TTT);
Parameter vobj_FlexDemand_out(RRR,TTT);
Parameter VOBJ_Penalties_Out(RRR,TTT);
Parameter VOBJtmp;

* -----------------------------------------------------------------------
FILE  OUT_TechnologyData_OUT           /"%data_path_out%OUT_TechnologyData.csv"            /;
FILE  OUT_VCONTENTHYDRORES_NT_OUT      /"%data_path_out%OUT_VCONTENTHYDRORES.csv"          /;
FILE  OUT_VHYDROSPILLAGE_OUT           /"%data_path_out%OUT_VHYDROSPILLAGE.csv"            /;
FILE  OUT_QHEQ_M_OUT                   /"%data_path_out%OUT_QHEQ_M.csv"                    /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCNEGEQ_M_OUT              /"%data_path_out%OUT_QANCNEGEQ_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCPOSEQ_M_OUT              /"%data_path_out%OUT_QANCPOSEQ_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCPOSEQ_PRI_M_OUT          /"%data_path_out%OUT_QANCPOSEQ_PRI_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCPOSEQ_SEC_M_OUT          /"%data_path_out%OUT_QANCPOSEQ_SEC_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCNEGEQ_PRI_M_OUT          /"%data_path_out%OUT_QANCNEGEQ_PRI_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QANCNEGEQ_SEC_M_OUT          /"%data_path_out%OUT_QANCNEGEQ_SEC_M.csv"               /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QNONSP_ANCNEGEQ_M_OUT        /"%data_path_out%OUT_QNONSP_ANCNEGEQ_M.csv"         /;
FILE  OUT_IWIND_REALISED_OUT           /"%data_path_out%OUT_IWIND_REALISED.csv"            /;
FILE  OUT_IWIND_REALISED2_OUT          /"%data_path_out%OUT_IWIND_REALISED2.csv"           /;
FILE  OUT_IWIND_REALISED3_OUT          /"%data_path_out%OUT_IWIND_REALISED3.csv"           /;
FILE  OUT_HELPVAR6_OUT                 /"%data_path_out%OUT_HELPVAR6.csv"                  /;
FILE  OUT_IDEMANDELEC_REALISED_OUT     /"%data_path_out%OUT_IDEMANDELEC_REALISED.csv"      /;
FILE  OUT_IDE_REALISED_RESIDUAL_OUT    /"%data_path_out%OUT_IDE_REALISED_RESIDUAL.csv"     /;
FILE  OUT_QEEQINT_M_OUT                /"%data_path_out%OUT_QEEQINT_M.csv"                 /;
$ifi '%Looping%' == 3hours FILE  OUT_QEEQINT_M_3h_OUT                /"%data_path_out%OUT_QEEQINT_M_3h.csv"                 /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_QNONSP_ANCPOSEQ_M_OUT        /"%data_path_out%OUT_QNONSP_ANCPOSEQ_M.csv"         /;
FILE  OUT_QESTOVOLT_M_OUT              /"%data_path_out%OUT_QESTOVOLT_M.csv"               /;
FILE  OUT_QHSTOVOLT_M_OUT              /"%data_path_out%OUT_QHSTOVOLT_M.csv"               /;
FILE  OUT_QHYRSSEQ_M_OUT               /"%data_path_out%OUT_QHYRSSEQ_M.csv"                /;
$ifi not '%NoStartUpCosts%' == Yes FILE  OUT_QGONLSTART_M_OUT             /"%data_path_out%OUT_QGONLSTART_M.csv"              /;
FILE  OUT_VDEMANDELECFLEXIBLE_OUT      /"%data_path_out%OUT_VDEMANDELECFLEXIBLE.csv"       /;
FILE  OUT_QEEQDAY_M_OUT                /"%data_path_out%OUT_QEEQDAY_M.csv"                 /;
FILE  OUT_QXCAPFBMCPlus_M_OUT          /"%data_path_out%OUT_QXCAPFBMCPlus_M.csv"           /;
FILE  OUT_QXCAPFBMCMinus_M_OUT         /"%data_path_out%OUT_QXCAPFBMCMinus_M.csv"          /;
FILE  OUT_VWINDSHEDDING_DAY_AHEAD_OUT  /"%data_path_out%OUT_VWINDSHEDDING_DAY_AHEAD.csv"   /;
FILE  OUT_IWIND_BID_OUT                /"%data_path_out%OUT_IWIND_BID.csv"                 /;
FILE  OUT_IWIND_BID2_OUT               /"%data_path_out%OUT_IWIND_BID2.csv"                /;
FILE  OUT_IDEMANDELEC_BID_OUT          /"%data_path_out%OUT_IDEMANDELEC_BID.csv"           /;
FILE  OUT_IDE_BID_RESIDUAL_OUT         /"%data_path_out%OUT_IDE_BID_RESIDUAL.csv"          /;
FILE  OUT_HEAT_CONS_T_OUT              /"%data_path_out%OUT_HEAT_CONS_T.csv"               /;
FILE  OUT_CaseName_OUT                 /"%data_path_out%OUT_CaseNames.csv"                 /;
FILE  OUT_CO2Price_OUT                 /"%data_path_out%OUT_CO2Price.csv"                  /;
FILE  OUT_FuelPrice_OUT                /"%data_path_out%OUT_FuelPrice.csv"                 /;
FILE  OUT_VOBJ_OUT                     /"%data_path_out%OUT_VOBJ.csv"                      /;
FILE  OUT_VOBJ_withoutSlacks_OUT       /"%data_path_out%OUT_VOBJ_withoutSlacks.csv"                      /;

$ifi '%HydroPenalty%' == Yes FILE  OUT_VQRESLEVELUP_DA_OUT           /"%data_path_out%OUT_VQRESLEVELUP_DA.csv"           /;
$ifi '%HydroPenalty%' == Yes FILE  OUT_VQRESLEVELDOWN_DA_OUT         /"%data_path_out%OUT_VQRESLEVELDOWN_DA.csv"           /;
$ifi '%HydroPenalty%' == Yes FILE  OUT_VQRESLEVELUP_ID_OUT           /"%data_path_out%OUT_VQRESLEVELUP_ID.csv"           /;
$ifi '%HydroPenalty%' == Yes FILE  OUT_VQRESLEVELDOWN_ID_OUT         /"%data_path_out%OUT_VQRESLEVELDOWN_ID.csv"           /;
$ifi '%HydroPenalty%' == Yes FILE  OUT_ISDP_HYDRORES_ENDTIME_DA_OUT  /"%data_path_out%OUT_ISDP_HYDRORES_ENDTIME_DA.csv"             /;
$ifi '%HydroPenalty%' == Yes FILE  OUT_ISDP_HYDRORES_ENDTIME_ID_OUT  /"%data_path_out%OUT_ISDP_HYDRORES_ENDTIME_ID.csv"             /;

FILE  OUT_PTDFCAP_SLACK_OUT            /"%data_path_out%OUT_PTDFCAP_SLACK.csv"             /;
FILE  OUT_VOBJ_R_T_ALL_OUT             /"%data_path_out%OUT_VOBJ_R_T_All.csv"              /;
FILE  OUT_VOBJ_R_T_CO2Cost_OUT         /"%data_path_out%OUT_VOBJ_R_T_CO2Cost.csv"          /;
FILE  OUT_VOBJ_R_T_CO2Cost_Start_OUT   /"%data_path_out%OUT_VOBJ_R_T_CO2Cost_Start.csv"    /;
FILE  OUT_VOBJ_R_T_Fuelcost_OUT        /"%data_path_out%OUT_VOBJ_R_T_Fuelcost.csv"         /;
$ifi not '%NoStartUpCosts%' == Yes FILE  OUT_VOBJ_R_T_StartCost_OUT       /"%data_path_out%OUT_VOBJ_R_T_StartCost.csv"        /;
FILE  OUT_VOBJ_R_T_OMVCost_OUT         /"%data_path_out%OUT_VOBJ_R_T_OMVCost.csv"          /;
FILE  OUT_VOBJ_R_T_Trans_OUT           /"%data_path_out%OUT_VOBJ_R_T_Trans.csv"            /;
FILE  OUT_VOBJ_R_T_Revenue_OUT         /"%data_path_out%OUT_VOBJ_R_T_Revenue.csv"          /;
FILE  OUT_VOBJ_R_T_TaxSub_OUT          /"%data_path_out%OUT_VOBJ_R_T_TaxSub.csv"           /;
FILE  OUT_VOBJ_R_T_FlexDemand_OUT      /"%data_path_out%OUT_VOBJ_R_T_FlexDemand.csv"       /;
FILE  OUT_VOBJ_R_T_Penalties_OUT       /"%data_path_out%OUT_VOBJ_R_T_Penalties_OUT.csv"    /;
FILE  OUT_ISDP_HYDRORES_OUT            /"%data_path_out%OUT_ISDP_HYDRORES.csv"             /;
FILE  OUT_ISDP_HYDRORES_STEP_OUT       /"%data_path_out%OUT_ISDP_HYDRORES_STEP.csv"        /;
FILE  OUT_IX3COUNTRY_T_Y_OUT           /"%data_path_out%OUT_IX3COUNTRY_T_Y.csv"            /;
FILE  ISOLVESTATUS_OUT                 /"%data_path_out%ISOLVESTATUS.txt"                  /;
FILE  SOLVESTATUS_OUT                  /"%data_path_out%SOLVESTATUS.txt"                   /;

FILE  ISOLVESTATUS_12hours_OUT         /"%data_path_out%ISOLVESTATUS_12hours.txt"          /;
FILE  OUT_IGELECCAPEFF_OUT             /"%data_path_out%OUT_IGELECCAPEFF.csv"              /;

FILE  OUT_VXELEC_T_OUT                 /"%data_path_out%OUT_VXELEC.csv"                    /;
FILE  OUT_VXELEC_T_Realised_OUT        /"%data_path_out%OUT_VXELEC_Realised.csv"           /;
FILE  OUT_VXELEC_T_Realised_woSaldo_OUT /"%data_path_out%OUT_VXELEC_Realised_woSaldo.csv"   /;
FILE  OUT_VXELEC_T_Realised_EnSAVes_OUT        /"%data_path_out%OUT_VXELEC_Realised_EnSAVes.csv"           /;
FILE  OUT_VXELEC_DPOS_T_OUT            /"%data_path_out%OUT_VXELEC_DPOS.csv"               /;
FILE  OUT_VXELEC_DNEG_T_OUT            /"%data_path_out%OUT_VXELEC_DNEG.csv"               /;
$ifi '%Looping%' == 3hours FILE  OUT_VXELEC_DPOS_T_3h_OUT                  /"%data_path_out%OUT_VXELEC_DPOS_3h.csv"               /;
$ifi '%Looping%' == 3hours FILE  OUT_VXELEC_DNEG_T_3h_OUT                  /"%data_path_out%OUT_VXELEC_DNEG_3h.csv"               /;


$ifi '%LFB%' == Yes FILE  OUT_VXELEC_DC_T_OUT        /"%data_path_out%OUT_VXELEC_DC.csv"           /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_T_OUT        /"%data_path_out%OUT_VXELEC_ExportFB.csv"           /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_DC_DPOS_T_OUT            /"%data_path_out%OUT_VXELEC_DC_DPOS.csv"               /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_DC_DNEG_T_OUT            /"%data_path_out%OUT_VXELEC_DC_DNEG.csv"               /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_DPOS_T_OUT            /"%data_path_out%OUT_VXELEC_ExportFB_DPOS.csv"               /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_DNEG_T_OUT            /"%data_path_out%OUT_VXELEC_ExportFB_DNEG.csv"               /;
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes FILE  OUT_VXELEC_DC_DPOS_T_3h_OUT            /"%data_path_out%OUT_VXELEC_DC_DPOS.csv"      /;
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes FILE  OUT_VXELEC_DC_DNEG_T_3h_OUT            /"%data_path_out%OUT_VXELEC_DC_DNEG.csv"   /;
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_DPOS_T_3h_OUT            /"%data_path_out%OUT_VXELEC_ExportFB_DPOS.csv"    /;
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_DNEG_T_3h_OUT            /"%data_path_out%OUT_VXELEC_ExportFB_DNEG.csv"  /;

$ifi '%LFB%' == Yes FILE  OUT_VXELEC_T_Realised_PhysicalFlows_OUT        /"%data_path_out%OUT_VXELEC_Realised_PhysicalFlows.csv"           /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_T_Realised_onlyNTCDC_OUT        /"%data_path_out%OUT_VXELEC_onlyNTCDC_Realised.csv"           /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_T_Realised_OUT        /"%data_path_out%OUT_VXELEC_ExportFB_Realised.csv"           /;
$ifi '%LFB%' == Yes FILE  OUT_VXELEC_ExportFB_T_Realised_ct_OUT        /"%data_path_out%OUT_VXELEC_ExportFB_Realised_ct.csv"           /;

FILE  OUT_IDEMAND_REPLACE_RES_OUT      /"%data_path_out%OUT_IDEMAND_REPLACE_RES.csv"       /;
FILE  OUT_CAPLOST_OUTAGE_OUT           /"%data_path_out%OUT_CAPLOST_OUTAGE.csv"            /;
FILE  VELEC_CONTENTSTORAGE_OUT         /"%data_path_out%OUT_VGELEC_CONTENTSTORAGE.csv"     /;
FILE  VELEC_DSMCONTENTSTORAGE_OUT         /"%data_path_out%OUT_VGELEC_DSMCONTENTSTORAGE.csv"     /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCNEG_OUT                   /"%data_path_out%OUT_VGE_ANCNEG.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCPOS_OUT                   /"%data_path_out%OUT_VGE_ANCPOS.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCPOS_PRI_OUT               /"%data_path_out%OUT_VGE_ANCPOS_PRI.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCPOS_SEC_OUT               /"%data_path_out%OUT_VGE_ANCPOS_SEC.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCNEG_PRI_OUT               /"%data_path_out%OUT_VGE_ANCNEG_PRI.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_ANCNEG_SEC_OUT               /"%data_path_out%OUT_VGE_ANCNEG_SEC.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCPOS_PRI_OUT      /"%data_path_out%OUT_VGE_CONSUMED_PRI_ANCPOS.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCPOS_SEC_OUT      /"%data_path_out%OUT_VGE_CONSUMED_SEC_ANCPOS.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCNEG_PRI_OUT      /"%data_path_out%OUT_VGE_CONSUMED_PRI_ANCNEG.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCNEG_SEC_OUT      /"%data_path_out%OUT_VGE_CONSUMED_SEC_ANCNEG.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_SPIN_ANCNEG_OUT              /"%data_path_out%OUT_VGE_SPIN_ANCNEG.csv"              /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_NONSP_ANCNEG_OUT    /"%data_path_out%OUT_VGE_CONSUMED_NONSP_ANCNEG.csv" /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCNEG_OUT          /"%data_path_out%OUT_VGE_CONSUMED_ANCNEG.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_ANCPOS_OUT          /"%data_path_out%OUT_VGE_CONSUMED_ANCPOS.csv"       /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_CONSUMED_NONSP_ANCPOS_OUT    /"%data_path_out%OUT_VGE_CONSUMED_NONSP_ANCPOS.csv" /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_NONSPIN_ANCPOS_OUT           /"%data_path_out%OUT_VGE_NONSP_ANCPOS.csv"          /;
FILE  VGELEC_CONSUMED_OUT              /"%data_path_out%OUT_VGELEC_CONSUMED.csv"           /;
FILE  VGELEC_CONSUMED_Realised_OUT     /"%data_path_out%OUT_VGELEC_CONSUMED_Realised.csv"  /;
FILE  VGELEC_CONSUMED_DPOS_OUT         /"%data_path_out%OUT_VGELEC_CONSUMED_DPOS.csv"      /;
FILE  VGELEC_CONSUMED_DNEG_OUT         /"%data_path_out%OUT_VGELEC_CONSUMED_DNEG.csv"      /;
FILE  VGELEC_OUT                       /"%data_path_out%OUT_VGELEC.csv"                    /;
FILE  VGELEC_OUT_testror                       /"%data_path_out%OUT_VGELEC_testror.csv"                    /;
FILE  VGELEC_OUT_Realised              /"%data_path_out%OUT_VGELEC_Realised.csv"           /;
FILE  VGELEC_DPOS_OUT                  /"%data_path_out%OUT_VGELEC_DPOS.csv"               /;
$ifi '%Looping%' == 3hours FILE  VGELEC_DPOS_3h_OUT                  /"%data_path_out%OUT_VGELEC_DPOS_3h.csv"               /;
$ifi '%Looping%' == 3hours FILE  VGELEC_DNEG_3h_OUT                  /"%data_path_out%OUT_VGELEC_DNEG_3h.csv"               /;
$ifi '%Looping%' == 3hours FILE  VGELEC_CONSUMED_DPOS_3h_OUT         /"%data_path_out%OUT_VGELEC_CONSUMED_DPOS_3h.csv"      /;
$ifi '%Looping%' == 3hours FILE  VGELEC_CONSUMED_DNEG_3h_OUT         /"%data_path_out%OUT_VGELEC_CONSUMED_DNEG_3h.csv"      /;
FILE  VGELEC_DNEG_OUT                  /"%data_path_out%OUT_VGELEC_DNEG.csv"               /;
FILE  VGFUELUSAGE_OUT                  /"%data_path_out%OUT_VGFUELUSAGE.csv"               /;
*FILE  VGE_ANCPOS_5_OUT                 /"%data_path_out%OUT_VGE_ANCPOS_5.csv"              /;
$ifi not %NoReserveRestrictions%==YES FILE  VGE_SPIN_ANCPOS_OUT                 /"%data_path_out%OUT_VGE_SPIN_ANCPOS.csv"              /;
FILE  VGHEAT_OUT                       /"%data_path_out%OUT_VGHEAT.csv"                    /;
FILE  VGHEAT_CONSUMED_OUT              /"%data_path_out%OUT_VGHEAT_CONSUMED.csv"           /;
FILE  VGONLINE_OUT                     /"%data_path_out%OUT_VGONLINE.csv"                  /;
FILE  OUT_VQEEQINT_OUT                 /"%data_path_out%OUT_VQEEQINT.csv"                  /;
FILE  OUT_WINDLOSSES_OUT               /"%data_path_out%OUT_WINDLOSSES.csv"                  /;
FILE  OUT_VQDAYAHEAD_OUT               /"%data_path_out%OUT_VQDAYAHEAD.csv"                /;
FILE  OUT_VQBOUNDEMAX_OUT               /"%data_path_out%OUT_VQBOUNDEMAX.csv"                /;
FILE  OUT_VQBOUNDEMIN_OUT               /"%data_path_out%OUT_VQBOUNDEMIN.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQNONSP_ANCPOSEQ_OUT         /"%data_path_out%OUT_VQNONSP_ANCPOSEQ.csv"          /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCPOSEQ_OUT               /"%data_path_out%OUT_VQANCPOSEQ.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCNEGEQ_OUT               /"%data_path_out%OUT_VQANCNEGEQ.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCPOSEQ_PRI_OUT               /"%data_path_out%OUT_VQANCPOSEQ_PRI.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCPOSEQ_SEC_OUT               /"%data_path_out%OUT_VQANCPOSEQ_SEC.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCNEGEQ_PRI_OUT               /"%data_path_out%OUT_VQANCNEGEQ_PRI.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQANCNEGEQ_SEC_OUT               /"%data_path_out%OUT_VQANCNEGEQ_SEC.csv"                /;
$ifi not %NoReserveRestrictions%==YES FILE  OUT_VQNONSP_ANCNEGEQ_OUT         /"%data_path_out%OUT_VQNONSP_ANCNEGEQ.csv"          /;
FILE  OUT_VQPOREQ_OUT                  /"%data_path_out%OUT_VQPOREQ.csv"                   /;
FILE  VWINDCUR_ANCPOS_OUT              /"%data_path_out%OUT_VWINDCUR_ANCPOS.csv"           /;
$ifi not '%NoStartUpCosts%' == Yes FILE  VGSTARTUP_OUT                    /"%data_path_out%OUT_VGSTARTUP.csv"                 /;
FILE  VHEAT_CONTENTSTORAGE_OUT         /"%data_path_out%OUT_VGHEAT_CONTENTSTORAGE.csv"     /;
FILE  BASETIME_OUT                     /"%data_path_out%OUT_BASETIMES.csv"                 /;
FILE  UNITGROUPS_IN_CASE_OUT           /"%data_path_out%OUT_UNITGROUPSINCASE.csv"          /;
FILE  ISDP_STORAGE_OUT                 /"%data_path_out%OUT_ISDP_STORAGE.csv"              /;
FILE  ISDP_ONLINE_OUT                  /"%data_path_out%OUT_ISDP_ONLINE.csv"               /;


FILE  OUT_ADJUSTEDCOST_R_DA            /"%data_path_out%OUT_ADJUSTEDCOST_R_DA.csv"         /;
FILE  OUT_ADJUSTEDCOST_R_DA_2          /"%data_path_out%OUT_ADJUSTEDCOST_R_DA_2.csv"       /;
FILE  OUT_ADJUSTEDCOST_R_DA_ID         /"%data_path_out%OUT_ADJUSTEDCOST_R_DA_ID.csv"      /;
FILE  OUT_ADJUSTEDCOST_R_ID            /"%data_path_out%OUT_ADJUSTEDCOST_R_ID.csv"         /;
FILE  OUT_CONGESTIONRENT_DA            /"%data_path_out%OUT_CONGESTIONRENT_DA.csv"         /;
FILE  OUT_CONGESTIONRENT_DA_2          /"%data_path_out%OUT_CONGESTIONRENT_DA_2.csv"       /;
FILE  OUT_CONGESTIONRENT_DA_ID         /"%data_path_out%OUT_CONGESTIONRENT_DA_ID.csv"      /;
FILE  OUT_CONGESTIONRENT_ID            /"%data_path_out%OUT_CONGESTIONRENT_ID.csv"         /;
FILE  OUT_CONGESTIONRENT_PB_DA         /"%data_path_out%OUT_CONGESTIONRENT_PB_DA.csv"      /;
FILE  OUT_CONGESTIONRENT_PB_DA_2       /"%data_path_out%OUT_CONGESTIONRENT_PB_DA_2.csv"    /;
FILE  OUT_CONGESTIONRENT_PB_DA_ID      /"%data_path_out%OUT_CONGESTIONRENT_PB_DA_ID.csv"   /;
FILE  OUT_CONGESTIONRENT_PB_ID         /"%data_path_out%OUT_CONGESTIONRENT_PB_ID.csv"      /;
FILE  OUT_CONSUMERRENT_DA              /"%data_path_out%OUT_CONSUMERRENT_DA.csv"           /;
FILE  OUT_CONSUMERRENT_DA_2            /"%data_path_out%OUT_CONSUMERRENT_DA_2.csv"         /;
FILE  OUT_CONSUMERRENT_ID              /"%data_path_out%OUT_CONSUMERRENT_ID.csv"           /;
FILE  OUT_PRODUCERRENT_DA              /"%data_path_out%OUT_PRODUCERRENT_DA.csv"           /;
FILE  OUT_PRODUCERRENT_DA_2            /"%data_path_out%OUT_PRODUCERRENT_DA_2.csv"         /;
FILE  OUT_PRODUCERRENT_DA_ID           /"%data_path_out%OUT_PRODUCERRENT_DA_ID.csv"        /;
FILE  OUT_PRODUCERRENT_ID              /"%data_path_out%OUT_PRODUCERRENT_ID.csv"           /;
FILE  OUT_VOBJ_R_T_PRODCOST_DA         /"%data_path_out%OUT_VOBJ_R_T_Prodcost_DA.csv"      /;
FILE  OUT_VOBJ_R_T_PRODCOST_ID         /"%data_path_out%OUT_VOBJ_R_T_Prodcost_ID.csv"      /;
FILE  OUT_VOBJ_R_T_INCLSLACK_DA        /"%data_path_out%OUT_VOBJ_R_T_INCLSLACK_DA.csv"     /;
FILE  OUT_VOBJ_R_T_INCLSLACK_ID        /"%data_path_out%OUT_VOBJ_R_T_INCLSLACK_ID.csv"     /;


FILE  OUT_MAXDELTA_MinChp_OUT            /"%data_path_out%OUT_MAXDELTA_MinChp.csv"/;
FILE  OUT_Change_Minchp_OUT              /"%data_path_out%OUT_Change_Minchp.csv"/;
FILE  OUT_MAXDELTA_MinChpRAMP_OUT        /"%data_path_out%OUT_MAXDELTA_MinChpRAMP.csv"/;
FILE  OUT_VCONTENTHYDRORES_NT_OUT_ct     /"%data_path_out%OUT_VCONTENTHYDRORES_ct.csv"/;
FILE  VELEC_CONTENTSTORAGE_OUT_ct        /"%data_path_out%OUT_VGELEC_CONTENTSTORAGE_ct.csv"/;

FILE  EE_DUMP_OUT                /"%data_path_out%OUT_EE_DUMP.csv"/;
FILE  HLINK_OUT                  /"%data_path_out%OUT_HLINK.csv"/;
FILE  HLINK_DC_OUT               /"%data_path_out%OUT_HLINK_DC.csv"/;
FILE  HSTA_OUT                   /"%data_path_out%OUT_HSTA.csv"/;
FILE  HSTA_OUT_HEADER            /"%data_path_out%OUT_HSTA_HEADER.csv"/;
FILE  HTAR_OUT                   /"%data_path_out%OUT_HTAR.csv"/;
FILE  CNEC_RAM_UNUSED_OUT        /"%data_path_out%OUT_CNEC_RAM_UNUSED.csv"/;
FILE  CNEC_RAM_USED_OUT          /"%data_path_out%OUT_CNEC_RAM_USED.csv"/;
FILE  OUT_IDEMAND_Nonspin_OUT    /"%data_path_out%OUT_IDEMAND_Nonspin.csv"/;
FILE  OUT_IGELECCAPACITY_Y_OUT   /"%data_path_out%OUT_IGELECCAPACITY_Y.csv"/;
FILE  OUT_IVXELEC_Phys_OUT       /"%data_path_out%OUT_IVXELEC_Phys.csv"/;
FILE  VELEC_CONTENTSTORAGE_OUT_2 /"%data_path_out%OUT_VGELEC_CONTENTSTORAGE_2.csv"/;
FILE  VGE_ANCNEG_PRI_OUT_ct      /"%data_path_out%OUT_VGE_ANCNEG_PRI_ct.csv"/;
FILE  VGE_ANCNEG_SEC_OUT_ct      /"%data_path_out%OUT_VGE_ANCNEG_SEC_ct.csv"/;
FILE  VGE_ANCPOS_PRI_OUT_ct      /"%data_path_out%OUT_VGE_ANCPOS_PRI_ct.csv"/;
FILE  VGE_ANCPOS_SEC_OUT_ct      /"%data_path_out%OUT_VGE_ANCPOS_SEC_ct.csv"/;
FILE  VGE_ANCPOS_TER_OUT_ct      /"%data_path_out%OUT_VGE_ANCPOS_TER_ct.csv"/;
FILE  VGE_ANCNEG_TER_OUT_ct      /"%data_path_out%OUT_VGE_ANCNEG_TER_ct.csv"/;
FILE  VGSHUTDOWN_OUT             /"%data_path_out%OUT_VGSHUTDOWN.csv"/;
FILE  VGTURBINE_ON_OUT           /"%data_path_out%OUT_VGTURBINE_ON.csv"/;
FILE  OUT_VHYDROSPILLAGE_neg_OUT /"%data_path_out%OUT_VHYDROSPILLAGE_neg.csv"/;
FILE  OUT_VXE_NONSPIN_ANCPOS_T_OUT       /"%data_path_out%OUT_VXE_NONSPIN_ANCPOS_T.csv"/;
FILE  AUSLAND_OUT                /"%data_path_out%OUT_AUSLAND.csv"/;
FILE  OUT_IDEMAND_ANCNEG_OUT     /"%data_path_out%OUT_IDEMAND_ANCNEG.csv"/;
FILE  OUT_IDEMAND_ANCNEG_PRI_OUT /"%data_path_out%OUT_IDEMAND_ANCNEG_PRI.csv"/;
FILE  OUT_IDEMAND_ANCNEG_SEC_OUT /"%data_path_out%OUT_IDEMAND_ANCNEG_SEC.csv"/;
FILE  OUT_IDEMAND_ANCPOS_OUT     /"%data_path_out%OUT_IDEMAND_ANCPOS.csv"/;
FILE  OUT_IDEMAND_ANCPOS_PRI_OUT /"%data_path_out%OUT_IDEMAND_ANCPOS_PRI.csv"/;
FILE  OUT_IDEMAND_ANCPOS_SEC_OUT /"%data_path_out%OUT_IDEMAND_ANCPOS_SEC.csv"/;
FILE  RDMAX_OUT                  /"%data_path_out%OUT_RDMAX.csv"/;
FILE  RDMIN_OUT                  /"%data_path_out%OUT_RDMIN.csv"/;
FILE  RDMUSTRUN_OUT              /"%data_path_out%OUT_RDMUSTRUN.csv"/;
FILE  OUT_VSOLARSHEDDING_DAY_AHEAD_OUT   /"%data_path_out%OUT_VSOLARSHEDDING_DAY_AHEAD.csv"/;
FILE  OUT_VWINDCUR_ANCPOS_OUT    /"%data_path_out%OUT_VWINDCUR_ANCPOS.csv"/;
FILE  OUT_PTDFMCPLUS_DAY         /"%data_path_out%OUT_PTDFMCPLUS_Day.csv"/;
FILE  ISOLVESTATUS_COMPACT_OUT   /"%data_path_out%ISOLVESTATUS_COMPACT.csv"/;
FILE ISOLVESTATUS_COMPACT_OUT_focustime /"%data_path_out%ISOLVESTATUS_COMPACT_OUT_focustime.csv"/;
FILE  ISOLVESTATUS_EXTENDED_OUT  /"%data_path_out%ISOLVESTATUS_EXTENDED.txt"/;
FILE  OUT_IGELECCAPEFF_OUT_ct    /"%data_path_out%OUT_IGELECCAPEFF_ct.csv"/;
FILE  OUT_QEEQDAY_M_OUT_ct       /"%data_path_out%OUT_QEEQDAY_M_ct.csv"/;
FILE  OUT_QXEXP_M_OUT      	 /"%data_path_out%OUT_QXEXP_M.csv"/;

$ifi '%CCGT_Imp%' == Yes  FILE  OUT_VQCCGT_OUT       /"%data_path_out%OUT_VQCCGT.csv"/;
$ifi '%CCGT_Imp%' == Yes  FILE     CCGT_STEAM_OUT   /"%data_path_out%OUT_CCGT_STEAM.csv"/;
$ifi '%CCGT_Imp%' == Yes  FILE     VGSTEAM_OUT      /"%data_path_out%OUT_VGSTEAM.csv"/;

FILE  OUT_OPMARG_GDISPATCH_T           /"%data_path_out%OUT_OPMARG_GDISPATCH_T.csv"        /;

$ifi '%FLEX_addon%' == Yes  FILE CO2EMISSIONS_OUT                 /"%data_path_out%OUT_CO2EMISSIONS.csv"              /;
$ifi '%FLEX_addon%' == Yes  FILE FLEX_OUT                         /"%data_path_out%OUT_FLEX.csv"                      /;
$ifi '%FLEX_addon%' == Yes  FILE DEMANDSIDE_OUT                   /"%data_path_out%OUT_DEMANDSIDE.csv"                /;
$ifi '%FLEX_addon%' == Yes  FILE HEAT_OUT                         /"%data_path_out%OUT_HEAT.csv"                      /;


* This is used for writing out gdx-files
FILE bat /renam.bat/;
