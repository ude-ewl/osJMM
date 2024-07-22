$ifi '%LFB_BEX%' ==  Yes    IFLBMC_CNEC_RAM_USED(TRL,T)=
$ifi '%LFB_BEX%' ==  Yes         Sum(RRR_FLBMC,IPTDF_FLG(RRR_FLBMC,TRL,T)*
$ifi '%LFB_BEX%' ==  Yes           (
$ifi '%LFB_BEX%' ==  Yes              SUM(IRRRI,VXELEC_T.l(T,RRR_FLBMC,IRRRI)$IEXPORT_FLBMC_Y(RRR_FLBMC,IRRRI)
$ifi '%LFB_BEX%' ==  Yes              + ITRANSMISSION_INTRADAY_YES * (VXELEC_DPOS_T.l(T,RRR_FLBMC,IRRRI)$IEXPORT_FLBMC_Y(RRR_FLBMC,IRRRI) -VXELEC_DNEG_T.l(T,RRR_FLBMC,IRRRI)$IEXPORT_FLBMC_Y(RRR_FLBMC,IRRRI))
$ifi '%LFB_BEX%' ==  Yes              + ITRANSMISSION_NONSP_YES * VXE_NONSPIN_ANCPOS.l(T,RRR_FLBMC,IRRRI)$IEXPORT_FLBMC_Y(RRR_FLBMC,IRRRI)
$ifi '%LFB_BEX%' ==  Yes                 )
$ifi '%LFB_BEX%' ==  Yes              - SUM(IRRRE,VXELEC_T.l(T,IRRRE,RRR_FLBMC)$IEXPORT_FLBMC_Y(IRRRE,RRR_FLBMC)
$ifi '%LFB_BEX%' ==  Yes              + ITRANSMISSION_INTRADAY_YES * (VXELEC_DPOS_T.l(T,IRRRE,RRR_FLBMC)$IEXPORT_FLBMC_Y(IRRRE,RRR_FLBMC) - VXELEC_DNEG_T.l(T,IRRRE,RRR_FLBMC)$IEXPORT_FLBMC_Y(IRRRE,RRR_FLBMC))
$ifi '%LFB_BEX%' ==  Yes              + ITRANSMISSION_NONSP_YES * VXE_NONSPIN_ANCPOS.l(T,IRRRE,RRR_FLBMC)$IEXPORT_FLBMC_Y(IRRRE,RRR_FLBMC)
$ifi '%LFB_BEX%' ==  Yes                   )
$ifi '%LFB_BEX%' ==  Yes            )
$ifi '%LFB_BEX%' ==  Yes         );

$ifi '%LFB_NE%' ==  Yes    IFLBMC_CNEC_RAM_USED(TRL,T)=
$ifi '%LFB_NE%' ==  Yes         Sum(RRR_FLBMC,IPTDF_FLG(RRR_FLBMC,TRL,T)*
$ifi '%LFB_NE%' ==  Yes           (
$ifi '%LFB_NE%' ==  Yes              VXELEC_EXPORT_T.l(T,RRR_FLBMC)   
$ifi '%LFB_NE%' ==  Yes            + ITRANSMISSION_INTRADAY_YES * (VXELEC_EXPORT_DPOS_T.l(T,RRR_FLBMC) - VXELEC_EXPORT_DPOS_T.l(T,RRR_FLBMC))
*$ifi '%LFB_NE%' ==  Yes            +  SUM(IRRRI$RRR_FLBMC,
*$ifi '%LFB_NE%' ==  Yes                 + ITRANSMISSION_NONSP_YES * (VXE_NONSPIN_ANCPOS.l(T,RRR_FLBMC,IRRRI) - VXE_NONSPIN_ANCNEG.l(T,RRR_FLBMC,IRRRI))
*$ifi '%LFB_NE%' ==  Yes                   )
*$ifi '%LFB_NE%' ==  Yes            -  SUM(IRRRE$RRR_FLBMC,
*$ifi '%LFB_NE%' ==  Yes                 + ITRANSMISSION_NONSP_YES * (VXE_NONSPIN_ANCPOS.l(T,IRRRE,RRR_FLBMC) - VXE_NONSPIN_ANCNEG.l(T,IRRRE,RRR_FLBMC))
*$ifi '%LFB_NE%' ==  Yes                   )
$ifi '%LFB_NE%' ==  Yes            )
$ifi '%LFB_NE%' ==  Yes         );


$ifi '%LFB%' ==  Yes    PUT CNEC_RAM_USED_OUT;
$ifi '%LFB%' ==  Yes    CNEC_RAM_USED_OUT.PW = 1255;
$ifi '%LFB%' ==  Yes    CNEC_RAM_USED_OUT.PS = 1;
$ifi '%LFB%' ==  Yes    CNEC_RAM_USED_OUT.nJ = 2;

$ifi '%LFB%' ==  Yes    if (INO_SOLVE EQ 1,
$ifi '%LFB%' ==  Yes       PUT "CaseId,BaseTime,TRL,Value" /;
$ifi '%LFB%' ==  Yes    );

* LOOP WITHIN BASENODE
$ifi '%LFB%' ==  Yes    LOOP (CaseId,
$ifi '%LFB%' ==  Yes   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
$ifi '%LFB%' ==  Yes                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
$ifi '%LFB%' ==  Yes      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
$ifi '%LFB%' ==  Yes                    LOOP (TRL,
$ifi '%LFB%' ==  Yes              if ((IFLBMC_CNEC_RAM_USED(TRL,T)>0+Epsilon),
$ifi '%LFB%' ==  Yes                            Put CaseId.TL:0,",",BASETIME.TL:0,",",TRL.TL:0,",",
$ifi '%LFB%' ==  Yes                            IFLBMC_CNEC_RAM_USED(TRL,T) :0:4/ ;
$ifi '%LFB%' ==  Yes                     );
$ifi '%LFB%' ==  Yes              if ((IFLBMC_CNEC_RAM_USED(TRL,T)<0-Epsilon),
$ifi '%LFB%' ==  Yes                            Put CaseId.TL:0,",",BASETIME.TL:0,",",TRL.TL:0,",",
$ifi '%LFB%' ==  Yes                            IFLBMC_CNEC_RAM_USED(TRL,T) :0:4/ ;
$ifi '%LFB%' ==  Yes                     );
$ifi '%LFB%' ==  Yes           );
$ifi '%LFB%' ==  Yes  );
$ifi '%LFB%' ==  Yes  );
$ifi '%LFB%' ==  Yes  );

