* Unit commitment add-on to Wilmar
* Variables and equations


*============================================================================================================
*============================================================================================================
$ifi %UnitCmin%==yes    EQUATION  QGONLOPMIP(T,AAA,G)
$ifi %UnitCmin%==yes    EQUATION  QGONLSDMIP(T,AAA,G)


$ifi %UnitCmin%==yes    EQUATION  QGSTARTFUEL(T,AAA,G)  'Start-up fuel consumption for power plants without outtime dependant start-up fuel consumption';
$ifi %UnitCmin%==yes    EQUATION  QUCELECSTO4(T,AAA,G)   'Equation securing that the pumped hydro station is either in generating or pumping mode';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD1(T,AAA,G)   'Loading (pumping) lower than loading capacity';
$ifi %UnitCmin%==yes    EQUATION  QESTOLOAD2(T,AAA,G)   'Loading (pumping) higher than minimum loading capacity';


*============================================================================================================


* ---- QGONLOPMIP: minimum operation time ---- *{

QGONLOPMIP(T,IAGK_Y(IA,IGMINOPERATION_NOALWAYS))$IT_OPT(T)..

   SUM(T_WITH_HIST $(ORD(T_WITH_HIST) GT (ORD(T) - GDATA(IA,IGMINOPERATION_NOALWAYS,'GDMINOPERATION') + 24 ) AND
                    (ORD(T_WITH_HIST) LE (ORD(T) + 24))),
      VGSTARTUP_T(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS)
      )
   =L=
   VGONLINE_T(T,IA,IGMINOPERATION_NOALWAYS) + epsilon
   + VQONLOP(T,IA,IGMINOPERATION_NOALWAYS,'IPLUS')

*   VGONLINE_T(T,IA,IGMINOPERATION_NOALWAYS)
*   =G=
*   VGONLINE_T(T-1,IA,IGMINOPERATION_NOALWAYS,T-1) - VGONLINE_T(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS)
*   - VQONLOP(T_WITH_HIST,IA,IGMINOPERATION_NOALWAYS,'IPLUS')
;
*}


* ---- QGONLSDMIP : minimum shut down time ---- *{


QGONLSDMIP(T,IAGK_Y(IA,IGUC_NOALWAYS)) $IT_OPT(T)..

SUM(T_WITH_HIST $( (ORD(T_WITH_HIST) GT (ORD(T)  - GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') + 24 )) AND
                   (ORD(T_WITH_HIST) LE (ORD(T) + 24)) AND
                   (GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN') > 1)
                 ),
                 VGSTARTUP_T(T_WITH_HIST,IA,IGUC_NOALWAYS)
                 )
    =L=
    1 - VGONLINE_T(T-GDATA(IA,IGUC_NOALWAYS,'GDMINSHUTDOWN'),IA,IGUC_NOALWAYS)
         + epsilon + VQONLOP(T,IA,IGUC_NOALWAYS,'IMINUS')

*  (1-IG_UNP_OUTAGE(IA,IGUC_NOALWAYS,T)) *
*  (IGELECCAPACITY_Y(IA,IGUC_NOALWAYS)+2*Epsilon
*   - ( VGONLINE_T(T_WITH_HIST,IA,IGUC_NOALWAYS) - VGONLINE_T(T-1,IA,IGUC_NOALWAYS))
*  )
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


