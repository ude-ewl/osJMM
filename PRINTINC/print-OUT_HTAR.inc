* File: print-OUT_HTAR.inc

* This file will print the value of VXELEC_T_Realised in table format
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.


PUT HTAR_OUT;
HTAR_OUT.PW = 32767;
HTAR_OUT.PS = 1;
HTAR_OUT.nJ = 2;


* -----------------------------------------------------------------------------
* HEAD LINE
                if (INO_SOLVE EQ 1,
                Put "BaseTime";
           LOOP (IR,
                 Put ",",IR.TL:0,"_ex",",",IR.TL:0,"_im",",",IR.TL:0,"_dump",",",IR.TL:0,"_ens";
                                 );
      Put /;
       );

LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
               Put BASETIME.TL:0;
         LOOP (IR,
                  Put ",",SUM(IRE,   VXELEC_T.l  (T,IR,IRE)
                        + VXELEC_DPOS_T.l(T,IR,IRE)
                        - VXELEC_DNEG_T.l(T,IR,IRE)) :0:2 ;
                  Put ",",SUM(IRE,   VXELEC_T.l  (T,IRE,IR)
                        + VXELEC_DPOS_T.l(T,IRE,IR)
                        - VXELEC_DNEG_T.l(T,IRE,IR)) :0:2 ;
                 LOOP(IPLUSMINUS('IPLUS'),
                 Put ",", (VQDAYAHEAD.L(T,IR,'IPLUS') + VWINDSHEDDING_DAY_AHEAD.L(T,IR)
			+ VSOLARSHEDDING_DAY_AHEAD.L(T,IR)
) :0:2;
                        );
                LOOP (IPLUSMINUS('IMINUS'),
                Put ",",VQDAYAHEAD.L(T,IR,'IMINUS'):0:2 ;
                        );
               );
    Put /;  );
   );
);
