* File: print-OUT_EE_DUMP.inc

PUT EE_DUMP_OUT;
EE_DUMP_OUT.PW = 32767;
EE_DUMP_OUT.PS = 1;
EE_DUMP_OUT.nJ = 2;

*----------------------------------------------------------------------------
* Sum up generation for each region over all renewable energy sources and all areas
* to the parameter RES_SUM

* Define initial value of RES_SUM
        RES_SUM(RRR,T)=0;


    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <=  IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,

            LOOP (RRRAAA(IR,IA),

*              Run of river VRUNRIVERCUR_ANCPOS included
                    LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGRUNOFRIVER,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                                                                   IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                                        );
                                                );


*              Solar VSOLARCUR_ANCPOS included
            HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGSOLAR));
                                        LOOP (IAGK_Y(IA,IGSOLAR)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGSOLAR,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                           ISOLAR_VAR_T(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes    -VSOLARCUR_ANCPOS.l(IR,T)
                                                        );
                              );

*            Solarth included
                                        LOOP(IAGK_Y(IA,IGSOLARTH)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGSOLARTH,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                           ISOLARTH_VAR_T(IR,T);
                                                        );
                                                );

*            Geothheat included
                                        LOOP(IAGK_Y(IA,IGGEOTHHEAT)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGGEOTHHEAT,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                           IGEOTHHEAT_VAR_T(IR,T);
                                                        );
                                                );


*              Wave
                                        LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGWAVE,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                                                                   IWAVE_VAR_T(IR,T);
                                                        );
                                        );



*              Biomass VBIOMASSCUR_ANCPOS included
                                        LOOP (IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGBIOMASS,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                                                                   IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)
                                                        );
                                         );



*                               OthRes
                                        LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                                                LOOP (G_GId(IGOthRes,GId),
                                                        RES_SUM(IR,T)=RES_SUM(IR,T) +
                                                                                   IOthRes_VAR_T(IR,T);
                                                        );
                                                );

                                );


*              Wind Stochastic
            LOOP (IR,
            HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGWIND));
                                LOOP (IAGK_Y(IA,IGWIND)$ RRRAAA(IR,IA),
*                       LOOP (G_GId(IGWIND,GId),
                                                RES_SUM(IR,T)=RES_SUM(IR,T) +
                                            IWIND_REALISED_IR(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning%' == Yes              -VWINDCUR_ANCPOS.l(T,IR)
*                       );
                    );
                );





                        );
                );



*--------------------------------------------------------------------
* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "BaseTime",
                LOOP (IR,
                                Put ",",IR.TL:0,"_dump",",",IR.TL:0,"_RES_sum";

* Run of river VRUNRIVERCUR_ANCPOS included
                        LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                                LOOP (G(IGRUNOFRIVER),
                                                Put ",", G.TL:0;
                    );
                                );

* Solarth included
                                LOOP(IAGK_Y(IA,IGSOLARTH)$ RRRAAA(IR,IA),
                    LOOP (G(IGSOLARTH),
                                                Put ",", G.TL:0;
                                        );
                );
* Geothheat included
                                LOOP(IAGK_Y(IA,IGGEOTHHEAT)$ RRRAAA(IR,IA),
                    LOOP (G(IGGEOTHHEAT),
                                                Put ",", G.TL:0;
                                        );
                                 );


* Solar VSOLARCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGSOLAR)$ RRRAAA(IR,IA),
                                        LOOP (G(IGSOLAR),
                                                Put ",", G.TL:0;
                                        );
                                );


* Biomass VBIOMASSCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                                        LOOP (G(IGBIOMASS),
                                                Put ",", G.TL:0;
                                        );
                                );
* OthRes
                                LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                    LOOP (G(IGOthRes),
                                                Put ",", G.TL:0;
                    );
                                );

* Wave
                                LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                    LOOP (G(IGWAVE),
                                                Put ",", G.TL:0;
                    );
                );
                );
                        LOOP (IR,
* Wind Stochastic
                                LOOP (IAGK_Y(IA,IGWIND)$ RRRAAA(IR,IA),
                                        LOOP (G(IGWIND),
                                                Put ",", G.TL:0;
                                        );
                                );
                        );

   Put /;
);

*-------------------------------------------------------------------------------
* print out variables and parameters depending on Unit Groups and T

* allocate realised wind power production, unregulated hydropower production and
* solar production to variable HSTA, such that they can be used in the
* output database


    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
          LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
                      Put BASETIME.TL:0,

                                LOOP (IR,
* Dump power
                                                LOOP(IPLUSMINUS('IPLUS'),
                                                        Put ",", (VQDAYAHEAD.L(T,IR,'IPLUS') + VWINDSHEDDING_DAY_AHEAD.L(T,IR)
+ VSOLARSHEDDING_DAY_AHEAD.L(T,IR)
):0:2;
                                                        );

* Renewable energy sources total generation
                                                        Put ",",RES_SUM(IR,T):0:2;

* Run of river VRUNRIVERCUR_ANCPOS included
                        LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                                LOOP (G_GId(IGRUNOFRIVER,GId),
                                                if((VGELEC_CONSUMED_T.l    (T,IA,IGRUNOFRIVER)
                                                + VGELEC_CONSUMED_DPOS_T.l(T,IA,IGRUNOFRIVER)
                                                - VGELEC_CONSUMED_DNEG_T.l(T,IA,IGRUNOFRIVER))<=0,
                                                Put ",",(IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                                        ):0:2 ;
                                                );
                    );
                                );

* Solarth included
                                LOOP(IAGK_Y(IA,IGSOLARTH)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGSOLARTH,GId),
                         Put ",", ISOLARTH_VAR_T(IR,T):0:2 ;
                                        );
                );

* Geothheat included
                                LOOP(IAGK_Y(IA,IGGEOTHHEAT)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGGEOTHHEAT,GId),
                         Put ",", IGEOTHHEAT_VAR_T(IR,T):0:2 ;
                                        );
                );

* Solar VSOLARCUR_ANCPOS included
            HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGSOLAR));
                                LOOP (IAGK_Y(IA,IGSOLAR)$ RRRAAA(IR,IA),
                                        LOOP (G_GId(IGSOLAR,GId),
                                                Put ",",(ISOLAR_VAR_T(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes    -VSOLARCUR_ANCPOS.l(IR,T)
                               ):0:2 ;
                                        );
                                );


* Biomass VBIOMASSCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                                        LOOP (G_GId(IGBIOMASS,GId),
                       Put ",", (IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)
                         ):0:2;

                                        );
                                );
* OthRes
                                LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGOthRes,GId),
                         Put ",", IOthRes_VAR_T(IR,T) :0:2 ;
                    );
                                );

* Wave
                                LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGWAVE,GId),
                         Put ",",IWAVE_VAR_T(IR,T) :0:2 ;
                    );
                );
                );
                        LOOP (IR,
            HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGWIND));
* Wind Stochastic
                                LOOP (IAGK_Y(IA,IGWIND)$ RRRAAA(IR,IA),
                                        LOOP (G_GId(IGWIND,GId),
                       Put ",",(IWIND_REALISED_IR(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning%' == Yes      -VWINDCUR_ANCPOS.l(T,IR)
                         ):0:2 ;
                                        );
                                );
                        );

Put /; );
  );


*-------------------------------------------------------------------------------
