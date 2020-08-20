modded class PlayerBase
{
	ref	MSBC_Manager	m_MSBC_Manager;
	
	override void OnConnect()
	{
		super.OnConnect();
		
		m_MSBC_Manager	=	new MSBC_Manager( this );
		m_MSBC_Manager.OnConnect();
	}
	
	override void OnDisconnect()
	{
		m_MSBC_Manager.OnDisconnect();

		super.OnDisconnect();
	}	
}