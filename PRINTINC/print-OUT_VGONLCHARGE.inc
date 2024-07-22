* File: print-OUT_VGONLCHARGE.inc

PUT VGONLCHARGE_OUT;
VGONLCHARGE_OUT.PW = 32767;
VGONLCHARGE_OUT.PS = 1;
VGONLCHARGE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

*-------------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed


LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGELECSTORAGE),
$ontext
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE1') )
                       AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE1')) ),
         LOOP (T  $( ((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1)) AND
                       ((ORD(T)-1) <= IDURATION_STAGE('STAGE1'))  ),
$offtext
        LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
           LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGELECSTORAGE,GId),
                  if (VGONLCHARGE.l(T,IA,IGELECSTORAGE)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VGONLCHARGE.l(T,IA,IGELECSTORAGE) :0:4/ ;
                  );
               );
           );
        );
      );
   );
);