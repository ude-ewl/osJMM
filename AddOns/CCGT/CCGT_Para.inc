LOOP(CCGT_Group,

   CCGT_Capacity_sum_full(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G)) , IGELECCAPACITY_Y(IA,G));
   CCGT_Capacity_GT_full(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)) , IGELECCAPACITY_Y(IA,G));
   CCGT_Capacity_ST_full(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)) , IGELECCAPACITY_Y(IA,G));
   CCGT_Capacity_sum_min(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G)) , IGELECCAPACITY_Y(IA,G)*GDATA(IA, G, 'GDMINLOADFACTOR'));
   CCGT_Capacity_GT_min(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)) , IGELECCAPACITY_Y(IA,G)*GDATA(IA, G, 'GDMINLOADFACTOR'));
   CCGT_Capacity_ST_min(CCGT_Group)= Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)) , IGELECCAPACITY_Y(IA,G)*GDATA(IA, G, 'GDMINLOADFACTOR'));

   if (HELPVAR_CCGT_Adj eq 1,
		  if((smin(IAGK_Y(IA,G)$(CCGT_Group_G(CCGT_Group, G)),(GDATA(IA, G, 'GDFULLLOAD'))) lt 0.5),
       display 'WARNING: CCGT-Parameter-Adjustment seems not to be necessary. Please check Input!'
         );
	     if ((CCGT_Capacity_sum_full(CCGT_Group) ne (CCGT_Capacity_GT_full(CCGT_Group)+CCGT_Capacity_ST_full(CCGT_Group))) AND
	        ((CCGT_Capacity_GT_full(CCGT_Group)+CCGT_Capacity_ST_full(CCGT_Group)) ne 0),
	           display 'WARNING: CCGT_Group-power plants are only partly assigned to GT or ST'
	        );

    CCGT_eta_fulload(CCGT_Group)= smax(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND (CCGT_ST(G) OR CCGT_ST(G))),GDATA(IA, G, 'GDFULLLOAD'));
    CCGT_eta_partload(CCGT_Group)= smax(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND (CCGT_ST(G) OR CCGT_ST(G))),GDATA(IA, G, 'GDPARTLOAD'));



    GDATA(IA, G, 'GDFULLLOAD')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)AND IAGK_Y(IA,G))=
        (CCGT_eta_fulload(CCGT_Group) * CCGT_Capacity_GT_full(CCGT_Group))
        /CCGT_Capacity_sum_full(CCGT_Group);

    CCGT_GT_GDFULLLOAD_adj(CCGT_Group)=smax(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)),GDATA(IA, G, 'GDFULLLOAD'));

    GDATA(IA, G, 'GDFULLLOAD')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)AND IAGK_Y(IA,G))=
        (CCGT_eta_fulload(CCGT_Group) - CCGT_GT_GDFULLLOAD_adj(CCGT_Group))
        /((1-CCGT_GT_GDFULLLOAD_adj(CCGT_Group))*CCGT_etaSteamProd);

    if ((CCGT_Capacity_sum_min(CCGT_Group) eq 0) or (CCGT_Capacity_GT_min(CCGT_Group) eq 0),

        GDATA(IA, G, 'GDPARTLOAD')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)AND IAGK_Y(IA,G))=
            (CCGT_eta_partload(CCGT_Group)*CCGT_Capacity_GT_full(CCGT_Group))
            /CCGT_Capacity_sum_full(CCGT_Group);
    ELSE
        GDATA(IA, G, 'GDPARTLOAD')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)AND IAGK_Y(IA,G))=
            (CCGT_eta_partload(CCGT_Group)*CCGT_Capacity_GT_min(CCGT_Group))
            /CCGT_Capacity_sum_min(CCGT_Group);
        );
*end change
    CCGT_GT_GDPARTLOAD_adj(CCGT_Group)=smax(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G)AND CCGT_GT(G)),GDATA(IA, G, 'GDPARTLOAD'));

    GDATA(IA, G, 'GDPARTLOAD')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)AND IAGK_Y(IA,G))=
        (CCGT_eta_partload(CCGT_Group)-CCGT_GT_GDPARTLOAD_adj(CCGT_Group))
        /((1-CCGT_GT_GDPARTLOAD_adj(CCGT_Group))*CCGT_etaSteamProd);

    GDATA(IA, G, 'GDFE_SLOPE')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)AND IAGK_Y(IA,G))=
        (IGELECCAPACITY_Y (IA, G)*(GDATA(IA, G, 'GDPARTLOAD')-GDATA(IA, G, 'GDMINLOADFACTOR')*GDATA(IA, G, 'GDFULLLOAD')))
        /(IGELECCAPACITY_Y (IA, G)*(1-GDATA(IA, G, 'GDMINLOADFACTOR'))*GDATA(IA, G, 'GDPARTLOAD')*GDATA(IA, G, 'GDFULLLOAD'));

    GDATA(IA, G, 'GDFE_SECTION')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)AND IAGK_Y(IA,G))=
        (IGELECCAPACITY_Y (IA, G)*GDATA(IA, G, 'GDMINLOADFACTOR')*(GDATA(IA, G, 'GDFULLLOAD')-GDATA(IA, G, 'GDPARTLOAD')))
        /(IGELECCAPACITY_Y (IA, G)*(1-GDATA(IA, G, 'GDMINLOADFACTOR'))*GDATA(IA, G, 'GDPARTLOAD')*GDATA(IA, G, 'GDFULLLOAD'));

    GDATA(IA, G, 'GDFE_SECTION')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)AND IAGK_Y(IA,G))=        0;
    GDATA(IA, G, 'GDFE_SLOPE')$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G)AND IAGK_Y(IA,G))=        0;


    CCGT_STEAM_full(CCGT_Group) =
       (CCGT_Capacity_sum_full(CCGT_Group) / CCGT_eta_fulload(CCGT_Group))
       *(1-CCGT_GT_GDFULLLOAD_adj(CCGT_Group)) * CCGT_etaSteamProd;

    CCGT_STEAM_min(CCGT_Group) =
         (CCGT_Capacity_sum_min(CCGT_Group) / CCGT_eta_partload(CCGT_Group))
         *(1-CCGT_GT_GDPARTLOAD_adj(CCGT_Group)) * CCGT_etaSteamProd;

  );

  if (HELPVAR_CCGT_Adj ne 1,
     CCGT_STEAM_full(CCGT_Group)=Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)),
        (IGELECCAPACITY_Y(IA,G)/GDATA(IA, G, 'GDFULLLOAD'))*(1-GDATA(IA, G, 'GDFULLLOAD'))*CCGT_etaSteamProd);

     CCGT_STEAM_min(CCGT_Group)=Sum(IAGK_Y(IA, G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_GT(G)),
         ((IGELECCAPACITY_Y(IA,G)*GDATA(IA, G, 'GDMINLOADFACTOR')/GDATA(IA, G, 'GDPARTLOAD'))*(1-GDATA(IA, G, 'GDPARTLOAD'))*CCGT_etaSteamProd));
   );

    if (((CCGT_Capacity_sum_min(CCGT_Group) eq 0) or (CCGT_Capacity_ST_min(CCGT_Group) eq 0)),

        CCGT_slope_STEAM(IA,G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G) AND IAGK_Y(IA,G))=
          ((CCGT_STEAM_full(CCGT_Group)*(IGELECCAPACITY_Y (IA, G)/CCGT_Capacity_ST_full(CCGT_Group)))
          -(CCGT_STEAM_min(CCGT_Group)*0))
          /(IGELECCAPACITY_Y (IA, G)*(1-GDATA(IA, G, 'GDMINLOADFACTOR')));
    ELSE
        CCGT_slope_STEAM(IA,G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G) AND IAGK_Y(IA,G))=
          ((CCGT_STEAM_full(CCGT_Group)*(IGELECCAPACITY_Y (IA, G)/CCGT_Capacity_ST_full(CCGT_Group)))
          -(CCGT_STEAM_min(CCGT_Group)*((IGELECCAPACITY_Y (IA, G)*GDATA(IA, G, 'GDMINLOADFACTOR'))/CCGT_Capacity_ST_min(CCGT_Group))))
          /(IGELECCAPACITY_Y (IA, G)*(1-GDATA(IA, G, 'GDMINLOADFACTOR')));
        );

  CCGT_section_STEAM(IA,G)$(CCGT_Group_G(CCGT_Group, G) AND CCGT_ST(G) AND IAGK_Y(IA,G))=
      CCGT_STEAM_full(CCGT_Group)*(IGELECCAPACITY_Y (IA, G)/CCGT_Capacity_ST_full(CCGT_Group))
      -CCGT_slope_STEAM(IA,G)*IGELECCAPACITY_Y (IA, G);


);
