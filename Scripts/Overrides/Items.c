/*
modded class MagazineStorage
{
	override void ServerAddAmmoCount(int ammoCount)
	{
		super.ServerAddAmmoCount(ammoCount);
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		PlayerBase owner = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!owner) {
			h_madHelper.NoPlayerLog("MAGAZINE_COUNT", h_madHelper.ItemInfo(this));
		} else {
			h_madHelper.PlayerLog(owner, "MAGAZINE_COUNT", h_madHelper.ItemInfo(this));
		}
	}
}

modded class Ammunition_Base
{
	override void ServerAddAmmoCount(int ammoCount)
	{
		super.ServerAddAmmoCount(ammoCount);
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		PlayerBase owner = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!owner) {
			h_madHelper.NoPlayerLog("AMMO_COUNT", h_madHelper.ItemInfo(this));
		} else {
			h_madHelper.PlayerLog(owner, "AMMO_COUNT", h_madHelper.ItemInfo(this));
		}
	}
}
*/


modded class ItemBase
{
	MoreAdminLogs h_madHelper;
	
	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		h_madHelper.LogItemHit(this, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	
	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );
		h_madHelper.LogItemKilled(this, killer);
	}
	
	
	/*
	override void Open() {
		super.Open();
		if (!h_madHelper || !h_madHelper.IsEnabled() || !this) return;
		Man killer = this.GetHierarchyRootPlayer();
		string itemInfo = h_madHelper.ItemInfo(this);
		string message = string.Format("#open item=<%1>", itemInfo);
		if (h_madHelper.IsJson()) message = string.Format("{ \"tag\": \"#open\", \"item\": \"%1\" }", itemInfo);
		if (killer) {
			PlayerBase player = PlayerBase.Cast(killer);
			h_madHelper.PlayerLog(player, "ITEM_OPEN_CLOSE", message);
		} else {
			h_madHelper.NoPlayerLog("ITEM_OPEN_CLOSE", message);
		}
	}
	
	override void Close() {
		super.Close();
		if (!h_madHelper || !h_madHelper.IsEnabled() || !this) return;
		Man killer = this.GetHierarchyRootPlayer();
		string itemInfo = h_madHelper.ItemInfo(this);
		string message = string.Format("#close item=<%1>", itemInfo);
		if (h_madHelper.IsJson()) message = string.Format("{ \"tag\": \"#close\", \"item\": \"%1\" }", itemInfo);
		if (killer) {
			PlayerBase player = PlayerBase.Cast(killer);
			h_madHelper.PlayerLog(player, "ITEM_OPEN_CLOSE", message);
		} else {
			h_madHelper.NoPlayerLog("ITEM_OPEN_CLOSE", message);
		}
	}
	*/
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if (h_madHelper) h_madHelper.LogItemPlaced(this, player, position , orientation );
		super.OnPlacementComplete( player, position , orientation );
	}
	
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		if (h_madHelper) h_madHelper.LogItemLocation(this, oldLoc, newLoc);
		super.EEItemLocationChanged(oldLoc,newLoc);
	}
	
	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		if (h_madHelper) h_madHelper.LogItemAttached(this, parent, slot_id);
		super.OnWasAttached(parent, slot_id);
	}
	
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		if (h_madHelper) h_madHelper.LogItemDetached(this, parent, slot_id);
		super.OnWasDetached(parent, slot_id);
	}
	
	override bool SetQuantity(float value, bool destroy_config = true, bool destroy_forced = false, bool allow_client = false, bool clamp_to_stack_max = true)
	{
		bool doDestory = super.SetQuantity(value, destroy_config , destroy_forced , allow_client, clamp_to_stack_max);
		if (h_madHelper) h_madHelper.LogItemQuantity(this, value, doDestory);
		return doDestory;
	}
}

modded class BaseBuildingBase {
	//MoreAdminLogs h_madHelper;
	void BaseBuildingBase() 
	{
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		h_madHelper.LogBaseDestroyPart(this, player, part_name, action_id, destroyed_by_connected_part );
		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );
	}
	
	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		h_madHelper.LogBaseDismantlePart(this, player, part_name, action_id );
		super.OnPartDismantledServer( player, part_name, action_id );
	}
	
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		h_madHelper.LogBaseBuildPart(this, player, part_name, action_id );
		super.OnPartBuiltServer( player, part_name, action_id );
	}
}