* File: print-OUT_VXELEC_T.inc

* This file will print the value of PTDFCAP_SLACK
* PTDFCAP_SLACK depends on the sets RRR, RRR, and T.

PUT OUT_PTDFCAP_SLACK_OUT;
OUT_PTDFCAP_SLACK_OUT.PW = 32767;
OUT_PTDFCAP_SLACK_OUT.PS = 1;
OUT_PTDFCAP_SLACK_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region1,ID_Region2,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
         LOOP ((IRE,IRI),
            if ((PTDFCAP_SLACK.l(IRE,IRI,T)>0),
               Put CaseId.TL:0,",";
               LOOP(R_RId(IRE,RId),
                  Put RId.TL:0,",";
               );
               LOOP(R_RId(IRI,RId),
                  Put RId.TL:0,",",BASETIME.TL:0,",",PTDFCAP_SLACK.l(IRE,IRI,T) :0:4/ ;
               );
            );
         );
      );
   );
);




