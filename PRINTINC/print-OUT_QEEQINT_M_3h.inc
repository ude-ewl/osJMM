* File: print-OUT_QEEQINT_M_3h.inc

* This file will print the marginal value of equation QEEQINT, which is
* interpreted as the electricity price on the intraday market.
* QEEQINT_M depends on the sets RRR, Node and T

* When running the deterministic version of the JMM QEEQINT is taken out
* of the model so QEEQDAY is written out in stead.

PUT OUT_QEEQINT_M_3h_OUT;
OUT_QEEQINT_M_3h_OUT.PW = 32767;
OUT_QEEQINT_M_3h_OUT.PS = 1;
OUT_QEEQINT_M_3h_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,Infotime,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
$if not "%JMM_Det%" == yes   LOOP (IR,
$if not "%JMM_Det%" == yes      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$if not "%JMM_Det%" == yes                       AND (ORD(BASETIME) <= IBASEFINALTIME) ),
$if not "%JMM_Det%" == yes         LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
$if not "%JMM_Det%" == yes            LOOP (R_RId(IR, RId),
$if not "%JMM_Det%" == yes               Put CaseId.TL:0,",",RId.TL:0,",",INFOTIME.TL:0,",",BASETIME.TL:0,",",
$if not "%JMM_Det%" == yes               QEEQINT.M(T,IR):0:4/ ;
$if not "%JMM_Det%" == yes            );
$if not "%JMM_Det%" == yes         );
$if not "%JMM_Det%" == yes      );
$if not "%JMM_Det%" == yes   );

*$if "%JMM_Det%" == yes   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
*$if "%JMM_Det%" == yes      LOOP (IR,
*$if "%JMM_Det%" == yes         LOOP(BASETIME  $( ( ORD(BASETIME) >= (IBASESTARTTIME + 2*NO_HOURS_OVERLAP) )
*$if "%JMM_Det%" == yes                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
*$if "%JMM_Det%" == yes            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
*$if "%JMM_Det%" == yes               LOOP (R_RId(IR, RId),
*$if "%JMM_Det%" == yes                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",QEEQDAY.M(T,IR):0:4/;
*$if "%JMM_Det%" == yes               );
*$if "%JMM_Det%" == yes            );
*$if "%JMM_Det%" == yes         );
*$if "%JMM_Det%" == yes     );
*$if "%JMM_Det%" == yes   );
);
