PUT HLINK_DC_OUT;
HLINK_DC_OUT.PW = 1800;
HLINK_DC_OUT.PS = 1;
HLINK_DC_OUT.nJ = 2;

* -----------------------------------------------------------------------------

$ifi '%LFB%'==  Yes  if (INO_SOLVE EQ 1,
$ifi '%LFB%' ==  Yes   Put "BaseTime";
$ifi '%LFB%' ==  Yes    LOOP (IEXPORT_FLBMC_DC_Y(IR,IRE),
$ifi '%LFB%' ==  Yes     LOOP(R_RId(IR,RId),
$ifi '%LFB%' ==  Yes      Put ",",IR.TL:0,"-";
$ifi '%LFB%' ==  Yes     );
$ifi '%LFB%' ==  Yes      LOOP(R_RId(IRE,RId),
$ifi '%LFB%' ==  Yes       Put IRE.TL:0 ;
$ifi '%LFB%' ==  Yes      );
$ifi '%LFB%' ==  Yes     );

$ifi '%LFB%' ==  Yes   Put /;
$ifi '%LFB%' ==  Yes    );

$ifi '%LFB%' ==  Yes  LOOP (CaseId,
$ifi '%LFB%' ==  Yes   LOOP(BASETIME$( ( ORD(BASETIME) >= IBASESTARTTIME)
$ifi '%LFB%' ==  Yes                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
$ifi '%LFB%' ==  Yes      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
$ifi '%LFB%' ==  Yes                 Put BASETIME.TL:0;
$ifi '%LFB%' ==  Yes           LOOP (IEXPORT_FLBMC_DC_Y(IR,IRE),
$ifi '%LFB%' ==  Yes                 LOOP(R_RId(IR,RId),
$ifi '%LFB%' ==  Yes                 );
$ifi '%LFB%' ==  Yes                 LOOP(R_RId(IRE,RId),
$ifi '%LFB%' ==  Yes                    Put "," , (VXELEC_DC_T.l (T,IR,IRE)+VXELEC_DC_DPOS_T.l(T,IR,IRE)-VXELEC_DC_DNEG_T.l(T,IR,IRE)):0:2;
$ifi '%LFB%' ==  Yes                 );
$ifi '%LFB%' ==  Yes           );
$ifi '%LFB%' ==  Yes      Put /;  );
$ifi '%LFB%' ==  Yes     );
$ifi '%LFB%' ==  Yes  );

