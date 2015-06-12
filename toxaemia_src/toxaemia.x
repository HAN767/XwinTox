struct ToxSaveData_s
{
	string Data<>;
	int Len;
};

typedef struct ToxSaveData_s ToxSaveData_t;

program TOXAEMIA_PROG {
	version TOXAEMIA_VERS1 {
		/* ToxConnect
		 * Connects to Tox
		 * int BootstrapPort, string BootstrapAddress<>, string BootstrapKey<>, 
		 * string Name<>, string Status<> */
		int ToxConnect(int, string<>, string<>, string<>, string<>) = 1;
		void ToxDisconnect() = 2;
		ToxSaveData_t ToxGetSaveData() = 3;
	} = 1;
} = 0x22159817;
