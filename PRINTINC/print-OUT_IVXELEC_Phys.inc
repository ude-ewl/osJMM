* File: print-OUT_QXKPTDF3Minus_M.inc

* This file will print the physical cross-border flows as computed using PTDFs.
* VXELEC_Phys_T depends on the sets RRRe, RRRi and T


PUT OUT_IVXELEC_Phys_OUT;
OUT_IVXELEC_Phys_OUT.PW = 32767;
OUT_IVXELEC_Phys_OUT.PS = 1;
OUT_IVXELEC_Phys_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region1,ID_Region2,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
 IVXELEC_Phys_T(IRE,IRI,T) $((ORD(T)-1)<=NO_HOURS_OVERLAP) =
 Sum(IR, PTDF(IR,IRE,IRI)*(ITRANSMISSION_INTRADAY_YES*SUM(IRRRI,(VXELEC_DPOS_T.l(T,IR,IRRRI)-VXELEC_DNEG_T.l(T,IR,IRRRI))$I_AC_Line_y(IR,IRRRI))$(NOT((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)))
 - ITRANSMISSION_INTRADAY_YES*SUM(IRRRE,(VXELEC_DPOS_T.l(T,IRRRE,IR)-VXELEC_DNEG_T.l(T,IRRRE,IR))$I_AC_Line_y(IRRRE,IR))$(Not((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T)))))
 + ITRANSMISSION_INTRADAY_YES*(VXELEC_DPOS_T.l(T,IRE,IRI) - VXELEC_DNEG_T.l(T,IRE,IRI))$(I_DC_Line_Y(IRE,IRI) AND (Not((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))))
 + Sum(IR, PTDF(IR,IRE,IRI)*SUM(IRRRI,VXELEC_T.l(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI))- PTDF(IR,IRE,IRI)*SUM(IRRRE,VXELEC_T.l(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR)))
 + VXELEC_T.l(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)

* -----------------------------------------------------------------------------
 LOOP (CaseId,
    LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                        AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT) ),
       LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)) ,
          LOOP (IEXPORT_Y(IR,IRE),
             if ( (IVXELEC_Phys_T(IR,IRE,T)<>0),
                Put CaseId.TL:0,",";
                LOOP(R_RId(IR,RId),
                   Put RId.TL:0,",";
                );
                LOOP(R_RId(IRE,RId),
                   Put RId.TL:0,",",BASETIME.TL:0,",",
                       IVXELEC_Phys_T(IR,IRE,T) :0:4/ ;
                );
             );
          );
       );
    );
 );
