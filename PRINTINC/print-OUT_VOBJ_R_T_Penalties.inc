* File: print-OUT_VOBJ_R_T_Penalties.inc

PUT OUT_VOBJ_R_T_Penalties_Out;
OUT_VOBJ_R_T_Penalties_OUT.PW = 32767;
OUT_VOBJ_R_T_Penalties_OUT.PS = 1;
OUT_VOBJ_R_T_Penalties_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),

  VOBJ_Penalties_Out(IR,t)

  =

* Infeasibility penalties:
* (VQDAYAHEAD)
  +          IPENALTY_ENS * VQDAYAHEAD.L(T,IR,'IMINUS') + IPENALTY_DUM * VQDAYAHEAD.L(T,IR,'IPLUS')

* (VQINTRADAY)
   +         IPENALTY * (VQINTRADAY.L(T,IR,'IMINUS') + VQINTRADAY.L(T,IR,'IPLUS'))

* (VQMAXCAP)
   + SUM(IAGK_Y(IA,IGDISPATCH),
            IPENALTYCAP * VQMAXCAP.L(T,IA,IGDISPATCH,'IPLUS')

* on to see better infeasibilities of fixed heat generation
            + IPENALTYCAP * VQMINCAP.L(T,IA,IGDISPATCH,'IMINUS')
      )

* on to see better infeasibilities of fixed heat generation
$ontext
   + SUM(IAGK_Y(IA,IGHEAT),
            IPENALTY * VQMAXCAPHEAT.L(T,IA,IGHEAT,'IPLUS')
      )
$offtext

* (VQBOUNDEMAX, VQBOUNDEMIN)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        
              IPENALTYBOUNDE * VQBOUNDEMAX.l(T,IA,IGDISPATCH,'IPLUS')
            + IPENALTYBOUNDE * VQBOUNDEMIN.l(T,IA,IGDISPATCH,'IMINUS')
           
      )

* #(VQONLOP)
* in the following line, VQONLOP is penalised with IPENALTY,
* Within the Printout for VOBJ_R_T_Inclslack, IPENALTYVQONLOP is used (see "choice"-file)
  +  SUM(IAGK_Y(IA,IGMINOPERATION_NOALWAYS),
            IPENALTY *
*$ifi     '%UnitCmin%' == Yes     IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T)*
IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T)*
            (VQONLOP.L(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS') + VQONLOP.L(T,IA,IGMINOPERATION_NOALWAYS,'IMINUS'))
      )

* #(VQANCPOSEQ, VQANCNEGEQ)
    + SUM(TSO_RRR,
        IPENALTY_SpinRes * (VQANCPOSEQ.L(T,TSO_RRR,'IPLUS') + VQANCPOSEQ.L(T,TSO_RRR,'IMINUS'))
      + IPENALTY_SpinRes * (VQANCNEGEQ.L(T,TSO_RRR,'IPLUS') + VQANCNEGEQ.L(T,TSO_RRR,'IMINUS'))
     )

* #(PTDFCAP_SLACK)
$ifi '%LFB_Intraday%' ==  Yes        IPENALTYPTDF * PTDFCAP_SLACK.L(IRE,IRI,T)


$ifi '%LFB_NE%' ==  Yes   +  Sum(TRL,
$ifi '%LFB_NE%' ==  Yes        IPENALTYPTDF * (PTDF_SLACK_FLG.L(TRL,T,'IPLUS') + PTDF_SLACK_FLG.L(TRL,T,'IMINUS'))
$ifi '%LFB_NE%' ==  Yes      )

* #(VQHEQ)
  +  SUM(IA,
           IPENALTYHEAT * (VQHEQ.L(T,IA,'IPLUS')+ VQHEQ.L(T,IA,'IMINUS'))
      )

* #(VQHEQNATGAS)
$ifi Not '%DeRD%'==Yes  +  SUM(IA,
$ifi Not '%DeRD%'==Yes           IPENALTYHEAT * VQHEQNATGAS.L(T,IA,'IMINUS')
$ifi Not '%DeRD%'==Yes         )

* #(VQNONSP_ANCPOSEQ)
   +  SUM(TSO_RRR,
           IPENALTY1 * (VQNONSP_ANCPOSEQ.L(T,TSO_RRR,'IPLUS')+VQNONSP_ANCPOSEQ.L(T,TSO_RRR,'IMINUS'))
      )

* #(VQNONSP_ANCNEGEQ)
   +  SUM(TSO_RRR,
           IPENALTY1 * (VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IPLUS')+VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IMINUS'))
      )

* #(VQESTOLOAD1)
$ifi '%UnitCmin%'==Yes  + SUM(IAGK_Y(IA,IGELECSTORAGE),
$ifi '%UnitCmin%'==Yes               IPENALTY * (VQESTOLOAD1.L(T,IA,IGELECSTORAGE,'IMINUS') + VQESTOLOAD1.L(T,IA,IGELECSTORAGE,'IPLUS'))
$ifi '%UnitCmin%'==Yes    )

* P�nale f�r Redispatch von Kraftwerken, die sich nicht in der N�he des Engpasses befinden
$ifi '%DeRD%'==Yes  + SUM(IAGK_Y(IA,IGDISPATCH),
$ifi '%DeRD%'==Yes        IPENALTYforRD_T(IGDISPATCH,T) *
$ifi '%DeRD%'==Yes         (
$ifi '%DeRD%'==Yes             VGELEC_DPOS_T.L(T,IA,IGDISPATCH) + VGELEC_DNEG_T.L(T,IA,IGDISPATCH)
$ifi '%DeRD%'==Yes             + (VGELEC_CONSUMED_DPOS_T.L(T,IA,IGDISPATCH) + VGELEC_CONSUMED_DNEG_T.L(T,IA,IGDISPATCH))$IGELECSTORAGE(IGDISPATCH)
$ifi '%DeRD%'==Yes         )
$ifi '%DeRD%'==Yes    )

* P�nale f�r Windabschaltung

+ IPENALTYWINDCURT *
            (
$ifi '%Renewable_Spinning%' == Yes           VWINDCUR_ANCPOS.L(T,IR) +
            VWINDSHEDDING_DAY_AHEAD.L(T,IR))

+  IPENALTY_SOLAR *
            VSOLARSHEDDING_DAY_AHEAD.L(T,IR)

* Penalty for large ramps
  + SUM(IAGK_Y(IA,IGDISPATCH),
        IPENALTYRAMP *
        (VGRAMP_UP.L(T,IA,IGDISPATCH) + VGRAMP_DOWN.L(T,IA,IGDISPATCH))
    )

   + SUM(IA,
            IHydro_Fill*VHYDROSPILLAGE_NEG.L(T,IA)
           )


    + SUM(IA,
            IHydro_Dum*VHYDROSPILLAGE.L(T,IA)
           )

* to have adequate references for rel. gap when variable costs are close to zero
*only valid for MIP runs
*TKMK$ifi '%UnitCmin%' == Yes + 100000
;
* end loop (IR,t
);




if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP(IR,
            LOOP (R_RId(IR,RId),
               Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                  VOBJ_Penalties_Out(IR,t):0:4/ ;
            );
         );
      );
   );
);
