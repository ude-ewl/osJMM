* File: print-OUT_IDEMAND_ANCNEG.inc

PUT OUT_IDEMAND_ANCNEG_OUT;
OUT_IDEMAND_ANCNEG_OUT.PW = 32767;
OUT_IDEMAND_ANCNEG_OUT.PS = 1;
OUT_IDEMAND_ANCNEG_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,RegionID,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------

LOOP (CaseId,
  LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                      AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
        LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
              LOOP(TSO_RRR,
*              LOOP(R_RID(IR,RID),
                    if(Idemand_AncNEG(TSO_RRR,T)>0,
                       Put CaseId.TL:0,",",TSO_RRR.TL:0,",",BASETIME.TL:0,",",Idemand_ANCNEG(TSO_RRR,T):0:4/ ;
                 );
              );
        );
  );
);
