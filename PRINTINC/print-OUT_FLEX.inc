* File: print-OUT_FLEX.inc

PUT FLEX_OUT;
FLEX_OUT.PW = 32000;
FLEX_OUT.PS = 1;
FLEX_OUT.nJ = 2;


* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "BaseTime",

* Heatpumps
   LOOP (IAGK_Y(IA,IGHEAT_NO_STORAGE),
                LOOP(G(IGHEAT_NO_STORAGE),
                                Put ",", G.TL:0;
                                                );
                                );
* DSM
   LOOP (IAGK_Y(IA,IGDSM),
                LOOP(G(IGDSM),
                                Put ",", G.TL:0, "_down";
                                Put ",", G.TL:0, "_up";
                                                );
                                );
   Put /;
);

*-------------------------------------------------------------------------------
* print out variables and parameters depending on Unit Groups and T

* allocate realised wind power production, unregulated hydropower production and
* solar production to variable HSTA, such that they can be used in the
* output database


LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                      Put BASETIME.TL:0,
* Heatpumps
            LOOP (IAGK_Y(IA,IGHEAT_NO_STORAGE),
                                                Put ",",(- VGELEC_CONSUMED_T.L(T,IA,IGHEAT_NO_STORAGE) 
                                                         - VGELEC_CONSUMED_DPOS_T.L(T,IA,IGHEAT_NO_STORAGE) 
                                                         + VGELEC_CONSUMED_DNEG_T.L(T,IA,IGHEAT_NO_STORAGE)):0:2 ;
                        );
* DSM
            LOOP (IAGK_Y(IA,IGDSM),
                                                Put ",",(SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), 
												            VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') NE 34)
															)):0:2 ;
                                                Put ",",(SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), 
												          - VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') EQ 34)
												          - SUM(ITALIAS_WITH_HIST$(IShiftDSM(IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST)), VGDSMREACT_T.L(IA,IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST))$(GDATA(IA,IGDSM,'GDTYPE') EQ 31 OR GDATA(IA,IGDSM,'GDTYPE') EQ 32)
															)):0:2 ;
                        );

Put /; );
  );
);

*-------------------------------------------------------------------------------
