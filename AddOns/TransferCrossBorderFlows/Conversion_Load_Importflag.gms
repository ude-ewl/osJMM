*$include "Path_name.inc"
$show

$onempty
$offlisting
*Table of content:
*(1) Read in of required sets and results from ENTSO - Model - Run
*(2) Perform operation - Creation of new load time series for national model runs
*(3) Write new load time series for national model runs


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

*-------------------------------------------------------------------------------
*Inclusion of input data
*-------------------------------------------------------------------------------

SET IPLUSMINUS   'Violation of equation'  /IPLUS Violation of equation upwards, IMINUS  Violation of equation downwards/;

Set R
/
$include "%data_path%%source_mg_folder%%data_path_in_mg_root_rel%O set R.inc";
/;
Alias(R,RIM);
Alias(R,REX);

Set RID
/
$include "%data_path%%source_mg_folder%%data_path_in_mg_root_rel%O set RId.inc";
/;
Alias (RID,RIDIM);
Alias (RID,RIDEX);

Set R_RID
/
$include "%data_path%%source_mg_folder%%data_path_in_mg_root_rel%O set R_RId.inc";
/;


Set Basetime
/
$include "%data_path%%source_mg_folder%%data_path_in_mg_root_rel%O set Basetime.inc";
/;

*Set CaseID /1*99/;
Alias(CaseID,*);

Parameter Load(R,Basetime)
/
$include "%data_path%%source_mg_folder%%data_path_in_mg_root_rel%O Par Base_DE_Var Det.inc";
/;

$set sed sed
$onecho > sedscript
1d
$offecho

$call sed -f sedscript %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VXELEC_Realised.csv > %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VXELEC_Realised_no_header.csv
Parameter Exchanges_ID(CaseID,RIDEX,RIDIM,Basetime)
/
$ondelim
$INCLUDE "%data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VXELEC_Realised_no_header.csv";
$offdelim
/;

$call sed -f sedscript %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VWINDSHEDDING_DAY_AHEAD.csv > %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VWINDSHEDDING_DAY_AHEAD_no_header.csv
Parameter VWindshedding(CaseID,RID,Basetime)
/
$ondelim
$INCLUDE "%data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VWINDSHEDDING_DAY_AHEAD_no_header.csv";
$offdelim
/;

$call sed -f sedscript %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VSOLARSHEDDING_DAY_AHEAD.csv > %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VSOLARSHEDDING_DAY_AHEAD_no_header.csv
Parameter VSolarshedding(CaseID,RID,Basetime)
/
$ondelim
$INCLUDE "%data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VSOLARSHEDDING_DAY_AHEAD_no_header.csv";
$offdelim
/;

$call sed -f sedscript %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VQDAYAHEAD.csv > %data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VQDAYAHEAD_no_header.csv
Parameter VQDAYAHEAD_CRID(CaseID,RID,Basetime, IPLUSMINUS)
/                            
$ondelim
$INCLUDE "%data_path%%source_mg_folder%%data_path_out_mg_root_rel%OUT_VQDAYAHEAD_no_header.csv";
$offdelim
/;


*-------------------------------------------------------------------------------
*Data checks: caseID
ABORT$( SUM(CaseID, 1$(SUM((RIDEX,RIDIM,Basetime), Exchanges_ID(CaseID,RIDEX,RIDIM,Basetime))>0) ) > 1) "ERROR: There are more than one CaseIDs in use!";


*-------------------------------------------------------------------------------
Parameter Exchanges_L2 (CaseID,REX,RIDIM,Basetime);
Parameter Exchanges (CaseID,REX,RIM,Basetime);
Exchanges_L2(CaseID,REX,RIDIM,Basetime) = sum(R_RID(REX,RIDEX), Exchanges_ID(CaseID,RIDEX,RIDIM,Basetime));
Exchanges(CaseID,REX,RIM,Basetime) = sum(R_RID(RIM,RIDIM), Exchanges_L2(CaseID,REX,RIDIM,Basetime));


Parameter WindsheddingL2(R,Basetime);
Parameter SolarsheddingL2(R,Basetime);

*-------------------------------------------------------------------------------
*2 Perform operation
*-------------------------------------------------------------------------------
Parameter VQDAYAHEAD_Plus(R,Basetime);
Parameter Balance(R,Basetime);
Parameter Imports(R,Basetime);
Parameter Exports(R,Basetime);
Parameter ImportFlag(R,Basetime);
Parameter ImportFlag2(R,Basetime);
Parameter NoDumpInENTSOFlag(R,Basetime);

*Loop(CaseID$(ord(caseID)=1),
* Imports(R,Basetime)=Sum(REX,Exchanges(CaseID,Rex,R,Basetime));
* Exports(R,Basetime)=Sum(RIM,Exchanges(CaseID,R,RIM,Basetime));
* WindsheddingL2(R,Basetime)=Sum(R_RID(R,RID),VWindshedding(CaseID,RID,Basetime));
* SolarsheddingL2(R,Basetime)=Sum(R_RID(R,RID),VSolarshedding(CaseID,RID,Basetime));
*  );

VQDAYAHEAD_Plus(R,Basetime)=Sum((R_RID(R,RID), CaseID), VQDAYAHEAD_CRID(CaseID, RID, Basetime, 'IPLUS'));

Imports(R,Basetime)=Sum((REX, CaseID),Exchanges(CaseID,Rex,R,Basetime));
Exports(R,Basetime)=Sum((RIM, CaseID),Exchanges(CaseID,R,RIM,Basetime));
WindsheddingL2(R,Basetime)=Sum((R_RID(R,RID), CaseID),VWindshedding(CaseID,RID,Basetime));
SolarsheddingL2(R,Basetime)=Sum((R_RID(R,RID), CaseID),VSolarshedding(CaseID,RID,Basetime));

Balance(R,Basetime) = Exports(R,Basetime) - IMports (R,Basetime);

*ImportFlag(R,Basetime) = 0;
*ImportFlag(R,Basetime) = 1$(Imports(R,Basetime) > 0);
*ImportFlag(R,Basetime) = 0$((WindsheddingL2(R,Basetime) GT 0) OR (SolarsheddingL2(R,Basetime) GT 0) OR (Imports(R,Basetime) = 0));
*ImportFlag(R,Basetime) = 0;
*ImportFlag(R,Basetime) = 1$(not((WindsheddingL2(R,Basetime) GT 0) OR (SolarsheddingL2(R,Basetime) GT 0) OR (Imports(R,Basetime) = 0)));   ;
*ImportFlag2(R,Basetime) = 1$(Imports(R,Basetime) > 0);
*NoDumpInENTSOFlag(R,Basetime) = 1$( (WindsheddingL2(R,Basetime) EQ 0) AND (SolarsheddingL2(R,Basetime) EQ 0) AND (VQDAYAHEAD_Plus(R,Basetime) EQ 0) );

ImportFlag(R,Basetime) = 0;
ImportFlag(R,Basetime)$(Imports(R,Basetime) > 0) = 1;
NoDumpInENTSOFlag(R,Basetime) = ImportFlag(R,Basetime);
NoDumpInENTSOFlag(R,Basetime)$( (WindsheddingL2(R,Basetime) EQ 0) AND (SolarsheddingL2(R,Basetime) EQ 0) AND (VQDAYAHEAD_Plus(R,Basetime) EQ 0) ) = 1;

display Importflag, windsheddingL2, SolarsheddingL2, Imports;
*-------------------------------------------------------------------------------
*3 Create Output Files
*-------------------------------------------------------------------------------
ALIAS(runFolders, *);
ALIAS(RRR, R)
set folderMapping(RRR,runFolders) /
$INCLUDE %data_path%/reg_folder_mapping.gset
/;
DISPLAY folderMapping;



file tmp_Balance /tmp_Balance.inc/;
put tmp_Balance;
loop(folderMapping(RRR,runFolders),
    put_utilities 'ren' / '%data_path%':0 runFolders.tl:0 '/%data_path_in_mg_root_rel%O Par Base_Export_Bal_NotOptimised_Var_T.inc':0;
	put '* Written by Conversion_Load_Importflag.gms' /;
    loop(BASETIME,
        If(Balance(RRR, BASETIME) NE 0,
            put RRR.tl:0, '.', BASETIME.tl:0, '=' Balance(RRR, BASETIME):0:2 /;
        )
    );
);
putclose tmp_Balance;

file tmp_ImportFlag /tmp_ImportFlag.inc/;
put tmp_ImportFlag;
loop(folderMapping(RRR,runFolders),
    put_utilities 'ren' / '%data_path%':0 runFolders.tl:0 '/%data_path_in_mg_root_rel%O Par Base IMPORT_FLAG.inc':0;
	put '* Written by Conversion_Load_Importflag.gms' /;
    loop(BASETIME,
        If(ImportFlag(RRR, BASETIME) NE 0,
			put RRR.tl:0, '.', BASETIME.tl:0, '=' ImportFlag(RRR, BASETIME):0:2 /;
		)
    );
);
putclose tmp_ImportFlag;

file tmp_NoDumpInENTSOFlag /tmp_NoDumpInENTSOFlag.inc/;
put tmp_NoDumpInENTSOFlag;
loop(folderMapping(RRR,runFolders),
    put_utilities 'ren' / '%data_path%':0 runFolders.tl:0 '/%data_path_in_mg_root_rel%O Par Base NoDumpInENTSO_Flag.inc':0;
	put '* Written by Conversion_Load_Importflag.gms' /;
    loop(BASETIME,
        If(NoDumpInENTSOFlag(RRR, BASETIME) NE 0,
			put RRR.tl:0, '.', BASETIME.tl:0, '=' NoDumpInENTSOFlag(RRR, BASETIME):0:2 /;
		)
    );
);
putclose tmp_NoDumpInENTSOFlag;
