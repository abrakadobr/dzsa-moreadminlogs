modded class PluginAdminLog
{
	MoreAdminLogs h_madHelper;
	
	void PluginAdminLog()
	{
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
	}
			
	override void PlayerKilled( PlayerBase player, Object source )  // PlayerBase.c   
	{
		if (h_madHelper.IsOverride()) return;
		super.PlayerKilled( player, source);
	}
		
	override void PlayerHitBy( TotalDamageResult damageResult, int damageType, PlayerBase player, EntityAI source, int component, string dmgZone, string ammo ) // PlayerBase.c 
	{
		if (h_madHelper.IsOverride()) return;
		super.PlayerHitBy( damageResult, damageType, player, source, component, dmgZone, ammo );
	}
	
	override void UnconStart( PlayerBase player )	//  PlayerBase.c  
	{
		if (h_madHelper.IsOverride()) return;
		super.UnconStart( player );
	}
	
	override void UnconStop( PlayerBase player ) //  PlayerBase.c 
	{	
		if (h_madHelper.IsOverride()) return;
		super.UnconStop( player );
	}
		
	override void OnPlacementComplete( Man player, ItemBase item ) // ItemBase.c
	{
		if (h_madHelper.IsOverride()) return;
		super.OnPlacementComplete( player, item );
	}
	
	override void OnContinouousAction( ActionData action_data )	// ActionContinouousBase.c
	{
		if (h_madHelper.IsOverride()) return;
		super.OnContinouousAction( action_data );
	}
	
	override void Suicide( PlayerBase player )  // EmoteManager.c 
	{
		if (h_madHelper.IsOverride()) {
			h_madHelper.PlayerDeathDetails(player, "suicide");
		} else {
			super.Suicide( player );
		}
	}
	
	override void BleedingOut( PlayerBase player )  // Bleeding.c
	{
		if (h_madHelper.IsOverride()) {
			h_madHelper.PlayerDeathDetails(player, "bledout");
		} else {
		super.BleedingOut( player );
		}
	}
	
	override void PlayerList()
	{
		if (h_madHelper.IsOverride()) {
			h_madHelper.OnlineList();
		} else {
			super.PlayerList();
		}
	}
	
}