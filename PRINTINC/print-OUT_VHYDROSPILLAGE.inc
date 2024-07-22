* File: print-OUT_VHYDROSPILLAGE.inc

PUT OUT_VHYDROSPILLAGE_OUT;
OUT_VHYDROSPILLAGE_OUT.PW = 32767;
OUT_VHYDROSPILLAGE_OUT.PS = 1;
OUT_VHYDROSPILLAGE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA and T


LOOP (CaseId,
   LOOP (IAHYDRO,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
             LOOP(A_AId(IAHYDRO,AId),
                if(VHYDROSPILLAGE.l(T,IAHYDRO)>0,
                   Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",",
                   VHYDROSPILLAGE.l(T,IAHYDRO) :0:4/ ;
                );
             );
         );
      );
   );
);