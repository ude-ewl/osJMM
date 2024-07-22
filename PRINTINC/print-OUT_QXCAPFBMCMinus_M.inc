* File: print-OUT_QXCAPFBMCMinus.inc

* This file will print the value of QXCAPFBMCMinus
* QXCAPFBMCMinus depends on the sets INSFD_Y_FLG and T.

PUT OUT_QXCAPFBMCMinus_M_OUT;
OUT_QXCAPFBMCMinus_M_OUT.PW = 1255;
OUT_QXCAPFBMCMinus_M_OUT.PS = 1;
OUT_QXCAPFBMCMinus_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,FLG,BaseTime,Value" /;
);


* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (INSFD_Y_FLG(FLG),
              if ((abs(QXCAPFBMCMinus.M(T,INSFD_Y_FLG))>EPSILON),
                 Put CaseId.TL:0,",";
                 Put INSFD_Y_FLG.TL:0,",",BASETIME.TL:0,",",(QXCAPFBMCMinus.M(T,INSFD_Y_FLG)) :0:4/ ;
              );
           );
        );
     );
);
