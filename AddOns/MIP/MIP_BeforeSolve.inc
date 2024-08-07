$ontext
IF ((INO_SOLVE EQ 0),
   LOOP((IA,IGUC)$IAGK_Y(IA,IGUC),
      If((GSTARTVALUEDATA(IA,IGUC,'GSSDTIME')>0),
         UCINI(IA,IGUC)= -GSTARTVALUEDATA(IA,IGUC,'GSSDTIME')
      );
      IF((GSTARTVALUEDATA(IA,IGUC,'GSOPTIME')>0),
         UCINI(IA,IGUC)= GSTARTVALUEDATA(IA,IGUC,'GSOPTIME')
      );
      LOOP (T_WITH_HIST $(ORD(T_WITH_HIST)<=25),
         VGONLINE_T.FX(T_WITH_HIST,IA,IGUC)= 0
      );
   );
);
$offtext

IF ((INO_SOLVE EQ 0),
 LOOP( IAGK_Y(IA,IGUC)$(Start_OnOff(IA,IGUC)= 0),
   LOOP (T_WITH_HIST $(ORD(T_WITH_HIST)<=25),
    UCINI(IA,IGUC) = - 100;
   );
 );
 LOOP( IAGK_Y(IA,IGUC)$(Start_OnOff(IA,IGUC)=1),
   LOOP (T_WITH_HIST $(ORD(T_WITH_HIST)<=25),
    VGONLINE_T.FX(T_WITH_HIST,IA,IGUC)= 1;
    UCINI(IA,IGUC) = 100;
   );
 );
);


LOOP( IAGK_Y(IA,IGUC),
   IUCMININIU(IA,IGUC)=MAX(0,MIN(CARD(T),0+(GDATA(IA,IGUC,'GDMINOPERATION')-UCINI(IA,IGUC))$(UCINI(IA,IGUC) GT 0)));
   IUCMININID(IA,IGUC)=MAX(0,MIN(CARD(T),0+(GDATA(IA,IGUC,'GDMINSHUTDOWN')+UCINI(IA,IGUC))$(UCINI(IA,IGUC) LT 0)));
);



* reduce  IUCMININIU when a new outage occurs
$ontext
LOOP( IAGK_Y(IA,IGUC) $IGELEC_UNP_OUTAGE(IA,IGUC),
   IF ( IG_UNP_OUTAGE(IA,IGUC,'T01') EQ 1,
      IUCMININIU(IA,IGUC) =0) ;
   IF ((IG_UNP_OUTAGE(IA,IGUC,'T01') EQ 0) AND (IG_UNP_OUTAGE(IA,IGUC,'T02') EQ 1) ,
      IUCMININIU(IA,IGUC) =MIN (1,IUCMININIU(IA,IGUC))) ;
   IF ((IG_UNP_OUTAGE(IA,IGUC,'T01') EQ 0) AND (IG_UNP_OUTAGE(IA,IGUC,'T02') EQ 0)
      AND (IG_UNP_OUTAGE(IA,IGUC,'T03') EQ 1) ,
     IUCMININIU(IA,IGUC) =MIN (2,IUCMININIU(IA,IGUC))) ;
);
$offtext

*LOOP( IAGK_Y(IA,IGUC)$IGELEC_UNP_OUTAGE(IA,IGUC),
LOOP( IAGK_Y(IA,IGUC),
 LOOP(T,
  IF ( IG_UNP_OUTAGE(IA,IGUC,T) EQ 1,
   IUCMININIU(IA,IGUC) = MIN(ORD(T),IUCMININIU(IA,IGUC))
   );
* reduce  IUCMININIU when revision occurs
  IF ( IGkderate(IA,IGUC,T) EQ 0,
   IUCMININIU(IA,IGUC) = MIN(ORD(T),IUCMININIU(IA,IGUC))
  );
 );
);

*increase initial remaining downtime for CHP
*Avoid problems with equation QUC25
LOOP( IAGK_Y(IA,IGCHPBOUNDE),
 LOOP(T$((ORD(T)+1) < GDATA(IA,IGCHPBOUNDE,'GDMINSHUTDOWN') AND ORD(T) > 2) ,
  IF ( (IMINCHPFACTOR(IA,IGCHPBOUNDE,T) > 0) AND (SUM(ITALIAS$((ORD(T)-ORD(ITALIAS))=1),(IMAXCHPFACTOR(IA,IGCHPBOUNDE,ITALIAS) = 0) ) OR SUM(ITALIAS$((ORD(T)-ORD(ITALIAS))=1),(IGKDERATE(IA,IGCHPBOUNDE,ITALIAS) = 0) ) ),
   IUCMININID(IA,IGCHPBOUNDE) = ORD(T)-2;
   );
 );
);

LOOP( IAGK_Y(IA,IGCHPBOUNDE),
 LOOP(T$((ORD(T)+1) < GDATA(IA,IGCHPBOUNDE,'GDMINOPERATION') AND ORD(T) > 2) ,
  IF ( (IMAXCHPFACTOR(IA,IGCHPBOUNDE,T) = 0) AND SUM(ITALIAS$((ORD(T)-ORD(ITALIAS))=1),(IMINCHPFACTOR(IA,IGCHPBOUNDE,ITALIAS) > 0) ),
   IUCMININIU(IA,IGCHPBOUNDE) = ORD(T)-2;
   );
 );
);

display 'xxx'
display IUCMININID, IUCMININIU;

* Update the start-up time of a unit dependant on how many hours the unit has been offline at the
* beginning of a planning loop
* IGSTARTUPTIME varying between planning loops give infeasibilities for some unknown reason
* Therefore the code below is not used at present
$ontext
LOOP((IA,IGUC)$IAGK_Y(IA,IGUC),
   IF(UCINI(IA,IGUC)GE 0,
      IGSTARTUPTIME(IA,IGUC)=GDATA(IA,IGUC,'GDLEADTIMEHOT');
   else
      IF((-UCINI(IA,IGUC)) GE GDATA(IA,IGUC,'GDHOTWARM'),
         IF((-UCINI(IA,IGUC)) GE (GDATA(IA,IGUC,'GDHOTWARM')+GDATA(IA,IGUC,'GDWARMCOLD')),
            IGSTARTUPTIME(IA,IGUC)=GDATA(IA,IGUC,'GDLEADTIMECOLD');
         else
            IGSTARTUPTIME(IA,IGUC)=GDATA(IA,IGUC,'GDLEADTIMEWARM');
         );
      else
         IGSTARTUPTIME(IA,IGUC)=GDATA(IA,IGUC,'GDLEADTIMEHOT');
      );
   );
);


display IGSTARTUPTIME;
$offtext
