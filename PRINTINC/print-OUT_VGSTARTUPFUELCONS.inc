* File: print-OUT_VGSTARTUPFUELCONS.inc

PUT VGSTARTUPFUELCONS_OUT;
VGSTARTUPFUELCONS_OUT.PW = 32767;
VGSTARTUPFUELCONS_OUT.PS = 1;
VGSTARTUPFUELCONS_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed


LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGUC),
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
                LOOP(G_GId(IGUC,GId),
                  if(VGSTARTUPFUELCONS.l(T,IA,IGUC)>0,
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                     VGSTARTUPFUELCONS.l(T,IA,IGUC) :0:4/ ;
                  );
               );
            );
         );
      );
   );

);