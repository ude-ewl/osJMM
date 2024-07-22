*Test to ensure only one option for representation of exchanges/load flows is activated
Parameter testnwr;
testnwr = 0;
$ifi '%No_Load_Flow%' == Yes            testnwr = testnwr + 1;
$ifi '%LFB_Intraday%'  == Yes           testnwr = testnwr + 1;

if (testnwr = 1,
    display "Network representation check passed";
 else
 abort "Only one option for network representation allowed";
);