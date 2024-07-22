* File: print-OUT_VGE_ANCNEG_TER_ct.inc

PUT VGE_ANCNEG_TER_OUT_ct;
VGE_ANCNEG_TER_OUT_ct.PW = 32767;
VGE_ANCNEG_TER_OUT_ct.PS = 1;
VGE_ANCNEG_TER_OUT_ct.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT Put "BaseTime";
	LOOP (IAGK_Y(IA,IGSPINNING),
		LOOP(G(IGSPINNING),
			Put ",",G.TL:0;
			);
		);
);

* -----------------------------------------------------------------------------
* Variables depending on AAA, GGG and Day

LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                    AND (ORD(BASETIME) <=IBASEFINALTIMEWRITEOUT ) ),
    LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                     Put / BASETIME.TL:0,
	LOOP (IAGK_Y(IA,IGSPINNING),
               LOOP(G_GId(IGSPINNING,GId),
                     Put ",",(VGE_SPIN_ANCNEG.l(T,IA,IGSPINNING)+VGE_CONSUMED_NONSP_ANCNEG.l(T,IA,IGSPINNING)):0:2;
               );
      	);
    );
);
