* 'Power output in MW corresponding to the T'th interval of the shut-down process'
*$ifi %UnitCramp%==yes   PARAMETER UCGSHUTD(G,T);
*$ifi %UnitCramp%==yes   UCGSHUTD(G,T)=1000;

*'Power output in MW corresponding to the T'th interval of the start-up process'
*$ifi %UnitCramp%==yes   PARAMETER UCGSTARTU(G,T);
*$ifi %UnitCramp%==yes   UCGSTARTU(G,T)=1000;

$ifi %UnitCscc%==yes   Set i  'Set larger than largest value of GDUCSCCC'  /1*30/;

* Startup costs when unit has been offline for i hours
$ifi %UnitCscc%==yes   PARAMETER UCSCCCOST(G,i)
$ifi %UnitCscc%==yes   /
$ifi %UnitCscc%==yes   $INCLUDE "%data_path_in%/inc_database/O Parameter UCSCCCOST.inc";
$ifi %UnitCscc%==yes   /;


$ifi %UnitCscc%==yes   SET GSTARTCOSTOUTTIME(G) 'Technologies with outtime dependant start-up costs'
$ifi %UnitCscc%==yes   /
$ifi %UnitCscc%==yes   $INCLUDE "%data_path_in%/inc_database/O Set GSTARTCOSTOUTTIME.inc";
$ifi %UnitCscc%==yes   /;

*$ifi %UnitCramp%==yes   GDATA(AAA,G,'GDUCDURD')=1;        /* 'Unit commitment: UnitCramp: duration of shut down process (hours)'   */
*$ifi %UnitCramp%==yes   GDATA(AAA,G,'GDUCDURU')=1;        /* 'Unit commitment: UnitCramp: duration of start up process (hours)'    */

*$ifi %UnitCramp%==yes    GDATA(AAA,G,'GDUCRAMPU')=1000;    /* 'Unit commitment: ramp-up limit (MW/h)'  */
*$ifi %UnitCramp%==yes    GDATA(AAA,G,'GDUCRAMPD')=1000;    /* 'Unit commitment: ramp-DOWN limit (MW/h)'  */