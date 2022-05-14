class CfgPatches
{
	class MoreAdminLogs
	{
		requiredAddons[]={"DZ_Data"};
	};
};

class CfgMods
{
	class MoreAdminLogs
	{
		type = "mod";

		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"MoreAdminLogs/Scripts/Overrides"};
			};
		};
	};
};