#pragma once


enum IRC_Perm {
	IRC_Public = 0,
	IRC_Supporter = 1,
	IRC_Premium = 2,
	IRC_Mod = 3,
	IRC_Admin = 4,
	IRC_Dev = 5
};
_inline int GetMaxPerm(const DWORD Priv) {

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
	
	std::array<_User*, MAX_USER_COUNT> IRCUsers;

	int ViewLevel;
	int WriteLevel;
	int NameSum;
	bool AutoJoin;

	_Channel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel, const bool AJ = 0) :IRCUsers({0}) {
		NameSum = WeakStringToInt(Name);
		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;

		AutoJoin = AJ;
	}

	void CreateChannel(const std::string Name, const std::string Desc, const int viewlevel, const int writeLevel, const bool AJ = 0){
		NameSum = WeakStringToInt(Name);
		ChannelName = Name;
		ChannelDesc = Desc;

		ViewLevel = viewlevel;
		WriteLevel = writeLevel;
		AutoJoin = AJ;

		IRCUsers.fill(0);
	}

	bool CleanChannel(){

		std::array<_User*, MAX_USER_COUNT> NewUsers = { 0 };
		
		DWORD NOffset = 0;

		for (auto& u : IRCUsers) {
			_User*const User = u;

			if (!User || !User->choToken)
				continue;
			NewUsers[NOffset] = User;
			NOffset++;
		}

		if (memcmp(&NewUsers[0], &IRCUsers[0],sizeof(_User*) * MAX_USER_COUNT))
			return 0;

		IRCUsers = _M(NewUsers);
		ChannelCount = NOffset;

		return 1;
	}

	void JoinChannel(_User* u){

		if (ViewLevel > GetMaxPerm(u->privileges))return;

		Lock.lock();

		bool NeedToAdd = 1;

		for(const auto& User : IRCUsers)
			if (User == u){
				NeedToAdd = 0;
				break;
			}
		
		do {

			if (NeedToAdd){
				for (auto& User : IRCUsers)
					if (!User){

						u->AddChannel((size_t)this);

						User = u;
						ChannelCount++;

						NeedToAdd = 0;
						break;
					}
			}

		} while (NeedToAdd && CleanChannel());
		
		Lock.unlock();
	}

	void PartChannel(_User* u, const DWORD Offset = 0){

		if (Offset > MAX_USER_COUNT)
			return;

		Lock.lock();

		if (Offset && IRCUsers[Offset] == u){

			IRCUsers[Offset] = 0;
			ChannelCount--;
			u->RemoveChannel((size_t)this);

		}else for (auto& User : IRCUsers)
			if (User == u){
				User = 0;
				ChannelCount--;
				u->RemoveChannel((size_t)this);
				break;
			}

		Lock.unlock();
	}

	void SendPublicMessage(_User* Sender, const _BanchoPacket &b){
		
		if (!ChannelCount || WriteLevel > GetMaxPerm(Sender->privileges))return;

		const DWORD& ID = Sender->UserID;

		for (auto const User : IRCUsers){

			if (!User || !User->choToken || User == Sender || User->isBlocked(ID))
				continue;

			/*if (!User->choToken) {
				PartChannel(User, (size_t(&User) - size_t(&IRCUsers[0])) / sizeof(_User*));
				continue;
			}*/

			User->addQue(b);

		}

	}
	/*
	template <typename T>
	void Bot_SendMessage_Conditional(const std::string &&message, const T Cond){

		if (!ChannelCount)return;

		const _BanchoPacket b = bPacket::Message(BOT_NAME, ChannelName, message, 999);

		for (auto& User : IRCUsers) {
			
			if (!User || !User->choToken || !Cond(User))
				continue;
			
			User->addQue(b);

		}
	}*/


	void Bot_SendMessage(const std::string_view message){

		if (!ChannelCount)return;

		const _BanchoPacket b = bPacket::Message(BOT_NAME, ChannelName, message, 999);

		for (auto const User : IRCUsers){

			if (!User)
				continue;

			if (!User->choToken){
				PartChannel(User, (size_t(&User) - size_t(&IRCUsers[0])) / sizeof(_User*));
				continue;
			}

			User->addQue(b);
		}

	}

	_Channel():IRCUsers({ 0 }) {
		ViewLevel = 0;
		WriteLevel = 0;
		NameSum = 0;
		ChannelCount = 0;
		AutoJoin = 0;
	}

};



_Channel chan_Akatsuki("#akatsuki","Akatsuki General.", IRC_Public, IRC_Public,1);
_Channel chan_Announce("#announce", "Public announcements.", IRC_Public, IRC_Admin);//Announcement being an option is on purpose
_Channel chan_Supporter("#supporter", "Supporter only chat.", IRC_Supporter, IRC_Supporter,1);
_Channel chan_Admin("#admin", "Command dumpster.", IRC_Admin, IRC_Admin,1);
_Channel chan_DevLog("#devlog", "Log all the things.", IRC_Dev, IRC_Dev, 1);
_Channel chan_Lobby("#lobby", "Chat with others browsing for a lobby.", IRC_Public, IRC_Public);

_Channel* const ChannelList[] = {&chan_Akatsuki,&chan_Announce,&chan_Supporter,&chan_Admin,&chan_DevLog,&chan_Lobby};

constexpr size_t ChannelListSize() noexcept{
	return sizeof(ChannelList) / sizeof(_Channel*);
}

_inline _Channel* GetChannelByName(const std::string &Name){

	const int Sum = WeakStringToInt(Name);

	for (DWORD i = 0; i < ChannelListSize(); i++)
		if (ChannelList[i]->NameSum == Sum)
			return ChannelList[i];
	
	return 0;
}
