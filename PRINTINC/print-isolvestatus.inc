* File: print-solvestatus.inc

* This file will print the solve status of each run, the time used in each
* solve, and some error messages depending on the activation of dummy plants

*-----------------------------------------------------------------------------

* print solve status and time consumption

PUT ISOLVESTATUS_OUT;
ISOLVESTATUS_OUT.PW = 32767;
ISOLVESTATUS_OUT.PS = 1;
ISOLVESTATUS_OUT.nJ = 2;

ERRORCOUNT =0;
WARNINGCOUNT=0;


* First print headings:

if ((INO_SOLVE EQ 0),
PUT "Version:              " SYSTEM.TITLE /;
PUT "File name:             isolvestatus.out" /;
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



*-----------------------------------------------------------------------------
* print error message if infeasibility in electricity balance equation on the day
* ahead market QEEQDAY
LOOP (IR $(IBID_DAYAHEADMARKET_YES EQ 1),
   LOOP(BASETIME  $( (ORD(BASETIME) >= IBASESTARTTIME)
                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
        LOOP(T  $( (ORD(T)-1 = (ORD(BASETIME)-IBASESTARTTIME+1) ) AND IT_SPOTPERIOD(T)),
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
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
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

*-----------------------------------------------------------------------------
* print error message if infeasibility in CCGT equations (QGCCGT_STEAM or QGCCGT_ST)

$ifi '%CCGT_Imp%' == Yes LOOP (CCGT_Group,
$ifi '%CCGT_Imp%' == Yes    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$ifi '%CCGT_Imp%' == Yes                          AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
$ifi '%CCGT_Imp%' == Yes           LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
$ifi '%CCGT_Imp%' == Yes               if(VQCCGT.l(T,CCGT_Group,'IPLUS')>1,
$ifi '%CCGT_Imp%' == Yes                    Put "Error VQCCGT is infeasible ";
$ifi '%CCGT_Imp%' == Yes                    Put "VQCCGT_IPLUS: ", VQCCGT.l(T,CCGT_Group,'IPLUS'):0:6
$ifi '%CCGT_Imp%' == Yes                    Put ", CCGT_Group: "CCGT_Group.TL:0
$ifi '%CCGT_Imp%' == Yes                    Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
$ifi '%CCGT_Imp%' == Yes                    ERRORCOUNT=ERRORCOUNT+1;
$ifi '%CCGT_Imp%' == Yes               );
$ifi '%CCGT_Imp%' == Yes               if(VQCCGT.l(T,CCGT_Group,'IMINUS')>1,
$ifi '%CCGT_Imp%' == Yes                    Put "Error VQCCGT is infeasible ";
$ifi '%CCGT_Imp%' == Yes                    Put "VQCCGT_IMINUS: ", VQCCGT.l(T,CCGT_Group,'IMINUS'):0:6
$ifi '%CCGT_Imp%' == Yes                    Put ", CCGT_Group: "CCGT_Group.TL:0
$ifi '%CCGT_Imp%' == Yes                    Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
$ifi '%CCGT_Imp%' == Yes                    ERRORCOUNT=ERRORCOUNT+1;
$ifi '%CCGT_Imp%' == Yes               );
$ifi '%CCGT_Imp%' == Yes         );
$ifi '%CCGT_Imp%' == Yes     );
$ifi '%CCGT_Imp%' == Yes );

*-----------------------------------------------------------------------------
* print error message if infeasibility in heat balance equation QHEQ

LOOP (IA,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

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
* print error message if infeasibility in heat balance equation for natural gas driven heat suppliers QHEQNATGAS

$ifi Not '%DeMIP%'==Yes LOOP (IA,
$ifi Not '%DeMIP%'==Yes    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$ifi Not '%DeMIP%'==Yes                          AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
$ifi Not '%DeMIP%'==Yes         LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

$ifi Not '%DeMIP%'==Yes            if(VQHEQNATGAS.l(T,IA,'IMINUS')>0,
$ifi Not '%DeMIP%'==Yes                 Put "Error QHEQNATGAS is infeasible ";
$ifi Not '%DeMIP%'==Yes                 Put "VQHEQNATGAS_IMINUS: ", VQHEQNATGAS.l(T,IA,'IMINUS'):0:6
$ifi Not '%DeMIP%'==Yes                 Put ", In area: "IA.TL:0",Basetime period: "BASETIME.TL:0
$ifi Not '%DeMIP%'==Yes                 Put " Time: "T.TL:0//;
$ifi Not '%DeMIP%'==Yes                 ERRORCOUNT=ERRORCOUNT+1;
$ifi Not '%DeMIP%'==Yes            );
$ifi Not '%DeMIP%'==Yes      );
$ifi Not '%DeMIP%'==Yes   );
$ifi Not '%DeMIP%'==Yes );



*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for positive primary reserve

LOOP (TSO_RRR,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

      if(VQANCPOSEQ.l(T,TSO_RRR,'IPLUS')>0,
        Put "Error QANCPOSEQ is infeasible:  ";
        Put "VQANCPOSEQ_IPLUS: ", VQANCPOSEQ.l(T,TSO_RRR,'IPLUS'):0:6
        Put ", In TSO_region: "TSO_RRR.TL:0", Basetime period: "BASETIME.TL:0
        Put " Time: "T.TL:0//;
        ERRORCOUNT=ERRORCOUNT+1;
      );

            if(VQANCPOSEQ.l(T,TSO_RRR,'IMINUS')>0,
        Put "Error QANCPOSEQ is infeasible:  ";
        Put "VQANCPOSEQ_IMINUS: ", VQANCPOSEQ.l(T,TSO_RRR,'IMINUS'):0:6
        Put ", In TSO_region: "TSO_RRR.TL:0", Basetime period: "BASETIME.TL:0
        Put " Time: "T.TL:0//;
        ERRORCOUNT=ERRORCOUNT+1;
      );

      );
   );
);


*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for negative primary reserve

LOOP (TSO_RRR,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

     if(VQANCNEGEQ.l(T,TSO_RRR,'IPLUS')>0,
        Put "Error QANCNEGEQ is infeasible:  ";
        Put "VQANCNEGEQ_IPLUS: ", VQANCNEGEQ.l(T,TSO_RRR,'IPLUS'):0:6
        Put ", In TSO-region: "TSO_RRR.TL:0", Basetime period: "BASETIME.TL:0
        Put " Time: "T.TL:0//;
        ERRORCOUNT=ERRORCOUNT+1;
      );

      if(VQANCNEGEQ.l(T,TSO_RRR,'IMINUS')>0,
        Put "Error QANCNEGEQ is infeasible:  ";
        Put "VQANCNEGEQ_IMINUS: ", VQANCNEGEQ.l(T,TSO_RRR,'IMINUS'):0:6
        Put ", In TSO-region: "TSO_RRR.TL:0", Basetime period: "BASETIME.TL:0
        Put " Time: "T.TL:0//;
        ERRORCOUNT=ERRORCOUNT+1;
      );
    );
  );
);





*-----------------------------------------------------------------------------
* print error message if infeasibility in balance equation for replacement reserve

LOOP (TSO_RRR,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

            if(VQNONSP_ANCPOSEQ.l(T,TSO_RRR,'IPLUS')>0,
                 Put "Error QNONSP_ANCPOSEQ is infeasible ";
                 Put "VQNONSP_ANCPOSEQ_IPLUS: ", VQNONSP_ANCPOSEQ.l(T,TSO_RRR,'IPLUS'):0:6
                 Put ", In region: "TSO_RRR.TL:0",Basetime period: "BASETIME.TL:0
                 Put " Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );

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

*--------------------------------------------------------------------------
* print error message if infeasibility in maximum capacity inequality QGMAXCAPDISPATCH
$ontext
$ifi Not '%CHP%'==Yes  LOOP (IAGK_Y(IA,IGDISPATCH),
$ifi Not '%CHP%'==Yes     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$ifi Not '%CHP%'==Yes                           AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
$ifi Not '%CHP%'==Yes          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

$ifi Not '%CHP%'==Yes              if(VQMAXCAP.l(T,IA,IGDISPATCH,'IPLUS')>0,
$ifi Not '%CHP%'==Yes                   Put "Error VQMAXCAP is infeasible ";
$ifi Not '%CHP%'==Yes                   Put "VQMAXCAP_IPLUS: ", VQMAXCAP.l(T,IA,IGDISPATCH,'IPLUS'):0:6
$ifi Not '%CHP%'==Yes                   Put ", In area: "IA.TL:0", UnitGroup: "IGDISPATCH.TL:0
$ifi Not '%CHP%'==Yes                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
$ifi Not '%CHP%'==Yes                   ERRORCOUNT=ERRORCOUNT+1;
$ifi Not '%CHP%'==Yes              );
$ifi Not '%CHP%'==Yes        );
$ifi Not '%CHP%'==Yes     );
$ifi Not '%CHP%'==Yes  );
$offtext

LOOP (IAGK_Y(IA,IGDISPATCH),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

              if(VQMAXCAP.l(T,IA,IGDISPATCH,'IPLUS')>1,
                   Put "Error VQMAXCAP is infeasible ";
                   Put "VQMAXCAP_IPLUS: ", VQMAXCAP.l(T,IA,IGDISPATCH,'IPLUS'):0:6
                   Put ", In area: "IA.TL:0", UnitGroup: "IGDISPATCH.TL:0
                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                   ERRORCOUNT=ERRORCOUNT+1;
              );
        );
    );
);

LOOP (IAGK_Y(IA,IGDISPATCH),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

              if(VQBOUNDEMAX.l(T,IA,IGDISPATCH,'IPLUS')>1,
                   Put "Error VQBOUNDEMAX is infeasible ";
                   Put "VQBOUNDEMAX_IPLUS: ", VQBOUNDEMAX.l(T,IA,IGDISPATCH,'IPLUS'):0:6
                   Put ", In area: "IA.TL:0", UnitGroup: "IGDISPATCH.TL:0
                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                   ERRORCOUNT=ERRORCOUNT+1;
              );
        );
    );
);


LOOP (IAGK_Y(IA,IGDISPATCH),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

              if(VQBOUNDEMIN.l(T,IA,IGDISPATCH,'IMINUS')>1,
                   Put "Error VQBOUNDEMIN is infeasible ";
                   Put "VQBOUNDEMIN_IMINUS: ", VQBOUNDEMIN.l(T,IA,IGDISPATCH,'IMINUS'):0:6
                   Put ", In area: "IA.TL:0", UnitGroup: "IGDISPATCH.TL:0
                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                   ERRORCOUNT=ERRORCOUNT+1;
              );
        );
    );
);

* additional slacks to analyze infeasibilities in CHP-modelling
  LOOP (IAGK_Y(IA,IGDISPATCH),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                           AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

              if(VQMINCAP.l(T,IA,IGDISPATCH,'IMINUS')>1,
                   Put "Error VQMINCAP is infeasible ";
                   Put "VQMINCAP_IMINUS: ", VQMINCAP.l(T,IA,IGDISPATCH,'IMINUS'):0:6
                   Put ", In area: "IA.TL:0", UnitGroup: "IGDISPATCH.TL:0
                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                   ERRORCOUNT=ERRORCOUNT+1;
              );
        );
     );
  );

$ontext
  LOOP (IAGK_Y(IA,IGHEAT),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                           AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
          LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

              if(VQMAXCAPHEAT.l(T,IA,IGHEAT,'IPLUS')>0,
                   Put "Error VQMAXCAPHEAT is infeasible ";
                   Put "VQMAXCAPHEAT_IPLUS: ", VQMAXCAPHEAT.l(T,IA,IGHEAT,'IPLUS'):0:6
                   Put ", In area: "IA.TL:0", UnitGroup: "IGHEAT.TL:0
                   Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0
                   Put ", Heat: "VGHEAT_T.l(T,IA,IGHEAT):0:6", Capacity: "IGHEATCAPACITY_Y(IA,IGHEAT):0:6//;
                   ERRORCOUNT=ERRORCOUNT+1;
              );
        );
     );
  );
$offtext


*--------------------------------------------------------------------------
* print error message if infeasibility in QGONLOP

LOOP (IAGK_Y(IA,IGMINOPERATION_NOALWAYS),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

            if(VQONLOP.l(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS')>0.0001,
                 Put "Error QGONLOP is infeasible ";
                 Put "VQONLOP_IPLUS: ", VQONLOP.l(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS'):0:6
                 Put ", In area: "IA.TL:0", UnitGroup: "IGMINOPERATION_NOALWAYS.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );

      );
   );
);
LOOP (IAGK_Y(IA,IGUC_NOALWAYS),
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

            if(VQONLOP.l(T,IA,IGUC_NOALWAYS,'IMINUS')>0.0001,
                 Put "Error QGONLSD is infeasible ";
                 Put "VQONLOP_IMINUS: ", VQONLOP.l(T,IA,IGUC_NOALWAYS,'IMINUS'):0:6
                 Put ", In area: "IA.TL:0", UnitGroup: "IGUC_NOALWAYS.TL:0
                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
                 ERRORCOUNT=ERRORCOUNT+1;
            );
      );
   );
);


*-----------------------------------------------------------------------------
* print error message if infeasibility in equation QXKPTDF
* print error message if PTDF restrictions cannot be fulfilled
$ontext
$ifi '%LFB_DayAhead%' ==  Yes    LOOP (ISFD_Y_FLG(FLG),
$ifi '%LFB_DayAhead%' ==  Yes       LOOP(BASETIME  $( (ORD(BASETIME) >= IBASESTARTTIME)
$ifi '%LFB_DayAhead%' ==  Yes                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
$ifi '%LFB_DayAhead%' ==  Yes         LOOP(T  $( (ORD(T)-1 = (ORD(BASETIME)-IBASESTARTTIME+1) ) AND IT_SPOTPERIOD(T)),
$ifi '%LFB_DayAhead%' ==  Yes             IF(PTDF_SLACK_FLG.l(FLG,T)>0.0001,
$ifi '%LFB_DayAhead%' ==  Yes                     Put "Error in LFB_DayAhead restrictions: " ;
$ifi '%LFB_DayAhead%' ==  Yes                     Put "PTDF_SLACK_FLG: ", PTDF_SLACK_FLG.l(FLG,T):0:2
$ifi '%LFB_DayAhead%' ==  Yes                     Put ", for flowgate: "FLG.TL:0,"Basetime period: "BASETIME.TL:0
$ifi '%LFB_DayAhead%' ==  Yes                     Put " Time: "T.TL:0//;
$ifi '%LFB_DayAhead%' ==  Yes                     ERRORCOUNT=ERRORCOUNT+1;
$ifi '%LFB_DayAhead%' ==  Yes             );
$ifi '%LFB_DayAhead%' ==  Yes          );
$ifi '%LFB_DayAhead%' ==  Yes       );
$ifi '%LFB_DayAhead%' ==  Yes    );
$offext
*QXCAPFBMCPlus(T,ISFD_Y_FLG(FLG))$IT_OPT(T)..
*IXCAPACITY_FLG_SFD(FLG,T) + PTDF_SLACK_FLG(FLG,T,'IPLUS')
*;


$ifi '%LFB%' ==  Yes    LOOP (ISFD_Y_FLG(FLG),
$ifi '%LFB%' ==  Yes       LOOP(BASETIME  $( (ORD(BASETIME) >= IBASESTARTTIME)
$ifi '%LFB%' ==  Yes                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
$ifi '%LFB%' ==  Yes         LOOP(T  $(ORD(T)-1 = (ORD(BASETIME)-IBASESTARTTIME+1)),

$ifi '%LFB%' ==  Yes             IF(PTDF_SLACK_FLG.l(FLG,T,'IPLUS')>0.0001,
$ifi '%LFB%' ==  Yes                     Put "Error in LFB restrictions (SFD): " ;
$ifi '%LFB%' ==  Yes                     Put "PTDF_SLACK_FLG: ", PTDF_SLACK_FLG.l(FLG,T,'IPLUS'):0:2
$ifi '%LFB%' ==  Yes                     Put ", for flowgate: "FLG.TL:0,"Basetime period: "BASETIME.TL:0
$ifi '%LFB%' ==  Yes                     Put " Time: "T.TL:0//;
$ifi '%LFB%' ==  Yes                     ERRORCOUNT=ERRORCOUNT+1;
$ifi '%LFB%' ==  Yes             );
$ifi '%LFB%' ==  Yes          );
$ifi '%LFB%' ==  Yes       );
$ifi '%LFB%' ==  Yes    );

*QXCAPFBMCMinus(T,INSFD_Y_FLG(FLG))$IT_OPT(T)..
*IXCAPACITY_FLG_NSFD(FLG,T) - PTDF_SLACK_FLG(FLG,T,'IMINUS')
*;

$ifi '%LFB%' ==  Yes    LOOP (INSFD_Y_FLG(FLG),
$ifi '%LFB%' ==  Yes       LOOP(BASETIME  $( (ORD(BASETIME) >= IBASESTARTTIME)
$ifi '%LFB%' ==  Yes                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
$ifi '%LFB%' ==  Yes         LOOP(T  $(ORD(T)-1 = (ORD(BASETIME)-IBASESTARTTIME+1)),

$ifi '%LFB%' ==  Yes             IF(PTDF_SLACK_FLG.l(FLG,T,'IMINUS')>0.0001,
$ifi '%LFB%' ==  Yes                     Put "Error in LFB restrictions (NSFD): " ;
$ifi '%LFB%' ==  Yes                     Put "PTDF_SLACK_FLG: ", PTDF_SLACK_FLG.l(FLG,T,'IMINUS'):0:2
$ifi '%LFB%' ==  Yes                     Put ", for flowgate: "FLG.TL:0,"Basetime period: "BASETIME.TL:0
$ifi '%LFB%' ==  Yes                     Put " Time: "T.TL:0//;
$ifi '%LFB%' ==  Yes                     ERRORCOUNT=ERRORCOUNT+1;
$ifi '%LFB%' ==  Yes             );
$ifi '%LFB%' ==  Yes          );
$ifi '%LFB%' ==  Yes       );
$ifi '%LFB%' ==  Yes    );



*$ifi '%UnitCmin%'==Yes $INCLUDE '%code_path_addons%/Mip/MIP_Print_isolvestatus.inc'
*--------------------------------------------------------------------------
* print error message if infeasibility in QESTOLOAD1

$ontext
$ifi '%UnitCmin%'==Yes LOOP (IAGK_Y(IA,IGELECSTORAGE),
$ifi '%UnitCmin%'==Yes   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$ifi '%UnitCmin%'==Yes                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
$ifi '%UnitCmin%'==Yes        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
$ifi '%UnitCmin%'==Yes            if(VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IMINUS')>0,
$ifi '%UnitCmin%'==Yes                 Put "Error QESTOLOAD1 is infeasible ";
$ifi '%UnitCmin%'==Yes                 Put "VQESTOLOAD1_IMINUS: ", VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IMINUS'):0:6
$ifi '%UnitCmin%'==Yes                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECSTORAGE.TL:0
$ifi '%UnitCmin%'==Yes                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
$ifi '%UnitCmin%'==Yes                 ERRORCOUNT=ERRORCOUNT+1;
$ifi '%UnitCmin%'==Yes            );
$ifi '%UnitCmin%'==Yes            if(VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IPLUS')>0,
$ifi '%UnitCmin%'==Yes                 Put "Error QESTOLOAD1 is infeasible ";
$ifi '%UnitCmin%'==Yes                 Put "VQESTOLOAD1_IPLUS: ", VQESTOLOAD1.l(T,IA,IGELECSTORAGE,'IPLUS'):0:6
$ifi '%UnitCmin%'==Yes                 Put ", In area: "IA.TL:0",UnitGroup: "IGELECSTORAGE.TL:0
$ifi '%UnitCmin%'==Yes                 Put ", Basetime period: "BASETIME.TL:0", Time: "T.TL:0//;
$ifi '%UnitCmin%'==Yes                 ERRORCOUNT=ERRORCOUNT+1;
$ifi '%UnitCmin%'==Yes            );
$ifi '%UnitCmin%'==Yes      );
$ifi '%UnitCmin%'==Yes   );
$ifi '%UnitCmin%'==Yes );
$offtext

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


* The same information without the detailed error and warnings messages is printed
* to SOLVESTATUS.txt:

PUT SOLVESTATUS_OUT;
SOLVESTATUS_OUT.PW = 32767;
SOLVESTATUS_OUT.nJ = 2;

* First print headings:

if ((INO_SOLVE EQ 0),
   PUT "Version:              " SYSTEM.TITLE /;
   PUT "File name:             solvestatus.txt" /;
   PUT "File created:         " SYSTEM.DATE " " SYSTEM.TIME /;
   PUT "Case-identification:  " OCASEID.TS /;
   PUT "SOLVE STATUS: "/;
);
   PUT /;

IF (ISTATUSSOLV=1,
  PUT INFOTIME.tl:14,";","The run was ended normally"/;
  EndTime=jnow;
  hour   = ghour(EndTime);
  minute = gminute(EndTime);
  second = gsecond(EndTime);
  PUT "The run ended","  ",hour,":",minute,":",second/;
* #KD  PUT wilmarbase1.numnz /;
);
* .numnz gives the number of nonzero entries in the model coefficient matrix.

IF (ISTATUSSOLV=2,
  PUT INFOTIME.tl:14,";","The run was not ended normally"/;
  EndTime=jnow;
  hour   = ghour(EndTime);
  minute = gminute(EndTime);
  second = gsecond(EndTime);
  PUT "The run ended","  ",hour,":",minute,":",second/;
*  PUT wilmarbase1.numnz /;
);
IF (ISTATUSSOLV=3,
  PUT INFOTIME.tl:14,";", "The run was interrupted because the resource limit was exceeded"/;
  EndTime=jnow;
  hour   = ghour(EndTime);
  minute = gminute(EndTime);
  second = gsecond(EndTime);
  PUT "The run ended","  ",hour,":",minute,":",second/;
*  PUT wilmarbase1.numnz /;
);
IF (ISTATUSMOD=1,
  PUT INFOTIME.tl:14,";","The optimal solution was found"/;
 );
IF (ISTATUSMOD=2,
  PUT INFOTIME.tl:14,";","The optimal solution was NOT found"/;
);
IF (ISTATUSMOD=4,
  PUT INFOTIME.tl:14,";","The problem is infeasible"/;
);

IF((ERRORCOUNT GT 0),
   IF ((ERRORCOUNT EQ 1),
      PUT " An error was detected after optimisation of period " INFOTIME.tl:14 /;
   ELSE
      PUT ERRORCOUNT:5:0 " errors were detected after optimisation of period " INFOTIME.tl:14 /;
   );
   PUT "See the file ISOLVESTATUS.txt for details about errors."
   PUT //;
);

IF((WARNINGCOUNT GT 0),
   IF ((WARNINGCOUNT EQ 1),
      PUT " A warning was detected after optimisation of period " INFOTIME.tl:14 /;
   ELSE
      PUT WARNINGCOUNT:5:0 " warnings were detected after optimisation of period " INFOTIME.tl:14 /;
   );
   PUT "See the file ISOLVESTATUS.txt for details about warnings."
   PUT //;
);
