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

/*
modded class DayZPlayer {
	override bool HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		bool res = super.HeadingModel(pDt, pModel);
		string p = string.Format("%1/%2", pModel.m_fOrientationAngle, pModel.m_fHeadingAngle);
		Print(p);
		return res;
	}
}
*/

// players
modded class PlayerBase
{
	MoreAdminLogs h_madHelper;
	float h_lastSpeed;
	float h_lastMovementUpdateTime;
	string h_lastMove;
	string h_lastStance;
	
	vector h_lastHead;
	vector h_lastAim;
	
	override void Init()
	{
		super.Init();
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_lastSpeed = 0;
		h_lastMovementUpdateTime = 0;
		h_lastMove = "idle";
		h_lastStance = "stance_erect";
		h_lastHead = "0 0 0";
		h_lastAim = "0 0 0";
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
	
	// heading/aiming
	/*
	override bool	HeadingModel(float pDt, SDayZPlayerHeadingModel pModel)
	{
		bool result = super.HeadingModel(pDt, pModel);
		// if (result) {
			h_lastHead[0] = pModel.m_fOrientationAngle;
			h_lastHead[1] = pModel.m_fHeadingAngle;
		// }
		return result;
	}	

	override bool	AimingModel(float pDt, SDayZPlayerAimingModel pModel)
	{
		bool result = super.AimingModel(pDt, pModel);
		// if (result) {
			h_lastAim[0] = pModel.m_fCurrentAimX;
			h_lastAim[1] = pModel.m_fCurrentAimY;
		// }
		return result;
	}
	*/
	
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
	
	override void EOnPostFrame(IEntity other, int extra)
	{
		super.EOnFrame(other, extra);
		if (!h_madHelper.LogPositions()) return;
			
		float delta_time = (GetGame().GetTime() - h_lastMovementUpdateTime) / 1000;
		if (h_madHelper.PlayerPositionTime() < delta_time) {
			h_madHelper.LogPlayerPosition(this);
			h_lastMovementUpdateTime = GetGame().GetTime();
		}
	}
	
	void ResetPositionLogTimer()
	{
		h_lastMovementUpdateTime = GetGame().GetTime();
	}
	
	// movement/vehicle events
	override void OnCommandHandlerTick(float delta_time, int pCurrentCommandID)
	{
		super.OnCommandHandlerTick(delta_time, pCurrentCommandID);
		if (!h_madHelper.IsEnabled()) return;
		
		HumanCommandMove mc = GetCommand_Move();
		if (mc) {
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
						h_lastMovementUpdateTime = GetGame().GetTime();
					}
				} else {
					/*
					if (speed == 0) {
						h_madHelper.LogPlayerVehicle(this, "#stop");
					} else {
						h_madHelper.LogPlayerVehicle(this, "#drive");
					}
					*/
				}
				h_lastSpeed = speed;
			}
		}
		
		HumanCommandVehicle vc = GetCommand_Vehicle();
		if (vc) {
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
		
		string nextStance = "none";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_PRONE) ) nextStance = "#stance_prone";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDPRONE) ) nextStance = "#stance_prone_rised";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_CROUCH) ) nextStance = "#stance_crouch";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDCROUCH) ) nextStance = "#stance_crouch_rised";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_ERECT) ) nextStance = "#stance_erect";
		if ( IsStance(newStance, DayZPlayerConstants.STANCEMASK_RAISEDERECT) ) nextStance = "#stance_erect_rised";
		if (nextStance == h_lastStance) return;
		h_lastStance = nextStance;
		h_madHelper.LogPlayerMovement(this, nextStance );
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

	/// PLAYER HITS/KILLS
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