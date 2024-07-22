* File: print-OUT_IDEMANDELEC_REALISED.inc

* This file will print the root node value of the electricity demand
* IDEMANDELEC_REALISED_IR depends on the sets RRR, Node and T

PUT OUT_IDEMANDELEC_REALISED_OUT;
OUT_IDEMANDELEC_REALISED_OUT.PW = 32767;
OUT_IDEMANDELEC_REALISED_OUT.PS = 1;
OUT_IDEMANDELEC_REALISED_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
         LOOP (IR,
            LOOP (R_RId(IR,RId),
                Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                (
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(IGDSM$IAGK_Y(IA,IGDSM), SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST),
$ifi '%FLEX_addon%' == Yes - VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') EQ 31 OR GDATA(IA,IGDSM,'GDTYPE') EQ 32 OR GDATA(IA,IGDSM,'GDTYPE') EQ 33)
$ifi '%FLEX_addon%' == Yes + VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') EQ 34)
$ifi '%FLEX_addon%' == Yes + SUM(ITALIAS_WITH_HIST$(IShiftDSM(IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST)), VGDSMREACT_T.L(IA,IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST))$(GDATA(IA,IGDSM,'GDTYPE') EQ 31 OR GDATA(IA,IGDSM,'GDTYPE') EQ 32)
$ifi '%FLEX_addon%' == Yes ))) +
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), IDEMAND_EV(IA,T_WITH_HIST))) +
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(IGHEAT_NO_STORAGE$IAGK_Y(IA,IGHEAT_NO_STORAGE), VGELEC_CONSUMED_T.L(T,IA,IGHEAT_NO_STORAGE))) +
                  IDEMANDELEC_VAR_T(IR,T)/(1-DISLOSS_E(IR))
                  ):0:4/ ;
              );
         );

$ontext
         LOOP (IR_DET,
            LOOP (R_RId(IR_DET,RId),
                Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                (IDEMANDELEC_BID_IR(IR_DET,T)/(1-DISLOSS_E(IR_DET))):0:4/ ;
            );
         );
$offtext
      );
   );
);
