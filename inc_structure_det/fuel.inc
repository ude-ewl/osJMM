* File Fuel.inc
*------------------------------------------------------------------------------

* PARAMETER FDATA contains information about emission characteristics of fuels.
* In additon it contains a numerical code identication of the individual fuels.
* Units: kg/GJ

Parameter FDATA(FFF,FUELDATASET)   'Fuel specific values'
/
$INCLUDE "%data_path_in%inc_database\O Par FDATA.inc";
/;
