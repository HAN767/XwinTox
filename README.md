XwinTox and Toxaemia
==============

Overview
--------------

*XwinTox* (and its counterpart, *Toxaemia*,) is a different Tox client: 
a radical one. 

It maintains a true division between interface and logic by placing libToxCore 
communication in one binary, processing messages received from the libToxCore 
binary in one thread of another binary, and implementing the user interface in 
another thread of that latter binary. *XwinTox* is the user interface side; 
*Toxaemia* is the service that communicates with Tox.

XwinTox's development team rejects the bourgeois notion of commercial exploitation
as being a freedom in itself; we note that while the GPL and Stallman's principles
in general are a good thing that have promoted within software a greater level of
the freedoms all are entitled to, it has not gone far enough: it is too easy for
corporate entities to carve out a hegemonic position within free software projects.
Regardless, XwinTox and Toxaemia are free software; XwinTox and Toxaemia may be 
used under the provisions of the GPL version 3.0 or later, which may be found in 
full form in the file CPYRIGHT. LibSunshine, the Sunshine Software Support Library,
may be used under the provisions of the ZLib Licence, a copy of which is included
within the file CPYRIGHT in the libsunshine_src folder.


Building
--------------

*Note*: **XwinTox natively uses the SunshinePro Toolchain.
However, as SunshinePro D-Make has a POSIX featureset with a few
GNU extensions I implemented, it is possible to build with *GNU Make*.**

You will need a C compiler capable of building C11 (recent GCC or Clang will
work) and a C++ compiler capable of building C++11 (as before, recent GCC or
Clang will work). You will also require FLTK (including headers) and rpcgen,
which is provided by SunRPC.

Modify ```Makefile.cfg``` as appropriate to your platform and run GNU make.
If you want Clang's static analyser to run (this is not available if you use GNU
CC) you can do so by running ```make real```

Toxaemia Protocol
--------------

Copied from toxaemia.x.

```c
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
	GNAMES =24
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

		/* ToxSendFriendRequest
		 * Send a friend request.
		 * Returns number of new friend, or -1 for failure.
		 * string<> => Tox ID of friend
		 * string<> => Message to include in request */
		int ToxSendFriendRequest(string<>, string<>) = 6;

		/* ToxGetFriendList
		 * Returns friendlist as an array of friend IDs. */
		ToxFriends_t ToxGetFriendList() = 7;

		/* ToxGetFriend
		 * Returns data of a friend as ToxFriend_t.
		 * unsigned int => Friend number to retrieve data for */
		ToxFriend_t ToxGetFriend(unsigned int) = 8;

		/* ToxSendMessage
		 * Sends a message to a contact
		 * unsigned int => Friend number to send message to
		 * string<> => Message to send */
		void ToxSendMessage(unsigned int, string<>) = 9;

		/* ToxDeleteFriend
		 * Deletes a contact.
		 * unsigned int => Friend number to delete */
		void ToxDeleteFriend(unsigned int) = 10;

		/* ToxCreateGroupChat
		 * Creates a new groupchat
		 * Returns number of new friend, or -1 for failure. */
		int ToxCreateGroupChat() = 11;
	} = 1;
} = 0x22159817;
```


