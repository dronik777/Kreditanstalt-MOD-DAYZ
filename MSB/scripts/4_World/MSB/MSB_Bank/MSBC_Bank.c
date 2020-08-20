class MSBC_Bank
{
	private	PlayerBase	m_Player;
	private	string		m_UID			=	"";
	private	bool		m_BankOpened	=	false;

	void MSBC_Bank( PlayerBase v_Player )
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Вход]: MSBC_Bank" ); }
		m_Player	=	PlayerBase.Cast( v_Player );
		m_UID		=	m_Player.GetIdentity().GetPlainId();
	}
	
	void ~MSBC_Bank()
	{
		if ( MSB_INT_DEBUG_LEVEL > 3 )
		{ MSBC_StaticFunctions.PrintDebug( "[Выход]: MSBC_Bank" ); }
	}
	
	bool IsBankOpen()
	{ return m_BankOpened; }
	
	void OnConnect()
	{
		int 					v_I		=	0;
		string					v_Data	=	"";
		MSBC_Bank_Location	v_BankLocation;

		for ( v_I	=	0; v_I < MSBC_Bank_Data.m_MSBC_Bank_Location.Count(); v_I++ )
		{
			v_BankLocation	=	MSBC_Bank_Data.m_MSBC_Bank_Location[v_I];
			v_Data	=	string.Format( "%1|%2|%3", MSB_INT_STC_PACKET_ID_BANK_DATA, v_BankLocation.GetStrPosition(), v_BankLocation.GetRadius() );
			MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false ); 
		}
		v_Data	=	string.Format( "%1|%2|%3", MSB_INT_STC_PACKET_ID_BANK_HOT_KEY, MSBC_Bank_Data.m_HotKey );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
	}
	
	void OnDisconnect()
	{
		
	}

	void OnRPC( string v_strData )
	{
		TStringArray	v_Data		=	{};
		int				v_PacketId	=	-1;
		
		v_strData.Split( "|", v_Data );
		v_PacketId	=	v_Data[0].ToInt();
		switch ( v_PacketId )
		{
			case MSB_INT_CTS_PACKET_ID_BANK_GET_PLAYERS:
				SendPlayers();
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_GET_CASH:
				MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_CASH, GetCashValue() ), NULL, false );		
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_GET_BANK:
				MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_BANK, GetBankValue( m_UID ) ), NULL, false );
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_CASH_TO_BANK:
				CashToBank( v_Data[1].ToInt() );
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_BANK_TO_CASH:
				BankToCash( v_Data[1].ToInt() );
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_TO_OTHER_BANK:
				BankToBank( v_Data[1], v_Data[2].ToInt() );
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_OPEN:
				m_BankOpened	=	true;
			break;
			case MSB_INT_CTS_PACKET_ID_BANK_CLOSE:
				m_BankOpened	=	false;
			break;
		}
	}
	
	void SendPlayers()
	{
		PlayerBase		v_Player	=	NULL;
		ref	array<Man>	v_Players	=	new array<Man>;				
		string			v_Data		=	"";

		GetGame().GetPlayers( v_Players );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, MSB_INT_STC_PACKET_ID_BANK_CLEAR_PLAYERS.ToString(), NULL, false );
		for ( int v_I = 0; v_I < v_Players.Count(); v_I++ )
		{
			v_Player	=	PlayerBase.Cast( v_Players[v_I] );
			if ( !v_Player )
			{ continue; }
			if ( m_Player ==  v_Player )
			{ continue; }
			v_Data	=	string.Format( "%1|%2|%3", MSB_INT_STC_PACKET_ID_BANK_PLAYER, v_Player.GetIdentity().GetName(), v_Player.GetIdentity().GetPlainId() );
			MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
		}
	}
	
	bool IsBankLocation()
	{
		vector					v_Position	=	m_Player.GetPosition();
		int						v_I			=	0;
		float					v_Distance	=	0;
		MSBC_Bank_Location	v_BankLocation;
		
		for ( v_I = 0; v_I < MSBC_Bank_Data.m_MSBC_Bank_Location.Count(); v_I++ )
		{
			v_BankLocation	=	MSBC_Bank_Data.m_MSBC_Bank_Location[v_I];
			v_Distance		=	MSBC_StaticFunctions.Distance2D( v_Position, v_BankLocation.GetPosition() );
			if ( v_Distance <= v_BankLocation.GetRadius() )
			{ return true; }
		}
		return false;
	}
	
	int GetCurrencyValueByClassname( string v_ClassName, int v_Amount )
	{
		int v_Value	=	0;
		switch ( v_ClassName )
		{
			case "MoneyRuble1":
				v_Value	=	v_Amount;
			break;
			case "MoneyRuble5":
				v_Value	=	v_Amount * 5;
			break;
			case "MoneyRuble10":
				v_Value	=	v_Amount * 10;
			break;
			case "MoneyRuble25":
				v_Value	=	v_Amount * 25;
			break;
			case "MoneyRuble50":
				v_Value	=	v_Amount * 50;
			break;
			case "MoneyRuble100":
				v_Value	=	v_Amount * 100;
			break;				
		}
		return v_Value;
	}
	
	int GetCashValue()
	{
		array<EntityAI>	v_Objects	=	new array<EntityAI>;
		int				v_I			=	0;
		ItemBase		v_ItemBase;
		Magazine		v_Magazine;
		string			v_Type		=	"";
		int				v_Amount	=	0;
		int				v_Result	=	0;

		m_Player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, v_Objects );
		for ( v_I = 0; v_I < v_Objects.Count(); v_I++ )
		{
			v_ItemBase	=	ItemBase.Cast(v_Objects[v_I]);
			if ( !v_ItemBase )
			{ continue; }
			v_Type	=	v_ItemBase.GetType();
			if ( MSBC_Bank_Data.m_CurrecyClassnames.Find( v_Type ) < 0 )
			{ continue; }
			if ( v_ItemBase.IsMagazine() )
			{
				v_Magazine	=	Magazine.Cast( v_ItemBase );
				if ( v_Magazine )
				{ v_Amount	=	v_Magazine.GetAmmoCount(); }
			}
			else
			{ v_Amount	=	QuantityConversions.GetItemQuantity( v_ItemBase ); }
			v_Result	=	v_Result + GetCurrencyValueByClassname( v_Type, v_Amount );
		}
		return v_Result;
	}
	
	void DeleteAllPlayerCurrency()
	{
		array<EntityAI>	v_Objects	=	new array<EntityAI>;
		int				v_I			=	0;
		ItemBase		v_ItemBase;
		string			v_Type		=	"";

		m_Player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, v_Objects );
		for ( v_I = 0; v_I < v_Objects.Count(); v_I++ )
		{
			v_ItemBase	=	ItemBase.Cast( v_Objects[v_I] );
			if ( !v_ItemBase )
			{ continue; }
			v_Type	=	v_ItemBase.GetType();
			if ( MSBC_Bank_Data.m_CurrecyClassnames.Find( v_Type ) < 0 )
			{ continue; }
			GetGame().ObjectDelete( v_ItemBase );	
		}
	}
	
	void GiveCurrencyByNominal( int v_Nominal, int v_Amount )
	{
		string		v_ClassName			=	"";
		int			v_TMP				=	0;
		int			v_I					=	0;
		EntityAI 	v_CurrencyEntityAI;
		ItemBase 	v_CurrencyItemBase;
		vector		v_PositionPlayer	=	m_Player.GetPosition();
		
		v_PositionPlayer[1]	=	GetGame().SurfaceY( v_PositionPlayer[0], v_PositionPlayer[2] );
		switch ( v_Nominal )
		{
			case 100:
				v_ClassName	=	"MoneyRuble100";
			break;
			case 50:
				v_ClassName	=	"MoneyRuble50";
			break;
			case 25:
				v_ClassName	=	"MoneyRuble25";
			break;
			case 10:
				v_ClassName	=	"MoneyRuble10";
			break;
			case 5:
				v_ClassName	=	"MoneyRuble5";
			break;
			case 1:
				v_ClassName	=	"MoneyRuble1";
			break;				
		}
		v_TMP	=	(int)(v_Amount / 500);
		for ( v_I = 0; v_I < v_TMP; v_I++ )
		{
			if ( MSBC_StaticFunctions.CanAddInInventory( m_Player, v_ClassName ) )
			{
				v_CurrencyEntityAI	=	m_Player.GetInventory().CreateInInventory( v_ClassName );
				if (v_CurrencyEntityAI)
				{
					v_CurrencyItemBase	=	ItemBase.Cast( v_CurrencyEntityAI );			
					if ( v_CurrencyItemBase )
					{ v_CurrencyItemBase.SetQuantity( 500 ); }
				}
			}
			else
			{
				v_CurrencyEntityAI	=	EntityAI.Cast( GetGame().CreateObject( v_ClassName, v_PositionPlayer, false ) );
				if (v_CurrencyEntityAI)
				{
					v_CurrencyItemBase	=	ItemBase.Cast( v_CurrencyEntityAI );
					if (v_CurrencyItemBase)
					{ v_CurrencyItemBase.SetQuantity( 500 ); }
					MSBC_StaticFunctions.SendMessage(m_Player, "Деньги упали на землю");
				}
			}
		}		
		v_Amount	=	( v_Amount - ( v_TMP * 500 ) );
		if ( MSBC_StaticFunctions.CanAddInInventory( m_Player, v_ClassName ) )
		{
			v_CurrencyEntityAI	=	m_Player.GetInventory().CreateInInventory( v_ClassName );
			if ( v_CurrencyEntityAI )
			{
				v_CurrencyItemBase	=	ItemBase.Cast( v_CurrencyEntityAI );
				if ( v_CurrencyItemBase )
				{ v_CurrencyItemBase.SetQuantity( v_Amount ); }
			}
		}
		else
		{
			v_CurrencyEntityAI	=	EntityAI.Cast( GetGame().CreateObject( v_ClassName, v_PositionPlayer, false ) );
			if ( v_CurrencyEntityAI )
			{
				v_CurrencyItemBase	=	ItemBase.Cast( v_CurrencyEntityAI );
				if ( v_CurrencyItemBase )
				{ v_CurrencyItemBase.SetQuantity( v_Amount ); }
			}
		}
	}
	
	int ExchangeCurrencyByNominal( int v_CurrentValue, int v_Nominal )
	{
		int	v_CurrencyToAdd	=	0;
		
		v_CurrencyToAdd	=	(int)( v_CurrentValue / v_Nominal );
		if ( v_CurrencyToAdd > 0 )
		{
			GiveCurrencyByNominal( v_Nominal, v_CurrencyToAdd );
			v_CurrencyToAdd	=	v_CurrencyToAdd * v_Nominal;
			v_CurrentValue	=	v_CurrentValue - v_CurrencyToAdd;
		}
		return v_CurrentValue;
	}
	
	void ExchangeAllPlayerCurrency( int v_CurrentValue )
	{
		DeleteAllPlayerCurrency();
		if ( v_CurrentValue == 0 )
		{ return; }	
		v_CurrentValue	=	ExchangeCurrencyByNominal( v_CurrentValue, 100 );
		v_CurrentValue	=	ExchangeCurrencyByNominal( v_CurrentValue, 50 );
		v_CurrentValue	=	ExchangeCurrencyByNominal( v_CurrentValue, 25 );
		v_CurrentValue	=	ExchangeCurrencyByNominal( v_CurrentValue, 10 );
		v_CurrentValue	=	ExchangeCurrencyByNominal( v_CurrentValue, 5 );
		if ( v_CurrentValue > 0 )
		{ GiveCurrencyByNominal( 1, v_CurrentValue ); }
	}
	
	bool IsExistBank( string v_UID )
	{
		string	v_fName	=	string.Format( "%1%2%3.txt", MSB_STRING_BANK_FOLDER, MSB_STRING_BANK_PREFIX, v_UID );
		
		if ( FileExist( v_fName ) )
		{ return true; }
		return false;
	}
	
	int GetBankValue( string v_UID )
	{
		FileHandle	v_fHandle;
		string		v_fName		=	string.Format( "%1%2%3.txt", MSB_STRING_BANK_FOLDER, MSB_STRING_BANK_PREFIX, v_UID );
		string		v_Buffer	=	"";
			
		if ( !FileExist( v_fName ) )
		{ return 0; }
		v_fHandle	=	OpenFile( v_fName, FileMode.READ );
		if ( v_fHandle == 0 )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format(" [MSBC_Bank]: [Загрузка по UID]: [Ошибка]: Не удалось открыть файл по UID: %1", v_UID ) );
			CloseFile(v_fHandle);
			return -1;
		}
		while ( FGets( v_fHandle,  v_Buffer ) > 0 )
		{
			CloseFile(v_fHandle);	
			return v_Buffer.ToInt();
		}
		MSBC_StaticFunctions.PrintDebug( string.Format(" [MSBC_Bank]: [Загрузка по UID]: [Ошибка]: Не удалось прочитать данные в файле по UID: %1", v_UID ) );
		return -1;
	}
	
	bool SetBankValue( string v_UID, int v_Value )
	{
		FileHandle	v_fHandle;
		string		v_fName		=	string.Format( "%1%2%3.txt", MSB_STRING_BANK_FOLDER, MSB_STRING_BANK_PREFIX, v_UID );
		string		v_TMPfile	=	string.Format( "%1TMP_%2%3.txt", MSB_STRING_BANK_FOLDER, MSB_STRING_BANK_PREFIX, v_UID );
		string		v_Buffer	=	"";

		v_fHandle	=	OpenFile( v_TMPfile, FileMode.WRITE );
		if ( v_fHandle == 0 )
		{
			MSBC_StaticFunctions.PrintDebug( string.Format(" [MSBC_Bank]: [Запись по UID]: [Ошибка]: Не удалось создать временный файл по UID: %1", v_UID ) );
			CloseFile(v_fHandle);
			return false;
		}
		FPrintln( v_fHandle, v_Value );
		CloseFile( v_fHandle );
		if ( CopyFile( v_TMPfile, v_fName ) )
		{
			DeleteFile( v_TMPfile );
			return true;
		}
		else
		{
			MSBC_StaticFunctions.PrintDebug( string.Format(" [MSBC_Bank]: [Запись по UID]: [Ошибка]: Не удалось скопировать временный файл в основной по UID: %1", v_UID ) );
			DeleteFile( v_TMPfile );
			return false;
		}
	}
	
	void CashToBank( int v_Value )
	{
		int		v_BankValue	=	GetBankValue( m_UID );
		int 	v_CashValue	=	GetCashValue();
		string	v_Data		=	"";
		
		if ( v_BankValue < 0 )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		if ( v_Value > v_CashValue )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Недостаточно средств." );
			return;
		}
		v_BankValue	=	v_BankValue	+ v_Value;
		v_CashValue	=	v_CashValue - v_Value;
		if ( !SetBankValue( m_UID, v_BankValue ) )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		ExchangeAllPlayerCurrency( v_CashValue );
		MSBC_StaticFunctions.SendMessage( m_Player, string.Format( "Наличные %1 зачислены на банковский счёт.", v_Value ) );
		//	
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_BANK, v_BankValue );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_CASH, v_CashValue );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
	
	}
	
	void BankToCash( int v_Value )
	{
		int		v_BankValue	=	GetBankValue( m_UID );
		int 	v_CashValue	=	GetCashValue();
		string	v_Data		=	"";
		
		if ( v_BankValue < 0 )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		if ( v_BankValue < v_Value )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Недостаточно средств на банковском счету." );
			return;
		}
		v_BankValue	=	v_BankValue - v_Value;
		v_CashValue	=	v_CashValue + v_Value;
		if ( !SetBankValue( m_UID, v_BankValue ) )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		ExchangeAllPlayerCurrency( v_CashValue );
		MSBC_StaticFunctions.SendMessage( m_Player, string.Format( "Наличные %1 сняты с банковского счёта.", v_Value ) );
		//	
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_BANK, v_BankValue );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_CASH, v_CashValue );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
	}
	
	void BankToBank( string v_ToUID, int v_Value )
	{
		int			v_BankValue		=	GetBankValue( m_UID );
		int			v_ToBankValue	=	0;
		string		v_Data			=	"";
		PlayerBase	v_PlayerTo;
		
		if ( v_BankValue < 0 )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		if ( v_BankValue < v_Value )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Недостаточно средств на банковском счету." );
			return;
		}
		if ( v_ToUID == m_UID )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Вы не можете перевести средства самому себе." );
			return;			
		}
		if ( !IsExistBank( v_ToUID ) )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Неверный UID!" );
			return;			
		}
		v_ToBankValue	=	GetBankValue( v_ToUID );
		if ( v_ToBankValue < 0 )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		v_BankValue		=	v_BankValue - v_Value;
		v_ToBankValue	=	v_ToBankValue + v_Value;
		if ( !SetBankValue( m_UID, v_BankValue ) )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Ошибка, обратитесь к администрации." );
			return;
		}
		if ( !SetBankValue( v_ToUID, v_ToBankValue ) )
		{
			MSBC_StaticFunctions.SendMessage( m_Player, "Недостаточно средств на банковском счету." );
			return;
		}
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_BANK, v_BankValue );
		MSBC_StaticFunctions.SendPacket( m_Player, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
		
		v_PlayerTo	=	MSBC_StaticFunctions.GetPlayerBaseByUID( v_ToUID );
		if ( !v_PlayerTo )
		{ return; }
		if ( !v_PlayerTo.m_MSBC_Manager )
		{ return; }
		if ( !v_PlayerTo.m_MSBC_Manager.m_MSBC_Bank )
		{ return; }
		if ( !v_PlayerTo.m_MSBC_Manager.m_MSBC_Bank.IsBankOpen() )
		{ return; }
		v_Data	=	string.Format( "%1|%2", MSB_INT_STC_PACKET_ID_BANK_VALUE_BANK, v_ToBankValue );
		MSBC_StaticFunctions.SendPacket( v_PlayerTo, MSB_INT_STC_PACKET_ID_BANK, v_Data, NULL, false );
	}
	
}