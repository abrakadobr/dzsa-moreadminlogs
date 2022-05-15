
class MoreAdminLogs extends PluginBase
{
	const int LOG_MORE_ADMIN_LOGS_VERSION = 4;
	
	ref map<string, bool> eventsFilters;
	ref MoreAdminLogsHelper _helper;
	
    void MoreAdminLogs()
    {
		
		this.eventsFilters = new map<string, bool>();
		autoptr TStringArray codes = new TStringArray;
		codes.Insert("LOG_MORE_ADMIN_LOGS");
		codes.Insert("-");

		codes.Insert("LOG_JSON");
		codes.Insert("LOG_FILTER_ALERT");
		codes.Insert("LOG_OVERRIDE");
		codes.Insert("LOG_ONLINE");
		codes.Insert("LOG_NO_WORLD");
		codes.Insert("-");
		
		codes.Insert("LOG_PLAYER_MANAGEMENT");
		codes.Insert("LOG_PLAYER_MOVEMENT");
		codes.Insert("LOG_PLAYER_VEHICLE");
		codes.Insert("LOG_PLAYER_HEALTH");
		codes.Insert("LOG_PLAYER_GEAR");
		codes.Insert("LOG_PLAYER_HANDS");
		codes.Insert("-");
		
		codes.Insert("LOG_PLAYER_DAMAGE");
		codes.Insert("LOG_PLAYER_LETHAL_DAMAGE");
		codes.Insert("LOG_PLAYER_DEATH");
		codes.Insert("LOG_PLAYER_DEATH_DETAILS");
		codes.Insert("-");
		
		codes.Insert("LOG_ACTION_LOG");
		codes.Insert("LOG_ACTION_CONTINUOUS_LOG");
		codes.Insert("LOG_ACTION_CONTINUOUS_SERVER_LOG");
		codes.Insert("LOG_ACTION_ANIMATED_LOG");
		codes.Insert("LOG_CRAFT_LOG");
		codes.Insert("-");
		
		codes.Insert("LOG_ITEM_DAMAGE");
		codes.Insert("LOG_ITEM_LETHAL_DAMAGE");
		codes.Insert("LOG_ITEM_DEATH");
		codes.Insert("-");

		codes.Insert("LOG_ITEM_ATTACH");
		codes.Insert("LOG_ITEM_DETACH");
		codes.Insert("LOG_ITEM_QUANTITY");
		codes.Insert("LOG_ITEM_DESTROY");
		codes.Insert("LOG_ITEM_LOCATION");
		codes.Insert("LOG_ITEM_PLACED");
		codes.Insert("-");

		codes.Insert("LOG_BASEBUILDING");
		codes.Insert("-");

		codes.Insert("LOG_INFECTED_DAMAGE");
		codes.Insert("LOG_INFECTED_LETHAL_DAMAGE");
		codes.Insert("LOG_INFECTED_DEATH");
		codes.Insert("-");

		codes.Insert("LOG_ANIMAL_DAMAGE");
		codes.Insert("LOG_ANIMAL_LETHAL_DAMAGE");
		codes.Insert("LOG_ANIMAL_DEATH");
		// codes.Insert("-");

		// codes.Insert("LOG_MAGAZINE_COUNT");
		// codes.Insert("LOG_AMMO_COUNT");
		
		foreach (string key: codes) {
			if (key != "-") {
				string cfgKey = logConfigKey(key);
				string logKey = key.Substring(4, key.Length() - 1);
				this.eventsFilters[logKey] = GetGame().ServerConfigGetInt(cfgKey) == 1;
			}
		}
		
		this._helper = new MoreAdminLogsHelper(IsJson(), IsOverride());
		
		if (IsEnabled()) {
			string message = string.Format("*  More Admin Logs :: version=<%1> json=<%2> override=<%3>", LOG_MORE_ADMIN_LOGS_VERSION, IsJson(), IsOverride());
			DirectLog(message);
			string jsonMessage = "";
			foreach (string key1: codes) {
				if (key1 == "-") {
					DirectLog("*");
				} else {
					string fkey = key1.Substring(4, key1.Length() - 1);
					bool keyState = eventsFilters[fkey];
					message = string.Format("*  <%1> (%3) is <%2>", fkey , keyState, logConfigKey(key1));
					if (jsonMessage != "") jsonMessage += ", ";
					jsonMessage += string.Format("\"%1\":%2", fkey, keyState);
					DirectLog(message);
				}
			}
			DirectLog(string.Format("JSON{\"moreAdminLogs\":%1,\"version\":%2,\"json\":%3,\"override\":%4,\"config\":{%5}}", IsEnabled(), LOG_MORE_ADMIN_LOGS_VERSION, IsJson(), IsOverride(), jsonMessage));
		}
    }

    void ~MoreAdminLogs()
    {
		delete this._helper;
		delete this.eventsFilters;
	}
	
	MoreAdminLogsHelper helper() {
		return this._helper;
	}

	string logConfigKey(string logKey) {
		if (logKey == "LOG_MORE_ADMIN_LOGS") return "moreAdminLogs";
		string uppers = "QWERTYUIOPASDFGHJKLZXCVBNM";
		string lowers = "qwertyuiopasdfghjklzxcvbnm";
		string result = "";
		bool wordEnd = true;
		// skip LOG_
		for (int i = 4; i < logKey.Length(); i++) {
			string symb = logKey.Get(i);
			if (symb == "_") {
				wordEnd = true;
			} else {
				if (wordEnd) {
					result += symb;
					wordEnd = false;
				} else {
					int pos = uppers.IndexOf(symb);
					if (pos >= 0) result += lowers.Get(pos);
				}
			}
		}
		return "moreAdminLogs" + result;
	}
	
	bool IsEnabled()
	{
		return !!this.eventsFilters["MORE_ADMIN_LOGS"];
	}
	
	bool IsJson() {
		return !!this.eventsFilters["JSON"];
	}

	bool IsOverride() {
		return !!this.eventsFilters["OVERRIDE"];
	}

	bool IsNoWorld() {
		return !!this.eventsFilters["NO_WORLD"];
	}

	bool _FilterPass(string source)
	{
		if (!this.eventsFilters["MORE_ADMIN_LOGS"]) {	return false; }
		return !!this.eventsFilters[source];
	}

	bool FilterPass(string source)
	{
		bool ok = _FilterPass(source);
		if (!ok && this.eventsFilters["FILTER_ALERT"]) {
			DirectLog("FILTER!!! " + source);
		}
		return ok;
	}

	//////////////////////// LOGS

	/// PLAYER_MANAGEMENT
	void LogPlayerManagement(PlayerBase player, string tag)
	{
		if (!IsEnabled()) return;
		string message = tag;
		if (IsJson()) message = string.Format("{\"tag\":\"%1\"}", tag);
		PlayerLog(player, "PLAYER_MANAGEMENT", message, true);
	}
	
	/// PLAYER_HANDS
	void LogPlayerHands(PlayerBase player, string tag, EntityAI item)
	{
		if (!IsEnabled()) return;
		string message = string.Format("%1 item=<%2>", tag, _helper.EntityInfo(item));
		if (IsJson()) message = string.Format("{\"tag\":\"%1\",\"item\":%2}", tag, _helper.EntityInfo(item));
		PlayerLog(player, "PLAYER_HANDS", message);
	}
	
	/// PLAYER_MOVEMENT
	void LogPlayerMovement(PlayerBase player, string tag, float speed = 0)
	{
		if (!IsEnabled()) return;
		string message = tag;
		if (speed > 0) message = string.Format("%1 speed=<%2>", tag, speed);
		if (IsJson()) {
			if (speed > 0) {
				message = string.Format("{\"tag\":\"%1\",\"speed\":%2}", tag, speed);
			} else {
				message = string.Format("{\"tag\":\"%1\"}", tag);
			}
		}
		PlayerLog(player, "PLAYER_MOVEMENT", message);
	}
	
	/// PLAYER_VEHICLE
	void LogPlayerVehicle(PlayerBase player, string tag)
	{
		if (!IsEnabled()) return;
		string message = tag;
		if (IsJson()) message = string.Format("{\"tag\":\"%1\"}", tag);
		PlayerLog(player, "PLAYER_VEHICLE", message);
	}

	/// PLAYER_HEALTH
	void LogPlayerHealth(PlayerBase player, string tag)
	{
		if (!IsEnabled()) return;
		string message = tag;
		if (IsJson()) message = string.Format("{\"tag\":\"%1\"}", tag);
		PlayerLog(player, "PLAYER_HEALTH", message, true);
	}
	
	void LogPlayerHealthBleeds(PlayerBase player, int bleedingBits)
	{
		if (!IsEnabled()) return;
		string message = string.Format("#bleeds bits=<%1>", bleedingBits);
		if (IsJson()) message = string.Format("{\"tag\":\"#bleeds\",\"bits\":%1}", bleedingBits);
		PlayerLog(player, "PLAYER_HEALTH", message, true);
	}
	
	void LogPlayerHealthConsume(PlayerBase player, ItemBase source, float amount)
	{
		if (!IsEnabled()) return;
		string sourceInfo = _helper.ItemInfo(source);
		string message = string.Format("#consume source=<%1> amount=<%2>", sourceInfo, amount);
		if (IsJson()) message = string.Format("{\"tag\":\"#consume\",\"source\":%1,\"amount\":%2}", sourceInfo, amount);
		PlayerLog(player, "PLAYER_HEALTH", message, true);
	}

	/// PLAYER_GEAR
	void LogPlayerGear(PlayerBase player, string tag, EntityAI item, string slot_name)
	{
		if (!IsEnabled()) return;
		string itemInfo = _helper.EntityInfo(item);
		string message = string.Format("%1 item=<%1> slot=<%3>", tag, itemInfo, slot_name);
		if (IsJson()) message = string.Format("{\"tag\":\"%1\",\"item\":%2,\"slot\":\"%3\"}", tag, itemInfo, slot_name);
		PlayerLog(player, "PLAYER_GEAR", message);
	}
	
	/// PLAYER_INVENTORY
	void LogPlayerInventory(PlayerBase player, string tag, EntityAI item, EntityAI from, EntityAI to)
	{
		if (!IsEnabled()) return;
		string itemInfo = _helper.EntityInfo(item);
		string fromInfo = _helper.EntityInfo(item);
		string toInfo = _helper.EntityInfo(item);
		string message = string.Format("#%1 item=<%2> from=<%3> to=<%4>", tag, itemInfo, fromInfo, toInfo);
		if (IsJson()) message = string.Format("{\"tag\":\"%1\",\"item\":%2,\"from\":%3,\"to\":%4}", tag, itemInfo, fromInfo, toInfo);
		PlayerLog(player, "PLAYER_INVENTORY", message);
	}
	
	/// PLAYER DAMAGE
	void LogPlayerHit(PlayerBase player, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef, bool sameAlive = true)
	{
		if (!IsEnabled() || !IsOverride()) return;
		LogHit("PLAYER", player, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, sameAlive);
	}

	/// PLAYER DEATH
	void LogPlayerKilled(PlayerBase player, Object killer)
	{
		if (!IsEnabled() || !IsOverride()) return;
		LogKilled("PLAYER", player, killer);
	}
	
	/// PLAYER DELETE (corpse or alive logout)
	void LogPlayerDelete(PlayerBase player, EntityAI parent)
	{
		if (!IsEnabled()) return;
		string message = string.Format("");
		if (IsJson()) {
			PlayerLog(player, "PLAYER_DEATH", "{\"tag\":\"#corpse_destoryed\"}");
		} else {
			PlayerLog(player, "PLAYER_DEATH", "#corpse_destoryed");
		}
	}

	void PlayerDeathDetails(PlayerBase player, string details)
	{
		string msg = string.Format("details=<%1>");
		if (IsJson()) msg = string.Format("{\"details\":\"%1\"}", details);
		PlayerLog(player, "LOG_PLAYER_DEATH_DETAILS", msg);
	}

	/////////////////////////		ANIMALS
	void LogAnimalHit(AnimalBase target, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef, bool sameAlive = true)
	{
		if (!IsEnabled()) return;
		LogHit("ANIMAL", target, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, sameAlive);
	}
	
	void LogAnimalKilled(AnimalBase target, Object killer)
	{
		if (!IsEnabled()) return;
		LogKilled("ANIMAL", target, killer);
	}
	
	/////////////////////////		INFECTED
	void LogInfectedHit(ZombieBase target, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef, bool sameAlive = true)
	{
		if (!IsEnabled()) return;
		LogHit("INFECTED", target, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, sameAlive);
	}
	
	void LogInfectedKilled(ZombieBase target, Object killer)
	{
		if (!IsEnabled()) return;
		LogKilled("INFECTED", target, killer);
	}

	/////////////////////////		ACTIONS
	void LogAction(ActionBase action, string logGroup, ActionData actionData)
	{
		if (!IsEnabled()) return;
		PlayerLog(actionData.m_Player, "ACTION_" + logGroup + "LOG", action.GetAdminLogMessage(actionData));
	}

	/////////////////////////		ITEMS

	///	hit/kill
	void LogItemHit(ItemBase target, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!IsEnabled()) return;
		LogHit("ITEM", target, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	void LogItemKilled(ItemBase target, Object killer)
	{
		if (!IsEnabled()) return;
		LogKilled("ITEM", target, killer);
	}

	/// placed
	void LogItemPlaced(ItemBase target, Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if (!IsEnabled() || !IsOverride()) return;
		string itemInfo = _helper.ItemInfo(target);
		string message = string.Format("item=<%1> position=<%2> orientation=<%3>", itemInfo, _helper.Vector3Info(position), _helper.Vector3Info(orientation) );
		if (IsJson()) message = string.Format("{\"item\":%1,\"position\":%2,\"orientation\":%3}", itemInfo, _helper.Vector3Info(position), _helper.Vector3Info(orientation) );
		PlayerLog(PlayerBase.Cast( player ), "ITEM_PLACED", message);
	}
	
	/// location
	void LogItemLocation(ItemBase item, notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		if (!IsEnabled()) return;
		
		if (!item) return;
		string itemInfo = _helper.ItemInfo(item, false, false, false, true);
		
		PlayerBase newPlayer = null;
		PlayerBase oldPlayer = null;
		EntityAI oldParent = null;
		EntityAI newParent = null;

		if (oldLoc) oldParent = oldLoc.GetParent(); //.GetHierarchyParent();
		if (newLoc) newParent = newLoc.GetParent(); //.GetHierarchyParent();
		
		string oldParentInfo = _helper.EntityInfo(oldParent, true, true);
		string newParentInfo = _helper.EntityInfo(newParent, true, true);
		
		if ( newParent ) newPlayer = PlayerBase.Cast(newParent.GetHierarchyRootPlayer());
		if ( oldParent ) oldPlayer = PlayerBase.Cast(oldParent.GetHierarchyRootPlayer());
		
		autoptr map<InventoryLocationType, string> locations = new map<InventoryLocationType, string>;
		locations[InventoryLocationType.UNKNOWN] = "#world";
		locations[InventoryLocationType.GROUND] = "#ground";
		locations[InventoryLocationType.ATTACHMENT] = "#attachment";
		locations[InventoryLocationType.CARGO] = "#cargo";
		locations[InventoryLocationType.HANDS] = "#hands";
		locations[InventoryLocationType.PROXYCARGO] = "#proxy";
		
		bool passNoWorld = locations[oldLoc.GetType()] != "#world" && locations[newLoc.GetType()] != "#world";
		
		if (!oldPlayer && !newPlayer) {
			// care if it's not player related - may be cargo on ground near player
			if (IsJson()) {
				if (newParent && oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("{\"tag\":\"#located\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"from\":%2,\"to\":%3}", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (newParent && !oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("{\"tag\":\"#located\",\"tagFrom\":\"%3\",\"tagTo\":\"%4\",\"item\":%1,\"to\":%2}", itemInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (!newParent && oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("{\"tag\":\"#located\",\"tagFrom\":\"%3\",\"tagTo\":\"%4\",\"item\":%1,\"from\":%2}", itemInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (!newParent && !oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("{\"tag\":\"#located\",\"tagFrom\":\"%2\",\"tagTo\":\"%3\",\"item\":%1}", itemInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
			} else {
				if (newParent && oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("#located item=<%1> tagFrom=<%4> tagTo=<%5> from=<%2> to=<%3>", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (newParent && !oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("#located item=<%1> tagFrom=<%3> tagTo=<%4> to=<%2>", itemInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (!newParent && oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("#located item=<%1> tagFrom=<%3> tagTo=<%4> from=<%2>", itemInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
				if (!newParent && !oldParent) NoPlayerLog("ITEM_LOCATION", string.Format("#located item=<%1> tagFrom=<%2> tagTo=<%3>", itemInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]), passNoWorld);
			}
			return;
		}
		if (oldPlayer && !newPlayer)
		{
			if (IsJson()) {
				if (newParent) PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#out\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"from\":%2,\"to\":%3}", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
				if (!newParent) PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#out\",\"tagFrom\":\"%3\",\"tagTo\":\"%4\",\"item\":%1,\"from\":%2}", itemInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			} else {
				if (newParent) PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("#out item=<%1> tagFrom=<%4> tagTo=<%5> from=<%2> to=<%3>", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
				if (!newParent) PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("#out item=<%1> tagFrom=<%3> tagTo=<%4> from=<%2>", itemInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			}
			return;
		}
		if (!oldPlayer && newPlayer)
		{
			if (IsJson()) {
				if (oldParent) PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#in\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"to\":%2,\"from\":%3}", itemInfo, newParentInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
				if (!oldParent) PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#in\",\"tagFrom\":\"%3\",\"tagTo\":\"%4\",\"item\":%1,\"to\":%2}", itemInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			} else {
				if (oldParent) PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("#in item=<%1> tagFrom=<%4> tagTo=<%5> to=<%2> from=<%3>", itemInfo, newParentInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
				if (!oldParent) PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("#in item=<%1> tagFrom=<%3> tagTo=<%4> to=<%2>", itemInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			}
			return;
		}
		if (oldPlayer == newPlayer)
		{
			if (IsJson()) {
				PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#move\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"from\":%2,\"to\":%3}", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			} else {
				PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("#move item=<%1> tagFrom=<%4> tagTo=<%5> from=<%2> to=<%3>", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			}
			return;
		}
		if (IsJson()) {
			PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("{\"tag\":\"#out\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"from\":%2,\"to\":%3}", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("{\"move\":\"#in\",\"tagFrom\":\"%4\",\"tagTo\":\"%5\",\"item\":%1,\"to\":%2,\"from\":%3}", itemInfo, newParentInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
		} else {
			PlayerLog(oldPlayer, "ITEM_LOCATION", string.Format("#out item=<%1> tagFrom=<%4> tagTo=<%5> from=<%2> to=<%3>", itemInfo, oldParentInfo, newParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
			PlayerLog(newPlayer, "ITEM_LOCATION", string.Format("#in item=<%1> tagFrom=<%4> tagTo=<%5> to=<%2> from=<%3>", itemInfo, newParentInfo, oldParentInfo, locations[oldLoc.GetType()], locations[newLoc.GetType()]));
		}
	}

	///	attached
	void LogItemAttached(ItemBase target, EntityAI parent, int slot_id )
	{
		if (!IsEnabled()) return;
		
		string itemInfo = _helper.ItemInfo(target, false, true, false, true);
		string parentInfo = _helper.EntityInfo(parent);
		PlayerBase player = PlayerBase.Cast(parent.GetHierarchyRootPlayer());
		if (player) {
			if (IsJson()) {
				PlayerLog(player, "ITEM_ATTACH", string.Format("{\"item\":%1,\"to\":%2,\"slotId\":%3}", itemInfo, parentInfo, slot_id));
			} else {
				PlayerLog(player, "ITEM_ATTACH", string.Format("item=<%1> to=<%2> slotId=<%3>", itemInfo, parentInfo, slot_id));
			}
		} else {
			// care if it's not player related - may be cargo on ground near player
			if (IsJson()) {
				NoPlayerLog("ITEM_ATTACH", string.Format("{\"item\":%1,\"to\":%2,\"slotId\":%3}", itemInfo, parentInfo, slot_id));
			} else {
				NoPlayerLog("ITEM_ATTACH", string.Format("item=<%1> to=<%2> slotId=<%3>", itemInfo, parentInfo, slot_id));
			}
		}
		
	}

	///	detached
	void LogItemDetached(ItemBase target, EntityAI parent, int slot_id )
	{
		if (!IsEnabled()) return;
		
		string itemInfo = _helper.ItemInfo(target, false, false, false, true);
		string parentInfo = _helper.EntityInfo(parent);
		PlayerBase player = PlayerBase.Cast(parent.GetHierarchyRootPlayer());
		if (player) {
			if (IsJson()) {
				if (parent) { PlayerLog(player, "ITEM_DETACH", string.Format("{\"item\":%1,\"from\":%2,\"slotId\":%3}", itemInfo, parentInfo, slot_id)); }
				else { PlayerLog(player, "ITEM_DETACH", string.Format("{\"item\":%1}", itemInfo)); }
			} else {
				PlayerLog(player, "ITEM_DETACH", string.Format("item=<%1> from=<%2> slotId=<%3>", itemInfo, parentInfo, slot_id));
			}
		} else {
			// care if it's not player related - may be cargo on ground near player
			if (IsJson()) {
				NoPlayerLog("ITEM_DETACH", string.Format("{\"item\":%1,\"from\":%2,\"slotId\":%3}", itemInfo, parentInfo, slot_id));
			} else {
				NoPlayerLog("ITEM_DETACH", string.Format("item=<%1> from=<%2> slotId=<%3>", itemInfo, parentInfo, slot_id));
			}
		}
	}

	///	quantity
	void LogItemQuantity(ItemBase target, float value, bool wasDestroyed)
	{
		if (!IsEnabled()) return;
		
		PlayerBase player = PlayerBase.Cast(target.GetHierarchyRootPlayer());
		string itemInfo = _helper.ItemInfo(target, true, true, false, true);
		if (IsJson()) {
			PlayerLog(player, "ITEM_QUANTITY", string.Format("{\"tag\":\"#set\",\"item\":%1,\"quantity\":%2}", itemInfo, target.GetQuantity()));
		} else {
			PlayerLog(player, "ITEM_QUANTITY", string.Format("#set item=<%1> quantity=<%2>", itemInfo, target.GetQuantity()));
		}
		if (wasDestroyed) {
			if (IsJson()) {
				PlayerLog(player, "ITEM_DESTROY", string.Format("{\"tag\":\"#quantity\",\"item\":%1}", itemInfo));
			} else {
				PlayerLog(player, "ITEM_DESTROY", string.Format("#quantity item=<%1>", itemInfo));
			}
		}
	}

	/////////////////////////		BaseBuilding logs
	
	void LogBaseBuildPart(BaseBuildingBase base,  notnull Man player, string part_name, int action_id )
	{
		if (!IsEnabled()) return;
		string message = string.Format("#build id=<%1> type=<%2> part=<%3>", base.GetID(), base.GetType(), part_name);
		if (IsJson()) message = string.Format("{ \"tag\":\"#build\",\"id\":\"%1\",\"type\":\"%2\",\"part\":\"%3\"}", base.GetID(), base.GetType(), part_name);
		PlayerLog(PlayerBase.Cast(player), "BASEBUILDING", message);
	}
	
	void LogBaseDismantlePart(BaseBuildingBase base,  notnull Man player, string part_name, int action_id )
	{
		if (!IsEnabled()) return;
		string message = string.Format("#dismantle id=<%1> type=<%2> part=<%3>", base.GetID(), base.GetType(), part_name);
		if (IsJson()) message = string.Format("{ \"tag\":\"#dismantle\",\"id\":\"%1\",\"type\":\"%2\",\"part\":\"%3\"}", base.GetID(), base.GetType(), part_name);
		PlayerLog(PlayerBase.Cast(player), "BASEBUILDING", message);
	}
	
	void LogBaseDestroyPart( BaseBuildingBase base, Man player, string part_name, int action_id, bool destroyed_by_connected_part )
	{
		if (!IsEnabled()) return;
		string message = string.Format("#destroy id=<%1> type=<%2> part=<%3>", base.GetID(), base.GetType(), part_name);
		if (IsJson()) message = string.Format("{ \"tag\":\"#destroy\",\"id\":\"%1\",\"type\":\"%2\",\"part\":\"%3\"}", base.GetID(), base.GetType(), part_name);
		PlayerLog(PlayerBase.Cast(player), "BASEBUILDING", message);
	}

	
	/////////////////////////		hit/kill/destroy  globals
	
	/// hit log
	void LogHit(string logGroup, EntityAI target, TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef, bool sameAlive = true)
	{
		string targetInfo = _helper.EntityInfo(target, true, true, true);
		string sourceInfo = "null";
		PlayerBase sourcePlayer = null;
		string sourcePlayerInfo = "null";
		float distance = 0;
		
		if (source) {
			distance = vector.Distance( target.GetPosition(), source.GetPosition() );
			if (distance < 0.01) distance = 0;
			sourceInfo = _helper.EntityInfo(source);
			if (source.GetHierarchyRootPlayer()) {
				sourcePlayer = PlayerBase.Cast(source.GetHierarchyRootPlayer());
				sourcePlayerInfo = _helper.PlayerInfo(sourcePlayer, true);
			}
			
			if (damageType == DT_CLOSE_COMBAT) {
				if (source.IsPlayer()) {
					sourcePlayer = PlayerBase.Cast(source);
					sourcePlayerInfo = _helper.PlayerInfo(sourcePlayer, true);
					sourceInfo = "melee";
					if (IsJson()) sourceInfo = "\"melee\"";
				}
			}
			if (damageType == DT_CUSTOM) {
				if (source.GetType() == "AreaDamageManager")
				{
					EntityAI sourceParent = EntityAI.Cast( source.GetParent() ); // need GetParent of source or no?
					if ( sourceParent )
					{
						sourceInfo = _helper.EntityInfo(sourceParent);
					}
				}
				if (ammo == "FallDamage") {
					sourceInfo = "#fall";
					sourcePlayerInfo = "null";
					if (IsJson()) sourceInfo = "\"#fall\"";
				}
			}
		}

		float dmg2Zone = 0;
		float dmg = 0;
		float zoneHealth = 0;
		bool showZoneHealth = false;
		if ( damageResult) {
			dmg2Zone = damageResult.GetDamage(dmgZone, "");
			dmg = damageResult.GetHighestDamage("Health");
			//ItemBase ibTarget = ItemBase.Cast(target);
			//if (ibTarget) {
				showZoneHealth = true;
				zoneHealth = (target.GetHealth(dmgZone, "") * 100) / target.GetMaxHealth(dmgZone, "");
			//}
		}
		
		string code = logGroup + "_DAMAGE";
		if (!target.IsAlive()) {
			// if (sameAlive) { code = logGroup + "_CORPSE_DAMAGE"; }
			// else  { code = logGroup + "_LETHAL_DAMAGE"; }
			code = logGroup + "_LETHAL_DAMAGE";
		}
		string message = "";
		if (showZoneHealth) {
			message = string.Format("source=<%1> target=<%2> ammo=<%3> zone=<%4> dmgZone=<%5> zoneHp100=<%9> dmgHealth=<%6> hitter=<%7> distance=<%8>", sourceInfo, targetInfo, ammo, dmgZone, dmg2Zone, dmg, sourcePlayerInfo, distance, zoneHealth);
			if (IsJson()) message = string.Format("{\"source\":%1,\"target\":%2,\"ammo\":\"%3\",\"zone\":\"%4\",\"dmgZone\":%5,\"zoneHp100\":%9,\"dmgHealth\":%6,\"hitter\":%7,\"distance\":%8}", sourceInfo, targetInfo, ammo, dmgZone, dmg2Zone, dmg, sourcePlayerInfo, distance, zoneHealth);
		} else {
			message = string.Format("source=<%1> target=<%2> ammo=<%3> zone=<%4> dmgZone=<%5> dmgHealth=<%6> hitter=<%7> distance=<%8>", sourceInfo, targetInfo, ammo, dmgZone, dmg2Zone, dmg, sourcePlayerInfo, distance);
			if (IsJson()) message = string.Format("{\"source\":%1,\"target\":%2,\"ammo\":\"%3\",\"zone\":\"%4\",\"dmgZone\":%5,\"dmgHealth\":%6,\"hitter\":%7,\"distance\":%8}", sourceInfo, targetInfo, ammo, dmgZone, dmg2Zone, dmg, sourcePlayerInfo, distance);
		}
		if (target.IsPlayer()) {
			PlayerLog(PlayerBase.Cast(target), code, message, true);
			return;
		}
		if (sourcePlayer) {
			PlayerLog(sourcePlayer, code, message, true);
			return;
		}
		BaseBuildingBase bbItem = BaseBuildingBase.Cast(target);
		NoPlayerLog(code, message, !!bbItem);
	}
	
	/// kill log
	void LogKilled(string logGroup, EntityAI target, Object killer)
	{
		if (!IsEnabled() || !IsOverride()) return;
		
		string targetInfo = _helper.EntityInfo(target);
		string killerInfo = "null";
		EntityAI eKiller = EntityAI.Cast(killer);
		PlayerBase realKiller = NULL;
		
		if (killer) {
			string sourceInfo = _helper.ObjectInfo(killer);
			float distance = vector.Distance( target.GetPosition(), killer.GetPosition() );
			if (distance < 0.01) distance = 0; // fix veeeery small values like 1.0-eXX
			if (killer == target) {
				sourceInfo = "self";
				killerInfo = "self";
				if (IsJson()) {
					sourceInfo = "\"self\"";
					killerInfo = "\"self\"";
				}
			} else {
				if (eKiller) {
					if (eKiller.IsZombie() || eKiller.IsAnimal()) {
						killerInfo = _helper.EntityInfo(eKiller);
					}
					if (eKiller.IsPlayer()) {
						killerInfo = _helper.PlayerInfo(PlayerBase.Cast(eKiller), true);
						sourceInfo = "melee";
						if (IsJson()) sourceInfo = "\"melee\"";
					}
					if (eKiller.IsMeleeWeapon() || eKiller.IsWeapon()) {
						sourceInfo = _helper.EntityInfo(eKiller);
						killerInfo = _helper.PlayerInfo(PlayerBase.Cast(eKiller.GetHierarchyRootPlayer()), true);
						realKiller = PlayerBase.Cast(eKiller.GetHierarchyRootPlayer());
					}
					if (eKiller.GetType() == "AreaDamageManager")
					{
						killerInfo = _helper.EntityInfo(eKiller);
						EntityAI sourceParent = EntityAI.Cast( eKiller.GetParent() );
						if ( sourceParent )
						{
							sourceInfo = _helper.EntityInfo(sourceParent);
						}
					}
				}
			}
		}
		string message = string.Format("#death target=<%1> source=<%2> killer=<%3> distance=<%4>", targetInfo, sourceInfo, killerInfo, distance);
		if (IsJson()) message = string.Format("{\"tag\":\"#death\",\"target\":%1,\"source\":%2,\"killer\":%3,\"distance\":%4}", targetInfo, sourceInfo, killerInfo, distance);
		if (target.IsPlayer()) {
			PlayerLog(PlayerBase.Cast(target), logGroup + "_DEATH", message);
			return;
		}
		if (eKiller.IsPlayer()) {
			PlayerLog(PlayerBase.Cast(eKiller), logGroup + "_DEATH", message);
			return;
		}
		if (realKiller) {
			PlayerLog(realKiller, logGroup + "_DEATH", message);
			return;
		}
		NoPlayerLog(logGroup + "_DEATH", message, true);
	}

	/////////////////////////    other

	void OnlineList()
	{
		if (!IsEnabled()) return;
		
		ref array<Man> players = new array<Man>();
		GetGame().GetPlayers( players );
		
		if ( players.Count() != 0 )
		{	
			foreach ( int index, Man player: players )
			{
				string playerInfo = _helper.PlayerInfo(PlayerBase.Cast(player), true);
				if (IsJson()) {
					NoPlayerLog("ONLINE", string.Format("{\"online\":%1,\"index\":%2,\"player\":%3}", players.Count(), index, playerInfo));
				} else {
					NoPlayerLog("ONLINE", string.Format("online=<%1> index=<%2> player=<%3>", players.Count(), index, playerInfo));
				}
			}
		} else {
			if (IsJson()) {
				NoPlayerLog("ONLINE", "{\"online\":0}");
			} else {
				NoPlayerLog("ONLINE", "online=<0>");
			}
		}
		delete players;
	}

	////////////////////////
	
	void PlayerLog(PlayerBase player,  string source, string message, bool withStatsOrPass = false) 
	{
		if (!FilterPass(source)) return;
		if (!player) {
			NoPlayerLog(source, message, withStatsOrPass);
			return;
		}
		string playerInfo = _helper.PlayerInfo( player, withStatsOrPass );
		string msg = string.Format("[%1] - [%3] - [%2]", source, playerInfo, message);
		if (IsJson()) msg = string.Format("JSON{\"event\":\"%1\",\"data\":%3,\"player\":%2}", source, playerInfo, message);
		DirectLog(msg);
	}

	void NoPlayerLog(string source, string message, bool passNoWorld = false)
	{
		if (source == "ONLINE" || source == "BASEBUILDING") {
			if (!FilterPass(source)) return;
		} else {
			if (IsNoWorld() && !passNoWorld) return;
			if (!FilterPass(source)) return;
		}
		string msg = string.Format("[%1] - [%2]", source, message);
		if (IsJson()) msg = string.Format("JSON{\"event\":\"%1\",\"data\":%2}", source, message);
		DirectLog(msg);
	}
	
	void DirectLog(string message)
	{
		GetGame().AdminLog(message);
	}


}