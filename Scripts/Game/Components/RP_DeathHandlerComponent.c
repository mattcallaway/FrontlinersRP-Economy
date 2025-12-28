[ComponentEditorProps(category: "RP", description: "Handles death economy logic.")]
class RP_DeathHandlerComponentClass : ScriptComponentClass
{
}

class RP_DeathHandlerComponent : ScriptComponent
{
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SCR_CharacterDamageManagerComponent dmg = SCR_CharacterDamageManagerComponent.Cast(owner.FindComponent(SCR_CharacterDamageManagerComponent));
		if (dmg)
			dmg.GetOnDamageStateChanged().Insert(OnDeath);
	}
	
	override void OnDelete(IEntity owner)
	{
		SCR_CharacterDamageManagerComponent dmg = SCR_CharacterDamageManagerComponent.Cast(owner.FindComponent(SCR_CharacterDamageManagerComponent));
		if (dmg)
			dmg.GetOnDamageStateChanged().Remove(OnDeath);
			
		super.OnDelete(owner);
	}

	protected void OnDeath(EDamageState state)
	{
		if (state != EDamageState.DEAD)
			return;

		// Server authoritative check
		if (Replication.IsClient())
			return;
			
		IEntity owner = GetOwner();
		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(owner.FindComponent(RP_PlayerEconomyComponent));
		if (!econ) return;

		// Player has died. 
		// The wallet item on the corpse retains the current value (protected by the check in SyncWalletToItem).
		// We must now zero out the "virtual" wallet so that:
		// 1. If they disconnect, 0 is saved.
		// 2. When they respawn, they start with 0.
		
		int currentWallet = econ.GetWalletBalance();
		if (currentWallet > 0)
		{
			econ.SubtractFromWallet(currentWallet);
			Print(string.Format("RP_DeathHandler: Reset wallet of %1 to 0 on death (Loot preserved on corpse).", owner), LogLevel.NORMAL);
		}
	}
}
