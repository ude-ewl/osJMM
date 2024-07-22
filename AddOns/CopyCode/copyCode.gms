* DESCRIPTION
*
* This AddOn copies the current code in a subdirectory of the output directory. In order 
* to ensure that the code in the output directory the code used for the last simulation
* the directory containing the code is removed before copying (if it exists).
*
* Requirements: The code has to be located in a directory called 'base'
*
* Warning: If a file or directory of previously copied code is open, the operations fail
* and it is not guaranteed that the used code is the most recent.
*
* BEWARE: The directory 'code_used' in the output directory is deleted on start of JMM


* Copy the JMM version to out path. The file verinfo.txt is automatically generated be git if the hooks
* are set up correctly.
$CALL ="%gams.sysdir%gbin\rm.exe" -f %data_path_out%JMM_VERSION.txt
$CALL ="%gams.sysdir%gbin\cp.exe" -f %code_path%..\verinfo.txt %data_path_out%JMM_VERSION.txt

* Copy the JMM code to  out path. This operation only succeeds if the %code_path% points to a directory 
* called 'base'. The code is deleted on startup to ensure that the code is the code used for 
* calculation
$CALL ="%gams.sysdir%gbin\rm.exe" -rf %data_path_out%code_used ; sleep 1s

$IFTHENI %DoCopyCode% == yes
$ CALL sleep 3s

$ CALL ="%gams.sysdir%gbin\mkdir.exe" -p %data_path_out%code_used
  
  FILE fWarningCodeUsed / "%data_path_out%\code_used\WARNING_THIS_DIR_WIL_GET_DELETED" /;
  PUT fWarningCodeUsed, "!!! Warning: The content of this directory is ONLY for reference!" //;
  PUT fWarningCodeUsed, "The content of this directory documents the code used in the last model " /;
  PUT fWarningCodeUsed, "run. It is therefore deleted the next time the model is started in order "/;
  PUT fWarningCodeUsed, "to ensure that the content is always the code used to obtain the results " /;
  PUT fWarningCodeUsed, "in the parent folder" /;
  PUTCLOSE  fWarningCodeUsed;

$ CALL ="%gams.sysdir%gbin\cp.exe" -rfp %code_path%..\base\* %data_path_out%code_used
$ENDIF
