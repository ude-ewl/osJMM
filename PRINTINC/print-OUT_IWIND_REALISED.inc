* File: print-OUT_IWIND_REALISED.inc

* This file will print the root node value of the unregulated production
* equal to the wind power
* IWIND_REALISED_IR depends on the sets RRR, Node and T

PUT OUT_IWIND_REALISED_OUT;
OUT_IWIND_REALISED_OUT.PW = 32767;
OUT_IWIND_REALISED_OUT.PS = 1;
OUT_IWIND_REALISED_OUT.nJ = 2;
if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                   AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
         LOOP (IR,
              HELPVAR6(IR,T)=SUM(IAGK_Y(IA,IGWIND)$RRRAAA(IR,IA),vgelec_dneg_t.l(T,IA,IGWIND));
              LOOP (R_RId(IR,RId),
                Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
               (IWIND_REALISED_IR(IR,T)-HELPVAR6(IR,T)
$ifi '%Renewable_spinning%' == YES  -VWINDCUR_ANCPOS.l(T,IR)
                                                                  ):0:4/ ;
              );
         );
         
      );
   );
);
