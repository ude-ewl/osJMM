* * File: print-OUT_QEEQINT_M.inc
* File: print-OUT_CONSUMERRENT_DA.inc

* This file will print the marginal value of equation QEEQINT, which is
* interpreted as the electricity price on the intraday market. QEEQINT is then
* multiplied with the DEMAND within each Region. Therfore it delivers the Consumerrent
* QEEQINT_M depends on the sets RRR, Node and T, Demand depends on the sets RRR and T


 PUT OUT_CONSUMERRENT_DA;
 OUT_CONSUMERRENT_DA.PW = 32767;
 OUT_CONSUMERRENT_DA.PS = 1;
 OUT_CONSUMERRENT_DA.nJ = 2;



if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
   LOOP (CaseId,
$ontext
      LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME('STAGE2') )
                      AND (ORD(BASETIME) <= IBASEFINALTIME('STAGE3')) ),
         LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME('STAGE1')+1))  ),
$offtext
 LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME + NO_HOURS_OVERLAP)
                      AND (ORD(BASETIME) <= IBASEFINALTIME) ),
         LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
           LOOP(IR,
               LOOP (R_RId(IR, RId),
                  Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                  ((QEEQDAY.M(T,IR)*(-1)*
                                              (
                                                (IDEMANDELEC_BID_IR(IR,T)/(1-DISLOSS_E(IR)))
                                                + IX3COUNTRY_T_Y(IR,T)
                                              )
                                              )+(
                      sum(IA$(RRRAAA(IR,IA)), IDEMANDHEAT(IA,T) * (-QHEQ.M(T,IA))

                          )
                        )
                    ):0:4/;
               );
            );
         );
     );
   );
);

