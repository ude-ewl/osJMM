PARAMETERS
   UCINI(AAA,G)  'Number of hours the unit has been up (>0) or down (<0) at the beginning of a new planning loop'
*   UCMININI_TEMP(AAA,G)  'Number of hours the unit has been up (>0) or down (<0) in NODE_OOO in the lastplanning loop'
   IUCMININIU(AAA,G) 'Unit commitment, UnitCmin: internal parameter, initial remaining uptime'
   IUCMININID(AAA,G) 'Unit commitment, UnitCmin: internal parameter, initial remaining downtime'
   VGONLINE_SUM2(AAA,G)
   VGONLINE_SUM3(AAA,G)
$ifi %UnitCramp%==yes   UCRAMPINI(AAA,G) 'Unit commitment: UnitCramp: Initial state'

;

$ifi %UnitCscc%==yes   Set IGUCSCC(G)  'Set of power plants with outtime dependant start-up times';
$ifi %UnitCscc%==yes   IGUCSCC(G) = GSTARTCOSTOUTTIME(G);