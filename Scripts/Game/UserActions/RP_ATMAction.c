class RP_ATMAction : ScriptedUserAction
{
	[Attribute("100", UIWidgets.EditBox, "Amount to Deposit/Withdraw")]
	protected int m_iAmount;

	[Attribute("0", UIWidgets.ComboBox, "Action Type", enums: { ParamEnum("Deposit", "0"), ParamEnum("Withdraw", "1") })]
	protected int m_iType;

	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RP_ATMComponent atm = RP_ATMComponent.Cast(pOwnerEntity.FindComponent(RP_ATMComponent));
		if (!atm) return;

		if (m_iType == 0)
			atm.Deposit(pUserEntity, m_iAmount);
		else
			atm.Withdraw(pUserEntity, m_iAmount);
	}

	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}
	
	override bool GetActionNameScript(out string outName)
	{
		if (m_iType == 0) outName = "Deposit $" + m_iAmount;
		else outName = "Withdraw $" + m_iAmount;
		return true;
	}
}
