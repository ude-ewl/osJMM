* File: print-OUT_RDMAX.inc

PUT RDMAX_OUT;
RDMAX_OUT.PW = 32767;
RDMAX_OUT.PS = 1;
RDMAX_OUT.nJ = 2;

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
                                                PUT (IMAXCHPFACTOR(IA,IGCHPBOUNDE,T)*IGELECCAPEFF(IA,IGCHPBOUNDE,T)
                                                        - sum(IGSPINNING(IGDISPATCH), VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)+VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)
																					 +VGE_SPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_NONSPIN_ANCPOS.l(T,IA,IGSPINNING)) ):0:2,",";
                                );
                LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (NOT(IGCHPBOUNDE(IGDISPATCH) OR IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH)))),
                                                PUT (IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDISPATCH,T)
                                                      - sum(IGSPINNING(IGDISPATCH),VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)+VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)  
																				  +VGE_SPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_NONSPIN_ANCPOS.l(T,IA,IGSPINNING)) ):0:2,",";
                       );
					   
                LOOP ((IAGK_Y(IA,IGDISPATCH))$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0 AND (IGELECSTORAGE(IGDISPATCH) OR IGHYDRORES(IGDISPATCH))),
											   /* pump mode */
											   if (VGELEC_T.l(T,IA,IGDISPATCH) - VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH)  <= 0,
													   PUT (Min(0,IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDISPATCH,T)
													              - sum(IGSPINNING(IGDISPATCH),
																	    VGE_ANCPOS_PRI.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCPOS_PRI.l(T,IA,IGSPINNING)
																	   +VGE_ANCPOS_SEC.l(T,IA,IGSPINNING)+VGE_CONSUMED_ANCPOS_SEC.l(T,IA,IGSPINNING)
																	   +VGE_SPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_NONSPIN_ANCPOS.l(T,IA,IGSPINNING)+VGE_CONSUMED_NONSP_ANCPOS.l(T,IA,IGSPINNING)))):0:2,",";
											   /* turbine mode */
											   else
											   PUT (VGELEC_T.l(T,IA,IGDISPATCH) - VGELEC_CONSUMED_T.l(T,IA,IGDISPATCH)):0:2,",";
												   );
                       );

            );
       );
