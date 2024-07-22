* File: print-OUT_VOBJ.inc

* This file will print the production and transmission costs

PUT OUT_VOBJ_OUT;
OUT_VOBJ_OUT.PW = 32767;
OUT_VOBJ_OUT.PS = 1;
OUT_VOBJ_OUT.nJ = 2;


if (INO_SOLVE EQ 1,
   PUT "CaseId,InfoTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     Put CaseId.TL:0,",",INFOTIME.TL:0,",",VOBJ.l:0:4/ ;
);