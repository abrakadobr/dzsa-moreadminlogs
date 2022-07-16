modded class WeaponManager
{
	override void Fire(Weapon_Base wpn)
	{
		super.Fire(wpn);
		MoreAdminLogs h_madHelper = MoreAdminLogs.Cast(GetPlugin(MoreAdminLogs));
		h_madHelper.LogWeapon(m_player,"#fire", wpn);
	}
}