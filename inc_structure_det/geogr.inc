* File Geogr.inc
*------------------------------------------------------------------------------

* The file contains information that is geographically specific
* in relation to the following subjects:

* DISTRIBUTION
* TAXES
* EMISSION TAXES
* ELASTIC DEMANDS
* DEMANDS FOR SPINNING RESERVE
* MINIMUM NUMBER OF ONLINE UNITS REQUIRED


* DISTRIBUTION  ---------------------------------------------------------------

* PARAMETER DISLOSS_E holds the loss in electricity distribution,
* as a fraction of the electricity generated.
* Unit: (none)

PARAMETER DISLOSS_E(RRR)  'Loss in electricity distribution'
/
$INCLUDE "%data_path_in%inc_database\O Par DISLOSS_E.inc";
/;

* PARAMETER DISLOSS_H holds the loss in heat distribution,
* as a fraction of heat generated.

* Unit: (none). Minimum 4 digits after decimal point when calculated

PARAMETER DISLOSS_H(AAA)  'Loss in heat distribution'
/
$INCLUDE "%data_path_in%inc_database\O Par DISLOSS_H.inc";
/;

* PARAMETER DISCOST_E holds the cost of electricity distribution,
* given relative to end consumption.

* Unit: EUR2002/MWh.

PARAMETER DISCOST_E(RRR)  'Cost of electricity distribution'
/
$INCLUDE "%data_path_in%inc_database\O Par DISCOST_E1.inc";
/;

* PARAMETER DISLOSS_H holds the loss in heat distribution,
* as a fraction of heat generated.

* Unit: EUR2002/MWh.

PARAMETER DISCOST_H(AAA)  'Cost of heat distribution'
/
$INCLUDE "%data_path_in%inc_database\O Par DISCOST_H.inc";
/;

* TAXES -------------------------------------------------------

* PARAMETER TAX_HEATPUMP holds heat pump consumers' tax on electricity consumption.
* Unit: EUR2002/MWh.

PARAMETER TAX_HEATPUMP(C)  'Heat pump consumers tax on electricity consumption'
/
$INCLUDE "%data_path_in%inc_database\O Par TAX_HEATPUMP.inc";
/;

* PARAMETER ELEC_SUBSIDY holds subsidy on electricity produced from biomass or waste
* Unit: EUR2002/MWh. Value < 0!

PARAMETER ELEC_SUBSIDY(C,FFF)  'Subsidy on electricity from biomass or waste'
/
$INCLUDE "%data_path_in%inc_database\O Par ELEC_SUBSIDY.inc";
/;

* PARAMETER TAX_DE holds consumers' tax on electricity consumption.
* Unit: EUR2002/MWh.

PARAMETER TAX_DE(C)  'Consumers tax on electricity consumption'
/
$INCLUDE "%data_path_in%inc_database\O Par TAX_DE.inc";
/;

* PARAMETER TAX_DH holds consumers' tax on heat consumption.
* Unit: EUR2002/MWh.

PARAMETER TAX_DH(C)  'Consumers tax on heat consumption'
/
$INCLUDE "%data_path_in%inc_database\O Par TAX_DH.inc";
/;

* EMISSION TAXES ---------------------------------------------------------------

* --- Daily or weekly emission taxes
PARAMETER M_POL(MPOLSET,C,WEEKS)  'Emission policy data'
/
$INCLUDE "%data_path_in%inc_database\O Par M_POL Week.inc";
/;

* PARAMETERS RELATED TO ELASTIC DEMANDS-----------------------------------------

* PARAMETER DEFP_BASE gives an annual average consumer price
* of electricity (including taxes) in the base year.
* Unit: EUR2002/MWh

* It is used as nominal value for calculations in relation to elastic demands.

PARAMETER DEFP_BASE(RRR)  'Nominal annual average consumer electricity price'
/
$INCLUDE "%data_path_in%inc_database\O Par DEFP_BASE.inc";
/;

* Heat:

* PARAMETER DHFP_BASE gives the annual average consumer price
* of heat(including taxes) in the base year.
* Unit: EUR2002/MWh.

* It is used as nominal value for calculations in relation to elastic demands.

PARAMETER DHFP_BASE(AAA)  'Nominal annual average consumer heat price'
/
$INCLUDE "%data_path_in%inc_database\O Par DHFP_BASE.inc";
/;
