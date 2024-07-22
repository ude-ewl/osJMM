* File: print-OUT_VOBJ_R_T_StartCost.inc

* This file will print the production and transmission costs in the root node
* divided upon hour and region.

PUT OUT_VOBJ_R_T_StartCost_OUT;
OUT_VOBJ_R_T_StartCost_OUT.PW = 32767;
OUT_VOBJ_R_T_StartCost_OUT.PS = 1;
OUT_VOBJ_R_T_StartCost_OUT.nJ = 2;


LOOP ((IR,T)$(ORD(T)>1 AND ORD(T)<= NO_HOURS_OVERLAP +1),
   VOBJ_StartCost_out(IR,t)

 =

* #5 (VGSTARTUP_T) * trkr: Start-up variable costs:
  + SUM(IAGK_Y(IA,IGUC)$RRRAAA(IR,IA),
        GDATA(IA,IGUC,'GDSTARTUPCOST')*
$ifi not '%NoStartUpCosts%' == Yes  IGELECCAPEFF(IA,IGUC,T)*
        VGSTARTUP_T.L(T,IA,IGUC)
    )

* #18a (VGSTARTUPFUELCONS and VGSTARTUP_T, resp.)
* Start-up fuel costs: for power plants without outtime dependant start-up fuel consumption
* Set IGNOUCSCC = IGUC if UnitCscc=No. IGNOUCSCC=IGUC-IGUCSCC if UnitCscc=Yes

  + SUM( (IAGK_Y(IA,IGNOUCSCC),FFF)$((GDATA(IA,IGNOUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
  (IFUELPRICE_Y(IA,FFF,T)  + IFUELTRANS_Y(IA,IGNOUCSCC,T))*
$ifi     '%UnitCmin%' == Yes        VGSTARTUPFUELCONS.L(T,IA,IGNOUCSCC)
$ifi not '%NoStartUpCosts%' == Yes $ifi Not '%UnitCmin%' == Yes        GDATA(IA,IGNOUCSCC,'GDSTARTUPFUELCONSHot') * VGSTARTUP_T.L(T,IA,IGNOUCSCC) * IGELECCAPEFF(IA,IGNOUCSCC,T)
  )

* #18b (VGSTARTUPFUELCONS)
* Start-up fuel costs: for power plants with outtime dependant start-up fuel consumption

$ifi %UnitCscc%==yes   + SUM( (IAGK_Y(IA,IGUCSCC),FFF)$((GDATA(IA,IGUCSCC,'GDSTARTUPFUEL') EQ FDATA(FFF,'FDNB')) AND RRRAAA(IR,IA)),
$ifi %UnitCscc%==yes      (IFUELPRICE_Y(IA,FFF,T)  + IFUELTRANS_Y(IA,IGNOUCSCC,T))*
$ifi %UnitCscc%==yes         VGSTARTUPFUELCONS.L(T,IA,IGUCSCC))


;
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
                  VOBJ_StartCost_out(IR,T):0:4/ ;
            );
         );
      );
   );
);
