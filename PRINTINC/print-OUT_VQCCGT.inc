* File: print-OUT_VQCCGT.inc

PUT OUT_VQCCGT_OUT;
OUT_VQCCGT_OUT.PW = 32767;
OUT_VQCCGT_OUT.PS = 1;
OUT_VQCCGT_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,CCGT_Group,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA and T


LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
             LOOP(CCGT_Group,
               LOOP(IPLUSMINUS$VQCCGT.l(T,CCGT_Group,IPLUSMINUS),
                if(VQCCGT.l(T,CCGT_Group,IPLUSMINUS)>0,
                   Put CaseId.TL:0,",",CCGT_Group.TL:0,",",BASETIME.TL:0,",",IPLUSMINUS.TL:0,",",
                   VQCCGT.l(T,CCGT_Group,IPLUSMINUS) :0:4/ ;
                );
               );
             );
      );
   );
);
