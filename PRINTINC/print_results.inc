* File print results_node_t.inc.
* For specifying printing of results for simulations

* This file is part of the Wilmar model

* -----------------------------------------------------------------------------

TWRITEOUT(BASETIME,T) = NO;
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
         TWRITEOUT(BASETIME,T) = YES;
      );
   );

* -----------------------------------------------------------------------------
* Contents:

* Each of the following files provides a specific set of output to a
* specific file, cf. the file print2.inc.

* Variables (alphabetical order)

$INCLUDE "%code_path_printinc%print-OUT_VCONTENTHYDRORES_NT.inc";
$INCLUDE "%code_path_printinc%print-OUT_VDEMANDELECFLEXIBLE.inc";
$INCLUDE "%code_path_printinc%print-OUT_VELEC_CONTENTSTORAGE.inc";
* DSM
$INCLUDE "%code_path_printinc%print-OUT_VELEC_DSMCONTENTSTORAGE.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCPOS_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCPOS_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCNEG_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCNEG_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_SPIN_ANCNEG.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCNEG.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_ANCPOS.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_NONSPIN_ANCPOS.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_SPIN_ANCPOS.inc";


$INCLUDE "%code_path_printinc%print-OUT_VGELEC.inc";
*$INCLUDE "%code_path_printinc%print-OUT_VGELECtestror.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED_DNEG.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED_DPOS.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED_Realised.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_DNEG.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_DPOS.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VGELEC_DPOS_3h.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VGELEC_DNEG_3h.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED_DPOS_3h.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VGELEC_CONSUMED_DNEG_3h.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGELEC_Realised.inc";


$INCLUDE "%code_path_printinc%print-OUT_VGFUELUSAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGHEAT.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGHEAT_CONSUMED.inc";
$if     '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VGONLCHARGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGONLINE.inc";
$ifi not '%NoStartUpCosts%' == Yes $if Not '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VGSTARTUP.inc";
$if     '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VGSTARTUPFUELCONS.inc";
$INCLUDE "%code_path_printinc%print-OUT_VHEAT_CONTENTSTORAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_VHYDROSPILLAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_withoutSlacks.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_All.inc";

*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VQRESLEVELUP_DA.inc";
*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VQRESLEVELDOWN_DA.inc";
*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VQRESLEVELUP_ID.inc";
*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VQRESLEVELDOWN_ID.inc";
*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_ISDP_HYDRORES_ENDTIME_DA.inc";
*$ifi '%HydroPenalty%' == Yes $INCLUDE "%code_path_printinc%print-OUT_ISDP_HYDRORES_ENDTIME_ID.inc";

* slacks
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCNEGEQ.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCPOSEQ.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCPOSEQ_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCPOSEQ_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCNEGEQ_PRI.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQANCNEGEQ_SEC.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQNONSP_ANCNEGEQ.inc";
$INCLUDE "%code_path_printinc%print-OUT_VQDAYAHEAD.inc";
$INCLUDE "%code_path_printinc%print-OUT_VQEEQINT.inc";
$INCLUDE "%code_path_printinc%print-OUT_VQBOUNDEMIN.inc";
$INCLUDE "%code_path_printinc%print-OUT_VQBOUNDEMAX.inc";
$if     '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VQESTOLOAD1.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VQNONSP_ANCPOSEQ.inc";

$ifi '%Renewable_Spinning%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VWINDCUR_ANCPOS.inc";
$INCLUDE "%code_path_printinc%print-OUT_VWINDSHEDDING_DAY_AHEAD.inc";

$INCLUDE "%code_path_printinc%print-OUT_VXELEC_T.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXELEC_DPOS_T.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXELEC_DNEG_T.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DPOS_T_3h.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DNEG_T_3h.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised_woSaldo.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised_EnSAVes.inc";
*$INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised_netexport.inc";

$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DC_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DC_DPOS_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DC_DNEG_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_DPOS_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_DNEG_T.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXEXP_M.inc";
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DC_DPOS_T_3h.inc";
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_DC_DNEG_T_3h.inc";
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_DPOS_T_3h.inc";
$ifi '%Looping%' == 3hours $ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_DNEG_T_3h.inc";

*$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised_PhysicalFlows.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_T_Realised_onlyNTCDC.inc";
$ifi '%LFB_NE%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_T_Realised.inc";
$ifi '%LFB_NE%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VXELEC_ExportFB_T_Realised_ct.inc";


*$ifi '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXKPTDF5Plus.txt";
*$ifi '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXKPTDF5Minus.txt";
*$ifi '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXKPTDF4Plus.txt";
*$ifi '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXKPTDF4Minus.txt";


$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_NONSP_ANCPOS.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_VGE_CONSUMED_NONSP_ANCNEG.inc";

* $INCLUDE "%code_path_printinc%print-OUT_PTDFCAP_SLACK.inc";


* marginals
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCNEGEQ_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCPOSEQ_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCPOSEQ_PRI_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCPOSEQ_SEC_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCNEGEQ_PRI_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QANCNEGEQ_SEC_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QNONSP_ANCNEGEQ_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_QEEQDAY_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_QEEQINT_M.inc";
$ifi '%Looping%' == 3hours $INCLUDE "%code_path_printinc%print-OUT_QEEQINT_M_3h.inc";
$INCLUDE "%code_path_printinc%print-OUT_QESTOVOLT_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_QHEQ_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_QHSTOVOLT_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_QHYRSSEQ_M.inc";
$ifi not %NoReserveRestrictions%==YES $INCLUDE "%code_path_printinc%print-OUT_QNONSP_ANCPOSEQ_M.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXCAPFBMCPlus_M.inc";
$ifi '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXCAPFBMCMinus_M.inc";
$ifi '%LFB_NE%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXCAPFBMCPlus_M.inc";
$ifi '%LFB_NE%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QXCAPFBMCMinus_M.inc";
$ifi not '%NoStartUpCosts%' == Yes $if Not '%UnitCmin%' == Yes $INCLUDE "%code_path_printinc%print-OUT_QGONLSTART_M.inc";


$INCLUDE "%code_path_printinc%print-OUT_CaseName.inc";
*$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_REPLACE_RES.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMANDELEC_REALISED.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_REALISED2.inc";


$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_inclSlack_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_ADJUSTEDCOST_R_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_DA.inc";
*$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_PB_DA.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONSUMERRENT_DA.inc";

$INCLUDE "%code_path_printinc%print-OUT_MAXDELTA_MinChp.inc";
$INCLUDE "%code_path_printinc%print-OUT_Change_MinCHP.inc";
$INCLUDE "%code_path_printinc%print-OUT_MAXDELTA_MinChpRAMP.inc";
*$INCLUDE "%code_path_printinc%Print-OUT_IGELECCAPEFF_ct.inc";
$INCLUDE "%code_path_printinc%Print-OUT_QEEQDAY_M_ct.inc";
$INCLUDE "%code_path_printinc%Print-OUT_VCONTENTHYDRORES_NT_ct.inc";
$INCLUDE "%code_path_printinc%Print-OUT_VELEC_CONTENTSTORAGE_ct.inc";
*$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_PRI_ct.inc";

$INCLUDE "%code_path_printinc%print-OUT_BASETIME.inc";
$INCLUDE "%code_path_printinc%print-OUT_EE_DUMP.inc";
$INCLUDE "%code_path_printinc%print-OUT_HLINK.inc";
$INCLUDE "%code_path_printinc%print-OUT_HLINK_DC.inc";
$INCLUDE "%code_path_printinc%print-OUT_HSTA.inc";
$INCLUDE "%code_path_printinc%print-OUT_HSTA_HEADER.inc";
$INCLUDE "%code_path_printinc%print-OUT_HTAR.inc";
$include "%code_path_printinc%print-OUT_CNEC_RAM_UNUSED.inc";
$include "%code_path_printinc%print-OUT_CNEC_RAM_USED.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_Nonspin.inc";
$INCLUDE "%code_path_printinc%print-OUT_IGELECCAPACITY_Y.inc";
$INCLUDE "%code_path_printinc%print-OUT_IGELECCAPEFF.inc";
$if '%LFB%' == Yes $INCLUDE "%code_path_printinc%print-OUT_IVXELEC_Phys.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_ONLINE.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_REALISED.inc";
$INCLUDE "%code_path_printinc%print-OUT_IX3COUNTRY_T_Y.inc";
$if '%LFB_Intraday%' == Yes $INCLUDE "%code_path_printinc%print-OUT_PTDFCAP_SLACK.inc";
$INCLUDE "%code_path_printinc%print-OUT_TechnologyData.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_PRI_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_SEC_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCNEG_TER_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS_PRI_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS_SEC_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGE_ANCPOS_TER_ct.inc";
$INCLUDE "%code_path_printinc%print-OUT_VGTURBINE_ON.inc";
$INCLUDE "%code_path_printinc%print-OUT_VHYDROSPILLAGE_neg.inc";
$INCLUDE "%code_path_printinc%print-OUT_VXE_NONSPIN_ANCPOS_T.inc";
$INCLUDE "%code_path_printinc%print-OUT_HEAT_CONS_T.inc";
$INCLUDE "%code_path_printinc%print-OUT_UNITGROUPS_IN_CASE.inc";
$INCLUDE "%code_path_printinc%print-OUT_AUSLAND.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCNEG.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCNEG_PRI.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCNEG_SEC.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCPOS.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCPOS_PRI.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMAND_ANCPOS_SEC.inc";
$INCLUDE "%code_path_printinc%print-OUT_RDMAX.inc";
$INCLUDE "%code_path_printinc%print-OUT_RDMIN.inc";
$INCLUDE "%code_path_printinc%print-OUT_RDMUSTRUN.inc";
$INCLUDE "%code_path_printinc%print-OUT_VSOLARSHEDDING_DAY_AHEAD.inc";
$ifi '%Renewable_Spinning%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VWINDCUR_ANCPOS_M.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_STORAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_HYDRORES.inc";

$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_CO2Cost.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_CO2Cost_Start.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Fuelcost.inc";
$ifi not '%NoStartUpCosts%' == Yes $INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_StartCost.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_OMVCost.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Penalties.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Trans.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Revenue.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_TaxSub.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_FlexDemand.inc";


$ontext
$INCLUDE "%code_path_printinc%print-OUT_TechnologyData.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_REALISED.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_REALISED3.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_BID.inc";
$INCLUDE "%code_path_printinc%print-OUT_IWIND_BID2.inc";

$INCLUDE "%code_path_printinc%print-OUT_HELPVAR6.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDE_REALISED_RESIDUAL.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDEMANDELEC_BID.inc";
$INCLUDE "%code_path_printinc%print-OUT_IDE_BID_RESIDUAL.inc";

$INCLUDE "%code_path_printinc%print-OUT_HEAT_CONS_T.inc";
$INCLUDE "%code_path_printinc%print-OUT_CO2Price.inc";
$INCLUDE "%code_path_printinc%print-OUT_FuelPrice.inc";
$INCLUDE "%code_path_printinc%print-OUT_IX3COUNTRY_T_Y.inc";
$INCLUDE "%code_path_printinc%print-OUT_IGELECCAPEFF.inc";

$INCLUDE "%code_path_printinc%print-OUT_ISDP_HYDRORES.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_HYDRORES_STEP.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_STORAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_ISDP_ONLINE.inc";

$INCLUDE "%code_path_printinc%print-OUT_WINDLOSSES.inc";
$INCLUDE "%code_path_printinc%print-OUT_CAPLOST_OUTAGE.inc";
$INCLUDE "%code_path_printinc%print-OUT_BASETIME.inc";
$INCLUDE "%code_path_printinc%print-OUT_UNITGROUPS_IN_CASE.inc";


$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_inclSlack_DA.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Prodcost_DA.inc";
$INCLUDE "%code_path_printinc%print-OUT_VOBJ_R_T_Prodcost_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_ADJUSTEDCOST_R_DA.inc";
$INCLUDE "%code_path_printinc%print-OUT_ADJUSTEDCOST_R_DA_2.inc";
$INCLUDE "%code_path_printinc%print-OUT_ADJUSTEDCOST_R_DA_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_DA_2.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_DA_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_PB_DA_2.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_PB_DA_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONGESTIONRENT_PB_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONSUMERRENT_DA_2.inc";
$INCLUDE "%code_path_printinc%print-OUT_CONSUMERRENT_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_PRODUCERRENT_DA.inc";
$INCLUDE "%code_path_printinc%print-OUT_PRODUCERRENT_DA_2.inc";
$INCLUDE "%code_path_printinc%print-OUT_PRODUCERRENT_DA_ID.inc";
$INCLUDE "%code_path_printinc%print-OUT_PRODUCERRENT_ID.inc";

$offtext

$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_VQCCGT.inc";
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_CCGT_STEAM.inc";
$ifi '%CCGT_Imp%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_VGSTEAM.inc";

* Commented out the following file as the set id 'Erdgas_DE' is unknown for regions other than DE and ENTSO
*$INCLUDE "%code_path_printinc%print-OUT_OPMARG_GDISPATCH_T.inc";

$ifi '%FLEX_addon%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_CO2EMISSIONS.inc";
$ifi '%FLEX_addon%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_FLEX.inc";
$ifi '%FLEX_addon%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_DEMANDSIDE.inc";
$ifi '%FLEX_addon%' == Yes  $INCLUDE "%code_path_printinc%print-OUT_HEAT.inc";


