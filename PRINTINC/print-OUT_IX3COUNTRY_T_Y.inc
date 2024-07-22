* File: print-OUT_IX3COUNTRY_T_Y.inc

* This file will print the time step in the model parameter IX3COUNTRY_T_Y

PUT OUT_IX3COUNTRY_T_Y_OUT;
OUT_IX3COUNTRY_T_Y_OUT.PW = 32767;
OUT_IX3COUNTRY_T_Y_OUT.PS = 1;
OUT_IX3COUNTRY_T_Y_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
         LOOP (IR,
            LOOP (R_RId(IR,RId),
              PUT CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",IX3COUNTRY_T_Y(IR,T):0:4/;
            );
         );
      );
   );
);