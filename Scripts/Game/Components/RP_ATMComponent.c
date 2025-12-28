[ComponentEditorProps(category: "RP", description: "Handles ATM interactions.")]
class RP_ATMComponentClass : ScriptComponentClass
{
}

class RP_ATMComponent : ScriptComponent
{
	// Future: Integration with UserAction for "Use ATM"
	// For now, providing the logic APIs called by UI or Actions
	
	void Deposit(IEntity player, int amount)
	{
		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(player.FindComponent(RP_PlayerEconomyComponent));
		if (!econ) return;
		
		if (econ.SubtractFromWallet(amount))
		{
			econ.AddToBank(amount);
			Print(string.Format("ATM: Deposited %1", amount), LogLevel.NORMAL);
		}
	}
	
	void Withdraw(IEntity player, int amount)
	{
		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(player.FindComponent(RP_PlayerEconomyComponent));
		if (!econ) return;
		
		if (econ.SubtractFromBank(amount))
		{
			econ.AddToWallet(amount);
			Print(string.Format("ATM: Withdrew %1", amount), LogLevel.NORMAL);
		}
	}
}
