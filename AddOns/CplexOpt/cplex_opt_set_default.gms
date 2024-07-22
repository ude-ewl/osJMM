* Code to set default $setglobal values 
*
* This file has to be included with $batinclude
*   argument 1: name of the global variable
*   argument 2: default value

*if not set (empty)
* --> variable Type: not defined
* --> scope type: not defined
$ifthen not set opt_%1
$  setglobal opt_%1 "%2"
$  log Option 'opt_%1' not set. Now set to default value '%2'
$else
  opt_cplex_external_value('%1') = yes;
$endif
