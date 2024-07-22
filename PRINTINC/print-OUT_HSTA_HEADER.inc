* File: print-OUT_HSTA_HEADER.inc

PUT HSTA_OUT_HEADER;
HSTA_OUT.PW = 32767;
HSTA_OUT.PS = 1;
HSTA_OUT.nJ = 2;


* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "UnitGroup" /;
* Dispatchable
   LOOP (IAGK_Y(IA,IGDISPATCH),
                LOOP(G(IGDISPATCH),
                                Put G.TL:0 /;
                                                );
                                );
                LOOP (IR,
* Tidal
                                LOOP(IAGK_Y(IA,IGTIDALSTREAM)$ RRRAAA(IR,IA),
                    LOOP (G(IGTIDALSTREAM),
                                                Put G.TL:0 /;
                    );
                                );
* Run of river VRUNRIVERCUR_ANCPOS included
                        LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                                LOOP (G(IGRUNOFRIVER),
                                                Put G.TL:0 /;
                    );
                                );
* Solarth included
                                LOOP(IAGK_Y(IA,IGSOLARTH)$ RRRAAA(IR,IA),
                    LOOP (G(IGSOLARTH),
                                                Put  G.TL:0 /;
                                        );
                );

* Geothheat included
                                LOOP(IAGK_Y(IA,IGGEOTHHEAT)$ RRRAAA(IR,IA),
                    LOOP (G(IGGEOTHHEAT),
                                                Put G.TL:0 /;
                                        );
                                 );


* Solar VSOLARCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGSOLAR)$ RRRAAA(IR,IA),
                                        LOOP (G(IGSOLAR),
                                                Put  G.TL:0 /;
                                        );
                                );

* Biomass VBIOMASSCUR_ANCPOS included
                                LOOP (IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                                        LOOP (G(IGBIOMASS),
                                                Put  G.TL:0 /;
                                        );
                                );
* OthRes
                                LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                    LOOP (G(IGOthRes),
                                                Put  G.TL:0 /;
                    );
                                );

* Wave
                                LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                    LOOP (G(IGWAVE),
                                                Put  G.TL:0 /;
                    );
                );

* Distributed generation
               LOOP(IAGK_Y(IA,IGDISGEN)$ RRRAAA(IR,IA),
                   LOOP (G(IGDISGEN),
                            Put  G.TL:0 /;
                    );
               );

);
                        LOOP (IR,
* Wind Stochastic
                                LOOP (IAGK_Y(IA,IGWIND)$ RRRAAA(IR,IA),
                                        LOOP (G(IGWIND),
                                                Put  G.TL:0 /;
                                        );
                                );
                        );

   Put /;
);
              