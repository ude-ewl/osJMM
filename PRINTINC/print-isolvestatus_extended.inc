* File: print-solvestatus_extended.inc

* This file will print the solve status of each run, the time used in each
* solve, and some error messages depending on the activation of dummy plants

*-----------------------------------------------------------------------------

* print solve status and time consumption

PUT ISOLVESTATUS_EXTENDED_OUT;
ISOLVESTATUS_EXTENDED_OUT.PW = 32767;
ISOLVESTATUS_EXTENDED_OUT.PS = 1;
ISOLVESTATUS_EXTENDED_OUT.nJ = 2;

ERRORCOUNT =0;
WARNINGCOUNT=0;


* First print headings:

if ((INO_SOLVE EQ 0),
PUT "Version:              " SYSTEM.TITLE /;
PUT "File name:              isolvestatus_extended.out" /;
PUT "File created:         " SYSTEM.DATE " " SYSTEM.TIME /;
PUT "Case-identification:  " OCASEID.TS //;
PUT "SOLVE STATUS: "/;
PUT //;


);

EndTime=jnow;
hour   = ghour(EndTime);
minute = gminute(EndTime);
second = gsecond(EndTime);
PUT INFOTIME.tl:14,";"," The run ended","  ",hour,":",minute,":",second/;
put INFOTIME.tl:14,";",' Optimality status text:    ' WILMARBASE1.Tmodstat /;
put INFOTIME.tl:14,";",' Solver status text:        ' WILMARBASE1.Tsolstat /;
PUT INFOTIME.tl:14,";",' Number of unbounded:       ' wilmarbase1.numinfes /;
PUT INFOTIME.tl:14,";",' Number of infeasibilities: ' wilmarbase1.numinfes //;
$ontext
*-----------------------------------------------------------------------------
* print error message if infeasibility in equation QGNEGDEV
LOOP (IAGK_Y(IA,IGCON_HYRES_ELSTO),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
         AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGNEGDEV.l(IA,IGCON_HYRES_ELSTO,T,'IMINUS')>0,
                 Put "Error QGNEGDEV is infeasible: " ;
                 Put "VQGNEGDEV_IMINUS: ", VQGNEGDEV.l(T,IA,IGCON_HYRES_ELSTO,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0", UnitGroup: "IGCON_HYRES_ELSTO.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);


*-----------------------------------------------------------------------------
* print error message if infeasibility in equation QXK
LOOP (IEXPORT_Y(IRE,IRI),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQXK.l(IRI,IRE,T,'IMINUS')>0,
                 Put "Error QXK is infeasible: " ;
                 Put "VQXK_IMINUS: ", VQXK.l(IRI,IRE,T,'IMINUS'):0:2
                 Put ", between region: "IRE.TL:0" and region:"IRI.TL:0,"Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in equation QXK2
LOOP (IEXPORT_Y(IRE,IRI)$(IBID_DAYAHEADMARKET_YES EQ 1),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $((ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)) AND IT_SPOTPERIOD(T)),
            if(VQXK2.l(IRI,IRE,T,'IMINUS')>0,
                 Put "Error QXK2 is infeasible: " ;
                 Put "VQXK2_IMINUS: ", VQXK2.l(IRI,IRE,T,'IMINUS'):0:2
                 Put ", between region: "IRE.TL:0" and region:"IRI.TL:0,"Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);


*-----------------------------------------------------------------------------
* print error message if infeasibility in hydropower capacity equation QHYRSMAXPROD
LOOP (IAHYDRO,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQHYRSMAXPROD.l(IAHYDRO,T,'IMINUS')>0,
                 Put "Error QHYRSMAXPROD is infeasible: " ;
                 Put "VQHYRSMAXPROD_IMINUS: ", VQHYRSMAXPROD.l(IAHYDRO,T,'IMINUS'):0:2
                 Put ", In area: "IAHYDRO.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);


*-----------------------------------------------------------------------------
* print warning message if spillage in hydropower reservoir balance equation QHYRSSEQ
LOOP (IAHYDRO,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME)
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VHYDROSPILLAGE.l(T,IAHYDRO)>0,
                 Put "Warning water is spilled from hydro reservoirs: " ;
                 Put "VHYDROSPILLAGE: ", VHYDROSPILLAGE.l(T,IAHYDRO):0:2
                 Put ", In area: "IAHYDRO.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0"//;
                 WARNINGCOUNT=WARNINGCOUNT+1;
            );
         );
      );
   );
);
$offtext

*-----------------------------------------------------------------------------
* print error message if infeasibility in electricity balance equation on the day
* ahead market QEEQDAY
LOOP (IR$(IBID_DAYAHEADMARKET_YES EQ 1),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME)
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $((ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)) AND IT_SPOTPERIOD(T)),
         if(VQDAYAHEAD.l(T,IR,'IMINUS')>0,
            Put "Error QEEQDAY is infeasible: ";
            Put "VQDAYAHEAD_IMINUS: ", VQDAYAHEAD.l(T,IR,'IMINUS'):0:6
            Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
            Put " Time: "T.TL:0//;
            ERRORCOUNT=ERRORCOUNT+1;
         );
         if(VQDAYAHEAD.l(T,IR,'IPLUS')>0,
            Put "Error QEEQDAY is infeasible"/;
            Put "VQDAYAHEAD_IPLUS: ", VQDAYAHEAD.l(T,IR,'IPLUS'):0:6
            Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
            Put " Time: "T.TL:0//;
            ERRORCOUNT=ERRORCOUNT+1;
         );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in electricity balance equation on the
* intraday market QEEQINT

LOOP (IR,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQINTRADAY.l(T,IR,'IMINUS')>0,
                 Put "Error QEEQINT is infeasible ";
                 Put "VQINTRADAY_IMINUS: ", VQINTRADAY.l(T,IR,'IMINUS'):0:6
                 Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQINTRADAY.l(T,IR,'IPLUS')>0,
                 Put "Error QEEQINT is infeasible: ";
                 Put "VQINTRADAY_IPLUS: ", VQINTRADAY.l(T,IR,'IPLUS'):0:6
                 Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*--------------------------------------------------------------------------
* print error message if infeasibility in QGONLOP

LOOP (IAGK_Y(IA,IGMINOPERATION_NOALWAYS),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQONLOP.l(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS')>0,
                 Put "Error QGONLOP is infeasible ";
                 Put "VQONLOP_IPLUS: ", VQONLOP.l(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGMINOPERATION_NOALWAYS.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);



$ontext
*--------------------------------------------------------------------------
* print error message if infeasibility in QGUC1

LOOP (IAGK_Y(IA,IGUC),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME)
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQUC1.l(IA,IGUC,T,'IMINUS')>0,
                 Put "Error QGUC1 is infeasible ";
                 Put "VQUC1_IMINUS: ", VQUC1.l(IA,IGUC,T,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQUC1.l(IA,IGUC,T,'IPLUS')>0,
                 Put "Error QGUC1 is infeasible ";
                 Put "VQUC1_IPLUS: ", VQUC1.l(IA,IGUC,T,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
         );
      );
   );
);

*--------------------------------------------------------------------------
* print error message if infeasibility in QGUC3

LOOP (IAGK_Y(IA,IGUC),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQUC1.l(IA,IGUC,T,'IMINUS')>0,
                 Put "Error QGUC3 is infeasible ";
                 Put "VQUC3_IMINUS: ", VQUC3.l(IA,IGUC,T,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQUC3.l(IA,IGUC,T,'IPLUS')>0,
                 Put "Error QGUC3 is infeasible ";
                 Put "VQUC3_IPLUS: ", VQUC1.l(IA,IGUC,T,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*--------------------------------------------------------------------------
* print error message if infeasibility in QGUC4

LOOP (IAGK_Y(IA,IGUC),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQUC4.l(T,IA,IGUC,'IMINUS')>0,
                 Put "Error QGUC4 is infeasible ";
                 Put "VQUC4_IMINUS: ", VQUC4.l(T,IA,IGUC,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQUC4.l(T,IA,IGUC,'IPLUS')>0,
                 Put "Error QGUC4 is infeasible ";
                 Put "VQUC4_IPLUS: ", VQUC4.l(T,IA,IGUC,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);
$offtext

*-----------------------------------------------------------------------------
* print error message if infeasibility in heat balance equation QHEQ

LOOP (IA,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQHEQ.l(T,IA,'IMINUS')>0,
                 Put "Error QHEQ is infeasible ";
                 Put "VQHEQ_IMINUS: ", VQHEQ.l(T,IA,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
            if(VQHEQ.l(T,IA,'IPLUS')>0,
                 Put "Error QHEQ is infeasible ";
                 Put "VQHEQ_IPLUS: ", VQHEQ.l(T,IA,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in restriction for minimum number of
* units online



$ontext
LOOP (IA,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
         AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQMINONL.l(IA,T,'IPLUS')>0,
                 Put "Error QMINONL is infeasible ";
                 Put "VQMINONL_IPLUS: ", VQMINONL.l(IA,T,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);
$offtext

*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for replacement reserve

LOOP (TSO_RRR,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQNONSP_ANCPOSEQ.l(T,TSO_RRR,'IMINUS')>0,
                 Put "Error QNONSP_ANCPOSEQ is infeasible ";
                 Put "VQNONSP_ANCPOSEQ_IMINUS: ", VQNONSP_ANCPOSEQ.l(T,TSO_RRR,'IMINUS'):0:6
                 Put ", In region: "TSO_RRR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if demand for spinning reserves is not met
$ontext
LOOP (IR,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
         AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQPOREQ.l(IR,T,'IMINUS')>0,
                 Put "Error spinning reserve reduced ";
                 Put "VQPOREQ_IMINUS: ", VQPOREQ.l(IR,T,'IMINUS'):0:6
                 Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);
$offtext



*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for negative primary reserve
$ontext
LOOP (IR,
  LOOP( (DAY,BASEDAY) $(ORD(BASEDAY) = CEIL((ORD(INFOTIME)/ NO_ROLL_PERIODS_WITHIN_DAY))),
      if(VQANCNEGEQ.l(T,IR,'IMINUS')>0,
        Put "Error QANCNEGEQ is infeasible:  ";
        Put "VQANCNEGEQ_IMINUS: ", VQANCNEGEQ.l(T,IR,'IMINUS'):0:6
        Put ", In region: "IR.TL:0",Baseday : "BASEDAY.TL:0 //;
        ERRORCOUNT=ERRORCOUNT+1;
      );
  );
);
$offtext
*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for positive primary reserve

*LOOP (IR,
*  LOOP( (DAY,BASEDAY) $(ORD(BASEDAY) = CEIL((ORD(INFOTIME)/ NO_ROLL_PERIODS_WITHIN_DAY))),
*      if(VQANCPOSEQ.l(T,IR,'IMINUS')>0,
*        Put "Error QANCPOSEQ is infeasible:  ";
*        Put "VQANCPOSEQ_IMINUS: ", VQANCPOSEQ.l(T,IR,'IMINUS'):0:6
*        Put ", In region: "IR.TL:0",Baseday : "BASEDAY.TL:0 //;
*        ERRORCOUNT=ERRORCOUNT+1;
*      );
*   );
*);

$ontext
*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for negative secondary
* (non-spinning) reserve

LOOP (IR,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQNONSP_ANCNEGEQ.l(T,IR,'IMINUS')>0,
                 Put "Error QNONSP_ANCNEGEQ is infeasible ";
                 Put "VQNONSP_ANCNEGEQ_IMINUS: ", VQNONSP_ANCNEGEQ.l(T,IR,'IMINUS'):0:6
                 Put ", In region: "IR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in ramping rate equation

LOOP (IAGK_Y(IA,IGRAMP),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME)
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGRAMP.l(IA,IGRAMP,T,'IMINUS')>0,
                 Put "Error QGRAMP is infeasible ";
                 Put "VQGRAMP_IMINUS: ", VQGRAMP.l(IA,IGRAMP,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGRAMP.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGONLCAP

LOOP (IAGK_Y(IA,IGUC),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGONLCAP.l(IA,IGUC,T,'IMINUS')>0,
                 Put "Error QGONLCAP is infeasible ";
                 Put "VQGONLCAP_IMINUS: ", VQGONLCAP.l(IA,IGUC,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGUC.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGONLCND2

LOOP (IAGK_Y(IA,IGONL_CONDENSING),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGONLCND2.l(IA,IGONL_CONDENSING,T,'IMINUS')>0,
                 Put "Error QGONLCND2 is infeasible ";
                 Put "VQGONLCND2_IMINUS: ", VQGONLCND2.l(IA,IGONL_CONDENSING,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGONL_CONDENSING.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGBACKPR2

LOOP (IAGK_Y(IA,IGBACKPR),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGBACKPR2.l(IA,IGBACKPR,T,'IMINUS')>0,
                 Put "Error QGBACKPR2 is infeasible ";
                 Put "VQGBACKPR2_IMINUS: ", VQGBACKPR2.l(IA,IGBACKPR,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGBACKPR.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGONLSLOW

LOOP (IAGK_Y(IA,IGALLWAYSRUNNING),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQGONLSLOW.l(IA,IGALLWAYSRUNNING,T,'IMINUS')>0,
                 Put "Error QGRAMP is infeasible ";
                 Put "VQGONLSLOW_IMINUS: ", VQGONLSLOW.l(IA,IGALLWAYSRUNNING,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGALLWAYSRUNNING.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGCAPELEC1 equation

LOOP (IAGK_Y(IA,IGELECNOFLUCTUATION),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
            if(VQCAPELEC1.l(IA,IGELECNOFLUCTUATION,T,'IMINUS')>0,
                 Put "Error QGCAPELEC1 is infeasible ";
                 Put "VQCAPELEC1_IMINUS: ", VQCAPELEC1.l(IA,IGELECNOFLUCTUATION,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECNOFLUCTUATION.TL
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0"//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);

*-----------------------------------------------------------------------------
* print error message if infeasibility in QGCAPELEC3 equation

LOOP (IAGK_Y(IA,IGELECNOFLUCTUATION)$(IBID_DAYAHEADMARKET_YES EQ 1),
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $((ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)) AND IT_SPOTPERIOD(T)),
            if(VQCAPELEC3.l(IA,IGELECNOFLUCTUATION,T,'IMINUS')>0,
                 Put "Error QGCAPELEC3 is infeasible ";
                 Put "VQCAPELEC3_IMINUS: ", VQCAPELEC3.l(IA,IGELECNOFLUCTUATION,T,'IMINUS'):0:2
                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECNOFLUCTUATION.TL
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
       );
   );
);
$offtext
*-----------------------------------------------------------------------------
$ontext
*-----------------------------------------------------------------------------
* print warning messages if fluctuating production is not used for bidding at day ahead market
LOOP (IR,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
        IF( (VWINDSHEDDING_DAY_AHEAD.l(T,IR) GT 0 ),
                  PUT"Warning: Fluctuating plants (wind, run-of-river or solar) reduced production: ";
                  PUT "In region: "IR.TL:0", Basetime period: "BASETIME.TL:0/;
                  PUT" Time: "T.TL:3", VWINDSHEDDING_DAY_AHEAD Value (MW): " VWINDSHEDDING_DAY_AHEAD.L(T,IR):0:2//;
                  WARNINGCOUNT=WARNINGCOUNT+1;
        );
      );
   );
);

*-----------------------------------------------------------------------------
* print warning messages if wind power production have been sheeded
LOOP (IA,
   LOOP (BASETIME $( ( ORD(BASETIME) >= IBASESTARTTIME )
        AND (ORD(BASETIME) <= (IBASEFINALTIME)) ),
      LOOP( T $(ORD(T)-1= (ORD(BASETIME) - IBASESTARTTIME+1)),
          LOOP(IGWIND,
                IF((VGELEC_DNEG_T.L(T,IA,IGWIND) GT 0),
                  PUT"Warning: Fluctuating plants (wind, run-of-river or solar) reduced production: ";
                  PUT"Technology: "IGWIND.TL:0 ", In area: "IA.TL:0", Basetime period: "BASETIME.TL:0/;
                  PUT" Time: "T.TL:0", VGELEC_DNEG_T Value (MW): "VGELEC_DNEG_T.L(T,IA,IGWIND):0:2//;
                  WARNINGCOUNT=WARNINGCOUNT+1;
                );
         );
     );
   );
);
$offtext


$ifi '%JMM_MIP%'==Yes $INCLUDE '%code_path_addons%/Mip/MIP_Print_isolvestatus_BA.inc'

* The conclusion is printed:

IF((ERRORCOUNT GT 0),
   IF ((ERRORCOUNT EQ 1),
      PUT " An error was detected after optimisation of period " INFOTIME.tl:14 /;
   ELSE
      PUT ERRORCOUNT:5:0 " errors were detected after optimisation of period " INFOTIME.tl:14 /;
   );
   PUT //;
);

IF((WARNINGCOUNT GT 0),
   IF ((WARNINGCOUNT EQ 1),
      PUT " A warning was detected after optimisation of period " INFOTIME.tl:14 /;
   ELSE
      PUT WARNINGCOUNT:5:0 " warnings were detected after optimisation of period " INFOTIME.tl:14 /;
   );
   PUT //;
);

