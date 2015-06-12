struct ToxSaveData_s
{
	unsigned char Data<>;
};

typedef struct ToxSaveData_s ToxSaveData_t;

program TOXAEMIA_PROG {
	version TOXAEMIA_VERS1 {
		/* ToxConnect
		 * Connects to Tox
		 * int BootstrapPort, string BootstrapAddress<>, string BootstrapKey<>, 
		 * string Name<>, string Status<> */
		int ToxConnect(int, string<>, string<>, string<>, string<>) = 1;

		/* ToxDisconnect
		 * Disconnects from Tox */
		void ToxDisconnect() = 2;

		/* ToxGetSaveData
		 * Returns Tox save data as ToxSaveData_t */
		ToxSaveData_t ToxGetSaveData() = 3;

		/* ToxInstallSaveData
		 * Installs Tox save data
		 * ToxSaveData_t => Data to install */
		void ToxInstallSaveData(ToxSaveData_t) = 4;

		/* ToxSendFriendRequest
		 * Send a friend request
		 * string<> => Tox ID of friend
		 * string<> => Message to include in request */
		void ToxSendFriendRequest(string<>, string<>) = 5;
	} = 1;
} = 0x22159817;
