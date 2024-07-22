* --- Dummy file for calling main file that is located in an (upper-level) folder
* --- Before running check whether the following settings are made in the window right to the run button:
* --- Idir2 ..\..\ Idir3 ..\..\Model


$SETGLOBAL DeMIP NO
$SETGLOBAL debug YES
$SETGLOBAL mg_cur ENTSO
$SETGLOBAL code_path ../
$SETGLOBAL data_path_in ./
$SETGLOBAL data_path_out ./
*$SETGLOBAL opt_threads NA
*$SETGLOBAL opt_tilim NA
$SETGLOBAL DeRD NO

* use 'Sonstige NNE'/ EWL use 'Disgen' --> switch for minor data/code differences
* Code_version: EWL or UENB
$SETGLOBAL Code_version  EWL


$include JMM.gms


* Note:
* Choice file is included directly in the .gms file.
* For this, the choice file must be in the folder of this start file
