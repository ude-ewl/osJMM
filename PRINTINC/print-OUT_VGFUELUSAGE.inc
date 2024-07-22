* File: print-OUT_VGFUELUSAGE.inc

PUT VGFUELUSAGE_OUT;
VGFUELUSAGE_OUT.PW = 32767;
VGFUELUSAGE_OUT.PS = 1;
VGFUELUSAGE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);
*-------------------------------------------------------------------------------
* variables depending on AAA, GGG, Node and T. Only root node values are printed

LOOP (CaseId,
   LOOP (IAGK_Y(IA,IGUSINGFUEL),
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
         LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),            
            LOOP(A_AId(IA,AId),
               LOOP(G_GId(IGUSINGFUEL,GId),
                  if (vgfuelusage_t.l(T,IA,IGUSINGFUEL)>0,
                     Put CaseId.TL:0,",", GId.TL:0,",",BASETIME.TL:0,",",
                     vgfuelusage_t.l(T,IA,IGUSINGFUEL) :0:4/ ;
                  );
               );
            );
       );
     );
   );
);


$ontext
$ifi '%DISGEN_FIXED%' == Yes LOOP (CaseId,
$ifi '%DISGEN_FIXED%' == Yes   LOOP (IAGK_Y(IA,IGDISGEN),
$ifi '%DISGEN_FIXED%' == Yes     LOOP(BASETIME  $( (ORD(BASETIME) >= IBASESTARTTIME('STAGE1'))
$ifi '%DISGEN_FIXED%' == Yes                       AND ( ORD(BASETIME) <= IBASEFINALTIME('STAGE1'))
$ifi '%DISGEN_FIXED%' == Yes                     ) ,
$ifi '%DISGEN_FIXED%' == Yes      LOOP (T  $( ((ORD(T)-1) eq (ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1)) AND
$ifi '%DISGEN_FIXED%' == Yes                       ((ORD(T)-1) <= IDURATION_STAGE('STAGE1'))
$ifi '%DISGEN_FIXED%' == Yes                ) ,
$ifi '%DISGEN_FIXED%' == Yes            LOOP(A_AId(IA,AId),
$ifi '%DISGEN_FIXED%' == Yes               LOOP(G_GId(IGDISGEN,GId),
$ifi '%DISGEN_FIXED%' == Yes                 LOOP(IR $RRRAAA(IR,IA),
$ifi '%DISGEN_FIXED%' == Yes                    if (IDISGEN_VAR_T(IR,T)>0,
$ifi '%DISGEN_FIXED%' == Yes                      helpvar = (1/GDATA(IA,IGDISGEN,'GDFULLLOAD'))*IDISGEN_VAR_T(IR,T)  ;
$ifi '%DISGEN_FIXED%' == Yes                      Put CaseId.TL:0,",", GId.TL:0,",",BASETIME.TL:0,",",
$ifi '%DISGEN_FIXED%' == Yes                      helpvar:0:4/ ;
$ifi '%DISGEN_FIXED%' == Yes                    );
$ifi '%DISGEN_FIXED%' == Yes                  );
$ifi '%DISGEN_FIXED%' == Yes               );
$ifi '%DISGEN_FIXED%' == Yes             );
$ifi '%DISGEN_FIXED%' == Yes       );
$ifi '%DISGEN_FIXED%' == Yes     );
$ifi '%DISGEN_FIXED%' == Yes   );
$ifi '%DISGEN_FIXED%' == Yes );
$offtext