* File: print-OUT_VELEC_CONTENTSTORAGE_2.inc

PUT VELEC_CONTENTSTORAGE_OUT_2;
VELEC_CONTENTSTORAGE_OUT_2.PW = 32767;
VELEC_CONTENTSTORAGE_OUT_2.PS = 1;
VELEC_CONTENTSTORAGE_OUT_2.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,AreaId,PS,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),

          LOOP (IAPSK_Y(IAELECSTO,PS),
             LOOP(A_AId(IAELECSTO,AId),
*                if( VCONTENTSTORAGE_T_2.l(T,IAELECSTO,PS)>0,
                  Put CaseId.TL:0,",",AId.TL:0,",",PS.TL:0,",",BASETIME.TL:0,",",
                  VCONTENTSTORAGE_T_2.l(T,IAELECSTO,PS) :0:4/ ;
*                );
             );
          );
*$if %PlugIn% == Yes $include '../../base/model/PlugInAddOn/print-OUT_plugin.inc'
     );
   );

);