QXCAP(T,IEXPORT_Y_Day_ahead(IRE,IRI))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..

VXELEC_T(T,IRE,IRI)
+ ITRANSMISSION_INTRADAY_YES * (VXELEC_DPOS_T(T,IRE,IRI) - VXELEC_DNEG_T(T,IRE,IRI))
+ ITRANSMISSION_NONSP_YES * VXE_NONSPIN_ANCPOS(T,IRE,IRI) 
=L=
IXCAPACITY_Y_Day_ahead(IRE,IRI,T)
;

* Compliance with XCapacity-Day-Ahead (12-loop, only hours that are actually optimized DA)

QXCAPDAYAHEAD(T,IEXPORT_Y_Day_ahead(IRE,IRI))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..
VXELEC_T(T,IRE,IRI)
=L=
IXCAPACITY_Y_Day_ahead(IRE,IRI,T)
;


* The following two equations model that the flow in absolute terms is smaller than the capacity
* Since GAMS does not accept abs(x)<=cap in linear models, two separate equations have to be formulated: x<=cap and x>=-cap   
* Note that the terms on the left hand side of the equation are identical

*----------------------------------------------------------------------------------------------------------------------------------------------------
QXKPTDF5Plus(T,IEXPORT_Y(IRE,IRI)) $(IT_OPT_ALL(T) AND ((IBID_DAYAHEADMARKET_YES EQ 0) OR ( (IBID_DAYAHEADMARKET_YES EQ 1) AND (NOT IT_SPOTPERIOD(T))        )         ))..

Sum(IR, 
    PTDF(IR,IRE,IRI) *
       (SUM(IRRRI,
          VXELEC_T(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI)
          ) 
        -SUM(IRRRE,
          VXELEC_T(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR)
          )
       )
)     
+ VXELEC_T(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)
- IPTDF_Offset(IRE,IRI,T)

+ ITRANSMISSION_INTRADAY_YES * (
 Sum(IR, 
   PTDF(IR,IRE,IRI)*
     ( SUM(IRRRI,
         (VXELEC_DPOS_T(T,IR,IRRRI) - VXELEC_DNEG_T(T,IR,IRRRI))$I_AC_Line_y(IR,IRRRI)
        )

      - SUM(IRRRE,
         (VXELEC_DPOS_T(T,IRRRE,IR) - VXELEC_DNEG_T(T,IRRRE,IR))$I_AC_Line_y(IRRRE,IR)
        ) 
     )
 )
 + (VXELEC_DPOS_T(T,IRE,IRI) - VXELEC_DNEG_T(T,IRE,IRI))$I_DC_Line_Y(IRE,IRI)
) 

+ ITRANSMISSION_NONSP_YES * (
  Sum(IR, 
     PTDF(IR,IRE,IRI) * 
       (Sum(IRRRI, 
           VXE_NONSPIN_ANCPOS(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI)
           )
        -Sum(IRRRE,
           VXE_NONSPIN_ANCPOS(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR)
           )
       )
     )
  + VXE_NONSPIN_ANCPOS(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)
)

=L=

IXCAPACITY_Y(IRE,IRI,T) + PTDFCAP_SLACK(IRE,IRI,T)
;



QXKPTDF5Minus(T,IEXPORT_Y(IRE,IRI))  $(IT_OPT_ALL(T) AND ((IBID_DAYAHEADMARKET_YES EQ 0) OR ( (IBID_DAYAHEADMARKET_YES EQ 1) AND (NOT IT_SPOTPERIOD(T))        )         ))..


Sum(IR, PTDF(IR,IRE,IRI) * (SUM(IRRRI, VXELEC_T(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI)) 
                           -SUM(IRRRE, VXELEC_T(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR)))
)     
+ VXELEC_T(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)
- IPTDF_Offset(IRE,IRI,T)

+ ITRANSMISSION_INTRADAY_YES * (Sum(IR, PTDF(IR,IRE,IRI)* 
      (SUM(IRRRI, (VXELEC_DPOS_T(T,IR,IRRRI) - VXELEC_DNEG_T(T,IR,IRRRI))$I_AC_Line_y(IR,IRRRI))
      - SUM(IRRRE, (VXELEC_DPOS_T(T,IRRRE,IR) - VXELEC_DNEG_T(T,IRRRE,IR))$I_AC_Line_y(IRRRE,IR)) 
     )
 )
 + (VXELEC_DPOS_T(T,IRE,IRI) - VXELEC_DNEG_T(T,IRE,IRI))$I_DC_Line_Y(IRE,IRI)
) 

+ ITRANSMISSION_NONSP_YES * (
  Sum(IR, PTDF(IR,IRE,IRI) * (Sum(IRRRI, VXE_NONSPIN_ANCPOS(T,IR,IRRRI)$I_AC_Line_y(IR,IRRRI))
                             -Sum(IRRRE, VXE_NONSPIN_ANCPOS(T,IRRRE,IR)$I_AC_Line_y(IRRRE,IR))
       )
     )
  + VXE_NONSPIN_ANCPOS(T,IRE,IRI)$I_DC_Line_Y(IRE,IRI)
)

=G=
-IXCAPACITY_Y(IRI,IRE,T) - PTDFCAP_SLACK(IRI,IRE,T)
;
