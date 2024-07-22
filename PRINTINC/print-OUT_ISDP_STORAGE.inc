* File: print-OUT_ISDP_STORAGE.inc

PUT ISDP_STORAGE_OUT;
ISDP_STORAGE_OUT.PW = 32767;
ISDP_STORAGE_OUT.PS = 1;
ISDP_STORAGE_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,UnitGroupId,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* variables depending on AAA, GGG, and Node. Only root node values are printed


LOOP (CaseId,
      LOOP(BASETIME  $(ORD(BASETIME) = IBASEFINALTIMEWRITEOUT),
         LOOP (T $((ORD(T)-1) = INO_HOURS_OPT_PERIOD ),

          LOOP (IAGK_Y(IA,IGSTORAGE),
             LOOP(A_AId(IA,AId),
                LOOP(G_GId(IGSTORAGE,GId),
                     Put CaseId.TL:0,",",GId.TL:0,",",BASETIME.TL:0,",",
                      (ISDP_STORAGE(IA,IGSTORAGE)$(ISDP_STORAGE(IA,IGSTORAGE) NE 0) + ISDP_REFPRICE(IA)$(ISDP_STORAGE(IA,IGSTORAGE) EQ 0))  :0:4/ ;
               );
            );
         );
      );
   );
);
