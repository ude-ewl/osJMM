* File: print-OUT_OPMARG_GDISPATCH_T.inc

* This file will print the operation margins for dispatachble units in the root node
* divided by hours.

PUT OUT_OPMARG_GDISPATCH_T;
OUT_OPMARG_GDISPATCH_T.PW = 32767;
OUT_OPMARG_GDISPATCH_T.PS = 1;
OUT_OPMARG_GDISPATCH_T.nJ = 2;


LOOP ((IAGK_Y(IA,IGDISPATCH),IR,T)$(RRRAAA(IR,IA) AND ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),

   OPMARG_GDISPATCH(IAGK_Y,T)

 =

*Intraday
*     Production
    + ((VGELEC_T.L (T,IAGK_Y)+VGELEC_DPOS_T.L(T,IAGK_Y)-VGELEC_DNEG_T.L(T,IAGK_Y))
*     Pumping
     - (VGELEC_CONSUMED_T.L (T,IAGK_Y)+VGELEC_CONSUMED_DPOS_T.L(T,IAGK_Y)
        -VGELEC_CONSUMED_DNEG_T.L(T,IAGK_Y))$IGELECSTORAGE(IGDISPATCH)

     )* (QEEQINT.M(T,IR) $(QEEQINT.M(T,IR)<=1000.)+1000.$(QEEQINT.M(T,IR)>1000.))


* Heat
*               Production
   + (VGHEAT_T.L(T,IAGK_Y)$IGHEAT(IGDISPATCH)
*     Storage charging
     - VGHEAT_CONSUMED_T.L(T,IAGK_Y)$IGHEATSTORAGE(IGDISPATCH)

     )* QHEQ.M(T,IA)

* Heat FIXQ
*               Production
   + (VGHEAT_T.L(T,IAGK_Y)$IGHEAT(IGDISPATCH)
*     Storage charging
     - VGHEAT_CONSUMED_T.L(T,IAGK_Y)$IGHEATSTORAGE(IGDISPATCH)


     )* IFUELPRICE_Y(IA,'Erdgas_DE',T)/0.85

*Ancillary Services Positive
*               Production
   + (VGE_ANCPOS.L (T,IAGK_Y)
*     Pumping
     + VGE_CONSUMED_ANCPOS.L(IAGK_Y,T)$IGELECSTORAGE(IGDISPATCH)

     )* SUM(TSO_RRR $TSO_RRRRRR(TSO_RRR,IR),QANCPOSEQ.M(T,TSO_RRR))
*Ancillary Services Negative
*               Production
   + (VGE_ANCNEG.L (T,IAGK_Y)
*     Pumping
     + VGE_CONSUMED_ANCNEG.L(T,IAGK_Y)$IGELECSTORAGE(IGDISPATCH)

     )* SUM(TSO_RRR $TSO_RRRRRR(TSO_RRR,IR),QANCNEGEQ.M(T,TSO_RRR))
*Ancillary Services Non-Spinning Positive
*               Production
   + (VGE_NONSPIN_ANCPOS.L(T,IAGK_Y)
*     Pumping
     + VGE_CONSUMED_NONSP_ANCPOS.L(T,IAGK_Y)$IGELECSTORAGE(IGDISPATCH)

     )* SUM(TSO_RRR $TSO_RRRRRR(TSO_RRR,IR),QNONSP_ANCPOSEQ.M(T,TSO_RRR))


* Costs
-(
* (VGFUELUSAGE_T)
* Cost of fuel consumption during optimisation period:

   SUM( FFF $( GDATA(IAGK_Y,'GDFUEL') EQ FDATA(FFF,'FDNB')) ,
        IFUELPRICE_Y(IA,FFF,T) * VGFUELUSAGE_T.L(T,IA,IGDISPATCH)
      )


* (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Operation and maintainance cost:
* attention: seperate calculation of maintentance costs as not for all units if IGELEC vgELEC_DNEG is defined
* ensure that we have one unit for IGRUNOFRIVER per area and one unit IGSOLAR, IGTIDALSTREAM, IGWAVE in each region

  + GDATA(IA,IGDISPATCH,'GDOMVCOST')*
          (VGELEC_T.L(T,IA,IGDISPATCH) + VGELEC_DPOS_T.L(T,IA,IGDISPATCH) - VGELEC_DNEG_T.L(T,IA,IGDISPATCH))


* #3+4 (VGHEAT_T)
* CHPBF: IAFLEXQ

  +  (GDATA(IA,IGDISPATCH,'GDOMVCOST') * GDATA(IA,IGDISPATCH,'GDCV') * VGHEAT_T.L(T,IA,IGDISPATCH))$IGELECANDHEAT(IGDISPATCH)
       + (GDATA(IA,IGDISPATCH,'GDOMVCOST') * VGHEAT_T.L(T,IA,IGDISPATCH))$IGHEATONLY(IGDISPATCH)


* #5 (VGSTARTUP_T) * trkr: Start-up variable costs:
  + (GDATA(IA,IGDISPATCH,'GDSTARTUPCOST')*
$ifi     '%UnitCmin%' == Yes IGELECCAPEFF(IA,IGDISPATCH,T)*
        VGSTARTUP_T.L(T,IA,IGDISPATCH))$IGUC(IGDISPATCH)


* #7 (VGFUELUSAGE_T)
* Tax on fuel usage for producing power and heat (condensing and chp) in Germany.
* Used fuel * fuel tax * 3.6, which is a multiplier from MWh to GJ:
* not used in DeRD
* IAFLEXQ

  +  SUM(sameas(C,'DE')$ICA(C,IA),
          GDATA(IA,IGDISPATCH,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T.L(T,IA,IGDISPATCH)
           )


* #9 (VGFUELUSAGE_T)
* Emission taxes CO2, SO2:

   +  SUM(C,
         SUM( FFF$( (GDATA(IA,IGDISPATCH,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) ),
               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2') * IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGDISPATCH,'GDDESO2'))) * CONVERT_KG_PER_GJ_T_PER_MWH *
               VGFUELUSAGE_T.L(T,IA,IGDISPATCH)
         )
      )


* #10 (VGELEC_T, VGELEC_DPOS_T, VGELEC_DNEG_T)
* Subsidy on electricity produced using biomass or waste: (values of ELEC_SUBSIDY <0)

   +  SUM(C,
         SUM(FFF$( (GDATA(IA,IGDISPATCH,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) ),
                 ELEC_SUBSIDY(C,FFF)*
                 (VGELEC_T.L(T,IA,IGDISPATCH) + VGELEC_DPOS_T.L(T,IA,IGDISPATCH) - VGELEC_DNEG_T.L(T,IA,IGDISPATCH))
         )
      )


* #18a (VGSTARTUPFUELCONS and VGSTARTUP_T, resp.)
* Start-up fuel costs: for power plants without outtime dependent start-up fuel consumption
* Set IGNOUCSCC = IGUC if UnitCscc=No. IGNOUCSCC=IGUC-IGUCSCC if UnitCscc=Yes

  + SUM(FFF $((GDATA(IA,IGDISPATCH,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')) ),
  IFUELPRICE_Y(IA,FFF,T)*
$ifi     '%UnitCmin%' == Yes        VGSTARTUPFUELCONS.L(T,IA,IGDISPATCH)
$ifi Not '%UnitCmin%' == Yes        GDATA(IA,IGDISPATCH,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGDISPATCH)
  )$IGNOUCSCC(IGDISPATCH)

* #18b (VGSTARTUPFUELCONS)
* Start-up fuel costs: for power plants with outtime dependant start-up fuel consumption
* 

$ifi %UnitCscc%==yes   + SUM( FFF $((GDATA(IA,IGDISPATCH,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB'))),
$ifi %UnitCscc%==yes      IFUELPRICE_Y(IA,FFF,T)*
$ifi %UnitCscc%==yes         VGSTARTUPFUELCONS.L(T,IA,IGDISPATCH))$IGUCSCC(IGDISPATCH)


* #19a (VGSTARTUP_T bzw. VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (see #9):
   + SUM( (C,FFF)$( (GDATA(IA,IGDISPATCH,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA)),

$ifi Not '%UnitCmin%' == Yes  GDATA(IA,IGDISPATCH,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGDISPATCH)*
$ifi     '%UnitCmin%' == Yes  VGSTARTUPFUELCONS.L(T,IA,IGDISPATCH)*
               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGDISPATCH,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
         )$IGNOUCSCC(IGDISPATCH)

* #19b (VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (out-time dependant start-up):
$ifi %UnitCscc%==yes   + SUM( C,FFF)$( (GDATA(IA,IGDISPATCH,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA)),
$ifi %UnitCscc%==yes             VGSTARTUPFUELCONS.L(T,IA,IGDISPATCH)*
$ifi %UnitCscc%==yes               (FDATA(FFF,'FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISPATCH,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi %UnitCscc%==yes         )$IGUCSCC(IGDISPATCH)



* #20 (VGFUELUSAGE_T)
* Tax reduction on fuel usage for producing power and heat in chp (applied to Germany).
* Fuel use * fuel tax * 3.6, which is a multiplier from GJ to MWH:

  -  SUM(sameas(C,'DE')$(ICA(C,IA) ),
         GDATA(IA,IGDISPATCH,'GDFUELTAXRED')* 3.6 * VGFUELUSAGE_T.L(T,IA,IGDISPATCH)
        )$IGELECANDHEAT(IGDISPATCH)



* #21 (VGHEAT_T)
* Tax on fuel usage for producing heat in chp production.
* Produced heat * fuel tax (conversion efficiency from fuel to heat included)
*               * 3.6, which is a multiplier from GJ to MWH. Applies not for Germany:


  +  SUM(C $(Not sameas(C,'DE') AND ICA(C,IA)),
              GDATA(IA,IGDISPATCH,'GDFUELTAX') * 3.6 * VGHEAT_T.L(T,IA,IGDISPATCH)
           )$IGELECANDHEAT(IGDISPATCH)



* #22 (VGHEAT_T)
* Subsidy for CHP Plants (KWK-Zuschlag).
* Produced heat * CHP coefficient * CHP-subsidy

  -   SUM(C $(sameas(C,'DE') AND ICA(C,IA)),
           GDATA(IA,IGDISPATCH,'GDCB') *
               (GDATA(IA,IGDISPATCH,'GDCHP_SUBSIDY1') + GDATA(IA,IGDISPATCH,'GDCHP_SUBSIDY2') + GDATA(IA,IGDISPATCH,'GDCHP_SUBSIDY3')) *
               VGHEAT_T.L(T,IA,IGDISPATCH)
          )$IGELECANDHEAT(IGDISPATCH)

* #23 Tax on fuels of heatboilers (VGFUELUSAGE_T)
* Used fuel * fuel tax * 3.6, which is a multiplier from GJ to MWH:
* IAFLEXQ

  +  SUM(C $(ICA(C,IA)),
             GDATA(IA,IGDISPATCH,'GDFUELTAX') * 3.6 * VGFUELUSAGE_T.L(T,IA,IGDISPATCH)
        )$IGHEATBOILER(IGDISPATCH)

* Penalty for large ramps
  +     IPENALTYRAMP *
        (VGRAMP_UP.L(T,IA,IGDISPATCH) + VGRAMP_DOWN.L(T,IA,IGDISPATCH))

* end sum over cost terms
)
;

* end loop (IAGK_Y,t
);








if (INO_SOLVE EQ 1,
   PUT "CaseId,Area,UnitGroup,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP(IAGK_Y(IA,IGDISPATCH),
            IF (abs(OPMARG_GDISPATCH(IAGK_Y,T))>epsilon,
                Put CaseId.TL:0,",",IA.TL:0,",",IGDISPATCH.TL:0,",",BASETIME.TL:0,",",
                  OPMARG_GDISPATCH(IAGK_Y,T):0:4/ ;
               );
            );
         );
      );
   );
