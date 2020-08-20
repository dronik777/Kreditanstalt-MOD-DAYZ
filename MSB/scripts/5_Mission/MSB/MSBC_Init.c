class MSBC_Init
{
	void MSBC_Init()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Вход]: MSBC_Init" ); }
		OnInit();
	}
	
	void ~MSBC_Init()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Выход]: MSBC_Init" ); }		
	}
	
	void OnInit()
	{
		new MSBC_Bank_Data();
	}
}