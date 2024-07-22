* Modified last time: 2015.01.20

* This file will print the value of VXELEC_T_Realised in table format 
* VXELEC_T_Realised depends on the sets RRR, RRR, and T.

PUT OUT_VXELEC_ExportFB_T_Realised_ct_OUT;
OUT_VXELEC_ExportFB_T_Realised_ct_OUT.PW = 32767;
OUT_VXELEC_ExportFB_T_Realised_ct_OUT.PS = 1;
OUT_VXELEC_ExportFB_T_Realised_ct_OUT.nJ = 2;

* -----------------------------------------------------------------------------
* HEAD LINE
		if (INO_SOLVE EQ 1,
                Put "BaseTime";
           LOOP (RRR_FLBMC(IR),
                 Put ",",IR.TL:0;
                 );
      Put /;
       );  

LOOP (CaseId,
   LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME)
                       AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
      LOOP (T  $((ORD(T)-1)=(ORD(BASETIME)-IBASESTARTTIME+1)),
               Put BASETIME.TL:0;
     	   LOOP (RRR_FLBMC(IR),
		       Put ",",(VXELEC_EXPORT_T.L(T,IR)+VXELEC_EXPORT_DPOS_T.L(T,IR)-VXELEC_EXPORT_DNEG_T.L(T,IR)) :0:2 ;
              ) ;
    Put /;  );
  )		   
);   
