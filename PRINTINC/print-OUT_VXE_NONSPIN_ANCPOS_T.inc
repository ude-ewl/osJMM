* File: print-OUT_VXE_NONSPIN_ANCPOS_T.inc

* This file will print the value of VXE_NONSPIN_ANCPOS
* VXE_NONSPIN_ANCPOS_T depends on the sets RRR, RRR, and T.

PUT OUT_VXE_NONSPIN_ANCPOS_T_OUT;
OUT_VXE_NONSPIN_ANCPOS_T_OUT.PW = 32767;
OUT_VXE_NONSPIN_ANCPOS_T_OUT.PS = 1;
OUT_VXE_NONSPIN_ANCPOS_T_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region1,ID_Region2,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
         LOOP (IEXPORT_Y(IR,IRE),
            if ((VXE_NONSPIN_ANCPOS.l(T,IR,IRE)>0),
               Put CaseId.TL:0,",";
               LOOP(R_RId(IR,RId),
                  Put RId.TL:0,",";
               );
               LOOP(R_RId(IRE,RId),
                  Put RId.TL:0,",",BASETIME.TL:0,",",VXE_NONSPIN_ANCPOS.l(T,IR,IRE) :0:4/ ;
               );
            );
         );
      );
   );
);