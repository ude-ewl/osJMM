* QXK1A and QXK2A have been designed for NTC based market coupling.
* For the Regions where Flow-Based MC is applicable, the equations are no longer used and they are replaced by QXK8PTDFPlus/Minus or QXKPTDF10Plus/Minus respectively.
* For the group NTC equations the same should apply.


* Copy of LFB_No_Load_Flow
*-------------------------------------------------------------------------------
* Electricity transmission is limited by transmission capacity:
* Electricity export fixed on day-ahead market + increased electricity export
* on intraday market - decreased electricity export on intraday market
* + electricity export reserved for secondary reserve <= Transmission capacity

* New name: QXCAPNTC before: QXCAP

QXCAPNTC(T,IEXPORT_NTC_Y(IRE,IRI))$IT_OPT(T)..
VXELEC_T(T,IRE,IRI)
+ ITRANSMISSION_INTRADAY_YES * (VXELEC_DPOS_T(T,IRE,IRI) - VXELEC_DNEG_T(T,IRE,IRI))
+ ITRANSMISSION_NONSP_YES * VXE_NONSPIN_ANCPOS(T,IRE,IRI)
=L=
IXCAPACITY_Y(IRE,IRI,T)
;

* QXCAPDAYAHEAD removed since redundant
* New constraints which model DC-lines in Flow Based MC regions
* Renamed QXK1B into QXCAPDC

QXCAPDC(T,IEXPORT_FLBMC_DC_Y(IRE,IRI))$IT_OPT(T)..
VXELEC_DC_T(T,IRE,IRI) + VXELEC_DC_DPOS_T(T,IRE,IRI) - VXELEC_DC_DNEG_T(T,IRE,IRI)
=L=
IXCAPACITY_NO_FLG(IRE,IRI,T) 
;


QXCAPFBMCPlus(T,ISFD_Y_FLG(FLG))$IT_OPT(T)..

SUM(RRR_FLBMC,
   IPTDF_FLG(RRR_FLBMC,FLG,T) * (VXELEC_EXPORT_T(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T(T,RRR_FLBMC))
   )
=L=

IXCAPACITY_FLG_SFD(FLG,T) + PTDF_SLACK_FLG(FLG,T,'IPLUS')
;

QXCAPFBMCMinus(T,INSFD_Y_FLG(FLG))$IT_OPT(T)..

Sum(RRR_FLBMC,
    IPTDF_FLG(RRR_FLBMC,FLG,T)* (VXELEC_EXPORT_T(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T(T,RRR_FLBMC))
    )
=G=

IXCAPACITY_FLG_NSFD(FLG,T) + PTDF_SLACK_FLG(FLG,T,'IMINUS')
;

QXEXP(T)$IT_OPT(T)..
Sum(RRR_FLBMC,
    VXELEC_EXPORT_T(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T(T,RRR_FLBMC)
    )
=E=

0;

QXDCEFB(T,DCEFB)$IT_OPT(T)..
Sum(RRR_FLBMC$DCEFB_R(DCEFB,RRR_FLBMC),
    VXELEC_EXPORT_T(T,RRR_FLBMC) + VXELEC_EXPORT_DPOS_T(T,RRR_FLBMC) - VXELEC_EXPORT_DNEG_T(T,RRR_FLBMC)
    )
=E=
0;

