# Arma Reforger RP Economy Mod

A clean-room implementation of a Role-Play economy system for Arma Reforger, designed for server-authoritative multiplayer environments.

## 🌟 Features

### 💵 Physical Wallet System
*   **Inventory Item**: Your cash is represented by a physical `RP_WalletItem`.
*   **Lootable**: Wallets can be dropped, traded, or looted from dead bodies.
*   **Smart Transfer**: Picking up another player's wallet automatically transfers the funds to your wallet and empties the item.
*   **Owner Recognition**: Picking up your own wallet simply retrieves the item without triggering a transaction.

### 🏦 Persistent Banking
*   **ATM System**: Safe storage for your funds.
*   **Persistence**: Wallet and Bank balances are saved to a database via the **Enfusion Database Framework (EDF)**.
*   **Server-Side Logic**: All transactions are verified on the server to prevent cheating.

### ☠️ Immersive Death Handling
*   **Risk vs. Reward**: When you die, your virtual wallet balance is wiped (so you respawn with $0).
*   **Corpse Looting**: The physical wallet remains on your corpse, containing the funds you had at the time of death. Other players (or you, if you return) can loot it.
*   **Bank Safety**: Funds in the bank are never lost on death.

## 🏗️ Architecture

This mod is built using Enforce Script and follows standard component-entity patterns.

### Core Components (`Scripts/Game/Components/`)
*   **`RP_PlayerEconomyComponent`**: The central manager for a player's finances. Handles `m_iWalletBalance` and `m_iBankBalance`.
*   **`RP_GameModeComponent`**: Manages the lifecycle of player data (Loading on Connect, Saving on Disconnect) and spawns initial wallet items.
*   **`RP_DeathHandlerComponent`**: Intercepts death events to handle the logic of wiping the new life's wallet while preserving the old life's loot.
*   **`RP_ATMComponent`**: Provides API methods for ATM interactions (Deposit/Withdraw).

### Items (`Scripts/Game/Items/`)
*   **`RP_WalletItem`**: A smart inventory item that acts as the physical token for currency. It contains logic to "absorb" funds when picked up by non-owners.

## 📦 Dependencies

*   **Enfusion Database Framework (EDF)**: Required for data persistence.

## 🛠️ Installation & Usage

1.  **Workbench**: Open the project in Arma Reforger Workbench.
2.  **Mission**: Load `Conf/Missions/FrontlinerRP.conf`.
3.  **Testing**:
    *   Play in Multiplayer mode.
    *   Use the `FrontlinerRP` GameMode prefab which has the necessary components attached.

## 🤝 Contributing

This project is a clean implementation using only public APIs.

1.  Fork the repository.
2.  Create a feature branch.
3.  Submit a Pull Request.

## 📄 License

MIT License.
