* File: print-OUT_QXEXP_M.inc

PUT OUT_QXEXP_M_OUT;
OUT_QXEXP_M_OUT.PW = 32767;
OUT_QXEXP_M_OUT.PS = 1;
OUT_QXEXP_M_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* write out marginals in SPOTPERIOD

if (INO_SOLVE EQ 1,
  LOOP (CaseId,
      
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                         AND (ORD(BASETIME) <= IBASEFINALTIME 
$ifi '%Looping%' == week				     - 12
							      ) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               
                  Put CaseId.TL:0,",",BASETIME.TL:0,",",QXEXP.M(T):0:4/;
               
            );
         );
      
   );
);


* LOOP WITHIN BASENODE
if (INO_SOLVE GT 1,
LOOP (CaseId,
   IF ((IBID_DAYAHEADMARKET_YES  EQ 1),
      
         LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME 
$ifi NOT '%Looping%' == week				     + NO_HOURS_OVERLAP 
										)
                         AND (ORD(BASETIME) <= IBASEFINALTIME 
$ifi '%Looping%' == week				     - 12
										) ),
            LOOP (T  $( IT_SPOTPERIOD(T)$((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1))  ),
               
                  Put CaseId.TL:0,",",BASETIME.TL:0,",",QXEXP.M(T):0:4/;
               );
            
         );
     
   );
);
);