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
	FCONN =1,
	FNAME =2,
	FSTATUSM =3,
	FSTATUS =4,
	FMESSAGE =5,
	FREQUEST =6,
	FADDED =6
};

struct ToxEvent_s
{
	ToxEventType type;
	int paramid;
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
		 * Returns number of new friend, or -1 for failure.
		 * string<> => Tox ID of friend
		 * string<> => Message to include in request */
		int ToxSendFriendRequest(string<>, string<>) = 6;

		/* ToxGetFriendList
		 * Returns friendlist as an array of friend IDs */
		ToxFriends_t ToxGetFriendList() = 7;

		/* ToxGetFriend
		 * Returns data of a friend as ToxFriend_t
		 * unsigned int => Friend number to retrieve data for */
		ToxFriend_t ToxGetFriend(unsigned int) = 8;

		/* ToxSendMessage
		 * Sends a message to a contact
		 * unsigned int => Friend number to send message to
		 * string<> => Message to send */
		void ToxSendMessage(unsigned int, string<>) = 9;

		/* ToxDeleteFriend
		 * Deletes a contact
		 * unsigned int => Friend number to delete */
		void ToxDeleteFriend(unsigned int) = 10;
	} = 1;
} = 0x22159817;
