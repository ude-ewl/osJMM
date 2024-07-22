* File: print-OUT_HSTA.inc

PUT HSTA_OUT;
HSTA_OUT.PW = 32767;
HSTA_OUT.PS = 1;
HSTA_OUT.nJ = 2;


* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "BaseTime",
* Dispatchable
   LOOP (IAGK_Y(IA,IGDISPATCH),
                LOOP(G(IGDISPATCH),
                                Put ",", G.TL:0;
                                                );
                                );
                LOOP (IR,
* Tidal
                                LOOP(IAGK_Y(IA,IGTIDALSTREAM)$ RRRAAA(IR,IA),
                    LOOP (G(IGTIDALSTREAM),
                                                Put ",", G.TL:0;
                    );
                                );
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

* Distributed generation
               LOOP(IAGK_Y(IA,IGDISGEN)$ RRRAAA(IR,IA),
                   LOOP (G(IGDISGEN),
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


LOOP (CaseId,
    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
          LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                      Put BASETIME.TL:0,
* LOOP (GId,
* Dispatchable
            LOOP (IAGK_Y(IA,IGDISPATCH),
                LOOP(G_GId(IGDISPATCH,GId),
                                                Put ",",(vgelec_t.l       (T,IA,IGDISPATCH)
                                                + vgelec_dpos_t.l         (T,IA,IGDISPATCH)
                                                - vgelec_dneg_t.l         (T,IA,IGDISPATCH)
                                                - VGELEC_CONSUMED_T.l     (T,IA,IGDISPATCH)
                                                - VGELEC_CONSUMED_DPOS_T.l(T,IA,IGDISPATCH)
                                                + VGELEC_CONSUMED_DNEG_T.l(T,IA,IGDISPATCH)):0:2 ;
                                );
                        );
                LOOP (IR,

                        LOOP(IPLUSMINUS$IPLUSMINUS('IPLUS'),
                                VQDAYAHEAD_T2(T,IR) = VQDAYAHEAD.l(T,IR,'IPLUS');)

* Tidal
                                LOOP(IAGK_Y(IA,IGTIDALSTREAM)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGTIDALSTREAM,GId),
                         Put ",",  ITIDALSTREAM_VAR_T2(IGTIDALSTREAM, IR,T) :0:2 ;
                    );
                                );

* Run of river VRUNRIVERCUR_ANCPOS included
                        LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                                LOOP (G_GId(IGRUNOFRIVER,GId),
                                                if((VGELEC_CONSUMED_T.l   (T,IA,IGRUNOFRIVER)
                                                + VGELEC_CONSUMED_DPOS_T.l(T,IA,IGRUNOFRIVER)
                                                - VGELEC_CONSUMED_DNEG_T.l(T,IA,IGRUNOFRIVER))<=0,
                                If ((0.5*IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                -VQDAYAHEAD_T2(T,IR))>0,
                                Put ",",(IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                -VQDAYAHEAD_T2(T,IR)) :0:2 ;
                                VQDAYAHEAD_T2(T,IR) = 0;
                                Else
                                Put ",",(0.5*IRUNRIVER_VAR_T(IR,T)) :0:2 ;
                                VQDAYAHEAD_T2(T,IR) = VQDAYAHEAD_T2(T,IR)
                                - (0.5*IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                                                );
                                                        );
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
                                             -VSOLARSHEDDING_DAY_AHEAD.L(T,IR)
                               ):0:2 ;
                                        );
                                );


* Biomass VBIOMASSCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                                        LOOP (G_GId(IGBIOMASS,GId),
                                        If ((0.5*IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)
                                        -VQDAYAHEAD_T2(T,IR))>0,
                       Put ",", (IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)
                                                                -VQDAYAHEAD_T2(T,IR)):0:2;
                                        VQDAYAHEAD_T2(T,IR) = 0;
                                        Else
                                        Put ",",(0.5*IBIOMASS_VAR_T(IR,T)) :0:2 ;
                                        VQDAYAHEAD_T2(T,IR) = VQDAYAHEAD_T2(T,IR)
                                        -(0.5*IBIOMASS_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VBIOMASSCUR_ANCPOS.l(IR,T)
                                                );
                                        );
                                );
                );

* OthRes
               LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGOthRes,GId),
                                                If(0.5*IOthRes_VAR_T(IR,T)-VQDAYAHEAD_T2(T,IR)>0,
                                                Put ",",(IOthRes_VAR_T(IR,T)-VQDAYAHEAD_T2(T,IR)) :0:2;
                                                VQDAYAHEAD_T2(T,IR) = 0;
                                                Else
                                                Put ",",(0.5*IOthRes_VAR_T(IR,T)) :0:2 ;
                                                VQDAYAHEAD_T2(T,IR) = VQDAYAHEAD_T2(T,IR) - 0.5*IOthRes_VAR_T(IR,T);
                                                        );
                                                );
                    );


* Wave
                                LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGWAVE,GId),
                                                If(IWAVE_VAR_T(IR,T)-VQDAYAHEAD_T2(T,IR)>0,
                                                Put ",",(IWAVE_VAR_T(IR,T)-VQDAYAHEAD_T2(T,IR)) :0:2;
                                                VQDAYAHEAD_T2(T,IR) = 0;
                                                Else
                                                Put ",",0 ;
                                                VQDAYAHEAD_T2(T,IR) = VQDAYAHEAD_T2(T,IR) - IWAVE_VAR_T(IR,T); );
                    );
                );



* Distributed generation
               LOOP(IAGK_Y(IA,IGDISGEN)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGDISGEN,GId),
                         Put ",",IDISGEN_VAR_T2(IR,T) :0:2 ;
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
                                        -VWINDSHEDDING_DAY_AHEAD.L(T,IR)
                         ):0:2 ;
                                        );
                                );
                        );

Put /; );
  );
);

*-------------------------------------------------------------------------------
