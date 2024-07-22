
*----------------------------------------------------------------------------------------------
*----------------------------------------------------------------------------------------------
* DESCRIPTION

* WARNING:
*   This code assumes that the file cplex.opt is read from the current directory !!!

*----------------------------------------------------------------------------------------------
*----------------------------------------------------------------------------------------------
* set default values if variable is not given

$IF not set opt_cplexfile ABORT cplex.opt is not set.

$LOG "Writing cplex options file: %opt_cplexfile%

*----------------------------------------------------------------------------------------------
* write cplex.opt with name from opt_cplexfile

FILE opt_file_%1 / %opt_cplexfile% /;

PUT opt_file_%1;
opt_file_%1.nr=0; opt_file_%1.nw=12; opt_file_%1.nz=0; opt_file_%1.nj=2;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '* 1) szenario / model run specific settings '/;
PUT '* ---------------------------------------------------------------------------------------'/;
IF( cplex_values_do_write('epgap') ne 0,
  PUT 'epgap ' cplex_values('epgap') /;
ELSE
  PUT ''/;
);
IF( cplex_values_do_write('EWLValues') ne 0,
	PUT 'tilim ' cplex_values('tilim'):0:0 /;
);

IF( cplex_values_do_write('UENBValues') ne 0,
  PUT 'tilim ' cplex_values('tilim'):0:0 /;
  PUT 'threads ' cplex_values('threads'):0:0 /;
ELSE
  PUT ''/;
);
PUT ''/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '* 2) performance settings (parameter tuning might result in performance improvements) '/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT 'scaind ' cplex_values('scaind'):0:0 /;
IF( cplex_values_do_write('UENBValues') ne 0,
   PUT 'parallelmode ' cplex_values('parallelmode'):0:0 /;
ELSE
  PUT ''/;
);
IF( cplex_values_do_write('polishing') ne 0,
  PUT 'polishtime ' cplex_values('polishtime'):0:0 /;
ELSE
  PUT ''/;
);
PUT ''/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '* 3) standard settings (should not be changed) '/;
PUT '* ---------------------------------------------------------------------------------------'/;
IF( cplex_values_do_write('UENBValues') ne 0,
   PUT 'epopt ' cplex_values('epopt') /;
   PUT 'eprhs ' cplex_values('eprhs') /;
ELSE
  PUT ''/;
);
PUT 'relaxfixedinfeas ' cplex_values('relaxfixedinfeas'):0:0 /;
PUT 'names yes'/;
PUT 'quality ' cplex_values('quality'):0:0 /;
PUT 'iis ' cplex_values('iis'):0:0 /;
PUT ''/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '* 4) min-run-time (should not be changed)'/;
PUT '* ---------------------------------------------------------------------------------------'/;
IF( cplex_values_do_write('min_run_time') ne 0,
  PUT 'iatriggertime ' cplex_values('iatriggertime'):0:0 /;
  PUT 'interactive yes' /;
  PUT 'iafile cplex.op2' /;
  PUT 'iatriggerfile readop2.gdx' /;
ELSE
  PUT ''////;
);

PUT ''/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '* 5) additional helpful settings for debugging and performance tests'/;
PUT '* ---------------------------------------------------------------------------------------'/;
PUT '*numericalemphasis 1'/;
PUT '* Might help in case of bad scaling.'/;
PUT ''/;
PUT '* epper 1e-8'/;
PUT '* Might help in case of degeneracy.'/;
PUT ''/;
PUT '* datacheck 2'/;
PUT '* 2 Data checking and model assistance on'/;
PUT ''/;
PUT 'lpmethod ' cplex_values('lpmethod'):0:0 /;
PUT '* 0 Automatic, 1 Primal Simplex, 2 Dual Simplex, 3 Network Simplex, 4 barrier, 5 Sifting, 6 Concurrent'/;
PUT ''/;
PUT '* solutiontype 2'/;
PUT '* primal-dual pair (turning off crossover) 2'/;
PUT ''/;
PUT '* parallelmode -1'/;
PUT '* -1 opportunistic'/;
PUT ''/;
IF( cplex_values_do_write('EWLValues') ne 0,
  PUT '* ---------------------------------------------------------------------------------------'/;
  PUT '* 6) EWL specific Values for Options'/;
  PUT '* ---------------------------------------------------------------------------------------'/;
  PUT 'mipstart ' cplex_values('mipstart') /;
  PUT 'rinsheur ' cplex_values('rinsheur') /;
  PUT 'heurfreq ' cplex_values('heurfreq') /;
ELSE
  PUT ''/;
);

PUTCLOSE opt_file_%1;
