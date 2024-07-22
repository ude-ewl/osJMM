
* Update the number of hours that the unit has been either online or offline at the beginning of a new
* planning loop

LOOP((IA,IGUC)$IAGK_Y(IA,IGUC),
*VGONLINE_SUM3(IA,IGUC)= SUM(T$((ORD(T)>1) AND (ORD(T)<5)),VGONLINE_T.l(T,IA,IGUC));
*VGONLINE_SUM2(IA,IGUC)= SUM(T$((ORD(T)>2) AND (ORD(T)<5)),VGONLINE_T.l(T,IA,IGUC));

Loop(T$( ord(T)=(NO_hours_overlap+1) ),
 UCINI(IA,IGUC) = Max(0,UCINI(IA,IGUC));
 If( Vgonline_T.l(T,IA,IGUC) = 1,
  Loop(ITALIAS$( (ord(ITALIAS)<=(no_hours_overlap+1)) and (ord(ITALIAS)>(no_hours_overlap+1-Min(No_Hours_Overlap,GDATA(IA,IGUC,'GDMINOPERATION'))))),
    If(Vgonline_T.l(ITALIAS,IA,IGUC) = 0,
    UCINI(IA,IGUC) = 0;
    Else
    UCINI(IA,IGUC) = UCINI(IA,IGUC) + 1;
    );
  );
);

 If( Vgonline_T.l(T,IA,IGUC) = 0,
  UCINI(IA,IGUC) = Min(0,UCINI(IA,IGUC));
  Loop(ITALIAS$( (ord(ITALIAS)<=(no_hours_overlap+1)) and (ord(ITALIAS)>(no_hours_overlap+1-Min(No_Hours_Overlap,GDATA(IA,IGUC,'GDMINSHUTDOWN'))))),
    If(Vgonline_T.l(ITALIAS,IA,IGUC) = 1,
    UCINI(IA,IGUC) = 0;
    Else
    UCINI(IA,IGUC) = UCINI(IA,IGUC) - 1;
    );
  );
);
);
);
$ontext
VGONLINE_SUM3(IA,IGUC)= SUM(T$((ORD(T)>1) AND (ORD(T)<=NO_HOURS_OVERLAP+1)),VGONLINE_T.l(T,IA,IGUC));
VGONLINE_SUM2(IA,IGUC)= SUM(T$((ORD(T)>2) AND (ORD(T)<=NO_HOURS_OVERLAP+1)),VGONLINE_T.l(T,IA,IGUC));


   If((VGONLINE_SUM3(IA,IGUC)= 0),
      If (UCINI(IA,IGUC)<0,
         UCINI(IA,IGUC) = UCINI(IA,IGUC) -NO_HOURS_OVERLAP;
      else
         UCINI(IA,IGUC) = -NO_HOURS_OVERLAP;
      );
   else
      If((VGONLINE_SUM2(IA,IGUC)= 0),
         UCINI(IA,IGUC) = -2;
      else
         If((VGONLINE_T.l('T03',IA,IGUC)= 0),
            UCINI(IA,IGUC) = -1;
         );
      );
   );

   If((VGONLINE_SUM3(IA,IGUC)= 3),
      If (UCINI(IA,IGUC)>0,
         UCINI(IA,IGUC) = UCINI(IA,IGUC) +3;
      else UCINI(IA,IGUC) = 3;
      );
   else
      If((VGONLINE_SUM2(IA,IGUC)= 2),
         UCINI(IA,IGUC) = 2;
      else
         If((VGONLINE_T.l('T03',IA,IGUC)= 1),
            UCINI(IA,IGUC) = 1;
         );
      );
   );
$offtext

