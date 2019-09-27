#pragma once


enum IRC_Perm {
	IRC_Public = 0,
	IRC_Supporter = 1,
	IRC_Premium = 2,
	IRC_Mod = 3,
	IRC_Admin = 4,
	IRC_Dev = 5
};
_inline byte GetMaxPerm(const DWORD Priv) {

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
	std::mutex Lock;
	std::array<_User*, MAX_USER_COUNT> IRCUsers;

	u32 NameSum;

	u8 ViewLevel:3, WriteLevel:3, AutoJoin:1, Hidden:1;
	u16 ChannelCount;

	_Channel(const std::string& ChannelName, const std::string &ChannelDesc, const int viewlevel, const int writeLevel, const bool AJ, const bool Hidden = 0):
			ChannelName(ChannelName),
			NameSum(WeakStringToInt(ChannelName)),
			ChannelDesc(ChannelDesc)
			{		
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

	void SendPublicMessage(_User* Sender, const VEC(byte) &b){
		
		if (!ChannelCount || WriteLevel > GetMaxPerm(Sender->privileges))
			return;

		const DWORD& ID = Sender->UserID;

		for (auto const User : IRCUsers){

			if (!User || !User->choToken || User == Sender || User->isBlocked(ID))
				continue;

			/*if (!User->choToken) {
				PartChannel(User, (size_t(&User) - size_t(&IRCUsers[0])) / sizeof(_User*));
				continue;
			}*/

			User->addQueVector(b);

		}

	}

	void Bot_SendMessage(const std::string_view message){

		if (!ChannelCount)
			return;

		VEC(byte) b;

		PacketBuilder::Build<Packet::Server::sendMessage, '-', 'v', 's', 'i'>(b,STACK(M_BOT_NAME),&message,&ChannelName, USERID_START - 1);

		for (auto const User : IRCUsers){

			if (!User)
				continue;
			if (!User->choToken){
				PartChannel(User, GetIndex(IRCUsers, User));
				continue;
			}

			User->addQueVector(b);
		}

	}
	/*
	_Channel():IRCUsers({ 0 }) {
		ViewLevel = 0;
		WriteLevel = 0;
		NameSum = 0;
		ChannelCount = 0;
		AutoJoin = 0;
	}*/

};

std::array ChannelTable = { _Channel("#General","General Chat.", IRC_Public,IRC_Public,1),
							_Channel("#announce", "Public announcements.", IRC_Public, IRC_Admin,1),
							_Channel("#supporter", "Supporter only chat.", IRC_Supporter, IRC_Supporter,1),
							_Channel("#admin", "Command dumpster.", IRC_Admin, IRC_Admin,1),
							_Channel("#devlog", "Log all the things.", IRC_Dev, IRC_Dev, 1),
							_Channel("#lobby", "", IRC_Public, IRC_Public,0,1)
						};
VEC(byte) PublicChannelCache;

_Channel& chan_General{ChannelTable[0]};
_Channel& chan_Announce{ChannelTable[1]};
_Channel& chan_Supporter{ChannelTable[2]};
_Channel& chan_Admin{ChannelTable[3]};
_Channel& chan_DevLog{ChannelTable[4]};
_Channel& chan_Lobby{ChannelTable[5]};

_inline _Channel* GetChannelByName(u32 Name){

	for (auto& Chan : ChannelTable)
		if (Chan.NameSum == Name)
			return &Chan;

	return 0;
}
