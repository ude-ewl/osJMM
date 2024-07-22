* File: print-juha-OUT_VGELEC.inc

PUT VGELEC_OUT;
VGELEC_OUT.PW = 32767;
VGELEC_OUT.PS = 1;
VGELEC_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);


*-------------------------------------------------------------------------------
* print out variables and parameters depending on AAA, GGG and T

* allocate realised wind power production, unregulated hydropower production and
* solar production to variable vgelec_t, such that they can be used in the
* output database

LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGDISPATCH)$(NOT IGPTG(IGDISPATCH)),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP(G_GId(IGDISPATCH,GId),
               if (vgelec_t.l(T,IA,IGDISPATCH)>0,
                  Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                  VGELEC_T.l(T,IA,IGDISPATCH) :0:4/ ;
               );
            );
         );
      );
   );
);

LOOP (CaseId,
  LOOP (IR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

               LOOP(IAGK_Y(IA,IGRUNOFRIVER) $ RRRAAA(IR,IA),
                    LOOP (G_GId(IGRUNOFRIVER,GId),
                      if ((IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                         )>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         (IRUNRIVER_VAR_T(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes -VRUNRIVERCUR_ANCPOS.l(IR,T)
                                                  ):0:4/ ;
                      );
                    );
               );
*              Solar VSOLARCUR_ANCPOS included
    HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGSOLAR)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGSOLAR));
               LOOP(IAGK_Y(IA,IGSOLAR)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGSOLAR,GId),
                      if((ISOLAR_VAR_T(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes    -VSOLARCUR_ANCPOS.l(IR,T)
                                                )>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         (ISOLAR_VAR_T(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning_legacy%' == Yes    -VSOLARCUR_ANCPOS.l(IR,T)
                                                  ) :0:4/ ;
                      );
                    );
               );
               LOOP(IAGK_Y(IA,IGTIDALSTREAM)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGTIDALSTREAM,GId),
                      if (ITIDALSTREAM_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         ITIDALSTREAM_VAR_T2(IGTIDALSTREAM,IR,T) :0:4/ ;
                      );
                    );
               );
               LOOP(IAGK_Y(IA,IGGEOTHHEAT)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGGEOTHHEAT,GId),
                      if (IGEOTHHEAT_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         IGEOTHHEAT_VAR_T(IR,T) :0:4/ ;
                      );
                    );
               );
*Solarth included
               LOOP(IAGK_Y(IA,IGSOLARTH)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGSOLARTH,GId),
                      if (ISOLARTH_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         ISOLARTH_VAR_T(IR,T) :0:4/ ;
                      );
                    );
               );
*biomass included
               LOOP(IAGK_Y(IA,IGBIOMASS)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGBIOMASS,GId),
                      if (IBIOMASS_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         IBIOMASS_VAR_T(IR,T) :0:4/ ;
                      );
                    );
               );
*OthRes included
               LOOP(IAGK_Y(IA,IGOthRes)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGOthRes,GId),
                      if (IOthRes_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         IOthRes_VAR_T(IR,T) :0:4/ ;
                      );
                    );
               );


               LOOP(IAGK_Y(IA,IGWAVE)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGWAVE,GId),
                      if (IWAVE_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         IWAVE_VAR_T(IR,T) :0:4/ ;
                      );
                    );
               );
               LOOP(IAGK_Y(IA,IGDISGEN)$ RRRAAA(IR,IA),
                    LOOP (G_GId(IGDISGEN,GId),
                      if (IDISGEN_VAR_T(IR,T)>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         IDISGEN_VAR_T2(IR,T) :0:4/ ;
                      );
                    );
               );
           );
       );
  );
);

LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP (IR,
            HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGWIND));
               LOOP(IAGK_Y(IA,IGWIND)$ RRRAAA(IR,IA),
                  LOOP(G_GId(IGWIND,GId),
                   if ((IWIND_REALISED_IR(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning%' == Yes     -VWINDCUR_ANCPOS.l(T,IR)
                                           )>0,
                         Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                         (IWIND_REALISED_IR(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_Spinning%' == Yes     -VWINDCUR_ANCPOS.l(T,IR)
                                                  ):0:4/ ;
                    );
                  );
               );
          );

      );
   );
);


*-------------------------------------------------------------------------------
