* File: print-OUT_RDMIN.inc

PUT RDMIN_OUT;
RDMIN_OUT.PW = 32767;
RDMIN_OUT.PS = 1;
RDMIN_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
        PUT "BaseTime";
		
         LOOP(IAGK_Y(IA,IGDISPATCH(IGCHPBOUNDE)),
         		LOOP(G_GId(IGDISPATCH,GId),
				 			PUT ",",GId.TL:0 ;      
			  	);   
			 	);

        LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (NOT(IGCHPBOUNDE(IGDISPATCH) OR IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH)))),		
 	       	LOOP(G_GId(IGDISPATCH,GId),
						PUT ",",GId.TL:0 ;   
			 		);  
				); 
		
        LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH))),
        	LOOP(G_GId(IGDISPATCH,GId),
						PUT ",",GId.TL:0 ;
					);
		     ); 			
		
    );

* -----------------------------------------------------------------------------

LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                 AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
   LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
         PUT / BASETIME.TL:0,",";
                LOOP(IAGK_Y(IA,IGDISPATCH(IGCHPBOUNDE)),
											If((IMINCHPFACTOR(IA,IGCHPBOUNDE,T) = 0 AND IMAXCHPFACTOR(IA,IGCHPBOUNDE,T) = 0),
													PUT (IMINCHPFACTOR(IA,IGCHPBOUNDE,T)):0,",";
											else
													PUT (MAX(IMINCHPFACTOR(IA,IGCHPBOUNDE,T),GData(IA,IGCHPBOUNDE,'GDMINLOADFACTOR'))*IGELECCAPEFF(IA,IGCHPBOUNDE,T)
															+ sum(IGSPINNING(IGDISPATCH), VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)+VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)+VGE_SPIN_ANCNEG.l(T,IA,IGSPINNING))):0:2,",";
												);
                      );

                LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (NOT(IGCHPBOUNDE(IGDISPATCH) OR IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH)))),
                                           PUT (GData(IA,IGDISPATCH,'GDMINLOADFACTOR')*IGELECCAPEFF(IA,IGDISPATCH,T)
                                                            + sum(IGSPINNING(IGDISPATCH),VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)+VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)+VGE_SPIN_ANCNEG.l(T,IA,IGSPINNING))  ):0:2,",";
                       );
								
                LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH))),
											/* pump mode */
											if (VGELEC_T.l(T,IA,IGDISPATCH) - VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH)  <= 0,
													PUT (VGELEC_T.l(T,IA,IGDISPATCH)-VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH)):0:2,",";
											/* turbine mode */
											else
													PUT ( MAX(0, -(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))* IGKDERATE(IA,IGDISPATCH,T)*(IGResLOADCAPACITY_Y(IA,IGDISPATCH)$IGHYDRORES(IGDISPATCH) + IGSTOLOADCAPACITY_Y(IA,IGDISPATCH)$IGELECSTORAGE(IGDISPATCH))
																		+ sum(IGSPINNING(IGDISPATCH),
																			 VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCNEG_PRI.l(T,IA,IGSPINNING)
																			+VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCNEG_SEC.l(T,IA,IGSPINNING)
																	        +VGE_SPIN_ANCNEG.l(T,IA,IGSPINNING)+VGE_CONSUMED_NONSP_ANCNEG.l(T,IA,IGSPINNING)       ))):0:2,",";
												);
                      );
                );

     );
