class RP_WalletItemClass : InventoryItemEntityClass
{
	// Prefab configuration if needed
}

class RP_WalletItem : InventoryItemEntity
{
	protected int m_iAmount;
	protected bool m_bLooted;
	protected string m_sOriginalOwnerUID;
	
	// Replicates the amount variable so it syncs to clients (if needed for UI, but prompt says Server Authoritative)
	// For now, valid Enfusion pattern is to keep logic on Server.
	
	void SetAmount(int amt) 
	{ 
		m_iAmount = Math.Max(0, amt); 
	}
	
	int GetAmount() 
	{ 
		return m_iAmount; 
	}

	void MarkOriginalOwner(string uid) 
	{ 
		m_sOriginalOwnerUID = uid; 
	}
	
	string GetOriginalOwnerUID()
	{
		return m_sOriginalOwnerUID;
	}

	override void OnItemAttached(IEntity newOwner)
	{
		super.OnItemAttached(newOwner);
		
		if (Replication.IsClient())
			return;

		if (m_bLooted || m_iAmount <= 0) 
			return;

		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(newOwner.FindComponent(RP_PlayerEconomyComponent));
		if (!econ) 
			return;

		string uid = GetPlayerUID(newOwner);
		
		// If returned to original owner, do nothing (they just hold the item)
		if (uid == m_sOriginalOwnerUID) 
			return;

		// Initial implementation: Absorb money immediately on pickup
		// "Can only transfer money once"
		econ.AddToWallet(m_iAmount);
		
		// "After money is transferred -> wallet becomes empty"
		m_iAmount = 0; 
		m_bLooted = true;
		
		Print(string.Format("RP_Wallet: Transferred %1 to player %2", m_iAmount, uid), LogLevel.NORMAL);
	}
	
	protected string GetPlayerUID(IEntity player)
	{
		// Helper to get UID from entity
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player);
		if (playerId == 0) return "";
		
		return GetGame().GetBackendApi().GetPlayerIdentityId(playerId).ToString();
	}
}
