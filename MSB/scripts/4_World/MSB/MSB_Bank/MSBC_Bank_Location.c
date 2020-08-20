class MSBC_Bank_Location
{
	vector	m_Position	=	"0 0 0";
	int		m_Radius	=	0;

	void MSBC_Bank_Location( vector v_Position, int v_Radius )
	{
		m_Position	=	v_Position;
		m_Radius	=	v_Radius;
	}
	
	void ~MSBC_Bank_Location()
	{
		
	}
	
	vector GetPosition()
	{ return m_Position; }
	
	string GetStrPosition()
	{ return string.Format( "%1 %2 %3", m_Position[0], m_Position[1], m_Position[2] ); }
	
	int GetRadius()
	{ return m_Radius; }
	
}