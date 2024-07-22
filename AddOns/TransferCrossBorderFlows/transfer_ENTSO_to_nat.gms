*--------------------------------------------------------------------------------------------------------
* TRANSFER CROSS BORDER FLOWS: ENTSO --> NATIONAL
*--------------------------------------------------------------------------------------------------------


* The following paths have to be supplied with setglobal:
*
*  - data_path: Path to the data root. This path has to contain:
*    - directory 01_ENTSO: directory containing the data of the 
*      ENTSO-run (for internal data structure see data_path_in_mg_root_rel)
*    - file reg_folder_mapping.gset: File containing the mapping of the region 
*      id to the name of the subdirectory containg the data, e.g. R_DE.02_DE.
*    - The directories of the market regions
*
*  - data_path_in_mg_root_rel: path the market input data relative to the  
*    path to the market region. This has to be the same for all market regions.
*    With trailing / ! Example:
*      path market region:          d:\test\data\02_DE
*      path market data:            d:\test\data\02_DE\base\model\inc_database\
*      -> data_path_in_mg_root_rel: base/model/inc_database

*  - data_path_out_mg_root_rel: path the output data of the market sim relative to 
*    the path to the market region. With trailing / !
*
*  - source_mg_folder: folder of the mg where the xborder flows are taken from (
*    should be 01_ENTSO) With trailing / !

$ifthen not set data_path
$   abort "%data_path% not set"
$endif

$ifthen not set data_path_in_mg_root_rel
$   abort "%data_path_in_mg_root_rel% not set"
$endif

$ifthen not set data_path_out_mg_root_rel
$   abort "%data_path_out_mg_root_rel% not set"
$endif

$ifthen not set source_mg_folder
$   abort "% source_mg_folder% not set"
$endif


*--------------------------------------------------------------------------------------------------------
set BASETIME /
$include '%data_path%%source_mg_folder%%data_path_in_mg_root_rel%/O Set BaseTime.inc'
/;
set RRR /
$include '%data_path%%source_mg_folder%%data_path_in_mg_root_rel%/O Set R.inc'
/;
*set runFolders             /02_DE,03_AT,04_CH,05_FR,06_BE,07_NL,08_DKW,09_PL,10_CZ,11_IT,12_SK,13_SI,14_HU/;
ALIAS(runFolders, *);
set folderMapping(RRR,runFolders) /
$INCLUDE %data_path%/reg_folder_mapping.gset
/;
DISPLAY folderMapping;

table BASE_X3FX_VAR_T(BASETIME,RRR) 'electrictiy exchange from ENTSO run'
$ondelim
$include '%data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_EXCHANGE.csv'
$offdelim

*--------------------------------------------------------------------------------------------------------
file tmp_X3FX /tmp_X3FX.inc/;
put tmp_X3FX;
loop(folderMapping(RRR,runFolders),
    put_utilities 'ren' / '%data_path%':0 runFolders.tl:0 '/%data_path_in_mg_root_rel%O Par Base_X3FX_Var_T.inc':0;
	put '* Written by transfer_ENSTO_to_nat.gms' /;
    loop(BASETIME,
        put RRR.tl:0, '.', BASETIME.tl:0, '=' BASE_X3FX_VAR_T(BASETIME,RRR):0:2 /;
    );
);
putclose;
