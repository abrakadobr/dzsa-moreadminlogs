
modded class ActionBase
{
	MoreAdminLogs h_madHelper;
	
	void ActionBase() {
		h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
	}
	
	override void OnEndServer(ActionData action_data)
	{
		h_madHelper.LogAction(this, "", action_data);
		super.OnEndServer(action_data);
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper && h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class AnimatedActionBase
{

	override protected void OnExecuteServer( ActionData action_data )
	{
		h_madHelper.LogAction(this, "ANIMATED_", action_data);
		super.OnExecuteServer(action_data);
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionContinuousBase
{
	override void OnFinishProgressServer( ActionData action_data )
	{
		h_madHelper.LogAction(this, "CONTINUOUS_SERVER_", action_data);
		super.OnFinishProgressServer(action_data);
	}
	
	override void OnFinishProgress( ActionData action_data )
	{
		h_madHelper.LogAction(this, "CONTINUOUS_", action_data);
		super.OnFinishProgress(action_data);
	}
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}	
}


modded class ActionFoldBaseBuildingObject
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogAction(this, "CONTINUOUS_", action_data);
		super.OnFinishProgressServer(action_data);
	}
}

/////////    fixing m_Text and GetAdminLogMessage

modded class ActionLoadMagazineQuick			{ void ActionLoadMagazineQuick() {	m_Text = "#load_magazine_quick"; } }
modded class ActionMineBush						{ void ActionMineBush() { m_Text = "#mine_bush"; } }
modded class ActionMineBushByHand				{ void ActionMineBushByHand()	{ m_Text = "#mine_bush_hands"; } }
modded class ActionMineTree						{ void ActionMineTree() { m_Text = "#mine_tree"; } }
modded class ActionMineTreeBark					{ void ActionMineTreeBark() { m_Text = "#mine_tree_bark"; } };
modded class ActionEmptyCookingPot				{ void ActionEmptyCookingPot() { m_Text = "#empty"; } };
modded class ActionDeployHuntingTrap			{ void ActionDeployHuntingTrap() {	m_Text = "#deploy_hunting_trap"; } }
modded class ActionBandageBase					{ void ActionBandageBase() { m_Text = "#bandage"; } }
modded class ActionDefibrilateBase				{ void ActionDefibrilateBase() { m_Text = "#defebrilate"; } }
modded class ActionCarDoors						{ void ActionCarDoors()	{ m_Text = "#car_door_inside"; } }
modded class ActionCarDoorsOutside				{ void ActionCarDoorsOutside() { m_Text = "#car_door_outside"; } }
modded class ActionToggleNVG					{ void ActionToggleNVG() {	m_Text = "#toggleNVG"; } }
modded class ActionTurnOffHeadtorch				{ void ActionTurnOffHeadtorch() { m_Text = "#headtorch_off"; } }
modded class ActionTurnOffHelmetFlashlight		{ void ActionTurnOffHelmetFlashlight() { m_Text = "#helmet_flashlight_off"; } }
modded class ActionTurnOnHeadtorch				{ void ActionTurnOnHeadtorch() { m_Text = "#headtorch_on"; } }
modded class ActionTurnOnHelmetFlashlight		{ void ActionTurnOnHelmetFlashlight() { m_Text = "#helmet_flashlight_on"; } }
modded class ActionUnfoldEntity					{ void ActionUnfoldEntity() {	m_Text = "#unfold"; } }
modded class FirearmActionDetachMagazine		{ void FirearmActionDetachMagazine() {	m_Text = "#detach_magazine"; } }
modded class FirearmActionDetachMagazine_Old	{ void FirearmActionDetachMagazine_Old() {	m_Text = "#detach_magazine_old"; } }
modded class FirearmActionMechanicManipulate	{ void FirearmActionMechanicManipulate() {	m_Text = "#hands_manipulate"; } }
modded class FirearmActionUnjam					{ void FirearmActionUnjam() {	m_Text = "#firearm_unjam"; } }
modded class FirearmActionAttachMagazine		{ void FirearmActionAttachMagazine() {	m_Text = "#attach_magazine"; } }
modded class FirearmActionAttachMagazineQuick	{ void FirearmActionAttachMagazineQuick() {	m_Text = "#attach_magazine_quick"; } }
modded class FirearmActionBase					{ void FirearmActionBase() {	m_Text = "#firearm_action"; } }

/*
modded class ActionToggleNVG { void ActionToggleNVG() {	m_Text = "#toggleNVG"; } }
modded class ActionToggleNVG { void ActionToggleNVG() {	m_Text = "#toggleNVG"; } }
modded class ActionToggleNVG { void ActionToggleNVG() {	m_Text = "#toggleNVG"; } }
*/

modded class ActionPackTent
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		// return m_Text;
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionWorldCraft { override string GetText() { return "#craft_recipe"; } }

modded class ActionDeconstructShelter
{
	void ActionDeconstructShelter() { m_Text = "#deconstruct"; }
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionBuildPart
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}


modded class ActionDestroyCombinationLock
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}


modded class ActionDestroyPart
{
	override string GetText() { return "#destroy"; }
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}


modded class ActionDigInStash
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionDigOutStash
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionDismantlePart
{
	string m_TextFixed;
	
	void ActionDismantlePart() {
		m_TextFixed = "#dismantle";
	}
	
	override string GetText() { return m_TextFixed; }
	
	override void OnActionInfoUpdate( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_TextFixed = "#dismantle";
		Object target_object = target.GetObject();
		if ( target_object && target_object.CanUseConstruction() )
		{
			string part_name = target_object.GetActionComponentName( target.GetComponentIndex() );
			m_TextFixed = "#dismantle " + part_name;
		} else {
			ConstructionActionData construction_action_data = player.GetConstructionActionData();	
			m_TextFixed = "#dismantle " + construction_action_data.GetTargetPart();
		}
	}		
	
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionMountBarbedWire
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionRepairPart
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}

modded class ActionUnmountBarbedWire
{
	override string GetAdminLogMessage(ActionData action_data)
	{
		if ( h_madHelper.IsEnabled() && h_madHelper.IsOverride() ) return h_madHelper.helper().ActionInfo(this, action_data);
		return super.GetAdminLogMessage(action_data);
	}
}