#pragma once


enum IRC_Perm {
	IRC_Public = 0,
	IRC_Supporter = 1,
	IRC_Premium = 2,
	IRC_Mod = 3,
	IRC_Admin = 4,
	IRC_Dev = 5
};
__forceinline IRC_Perm GetMaxPerm(const DWORD Priv) {

	if (Priv & Privileges::AdminDev)return IRC_Dev;
	if (Priv & Privileges::AdminBanUsers)return IRC_Admin;
	if (Priv & Privileges::AdminChatMod)return IRC_Mod;
	if (Priv & Privileges::Premium)return IRC_Premium;
	if (Priv & Privileges::UserDonor)return IRC_Supporter;

	return IRC_Public;
}


struct _Channel{

	std::string ChannelName;
	std::string ChannelDesc;
	int ChannelCount;
	std::mutex Lock;

	_User* ConnectedUsers[MAX_USER_COUNT];

	//std::vector<_User*> ConnectedUsers;

	int ViewLevel;
	int WriteLevel;


	_Channel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel){

		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;
	}

	void CreateChannel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel) {

		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;
	}

	void CleanChannel(){

		_User* NewUsers[MAX_USER_COUNT];
		ZeroMemory(NewUsers, sizeof(_User*) * MAX_USER_COUNT);
		DWORD NOffset = 0;

		for (DWORD i = 0; i < MAX_USER_COUNT; i++){
			if (ConnectedUsers[i] && ConnectedUsers[i]->choToken){
				NewUsers[NOffset] = ConnectedUsers[i];
				NOffset++;
			}
		}

		memcpy(ConnectedUsers, NewUsers, sizeof(_User*) * MAX_USER_COUNT);
	}

	void JoinChannel(_User* u){

		if (ViewLevel > GetMaxPerm(u->privileges))return;

		Lock.lock();
		ChannelCount++;
		bool NotIn = 1;

		for (DWORD i = 0; i < MAX_USER_COUNT; i++) {
			if (ConnectedUsers[i] == u){
				NotIn = 0;
				break;
			}
		}
		if (NotIn) {

			for (DWORD i = 0; i < MAX_USER_COUNT; i++) {
				if (ConnectedUsers[i] == 0) {
					NotIn = 0;
					ConnectedUsers[i] = u;
					break;
				}
			}

		}

		if (NotIn) {
			CleanChannel();
			for (DWORD i = 0; i < MAX_USER_COUNT; i++){
				if (ConnectedUsers[i] == 0){
					ConnectedUsers[i] = u;
					break;
				}
			}

		}

		Lock.unlock();

	}
	void PartChannel(_User* u, const DWORD Offset = 0){

		Lock.lock();
		ChannelCount--;

		if(Offset && ConnectedUsers[Offset] == u)
			ConnectedUsers[Offset] = 0;
		else {
			for (DWORD i = 0; i < MAX_USER_COUNT; i++)
				if (ConnectedUsers[i] == u) {
					ConnectedUsers[i] = 0;
					break;
				}
		}
		Lock.unlock();

	}


	void SendPublicMessage(_User* Sender, const _BanchoPacket b){

		if (WriteLevel > GetMaxPerm(Sender->privileges))return;

		for (DWORD i = 0; i < MAX_USER_COUNT; i++){

			_User *u = ConnectedUsers[i];

			if (!u || u == Sender)continue;
			if (!u->choToken) {
				PartChannel(u,i);
				continue;
			}

			ConnectedUsers[i]->addQue(b);

		}

	}
	void Bot_SendMessage(const std::string &Name, const DWORD UserID, std::string message) {

		const _BanchoPacket b = bPacket::Message(Name, ChannelName, message, 999);

		for (DWORD i = 0; i < MAX_USER_COUNT; i++) {

			_User *u = ConnectedUsers[i];

			if (!u || !u->choToken)continue;
			if (!u->choToken) {
				PartChannel(u);
				continue;
			}

			ConnectedUsers[i]->addQue(b);

		}

	}

	_Channel() {
		ViewLevel = 0;
		WriteLevel = 0;
		ZeroMemory(ConnectedUsers, sizeof(_User*) * MAX_USER_COUNT);
	}

};


_Channel chan_Akatsuki("#akatsuki","Akatsuki General.", IRC_Public, IRC_Public);
_Channel chan_Lobby("#lobby", "Chat with others browsing for a lobby.", IRC_Public, IRC_Public);
_Channel chan_Announce("#announce", "Alerts you when something interesting happens", IRC_Public, IRC_Admin);

__forceinline _Channel* GetChannelByName(const std::string &Name){
	if (Name == chan_Akatsuki.ChannelName)return &chan_Akatsuki;
	if (Name == chan_Lobby.ChannelName)return &chan_Lobby;
	if (Name == chan_Announce.ChannelName)return &chan_Announce;


	return 0;
}
