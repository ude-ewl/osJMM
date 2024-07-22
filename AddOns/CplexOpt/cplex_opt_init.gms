
$LOG "Setting externally given or default values for cplex options. These may be overwritten in the following"

SET opt_cplex_ids 
/ 
  threads, epgap, tilim, scaind, parallelmode, polishtime,
  epopt, eprhs, relaxfixedinfeas, quality, iis,
  iatriggertime, lpmethod
$ifi '%Code_version%'==EWL ,mipstart,rinsheur,heurfreq
/;               

SET opt_cplex_external_value(opt_cplex_ids);
opt_cplex_external_value(opt_cplex_ids) = no;

$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" threads 1
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" tilim 36000
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" epopt 1.0e-6
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" eprhs 1.0E-6
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" relaxfixedinfeas 1
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" quality 1
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" iis 1

$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" epgap 0.001
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" iatriggertime 60
$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" polishtime 60

$batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" lpmethod 0

$ifi '%Code_version%'==EWL $batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" mipstart 1
$ifi '%Code_version%'==EWL $batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" rinsheur 50
$ifi '%Code_version%'==EWL $batinclude "%code_path_addons%\CplexOpt\cplex_opt_set_default" heurfreq 50

SET opt_cplex_category
/  
  epgap, min_run_time, polishing 
  ,UENBValues, EWLValues
/;

PARAMETER cplex_values(opt_cplex_ids);

cplex_values('threads') = %opt_threads%;
cplex_values('epgap') = %opt_epgap%;
cplex_values('tilim') = %opt_tilim%;
cplex_values('scaind') = 1;
cplex_values('parallelmode') = 1;
cplex_values('polishtime') = %opt_polishtime%;
cplex_values('epopt') = %opt_epopt%;
cplex_values('eprhs') = %opt_eprhs%;
cplex_values('relaxfixedinfeas') = %opt_relaxfixedinfeas%;
cplex_values('quality') = %opt_quality%;
cplex_values('iis') = %opt_iis%;
cplex_values('iatriggertime') = %opt_iatriggertime%;
cplex_values('lpmethod') = %opt_lpmethod%;
$ifi '%Code_version%'==EWL cplex_values('mipstart') = %opt_mipstart%;
$ifi '%Code_version%'==EWL cplex_values('rinsheur') = %opt_rinsheur%;
$ifi '%Code_version%'==EWL cplex_values('heurfreq') = %opt_heurfreq%;

PARAMETER cplex_values_do_write(opt_cplex_category);
cplex_values_do_write(opt_cplex_category) = 0;
