* File: print-OUT_VQEEQINT.inc

* This file will print the marginal value of equation VQEEQINT, which is
* interpreted as the electricity price on the intraday market.
* VQEEQINT depends on the sets RRR, Node and T

PUT OUT_VQEEQINT_OUT;
OUT_VQEEQINT_OUT.PW = 32767;
OUT_VQEEQINT_OUT.PS = 1;
OUT_VQEEQINT_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,IPLUSMINUS,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP (IR,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (R_RId(IR, RId),
            LOOP(IPLUSMINUS $VQINTRADAY.L(T,IR,IPLUSMINUS),
              Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
              IPLUSMINUS.TL:0,",",VQINTRADAY.L(T,IR,IPLUSMINUS):0:4/ ;
            );
            );
        );
     );
  );
);