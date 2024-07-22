* File: print-OUT_IGELECCAPEFF_ct.inc

PUT OUT_IGELECCAPEFF_OUT_ct;
OUT_IGELECCAPEFF_OUT_ct.PW = 32767;
OUT_IGELECCAPEFF_OUT_ct.PS = 1;
OUT_IGELECCAPEFF_OUT_ct.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "BaseTime";
   LOOP (G(IGDISPATCH), PUT ",",G.TL:0);
);
* LOOP (G_GId(IGELEC,GId), PUT GId.TL:0, ",");

* -----------------------------------------------------------------------------
* The output represents the share of unavailable capacity to total capacity


LOOP(TWRITEOUT(BASETIME,T),
                                        PUT / BASETIME.TL:0;
                LOOP ((IGDISPATCH,IA)$(IGELECCAPACITY_Y(IA,IGDISPATCH) gt 0),
                 LOOP (G_GID(IGDISPATCH,GId),
                                 PUT ",",(1-IGELECCAPEFF(IA,IGDISPATCH,T)*IMaxGen(IA,IGDispatch,T)/IGELECCAPACITY_Y(IA,IGDISPATCH)):0:2;

                                );
                        );
        );
