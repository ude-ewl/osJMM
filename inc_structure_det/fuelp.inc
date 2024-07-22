* File Fuelp.inc
*------------------------------------------------------------------------------

* PARAMETER FUELPRICE_GJ contains fuel prices.
* Units: EUR2002/GJ

* ---- Weekly fuel prices
PARAMETER FUELPRICE_PER_GJ(AAA,FFF,WEEKS)  'Fuel prices (Euro/GJ)'
/
$INCLUDE "%data_path_in%inc_database\O PAR FUELPRICE_GJ Week.inc";
/;

PARAMETER FUELPRICE(AAA,FFF,WEEKS)    'Fuel prices (Euro/MWh)';
FUELPRICE(AAA,FFF,WEEKS)      = 3.6 * FUELPRICE_PER_GJ(AAA,FFF,WEEKS);

* EWL Version
* FUELPRICE(AAA,'DISGEN',WEEKS) = FUELPRICE(AAA,'NAT_GAS',WEEKS);
* FUELPRICE(AAA,'Sonstige_NEE',WEEKS) = FUELPRICE(AAA,'Erdgas',WEEKS);