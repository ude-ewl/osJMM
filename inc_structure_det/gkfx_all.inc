* File gkfx_all.inc
*------------------------------------------------------------------------------

PARAMETER GKFXELEC(YYY,AAA,G)  'Power capacity of generation technologies'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFXELEC.inc";
/;

PARAMETER GKFXHEAT(YYY,AAA,G)  'Heat capacity of generation technologies'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFXHEAT.inc";
/
;
$ontext
PARAMETER GKFXHEAT_CHPTool(YYY,AAA,G)  'Heat capacity of generation technologies corrected by CHP Tool'
/
$INCLUDE "inc_database/O Par GKFXHEAT_CHPTool.inc";
/
;

GKFXHEAT(YYY,AAA_CHP_Exo,GCHP_FixQ) = GKFXHEAT_CHPTool(YYY,AAA_CHP_Exo,GCHP_FixQ);
$offtext

$ifi '%CCGT_Imp%' == Yes PARAMETER GKFXSTEAM(YYY,AAA,G)  'Power capacity of generation technologies'
$ifi '%CCGT_Imp%' == Yes /
$ifi '%CCGT_Imp%' == Yes $INCLUDE "%data_path_in%inc_database\O Par GKFXSTEAM.inc";
$ifi '%CCGT_Imp%' == Yes /;

*----- storages -------------------------------------------------------------
PARAMETER GKFX_CONTENTSTORAGE(YYY,AAA,G) 'Storage capacity of heat and power storages in MWh'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFX_CONTENTSTORAGE.inc";
$INCLUDE "%data_path_in%inc_database\O Par GKFX_CONTENTSTORAGE Heat.inc";
/;

PARAMETER GKFX_MINCONTENTSTORAGE(YYY,AAA,G) 'Minimum storage content of heat and power storages in MWh'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFX_MINCONTENTSTORAGE.inc";
/;

PARAMETER  GKFX_CHARGINGSTORAGE(YYY,AAA,G) 'Capacity of charging (loading) the heat or power storage'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFX_CHARGINGSTORAGE.inc";
$INCLUDE "%data_path_in%inc_database\O Par GKFX_CHARGINGSTORAGE Heat.inc";
/;

Parameter GKFX_CHARGINGRES(YYY,AAA,G) 'Capacity of pumping into reservoir'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFX_CHARGINGReservoir.inc";
/;

PARAMETER  GKFX_MINCHARGINGSTORAGE(YYY,AAA,G) 'Minimum capacity of charging (loading) the heat or power storage'
/
$INCLUDE "%data_path_in%inc_database\O Par GKFX_MINCHARGINGSTORAGE.inc";
/;

* --- hydro reservoir --------------------------------------------------------
PARAMETER GKMAX_CONTENTHYDRORES(YYY,AAA,G) 'Hydro reservoir capacity in MWh'
/
$INCLUDE "%data_path_in%inc_database\O Par GKMAX_CONTENTHYDRORES.inc";
/
;

PARAMETER GKMIN_CONTENTHYDRORES(YYY,AAA,G) 'Minimum hydro reservoir content of in MWh'
/
$INCLUDE "%data_path_in%inc_database\O Par GKMIN_CONTENTHYDRORES.inc";
/;

AG_CHP_fixQ_Y(AAA,G) = NO;
$ifi '%CHP%' == Yes AG_CHP_fixQ_Y(AAA,G) = YES$(GCHP_FixQ(G) AND AAA_CHP_Exo(AAA) AND (SUM(YYY,(GKFXELEC(YYY,AAA,G)) >0) OR SUM(YYY,(GKFXHEAT(YYY,AAA,G)) >0) ));

$ifi '%CHP%'==YES GDATA(AG_CHP_fixQ_Y(AAA_CHP_Exo,GCHP_FixQ),'GDCV') = 0;

$ifi '%CHP%'==YES LOOP (AAA_CHP_Exo,
$ifi '%CHP%'==YES LOOP (GCHP_FixQ,
$ifi '%CHP%'==YES GDATA(AG_CHP_fixQ_Y(AAA_CHP_Exo,GCHP_FixQ),'GDFE_SLOPE')$(GONESLOPES_CHPTool(GCHP_FixQ)) = GDATA_CHPTool(AAA_CHP_Exo,GCHP_FixQ,'GDFE_SLOPE')$(GONESLOPES_CHPTool(GCHP_FixQ));
$ifi '%CHP%'==YES GDATA(AG_CHP_fixQ_Y(AAA_CHP_Exo,GCHP_FixQ),'GDFE_SECTION') = GDATA_CHPTool(AAA_CHP_Exo,GCHP_FixQ,'GDFE_SECTION');
$ifi '%CHP%'==YES GDATA(AG_CHP_fixQ_Y(AAA_CHP_Exo,GCHP_FixQ),'GDCB') = GDATA_CHPTool(AAA_CHP_Exo,GCHP_FixQ,'GDCB');
$ifi '%CHP%'==YES GDATA(AG_CHP_fixQ_Y(AAA_CHP_Exo,GCHP_FixQ),'GDCV') = GDATA_CHPTool(AAA_CHP_Exo,GCHP_FixQ,'GDCV');
$ifi '%CHP%'==YES );
$ifi '%CHP%'==YES );