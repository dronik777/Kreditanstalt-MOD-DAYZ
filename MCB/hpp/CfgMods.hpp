class CfgMods
{
	class MCB
	{
		dir				=	"MCB";
		picture			=	"";
		action			=	"";
		hideName		=	1;
		hidePicture		=	1;
		name			=	"MCB";
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
				files[]	=	{"MCB/scripts/3_Game"};
			}
			class worldScriptModule
			{
				value	=	"";
				files[]	=	{"MCB/scripts/4_World"};
			}
			class missionScriptModule
			{
				value	=	"";
				files[]	=	{"MCB/scripts/5_Mission"};
			}
		}
	}
}