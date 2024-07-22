* File: print-OUT_VOBJ_R_T_TaxSub.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_TaxSub_OUT;
OUT_VOBJ_R_T_TaxSub_OUT.PW = 32767;
OUT_VOBJ_R_T_TaxSub_OUT.PS = 1;
OUT_VOBJ_R_T_TaxSub_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_TaxSub_out(IR,t)

 =

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


;
* end loop (IR,t
);




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
                  VOBJ_TaxSub_out(IR,T):0:4/ ;
            );
         );
      );
   );
);