* ' different slopes  of the piecewise linear approximated fuel consumption function'
PARAMETER FE_SLOPE_APPROX(AAA,G,IHRS)
/
$INCLUDE "%data_path_in%inc_database\O Par FE_SLOPE_APPROX.inc";
/;

* ' the right border of the intervall in relation   to the total capacity , e.g. the last right border has to have the value 1'

PARAMETER FE_RIGHTBORDER_APPROX(AAA,G,IHRS)
/
$INCLUDE "%data_path_in%inc_database\O Par FE_RIGHTBORDER_APPROX.inc";
/;
