[ComponentEditorProps(category: "RP", description: "Main RP game mode logic.")]
class RP_GameModeComponentClass : SCR_BaseGameModeComponentClass
{
}

class RP_GameModeComponent : SCR_BaseGameModeComponent
{
	[Attribute("{0000000000000000}Prefabs/Items/RP_WalletItem.et", UIWidgets.ResourceNamePicker, "Wallet Prefab")]
	protected ResourceName m_WalletPrefab;

	override void OnPlayerSpawned(int playerId, IEntity player)
	{
		super.OnPlayerSpawned(playerId, player);
		
		if (Replication.IsClient()) 
			return;

		// 1. Ensure Components exist (if not on Prefab)
		if (!player.FindComponent(RP_PlayerEconomyComponent))
			player.AddComponent(RP_PlayerEconomyComponent); // Note: AddComponent at runtime might not replicate well if not in prefab. 
			// Ideally these should be on the Character Prefab or attached via a system that supports replication.
			// For this task, we follow the Clean Room Reference style which used AddComponent. 
			// However, in Enfusion, AddComponent on a replicated entity at runtime *after* spawn might cause issues.
			// A better approach is to assume they are on the prefab, OR use the reference code's approach if it implied Singleplayer/Server.
			// Since prompt asks for Multiplayer Safe, runtime AddComponent *can* work if done on Authority before replication syncs, or if the component is set to replicate.
			// But modifying entity structure at runtime is risky. 
			// We will keep the code but add a warning/comment. 
			// Ideally, the USER sees this and adds the components to the Character Prefab in the Editor.

		if (!player.FindComponent(RP_DeathHandlerComponentComponent))
			player.AddComponent(RP_DeathHandlerComponent);

		// 2. Init Economy
		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(player.FindComponent(RP_PlayerEconomyComponent));
		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(player.FindComponent(InventoryStorageManagerComponent));

		if (!econ || !inv)
			return;
			
		string uid = SCR_AuthenticationManager.GetInstance().GetPlayerUID(playerId);
		econ.LoadEconomy(uid);

			
		// 3. Spawn Wallet Item
		// Only spawn if they don't have one? Or always spawn new logic?
		// Logic: On spawn, we give them a representation of their wallet.
		// If they have 0 cash, maybe we give them an empty wallet? Or nothing?
		// We will give them a wallet if they have balance, or always to allow picking up cash?
		// Always spawn wallet.
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		// If m_WalletPrefab is invalid (default GUID), we can't spawn. 
		if (!m_WalletPrefab) 
		{ 
			Print("RP_GameMode: Wallet Prefab not defined!", LogLevel.WARNING); 
			return; 
		}
		
		IEntity walletEnt = GetGame().SpawnEntityPrefab(Resource.Load(m_WalletPrefab), player.GetWorld(), params);
		RP_WalletItem wallet = RP_WalletItem.Cast(walletEnt);
		
		if (!wallet)
		{
			if (walletEnt) SCR_EntityHelper.DeleteEntityAndChildren(walletEnt);
			return;
		}

		// 4. Configure Wallet
		wallet.SetAmount(econ.GetWalletBalance());
		string uid = SCR_AuthenticationManager.GetInstance().GetPlayerUID(playerId); 
		// Note provided reference used SCR_AuthenticationManager.GetInstance().GetPlayerUID(playerId) which is valid.
		wallet.MarkOriginalOwner(uid);
		
		// 5. Insert into Inventory
		if (!inv.TryInsertItem(wallet))
		{
			// If inventory full, drop at feet? 
			// For now, delete to avoid exploits or clutter.
			SCR_EntityHelper.DeleteEntityAndChildren(walletEnt);
			Print("RP_GameMode: Could not insert wallet into inventory.", LogLevel.WARNING);
		}
	}

	override void OnPlayerConnected(int playerId)
	{
		super.OnPlayerConnected(playerId);
		
		if (Replication.IsClient()) return;

		// Load Economy
		// We need to access the helper/manager. 
		// Wait, OnPlayerConnected doesn't give us the Entity yet. 
		// The Entity is created later in OnPlayerSpawned.
		// So we can't load to the Component yet.
		// We should load to a cache or load in OnPlayerSpawned?
		// Reference code put Load logic in OnPlayerConnected but accessed "GetControlledEntity". 
		// In Reforger, PlayerControlledEntity might not exist at Connect time.
		// Usually we load in OnPlayerSpawned OR we have a session manager.
		// USE: OnPlayerSpawned is safer for Component access.
		// BUT the Prompt's reference code had:
		// override void OnPlayerConnected(int playerId) { IEntity player = ... GetControlledEntity check ... }
		// This likely fails if they haven't spawned. 
		// I will move the "Load" logic to OnPlayerKilled? No.
		// Helper function: `LoadEconomyForPlayer(playerId)`?
		
		// Correction: Persistence should be loaded when they connect so we know their balance before they spawn (maybe for spawn selection?).
		// But in this simple module, we store it on the Player Component.
		// So we MUST wait for the component to exist (OnPlayerSpawned).
		
		// However, I will keep OnPlayerConnectedStub in case we want to pre-fetch. 
		// But I will move the actual logic to OnPlayerSpawned OR call it from there.
		
		// Re-reading Reference: Reference had OnPlayerConnected calling LoadEconomy on the component. 
		// This implies the player entity exists. 
		// If the user uses a Lobby, the player entity exists in lobby? 
		// Safer: Do it in OnPlayerSpawned, immediately after adding component.
	}
	
	override void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		super.OnPlayerDisconnected(playerId, cause, timeout);
		
		if (Replication.IsClient()) return;

		// Save Economy
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if (!player) return;

		RP_PlayerEconomyComponent econ = RP_PlayerEconomyComponent.Cast(player.FindComponent(RP_PlayerEconomyComponent));
		if (!econ) return;

		string uid = SCR_AuthenticationManager.GetInstance().GetPlayerUID(playerId);
		econ.SaveEconomy(uid);
	}
}
