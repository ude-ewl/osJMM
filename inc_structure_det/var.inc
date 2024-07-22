* File Var.inc
*------------------------------------------------------------------------------

* PARAMETER DEF_STEPS describes the elastic electricity demands
* It can either be done in relative terms or in absolute values.
* Unit: (none).
* If using the relative specification quantities are specified relative
* to the nominal demand in the hour in question.
* Thus, with e.q. quantity values of 1.04 and 1.03 for DEF_D2 and DEF_D1,
* respectively, this means that the first step in decrease of demand
* has a magnitude of 3% of demand in a base year
* and the second step has a magnitude of 1%, adding up to 4%.
* With e.q. price values of 0.98 and 0.95 for DEF_U1 and DEF_U2,
* respectively, this means that the first step increase of demand
* corresponds to a price of 98% of nominal and the second step
* corresponds to a price of 95% of nominal.
* Observe that the sequence of quantities should be increasing and the
* sequence of price numbers should be decreasing.

* If using the absolute values, DEF_STEPS_QUANT specifies the length
* of each step in the demand curve and DEF_STEPS_PRICE the price of each
* step in the demand curve. These steps are added/subtracted from the
* nominal demand in each hour, if elastic demand is activated.

* Right now the specification of the demand curve using absolute values
* is used

* Do not change the values in DEF_DINF!

* Data source:

PARAMETER DEF_STEPS_QUANT(RRR,DEF)  'Quanties of elastic electricity demands'
/
$INCLUDE "%data_path_in%inc_database\O Par DEF_STEPS_QUANT.inc";
/
;

PARAMETER DEF_STEPS_PRICE(RRR,DEF)  'Prices of elastic electricity demands'
/
$INCLUDE "%data_path_in%inc_database\O Par DEF_STEPS_PRICE.inc";
/
;
