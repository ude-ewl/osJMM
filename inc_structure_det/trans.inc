* File Trans.inc
* This file is part of JMM2.0 (March 2018).
*------------------------------------------------------------------------------

* PARAMETER XCAPACITY contains the electrical transmission capacities
* between pairs of regions.
* Units: MW.

* The electrical transmission capacity is the capacity
* disregarding an eventual loss (see the table XLOSS).
* Thus, if there is a loss, a maximum of XCAPACITY MW may be sent into
* the transmisison line, but at most (XCAPACITY) MW may be extracted.

PARAMETER XCAPACITY(RRR,RRR,BASETIME)  'Transmission capacity between regions'
/
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_1QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_2QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_3QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_4QU.inc";
/;

PARAMETER XCAPACITY_Day_ahead(RRR,RRR,BASETIME)  'Transmission capacity day-ahead between regions'
/
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_Day_ahead_1QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_Day_ahead_2QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_Day_ahead_3QU.inc";
$INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_Day_ahead_4QU.inc";
/;

$ifi '%LFB%' ==  Yes TABLE XCAPACITY_FLG_SFD(TRL,GRIDLOADCASE)  'Remaining Available Margin of Flowgate in Standard Flow Direction (positive)'
$ifi '%LFB%' ==  Yes $ondelim
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_FLG_SFD_GRIDLOADCASE.csv";
$ifi '%LFB%' ==  Yes $offdelim;

$ifi '%LFB%' ==  Yes TABLE XCAPACITY_FLG_NSFD(TRL,GRIDLOADCASE)  'Remaining Available Margin of Flowgate in Non-Standard Flow Direction (negative)'
$ifi '%LFB%' ==  Yes $ondelim
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_FLG_NSFD_GRIDLOADCASE.csv";
$ifi '%LFB%' ==  Yes $offdelim;


$ifi '%Code_version%'==UENB PARAMETER XCAPACITY_NO_FLG(RRR,RRR,BASETIME)  'Aggregate Non-Flowgate transmission capacity between regions (positive) - within Flow-Based MC region'
$ifi '%Code_version%'==EWL PARAMETER XCAPACITY_NO_FLG(RRR,RRR)  'Aggregate Non-Flowgate transmission capacity between regions (positive) - within Flow-Based MC region'
/
$ifi '%LFB%' ==  Yes $ondelim
$ifi  '%LFB%' == Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL.csv";
$ifi '%LFB%' ==  Yes $offdelim;
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_01_Jan.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_02_Feb.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_03_Mar.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_04_Apr.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_05_May.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_06_Jun.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_07_Jul.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_08_Aug.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_09_Sep.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_10_Oct.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_11_Nov.csv";
$ifi ('%LFB%' == Yes AND '%data_source%'=='Access') ==  Yes $INCLUDE "%data_path_in%inc_database\O Par XCAPACITY_NO_FLG_ALL_12_Dec.csv";
/;


* PARAMETER XCOST contains information about the electrical transmission
* cost between pairs of regions.
* Units: EUR2002/MW.

* The electrical transmission cost is applied to the electricity
* entering the transmission line.

* Observe: Unreasonable results may be found if there are neither
* cost nor loss associated with electrical transmission.
* Therefore for all (non-diagonal) entries there should be a positive number
* in either XCOST or in XLOSS.

PARAMETER XCOST(IRRRE,IRRRI)  'Transmission cost between countries'
/
$INCLUDE "%data_path_in%inc_database\O Par XCOST.inc";
/
;

* PARAMETER XLOSS contains the loss in transmission is expressed as a fraction
* of the power entering the transmission line.
* Units: (none)

PARAMETER XLOSS(RRR,RRR)  'Transmission loss between regions'
/
$INCLUDE "%data_path_in%inc_database\O Par XLOSS.inc";
/
;

*LFB_Intraday
$ifi '%LFB_intraday%' == Yes PARAMETER PTDF(RRR,RRR,RRR) 'PTDF values'
$ifi '%LFB_intraday%' == Yes /
$ifi '%LFB_intraday%' == Yes $INCLUDE "%data_path_in%inc_database\O Par PTDF.inc";
$ifi '%LFB_intraday%' == Yes /
$ifi '%LFB_intraday%' == Yes ;

$ifi Not '%No_Load_Flow%' == Yes PARAMETER XTYPE(RRR,RRR) 'Transmission type'
$ifi Not '%No_Load_Flow%' == Yes /
$ifi Not '%No_Load_Flow%' == Yes $INCLUDE "%data_path_in%inc_database\O Par Xtype.inc";
$ifi Not '%No_Load_Flow%' == Yes /
$ifi Not '%No_Load_Flow%' == Yes ;


PARAMETER IXNTCINGOING(INTC)
/
$ifi '%GroupNTC%' == Yes $INCLUDE "%data_path_in%inc_database\O Par R_NTC_in.inc";
/;

PARAMETER IXNTCOUTGOING(INTC)
/
$ifi '%GroupNTC%' == Yes $INCLUDE "%data_path_in%inc_database\O Par R_NTC_out.inc";
/;


$ifi '%LFB_intraday%' == Yes PARAMETER PTDF_Offset(RRR,RRR) 'Offset for line flows when using PTDF values'
$ifi '%LFB_intraday%' == Yes /
$ifi '%LFB_intraday%' == Yes $INCLUDE "%data_path_in%inc_database\O Par PTDF Offset.inc";
$ifi '%LFB_intraday%' == Yes /
$ifi '%LFB_intraday%' == Yes ;
* #cw off

* If day ahead load flow market coupling is chosen, define parameter XType
* (which will determine, if a transmission line is controllable (using up to
* maximum capacity or - to the opposite - be acting according to Kirchhoff's
* law) and get PTDF value

$ifi '%LFB%' ==  Yes TABLE PTDF_FLG(RRR,TRL,GRIDLOADCASE) 'PTDF values of Flowgates'
$ifi '%LFB%' ==  Yes $ondelim
$ifi '%LFB%' ==  Yes $INCLUDE "%data_path_in%inc_database\O Par PTDF_FLG_GRIDLOADCASE.csv";
$ifi '%LFB%' ==  Yes $offdelim
;
