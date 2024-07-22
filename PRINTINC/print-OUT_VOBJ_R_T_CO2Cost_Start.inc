* File: print-OUT_VOBJ_R_T_CO2Cost_Start_.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_CO2Cost_Start_OUT;
OUT_VOBJ_R_T_CO2Cost_Start_OUT.PW = 32767;
OUT_VOBJ_R_T_CO2Cost_Start_OUT.PS = 1;
OUT_VOBJ_R_T_CO2Cost_Start_OUT.nJ = 2;

LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_CO2Cost_Start_out(IR,t)

 =

* #19a (VGSTARTUP_T bzw. VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (see #9):
$ifi not '%NoStartUpCosts%' == Yes   +  SUM(C,
$ifi not '%NoStartUpCosts%' == Yes         SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$( (GDATA(IA,IGNOUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),

$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes  GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGNOUCSCC)*IGELECCAPEFF(IA,IGNOUCSCC,T)*
$ifi not '%NoStartUpCosts%' == Yes $ifi     '%UnitCmin%' == Yes  VGSTARTUPFUELCONS.L(T,IA,IGNOUCSCC)*
$ifi not '%NoStartUpCosts%' == Yes               (FDATA(FFF,'FDCO2') * IM_POL('TAX_CO2',C,T) + FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T) * (1-GDATA(IA,IGNOUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi not '%NoStartUpCosts%' == Yes         )
$ifi not '%NoStartUpCosts%' == Yes      )


* #19b (VGSTARTUPFUELCONS)
* Emission taxes CO2, SO2 start-up fuel consumption (out-time dependant start-up):
$ifi %UnitCscc%==yes   +  SUM(C,
$ifi %UnitCscc%==yes         SUM( (IAGK_Y(IA,IGUCSCC),FFF)$( (GDATA(IA,IGUCSCC,'GDFUEL') EQ FDATA(FFF,'FDNB')) AND ICA(C,IA) AND RRRAAA(IR,IA)),
$ifi %UnitCscc%==yes             VGSTARTUPFUELCONS.L(T,IA,IGUCSCC)*
$ifi %UnitCscc%==yes               (FDATA(FFF,'FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA(FFF,'FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGUCSCC,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi %UnitCscc%==yes
$ifi %UnitCscc%==yes         )
$ifi %UnitCscc%==yes      )


* #19c (constant)
  +  SUM(C,
          SUM(IAGK_Y(IA,IGDISGEN)$( ICA(C,IA) AND RRRAAA(IR,IA)),
                IDISGEN_VAR_T(IR,T) * (1/GDATA(IA,IGDISGEN,'GDFULLLOAD')) *
$ifi '%Code_version%'==EWL    (FDATA('DISGEN','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('DISGEN','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
$ifi '%Code_version%'==UENB   (FDATA('%Sonstige_NEE%','FDCO2')*IM_POL('TAX_CO2',C,T)+FDATA('%Sonstige_NEE%','FDSO2')*IM_POL('TAX_SO2',C,T)*(1-GDATA(IA,IGDISGEN,'GDDESO2')))*CONVERT_KG_PER_GJ_T_PER_MWH
       )
   )

;
* end loop (IR,t
);




if (INO_SOLVE EQ 1,
   PUT "CaseId,ID_Region,BaseTime,Value" /;
);

* -----------------------------------------------------------------------------
* LOOP WITHIN BASENODE
LOOP (CaseId,
     LOOP(BASETIME  $( ( ORD(BASETIME) >= IBASESTARTTIME )
                         AND (ORD(BASETIME) <= IBASEFINALTIMEWRITEOUT ) ),
        LOOP (T  $( (ORD(T)-1) = (ORD(BASETIME)-IBASESTARTTIME+1) ),

         LOOP(IR,
            LOOP (R_RId(IR,RId),
               Put CaseId.TL:0,",",RId.TL:0,",",BASETIME.TL:0,",",
                  VOBJ_CO2Cost_Start_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
