* File: print-OUT_PTDFMCPLUS_DAY.csv

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* QEEQDAY_M depends on the sets RRR and T

PUT OUT_PTDFMCPLUS_DAY;
OUT_PTDFMCPLUS_DAY.PW = 32767;
OUT_PTDFMCPLUS_DAY.PS = 1;
OUT_PTDFMCPLUS_DAY.nJ = 2;

if (INO_SOLVE EQ 1,
 PUT "CaseId,ID_Region1,ID_Region2,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               LOOP (R_RId(IR, RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",QXPTDF1Plus.M(IR,IR,T):0:4/;

LOOP(R_RId(IR,RId),
                    Put RId.TL:0,",";
                );
                 LOOP(R_RId(IRE,RId),
                 Put RId.TL:0,",",BASETIME.TL:0,",",VXELEC_T.l(T,IR,IRE) :0:4/ ;

               );
            );
         );
     );
   );
);


$ifi '%No_Load_Flow%' ==  Yes  LOOP (CaseId,
$ifi '%No_Load_Flow%' ==  Yes     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
$ifi '%No_Load_Flow%' ==  Yes                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
$ifi '%No_Load_Flow%' ==  Yes        LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
$ifi '%No_Load_Flow%' ==  Yes           LOOP (IEXPORT_Y(IR,IRE),
$ifi '%No_Load_Flow%' ==  Yes              if ((VXELEC_T.l(T,IR,IRE)>0),
$ifi '%No_Load_Flow%' ==  Yes                 Put CaseId.TL:0,",";
$ifi '%No_Load_Flow%' ==  Yes                 LOOP(R_RId(IR,RId),
$ifi '%No_Load_Flow%' ==  Yes                    Put RId.TL:0,",";
$ifi '%No_Load_Flow%' ==  Yes                 );
$ifi '%No_Load_Flow%' ==  Yes                 LOOP(R_RId(IRE,RId),
$ifi '%No_Load_Flow%' ==  Yes                    Put RId.TL:0,",",BASETIME.TL:0,",",VXELEC_T.l(T,IR,IRE) :0:4/ ;
$ifi '%No_Load_Flow%' ==  Yes                 );
$ifi '%No_Load_Flow%' ==  Yes              );
$ifi '%No_Load_Flow%' ==  Yes           );
$ifi '%No_Load_Flow%' ==  Yes        );
$ifi '%No_Load_Flow%' ==  Yes     );
$ifi '%No_Load_Flow%' ==  Yes  );
