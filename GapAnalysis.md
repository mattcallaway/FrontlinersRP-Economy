# Gap Analysis Report

**Project**: FrontlinersRP-Economy
**Date**: 2025-12-28
**Status**: 🟡 Partial Implementation (Script/Logic Core Present, Assets/Config Missing)

## 📊 Component Audit

| Component | Status | Path / Note | Action Required |
| :--- | :---: | :--- | :--- |
| **Project Configuration** | ❌ Missing | `addon.gproj` | **Create** standard project config. |
| **Economy Logic** | ✅ Present | `Scripts/Game/Components/RP_PlayerEconomyComponent.c` | None. |
| **Wallet Item Logic** | ✅ Present | `Scripts/Game/Items/RP_WalletItem.c` | None. |
| **Wallet Prefab** | ✅ Present | `Prefabs/Items/RP_WalletItem.et` | None. |
| **GameMode Logic** | ✅ Present | `Scripts/Game/Components/RP_GameModeComponent.c` | None. |
| **GameMode Prefab** | ✅ Present | `Prefabs/GameModes/FrontlinerRP_GameMode.et` | None. |
| **Database Schema** | ❌ Missing | `Scripts/Game/Database/RP_EconomyData.c` | **Create** EDF Entity class. |
| **ATM Logic** | ⚠️ Partial | `Scripts/Game/Components/RP_ATMComponent.c` | Logic exists, but no UserAction triggers. |
| **ATM Prefab** | ❌ Missing | `Prefabs/Props/RP_ATM.et` | **Create** placeholder physical ATM. |
| **ATM User Action** | ❌ Missing | `Scripts/Game/UserActions/RP_ATMAction.c` | **Create** interaction script. |
| **Item Registry** | ❌ Missing | `Conf/Entities/Items/RP_Items.conf` | **Create** catalog config. |
| **Mission Config** | ✅ Present | `Conf/Missions/FrontlinerRP.conf` | None. |

## 🛠️ Logical Gaps

### 1. Database Definition
The code references `EDF_DbManager.Get("rp_player_economy")`, but the actual data structure (`[EDF_DbEntity]`) is not defined. Without this, EDF cannot create or read the table.

### 2. Interaction Layer
`RP_ATMComponent` has `Deposit()` functions, but there is no way for a player to call them. We need a `ScriptedUserAction` attached to the ATM prefab to trigger the interaction.

### 3. Project Structure
Missing `addon.gproj`. The Workbench will not recognize this folder as a valid mod project without it.

## 📋 Recommendations

1.  **Immediate**: Create `addon.gproj` to validate project structure.
2.  **Critical**: Implement `RP_EconomyData.c` to define the EDF schema.
3.  **Functional**: Create `RP_ATMAction.c` and `RP_ATM.et` to allow testing of banking.
4.  **Config**: Register items in `RP_Items.conf`.
