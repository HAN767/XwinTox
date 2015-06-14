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
	string pubkey<>;
	string name<>;
	string statusm<>;
};

enum ToxEventType
{
	FCONN =0,
	FNAME =1,
	FSTATUSM =2,
	FSTATUS =3,
	FMESSAGE =4,
	FREQUEST =5
};


struct ToxEvent_s
{
	ToxEventType type;
	int param0;
	string param1<>;
	string param2<>;
	string param3<>;
};

typedef struct ToxSaveData_s ToxSaveData_t;
typedef struct ToxFriends_s ToxFriends_t;
typedef struct ToxFriend_s ToxFriend_t;
typedef struct ToxEvent_s ToxEvent_t;

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

		/* ToxGetEvent
		 * Retrieves the first event in the event queue
		 * Returns null if the event queue is empty */
		ToxEvent_t ToxGetEvent() = 3;

		/* ToxGetSaveData
		 * Returns Tox save data as ToxSaveData_t */
		ToxSaveData_t ToxGetSaveData() = 4;

		/* ToxInstallSaveData
		 * Installs Tox save data
		 * ToxSaveData_t => Data to install */
		void ToxInstallSaveData(ToxSaveData_t) = 5;

		/* ToxSendFriendRequest
		 * Send a friend request
		 * string<> => Tox ID of friend
		 * string<> => Message to include in request */
		void ToxSendFriendRequest(string<>, string<>) = 6;

		/* ToxGetFriendList
		 * Returns friendlist as an array of friend IDs */
		ToxFriends_t ToxGetFriendList() = 7;

		/* ToxGetFriend
		 * Returns data of a friend as ToxFriend_t
		 * unsigned int => Friend number to retrieve data for */
		ToxFriend_t ToxGetFriend(unsigned int) = 8;
	} = 1;
} = 0x22159817;
