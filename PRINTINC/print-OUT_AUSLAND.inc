* File: print-OUT_AUSLAND.inc

PUT AUSLAND_OUT;
AUSLAND_OUT.PW = 32000;
AUSLAND_OUT.PS = 1;
AUSLAND_OUT.nJ = 2;


*-------------------------------------------------------------------------------
* print out variables and parameters depending on GGG and T

* Sum up generation for each region over all generation sources and all areas
* to the parameter GENERSUM

* Define value of GENERSUM
        GENERSUM(IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1) =

*              Dispatchable
                    SUM(IAGK_Y(IA,IGDISPATCH)$RRRAAA(IR,IA),
                                                vgelec_t.l(T,IA,IGDISPATCH)
                                                + vgelec_dpos_t.l(T,IA,IGDISPATCH)
                                                - vgelec_dneg_t.l(T,IA,IGDISPATCH)
                                                - VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH)
                                                - VGELEC_CONSUMED_DPOS_T.l(T,IA,IGDISPATCH)
                                                + VGELEC_CONSUMED_DNEG_T.l(T,IA,IGDISPATCH)
                                                )

*              RunRiver
                 + IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)

*              Solar VSOLARCUR_ANCPOS included
                 + ISOLAR_VAR_T(IR,T)
                                 - SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGSOLAR))
$ifi '%Renewable_Spinning_legacy%' == Yes    -VSOLARCUR_ANCPOS.l(IR,T)

* Solarth included
                 + ISOLARTH_VAR_T(IR,T)

* Geothheat included
                 + IGEOTHHEAT_VAR_T(IR,T)

*              Tidal
                 + SUM(IAGK_Y(IA,IGTIDALSTREAM)$ RRRAAA(IR,IA),
                             ITIDALSTREAM_VAR_T2(IGTIDALSTREAM, IR,T)
                                        )

*              Wave
                + IWAVE_VAR_T(IR,T)

*              Biomass VBIOMASSCUR_ANCPOS included
                + IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)

*                               OthRes
                + IOthRes_VAR_T(IR,T)

*              Distributed generation
                + IDISGEN_VAR_T2(IR,T)

*              Wind Stochastic
                + IWIND_REALISED_IR(IR,T)
                                - SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA), vgelec_dneg_t.l(T,IA,IGWIND))
$ifi '%Renewable_Spinning%' == Yes              -VWINDCUR_ANCPOS.l(T,IR)

;



*-------------------------------------------------------------------------------
* Print out Exchange, Demand and Generation sum per region

* Headline
    if (INO_SOLVE EQ 1,
        Put "BaseTime";
        LOOP (IR,
            Put ",",IR.TL:0,",",IR.TL:0,"_B",",",IR.TL:0,"_E";
             );
        );

    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                        AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
                LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                                        PUT / BASETIME.TL:0;
            LOOP (IR,


* Print Exchange

* Pan-European simulation
            If(IX3country_T_Y(IR,T)=0,
* LOOP WITHIN BASENODE
$ifi '%No_Load_Flow%' ==  Yes                    Put ",",SUM(IRE,VXELEC_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          + VXELEC_DPOS_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          - VXELEC_DNEG_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          - (VXELEC_T.l(T,IRE,IR)
$ifi '%No_Load_Flow%' ==  Yes                          + VXELEC_DPOS_T.l(T,IRE,IR)
$ifi '%No_Load_Flow%' ==  Yes                          - VXELEC_DNEG_T.l(T,IRE,IR))) :0:2;

$ifi '%LFB_NE%' ==  Yes                          Put ",",(SUM(IRE,VXELEC_T.l(T,IR,IRE)
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_DPOS_T.l(T,IR,IRE)
$ifi '%LFB_NE%' ==  Yes                                - VXELEC_DNEG_T.l(T,IR,IRE)
$ifi '%LFB_NE%' ==  Yes                                - (VXELEC_T.l(T,IRE,IR)
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_DPOS_T.l(T,IRE,IR)
$ifi '%LFB_NE%' ==  Yes                                - VXELEC_DNEG_T.l(T,IRE,IR)))
$ifi '%LFB_NE%' ==  Yes                                + SUM(IRE,(VXELEC_DC_T.l(T,IR,IRE)
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_DC_DPOS_T.l(T,IR,IRE)
$ifi '%LFB_NE%' ==  Yes                                - VXELEC_DC_DNEG_T.l(T,IR,IRE))
$ifi '%LFB_NE%' ==  Yes                                - (VXELEC_DC_T.l(T,IRE,IR)
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_DC_DPOS_T.l(T,IRE,IR)
$ifi '%LFB_NE%' ==  Yes                                - VXELEC_DC_DNEG_T.l(T,IRE,IR)))
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_EXPORT_T.l(T,IR)
$ifi '%LFB_NE%' ==  Yes                                + VXELEC_EXPORT_DPOS_T.l(T,IR)
$ifi '%LFB_NE%' ==  Yes                                - VXELEC_EXPORT_DNEG_T.l(T,IR)) :0:2;

$ifi '%LFB_BEX%' ==  Yes                          Put ",",(SUM(IRE,VXELEC_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                + VXELEC_DPOS_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                - VXELEC_DNEG_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                - (VXELEC_T.l(T,IRE,IR)
$ifi '%LFB_BEX%' ==  Yes                                + VXELEC_DPOS_T.l(T,IRE,IR)
$ifi '%LFB_BEX%' ==  Yes                                - VXELEC_DNEG_T.l(T,IRE,IR)))
$ifi '%LFB_BEX%' ==  Yes                                + SUM(IRE,VXELEC_DC_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                + VXELEC_DC_DPOS_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                - VXELEC_DC_DNEG_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes                                - (VXELEC_DC_T.l(T,IRE,IR)
$ifi '%LFB_BEX%' ==  Yes                                + VXELEC_DC_DPOS_T.l(T,IRE,IR)
$ifi '%LFB_BEX%' ==  Yes                                - VXELEC_DC_DNEG_T.l(T,IRE,IR)))
$ifi '%LFB_BEX%' ==  Yes                                ):0:2;

                );
* Country-specific simulation
            if(IX3country_T_Y(IR,T) NE 0,
                Put ",",IX3COUNTRY_T_Y(IR,T):0:2;
               );

* Print Demand
*           LOOP (R_RId(IR,RId),
                LOOP(IPLUSMINUS('IMINUS'),
                                        Put ",",((-1)*(
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(IGDSM$IAGK_Y(IA,IGDSM), SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST),
$ifi '%FLEX_addon%' == Yes                                 - VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') EQ 31 OR GDATA(IA,IGDSM,'GDTYPE') EQ 32 OR GDATA(IA,IGDSM,'GDTYPE') EQ 33)
$ifi '%FLEX_addon%' == Yes                                 + VGDSMACT_T.L(IA,IGDSM,T_WITH_HIST)$(GDATA(IA,IGDSM,'GDTYPE') EQ 34)
$ifi '%FLEX_addon%' == Yes                                 + SUM(ITALIAS_WITH_HIST$(IShiftDSM(IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST)), VGDSMREACT_T.L(IA,IGDSM,ITALIAS_WITH_HIST,T_WITH_HIST))$(GDATA(IA,IGDSM,'GDTYPE') EQ 31 OR GDATA(IA,IGDSM,'GDTYPE') EQ 32)
$ifi '%FLEX_addon%' == Yes ))) +
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(T_WITH_HIST$SAMEAS(T,T_WITH_HIST), IDEMAND_EV(IA,T_WITH_HIST))) +
$ifi '%FLEX_addon%' == Yes SUM(IA$RRRAAA(IR,IA), SUM(IGHEAT_NO_STORAGE$IAGK_Y(IA,IGHEAT_NO_STORAGE), VGELEC_CONSUMED_T.L(T,IA,IGHEAT_NO_STORAGE))) +
                                        IDEMANDELEC_VAR_T(IR,T)/(1-DISLOSS_E(IR))-VQDAYAHEAD.L(T,IR,'IMINUS')
                                        )) :0:2;
                                        );
*               );



* Print Generation sum
                                LOOP(IPLUSMINUS('IPLUS'),
                    Put ",",(GENERSUM(IR,T)-VQDAYAHEAD.L(T,IR,'IPLUS')-VWINDSHEDDING_DAY_AHEAD.L(T,IR)
-VSOLARSHEDDING_DAY_AHEAD.L(T,IR)
):0:2;
                                        );
                                );
                        );
                );

