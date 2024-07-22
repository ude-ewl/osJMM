* File: print-OUT_VOBJ_R_T_OMVCost.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_OMVCost_OUT;
OUT_VOBJ_R_T_OMVCost_OUT.PW = 32767;
OUT_VOBJ_R_T_OMVCost_OUT.PS = 1;
OUT_VOBJ_R_T_OMVCost_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_OMVCost_out(IR,t)

 =
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

* #17a (constant, operation and maintainance costs of renewables)
  +
         (
          ITIDALSTREAM_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGTIDALSTREAM) $RRRAAA(IR,IA), GDATA(IA,IGTIDALSTREAM,'GDOMVCOST'))
          + IWAVE_VAR_T(IR,T)      * SUM(IAGK_Y(IA,IGWAVE) $RRRAAA(IR,IA), GDATA(IA,IGWAVE,'GDOMVCOST'))
          + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), GDATA(IA,IGDISGEN,'GDOMVCOST'))
          + IBIOMASS_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + IOthRes_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGBIOMASS) $RRRAAA(IR,IA), GDATA(IA,IGBIOMASS,'GDOMVCOST'))
          + ISOLARTH_VAR_T(IR,T)   * SUM(IAGK_Y(IA,IGSOLARTH) $RRRAAA(IR,IA), GDATA(IA,IGSOLARTH,'GDOMVCOST'))

$ifi '%Code_version%'==UENB     + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'%Sonstige_NEE%',T)+IFUELTRANS_Y(IA,IGDISGEN,T))  )
$ifi '%Code_version%'==EWL      + IDISGEN_VAR_T(IR,T)    * SUM(IAGK_Y(IA,IGDISGEN) $RRRAAA(IR,IA), (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) * (IFUELPRICE_Y(IA,'DISGEN',T)+IFUELTRANS_Y(IA,IGDISGEN,T)) )
          +                          SUM(IAGK_Y(IA,IGRUNOFRIVER) $RRRAAA(IR,IA), GDATA(IA,IGRUNOFRIVER,'GDOMVCOST') * IRUNRIVER_AAA_T(IA,T))
          + SUM(IAGK_Y(IA,IGSOLAR) $RRRAAA(IR,IA), GDATA(IA,IGSOLAR,'GDOMVCOST')) * (ISOLAR_VAR_T(IR,T)
          + IGEOTHHEAT_VAR_T(IR,T) * SUM(IAGK_Y(IA,IGGEOTHHEAT) $RRRAAA(IR,IA), GDATA(IA,IGGEOTHHEAT,'GDOMVCOST'))
        )
      )



* #17b (VWINDCUR_ANCPOS, VGELEC_DNEG_T): operation and maintainance costs of production from wind
   +      SUM( IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),
             GDATA(IA,IGWIND,'GDOMVCOST')*
            (IWIND_REALISED_IR(IR,T)  - VGELEC_DNEG_T.L(T,IA,IGWIND)
$ifi '%Renewable_Spinning%' == Yes - VWINDCUR_ANCPOS.L(T,IR)
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
                  VOBJ_OMVCost_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
