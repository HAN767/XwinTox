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
	short connected;
	int status;
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
	FADDED =7,
	GTITLE =21,
	GMESSAGE =22,
	GINVITE =23,
	GNAMES =24,
	GNEW =25
};

struct ToxEvent_s
{
	ToxEventType type;
	int paramid;
	int param0;
	string param1<>;
	opaque param2<>;
	short param3<>;
};

enum ToxMessageType
{
	MFRIEND =1,
	MGCHAT =2
};

struct Event_s
{
	int T;
	int ID;
	string S1<>;
	string S2<>;
	int I1;
	int I2;
	short SI1<>;
	short SI2<>;
	opaque O<>;
};

typedef struct ToxSaveData_s ToxSaveData_t;
typedef struct ToxFriends_s ToxFriends_t;
typedef struct ToxFriend_s ToxFriend_t;
typedef struct ToxEvent_s ToxEvent_t;
typedef struct Event_s Event_t;

program TOXAEMIA_PROG {
	version TOXAEMIA_VERS1 {
		/* ToxConnect
		 * Connects to Tox.
		 * int BootstrapPort, string BootstrapAddress<>, string BootstrapKey<>, 
		 * string Name<>, string Status<> */
		int ToxConnect(int, string<>, string<>, string<>, string<>) = 1;

		/* ToxDisconnect
		 * Disconnects from Tox. */
		void ToxDisconnect() = 2;

		/* ToxGetEvent
		 * Retrieves the first event in the event queue.
		 * Returns null if the event queue is empty. */
		ToxEvent_t ToxGetEvent() = 3;

		/* ToxGetSaveData
		 * Returns Tox save data as ToxSaveData_t. */
		ToxSaveData_t ToxGetSaveData() = 4;

		/* ToxInstallSaveData
		 * Installs Tox save data.
		 * ToxSaveData_t => Data to install */
		void ToxInstallSaveData(ToxSaveData_t) = 5;

		/* ToxSetNameStatus
		 * Sets name and status.
		 * string<> => New name
		 * string<> => New status */
		void ToxSetNameStatus(string<>, string<>) = 6;

		/* ToxSendFriendRequest
		 * Send a friend request.
		 * Returns number of new friend, or -1 for failure.
		 * string<> => Tox ID of friend
		 * string<> => Message to include in request */
		int ToxSendFriendRequest(string<>, string<>) = 7;

		/* ToxGetFriendList
		 * Returns friendlist as an array of friend IDs. */
		ToxFriends_t ToxGetFriendList() = 8;

		/* ToxGetFriend
		 * Returns data of a friend as ToxFriend_t.
		 * unsigned int => Friend number to retrieve data for */
		ToxFriend_t ToxGetFriend(unsigned int) = 9;

		/* ToxSendMessage
		 * Sends a message to a contact or groupchat.
		 * unsigned int => Friend number to send message to
		 * string<> => Message to send */
		void ToxSendMessage(ToxMessageType, unsigned int, string<>) = 10;

		/* ToxDeleteFriend
		 * Deletes a contact.
		 * unsigned int => Friend number to delete */
		void ToxDeleteFriend(unsigned int) = 11;

		/* ToxAddFriendNoRequest
		 * Adds a friend without sending a request.
		 * This is used to accept a friend request.
		 * Returns number of new friend, or -1 for failure.
		 * string<> => Tox ID to add */
		int ToxAddFriendNoRequest(string<>) = 12;

		/* ToxCreateGroupchat
		 * Creates a new groupchat
		 * Returns number of new groupchat, or -1 for failure. */
		int ToxCreateGroupchat() = 50;

		/* ToxLeaveGroupchat
		 * Leaves a groupchat
		 * unsigned int => Groupchat number to leave */
		void ToxLeaveGroupchat(unsigned int) = 51;
	} = 1;
} = 0x22159817;
