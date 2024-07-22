* Unit commitment add-on to Wilmar
* Variables and equations


*============================================================================================================


*============================================================================================================
$ifi %UnitCmin%==yes    EQUATION  QUC21(AAA,G)        'If unit is online from the start of the optimisation it has to be online in the remaining hours of the minimum up time' ;
$ifi %UnitCmin%==yes    EQUATION  QUC22(T,AAA,G)      'Unit has to be online in the minimum up time hours' ;
$ifi %UnitCmin%==yes    EQUATION  QUC23(T,AAA,G)      'If unit is started up with less than minimum up time hours left of the optimisation period the unit has to stay online in the remaining hours of the optimisation period' ;
$ifi %UnitCmin%==yes    EQUATION  QUC24(AAA,G)        'If unit is offline from the start of the optimisation it has to be offline in the remaining hours of the minimum down time    ' ;
$ifi %UnitCmin%==yes    EQUATION  QUC25(T,AAA,G)      'Unit has to be offline in the minimum down time hours' ;
$ifi %UnitCmin%==yes    EQUATION  QUC26(T,AAA,G)      'If unit is shut down with less than minimum down time hours left of the optimisation period the unit has to stay offline in the remaining hours of the optimisation period   ' ;
$ifi %UnitCmin%==yes    EQUATION  QGSTARTFUEL(T,AAA,G)  'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption';
$ontext
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO1(T,AAA,PS)   'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO2(T,AAA,PS,G)   'Equation securing that each pump is either generating, spinning in water or stand-still when station mode is generating';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO3(T,AAA,PS,G)   'Equation securing that each pump is pumping or stand-still when station mode is pumping';
$offtext
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO4(T,AAA,G)   'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD1(T,AAA,G)   'Loading (pumping) lower than loading capacity';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD2(T,AAA,G)   'Loading (pumping) higher than minimum loading capacity';

$ontext
* reactivate
$ifi %UnitCramp%==yes   EQUATION  QUC18(T,AAA,G)      'Unit constrained by ramp-up and start-up ramp rates' ;
$ifi %UnitCramp%==yes   EQUATION  QUC20(T,AAA,G)      'Unit constrained by ramp-down rates' ;

$ifi %UnitCscc%==yes    EQUATION  QUC12(T,T_WITH_HIST,AAA,G)   'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption' ;
$offtext

*============================================================================================================
* not used for DeutschlandMIP
QUC21(IAGK_Y(IA,IGUC))$(IUCMININIU(IA,IGUC) GT 0)..
     SUM(T$(((ORD(T)-1) LE IUCMININIU(IA,IGUC)) AND (ORD(T)>1)),
         (1-VGONLINE_T(T,IA,IGUC))*(1-IG_UNP_OUTAGE(IA,IGUC,T)) ) =E= 0
;

QUC22(T,IAGK_Y(IA,IGUC))$(IT_OPT(T) AND ((IUCMININIU(IA,IGUC)+1) LE (ORD(T)-1))
                                     AND ((ORD(T)-1) LE (INO_HOURS_OPT_PERIOD - GDATA(IA,IGUC,'GDMINOPERATION')+1))
                                     AND (IUCMININIU(IA,IGUC) LE ((INO_HOURS_OPT_PERIOD-1)-GDATA(IA,IGUC,'GDMINOPERATION'))))..

   SUM(ITALIAS $((ORD(ITALIAS) LE GDATA(IA,IGUC,'GDMINOPERATION')) AND IT_OPT_ALL(T+(ORD(ITALIAS)-1)) ),
      VGONLINE_T(T+(ORD(ITALIAS)-1),IA,IGUC) + Epsilon
   )
   =G= GDATA(IA,IGUC,'GDMINOPERATION')*(
         VGONLINE_T(T,IA,IGUC)
         - VGONLINE_T(T-1,IA,IGUC)
           )
;


QUC23(T,IAGK_Y(IA,IGUC))$(IT_OPT(T)
                          AND (INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC,'GDMINOPERATION')+2 LE (ORD(T)-1))
                          AND (2 LE GDATA(IA,IGUC,'GDMINOPERATION')))..
   SUM(ITALIAS $((ORD(ITALIAS) LE (GDATA(IA,IGUC,'GDMINOPERATION')-1)) AND IT_OPT_ALL(T+(ORD(ITALIAS)-1))),
     VGONLINE_T(T+(ORD(ITALIAS)-1),IA,IGUC)
   - VGONLINE_T(T,IA,IGUC)
   + VGONLINE_T(T-1,IA,IGUC)
   )
    =G= 0;
;

* not used for DeutschlandMIP
QUC24(IAGK_Y(IA,IGUC))$(IUCMININID(IA,IGUC) GT 0)..
     SUM(T $( IT_OPT(T) AND ((ORD(T)-1) LE IUCMININID(IA,IGUC)) ),
        VGONLINE_T(T,IA,IGUC) ) =E= 0;
;

QUC25(T,IAGK_Y(IA,IGUC))$(IT_OPT(T)
                       AND (IUCMININID(IA,IGUC)+1 LE (ORD(T)-1))
                       AND ((ORD(T)-1) LE (INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC,'GDMINSHUTDOWN')+1)))..
   SUM(ITALIAS$((ORD(ITALIAS) LE GDATA(IA,IGUC,'GDMINSHUTDOWN')) AND IT_OPT_ALL(T+(ORD(ITALIAS)-1))),
      1-VGONLINE_T(T+(ORD(ITALIAS)-1),IA,IGUC)
*      ) + 10*Epsilon #CW proposed change
      ) + Epsilon
   =G= GDATA(IA,IGUC,'GDMINSHUTDOWN')*(
         - VGONLINE_T(T,IA,IGUC)
         + VGONLINE_T(T-1,IA,IGUC)
        )
;

QUC26(T,IAGK_Y(IA,IGUC))$(IT_OPT(T)
                         AND(INO_HOURS_OPT_PERIOD-GDATA(IA,IGUC,'GDMINSHUTDOWN')+2 LE (ORD(T)-1))
                         AND (2 LE GDATA(IA,IGUC,'GDMINSHUTDOWN')))..
   SUM(ITALIAS $((ORD(ITALIAS) LE (GDATA(IA,IGUC,'GDMINSHUTDOWN')-1)) AND IT_OPT_ALL(T+(ORD(ITALIAS)-1))),
      1-VGONLINE_T(T+(ORD(ITALIAS)-1),IA,IGUC)
       + VGONLINE_T(T,IA,IGUC)
       - VGONLINE_T(T-1,IA,IGUC)
   )
    =G= 0
;

QGSTARTFUEL(T,IAGK_Y(IA,IGNOUCSCC))$IT_OPT(T)..

  VGSTARTUPFUELCONS(T,IA,IGNOUCSCC)
   =G=
  GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHOT')*IGELECCAPEFF(IA,IGNOUCSCC,T)*(
    VGONLINE_T(T,IA,IGNOUCSCC)
    - VGONLINE_T(T-1,IA,IGNOUCSCC)
  )
 ;

QUCELECSTO4(T,IAGK_Y(IA,IGELECSTORAGE))$IT_OPT(T)..

  VGONLINE_T(T,IA,IGELECSTORAGE)+ VGONLCHARGE(T,IA,IGELECSTORAGE)=L= 1
;

$ontext
* not used for DeutschlandMIP
QUCELECSTO1(T,IAPSK_Y(IA,PS))$IT_OPT(T)..

  VGGEN(IA,PS,T)+ VGPUMP(IA,PS,T)=L= 1
;

* not used for DeutschlandMIP
QUCELECSTO2(T,STACONF(IA,PS,IGELECSTORAGE))$IT_OPT(T)..

  VGONLINE_T(T,IA,IGELECSTORAGE) =L= VGGEN(IA,PS,T)
;

* not used for DeutschlandMIP
QUCELECSTO3(T,STACONF(IA,PS,IGELECSTORAGE))$IT_OPT(T)..

  VGONLCHARGE(T,IA,IGELECSTORAGE) =L= VGPUMP(IA,PS,T)
;
$offtext


* Loading (pumping) lower than loading capacity

QESTOLOAD1(T,IAGK_Y(IA,IGELECSTORAGE))$IT_OPT(T)..

    VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE)
  + VGELEC_CONSUMED_DPOS_T(T,IA,IGELECSTORAGE)
  - VGELEC_CONSUMED_DNEG_T(T,IA,IGELECSTORAGE)
  =L=
    VQESTOLOAD1(T,IA,IGELECSTORAGE,'IPLUS')
  + IGSTOLOADCAPACITY_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*VGONLCHARGE(T,IA,IGELECSTORAGE)*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
;

* Loading (pumping) higher than minimum loading capacity

QESTOLOAD2(T,IAGK_Y(IA,IGELECSTORAGE))$IT_OPT(T)..

    VGELEC_CONSUMED_T(T,IA,IGELECSTORAGE)
  + VGELEC_CONSUMED_DPOS_T(T,IA,IGELECSTORAGE)
  - VGELEC_CONSUMED_DNEG_T(T,IA,IGELECSTORAGE)
  =G=
  - VQESTOLOAD1(T,IA,IGELECSTORAGE,'IMINUS')
  + IGSTOLOADCAPACITYMIN_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*VGONLCHARGE(T,IA,IGELECSTORAGE)*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
;


$ontext
QUC18(INT(NODE,T),IAGK_Y(IA,IGUC)) $(ORD(T) >1) ..

    VGELEC_T(T,IA,IGUC)+ VGELEC_DPOS_T(INT,IA,IGUC)- VGELEC_DNEG_T(INT,IA,IGUC)
*  + SUM(DAY $IDT(DAY,T),VGE_ANCPOS(DAY,IA,IGUC)) $IGSPINNING(IGUC)
   + VGE_ANCPOS_5(INT,IA,IGUC)

  =L=

   SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
      VGELEC_T(ITALIAS,IA,IGUC) + VGELEC_DPOS_T(ITALIAS,IA,IGUC,INODEALIAS) - VGELEC_DNEG_T(ITALIAS,IA,IGUC,INODEALIAS)
      + GDATA(IA,IGUC,'GDRAMPUP')*VGONLINE_T(ITALIAS,IA,IGUC,INODEALIAS)
   )
   + GDATA(IA,IGUC,'GDSTARTUPRAMP')*(VGONLINE_T(INT,IA,IGUC)-
      SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
         VGONLINE_T(ITALIAS,IA,IGUC,INODEALIAS)
      ))
   + IGELECCAPEFF(IA,IGUC,T)* (1-VGONLINE_T(INT,IA,IGUC))

;

QUC20(INT(NODE,T),IAGK_Y(IA,IGUC)) $(ORD(T) >1) ..
   SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS)$(PREDECESSOR(NODE,INODEALIAS)$((ORD(T)-1) EQ ORD(ITALIAS)))),
      VGELEC_T(ITALIAS,IA,IGUC) + VGELEC_DPOS_T(ITALIAS,IA,IGUC,INODEALIAS) - VGELEC_DNEG_T(ITALIAS,IA,IGUC,INODEALIAS)
   )
   - (VGELEC_T(T,IA,IGUC) + VGELEC_DPOS_T(T,IA,IGUC,NODE) - VGELEC_DNEG_T(T,IA,IGUC,NODE))
  =L=

   GDATA(IA,IGUC,'GDRAMPUP')* VGONLINE_T(INT,IA,IGUC)+

   GDATA(IA,IGUC,'GDSTARTUPRAMP')*(
      SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS) AND PREDECESSOR(NODE,INODEALIAS) AND ((ORD(T)-1) EQ ORD(ITALIAS))),
         VGONLINE_T(ITALIAS,IA,IGUC,INODEALIAS))
         -VGONLINE_T(INT,IA,IGUC))

   + IGELECCAPEFF(IA,IGUC,T)* (1-
         SUM((INODEALIAS,ITALIAS)$(INT(INODEALIAS,ITALIAS)$(PREDECESSOR(NODE,INODEALIAS)$((ORD(T)-1) EQ ORD(ITALIAS)))),
            VGONLINE_T(ITALIAS,IA,IGUC,INODEALIAS)
         ))

;

QUC12(T,ITALIAS_WITH_HIST,IAGK_Y(IA,IGUCSCC),SCEN)$((ORD(ITALIAS_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')) AND
       ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (ORD(T) GT 1))..
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGSTARTUPFUELCONS(T,IA,IGUCSCC,NODE))
   =G=
   SUM(i$(ORD(i)=ORD(ITALIAS_WITH_HIST)),UCSCCCOST(IGUCSCC,i))*(
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_T(T,IA,IGUCSCC,NODE))-
   SUM((INODEALIAS,italias)$((ORD(italias)<= ORD(ITALIAS_WITH_HIST)) AND SCEN_NODE(SCEN,INODEALIAS) AND INT_WITH_HIST(INODEALIAS,T-ORD(italias))),
      VGONLINE_T(T-ORD(ITALIAS),IA,IGUCSCC,INODEALIAS))
   )
;
$offtext
