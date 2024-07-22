* File: print-OUT_VQBOUNDEMIN.inc



PUT OUT_VQBOUNDEMIN_OUT;
OUT_VQBOUNDEMIN_OUT.PW = 32767;
OUT_VQBOUNDEMIN_OUT.PS = 1;
OUT_VQBOUNDEMIN_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Unit_ID,IPLUSMINUS,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
  LOOP (IAGK_Y(IA,IGDISPATCH),
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (A_AId(IA, AId),
				LOOP(G_GId(IGDISPATCH,GId),
					LOOP(IPLUSMINUS,
						if(VQBOUNDEMIN.L(T,IA,IGDISPATCH,IPLUSMINUS)>0,
							Put CaseId.TL:0,",",AId.TL:0,",",BASETIME.TL:0,",", GId.TL:0,",",
							IPLUSMINUS.TL:0,",",VQBOUNDEMIN.L(T,IA,IGDISPATCH,IPLUSMINUS):0:4/ ;
						);
					);
				);
			);
		);
	);
);
);