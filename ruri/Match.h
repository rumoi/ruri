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
	DWORD Mods;
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
};

struct _Slot {

	_User *User;
	byte SlotStatus;
	byte SlotTeam;
	bool Loaded;
	bool Completed;
	bool Failed;
	bool Skipped;
	DWORD CurrentMods;

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

/*
#define SERVERMATCH_PAGES 3
#define SERVERMATCH_PERPAGE 14

#define SERVERMATCH_MAX 2 + (SERVERMATCH_PERPAGE * SERVERMATCH_PAGES)//15 on first page then 14 on every other page*/

struct _Match{
	bool Tournament;
	bool inProgress;
	bool PlayersLoading;
	std::mutex Lock;

	USHORT MatchId;
	DWORD HostID;
	_MatchSettings Settings;
	
	DWORD PlayerCount;
	_Slot Slot[NORMALMATCH_MAX_COUNT];

	int Seed;

	_BanchoPacket LobbyCache;

	_inline void sendUpdate(const _BanchoPacket &b, const _User* Sender = 0) {
		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
			_User *u = Slot[i].User;
			if (u && u != Sender && u->choToken)
				u->addQue(b);
		}
	}

	_inline void sendUpdates(const VEC(_BanchoPacket) &&b, const _User* Sender = 0){

		if (b.size() == 0)return;
		if (b.size() == 1)
			return sendUpdate(_M(b[0]), Sender);

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
			_User *u = Slot[i].User;
			if (u && u != Sender && u->choToken)
				u->addQue(b);
		}
	}

	_inline void ClearPlaying() {

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++) {
			if (Slot[i].SlotStatus == SlotStatus::Playing)
				Slot[i].SlotStatus = SlotStatus::NotReady;

			Slot[i].Completed = 0;
			Slot[i].Skipped = 0;
			Slot[i].Loaded = 0;
		}

		inProgress = 0;
		PlayersLoading = 0;
	}

	void removeUser(_User* u, const bool Kicked){

		if (!u || !u->CurrentMatchID)return;

		u->addQue(bPacket::GenericString(OPac::server_channelKicked, "#multiplayer"));

		u->CurrentMatchID = 0;
		PlayerCount--;

		if (PlayerCount == 0)
			return;

		if (HostID == u->UserID){

			for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){

				_User* slotUser = Slot[i].User;

				if (slotUser && slotUser != u) {
					HostID = slotUser->UserID;
					slotUser->addQue(_BanchoPacket(OPac::server_matchTransferHost));
					break;
				}
			}
		}

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
			if (Slot[i].User == u){

				Slot[i].User = 0;

				if (Kicked){
					u->addQue(bPacket4Byte(OPac::server_disposeMatch,MatchId));
					Slot[i].SlotStatus = SlotStatus::Locked;
				}else{
					if(PlayerCount == 0)
						u->addQue(bPacket4Byte(OPac::server_disposeMatch, MatchId));
					Slot[i].SlotStatus = SlotStatus::Open;
				}
				Slot[i].CurrentMods = 0;

				if (inProgress){//If we are leaving while playing. Check to see if we were the last user holding up everyone else to avoid an infinite wait. 

					bool AllFinished = 1;

					for (DWORD z = 0; z < NORMALMATCH_MAX_COUNT; z++)
						if (Slot[z].SlotStatus == SlotStatus::Playing && Slot[z].Completed == 0) {
							AllFinished = 0;
							break;
						}

					if (AllFinished) {
						ClearPlaying();
						sendUpdate(_BanchoPacket(OPac::server_matchComplete));
					}
				}

				if (PlayersLoading){//Same thing for match loading.

					bool AllLoaded = 1;

					for (DWORD z = 0; z < NORMALMATCH_MAX_COUNT; z++) {
						if (Slot[z].SlotStatus == SlotStatus::Playing && Slot[z].Loaded == 0) {
							AllLoaded = 0;
							break;
						}
					}

					if (AllLoaded){
						PlayersLoading = 0;
						inProgress = 1;
						sendUpdate(_BanchoPacket(OPac::server_matchAllPlayersLoaded));
					}
				}

				return;
			}

		}

	}

	_inline void UnreadyUsers(){
		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			if (Slot[i].SlotStatus == SlotStatus::Ready)
				Slot[i].SlotStatus = SlotStatus::NotReady;

		PlayersLoading = 0;
		inProgress = 0;
	}

	_Match(){
		MatchId = 0;
		PlayerCount = 0;
		Tournament = 0;

		HostID = 0;

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++) {
			Slot[i].User = 0;
			Slot[i].SlotStatus = SlotStatus::Open;
			Slot[i].SlotTeam = 0;
			Slot[i].CurrentMods = 0;
		}
		inProgress = 0;
		PlayersLoading = 0;
		Seed = 0;
	}
	void Reset() {
		PlayerCount = 0;
		Tournament = 0;

		HostID = 0;

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++) {
			Slot[i].User = 0;
			Slot[i].SlotStatus = SlotStatus::Open;
			Slot[i].SlotTeam = 0;
			Slot[i].CurrentMods = 0;
		}
		inProgress = 0;
		PlayersLoading = 0;
		Seed = 0;
		Settings = _MatchSettings();
	}


};

_Match Match[MAX_MULTI_COUNT];

/*
struct _CommunityMatch{
	
	float MinStar, MaxStar;
	_MatchSettings Settings;
	std::vector<_User*> User;

	_CommunityMatch(){
		MinStar = 0.5f;
		MaxStar = 10.f;
		User.clear();
	}

};*/

#define COMMUNITY_MATCH_COUNT 1
//_CommunityMatch communityMatch[COMMUNITY_MATCH_COUNT];

_Match* getMatchFromID(const USHORT ID) {

	if (ID == 0 || ID >= MAX_MULTI_COUNT)return 0;

	if (Match[ID - 1].PlayerCount)//TODO make sure this is thread safe.
		return &Match[ID - 1];

	return 0;
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


namespace bPacket {

	_BanchoPacket bMatch(USHORT Packet, _Match *m, bool SendPassword) {

		_BanchoPacket b(Packet);

		AddShort(b.Data, m->MatchId);
		b.Data.push_back((m->inProgress || m->PlayersLoading));
		b.Data.push_back(m->Settings.MatchType);
		AddInt(b.Data, m->Settings.Mods);
		AddString(b.Data, m->Settings.Name);

		if(SendPassword)AddString(b.Data, m->Settings.Password);
		else (m->Settings.Password.size()) ? AddString(b.Data, "*") : AddString(b.Data, "");		

		AddString(b.Data, m->Settings.BeatmapName);
		AddInt(b.Data, m->Settings.BeatmapID);
		AddString(b.Data, m->Settings.BeatmapChecksum);

		b.Data.resize(b.Data.size() + (NORMALMATCH_MAX_COUNT * 2));

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			b.Data[b.Data.size() - ((NORMALMATCH_MAX_COUNT * 2) - i)] = m->Slot[i].SlotStatus;
		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			b.Data[b.Data.size() - (NORMALMATCH_MAX_COUNT - i)] = m->Slot[i].SlotTeam;

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
			if (m->Slot[i].User)
				AddInt(b.Data, m->Slot[i].User->UserID);
		}

		AddInt(b.Data, m->HostID);
		b.Data.push_back(m->Settings.PlayMode);
		b.Data.push_back(m->Settings.ScoringType);
		b.Data.push_back(m->Settings.TeamType);
		b.Data.push_back(m->Settings.FreeMod);

		if (m->Settings.FreeMod){
			DWORD Mods[NORMALMATCH_MAX_COUNT];
			for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++){
				if (!m->Slot[i].User)Mods[i] = 0;
				else Mods[i] = m->Slot[i].CurrentMods;
			}
			AddMem(b.Data, Mods, 64);
		}
		AddInt(b.Data, m->Seed);

		if (!SendPassword)
			m->LobbyCache = b;
		else m->LobbyCache.Type = 0;

		return b;
	}

}
void Event_client_matchStart(_User *tP);
const std::string ProcessCommand(_User* u, const std::string& Command, DWORD &PrivateRes);


std::string ProcessCommandMultiPlayer(_User* u, const std::string &Command, DWORD &PrivateRes, _Match* m) {

	if (Command.size() == 0 || Command[0] != '!')return "";

	const DWORD Priv = u->privileges;
	PrivateRes = 1;

	const auto Split = Explode_View(Command, ' ',8);

	if (Split[0] == "!mp"){

		if (Split.size() < 2)return "";

		if (Split[1] == "host"){
			if (!(Priv & (Privileges::AdminDev | Privileges::UserTournamentStaff)))
				goto INSUFFICIENTPRIV;

			m->Lock.lock();
			
			if (m->HostID == u->UserID || m->inProgress) {
				m->Lock.unlock();
				return "You are already host.";
			}

			m->HostID = u->UserID;
			
			u->addQue(_BanchoPacket(OPac::server_matchTransferHost));
			m->UnreadyUsers();
			m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));

			m->Lock.unlock();
			PrivateRes = 0;
			return u->Username + " has forced host upon them self.";
		}

		if (Split[1] == "start") {
			m->Lock.lock();//Could check the host before the lock.

			if (m->HostID != u->UserID) {
				m->Lock.unlock();
				return "Only the host can force start the match.";
			}

			if (m->Settings.BeatmapID == -1){
				m->Lock.unlock();
				return "Starting a match without a map being selected is hard.";
			}

			m->Lock.unlock();

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