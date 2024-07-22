@echo off
:: Get name for first run and choice file
set default_choice_file=Choice.gms
set /p run_name="Enter run name: "
set /p input_choice_name="Enter name of choice file without extension (keep empty for default file (%default_choice_file%)): "
:: If no name is given for choice file, use default
if "%input_choice_name%"=="" (
    set choice_name=%default_choice_file%
 ) else (
    set choice_name=%input_choice_name%.gms
 )
echo " "
echo Using run name "%run_name%" with choice file %choice_name% 
echo " "
:: copy _default_printout to ../
:: rename _default_printout to Printout
xcopy ".\_default_printout" "..\Printout" /E /I /H
:: Copy _default_run_folder --> "run name"
xcopy "..\Printout\_default_run_folder" "..\Printout\%run_name%" /E /I /H
:: copy one Choice file from _default_Choice_files into "run folder" and name it choice.gms
copy ".\_default_Choice_files\%choice_name%" "..\Printout\%run_name%\choice.gms"
echo "________________________________________________"
echo Created run directory with name %run_name% with choice file %choice_name%
echo " "
echo " "
echo " "
echo Remember to rename this folder to 'model' if you have not already!
pause
