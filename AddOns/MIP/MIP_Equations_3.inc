* Unit commitment add-on to Wilmar
* Variables and equations


*============================================================================================================
*============================================================================================================
$ifi %UnitCmin%==yes $ifi not '%NoOpTimeRestrictions%' == Yes    EQUATION  QGONLOPMIP(T,AAA,G)   'Equation for minimum operation time';
$ifi %UnitCmin%==yes $ifi not '%NoOpTimeRestrictions%' == Yes    EQUATION  QGONLSDMIP(T,AAA,G)   'Equation for minimum shut-down time';

$ifi %UnitCmin%==yes $ifi not '%NoStartUpCosts%' == Yes     EQUATION  QGSTARTFUEL(T,AAA,G)  'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO4(T,AAA,G)  'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD1(T,AAA,G)   'Loading (pumping) lower than loading capacity';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD2(T,AAA,G)   'Loading (pumping) higher than minimum loading capacity';

$ifi %UnitCmin%==yes    EQUATION  QUC18(T,AAA,G)        'Unit constrained by ramp-up and start-up ramp rates' ;
$ifi %UnitCmin%==yes    EQUATION  QUC20(T,AAA,G)        'Unit constrained by ramp-down rates' ;

$ifi %UnitCscc%==yes    EQUATION  QUC12(T,ITALIAS_WITH_HIST,AAA,G)   'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption' ;

*============================================================================================================

* ---- QGONLOPMIP: minimum operation time ---- *{

$ifi not '%NoOpTimeRestrictions%' == Yes QGONLOPMIP(T,IAGK_Y(IA,IGMINOPERATION_NOALWAYS))$IT_OPT(T)..

$ifi not '%NoOpTimeRestrictions%' == Yes    SUM(T_WITH_HIST $(ORD(T_WITH_HIST) GT (ORD(T) - GDATA(IA,IGMINOPERATION_NOALWAYS,'GDMINOPERATION') + 24 ) AND
$ifi not '%NoOpTimeRestrictions%' == Yes                     (ORD(T_WITH_HIST) LE (ORD(T) + 24))),
$ifi not '%NoOpTimeRestrictions%' == Yes       VGSTARTUP_T(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS)
$ifi not '%NoOpTimeRestrictions%' == Yes       )
$ifi not '%NoOpTimeRestrictions%' == Yes    =L=
$ifi not '%NoOpTimeRestrictions%' == Yes    VGONLINE_T(T,IA,IGMINOPERATION_NOALWAYS) + epsilon
$ifi not '%NoOpTimeRestrictions%' == Yes    + VQONLOP(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS')
$ifi not '%NoOpTimeRestrictions%' == Yes ;
*}


* ---- QGONLSDMIP : minimum shut down time ---- *{

$ifi not '%NoOpTimeRestrictions%' == Yes QGONLSDMIP(T,IAGK_Y(IA,IGUC_NOALWAYS)) $IT_OPT(T)..

$ifi not '%NoOpTimeRestrictions%' == Yes SUM(T_WITH_HIST $( (ORD(T_WITH_HIST) GT (ORD(T)  - GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') + 24 )) AND
$ifi not '%NoOpTimeRestrictions%' == Yes                    (ORD(T_WITH_HIST) LE (ORD(T) + 24)) AND
$ifi not '%NoOpTimeRestrictions%' == Yes                    (GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') > 1)
$ifi not '%NoOpTimeRestrictions%' == Yes                  ),
$ifi not '%NoOpTimeRestrictions%' == Yes                  VGSTARTUP_T(T_WITH_HIST,IA,IGUC_NOALWAYS)
$ifi not '%NoOpTimeRestrictions%' == Yes                  )
$ifi not '%NoOpTimeRestrictions%' == Yes     =L=
$ifi not '%NoOpTimeRestrictions%' == Yes     1 - VGONLINE_T(T-GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN'),IA,IGUC_NOALWAYS)
$ifi not '%NoOpTimeRestrictions%' == Yes          + epsilon + VQONLOP(T,IA,IGUC_NOALWAYS,'IMINUS')
$ifi not '%NoOpTimeRestrictions%' == Yes ;



$ifi not '%NoStartUpCosts%' == Yes QGSTARTFUEL(T,IAGK_Y(IA,IGNOUCSCC))$IT_OPT(T)..

$ifi not '%NoStartUpCosts%' == Yes   VGSTARTUPFUELCONS(T,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes    =G=
$ifi not '%NoStartUpCosts%' == Yes   GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHOT')*IGELECCAPEFF(IA,IGNOUCSCC,T)*(
$ifi not '%NoStartUpCosts%' == Yes     VGONLINE_T(T,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes     - VGONLINE_T(T-1,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes   )
$ifi not '%NoStartUpCosts%' == Yes  ;

QUCELECSTO4(T,IAGK_Y(IA,IGELECSTORAGE))$IT_OPT(T)..

  VGONLINE_T(T,IA,IGELECSTORAGE) + VGONLCHARGE(T,IA,IGELECSTORAGE)
  =L=
  1
;



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
*TKMK EWL legacy  - VQESTOLOAD1(T,IA,IGELECSTORAGE,'IMINUS')
*TKMK EWL legacy  + IGSTOLOADCAPACITYMIN_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*VGONLCHARGE(T,IA,IGELECSTORAGE)*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
  + IGSTOLOADCAPACITYMIN_Y(IA,IGELECSTORAGE)*IGKDERATE(IA,IGELECSTORAGE,T)*(1-VGTURBINE_ON(T,IA,IGELECSTORAGE))*(1-IG_UNP_OUTAGE(IA,IGELECSTORAGE,T))
;


QUC18(T,IAGK_Y(IA,IGUC)) $IT_OPT(T) ..

    VGELEC_T(T,IA,IGUC)+ VGELEC_DPOS_T(T,IA,IGUC)- VGELEC_DNEG_T(T,IA,IGUC)
  + VGE_ANCPOS(T,IA,IGUC)$IGSPINNING(IGUC)
  + VGE_NONSPIN_ANCPOS(T,IA,IGUC)

  =L=

   VGELEC_T(T-1,IA,IGUC) + VGELEC_DPOS_T(T-1,IA,IGUC) - VGELEC_DNEG_T(T-1,IA,IGUC)
   + GDATA(IA,IGUC,'GDRAMPUP')*IGELECCAPEFF(IA,IGUC,T)*VGONLINE_T(T-1,IA,IGUC)
   + IGELECCAPEFF(IA,IGUC,T)*GDATA(IA,IGUC,'GDSTARTUPRAMP')* (VGONLINE_T(T,IA,IGUC)- VGONLINE_T(T-1,IA,IGUC))
   + IGELECCAPEFF(IA,IGUC,T)* (1-VGONLINE_T(T,IA,IGUC))

;

QUC20(T,IAGK_Y(IA,IGUC)) $IT_OPT(T)..
        VGELEC_T(T-1,IA,IGUC) + VGELEC_DPOS_T(T-1,IA,IGUC) - VGELEC_DNEG_T(T-1,IA,IGUC)
     - (VGELEC_T(T,IA,IGUC) + VGELEC_DPOS_T(T,IA,IGUC) - VGELEC_DNEG_T(T,IA,IGUC))
  =L=

   GDATA(IA,IGUC,'GDRAMPUP')* VGONLINE_T(T,IA,IGUC)*IGELECCAPEFF(IA,IGUC,T) +
   IGELECCAPEFF(IA,IGUC,T)*GDATA(IA,IGUC,'GDSTARTUPRAMP')*(VGONLINE_T(T-1,IA,IGUC)-VGONLINE_T(T,IA,IGUC))
   + IGELECCAPEFF(IA,IGUC,T)* (1-VGONLINE_T(T-1,IA,IGUC))

;

*============================================================================================================
$ifi not %UnitCscc%==yes $goto UnitC_label3

QUC12(T,ITALIAS_WITH_HIST,IAGK_Y(IA,IGUCSCC))$((ORD(ITALIAS_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')) AND
       ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (ORD(T) GT 1))..
   VGSTARTUPFUELCONS(T,IA,IGUCSCC)
   =G=
   SUM(i$(ORD(i)=ORD(ITALIAS_WITH_HIST)),UCSCCCOST(IGUCSCC,i))*(
   VGONLINE_T(T,IA,IGUCSCC)-VGONLINE_T(T-1,IA,IGUCSCC)
   )
;

$ontext
Original stochastic TSO version
QUC12(T,ITALIAS_WITH_HIST,,IAGK_Y(IA,IGUCSCC),SCEN)$((ORD(ITALIAS_WITH_HIST) LE GDATA(IA,IGUCSCC,'GDUCSCCC')) AND
       ((ORD(T)-1) LE INO_HOURS_OPT_PERIOD) AND (ORD(T) GT 1))..
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGSTARTUPFUELCONS(T,IA,IGUCSCC,NODE))
   =G=
   SUM(i$(ORD(i)=ORD(ITALIAS_WITH_HIST)),UCSCCCOST(IGUCSCC,i))*(
   SUM(NODE$(SCEN_NODE(SCEN,NODE) AND INT(NODE,T)),VGONLINE_NT(IA,IGUCSCC,NODE,T))-
   SUM((INODEALIAS,italias)$((ORD(italias)<= ORD(ITALIAS_WITH_HIST)) AND SCEN_NODE(SCEN,INODEALIAS) AND INT_WITH_HIST(INODEALIAS,T-ORD(italias))),
      VGONLINE_NT(IA,IGUCSCC,INODEALIAS,T-ORD(italias)))
   )
;
$offtext

$label  UnitC_label3
