LOOP ((IR,T)$((IBID_DAYAHEADMARKET_YES EQ 1) $IT_SPOTPERIOD(T)),
   UENBprod_region(IR,T) = SUM(IAGK_Y(IA,IGDISPATCH)$RRRAAA(IR,IA), VGELEC_T.l(T,IA,IGDISPATCH))
                           + ITIDALSTREAM_VAR_T(IR,T) + IWAVE_VAR_T(IR,T) + IRUNRIVER_VAR_T(IR,T) + ISOLAR_VAR_T(IR,T) + IDISGEN_VAR_T(IR,T)
                           - (IVWINDSHEDDING_DAYAHEAD_YES * VWINDSHEDDING_DAY_AHEAD.l(T,IR))
                           + IWIND_BID_IR(IR,T);
                           
   UENBtimeseries_pumphydro(IR,T) = SUM(IAGK_Y(IA,IGESTORAGE_HEATPUMP)$RRRAAA(IR,IA), VGELEC_CONSUMED_T.l(T,IA,IGESTORAGE_HEATPUMP));
);




$ifi '%LFB_Intraday%' ==  Yes  IVXELEC_Phys_T(IRE,IRI,T) $((ORD(T)-1)<= NO_HOURS_OVERLAP) =
$ifi '%LFB_Intraday%' ==  Yes  Sum(IR, PTDF(IR,IRE,IRI) * SUM(IRRRI, VXELEC_T.l(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI)) - PTDF(IR,IRE,IRI) * SUM(IRRRE, VXELEC_T.l(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR)))
$ifi '%LFB_Intraday%' ==  Yes  + VXELEC_T.l(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)
$ifi '%LFB_Intraday%' ==  Yes  - IPTDF_Offset(IRE,IRI,T); 


Loop (IRRRE,
   Loop (IRRRI $Regionengrenzen(IRRRE,IRRRI),
      Loop (T,
          IF (IVXELEC_Phys_T(IRRRE,IRRRI,T)> 0,
              IF (IVXELEC_Phys_T(IRRRE,IRRRI,T) > IXCAPACITY_Y(IRRRE,IRRRI,T),
                 IBOTTLENECK_T(IRRRE,IRRRI,T)       = 1;
                 IBOTTLENECK_menge_T(IRRRE,IRRRI,T) =  IVXELEC_Phys_T(IRRRE,IRRRI,T) - IXCAPACITY_Y(IRRRE,IRRRI,T);
              ELSE
                 IBOTTLENECK_T(IRRRE,IRRRI,T)       = 0;
                 IBOTTLENECK_menge_T(IRRRE,IRRRI,T) = 0;
              );
          ELSE
              IF (-IVXELEC_Phys_T(IRRRE,IRRRI,T) > IXCAPACITY_Y(IRRRI,IRRRE,T),
                 IBOTTLENECK_T(IRRRE,IRRRI,T)       = 1;
                 IBOTTLENECK_menge_T(IRRRE,IRRRI,T) = IVXELEC_Phys_T(IRRRE,IRRRI,T) + IXCAPACITY_Y(IRRRE,IRRRI,T);
              ELSE
                 IBOTTLENECK_T(IRRRE,IRRRI,T)       = 0;
                 IBOTTLENECK_menge_T(IRRRE,IRRRI,T) = 0;
                 );
          );
      );
   );
);

* In day-ahead loop all time steps that are planned day-ahead --> entire next day hours 0-23, i.e. T13-T36
IF ((IBID_DAYAHEADMARKET_YES EQ 1),
  LOOP((IGDISPATCH,T) $IT_SPOTPERIOD(T),
    IPENALTYforRD_T(IGDISPATCH,T) = 40;
  );
);

LOOP((IGDISPATCH,T)$((ORD(T)-1) <= NO_HOURS_OVERLAP),
    LOOP((IRRRE,IRRRI) $Bottleneck_G(IRRRE,IRRRI,IGDISPATCH),
* If there is a bottleneck
        IF (IBOTTLENECK_T(IRRRE,IRRRI,T) = 1,
* Then penalty = 0
           IPENALTYforRD_T(IGDISPATCH,T) = 0;
        );
    );
);
