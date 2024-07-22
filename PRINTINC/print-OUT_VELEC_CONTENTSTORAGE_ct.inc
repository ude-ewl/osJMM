* File: print-OUT_VELEC_CONTENTSTORAGE_ct.inc

PUT VELEC_CONTENTSTORAGE_OUT_ct;
VELEC_CONTENTSTORAGE_OUT_ct.PW = 32767;
VELEC_CONTENTSTORAGE_OUT_ct.PS = 1;
VELEC_CONTENTSTORAGE_OUT_ct.nJ = 2;

if (INO_SOLVE EQ 1,
   Put "BaseTime";
   LOOP(G(IGESTORAGE_DSM), Put ",",G.TL:0;
   );
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
							PUT / BASETIME.TL:0;
							
          LOOP (IAGK_Y(IA,IGESTORAGE_DSM),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGESTORAGE_DSM,GId),
                     Put ",",VCONTENTSTORAGE_T.l(T,IA,IGESTORAGE_DSM) :0:2;
               );
            );
         );
* $if %PlugIn% == Yes $include '../../base/model/PlugInAddOn/print-OUT_plugin.inc'
     );
   );

