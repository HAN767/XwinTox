struct ToxSaveData_s
{
	opaque Data<>;
};

struct ToxFriends_s
{
	unsigned int Data<>;
};

struct ToxFriend_s
{
	string name<>;
	string statusm<>;
};

typedef struct ToxSaveData_s ToxSaveData_t;
typedef struct ToxFriends_s ToxFriends_t;
typedef struct ToxFriend_s ToxFriend_t;

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

		/* ToxGetFriendList
		 * Returns friendlist as an array of friend IDs */
		ToxFriends_t ToxGetFriendList() = 6;

		/* ToxGetFriend
		 * Returns data of a friend as ToxFriend_t
		 * unsigned int => Friend number to retrieve data for */
		ToxFriend_t ToxGetFriend(unsigned int) = 7;
	} = 1;
} = 0x22159817;
