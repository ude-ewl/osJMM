* File: print-OUT_HLINK.inc

* This file will print the value of VXELEC_T_Realised in table format 
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.

PUT HLINK_OUT;
HLINK_OUT.PW = 32767;
HLINK_OUT.nJ = 2;

* -----------------------------------------------------------------------------
* HEAD LINE
						if (INO_SOLVE EQ 1,
								Put "BaseTime";
						   LOOP (IEXPORT_Y(IR,IRE),
								LOOP(R_RId(IR,RId),
								 Put ",",IR.TL:0,"-";
								 );
								 LOOP(R_RId(IRE,RId),
									Put IRE.TL:0 ;
								 );
						   );
		   
$ifi '%LFB_BEX%' ==  Yes   LOOP (IEXPORT_FLBMC_Y(IR,IRE),
$ifi '%LFB_BEX%' ==  Yes        LOOP(R_RId(IR,RId),
$ifi '%LFB_BEX%' ==  Yes         Put ",",IR.TL:0,"-";
$ifi '%LFB_BEX%' ==  Yes         );
$ifi '%LFB_BEX%' ==  Yes         LOOP(R_RId(IRE,RId),
$ifi '%LFB_BEX%' ==  Yes            Put IRE.TL:0 ;
$ifi '%LFB_BEX%' ==  Yes		 );
$ifi '%LFB_BEX%' ==  Yes   );
		   
						  Put /;
						   );  


LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
               Put BASETIME.TL:0;

			   LOOP (IEXPORT_Y(IR,IRE),
                LOOP(R_RId(IR,RId),
                 );
                 LOOP(R_RId(IRE,RId),
                  Put ",",(   VXELEC_T.l      (T,IR,IRE)
                        + VXELEC_DPOS_T.l(T,IR,IRE)
                        - VXELEC_DNEG_T.l(T,IR,IRE)) :0:2 ;
                 );
               );
$ifi '%LFB_BEX%' ==  Yes LOOP (IEXPORT_FLBMC_Y(IR,IRE),
$ifi '%LFB_BEX%' ==  Yes  LOOP(R_RId(IR,RId),
$ifi '%LFB_BEX%' ==  Yes   );
$ifi '%LFB_BEX%' ==  Yes   LOOP(R_RId(IRE,RId),
$ifi '%LFB_BEX%' ==  Yes    Put ",",(   VXELEC_T.l      (T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes         + VXELEC_DPOS_T.l(T,IR,IRE)
$ifi '%LFB_BEX%' ==  Yes         - VXELEC_DNEG_T.l(T,IR,IRE)) :0:2 ;
$ifi '%LFB_BEX%' ==  Yes   );
$ifi '%LFB_BEX%' ==  Yes );		 

   Put /;  );
   );
);
