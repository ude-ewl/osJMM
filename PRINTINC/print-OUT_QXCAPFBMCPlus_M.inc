* File: print-OUT_QXCAPFBMCPlus.inc

* This file will print the value of VXELEC_QXCAPFBMCPlusT
* QXCAPFBMCPlus depends on the sets ISFD_Y_FLG and T.

PUT OUT_QXCAPFBMCPlus_M_OUT;
OUT_QXCAPFBMCPlus_M_OUT.PW = 1255;
OUT_QXCAPFBMCPlus_M_OUT.PS = 1;
OUT_QXCAPFBMCPlus_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,FLG,BaseTime,Value" /;
);


* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (ISFD_Y_FLG(FLG),
              if ((abs(QXCAPFBMCPlus.M(T,ISFD_Y_FLG))>EPSILON),
                 Put CaseId.TL:0,",";
                 Put ISFD_Y_FLG.TL:0,",",BASETIME.TL:0,",",(QXCAPFBMCPlus.M(T,ISFD_Y_FLG)) :0:4/ ;
              );
           );
        );
     );
);
