$ifi not '%NoOpTimeRestrictions%' == Yes QGONLOPMIP
$ifi not '%NoOpTimeRestrictions%' == Yes QGONLSDMIP

$ifi not '%NoStartUpCosts%' == Yes QGSTARTFUEL
*QESTOLOAD1
QESTOLOAD2

$ifi %UnitCramp%==YES QUC18
*QUC20

*Switch for TSOs set to NO, therefore untested
$ifi %UnitCscc%==YES QUC12
