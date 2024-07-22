* File: print-OUT_VOBJ_withoutSlacks.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_withoutSlacks_OUT;
OUT_VOBJ_withoutSlacks_OUT.PW = 32767;
OUT_VOBJ_withoutSlacks_OUT.PS = 1;
OUT_VOBJ_withoutSlacks_OUT.nJ = 2;

VOBJtmp = VOBJ.L - (


* Infeasibility penalties:
* #(VQDAYAHEAD)
   + SUM(IR $(IBID_DAYAHEADMARKET_YES EQ 1),
        SUM(T $IT_SPOTPERIOD(T),
            IPENALTY_ENS * VQDAYAHEAD.L(T,IR,'IMINUS') + IPENALTY_DUM * VQDAYAHEAD.L(T,IR,'IPLUS')
           )
      )

* #(VQINTRADAY)
   + SUM(IR,
        SUM(T $IT_OPT(T),
            IPENALTY * (VQINTRADAY.L(T,IR,'IMINUS') + VQINTRADAY.L(T,IR,'IPLUS'))
           )
      )

* #(VQMAXCAP)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        SUM(T $IT_OPT(T),
            IPENALTYCAP *
            VQMAXCAP.L(T,IA,IGDISPATCH,'IPLUS')
           )
      )

* #(VQBOUNDEMAX, VQBOUNDEMIN)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        SUM(T $IT_OPT(T),
              IPENALTYBOUNDE * VQBOUNDEMAX.L(T,IA,IGDISPATCH,'IPLUS')
            + IPENALTYBOUNDE * VQBOUNDEMIN.L(T,IA,IGDISPATCH,'IMINUS')
           )
      )      

* ##(VQONLOP)
* in the following line, VQONLOP is penalised with IPENALTY,
* Within the Printout for VOBJ_R_T_Inclslack, IPENALTYVQONLOP is used (see "choice"-file)
  +  SUM(IAGK_Y(IA,IGMINOPERATION_NOALWAYS),
        SUM(T_WITH_HIST $(ORD(T_WITH_HIST) LE (24 + INO_HOURS_OPT_PERIOD + 1)),
            IPENALTY* VQONLOP.L(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IPLUS')
           )
      )

* #(VQANCPOSEQ, VQANCNEGEQ)
    + SUM((TSO_RRR,T),
        IPENALTY_SpinRes * (VQANCPOSEQ.L(T,TSO_RRR,'IMINUS') + VQANCNEGEQ.L(T,TSO_RRR,'IMINUS'))     
     )

* #(PTDFCAP_SLACK)
$ifi '%LFB_Intraday%' ==  Yes   +  Sum((IRE,IRI),
$ifi '%LFB_Intraday%' ==  Yes        SUM(T,IPENALTYPTDF * PTDFCAP_SLACK.L(IRE,IRI,T)
$ifi '%LFB_Intraday%' ==  Yes        )
$ifi '%LFB_Intraday%' ==  Yes      )

$ifi '%LFB_DayAhead%' ==  Yes   +  Sum(TRL,
$ifi '%LFB_DayAhead%' ==  Yes        SUM(T,IPENALTYPTDF * PTDF_SLACK_FLG.L(TRL,T)
$ifi '%LFB_DayAhead%' ==  Yes        )
$ifi '%LFB_DayAhead%' ==  Yes      )

* #(VQHEQ)
  +  SUM(IA,
        SUM(T $IT_OPT(T),
           IPENALTYHEAT * (VQHEQ.L(T,IA,'IPLUS')+ VQHEQ.L(T,IA,'IMINUS'))
        )
      )

* #(VQHEQNATGAS)
$ifi Not '%DeMIP%'==Yes  +  SUM(IA,
$ifi Not '%DeMIP%'==Yes        SUM(T $IT_OPT(T),
$ifi Not '%DeMIP%'==Yes           IPENALTYHEAT * VQHEQNATGAS.L(T,IA,'IMINUS')
$ifi Not '%DeMIP%'==Yes           )
$ifi Not '%DeMIP%'==Yes         )

* #(VQNONSP_ANCPOSEQ)
   +  SUM(TSO_RRR,
        SUM(T $IT_OPT(T),
           IPENALTY1 * VQNONSP_ANCPOSEQ.L(T,TSO_RRR,'IMINUS')
           )
      )




);

*$offtext

*display VOBJtmp;

*$ontext
* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     Put CaseId.TL:0,",",INFOTIME.TL:0,",",VOBJtmp:0:4/ ;
);
*$offtext
