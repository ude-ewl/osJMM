* File: print-OUT_VQDAYAHEAD.inc

* This file will print the marginal value of equation QEEQDAY, which is
* interpreted as the price on the day-ahead market.
* VQDAYAHEAD depends on the sets RRR and T

PUT OUT_VQDAYAHEAD_OUT;
OUT_VQDAYAHEAD_OUT.PW = 32767;
OUT_VQDAYAHEAD_OUT.PS = 1;
OUT_VQDAYAHEAD_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,IPLUSMINUS,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
        LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
    AND (ORD(BASETIME) <= IBASEFINALTIME-12) ),    
                     
           LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
               LOOP (R_RId(IR, RId),
                 LOOP(IPLUSMINUS$VQDAYAHEAD.L(T,IR,IPLUSMINUS),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",IPLUSMINUS.TL:0,",",VQDAYAHEAD.L(T,IR,IPLUSMINUS):0:4/;
                );
               );
            );
         );
     );
   );
);



$ontext
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      LOOP (IR,
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE3') )
                         AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE3')) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1))  ),
               LOOP (R_RId(IR, RId),
                 LOOP(IPLUSMINUS$VQDAYAHEAD.L(T,IR,IPLUSMINUS),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",IPLUSMINUS.TL:0,",",VQDAYAHEAD.L(T,IR,IPLUSMINUS):0:4/;
                );
               );
            );
         );
     );
   );
);
$offtext
