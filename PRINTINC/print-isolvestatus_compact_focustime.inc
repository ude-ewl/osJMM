* File: print-isolvestatus_compact_focustime.inc

PUT ISOLVESTATUS_COMPACT_OUT_focustime;
ISOLVESTATUS_COMPACT_OUT_focustime.PW = 32767;
ISOLVESTATUS_COMPACT_OUT_focustime.PS = 1;
ISOLVESTATUS_COMPACT_OUT_focustime.nJ = 2;

* -----------------------------------------------------------------------------

* First print headings:

if ((INO_SOLVE EQ 0),
PUT "Version:              " SYSTEM.TITLE /;
PUT "File name:              isolvestatus_compact_focustime.out" /;
PUT "File created:         " SYSTEM.DATE " " SYSTEM.TIME /;
PUT "Case-identification:  " OCASEID.TS //;
PUT "SOLVE STATUS - FOCUS TIME: "/;
PUT //;


);


* -----------------------------------------------------------------------------
* HEAD LINE
if (INO_SOLVE EQ 0,
    Put "Run,Solvestat,Modelstat,DualSolution,NumInfes,Warnings,Errors,etSolver [min], absGap [EUR], relGap [-], ";
	PUT	"Vobj [Mio. EUR], DumpConv [MWh], DumpEE [MWh], Dump [MWh], ENS [MWh],reserveSlacksPos [MWh],reserveSlacksNeg [MWh], HydroSpillagePos [MWh], HydroSpillageNeg [MWh], Dump_NoENTSODump [MWh]";
    Put /;
);
* -----------------------------------------------------------------------------

* LOOP WITHIN BASENODE
* RUN
Put INO_SOLVE:0:0;
Put ",";
*Solvestat
Put Wilmarbase1.solvestat:0:0;
Put ",";
*Modelstat
Put Wilmarbase1.modelstat:0:0;
Put ",";
*DualSolution
Put Wilmarbase1.marginals:0:0;
Put ",";
*NumInfes
Put Wilmarbase1.numInfes:0:0;
Put ",";
*Warnings
PUT WARNINGCOUNT:0:0 ;
Put ",";
*Errors
PUT ERRORCOUNT:0:0 ;
Put ",";
*etSolver
Put (Wilmarbase1.etSolver/60):0:0;
Put ",";
*abs. Gap
Put ((VOBJ.l - Wilmarbase1.objest)):0:0;
Put ",";
*rel. Gap
Put ((VOBJ.l - Wilmarbase1.objest)/max(VOBJ.l,1)):0:3;
Put ",";
*Vobj
Put (VOBJ.l/1000/1000):0:0;
Put ",";
*DumpConv
Put SUM((IR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VQDAYAHEAD.L(T,IR,'IPLUS')):0:3;
Put ",";
*DumpEE
Put SUM((IR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VWINDSHEDDING_DAY_AHEAD.L(T,IR)+VSOLARSHEDDING_DAY_AHEAD.L(T,IR)):0:3;
Put ",";
*DUMP
Put SUM((IR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VQDAYAHEAD.L(T,IR,'IPLUS')+VWINDSHEDDING_DAY_AHEAD.L(T,IR)+VSOLARSHEDDING_DAY_AHEAD.L(T,IR)):0:3;
Put ",";
*ENS
Put SUM((IR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VQDAYAHEAD.L(T,IR,'IMINUS')):0:3;
Put ",";
*reserveSlacksPos: VQNONSP_ANCPOSEQ = Tertiary Control Reserve, VQANCPOSEQ = Primary + Secondary Control Reserve
Put (SUM((TSO_RRR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VQNONSP_ANCPOSEQ.l(T,TSO_RRR,'IMINUS'))+SUM((TSO_RRR,T),VQANCPOSEQ.l(T,TSO_RRR,'IMINUS'))):0:3;
Put ",";
*reserveSlacksNeg: VQANCNEGEQ = Primary + Secondary Control Reserve (Negative Tertiary Control Reserve not implemented in JMM)
Put SUM((TSO_RRR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VQANCNEGEQ.l(T,TSO_RRR,'IMINUS')):0:3;
Put ",";
*HydroSpillagePos
Put SUM((AAa,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VHYDROSPILLAGE.l(T,AAA)):0:3;
Put ",";
*HydroSpillageNeg
Put SUM((AAa,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1),VHYDROSPILLAGE_NEG.l(T,AAA)):0:3;
Put ",";
*Dump_NoENTSODump
Put SUM(SHEDDING_FORBIDDEN_DUE_TO_ENTSO(IR,T)$(ord(t) gt 1 AND ord(t) le NO_HOURS_OVERLAP+1), VQDAYAHEAD.l(T,IR,'IPLUS') + VSOLARSHEDDING_DAY_AHEAD.l(T,IR) +  VWINDSHEDDING_DAY_AHEAD.l(T,IR) ):0:3;
Put/;

* SLACK OVERVIEW ---------------------------------------------

*NTC_CAP_SLACK(IRE,IRI,T) - für No_Load_Flow                                             in keiner Gleichung enthalten
*PTDFCAP_SLACK(IRE,IRI,T) - für PTDF_USE und LFB_Intraday                                fuer BA2017 nicht relevant (in aktuellem isolvestatus_compact noch nicht enthalten)
*VHYDROSPILLAGE(INT,IA) - Hydrospillage                                                  in aktuellem isolvestatus_compact enthalten
*VHYDROSPILLAGE_NEG(INT,IA) - Hydrospillage (NEGATIV)                                    in aktuellem isolvestatus_compact enthalten
*VQANCNEGEQ(Day,IR,'IMINUS') - Regelleistung / für Daily-Looping                         fuer BA2017 nicht relevant (in aktuellem isolvestatus_compact noch nicht enthalten)
*VQANCNEGEQ(T,IR,'IMINUS') - Regelleistung / für Weekly-Looping                          in aktuellem isolvestatus_compact enthalten
*VQANCPOSEQ(Day,IR,'IMINUS') - Regelleistung / für Daily-Looping                         fuer BA2017 nicht relevant
*VQANCPOSEQ(T,IR,'IMINUS') - Regelleistung / für Weekly-Looping                          in aktuellem isolvestatus_compact enthalten
*VQDAYAHEAD(T,IR,'IMINUS') - ENS                                                         in aktuellem isolvestatus_compact enthalten
*VQDAYAHEAD(T,IR,'IPLUS') - Dump                                                         in aktuellem isolvestatus_compact enthalten
*VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IMINUS') -Pumpspeicher                                in JMM auskommentiert
*VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IPLUS') -Pumpspeicher                                 in JMM auskommentiert
*VQHEQ(T,IA,NODE,'IMINUS') - Wärme                                                       in keiner Gleichung enthalten
*VQHEQ(T,IA,NODE,'IPLUS') - Wärme                                                        in keiner Gleichung enthalten
*VQINTRADAY(T,IR,NODE,'IMINUS') - IntraDay Dump                                          fuer BA2017 nicht relevant (in aktuellem isolvestatus_compact noch nicht enthalten)
*VQINTRADAY(T,IR,NODE,'IPLUS') - IntraDay ENS                                            fuer BA2017 nicht relevant (in aktuellem isolvestatus_compact noch nicht enthalten)
*VQMINONL(IA,INT,'IPLUS')                                                                in keiner Gleichung enthalten
*VQNONSP_ANCPOSEQ(T,IR,NODE,'IMINUS') - Regelleistung (Non-Spin)                         in aktuellem isolvestatus_compact enthalten
*VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALLWAYS,NODE,'IPLUS') - Min. Operation          in JMM auskommentiert
*VQONSLD(IA,IGUC_NOALLWAYS,NODE,T_WITH_HIST,'IPLUS') - Min. Downtime                     in JMM auskommentiert
*VQPOREQ(IR,INT,'IMINUS') - Regelleistung                                                in JMM auskommentiert
*VQUC4(T,IA,IGDISPATCH,NODE,'IMINUS') - Kapazitätsrestriktion                            in keiner Gleichung enthalten
*VQUC4(T,IA,IGDISPATCH,NODE,'IPLUS') - Kapazitätsrestriktion                             in JMM auskommentiert
*VSOLARSHEDDING_DAY_AHEAD - Solar Shedding                                               in aktuellem isolvestatus_compact enthalten
*VWINDSHEDDING_DAY_AHEAD - Wind Shedding                                                                                                 in aktuellem isolvestatus_compact enthalten


























