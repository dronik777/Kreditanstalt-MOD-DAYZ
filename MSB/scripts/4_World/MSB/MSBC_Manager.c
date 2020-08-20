class MSBC_Manager
{
	private 	PlayerBase 	m_Player;
	private		string		m_UID			=	"";
	private		string		m_Name			=	"";
			ref	MSBC_Bank	m_MSBC_Bank;

	void MSBC_Manager( PlayerBase v_Player )
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Вход]: MSBC_Manager" ); }
		m_Player		=	v_Player;
		m_UID			=	m_Player.GetIdentity().GetPlainId();
		m_Name			=	m_Player.GetIdentity().GetName();
		m_MSBC_Bank	=	new MSBC_Bank( m_Player );
	}
	
	void ~MSBC_Manager()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Выход]: MSBC_Manager" ); }
	}

	void OnConnect()
	{
		MSBC_StaticFunctions.PrintDebug( string.Format( "Присоединился игрок: %1, UID: %2", m_Name, m_UID ) );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_UID, m_UID, NULL, false );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_INIT, "", NULL, false );
		if ( m_MSBC_Bank )
		{ m_MSBC_Bank.OnConnect(); }
	}
	
	void OnDisconnect()
	{
		MSBC_StaticFunctions.PrintDebug( string.Format( "Отсоединился игрок: %1, UID: %2", m_Name, m_UID ) );
		if ( m_MSBC_Bank )
		{ m_MSBC_Bank.OnDisconnect(); }
	}

	void OnRPC( PlayerBase v_Player, ParamsReadContext v_ParamsReadContext )
	{
		Param3<int, string, Object>	v_Param3	=	new Param3<int, string, Object>( -1, "", NULL );	
		int							v_PacketID	=	-1;
		string						v_strData	=	"";
		Object						v_Object;
	
		v_ParamsReadContext.Read( v_Param3 );
		v_PacketID		=	v_Param3.param1;
		v_strData		=	v_Param3.param2;
		v_Object		=	v_Param3.param3;		
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( string.Format( "[OnRPC]: Пакет от игрока. Отправитель: %1. Данные - Идентификатор: %2, Строка: %3, Объект: %4.", v_Player.GetIdentity(), v_PacketID, v_strData, v_Object ) ); }
		switch ( v_PacketID )
		{
			case MSB_INT_CTS_PACKET_ID_BANK:
				if ( m_MSBC_Bank )
				{ m_MSBC_Bank.OnRPC( v_strData ); }
			break;
		}
	}
}