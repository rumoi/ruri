#pragma once

enum SlotStatus
{
	Open = 1,
	Locked = 2,
	NotReady = 4,
	Ready = 8,
	NoMap = 16,
	Playing = 32,
	Complete = 64,
	HasPlayer = NotReady | Ready | NoMap | Playing | Complete,
	Quit = 128
};

struct _MatchSettings{

	std::string Name;
	std::string Password;
	u32 Mods;
	int BeatmapID;
	std::string BeatmapName;
	std::string BeatmapChecksum;

	byte MatchType;
	byte PlayMode;
	byte ScoringType;
	byte TeamType;
	byte FreeMod;

	_MatchSettings() {
		Mods = 0;
		BeatmapID = 0;
		MatchType = 0;
		PlayMode = 0;
		ScoringType = 0;
		TeamType = 0;
		FreeMod = 0;
	}

	struct StatusChange {
		union {
			struct { u64 MB : 32, TeamType : 2, MatchType : 2, ScoringType : 2, FreeMod : 1; };
			u64 Total;
		};
	};

	StatusChange StatusFlags(){

		StatusChange Ret; Ret.Total = 0;

		Ret.MB = Mods ^ u32(BeatmapID);
		Ret.TeamType = TeamType;
		Ret.MatchType = MatchType;
		Ret.ScoringType = ScoringType;
		Ret.FreeMod = FreeMod;

		return Ret;
	}

};

struct _Slot{

	_User *User;

	DWORD CurrentMods;
	byte SlotStatus;
	byte SlotTeam;

	byte Loaded : 1, Completed : 1, Failed : 1, Skipped : 1;

	_Slot(){
		User = 0;
		SlotStatus = SlotStatus::Open;
		SlotTeam = 0;
		Loaded = 0;
		Completed = 0;
		Failed = 0;
		Skipped = 0;
		CurrentMods = 0;
	}
	
	_inline void resetPlaying() {
		Loaded = 0;
		Completed = 0;
		Failed = 0;
		Skipped = 0;
	}

	void reset(){
		SlotStatus = SlotStatus::Open;
		SlotTeam = 0;
		Loaded = 0;
		Completed = 0;
		Failed = 0;
		Skipped = 0;
		CurrentMods = 0;

	}

	_Slot(_User* u) {
		User = u;
		SlotStatus = SlotStatus::Open;
		SlotTeam = 0;
		Loaded = 0;
		Completed = 0;
		Failed = 0;
		Skipped = 0;
		CurrentMods = 0;
	}
};

#define NORMALMATCH_MAX_COUNT 16
#define MULTI_MAXSIZE 16

struct _Match{
	bool Tournament;
	bool inProgress;
	bool PlayersLoading;
	std::mutex Lock;

	USHORT MatchId;
	DWORD HostID;
	_MatchSettings Settings;
	
	DWORD PlayerCount;
	std::array<_Slot, MULTI_MAXSIZE> Slots;
	int Seed;
	int LastUpdate = 0;



	template<size_t Size>
		_inline void sendUpdate(const std::array<byte,Size> &Data, const _User*const Sender = 0){

			if constexpr (Size > 0)
				for (auto& S : Slots)
					if (_User* const u = S.User;
						u && u != Sender && u->choToken)
						u->addQueArray(Data);

		}

	_inline void sendUpdateVector(const std::vector<byte>& Data, const _User* const Sender = 0){

		if (likely(Data.size()))
			for (auto& S : Slots)
				if (_User* const u = S.User;
					u && u != Sender && u->choToken)
					u->addQueVector(Data);
	}


	_inline void ClearPlaying() {

		for (auto& S : Slots){
			if (S.SlotStatus == SlotStatus::Playing)
				S.SlotStatus = SlotStatus::NotReady;
			S.resetPlaying();
		}

		inProgress = 0;
		PlayersLoading = 0;
	}

	void removeUser(_User* u, const bool Kicked){

		if (!u || !u->CurrentMatchID)
			return;
		{
			constexpr auto b = PacketBuilder::CT::String_Packet(Packet::Server::channelKicked,"#multiplayer");
			u->addQueArray(b);
		}

		u->CurrentMatchID = 0;
		PlayerCount--;

		if (!PlayerCount)
			return;

		if (HostID == u->UserID)//The host is leaving. We need to assign a new host.
			for (auto& S : Slots)
				if (_User* slotUser = S.User; slotUser && slotUser != u){
					HostID = slotUser->UserID;
					slotUser->addQueArray(PacketBuilder::CT::PacketHeader(Packet::Server::matchTransferHost));
					break;
				}

		for (auto& Slot : Slots)
			if (Slot.User == u){

				Slot.reset();
				Slot.User = 0;

				if (Kicked){
					u->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::disposeMatch,'i'>(MatchId));
					Slot.SlotStatus = SlotStatus::Locked;
				}

				if (inProgress) {//If we are leaving while playing. Check to see if we were the last user holding up everyone else to avoid an infinite wait. 

					bool AllFinished = 1;

					for(const auto& Player : Slots)
						if (Player.SlotStatus == SlotStatus::Playing
							&& !Player.Completed) {
							AllFinished = 0;
							break;
						}

					if (AllFinished){
						ClearPlaying();
						sendUpdate(PacketBuilder::CT::PacketHeader(Packet::Server::matchComplete));
					}
				}
				if (PlayersLoading) {//Same for if everyone else is waiting for us to load the map.

					bool AllLoaded = 1;

					for (const auto& Player : Slots)
						if (Player.SlotStatus == SlotStatus::Playing
							&& !Player.Loaded) {
							AllLoaded = 0;
							break;
						}

					if (AllLoaded){
						PlayersLoading = 0;
						inProgress = 1;
						sendUpdate(PacketBuilder::CT::PacketHeader(Packet::Server::matchAllPlayersLoaded));
					}
				}

				break;
			}

	}

	_inline void UnreadyUsers(){
		for(auto& Slot : Slots)
			if (Slot.SlotStatus == SlotStatus::Ready)
				Slot.SlotStatus = SlotStatus::NotReady;
		PlayersLoading = 0;
		inProgress = 0;
	}


	void Reset() {
		PlayerCount = 0;
		Tournament = 0;

		HostID = 0;
		for (auto& Slot : Slots)
			Slot = _Slot();

		inProgress = 0;
		PlayersLoading = 0;
		Seed = 0;
		Settings = _MatchSettings();
	}

	_Match(){
		MatchId = 0;

		Reset();
	}

};

std::array<_Match, MAX_MULTI_COUNT> Match;

std::tuple<std::shared_mutex,int, VEC(byte)> LobbyCache[MAX_MULTI_COUNT];

_inline _Match* getMatchFromID(const USHORT ID){
	return ID && ID < MAX_MULTI_COUNT && Match[ID - 1].PlayerCount ? &Match[ID - 1] : 0;
}

std::mutex EmptyMatchLock;

_Match* getEmptyMatch(){

	EmptyMatchLock.lock();

	for (USHORT i = 0; i < MAX_MULTI_COUNT; i++)
		if (!Match[i].PlayerCount){
			Match[i].Reset();
			Match[i].Tournament = 1;
			Match[i].PlayerCount = 1;
			Match[i].MatchId = i + 1;
			EmptyMatchLock.unlock();
			return &Match[i];
		}
	EmptyMatchLock.unlock();

	return 0;
}


namespace bPacket{


	template<const bool SendPassword = 1>
		VEC(byte) bMatch(_Match *m){

			VEC(byte) Res; Res.reserve(256);
			
			if constexpr(SendPassword)
				PacketBuilder::Build<Packet::Server::updateMatch,'w','b','b','i','s','s','s','i','s'>(Res, m->MatchId, (m->inProgress || m->PlayersLoading), m->Settings.MatchType, m->Settings.Mods, &m->Settings.Name,
					&m->Settings.Password,&m->Settings.BeatmapName, m->Settings.BeatmapID,&m->Settings.BeatmapChecksum);
			else{
				m->Settings.Password.size() ?
				PacketBuilder::Build<Packet::Server::updateMatch, 'w', 'b', 'b', 'i', 's', '-', 's', 'i', 's'>(Res, m->MatchId, (m->inProgress || m->PlayersLoading), m->Settings.MatchType, m->Settings.Mods, &m->Settings.Name,
					STACK("*"), &m->Settings.BeatmapName, m->Settings.BeatmapID, &m->Settings.BeatmapChecksum) : PacketBuilder::Build<Packet::Server::updateMatch, 'w', 'b', 'b', 'i', 's', 'b', 's', 'i', 's'>(Res, m->MatchId, (m->inProgress || m->PlayersLoading), m->Settings.MatchType, m->Settings.Mods, &m->Settings.Name,
						0, &m->Settings.BeatmapName, m->Settings.BeatmapID, &m->Settings.BeatmapChecksum);
			}

			for (const auto& Slot : m->Slots)
				Res.push_back(Slot.SlotStatus);
			for (const auto& Slot : m->Slots)
				Res.push_back(Slot.SlotTeam);

			for (const auto& Slot : m->Slots)
				if(Slot.User)
					AddStream(Res, Slot.User->UserID);

			AddStream(Res, m->HostID);
			Res.push_back(m->Settings.PlayMode);
			Res.push_back(m->Settings.ScoringType);
			Res.push_back(m->Settings.TeamType);
			Res.push_back(m->Settings.FreeMod);

			if (m->Settings.FreeMod){

				DWORD Mods[NORMALMATCH_MAX_COUNT];

				for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
					if (!m->Slots[i].User)Mods[i] = 0;
					else Mods[i] = m->Slots[i].CurrentMods;
				}

				AddMem(Res, Mods, 64);
			}
			AddStream(Res, m->Seed);

			if (SendPassword)
				m->LastUpdate = clock();

			*(DWORD*)&Res[3] = Res.size() - 7;

			return Res;
		}

}
void Event_client_matchStart(_User *tP);
const std::string ProcessCommand(_User* u, const std::string_view Command, DWORD &PrivateRes);


std::string ProcessCommandMultiPlayer(_User* u, const std::string_view Command, DWORD &PrivateRes, _Match* m) {

	if (Command.size() == 0 || Command[0] != '!')return "";

	const DWORD Priv = u->privileges;
	PrivateRes = 1;

	const auto Split = Explode_View(Command, ' ',8);

	if (Split[0] == "!mp"){

		if (Split.size() < 2)return "";


		if (Split[1] == "here") {
			if (!(Priv & (Privileges::AdminDev | Privileges::UserTournamentStaff)))
				goto INSUFFICIENTPRIV;

			if (Split.size() != 3)
				return "!mp here <username>";

			_UserRef Target(GetUserFromNameSafe(USERNAMESAFE(std::string(Split[2]))), 0);

			if (!Target)
				return "Coult not find user";

			std::scoped_lock L(m->Lock);

			for (auto& Slot : m->Slots)
				if (Slot.User == Target.User)return "They are already here.";

			for (auto& Slot : m->Slots){
				if (!Slot.User){
					Slot.reset();
					Slot.User = Target.User;
					Slot.SlotStatus = SlotStatus::NotReady;

					auto MatchData = bPacket::bMatch(m);

					m->sendUpdateVector(MatchData, Target.User);

					Target->CurrentMatchID = GetIndex(m->Slots, Slot);

					*(USHORT*)MatchData.data() = (USHORT)Packet::Server::matchJoinSuccess;

					Target->addQueVector(MatchData);

					m->PlayerCount++;
					break;
				}
			}

			return Target->ProfileLink() + " forced into match";
		}

		if (Split[1] == "host"){
			if (!(Priv & (Privileges::AdminDev | Privileges::UserTournamentStaff)))
				goto INSUFFICIENTPRIV;

			m->Lock.lock();
			
			if (m->HostID == u->UserID || m->inProgress) {
				m->Lock.unlock();
				return "You are already host.";
			}

			m->HostID = u->UserID;
			
			u->addQueArray(PacketBuilder::CT::PacketHeader(Packet::Server::matchTransferHost));

			m->UnreadyUsers();
			m->sendUpdateVector(bPacket::bMatch(m));

			m->Lock.unlock();
			PrivateRes = 0;
			return u->ProfileLink() + " has forced host upon them self.";
		}

		if (Split[1] == "start") {

			if (std::scoped_lock L(m->Lock);1){//Could check the host before the lock.

				if (m->HostID != u->UserID)
					return "Only the host can force start the match.";				

				if (m->Settings.BeatmapID == -1)
					return "Starting a match without a map being selected is hard.";
			}
			Event_client_matchStart(u);
			PrivateRes = 0;
			return "Host has forced the match to start.";
		}

		if (Split[1] == "id")
			return "The ID of this match is " + std::to_string(m->MatchId) + " with the host being " + GetUsernameFromCache(m->HostID);
		
		return "";
	}

	return ProcessCommand(u, Command, PrivateRes);//Moving is not worth it.

INSUFFICIENTPRIV:return "You are not allowed to use that command.";
}