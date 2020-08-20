class CfgMods
{
	class MSB
	{
		dir				=	"MSB";
		picture			=	"";
		action			=	"";
		hideName		=	1;
		hidePicture		=	1;
		name			=	"MSB";
		credits			=	"123";
		author			=	"123";
		authorID		=	"0"; 
		version			=	"1.0"; 
		extra			=	0;
		type			=	"mod";
		dependencies[]	=	{"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule
			{
				value	=	"";
				files[]	=	{"MSB/scripts/3_Game"};
			}
			class worldScriptModule
			{
				value	=	"";
				files[]	=	{"MSB/scripts/4_World"};
			}
			class missionScriptModule
			{
				value	=	"";
				files[]	=	{"MSB/scripts/5_Mission"};
			}
		}
	}
}