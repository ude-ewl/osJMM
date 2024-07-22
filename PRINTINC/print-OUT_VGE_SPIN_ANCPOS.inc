* File: print-OUT_VGE_SPIN_ANCPOS.inc

PUT VGE_SPIN_ANCPOS_OUT;
VGE_SPIN_ANCPOS_OUT.PW = 32767;
VGE_SPIN_ANCPOS_OUT.PS = 1;
VGE_SPIN_ANCPOS_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
 PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed


LOOP (CaseId,
 LOOP (IAGK_Y(IA,IGELEC),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP(A_AId(IA,AId),
              LOOP(G_GId(IGELEC,GId),
                if(VGE_SPIN_ANCPOS.l(T,IA,IGELEC)>0,
                   Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                   VGE_SPIN_ANCPOS.l(T,IA,IGELEC) :0:4/ ;
                );
             );
          );
       );
    );
 );
);
