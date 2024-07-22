* File: print-OUT_VOBJ_R_T_All.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_ALL_OUT;
OUT_VOBJ_R_T_ALL_OUT.PW = 32000;
OUT_VOBJ_R_T_ALL_OUT.PS = 1;
OUT_VOBJ_R_T_ALL_OUT.nJ = 2;


*LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),

   VOBJ_ALL_out(IR,t)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1)

 =

* #1 (VGFUELUSAGE_T)
* Cost of fuel consumption during optimisation period:

   SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$(( GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
        (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUSINGFUEL,T)) * VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)
      )


* #2 (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Operation and maintainance cost:
* attention: seperate calculation of maintentance costs as not for all units if IGELEC vgELEC_DNEG is defined
* ensure that we have one unit for IGRUNOFRIVER per area and one unit IGSOLAR, IGTIDALSTREAM, IGWAVE in each region

  + SUM(IAGK_Y(IA,IGDISPATCH)$RRRAAA(IR,IA),

         GDATA(IA,IGDISPATCH,'GDOMVCOST')*
          (VGELEC_T.L(T,IA,IGDISPATCH) + VGELEC_DPOS_T.L(T,IA,IGDISPATCH) - VGELEC_DNEG_T.L(T,IA,IGDISPATCH))

    )


* #3+4 (VGHEAT_T)
* CHPBF: IAFLEXQ

  + SUM(IAGK_Y(IA,IGHEAT)$RRRAAA(IR,IA),

       (GDATA(IA,IGHEAT,'GDOMVCOST') * GDATA(IA,IGHEAT,'GDCV') * VGHEAT_T.L(T,IA,IGHEAT))$IGELECANDHEAT(IGHEAT)
       + (GDATA(IA,IGHEAT,'GDOMVCOST') * VGHEAT_T.L(T,IA,IGHEAT))$IGHEATONLY(IGHEAT)
    )


* #5 (VGSTARTUP_T) * trkr: Start-up variable costs:
$ifi not '%NoStartUpCosts%' == Yes  + SUM(IAGK_Y(IA,IGUC)$RRRAAA(IR,IA),
$ifi not '%NoStartUpCosts%' == Yes        GDATA(IA,IGUC,'GDSTARTUPCOST')*
*$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes IGELECCAPEFF(IA,IGUC,T)*
$ifi not '%NoStartUpCosts%' == Yes  IGELECCAPEFF(IA,IGUC,T)*
$ifi not '%NoStartUpCosts%' == Yes        VGSTARTUP_T.L(T,IA,IGUC)
$ifi not '%NoStartUpCosts%' == Yes    )


* Transmission cost:

  + SUM(IRE $IEXPORT_Y(IRE,IR),
         XCOST(IRE,IR)*
$ifi '%No_Load_Flow%' ==   Yes   (VXELEC_T.L(T,IRE,IR) + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T.L(T,IRE,IR) -VXELEC_DNEG_T.L(T,IRE,IR)) )
$ifi '%LFB_Intraday%' ==   Yes   (VXELEC_T.L(T,IRE,IR) + VXELEC_DPOS_T.L(T,IRE,IR) + VXELEC_DNEG_T.L(T,IRE,IR))
$ifi '%LFB_DayAhead%' ==   Yes   0
$ifi '%LFB_NE%'       ==   Yes   0
$ifi '%LFB_BEX%'      ==   Yes   (VXELEC_T.L(T,IRE,IR) + ITRANSMISSION_INTRADAY_YES *(VXELEC_DPOS_T.L(T,IRE,IR) -VXELEC_DNEG_T.L(T,IRE,IR)) )
    )


* #7 (VGFUELUSAGE_T)
* Tax on fuel usage for producing power and heat (condensing and chp) in Germany.
* Used fuel * fuel tax * 3.6, which is a multiplier from MWh to GJ:
* not used in DeRD
* IAFLEXQ

  +  SUM(sameas(C,'DE'),
       SUM(IAGK_Y(IA,IGUSINGFUEL)$(IGELECTHERMAL(IGUSINGFUEL) AND ICA(C,IA) AND RRRAAA(IR,IA)),

               GDATA(IA,IGUSINGFUEL,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)
           )
   )


* #8 (VGELEC_CONSUMED_T, VGELEC_CONSUMED_DPOS_T, VGELEC_CONSUMED_DNEG_T)
* Electricity tariff on electricity used for heating (heat pumps and electrical boilers):

   +  SUM(C,
         SUM(IAGK_Y(IA,IGHEATPUMP)$( ICA(C,IA) AND RRRAAA(IR,IA)),
             TAX_HEATPUMP(C)*
               (VGELEC_CONSUMED_T.L(T,IA,IGHEATPUMP) + VGELEC_CONSUMED_DPOS_T.L(T,IA,IGHEATPUMP)- VGELEC_CONSUMED_DNEG_T.L(T,IA,IGHEATPUMP))

         )
      )


* #9 (VGFUELUSAGE_T)
* Emission taxes CO2, SO2:

   +  SUM(C,
         SUM( (IAGK_Y(IA,IGUSINGFUEL),FFF)$( (GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),
               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2') * IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGUSINGFUEL,'GDDESO2'))) * CONVERT_KG_PER_GJ_T_PER_MWH *
               VGFUELUSAGE_T.L(T,IA,IGUSINGFUEL)
         )
      )


* #10 (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Subsidy on electricity produced using biomass or waste: (values of ELEC_SUBSIDY <0)

   +  SUM(C,
         SUM(FFF,
            SUM(IAGK_Y(IA,IGUSINGFUEL)$( (GDATA(IA,IGUSINGFUEL,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND IGELEC(IGUSINGFUEL) AND RRRAAA(IR,IA)),
                 ELEC_SUBSIDY(C,FFF)*
                 (VGELEC_T.L(T,IA,IGUSINGFUEL) + VGELEC_DPOS_T.L(T,IA,IGUSINGFUEL) - VGELEC_DNEG_T.L(T,IA,IGUSINGFUEL))
            )
         )
      )


$ontext
*  Revenue for unit being online:

  - SUM( (IAGK_Y(IA,IGUC),FFF)$( GDATA(IA,IGUC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB') AND RRRAAA(IR,IA)),
     GDATA(IA,IGUC,'GDSTARTUPFUELCONSHOT') * (IFUELPRICE_Y(IA,FFF,T)+ IFUELTRANS_Y(IA,IGUC,T)) *
*$ifi     '%UnitCmin%' == Yes        IGELECCAPEFF(IA,IGUC,T) *
     IGELECCAPEFF(IA,IGUC,T) *
         VGONLINE_T.L(T,IA,IGUC)
     )
$offtext


$ontext
* #12 (VCONTENTHYDRORES_T)
* Revenue for hydroreservoir
  - SUM(IAHYDRO_HYDRORES$(RRRAAA(IR,IAHYDRO_HYDRORES)),
       ISDP_HYDRORES(IAHYDRO_HYDRORES,T)
                                    *  (VCONTENTHYDRORES_T.L(T,IAHYDRO_HYDRORES) - VCONTENTHYDRORES_T.L('T00',IAHYDRO_HYDRORES)
       )

$ifi     '%DeRD%' == Yes              ( ISDP_HYDRORES(IAHYDRO,T) - ISDP_HYDRORES(IAHYDRO,ITALIAS)) * VCONTENTHYDRORES_T.L(T,IAHYDRO)

      )
$ifi '%HydroSupplyCurves%' == YES + sum (IAHYDROGK_Y(IAHYDRO_HYDRORES_STEP,IGHYDRORES)$(RRRAAA(IR,IAHYDRO_HYDRORES_STEP)),
$ifi '%HydroSupplyCurves%' == YES
$ifi '%HydroSupplyCurves%' == YES                 ISDP_HYDRORES_STEP(IAHYDRO_HYDRORES_STEP,IGHYDRORES,T)*(VGELEC_T.L(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES) + VGELEC_DPOS_T.L(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES) - VGELEC_DNEG_T.L(T,IAHYDRO_HYDRORES_STEP,IGHYDRORES))
$ifi '%HydroSupplyCurves%' == YES         )



* Penalty for deviation from reservoir reference level:
* 0-1% =  0.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 1-2% =  1.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* 2-3% =  2.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* ...
* 9-10% = 9.00 EUR/MWh / ((reservoir capacity in full load hours)/1000)
* --> penalty increases (100% -->300%) as storage capacity decreases (1000 flh --> 333 flh), allowing "weekly storages" (low storage capacity) a less flexible operation than "annual storages" (high storage capacity)
$ifi '%HydroPenalty%' == YES   + SUM(IAHYDRO$(RRRAAA(IR,IAHYDRO)),
$ifi '%HydroPenalty%' == YES         sum(IHYDFLXSTEP, VQRESLEVELUP.L(T,IAHYDRO,IHYDFLXSTEP)  *(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES       + sum(IHYDFLXSTEP, VQRESLEVELDOWN.L(T,IAHYDRO,IHYDFLXSTEP)*(ORD(IHYDFLXSTEP)-1)*(max(1,min(3,(SUM(IGHYDRORES,IGELECCAPACITY_Y(IAHYDRO,IGHYDRORES))*1000)/SUM(IGHYDRORES,IGHYDRORESCONTENTCAPACITY_Y(IAHYDRO,IGHYDRORES))))) )
$ifi '%HydroPenalty%' == YES   )

* # 13+14 (VCONTENTSTORAGE_T)

  - SUM(IAGK_Y(IA,IGSTORAGE)$(RRRAAA(IR,IA)),
*storage modeling switched to TSO version for weekly looping
$ifi NOT '%Looping%' == week     (ISDP_STORAGE(IA,IGSTORAGE) *
$ifi     '%Looping%' == week     ((   ISDP_STORAGE(IA,IGSTORAGE)$(ISDP_STORAGE(IA,IGSTORAGE) NE 0)
$ifi     '%Looping%' == week     + ISDP_REFPRICE(IA)$(ISDP_STORAGE(IA,IGSTORAGE) EQ 0)) *
           VCONTENTSTORAGE_T.L(T,IA,IGSTORAGE))
      )
$offtext



* # 15+16 (VDEMANDELECFLEXIBLE_T)
* Changes in consumers utility relative to electricity consumption:

  + IFLEXIBLE_DEF_YES *
    (
          SUM(DEF_D, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_D)* IDEFLEXIBLEPRICE_T(IR,DEF_D))
          - SUM(DEF_U, VDEMANDELECFLEXIBLE_T.L(T,IR,DEF_U)* IDEFLEXIBLEPRICE_T(IR,DEF_U))
    )
*end change



* #17a (constant, operation and maintainance costs of renewables?)
  +
         (
          ITIDALSTREAM_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGTIDALSTREAM) $RRRAAA(IR,IA), GDATA(IA,IGTIDALSTREAM,'GDOMVCOST'))
          + IWAVE_VAR_T(IR,T)      * SUM(IAGK_Y(IA,IGWAVE) $RRRAAA(IR,IA), GDATA(IA,IGWAVE,'GDOMVCOST'))
          + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), GDATA(IA,IGDISGEN,'GDOMVCOST'))
          + IBIOMASS_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + IOthRes_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + ISOLARTH_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGSOLARTH) $RRRAAA(IR,IA), GDATA(IA,IGSOLARTH,'GDOMVCOST'))



$ifi '%Code_version%'==UENB          + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'%Sonstige_NEE%',T)+IFUELTRANS_Y(IA,IGDISGEN,T))  )
$ifi '%Code_version%'==EWL          + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'DISGEN',T)+IFUELTRANS_Y(IA,IGDISGEN,T)) )
          +                          SUM(IAGK_Y(IA,IGRUNOFRIVER) $RRRAAA(IR,IA), GDATA(IA,IGRUNOFRIVER,'GDOMVCOST') * IRUNRIVER_AAA_T(IA,T))
          + ISOLAR_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGSOLAR) $RRRAAA(IR,IA), GDATA(IA,IGSOLAR,'GDOMVCOST'))
          + IGEOTHHEAT_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGGEOTHHEAT) $RRRAAA(IR,IA), GDATA(IA,IGGEOTHHEAT,'GDOMVCOST'))
      )


* #17b (VWINDCUR_ANCPOS, VGELEC_DNEG_T): operation and maintainance costs of production from wind
   +      SUM( IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),
             GDATA(IA,IGWIND,'GDOMVCOST')*
            (IWIND_REALISED_IR(IR,T)  - VGELEC_DNEG_T.L(T,IA,IGWIND)
$ifi '%Renewable_Spinning%' == Yes - VWINDCUR_ANCPOS.L(T,IR)
         )
      )

* #18a (VGSTARTUPFUELCONS and VGSTARTUP_T, resp.)
* Start-up fuel costs: for power plants without outtime dependant start-up fuel consumption
* Set IGNOUCSCC = IGUC if UnitCscc=No. IGNOUCSCC=IGUC-IGUCSCC if UnitCscc=Yes

$ifi not '%NoStartUpCosts%' == Yes  + SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$((GDATA(IA,IGNOUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
$ifi not '%NoStartUpCosts%' == Yes  (IFUELPRICE_Y(IA,FFF,T)  + IFUELTRANS_Y(IA,IGNOUCSCC,T))*
$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes        VGSTARTUPFUELCONS.L(T,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes        GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGNOUCSCC) * IGELECCAPEFF(IA,IGNOUCSCC,T)
$ifi not '%NoStartUpCosts%' == Yes  )

* #18b (VGSTARTUPFUELCONS)
* Start-up fuel costs: for power plants with outtime dependant start-up fuel consumption

$ifi %UnitCscc%==yes   + SUM( (IAGK_Y(IA,IGUCSCC),FFF)$((GDATA(IA,IGUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
$ifi %UnitCscc%==yes      (IFUELPRICE_Y(IA,FFF,T)  + IFUELTRANS_Y(IA,IGNOUCSCC,T))*
$ifi %UnitCscc%==yes         VGSTARTUPFUELCONS.L(T,IA,IGUCSCC))


* #19a (VGSTARTUP_T bzw. VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (see #9):
$ifi not '%NoStartUpCosts%' == Yes   +  SUM(C,
$ifi not '%NoStartUpCosts%' == Yes         SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$( (GDATA(IA,IGNOUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),

$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes  GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGNOUCSCC)*IGELECCAPEFF(IA,IGNOUCSCC,T)*
$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes  VGSTARTUPFUELCONS.L(T,IA,IGNOUCSCC)*
$ifi not '%NoStartUpCosts%' == Yes               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGNOUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi not '%NoStartUpCosts%' == Yes         )
$ifi not '%NoStartUpCosts%' == Yes      )


* #19b (VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (out-time dependant start-up):
$ifi %UnitCscc%==yes   +  SUM(C,
$ifi %UnitCscc%==yes         SUM( (IAGK_Y(IA,IGUCSCC),FFF)$( (GDATA(IA,IGUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),
$ifi %UnitCscc%==yes             VGSTARTUPFUELCONS.L(T,IA,IGUCSCC)*
$ifi %UnitCscc%==yes               (FDATA(FFF,'FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi %UnitCscc%==yes
$ifi %UnitCscc%==yes         )
$ifi %UnitCscc%==yes      )


* #19c (constant)
  +  SUM(C,
          SUM(IAGK_Y(IA,IGDISGEN)$( ICA(C,IA) AND RRRAAA(IR,IA)),
                IDISGEN_VAR_T(IR,T) * (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) *
$ifi '%Code_version%'==EWL         (FDATA('DISGEN','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('DISGEN','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi '%Code_version%'==UENB        (FDATA('%Sonstige_NEE%','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('%Sonstige_NEE%','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
       )
   )


* #20 (VGFUELUSAGE_T)
* Tax reduction on fuel usage for producing power and heat in chp (applied to Germany).
* Fuel use * fuel tax * 3.6, which is a multiplier from GJ to MWH:

  -  SUM(sameas(C,'DE'),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA) AND RRRAAA(IR,IA)),

              GDATA(IA,IGELECANDHEAT,'GDFUELTAXRED')* 3.6 * VGFUELUSAGE_T.L(T,IA,IGELECANDHEAT)
        )
    )




* #21 (VGHEAT_T)
* Tax on fuel usage for producing heat in chp production.
* Produced heat * fuel tax (conversion efficiency from fuel to heat included)
*               * 3.6, which is a multiplier from GJ to MWH. Applies not for Germany:



  +  SUM(C $(Not sameas(C,'DE')),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA) AND RRRAAA(IR,IA)),
              GDATA(IA,IGELECANDHEAT,'GDFUELTAX') * 3.6 * VGHEAT_T.L(T,IA,IGELECANDHEAT)
           )
     )



* #22 (VGHEAT_T)
* Subsidy for CHP Plants (KWK-Zuschlag).
* Produced heat * CHP coefficient * CHP-subsidy

  -  SUM(sameas(C,'DE'),
        SUM(IAGK_Y(IA,IGELECANDHEAT)$(ICA(C,IA) AND RRRAAA(IR,IA)),

               GDATA(IA,IGELECANDHEAT,'GDCB') *
               (GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY1') + GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY2') + GDATA(IA,IGELECANDHEAT,'GDCHP_SUBSIDY3')) *
               VGHEAT_T.L(T,IA,IGELECANDHEAT)
        )
     )


* #23 Tax on fuels of heatboilers (VGFUELUSAGE_T)
* Used fuel * fuel tax * 3.6, which is a multiplier from GJ to MWH:
* IAFLEXQ

  +  SUM(C,
        SUM(IAGK_Y(IA,IGHEATBOILER)$(ICA(C,IA) AND RRRAAA(IR,IA)),
             GDATA(IA,IGHEATBOILER,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T.L(T,IA,IGHEATBOILER)
        )
     )


$ifi '%PtG%' ==  Yes  - SUM(IAGK_Y(IA,IGPTG),
$ifi '%PtG%' ==  Yes     
$ifi '%PtG%' ==  Yes         IUSEVALUE(IA,IGPTG)*(VGELEC_T.L(T,IA,IGPTG) + VGELEC_DPOS_T.L(T,IA,IGPTG) - VGELEC_DNEG_T.L(T,IA,IGPTG))/GDATA(IA,IGPTG,'GDLOADEFF')
$ifi '%PtG%' ==  Yes      
$ifi '%PtG%' ==  Yes    )



* Infeasibility penalties:
* (VQDAYAHEAD)
* Split penalty for VQDAYAHEAD
  +          IPENALTY_ENS * VQDAYAHEAD.L(T,IR,'IMINUS') 
  +          IPENALTY_DUM * VQDAYAHEAD.L(T,IR,'IPLUS')

* #(VQINTRADAY)
   +         IPENALTY * (VQINTRADAY.L(T,IR,'IMINUS') + VQINTRADAY.L(T,IR,'IPLUS'))

* #(VQMAXCAP)
   + SUM(IAGK_Y(IA,IGDISPATCH)$(RRRAAA(IR,IA)),
            IPENALTYCAP * VQMAXCAP.L(T,IA,IGDISPATCH,'IPLUS')

* # on to see better infeasibilities of fixed heat generation
            + IPENALTYCAP * VQMINCAP.L(T,IA,IGDISPATCH,'IMINUS')
      )

* # on to see better infeasibilities of fixed heat generation
$ontext
   + SUM(IAGK_Y(IA,IGHEAT)$(RRRAAA(IR,IA)),
            IPENALTY * VQMAXCAPHEAT.L(T,IA,IGHEAT,'IPLUS')
      )
$offtext

* (VQBOUNDEMAX, VQBOUNDEMIN)
   + SUM(IAGK_Y(IA,IGDISPATCH),
        
              IPENALTYBOUNDE * VQBOUNDEMAX.L(T,IA,IGDISPATCH,'IPLUS')
            + IPENALTYBOUNDE * VQBOUNDEMIN.L(T,IA,IGDISPATCH,'IMINUS')
           
      )

* #(VQONLOP)
* in the following line, VQONLOP is penalised with IPENALTY,
* Within the Printout for VOBJ_R_T_Inclslack, IPENALTYVQONLOP is used (see "choice"-file)
  +  SUM(IAGK_Y(IA,IGMINOPERATION_NOALWAYS)$(RRRAAA(IR,IA)),
            IPENALTY *
IGELECCAPEFF(IA,IGMINOPERATION_NOALWAYS,T)*
            (VQONLOP.L(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS') + VQONLOP.L(T,IA,IGMINOPERATION_NOALWAYS,'IMINUS'))
      )

* #(VQANCPOSEQ, VQANCNEGEQ)
    + SUM(TSO_RRR$(TSO_RRRRRR(TSO_RRR,IR)),
        IPENALTY_SpinRes * (VQANCPOSEQ.L(T,TSO_RRR,'IPLUS') + VQANCPOSEQ.L(T,TSO_RRR,'IMINUS'))
      + IPENALTY_SpinRes * (VQANCNEGEQ.L(T,TSO_RRR,'IPLUS') + VQANCNEGEQ.L(T,TSO_RRR,'IMINUS'))
     )

* (PTDFCAP_SLACK)
$ifi '%LFB_Intraday%' ==  Yes        IPENALTYPTDF * PTDFCAP_SLACK.L(IRE,IRI,T)


$ifi '%LFB%' ==  Yes      +  Sum(TRL,
$ifi '%LFB%' ==  Yes           IPENALTYPTDF * (PTDF_SLACK_FLG.L(TRL,T,'IPLUS') + PTDF_SLACK_FLG.L(TRL,T,'IMINUS'))
$ifi '%LFB%' ==  Yes           )

* (VQHEQ)
  +  SUM(IA$(RRRAAA(IR,IA)),
           IPENALTYHEAT * (VQHEQ.L(T,IA,'IPLUS')+ VQHEQ.L(T,IA,'IMINUS'))
      )

* (VQHEQNATGAS)
$ifi Not '%DeRD%'==Yes  +  SUM(IA$(RRRAAA(IR,IA)),
$ifi Not '%DeRD%'==Yes           IPENALTYHEAT * VQHEQNATGAS.L(T,IA,'IMINUS')
$ifi Not '%DeRD%'==Yes         )

* (VQNONSP_ANCPOSEQ)
   +  SUM(TSO_RRR$(TSO_RRRRRR(TSO_RRR,IR)),
           IPENALTY1 * (VQNONSP_ANCPOSEQ.L(T,TSO_RRR,'IPLUS')+VQNONSP_ANCPOSEQ.L(T,TSO_RRR,'IMINUS'))
      )
* (VQNONSP_ANCNEGEQ)
   +  SUM(TSO_RRR$(TSO_RRRRRR(TSO_RRR,IR)),
           IPENALTY1 * (VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IPLUS')+VQNONSP_ANCNEGEQ.L(T,TSO_RRR,'IMINUS'))
      )

* (VQESTOLOAD1)
$ifi '%UnitCmin%'==Yes  + SUM(IAGK_Y(IA,IGELECSTORAGE)$(RRRAAA(IR,IA)),
$ifi '%UnitCmin%'==Yes               IPENALTY * (VQESTOLOAD1.L(T,IA,IGELECSTORAGE,'IMINUS') + VQESTOLOAD1.L(T,IA,IGELECSTORAGE,'IPLUS'))
$ifi '%UnitCmin%'==Yes    )

* P�nale f�r Redispatch von Kraftwerken, die sich nicht in der N�he des Engpasses befinden
$ifi '%DeRD%'==Yes  + SUM(IAGK_Y(IA,IGDISPATCH)$(RRRAAA(IR,IA)),
$ifi '%DeRD%'==Yes        IPENALTYforRD_T(IGDISPATCH,T) *
$ifi '%DeRD%'==Yes         (
$ifi '%DeRD%'==Yes             VGELEC_DPOS_T.L(T,IA,IGDISPATCH) + VGELEC_DNEG_T.L(T,IA,IGDISPATCH)
$ifi '%DeRD%'==Yes             + (VGELEC_CONSUMED_DPOS_T.L(T,IA,IGDISPATCH) + VGELEC_CONSUMED_DNEG_T.L(T,IA,IGDISPATCH))$IGELECSTORAGE(IGDISPATCH)
$ifi '%DeRD%'==Yes         )
$ifi '%DeRD%'==Yes    )

* P�nale f�r Windabschaltung

+ IPENALTYWINDCURT *
            (
$ifi '%Renewable_Spinning%' == Yes           VWINDCUR_ANCPOS.L(T,IR) +
            VWINDSHEDDING_DAY_AHEAD.L(T,IR))

+  IPENALTY_SOLAR *
            VSOLARSHEDDING_DAY_AHEAD.L(T,IR)


* Penalty for large ramps
  + SUM(IAGK_Y(IA,IGDISPATCH)$(RRRAAA(IR,IA)),
        IPENALTYRAMP *
        (VGRAMP_UP.L(T,IA,IGDISPATCH) + VGRAMP_DOWN.L(T,IA,IGDISPATCH))
    )

   + SUM(IA$(RRRAAA(IR,IA)),
            IHydro_Fill*VHYDROSPILLAGE_NEG.L(T,IA)
           )


    + SUM(IA$(RRRAAA(IR,IA)),
            IHydro_Dum*VHYDROSPILLAGE.L(T,IA)
           )


* to have adequate references for rel. gap when variable costs are close to zero
*TKMK$ifi '%UnitCmin%' == Yes + 100000

;

* end loop (IR,t
*);








if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP(IR,
            LOOP (R_RId(IR,RId),
               Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                  VOBJ_ALL_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
