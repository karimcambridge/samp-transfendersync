/*
	This include syncs TransFenders (mod shops) with all passengers of the
	vehicles and avoids vehicle collision with other vehicles

	@licence: GPLv3
        @credits: Y_Less (foreach for SA-MP)

	(c) Kar 2015
*/

#if !defined inc_transfendersync_
	#define inc_transfendersync_
#else
	#endinput
#endif

#include <a_samp>
#tryinclude <foreach>

GetVehicleMaxPassengers(iModel) // The include isn't about this, so no stock incase people already use it in their gamemodes
{
    if(400 <= iModel <= 611)
    {
        static s_MaxPassengers[] =
        {
            271782163, 288428337, 288559891, -2146225407, 327282960, 271651075, 268443408, 286339857, 319894289, 823136512, 805311233,
            285414161, 286331697, 268513553, 18026752, 286331152, 286261297, 286458129, 856765201, 286331137, 856690995, 269484528,
            51589393, -15658689, 322109713, -15527663, 65343
        };
        new result = ((s_MaxPassengers[(iModel -= 400) >>> 3] >>> ((iModel & 7) << 2)) & 0xF);
        switch(iModel)
        {
        	case 483: result = 2; // Camper (originally 1, which is wrong)
        }
        return result;
    }
    return 0xF;
}

public K_TSync_OnEnterExitModShop(playerid, enterexit, interiorid)
{
    printf("[System: OnEnterExitModShop] - playerid: %d - enterexit: %d - interiorid: %d", playerid, enterexit, interiorid);
    new vehicleid = GetPlayerVehicleID(playerid), maxPassengers, passengerCount;

   	maxPassengers = GetVehicleMaxPassengers(GetVehicleModel(vehicleid)) + 1; // counting driver

	if(enterexit) {
	    SetVehicleVirtualWorld(vehicleid, playerid);
		#if defined _FOREACH_LOCAL_VERSION
		foreach(Player, i)
		#else
		for(new i = 0; i < MAX_PLAYERS; ++i)
		#endif
		{
			if(GetPlayerVehicleID(i) == vehicleid) {
			    SetPlayerVirtualWorld(i, playerid);
			    if(++passengerCount == maxPassengers)
					break;
            }
		}
	}
	else {
        SetVehicleVirtualWorld(vehicleid, 0);
        #if defined _FOREACH_LOCAL_VERSION
		foreach(Player, i)
		#else
		for(new i = 0; i < MAX_PLAYERS; ++i)
		#endif
		{
			if(GetPlayerVehicleID(i) == vehicleid) {
			    SetPlayerVirtualWorld(i, 0);
				if(++passengerCount == maxPassengers)
					break;
			}
		}
    }
    return 1;
}

#if defined _ALS_OnEnterExitModShop
	#undef OnEnterExitModShop
#else
	#define _ALS_OnEnterExitModShop
#endif
#define OnEnterExitModShop K_TSync_OnEnterExitModShop

#if defined K_TSync_OnEnterExitModShop
	forward K_TSync_OnEnterExitModShop(playerid);
#endif
