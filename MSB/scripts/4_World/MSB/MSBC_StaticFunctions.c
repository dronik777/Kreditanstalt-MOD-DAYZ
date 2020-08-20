class MSBC_StaticFunctions
{
	static float Distance2D(vector v_PointA, vector v_PointB)
	{return Math.Sqrt( Math.Pow((v_PointA[0] - v_PointB[0]), 2) + Math.Pow((v_PointA[2] - v_PointB[2]), 2) );}

	static string GetDateTime()
	{
		int			v_Year, v_Month, v_Day, v_Hour, v_Minute, v_Second;
		string		v_strYear, v_strMonth, v_strDay, v_strHour, v_strMinute, v_strSecond, v_DateTime;
		
		GetYearMonthDay(v_Year, v_Month, v_Day);
		GetHourMinuteSecond(v_Hour, v_Minute, v_Second);
		v_strYear	=	v_Year.ToString();
		if (v_Month < 10)
		{v_strMonth	=	"0" + v_Month.ToString();}
		else
		{v_strMonth	=	v_Month.ToString();}
		if (v_Day < 10)
		{v_strDay	=	"0" + v_Day.ToString();}
		else
		{v_strDay	=	v_Day.ToString();}
		if (v_Hour < 10)
		{v_strHour	=	"0" + v_Hour.ToString();}
		else
		{v_strHour	=	v_Hour.ToString();}
		if (v_Minute < 10)
		{v_strMinute	=	"0" + v_Minute.ToString();}
		else
		{v_strMinute	=	v_Minute.ToString();}
		if (v_Second < 10)
		{v_strSecond	=	"0" + v_Second.ToString();}
		else
		{v_strSecond	=	v_Second.ToString();}
		return (v_strDay + "." + v_strMonth + "." + v_strYear + " " + v_strHour + ":" + v_strMinute + ":" + v_strSecond);
	}

	static void SaveLog(string v_FileName, string v_TextLog)
	{
		FileHandle	v_fHandle;					
		string		v_DateTime	=	GetDateTime();				
		if ( !FileExist(v_FileName) )
		{v_fHandle	=	OpenFile(v_FileName, FileMode.WRITE);}
		else
		{v_fHandle	=	OpenFile(v_FileName, FileMode.APPEND);}
		if ( v_fHandle == 0 )
		{
			Print("[" + v_DateTime + "]: [MSB]: [Журнал]: Не удалось открыть файл для записи: " + v_FileName);
			Print("[" + v_DateTime + "]: [MSB]: [Журнал]: [Запись]: " + v_TextLog);
			return;
		}
		FPrintln(v_fHandle, "[" + v_DateTime + "]: " + v_TextLog);
		CloseFile(v_fHandle);
	}

	static void PrintDebug( string v_Text )
	{ Print(string.Format("[%1]: [MSB]: %2", GetDateTime(), v_Text)); }			

	static PlayerBase GetPlayerBaseByIdentity(PlayerIdentity v_PlayerIdentity)
	{
		int	networkIdLowBits	=	0;
		int	networkIdHightBits	=	0;
	
		GetGame().GetPlayerNetworkIDByIdentityID(v_PlayerIdentity.GetPlayerId(), networkIdLowBits, networkIdHightBits );				
		return PlayerBase.Cast(GetGame().GetObjectByNetworkId(networkIdLowBits, networkIdHightBits ));
	}

	static void SendMessage( PlayerBase v_Player, string v_Message )
	{
		Param1<string>	v_ParamMessage	=	new Param1<string>(v_Message);
		GetGame().RPCSingleParam(v_Player, ERPCs.RPC_USER_ACTION_MESSAGE, v_ParamMessage, true, v_Player.GetIdentity());
	}
	
	static bool CanAddInInventory( PlayerBase v_Player, string v_ClassName )
	{
		EntityAI 			v_HandEntityAI;
		InventoryLocation	v_InventoryLocation;
				
		v_HandEntityAI	=	v_Player.GetHumanInventory().GetEntityInHands();
		if ( !v_HandEntityAI )
		{ return true; }				
		v_InventoryLocation	=	new InventoryLocation;
		if ( v_Player.GetInventory().FindFirstFreeLocationForNewEntity(v_ClassName, FindInventoryLocationType.ANY, v_InventoryLocation) )
		{ return true; }
		return false;			
	}
	
	static PlayerBase GetPlayerBaseByUID(string v_UID)
	{
		PlayerBase		v_Player	=	NULL;
		ref	array<Man>	v_Players	=	new array<Man>;				
	
		GetGame().GetPlayers(v_Players);
		for ( int v_I = 0; v_I < v_Players.Count(); v_I++ )
		{
			PlayerBase.CastTo( v_Player, v_Players.Get( v_I ) );
			if ( v_Player.GetIdentity().GetPlainId() == v_UID )
			{ return v_Player; }
		}
		return NULL;
	}
	
	static void SendPacket( PlayerBase v_Player, int v_PacketID, string v_strData, Object v_Object, bool v_Broadcast )
	{
			Param3<int, string, Object>	v_Param3	=	new Param3<int, string, Object>( v_PacketID, v_strData, v_Object );
			int							v_I			=	0;
		ref array<Man>					v_Players	=	new array<Man>;
		
		if ( !v_Player && !v_Broadcast )
		{ return; }
		if ( v_Broadcast )
		{
			GetGame().GetPlayers( v_Players );
			for ( v_I = 0; v_I < v_Players.Count(); v_I++ )
			{
				v_Player	=	PlayerBase.Cast( v_Players[v_I] );
				if ( v_Player )
				{ GetGame().RPCSingleParam( v_Player, MSB_INT_PACKET_ID_MOD, v_Param3, true, v_Player.GetIdentity() ); }
			}
			return;
		}
		GetGame().RPCSingleParam( v_Player, MSB_INT_PACKET_ID_MOD, v_Param3, true, v_Player.GetIdentity() );	
	}
}