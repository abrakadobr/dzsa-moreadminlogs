// animals
modded class AnimalBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool _isAlive = IsAlive();
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogAnimalHit(this, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, _isAlive == IsAlive());
	}
	
	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogAnimalKilled(this, killer);
	}

}

// infected
modded class ZombieBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool _isAlive = IsAlive();
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogInfectedHit(this, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, _isAlive == IsAlive());
	}
	
	override void EEKilled( Object killer )
	{
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogInfectedKilled(this, killer);
		super.EEKilled( killer );
	}
	
}

// players
modded class PlayerBase
{
	MoreAdminLogs h_madHelper;
	float h_lastSpeed;
	string h_lastMove;
	
	override void Init()
	{
		super.Init();
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_lastSpeed = 0;
		h_lastMove = "idle";
	}
	
	// management events
	override void OnSelectPlayer()
	{
		super.OnSelectPlayer();
		h_madHelper.LogPlayerManagement(this, "#selected");
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		bool ret = super.OnStoreLoad(ctx, version);
		h_madHelper.LogPlayerManagement(this, "#load");
		return ret;
	}
	
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		h_madHelper.LogPlayerManagement(this, "#loaded");
	}
	
	override void OnConnect()
	{
		super.OnConnect();
		h_madHelper.LogPlayerManagement(this, "#connect");
		h_madHelper.OnlineList();
	}
	
	override void OnReconnect()
	{
		super.OnReconnect();
		h_madHelper.LogPlayerManagement(this, "#reconnect");
	}
	
	override void OnDisconnect()
	{
		super.OnDisconnect();
		h_madHelper.LogPlayerManagement(this, "#disconnect");
	}
	
	// hands events
	override void EEItemOutOfHands(EntityAI item)
	{
		h_madHelper.LogPlayerHands(this, "#out", item);
		super.EEItemOutOfHands( item );
	}
	
	override void EEItemIntoHands(EntityAI item)
	{
		super.EEItemOutOfHands( item );
		h_madHelper.LogPlayerHands(this, "#in", item);
	}
	
	// movement/vehicle events
	override void OnCommandHandlerTick(float delta_time, int pCurrentCommandID)
	{
		super.OnCommandHandlerTick(delta_time, pCurrentCommandID);
		if (!h_madHelper.IsEnabled()) return;
		HumanCommandMove mc = GetCommand_Move();
		if (!mc) return;
		float speed = mc.GetCurrentMovementSpeed();
		string nextMove = h_lastMove;
		if (speed != h_lastSpeed) {
			if (!IsInVehicle()) {
				if (speed == 0) nextMove = "idle";
				if (speed > 0 && speed < 2) nextMove = "walk";
				if (speed >= 2 && speed < 2.5) nextMove = "run";
				if (speed > 2.5 && speed < 3.5) nextMove = "sprint";
				if (speed >= 3.5 ) nextMove = "fly";
				if (nextMove != h_lastMove) {
					float logSpeed = 0;
					if (nextMove == "fly") logSpeed = speed;
					h_madHelper.LogPlayerMovement(this, "#" + nextMove, logSpeed );
					h_lastMove = nextMove;
				}
			} else {
				if (speed == 0) {
					h_madHelper.LogPlayerVehicle(this, "#stop");
				} else {
					h_madHelper.LogPlayerVehicle(this, "#drive");
				}
			}
			h_lastSpeed = speed;
		}
	}
	
	override void OnCommandFallStart()
	{
		super.OnCommandFallStart();
		h_madHelper.LogPlayerMovement(this, "#fall_start" );
	}
	
	override void OnCommandFallFinish()
	{
		super.OnCommandFallFinish();
		h_madHelper.LogPlayerMovement(this, "#fall_finish" );
	}
	
	override void OnCommandLadderStart()
	{
		super.OnCommandLadderStart();
		h_madHelper.LogPlayerMovement(this, "#ladder_start" );
	}
	
	override void OnCommandLadderFinish()
	{
		super.OnCommandLadderFinish();
		h_madHelper.LogPlayerMovement(this, "#ladder_finish" );
	}
	
	override void OnVehicleSeatDriverEnter()
	{
		super.OnVehicleSeatDriverEnter();
		h_madHelper.LogPlayerMovement(this, "#driver_enter" );
		h_madHelper.LogPlayerVehicle(this, "#driver_enter" );
	}
	
	override void OnVehicleSeatDriverLeft()
	{
		super.OnVehicleSeatDriverLeft();
		h_madHelper.LogPlayerMovement(this, "#driver_left" );
		h_madHelper.LogPlayerVehicle(this, "#driver_left" );
	}
	
	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();
		h_madHelper.LogPlayerVehicle(this, "#enter" );
	}
	
	override void OnCommandVehicleFinish()
	{
		super.OnCommandVehicleFinish();
		h_madHelper.LogPlayerVehicle(this, "#leave" );
	}		

	override void OnJumpStart()
	{
		super.OnJumpStart();
		h_madHelper.LogPlayerMovement(this, "#jump" );
	}
	
	override void OnStanceChange(int previousStance, int newStance)
	{
		super.OnStanceChange(previousStance, newStance);
		if (!h_madHelper.IsEnabled()) return;
		
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_PRONE) ) h_madHelper.LogPlayerMovement(this, "#stance_prone" );
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDPRONE) ) h_madHelper.LogPlayerMovement(this, "#stance_prone_rised" );
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_CROUCH) ) h_madHelper.LogPlayerMovement(this, "#stance_crouch" );
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDCROUCH) ) h_madHelper.LogPlayerMovement(this, "#stance_crouch_rised" );
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_ERECT) ) h_madHelper.LogPlayerMovement(this, "#stance_erect" );
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDERECT) ) h_madHelper.LogPlayerMovement(this, "#stance_erect_rised" );
	}
	
	override void OnRollStart(bool isToTheRight)
	{
		super.OnRollStart(isToTheRight);
		if (!h_madHelper.IsEnabled()) return;
		string code = "#roll";
		if (isToTheRight) {
			code = "#roll_right";
		} else {
			code = "#roll_left";
		}
		h_madHelper.LogPlayerMovement(this, code );
	}

	override void OnCommandSwimStart()
	{
		super.OnCommandSwimStart();
		h_madHelper.LogPlayerMovement(this, "#swim_start" );
	}
	
	override void OnCommandSwimFinish()
	{
		super.OnCommandSwimFinish();
		h_madHelper.LogPlayerMovement(this, "#swim_finish" );
	}
	
	override void OnCommandClimbStart()
	{
		super.OnCommandClimbStart();
		h_madHelper.LogPlayerMovement(this, "#climb" );
	}
	
	override void OnJumpOutVehicleFinish(float carSpeed)
	{ 
		super.OnJumpOutVehicleFinish(carSpeed);
		h_madHelper.LogPlayerMovement(this, "#jump_out_car", carSpeed );
	}
	
	// health events
	override void OnUnconsciousStart()
	{
		super.OnUnconsciousStart();
		h_madHelper.LogPlayerHealth(this, "#unconscious_start" );
	}
	
	override void OnUnconsciousStop(int pCurrentCommandID)
	{	
		super.OnUnconsciousStop(pCurrentCommandID);
		h_madHelper.LogPlayerHealth(this, "#unconscious_stop" );
	}
	
	//Server side
	override void SetBrokenLegs(int stateId)
	{
		super.SetBrokenLegs(stateId);
		eBrokenLegs state = GetBrokenLegs();
		
		string code = "#legs";
		if ( state == eBrokenLegs.NO_BROKEN_LEGS )
		{
			code = "#legs_fine";
		}
		else if ( state == eBrokenLegs.BROKEN_LEGS )
		{
			code = "#legs_broken";
		}
		else if (state == eBrokenLegs.BROKEN_LEGS_SPLINT)
		{
			code = "#legs_splint";
		}
		h_madHelper.LogPlayerHealth(this, code );
	}	
	
	override void SetRestrained(bool is_restrained)
	{
		super.SetRestrained(is_restrained);
		if (is_restrained) {
			h_madHelper.LogPlayerHealth(this, "#restrained" );
		} else {
			h_madHelper.LogPlayerHealth(this, "#free" );
		}
	}
	
	override void SetBleedingBits(int bits)
	{
		super.SetBleedingBits(bits);
		h_madHelper.LogPlayerHealthBleeds(this, bits);
	}

	override bool Consume(ItemBase source, float amount, EConsumeType consume_type )
	{
		bool ok = super.Consume(source, amount, consume_type );
		if (ok) h_madHelper.LogPlayerHealthConsume(this, source, amount);
		return ok;
	}

	/// PLAYER_GEAR
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		h_madHelper.LogPlayerGear(this, "#wear", item, slot_name);
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		h_madHelper.LogPlayerGear(this, "#take_off", item, slot_name);
	}

	/*
	override void QuickReloadWeapon( EntityAI weapon )
	{
		super.QuickReloadWeapon( weapon );
		if (!h_madHelper.IsEnabled()) return;
		string weaponInfo = h_madHelper.EntityInfo(weapon);
		if (h_madHelper.IsJson()) {
			h_madHelper.PlayerLog(this, "PLAYER_WEAPON", string.Format("{ \"tag\": \"#quick_reload\", \"weapon\": <%1> }", weaponInfo));
		} else {
			h_madHelper.PlayerLog(this, "PLAYER_WEAPON", string.Format("#quick_reload weapon=<%1>", weaponInfo));
		}
	}
	
	//Reload weapon with given magazine
	override void ReloadWeapon( EntityAI weapon, EntityAI magazine )
	{
		super.ReloadWeapon( weapon, magazine );
		if (!h_madHelper.IsEnabled()) return;
		string weaponInfo = h_madHelper.EntityInfo(weapon);
		string magInfo = h_madHelper.EntityInfo(magazine);
		if (h_madHelper.IsJson()) {
			h_madHelper.PlayerLog(this, "PLAYER_WEAPON", string.Format("{ \"tag\": \"#reload\", \"weapon\": <%1>, \"magazine\": <%2> }", weaponInfo, magInfo));
		} else {
			h_madHelper.PlayerLog(this, "PLAYER_WEAPON", string.Format("#reload weapon=<%1> with=<%2>", weaponInfo, magInfo));
		}
	}
	*/
	
	//////////////////     items
	/*
	override bool DropItem(ItemBase item) {
		bool ok = super.DropItem(item);
		if (ok) h_madHelper.LogPlayerInventory(this, "#drop", item, EntityAI.Cast(item.GetParent()), null);
		return ok;
	}
	
	override bool PredictiveTakeEntityToTargetInventory (notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
	{
		bool ok = super.PredictiveTakeEntityToTargetInventory(target, flags, item);
		if (ok) h_madHelper.LogPlayerInventory(this, "#take_to_target", item, null, target);
		return ok;
	}
	*/

	/*
	override bool PredictiveTakeEntityToInventory (FindInventoryLocationType flags, notnull EntityAI item)
	{
		bool ok = super.PredictiveTakeEntityToInventory(flags, item);
		if (!h_madHelper.IsEnabled()) return ok;
		if (ok) {
			string message = string.Format("#take_to_inventory item=<%1>",  h_madHelper.EntityInfo(item));
			if (h_madHelper.IsJson()) message = string.Format("{ \"tag\": \"#take_to_inventory\", \"item\": %1 }",  h_madHelper.EntityInfo(item));
			h_madHelper.PlayerLog(this, "PLAYER_INVENTORY", message);
		}
		return ok;
	}
	
	override bool PredictiveDropEntity (notnull EntityAI item)
	{
		bool ok = super.PredictiveDropEntity(item);
		if (!h_madHelper.IsEnabled()) return ok;
		if (ok) {
			string message = string.Format("#drop_entity item=<%1>",  h_madHelper.EntityInfo(item));
			if (h_madHelper.IsJson()) message = string.Format("{ \"tag\": \"#drop_entity\", \"item\": %1 }",  h_madHelper.EntityInfo(item));
			h_madHelper.PlayerLog(this, "PLAYER_INVENTORY", message);
		}
		return ok;

	}
	*/
	
	/*
	override bool PredictiveTakeEntityToTargetAttachment (notnull EntityAI target, notnull EntityAI item)
	{
		Weapon_Base parentWpn = Weapon_Base.Cast(target);
		Magazine mag = Magazine.Cast(item);
		if (parentWpn && mag)
		{
			if( GetWeaponManager().CanAttachMagazine(parentWpn, mag) )
				return GetWeaponManager().AttachMagazine(mag);

			return false;
		}
		return super.PredictiveTakeEntityToTargetAttachment(target, item);
	}
	
	override bool PredictiveTakeEntityToTargetAttachmentEx (notnull EntityAI target, notnull EntityAI item, int slot)
	{
		Weapon_Base parentWpn = Weapon_Base.Cast(target);
		Magazine mag = Magazine.Cast(item);
		if (parentWpn && mag)
		{
			if( target.CanReceiveAttachment(item,slot) && GetWeaponManager().CanAttachMagazine(parentWpn, mag) )
				return GetWeaponManager().AttachMagazine(mag);

			return false;
		}
		return super.PredictiveTakeEntityToTargetAttachmentEx(target, item,slot);
	}
	
	override bool PredictiveTakeEntityToTargetCargo (notnull EntityAI target, notnull EntityAI item)
	{
		bool can_detach;
		if (CanRedirectToWeaponManager(item,can_detach))
		{
			InventoryLocation il = new InventoryLocation();
			if (can_detach && target.GetInventory().FindFreeLocationFor(item, FindInventoryLocationType.CARGO, il))
			{
				return GetWeaponManager().DetachMagazine(il);	
			}
			return false;	
		}
		return super.PredictiveTakeEntityToTargetCargo(target,item);
	}
	
	override bool PredictiveTakeEntityToTargetCargoEx (notnull CargoBase cargo, notnull EntityAI item, int row, int col)
	{
		bool can_detach;
		if (CanRedirectToWeaponManager(item, can_detach))
		{
			if (can_detach)
			{
				InventoryLocation dst = new InventoryLocation;
				dst.SetCargoAuto(cargo, item, row, col, item.GetInventory().GetFlipCargo());

				return GetWeaponManager().DetachMagazine(dst);
			}
			return false;
		}		
		return super.PredictiveTakeEntityToTargetCargoEx (cargo, item, row, col);
	}
	*/
	

	
	
	/*
	override bool PredictiveTakeToDst (notnull InventoryLocation src, notnull InventoryLocation dst)
	{
		EntityAI item = src.GetItem();
		bool ok = super.PredictiveTakeToDst (src, dst);
		if (ok) h_madHelper.PlayerPredictiveTakeToDst(player, item, src, dst);
		return ok;
	}
	*/
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		bool _isAlive = IsAlive();
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		h_madHelper.LogPlayerHit(this, damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef, _isAlive == IsAlive());
	}

	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );
		h_madHelper.LogPlayerKilled(this, killer);
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		h_madHelper.LogPlayerDelete(this, parent);
	}
	
}