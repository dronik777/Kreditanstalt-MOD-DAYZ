class MSBC_Bank_Data
{
	static ref	TStringArray 					m_CurrecyClassnames		=	{ "MoneyRuble1", "MoneyRuble5", "MoneyRuble10", "MoneyRuble25", "MoneyRuble50", "MoneyRuble100" };
	static ref	array<ref MSBC_Bank_Location>	m_MSBC_Bank_Location	=	new array<ref MSBC_Bank_Location>;
	static		int								m_HotKey				=	0;
	
	void MSBC_Bank_Data()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Вход]: MSBC_Bank_Data" ); }
		LoadBanksData();
		LoadHotKey();
	}
	
	void ~MSBC_Bank_Data()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Выход]: MSBC_Bank_Data" ); }
	}
	
	void LoadHotKey()
	{
		FileHandle	v_fHandle;
		string		v_Buffer	=	"";
		
		if ( !FileExist( MSB_STRING_BANK_HOT_KEY_CONFIG ) )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: Отсутствует файл: %1", MSB_STRING_BANK_HOT_KEY_CONFIG ) );
			return; 
		}
		v_fHandle	=	OpenFile( MSB_STRING_BANK_HOT_KEY_CONFIG, FileMode.READ );
		if ( v_fHandle == 0 )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: [Ошибка]: Не удалось открыть файл для чтения: %1", MSB_STRING_BANK_HOT_KEY_CONFIG ) );
			CloseFile( v_fHandle );
			return;
		}
		while ( FGets( v_fHandle,  v_Buffer ) > 0 )
		{}
		m_HotKey	=	v_Buffer.ToInt();
		CloseFile( v_fHandle );
		MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: Прочитан файл: %1", MSB_STRING_BANK_HOT_KEY_CONFIG ) );
	}
	
	void LoadBanksData()
	{
		FileHandle		v_fHandle;
		string			v_Buffer	=	"";
		TStringArray	v_strBank	=	{};
		
		m_MSBC_Bank_Location	=	{};		
		if ( !FileExist( MSB_STRING_BANK_DATA_CONFIG ) )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: Отсутствует файл: %1", MSB_STRING_BANK_DATA_CONFIG ) );
			return; 
		}
		v_fHandle	=	OpenFile( MSB_STRING_BANK_DATA_CONFIG, FileMode.READ );
		if ( v_fHandle == 0 )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: [Ошибка]: Не удалось открыть файл для чтения: %1", MSB_STRING_BANK_DATA_CONFIG ) );
			CloseFile( v_fHandle );
			return;
		}
		while ( FGets( v_fHandle,  v_Buffer ) > 0 )
		{
			v_strBank	=	{};
			v_Buffer.Split( "|", v_strBank );
			if ( v_strBank.Count() == 2 )
			{ m_MSBC_Bank_Location.Insert( new MSBC_Bank_Location( v_strBank[0].ToVector(), v_strBank[1].ToInt() ) ); }
		}
		CloseFile( v_fHandle );
		MSBC_StaticFunctions.PrintDebug( string.Format( "[MSBC_Bank_Loader]: Прочитан файл: %1", MSB_STRING_BANK_DATA_CONFIG ) );
	}
}