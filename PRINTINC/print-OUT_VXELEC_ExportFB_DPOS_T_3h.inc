* File: print-OUT_VXELEC_DPOS_T_3h.inc

* This file will print the value of VXELEC_DPOS_T for 3 hour loops
* VXELEC_DPOS_T depends on the sets RRR, RRR, and T.

PUT OUT_VXELEC_ExportFB_DPOS_T_3h_OUT;
OUT_VXELEC_ExportFB_DPOS_T_3h_OUT.PW = 1255;
OUT_VXELEC_ExportFB_DPOS_T_3h_OUT.PS = 1;
OUT_VXELEC_ExportFB_DPOS_T_3h_OUT.nJ = 2;

if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region1,Infotime,BaseTime,Value" /;
);


LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIME ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),
           LOOP (RRR_FLBMC(IR),
              if ((VXELEC_EXPORT_DPOS_T.L(T,IR)>EPSILON),
                 Put CaseId.TL:0,",";
                 LOOP(R_RId(IR,RId),
                    Put RId.TL:0,",",INFOTIME.TL:0,",",BASETIME.TL:0,",",VXELEC_EXPORT_DPOS_T.L(T,IR) :0:4/ ;
                 );
              );
           );
        );
    );
);