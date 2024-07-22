* File: print-OUT_VCONTENTHYDRORES_NT.inc

* This file will print the root node value of the variable VCONTENTHYDRORES_T,
* which is the content of hydro reservoirs
* VCONTENTHYDRORES_T depends on the sets AAA, GGG and T

PUT OUT_VCONTENTHYDRORES_NT_OUT;
OUT_VCONTENTHYDRORES_NT_OUT.PW = 32767;
OUT_VCONTENTHYDRORES_NT_OUT.PS = 1;
OUT_VCONTENTHYDRORES_NT_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaID,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
   LOOP (IA$(Sum(IGHYDRORES,IGELECCAPACITY_Y(IA,IGHYDRORES))>0),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
            LOOP (A_AId(IA,AId),
               if ((VCONTENTHYDRORES_T.l(T,IA)>0),
                  Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",",
                  VCONTENTHYDRORES_T.l(T,IA) :0:4/ ;
               );
            );
         );
      );
   );
);