* File: print-OUT_VCONTENTHYDRORES_NT_ct.inc

* This file will print the root node value of the variable VCONTENTHYDRORES_NT,
* which is the content of hydro reservoirs
* VCONTENTHYDRORES_NT depends on the sets AAA, GGG, Node and T

PUT OUT_VCONTENTHYDRORES_NT_OUT_ct;
OUT_VCONTENTHYDRORES_NT_OUT_ct.PW = 32767;
OUT_VCONTENTHYDRORES_NT_OUT_ct.PS = 1;
OUT_VCONTENTHYDRORES_NT_OUT_ct.nJ = 2;


* Headline: Generator name
if (INO_SOLVE EQ 1,
   Put "BaseTime";
     LOOP(IAGK_Y(IA,G)$(Sum(IGHYDRORES,IGELECCAPACITY_Y(IA,IGHYDRORES))>0),
            Put ",", G.TL:0;
     );
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE

LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
         LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
                                           PUT / BASETIME.TL:0;
          LOOP (IA$(Sum(IGHYDRORES,IGELECCAPACITY_Y(IA,IGHYDRORES))>0),
                        LOOP (A_AId(IA,AId),
                  Put ",",VCONTENTHYDRORES_T.l(T,IA) :0:2;
            );
         );
      );
   );
