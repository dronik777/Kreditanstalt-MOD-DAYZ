modded class DayZPlayerImplement
{
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC( sender, rpc_type, ctx );
		
		if ( !sender )
		{ return; }
		if ( rpc_type != MSB_INT_PACKET_ID_MOD )
		{ return; }
	
		PlayerBase	v_MSB_Player	=	MSBC_StaticFunctions.GetPlayerBaseByIdentity( sender );
		if ( !v_MSB_Player )
		{ return; }
		if ( !v_MSB_Player.m_MSBC_Manager )
		{ return; }
		v_MSB_Player.m_MSBC_Manager.OnRPC( v_MSB_Player, ctx );
	}
}