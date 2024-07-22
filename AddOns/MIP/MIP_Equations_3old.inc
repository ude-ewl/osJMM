* Unit commitment add-on to Wilmar
* Variables and equations

*============================================================================================================

*============================================================================================================

$ifi %UnitCmin%==yes    EQUATION  QUC21(AAA,G,SCEN)        'If unit is online from the start of the optimisation it has to be online in the remaining hours of the minimum up time' ;
$ifi %UnitCmin%==yes    EQUATION  QUC22(T,AAA,G,SCEN)      'Unit has to be online in the minimum up time hours' ;
$ifi %UnitCmin%==yes    EQUATION  QUC23(T,AAA,G,SCEN)      'If unit is started up with less than minimum up time hours left of the optimisation period the unit has to stay online in the remaining hours of the optimisation period' ;
$ifi %UnitCmin%==yes    EQUATION  QUC24(AAA,G,SCEN)        'If unit is offline from the start of the optimisation it has to be offline in the remaining hours of the minimum down time    ' ;
$ifi %UnitCmin%==yes    EQUATION  QUC25(T,AAA,G,SCEN)      'Unit has to be offline in the minimum down time hours' ;
$ifi %UnitCmin%==yes    EQUATION  QUC26(T,AAA,G,SCEN)      'If unit is shut down with less than minimum down time hours left of the optimisation period the unit has to stay offline in the remaining hours of the optimisation period   ' ;
*-------------------------------------------------------------------------------
*-------------------------------------------------------------------------------
$ifi %UnitCmin%==yes    EQUATION  QGSTARTFUEL(T,AAA,G,NODE)  'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO4(T,AAA,G,NODE)   'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO1(T,AAA,PS,NODE)   'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO2(T,AAA,PS,G,NODE)   'Equation securing that each pump is either generating, spinning in water or stand-still when station mode is generating';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO3(T,AAA,PS,G,NODE)   'Equation securing that each pump is pumping or stand-still when station mode is pumping';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD1(T,AAA,G,NODE)   'Loading (pumping) lower than loading capacity';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD2(T,AAA,G,NODE)   'Loading (pumping) higher than minimum loading capacity';

$ifi %UnitCramp%==yes   EQUATION  QUC18(T,AAA,G,NODE)      'Unit constrained by ramp-up and start-up ramp rates' ;
$ifi %UnitCramp%==yes   EQUATION  QUC20(T,AAA,G,NODE)      'Unit constrained by ramp-down rates' ;

$ifi %UnitCscc%==yes    EQUATION  QUC12(T,ITALIAS_WITH_HIST,AAA,G,SCEN)   'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption' ;

*============================================================================================================
$ifi not %UnitCmin%==yes $goto UnitC_label1

QUC21(IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$(IUCMININIU(IA,IGUC_NOALLWAYS) GT 0)..
     SUM(INT(NODE,T)$(((ORD(T)-1) LE IUCMININIU(IA,IGUC_NOALLWAYS)) AND (ORD(T)>1) AND SCEN_NODE(SCEN,NODE)),
         (1-VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T))*(1-IG_UNP_OUTAGE(IA,IGUC_NOALLWAYS,T))*IGKDERATE(IA,IGUC_NOALLWAYS,T) ) =E= 0
;

QUC22(T,IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$((ORD(T)>1) AND ((IUCMININIU(IA,IGUC_NOALLWAYS)+1) LE (ORD(T)-1)) AND ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD)
                                    AND ((ORD(T)-1) LE (INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')+1))
                                    AND (IUCMININIU(IA,IGUC_NOALLWAYS) LE ((INO_HOURS_OPT_PERIOD-1)-GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION'))))..
   SUM((INODEALIAS,ITALIAS)$((ORD(ITALIAS) LE GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')) AND SCEN_NODE(SCEN,INODEALIAS) AND INT(INODEALIAS,T+(ORD(ITALIAS)-1))),
      VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS,T+(ORD(ITALIAS)-1))
   )
   =G= GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')*(
         SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T))
         - SUM((INODEALIAS2,ITALIAS2)$(((ORD(T)-1) EQ ORD(ITALIAS2)) AND SCEN_NODE(SCEN,INODEALIAS2) AND INT(INODEALIAS2,ITALIAS2)),
            VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS2,ITALIAS2)
           ))
;

QUC23(T,IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$((ORD(T)>1) AND ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')+2 LE (ORD(T)-1)) AND (2 LE GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')))..
   SUM((INODEALIAS,ITALIAS)$((ORD(ITALIAS) LE (GDATA(IA,IGUC_NOALLWAYS,'GDMINOPERATION')-1)) AND SCEN_NODE(SCEN,INODEALIAS) AND INT(INODEALIAS,T+(ORD(ITALIAS)-1))),
     (VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS,T+(ORD(ITALIAS)-1)) -
         SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T))
         + SUM((INODEALIAS2,ITALIAS2)$(((ORD(T)-1) EQ ORD(ITALIAS2)) AND SCEN_NODE(SCEN,INODEALIAS2) AND INT(INODEALIAS2,ITALIAS2)),
            VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS2,ITALIAS2))
     )
   )
    =G= 0;
;

QUC24(IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$(IUCMININID(IA,IGUC_NOALLWAYS) GT 0)..
     SUM(INT(NODE,T)$(((ORD(T)-1) LE IUCMININID(IA,IGUC_NOALLWAYS)) AND (ORD(T)>1) AND SCEN_NODE(SCEN,NODE)),
        VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T) ) =E= 0;
;

QUC25(T,IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$((ORD(T)>1) AND ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (IUCMININID(IA,IGUC_NOALLWAYS)+1 LE (ORD(T)-1)) AND
                                 ((ORD(T)-1) LE (INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')+1)))..
   SUM((INODEALIAS,ITALIAS)$((ORD(ITALIAS) LE GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')) AND SCEN_NODE(SCEN,INODEALIAS) AND INT(INODEALIAS,T+(ORD(ITALIAS)-1))),
      1-VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS,T+(ORD(ITALIAS)-1)))
*   + VQUC25(T,IA,IGUC_NOALLWAYS,SCEN,'IPLUS')
   =G= GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')*(
         SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),-VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T))
         + SUM((INODEALIAS2,ITALIAS2)$(((ORD(T)-1) EQ ORD(ITALIAS2)) AND SCEN_NODE(SCEN,INODEALIAS2) AND INT(INODEALIAS2,ITALIAS2)),
            VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS2,ITALIAS2)
           ))
;

QUC26(T,IAGK_Y(IA,IGUC_NOALLWAYS),SCEN)$((ORD(T)>1) AND ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND(INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')+2 LE (ORD(T)-1)) AND (2 LE GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')))..
   SUM((INODEALIAS,ITALIAS)$((ORD(ITALIAS) LE (GDATA(IA,IGUC_NOALLWAYS,'GDMINSHUTDOWN')-1)) AND SCEN_NODE(SCEN,INODEALIAS) AND INT(INODEALIAS,T+(ORD(ITALIAS)-1))),
      1-VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS,T+(ORD(ITALIAS)-1)) +
         SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_NT(IA,IGUC_NOALLWAYS,NODE,T))
         - SUM((INODEALIAS2,ITALIAS2)$(((ORD(T)-1) EQ ORD(ITALIAS2)) AND SCEN_NODE(SCEN,INODEALIAS2) AND INT(INODEALIAS2,ITALIAS2)),
              VGONLINE_NT(IA,IGUC_NOALLWAYS,INODEALIAS2,ITALIAS2))
   )
    =G= 0
;

QGSTARTFUEL(INT(NODE,T),IAGK_Y(IA,IGNOUCSCC))$(ORD(T) GT 1) ..

  VGSTARTUPFUELCONS(T,IA,IGNOUCSCC,NODE)
   =G=
  GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHOT')*IGELECCAPEFF(IA,IGNOUCSCC,T)*(VGONLINE_NT(IA,IGNOUCSCC,NODE,T)
  - SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS)) ),
        VGONLINE_NT(IA,IGNOUCSCC,INODEALIAS,ITALIAS)))
 ;

* Loading (pumping) lower than loading capacity
$ontext
QESTOLOAD1(INT(NODE,T),IAGK_Y(IA,IGELECSTORAGE))$(ORD(T) >1)..

    VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE)
  + VGELEC_CONSUMED_DPOS_NT(IA,IGELECSTORAGE,INT)
  - VGELEC_CONSUMED_DNEG_NT(IA,IGELECSTORAGE,INT)
  =L=
    VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IPLUS')
*  - VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IMINUS')

  + IGSTOLOADCAPACITY_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*VGONLCHARGE(INT,IA,IGELECSTORAGE)*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
;
$offtext

* Loading (pumping) higher than minimum loading capacity

QESTOLOAD2(INT(NODE,T),IAGK_Y(IA,IGELECSTORAGE))$(ORD(T) >1)..

    VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE)
  + VGELEC_CONSUMED_DPOS_NT(IA,IGELECSTORAGE,INT)
  - VGELEC_CONSUMED_DNEG_NT(IA,IGELECSTORAGE,INT)
  =G=
*    VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IPLUS')
*  - VQESTOLOAD1(INT,IA,IGELECSTORAGE,'IMINUS')

*  + IGSTOLOADCAPACITYMIN_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*VGONLCHARGE(INT,IA,IGELECSTORAGE)*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
  + IGSTOLOADCAPACITYMIN_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*(1-VGTURBINE_ON(INT,IA,IGELECSTORAGE))*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))

;


$label  UnitC_label1
*============================================================================================================


*============================================================================================================
$ifi not %UnitCramp%==yes $goto UnitC_label2

QUC18(INT(NODE,T),IAGK_Y(IA,IGUC)) $(ORD(T) >1) ..

    VGELEC_T(T,IA,IGUC)+ VGELEC_DPOS_NT(IA,IGUC,INT)- VGELEC_DNEG_NT(IA,IGUC,INT)
$iftheni '%Looping%' == week   +                      VGE_ANCPOS(T,IA,IGUC)$IGSPINNING(IGUC)
$else                          + SUM(DAY $IDT(DAY,T), VGE_ANCPOS(DAY,IA,IGUC))$IGSPINNING(IGUC)
$endif
  + VGE_ANCPOS_5(INT,IA,IGUC)

  =L=

   SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
      VGELEC_T(ITALIAS,IA,IGUC) + VGELEC_DPOS_NT(IA,IGUC,INODEALIAS,ITALIAS) - VGELEC_DNEG_NT(IA,IGUC,INODEALIAS,ITALIAS)
      + GDATA(IA,IGUC,'GDRAMPUP')*IGELECCAPEFF(IA,IGUC,T)*VGONLINE_NT(IA,IGUC,INODEALIAS,ITALIAS)
   )
   + IGELECCAPEFF(IA,IGUC,T)*GDATA(IA,IGUC,'GDSTARTUPRAMP')*(VGONLINE_NT(IA,IGUC,INT)-
      SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
         VGONLINE_NT(IA,IGUC,INODEALIAS,ITALIAS)
      ))
   + IGELECCAPEFF(IA,IGUC,T)* (1-VGONLINE_NT(IA,IGUC,INT))

;

QUC20(INT(NODE,T),IAGK_Y(IA,IGUC)) $(ORD(T) >1) ..
   SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS)$(PREDECESSOR(NODE,INODEALIAS)$((ORD(T)-1) EQ ORD(ITALIAS)))),
      VGELEC_T(ITALIAS,IA,IGUC) + VGELEC_DPOS_NT(IA,IGUC,INODEALIAS,ITALIAS) - VGELEC_DNEG_NT(IA,IGUC,INODEALIAS,ITALIAS)
   )
   - (VGELEC_T(T,IA,IGUC) + VGELEC_DPOS_NT(IA,IGUC,NODE,T) - VGELEC_DNEG_NT(IA,IGUC,NODE,T))
  =L=

   GDATA(IA,IGUC,'GDRAMPUP')* VGONLINE_NT(IA,IGUC,INT)*IGELECCAPEFF(IA,IGUC,T) +

   IGELECCAPEFF(IA,IGUC,T)*GDATA(IA,IGUC,'GDSTARTUPRAMP')*(
      SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
         VGONLINE_NT(IA,IGUC,INODEALIAS,ITALIAS))
         -VGONLINE_NT(IA,IGUC,INT))

   + IGELECCAPEFF(IA,IGUC,T)* (1-
         SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS)$(PREDECESSOR(NODE,INODEALIAS)$((ORD(T)-1) EQ ORD(ITALIAS)))),
            VGONLINE_NT(IA,IGUC,INODEALIAS,ITALIAS)
         ))

;
$label  UnitC_label2
*============================================================================================================



*============================================================================================================
$ifi not %UnitCscc%==yes $goto UnitC_label3

QUC12(T,ITALIAS_WITH_HIST,IAGK_Y(IA,IGUCSCC),SCEN)$((ORD(ITALIAS_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')) AND
       ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (ORD(T) GT 1))..
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGSTARTUPFUELCONS(T,IA,IGUCSCC,NODE))
   =G=
   SUM(i$(ORD(i)=ORD(ITALIAS_WITH_HIST)),UCSCCCOST(IGUCSCC,i))*(
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_NT(IA,IGUCSCC,NODE,T))-
   SUM((INODEALIAS,italias)$((ORD(italias)<= ORD(ITALIAS_WITH_HIST)) AND SCEN_NODE(SCEN,INODEALIAS) AND INT_WITH_HIST(INODEALIAS,T-ORD(italias))),
      VGONLINE_NT(IA,IGUCSCC,INODEALIAS,T-ORD(italias)))
   )
;

$label  UnitC_label3
