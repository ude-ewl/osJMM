* File: print-OUT_CaseName.inc

* This file will print the name of the case

PUT OUT_CaseName_OUT;
OUT_CaseName_OUT.PW = 32767;
OUT_CaseName_OUT.PS = 1;
OUT_CaseName_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
*   PUT "CaseName,CaseId,WindCaseId" /;
   PUT "CaseName,CaseId" /;
   LOOP (CaseName,
      PUT CaseName.TL:0,",";
   );
   LOOP (CaseId,
*      PUT CaseId.TL:0,",";
       PUT CaseId.TL:0/;
   );
*   LOOP (WindCaseId,
*      PUT WindCaseId.TL:0/;
*   );
);