* Electricity transmission is limited by transmission capacity:
* Electricity export fixed on day-ahead market + increased electricity export
* on intraday market - decreased electricity export on intraday market
* + electricity export reserved for secondary reserve <= Transmission capacity

* QXCAP (formerly QXK1A)

QXCAP(T,IEXPORT_Y(IRE,IRI))$IT_OPT(T)..
  VXELEC_T(T,IRE,IRI)
  + ITRANSMISSION_INTRADAY_YES * ( VXELEC_DPOS_T(T,IRE,IRI) - VXELEC_DNEG_T(T,IRE,IRI))
  + ITRANSMISSION_NONSP_YES * VXE_NONSPIN_ANCPOS(T,IRE,IRI)
  =L=
  IXCAPACITY_Y(IRE,IRI,T)
;

* Restriction to ensure that transmission on day-ahead market is not larger
* than transmission limit

* Day-ahead, transmission on all connections and for all time steps
* This equation appears redundant since it is also captured by QXCAP

* QXCAPDAYAHEAD (formerly QXK2A)
QXCAPDAYAHEAD (T,IEXPORT_Y(IRE,IRI))$((IBID_DAYAHEADMARKET_YES EQ 1) AND IT_SPOTPERIOD(T))..
  VXELEC_T(T,IRE,IRI)
  =L=
  IXCAPACITY_Y(IRE,IRI,T)
;
