* File: print-OUT_RDMUSTRUN.inc

PUT RDMUSTRUN_OUT;
RDMUSTRUN_OUT.PW = 32767;
RDMUSTRUN_OUT.PS = 1;
RDMUSTRUN_OUT.nJ = 2;

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
  LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                                      PUT / BASETIME.TL:0,",";
         LOOP(IAGK_Y(IA,IGDISPATCH(IGCHPBOUNDE)),
                                        If((IMINCHPFACTOR(IA,IGDISPATCH,T)*IGELECCAPEFF(IA,IGDISPATCH,T)) > 0,
                                                PUT "1",",";
                                        Elseif  ( (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)),2) > 0) OR (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)),2) >0)		
										      OR  (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)),2) > 0) OR (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)),2) >0)),
                                                PUT "1",",";
                                        else
                                                PUT "0",",";
                                        );
                );
        LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (NOT(IGCHPBOUNDE(IGDISPATCH) OR IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH)))),
                                        If((     (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)),2) > 0) OR (ROUND(sum(IGSPINNING(IGDISPATCH), VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)),2) > 0)		
											OR   (ROUND(sum(IGSPINNING(IGDISPATCH),VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)),2) > 0) OR (ROUND(sum(IGSPINNING(IGDISPATCH), VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)),2) > 0)),
                                                PUT "1",",";
                                        Else
                                                PUT "0",",";
                                        );
                );
				
        LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH))),
										/* pump mode */
										if (VGELEC_T.l(T,IA,IGDISPATCH) - VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH) 	<= 0, 
											If(IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDISPATCH,T)
													              - sum(IGSPINNING(IGDISPATCH),
																	    ROUND(VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCPOS_PRI.l(T,IA,IGSPINNING),2)
																	   +ROUND(VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCPOS_SEC.l(T,IA,IGSPINNING),2)
																	   +VGE_SPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_NONSPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_CONSUMED_NONSP_ANCPOS.l(T,IA,IGSPINNING))											
																		<0,
												PUT "1",",";
											Else
												PUT "0",",";
											);
										/* turbine mode */
										Else
											if ( -(1-IG_UNP_OUTAGE(IA,IGDISPATCH,T))* IGKDERATE(IA,IGDISPATCH,T)*(IGResLOADCAPACITY_Y(IA,IGDISPATCH)$IGHYDRORES(IGDISPATCH) + IGSTOLOADCAPACITY_Y(IA,IGDISPATCH)$IGELECSTORAGE(IGDISPATCH)) 
												   +sum(IGSPINNING(IGDISPATCH),
															 ROUND(VGE_ANCNEG_PRI.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCNEG_PRI.l(T,IA,IGSPINNING),2)
															+ROUND(VGE_ANCNEG_SEC.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCNEG_SEC.l(T,IA,IGSPINNING),2) 
														) > 0,
												PUT "1",",";
										Else
												PUT "0",",";
											);
										  );
                );
        );
);


