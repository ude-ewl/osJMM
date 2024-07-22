* File: print-OUT_EXCHANGE.inc

PUT EXCHANGE_OUT;
EXCHANGE_OUT.PW = 32000;
EXCHANGE_OUT.nJ = 2;


*-------------------------------------------------------------------------------
* Print out Exchange sum per region

* Headline
    if (INO_SOLVE EQ 1,
        Put "BaseTime";
        LOOP (IR,
            Put ",",IR.TL:0;
             );
        );

    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                        AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
                LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
                                        PUT / BASETIME.TL:0;
            LOOP (IR,


* Print Exchange

* Pan-European simulation
            If(IX3country_T_Y(IR,T)=0,
* LOOP WITHIN BASENODE
$ifi '%No_Load_Flow%' ==  Yes                    Put ",",SUM(IRE,VXELEC_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          + VXELEC_DPOS_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          - VXELEC_DNEG_T.l(T,IR,IRE)
$ifi '%No_Load_Flow%' ==  Yes                          - (VXELEC_T.l(T,IRE,IR)
$ifi '%No_Load_Flow%' ==  Yes                          + VXELEC_DPOS_T.l(T,IRE,IR)
$ifi '%No_Load_Flow%' ==  Yes                          - VXELEC_DNEG_T.l(T,IRE,IR))) :0:2;
                );
* Country-specific simulation
            if(IX3country_T_Y(IR,T) NE 0,
                Put ",",IX3COUNTRY_T_Y(IR,T):0:2;
               );
                                );
                        );
                );

