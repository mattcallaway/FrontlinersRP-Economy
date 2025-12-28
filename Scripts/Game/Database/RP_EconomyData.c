[EDF_DbEntity(name: "rp_player_economy")]
class RP_EconomyData : EDF_DbEntity
{
	[EDF_DbField(isPrimaryKey: true)]
	string uid;

	[EDF_DbField]
	int wallet;

	[EDF_DbField]
	int bank;

	// Constructor
	void RP_EconomyData(string id, int w, int b) { uid = id; wallet = w; bank = b; }
}
