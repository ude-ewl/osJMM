* File: print-OUT_DEMANDSIDE.inc

PUT DEMANDSIDE_OUT;
DEMANDSIDE_OUT.PW = 32000;
DEMANDSIDE_OUT.PS = 1;
DEMANDSIDE_OUT.nJ = 2;


* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "BaseTime",
 LOOP (IR,

* Base Demand
                                Put ",", IR.TL:0, "_BASEDEMAND";
* Heatpumps
                                Put ",", IR.TL:0, "_HEATPUMPS";
* Electric Vehicles
                                Put ",", IR.TL:0, "_EV";
* DSM
                                Put ",", IR.TL:0, "_DSM";
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

 LOOP (IR,
 
* Base Demand
                                                Put ",",(- IDEMANDELEC_BID_IR(IR,T)):0:2 ;
* Heatpumps
                                                Put ",",
												(SUM(RRRAAA(IR,IA), 
													SUM(IAGK_Y(IA,IGHEAT_NO_STORAGE), 
                                                        - VGELEC_CONSUMED_T.L(T,IA,IGHEAT_NO_STORAGE) 
                                                        - VGELEC_CONSUMED_DPOS_T.L(T,IA,IGHEAT_NO_STORAGE) 
                                                        + VGELEC_CONSUMED_DNEG_T.L(T,IA,IGHEAT_NO_STORAGE)
												))):0:2 ;
* Electric Vehicles
                                                Put ",",
												(SUM(RRRAAA(IR,IA), 
													- IDEMAND_EV(IA,T)
													+ SUM(IAGK_Y(IA,IGDSM)$(GDATA(IA,IGDSM,'GDTYPE') EQ 31),
														 SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), + VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST))
                                                         - SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), SUM(ITALIAS_WITH_HIST, VGDSMREACT_T.L(IA,IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST)))
														 )
												)):0:2 ;
* DSM 
												Put ",",            
												(SUM(RRRAAA(IR,IA), 
													SUM(IAGK_Y(IA,IGDSM)$(GDATA(IA,IGDSM,'GDTYPE') NE 31), 
													     SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST))$(GDATA(IA,IGDSM,'GDTYPE') EQ 32 OR GDATA(IA,IGDSM,'GDTYPE') EQ 33)
													   - SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST))$(GDATA(IA,IGDSM,'GDTYPE') EQ 34)	 
                                                       - SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), SUM(ITALIAS_WITH_HIST, VGDSMREACT_T.L(IA,IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST)))$(GDATA(IA,IGDSM,'GDTYPE') EQ 32)
														)
												)):0:2 ;

 );
Put /; );
  );
);

*-------------------------------------------------------------------------------
