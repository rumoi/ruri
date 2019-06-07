#pragma once


enum IRC_Perm {
	IRC_Public = 0,
	IRC_Supporter = 1,
	IRC_Premium = 2,
	IRC_Mod = 3,
	IRC_Admin = 4,
	IRC_Dev = 5
};
__forceinline int GetMaxPerm(const DWORD Priv) {

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

	int ViewLevel;
	int WriteLevel;
	int NameSum;
	bool AutoJoin;

	_Channel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel, const bool AJ = 0) {
		NameSum = WeakStringToInt(Name);
		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;

		AutoJoin = AJ;
	}

	void CreateChannel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel, const bool AJ = 0) {
		NameSum = WeakStringToInt(Name);
		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;
		AutoJoin = AJ;
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
					ChannelCount++;
					break;
				}
			}

		}

		if (NotIn) {
			CleanChannel();
			for (DWORD i = 0; i < MAX_USER_COUNT; i++){
				if (ConnectedUsers[i] == 0){
					ConnectedUsers[i] = u;
					ChannelCount++;
					break;
				}
			}

		}
		Lock.unlock();
	}

	void PartChannel(_User* u, const DWORD Offset = 0){

		Lock.lock();

		if (Offset && ConnectedUsers[Offset] == u){
			ConnectedUsers[Offset] = 0;
			ChannelCount--;
		}else{
			for (DWORD i = 0; i < MAX_USER_COUNT; i++)
				if (ConnectedUsers[i] == u) {
					ConnectedUsers[i] = 0;
					ChannelCount--;
					break;
				}
		}
		Lock.unlock();
	}

	void SendPublicMessage(_User* Sender, const _BanchoPacket b){
		
		if (!ChannelCount || WriteLevel > GetMaxPerm(Sender->privileges))return;

		for (DWORD i = 0; i < MAX_USER_COUNT; i++){

			_User *u = ConnectedUsers[i];

			if (!u || u == Sender)continue;

			if (!u->choToken){
				PartChannel(u,i);
				continue;
			}

			u->addQue(b);
		}

	}

	void Bot_SendMessage(const std::string &message) {

		if (!ChannelCount)return;

		const _BanchoPacket b = bPacket::Message(BOT_NAME, ChannelName, message, 999);

		for (DWORD i = 0; i < MAX_USER_COUNT; i++){

			_User *u = ConnectedUsers[i];

			if (!u || !u->choToken)
				continue;

			if (!u->choToken){
				PartChannel(u);
				continue;
			}

			u->addQue(b);
		}

	}

	_Channel() {
		ViewLevel = 0;
		WriteLevel = 0;
		NameSum = 0;
		ZeroMemory(ConnectedUsers, sizeof(_User*) * MAX_USER_COUNT);
	}

};

_Channel chan_Akatsuki("#akatsuki","Akatsuki General.", IRC_Public, IRC_Public,1);
_Channel chan_Announce("#announce", "Public announcements.", IRC_Public, IRC_Admin);//Announcement being an option is on purpose
_Channel chan_Supporter("#supporter", "Supporter only chat.", IRC_Supporter, IRC_Supporter,1);
_Channel chan_Admin("#admin", "Command dumpster.", IRC_Admin, IRC_Admin,1);
_Channel chan_DevLog("#devlog", "Log all the things.", IRC_Dev, IRC_Dev, 1);
_Channel chan_Lobby("#lobby", "Chat with others browsing for a lobby.", IRC_Public, IRC_Public);

const std::vector<_Channel*> ChannelList = {&chan_Akatsuki,&chan_Announce,&chan_Supporter,&chan_Admin,&chan_DevLog,&chan_Lobby};

__forceinline _Channel* GetChannelByName(const std::string &Name){

	const int Sum = WeakStringToInt(Name);

	for (DWORD i = 0; i < ChannelList.size(); i++)
		if (ChannelList[i]->NameSum == Sum)
			return ChannelList[i];
	
	return 0;
}
