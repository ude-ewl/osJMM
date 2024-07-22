* File: print-OUT_HEAT.inc

PUT HEAT_OUT;
HEAT_OUT.PW = 32000;
HEAT_OUT.PS = 1;
HEAT_OUT.nJ = 2;


* This Code generates the headline of the output CSV file
if (INO_SOLVE EQ 1,
     Put "BaseTime",
   LOOP (IAGK_Y(IA,IGHEAT),
                LOOP(G(IGHEAT),
                                Put ",", G.TL:0;
                                                );
                                );
   Put /;
);

*-------------------------------------------------------------------------------
* print out variables and parameters depending on Unit Groups and T

* allocate realised wind power production, unregulated hydropower production and
* solar production to variable HSTA, such that they can be used in the
* output database


LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                      Put BASETIME.TL:0,

            LOOP (IAGK_Y(IA,IGHEAT),
                                                Put ",", (VGHEAT_T.L(T,IA,IGHEAT) 
														- VGHEAT_CONSUMED_T.L(T,IA,IGHEAT)$(IGHEATSTORAGE(IGHEAT))
														 ):0:2 ;
                 );

Put /; );
  );
);

*-------------------------------------------------------------------------------
