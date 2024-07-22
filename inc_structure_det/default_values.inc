* File default_values.inc
*------------------------------------------------------------------------------

Loop ((AAA,G)$(GSTARTVALUEDATA(AAA,G,'GSSDTIME') eq 0),
  GSTARTVALUEDATA(AAA,G,'GSSDTIME')=30;
);

LOOP ((AAA,G) $((GSTARTVALUEDATA(AAA,G,'GSCONTENTSTORAGE') eq 0) AND (NOT IGELECSTODSM(G))),
  GSTARTVALUEDATA(AAA,G,'GSCONTENTSTORAGE')=0.5;
);

LOOP ((AAA,G) $( IGPTG(G)),
  GSTARTVALUEDATA(AAA,G,'GSCONTENTSTORAGE')=0;
);


LOOP ((AAA,G) $(IGESTORAGE_DSM(G)),
 IF((GDATA(AAA,G,'GDLOADEFF') eq 0),
  GDATA(AAA,G,'GDLOADEFF') = 1;
 );
);

LOOP ((AAA,G) $(IGESTORAGE_DSM(G)),
 IF((GDATA(AAA,G,'GDUNLOADEFF') eq 0),
  GDATA(AAA,G,'GDUNLOADEFF') = 1;
 );
);

LOOP ((AAA,G) $((IGSTORAGE(G) or IGHYDRORES(G))),
 IF (GDATA(AAA,G,'GDSTOLOSS') eq 0,
  GDATA(AAA,G,'GDSTOLOSS') = 0;
 );
);

*#UeNB Version  20.08.2019
*in UeNB-Version default value for GDCV values is activated
* set default value gdcv for all extection condensing units
*LOOP ((AAA,G)$( GDATA(AAA,G,'GDTYPE') eq  3),
*  IF ( GDATA(AAA,G,'GDCV') eq 0,
*      GDATA(AAA,G,'GDCV')=0.7
*  );
*);

* set default value GDCB  for all extection condensing units and backpressure units
LOOP ((AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  2) OR (GDATA(AAA,G,'GDTYPE') eq  3)),
  IF (GDATA(AAA,G,'GDCB') eq 0,
      GDATA(AAA,G,'GDCB')=0.5
  );
);

* set default value for rampup
LOOP ( (AAA,G) $( (GDATA(AAA,G,'GDTYPE') eq  1)),
  IF (GDATA(AAA,G,'GDSTARTUPRAMP') eq 0,
      GDATA(AAA,G,'GDSTARTUPRAMP') = GDATA(AAA,G,'GDminloadfactor');
  );
);

LOOP ((AAA,G) $(GDATA(AAA,G,'GDTYPE') eq  1),
  IF (GDATA(AAA,G,'GDRAMPUP') eq 0,
      GDATA(AAA,G,'GDRAMPUP')=(1);
  );
);


*set IMinGen(IA,IGDISPATCH,T) for WASTE and BFG units without heat extraction
*reflect production restriction due to obligation of using input fuel (WASTE or BFG)
*BFG fuelID 203 target FLH 46%
*WASTE fuelID 10 target FLH 70%

$ifi %MinGen_Manipulation%==yes LOOP ((AAA,G,Y)$((not GDATA(AAA,G,'GDTYPE') eq  2)and (not GDATA(AAA,G,'GDTYPE') eq  3)and (GDATA(AAA,G,'GDFUEL') eq 203 or GDATA(AAA,G,'GDFUEL') eq 10)),
$ifi %MinGen_Manipulation%==yes        LOOP (WEEKS,
$ifi %MinGen_Manipulation%==yes            BASE_MinGeneration_Var_T(AAA,G,WEEKS)$(GDATA(AAA,G,'GDFUEL') eq 203)=0.9;
$ifi %MinGen_Manipulation%==yes            BASE_MinGeneration_Var_T(AAA,G,WEEKS)$(GDATA(AAA,G,'GDFUEL') eq 10)=0.9;
$ifi %MinGen_Manipulation%==yes        );
$ifi %MinGen_Manipulation%==yes    GKFXELEC(Y,AAA,G)$(GDATA(AAA,G,'GDFUEL') eq 203)=GKFXELEC(Y,AAA,G)*0.51;
$ifi %MinGen_Manipulation%==yes    GKFXELEC(Y,AAA,G)$(GDATA(AAA,G,'GDFUEL') eq 10)=GKFXELEC(Y,AAA,G)*0.75;
$ifi %MinGen_Manipulation%==yes   );




* GDCV:
* GDCV is relevant only for extraction units (GDTYPE 3),
* it should probably be in the range 0 to 0.3.
* The GDCV value is relevant only for extraction units (GDTYPE 3)
* and should not be filled in for other units
* (it is however set to 1 for back pressure units such that the fuel consumption
* for these units can be found using the same formula as for extraction units).

* GDCB
* GDCB is relevant only for backpressure and extraction units (GDTYPE 2 and 3),
* it should probably be in the range 0.1 to 1.
