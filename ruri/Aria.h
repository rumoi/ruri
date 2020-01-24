#pragma once

#include "aes.h"
#include "Base64.h"
#define ModuleName "Aria"

struct _Score {

	std::string UserName;
	std::string BeatmapHash;
	int Score;
	DWORD Mods;
	USHORT count300;
	USHORT count100;
	USHORT count50;
	USHORT countGeki;
	USHORT countKatu;
	USHORT countMiss;
	USHORT MaxCombo;
	bool FullCombo;
	std::string Rank;
	byte GameMode;

	float GetAcc() const {
		int TotalHits = count300 + count100 + count50 + countMiss;

		if (TotalHits == 0)return 0.f;

		return float(float(count50 * 50 + count100 * 100 + count300 * 300) / (TotalHits * 300)) * 100;
	}
	_Score() {

		Score = 0;
		Mods = 0;
		count300 = 0;
		count100 = 0;
		count50 = 0;
		countGeki = 0;
		countKatu = 0;
		countMiss = 0;
		MaxCombo = 0;
		FullCombo = 0;
		GameMode = 0;

	}
};

struct _ScoreCache{

	DWORD UserID;
	uint64_t ScoreID;
	int Score;
	int Time;
	DWORD Mods;
	USHORT count300;
	USHORT count100;
	USHORT count50;
	USHORT countGeki;
	USHORT countKatu;
	USHORT countMiss;
	USHORT MaxCombo;
	float pp;
	bool FullCombo;
	byte GameMode;
	bool Loved;

	float GetAcc() const {
		int TotalHits = count300 + count100 + count50 + countMiss;

		if (TotalHits == 0)return 0.f;

		return float(float(count50 * 50 + count100 * 100 + count300 * 300) / (TotalHits * 300)) * 100;
	}

	_ScoreCache(const _Score &s, const DWORD UID, const float PP){
		ScoreID = 0;
		UserID = UID;
		pp = PP;
		GameMode = s.GameMode;
		Score = s.Score;
		Mods = s.Mods;
		count300 = s.count300;
		count100 = s.count100;
		count50 = s.count50;
		countMiss = s.countMiss;
		countGeki = s.countGeki;
		countKatu = s.countKatu;
		MaxCombo = s.MaxCombo;
		FullCombo = s.FullCombo;
		Loved = 0;
		Time = time(0);
	}

	_ScoreCache() {
		UserID = 0;
		ScoreID = 0;
		GameMode = 0;
		Loved = 0;
		Score = 0;
		Time = 0;
		Mods = 0;
		count300 = 0;
		count100 = 0;
		count50 = 0;
		countGeki = 0;
		countKatu = 0;
		countMiss = 0;
		MaxCombo = 0;
		FullCombo = 0;
		pp = 0;
	}

	//id,score,max_combo,50_count,100_count,300_count,misses_count,katus_count,gekis_count,full_combo,mods,userid,pp,time
	_ScoreCache(sql::ResultSet *r, const byte GM){
		GameMode = GM;
		Loved = 0;
		ScoreID = r->getInt64(1);
		Score = r->getInt(2);
		MaxCombo = r->getInt(3);
		count50 = r->getInt(4);
		count100 = r->getInt(5);
		count300 = r->getInt(6);
		countMiss = r->getInt(7);
		countKatu = r->getInt(8);
		countGeki = r->getInt(9);
		FullCombo = r->getBoolean(10);
		Mods = DWORD(r->getInt(11));
		UserID = r->getInt(12);
		pp = r->getDouble(13);
		Time = r->getInt(14);
	}

};

bool SortScoreCacheByScore(const _ScoreCache &i, const _ScoreCache &j) { return (i.Score > j.Score); }
bool SortScoreCacheByPP(const _ScoreCache &i, const _ScoreCache &j) { return (i.pp > j.pp); }

void AppendScoreToString(std::string *s,const DWORD Rank,_ScoreCache& Score, const bool New) {

	if (!s)return;

	const std::string Suffix = New ? "After" : "Before";

	*s += "|rank" + Suffix + ": " + std::to_string(Rank)
	    + "|maxCombo" + Suffix + ": " + std::to_string(Score.MaxCombo)
		+ "|accuracy" + Suffix + ": " + std::to_string(Score.GetAcc())
		+ "|accuracy" + Suffix + ": " + std::to_string(Score.GetAcc())
		+ "|rankedScore" + Suffix + ": " + std::to_string(Score.Score)
		+ "|totalScore" + Suffix + ": " + std::to_string(Score.Score)
		+ "|pp" + Suffix + ": " + std::to_string(Score.pp);
}

struct _LeaderBoardCache{
	DWORD PlayCount;
	DWORD PassCount;

	std::shared_mutex ScoreLock;
	std::vector<_ScoreCache> ScoreCache;

	_LeaderBoardCache() {
		PlayCount = 0;
		PassCount = 0;
	}

	template<bool DontLock = 0>
	DWORD GetRankByUID(const DWORD UID){

		DWORD Rank = 0;

		if constexpr(!DontLock)
			ScoreLock.lock_shared();

		for (DWORD i = 0; i < ScoreCache.size(); i++){
			if (ScoreCache[i].UserID == UID) {
				Rank = i + 1;
				break;
			}
		}
		if constexpr(!DontLock)
			ScoreLock.unlock_shared();

		return Rank;
	}
	_ScoreCache GetScoreByRank(const DWORD Rank){

		_ScoreCache s;

		if (Rank != 0){
			ScoreLock.lock_shared();

			if (Rank <= ScoreCache.size())
				s = ScoreCache[Rank - 1];

			ScoreLock.unlock_shared();
		}
		return s;
	}
	_ScoreCache GetScoreByUID(const DWORD UID) {

		_ScoreCache s;

		if (UID == 0)
			return s;

		ScoreLock.lock_shared();

		for (const auto& SC : ScoreCache) {
			if (SC.UserID == UID) {
				s = SC;
				break;
			}
		}
		ScoreLock.unlock_shared();

		return s;
	}

	bool AddScore(_ScoreCache &s, const DWORD BID, const std::string &MapName, std::string MD5, _SQLCon *SQL = 0, std::string *Ret = 0) {//Should really clean this up.
		
		PlayCount++;
		PassCount++;//dont really care about this getting malformed..

		bool Done = 0;
		bool NewTop = 0;

		MD5 = REMOVEQUOTES(MD5);

		const bool TableOffset = (RELAX_MODE && s.Mods & Relax);

		if (MD5.size() != 32){
			printf("AddScore Incorrect md5 length\n");//should never happen
			return 0;
		}

		ScoreLock.lock();
		
		int LastRank = 0;
		_ScoreCache LastScore;

		if (s.Loved)
			s.Score = int(s.pp + 0.5f);

		VEC(_SQLKey) ScoreInsert = {
			_SQLKey("beatmap_md5", std::string(MD5)),
			_SQLKey("userid", s.UserID),
			_SQLKey("score", s.Score),
			_SQLKey("max_combo", s.MaxCombo),
			_SQLKey("full_combo", s.FullCombo),
			_SQLKey("mods", s.Mods),
			_SQLKey("300_count", s.count300),
			_SQLKey("100_count", s.count100),
			_SQLKey("50_count", s.count50),
			_SQLKey("katus_count", s.countKatu),
			_SQLKey("gekis_count", s.countGeki),
			_SQLKey("misses_count", s.countMiss),
			_SQLKey("time", s.Time),
			_SQLKey("play_mode", s.GameMode),
			_SQLKey("accuracy", std::to_string(s.GetAcc())),
			_SQLKey("pp", (!s.Loved) ? std::to_string(s.pp) : "0.001")
		};

		auto& CompletedValue = ScoreInsert.emplace_back("completed", 3);

		for (DWORD i = 0; i < ScoreCache.size(); i++){
			if (ScoreCache[i].UserID == s.UserID){
				LastScore = ScoreCache[i];
				Done = 1;
				if ((!s.Loved) ? (ScoreCache[i].pp < s.pp) : (ScoreCache[i].Score < s.Score)){

					if (SQL){

						std::unique_ptr<sql::ResultSet> res{(sql::ResultSet*)0};

						mlock (SQL->Lock){

							SQL->ExecuteUPDATE("UPDATE " + Score_Table_Name[TableOffset] + " SET completed = 2 WHERE id = " + std::to_string(ScoreCache[i].ScoreID) + " LIMIT 1", 1);

							SQL->ExecuteUPDATE(SQL_INSERT(Score_Table_Name[TableOffset], ScoreInsert), 1);

							res.reset(SQL->ExecuteQuery("SELECT LAST_INSERT_ID() FROM " + Score_Table_Name[TableOffset], 1));

						}

						if (res && res->next())
							s.ScoreID = res->getInt64(1);

					}
					ScoreCache[i] = s;
					NewTop = 1;
					LastRank = i + 1;
				}else{
					CompletedValue.Value = "2";
					SQL->ExecuteUPDATE(SQL_INSERT(Score_Table_Name[TableOffset], ScoreInsert));
				}
				break;
			}
		}
		if (!Done){

			SQL->Lock.lock();

			SQL->ExecuteUPDATE(SQL_INSERT(Score_Table_Name[TableOffset], _M(ScoreInsert)), 1);

			auto res = SQL->ExecuteQuery("SELECT LAST_INSERT_ID() FROM " + Score_Table_Name[TableOffset], 1);

			SQL->Lock.unlock();

			if (res && res->next())
				s.ScoreID = res->getUInt64(1);

			DeleteAndNull(res);

			ScoreCache.push_back(s);
			NewTop = 1;
		}
		#undef TableName

		if(NewTop)
			std::sort(ScoreCache.begin(), ScoreCache.end(), (RELAX_MODE && (s.Mods & Relax)) ? SortScoreCacheByPP : SortScoreCacheByScore);//Could optimize this by bounding it. Will do it if this ever becomes a problem (doubt it)

		const DWORD NewRank = (!NewTop) ? 0 : GetRankByUID<true>(s.UserID);
		
		ScoreLock.unlock();

		if (!s.Loved && NewRank == 1 && ScoreCache.size() > 5)
			chan_Announce.Bot_SendMessage(
				"[https://osu.ppy.sh/u/" + std::to_string(s.UserID) + " " + GetUsernameFromCache(s.UserID) + "] has achieved #1 on [https://osu.ppy.sh/b/"+ std::to_string(BID) +" "+ MapName +"] ( " + RoundTo2(s.pp) + "pp )");

		if (Ret){
			AppendScoreToString(Ret, LastRank, LastScore, 0);
			AppendScoreToString(Ret, NewRank, s, 1);
			*Ret += "|onlineScoreId: " + std::to_string(s.ScoreID);
		}

		return NewTop;
	}

};

//I know this is sub optimal but ripple did not iterate to include beatmap sets properly
//Which means that a lot of this is hacky just for a single feature (updateable vs un-submited). I would change the ripple API its self but that would require me to edit a lot more.

struct _BeatmapData{

	DWORD BeatmapID;
	DWORD SetID;
	float Rating;
	std::string DisplayTitle;
	std::string DiffName;
	std::string Hash;
	std::array<_LeaderBoardCache*, GM_MAX + 1> lBoard;
	int RankStatus;

	struct cAtomic {
		std::atomic<int> v;
		cAtomic(const cAtomic& o) {
			v.store(o.v.load());
		}
		cAtomic(const int v) : v{ v } {};
	} rCount;

	~_BeatmapData(){

		rCount.v--;

		while(rCount.v > 0)//This should be fine.
			Sleep(0);

		BeatmapID = 0;
		SetID = 0;
		Rating = 0;

		for (auto& i : lBoard){
			DeleteAndNull(i);
		}

	}

	_BeatmapData() : rCount(1){
		BeatmapID = 0;
		SetID = 0;
		RankStatus = RankStatus::UNKNOWN;
		Rating = 0.f;
		ZeroMemory(lBoard.data(), sizeof(lBoard));
	}

	_BeatmapData(const int RankStatus) : RankStatus(RankStatus), rCount(0){
		BeatmapID = 0;
		SetID = 0;
		Rating = 0.f;
		ZeroMemory(lBoard.data(), sizeof(lBoard));
	}

	_LeaderBoardCache* GetLeaderBoard(const DWORD Mode, _SQLCon* const SQL){

		if (Mode > GM_MAX)
			return 0;

		if (lBoard[Mode])
			return lBoard[Mode];

		if (SQL){
			
			const bool TableOffset = (RELAX_MODE && Mode >= 4);

			const std::string OrderBy = TableOffset ? "pp" : "score";

			_LeaderBoardCache* L = new _LeaderBoardCache();
			L->ScoreCache.reserve(64);

			const DWORD GM = al_min(GM_MAX, Mode);

			if (auto * res = SQL->ExecuteQuery("SELECT id,score,max_combo,50_count,100_count,300_count,misses_count,katus_count,gekis_count,full_combo,mods,userid,pp,time FROM " + Score_Table_Name[TableOffset] + " WHERE completed = 3 AND beatmap_md5 = '" + Hash + "' AND play_mode = " + std::to_string(Mode % 4) + " AND pp > 0 ORDER BY " + OrderBy + " DESC");
				res) {
				while (res->next())
					L->ScoreCache.emplace_back(res, GM);
			}
			if (likely(lBoard[Mode] == 0))
				lBoard[Mode] = L;
			else delete L;//Another request beat us to it.

			return lBoard[Mode];
		}
		return 0;
	}

	bool AddScore(const DWORD Mode, _ScoreCache &s, _SQLCon *Con, std::string* Ret) {

		if (RankStatus < RankStatus::RANKED || s.pp == 0.f)return 0;

		auto* L = GetLeaderBoard(Mode, Con);

		return L ? L->AddScore(s, BeatmapID, DisplayTitle, Hash, Con,Ret) : 0;
	}

};


struct _BeatmapDataRef {

	_BeatmapData* Ref = 0;

	~_BeatmapDataRef() {
		if (Ref)
			Ref->rCount.v--;
		Ref = 0;
	}

};

struct _BeatmapSet{

	u32 ID;
	u32 LastUpdate;
	std::vector<_BeatmapData> Maps;
	std::shared_mutex MapUpdateLock;
	bool Deleted;

	_BeatmapSet(){
		LastUpdate = 0;
		ID = 0;
		Deleted = 0;
		Maps.reserve(16);
	}

	_BeatmapSet(const DWORD SetID){
		ID = SetID;
		LastUpdate = 0;
		Deleted = 0;
		Maps.reserve(16);
	}

	_BeatmapSet(const DWORD SetID, const bool Del) {
		ID = SetID;
		LastUpdate = 0;
		Deleted = Del;
		Maps.reserve(16);
	}

	~_BeatmapSet(){
		ID = 0;
		LastUpdate = 0;
		Deleted = 0;
	}

};

_BeatmapSet* BeatmapSet_Cache[2097152] = {};

template<u32 Key>
_inline std::string_view JsonListGet(const std::vector<std::pair<int, std::string_view>> &List){

	for (const auto [K, V] : List)
		if (K == Key)
			return V;

	return std::string_view();
}

void ExtractDiffName(std::string &SRC){

	DWORD Start = 0;
	DWORD End = 0;

	for (DWORD i = SRC.size(); i > 0; i--) {
		if (!End) {
			if (SRC[i - 1] == ']')
				End = i - 1;
		}else{
			if (SRC[i-1] == '[') {
				Start = i;
				break;
			}
		}
	}
	if (Start && End)//TODO: Don't.
		SRC = std::string(SRC.begin() + Start, SRC.begin() + End);
	else SRC.clear();
}


void FileNameClean(std::string &S){
	std::string Return;
	Return.reserve(S.size());
	
	const char Banned[] = { '/','\\',':','*','?','"', '<','>','|' };
	
	for (DWORD i = 0; i < S.size();i++) {

		bool Add = 1;

		for (size_t z = 0; z < sizeof(Banned); z++) {
			if (S[i] == Banned[z]) {
				Add = 0;
				break;
			}
		} 

		if (Add)
			Return.push_back(S[i]);
	}
	S = Return;
}


//Calling GetBeatmapSetFromSetID can collect setID information from the osu API if it misses in the database
_BeatmapSet *GetBeatmapSetFromSetID(const DWORD SetID, _SQLCon* SQLCon, bool ForceUpdate = 0){

	if ((!SetID || SetID >= aSize(BeatmapSet_Cache)))
		return 0;

	if (!ForceUpdate && BeatmapSet_Cache[SetID])
		return BeatmapSet_Cache[SetID];

	if (!SQLCon && !ForceUpdate)
		return 0;

	//Set ID is not in the cache. We need to add it.
	
	_BeatmapSet* Set = BeatmapSet_Cache[SetID] ? BeatmapSet_Cache[SetID] : new _BeatmapSet();

	std::scoped_lock<std::shared_mutex> L(Set->MapUpdateLock);

	Set->ID = SetID;

	BeatmapSet_Cache[SetID] = Set;

	Set->Maps.clear();

	const auto GetMapData = [&](_SQLCon* SQL)->bool{

		sql::ResultSet* res = SQL ? SQL->ExecuteQuery("SELECT ranked, beatmap_id, song_name, rating, beatmap_md5 FROM beatmaps WHERE beatmapset_id = " + std::to_string(SetID)) : 0;

		bool Ret = 0;

		if (res && res->next()){
			Ret = 1;

			do{

				auto& l = Set->Maps.emplace_back();

				l.SetID = SetID;

				l.RankStatus = res->getInt(1);

				l.BeatmapID = res->getInt(2);

				l.DisplayTitle = res->getString(3);
				l.DiffName = l.DisplayTitle;

				ExtractDiffName(l.DiffName);
				FileNameClean(l.DiffName);
				ReplaceAll(l.DisplayTitle, "\n", "|");

				l.Rating = res->getDouble(4);
				l.Hash = res->getString(5);

				if (l.Hash.size() != 32)
					Set->Maps.pop_back();

			} while (res->next());

		}
		DeleteAndNull(res);

		return Ret;
	};

	if (!ForceUpdate && GetMapData(SQLCon))
		return Set;

	if (SQLCon){//Failed - Or requesting an updated version.

		LogMessage("Getting beatmap data from the osu!API", ModuleName);

		const std::string ApiRes = GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "s=" + std::to_string(SetID));

		if (ApiRes.size()){
			
			const auto BeatmapData = JsonListSplit(ApiRes,48);

			if (BeatmapData.size()){

				VEC(DWORD) AddedMaps;
				AddedMaps.reserve(BeatmapData.size());

				for (auto& MapData : BeatmapData){

					std::string MD5 = std::string(JsonListGet<"file_md5"_HU>(MapData));

					{
						for (char& c : MD5)
							if (c == '\'')
								c = ' ';
					}

					const u32 beatmap_id = STN<u32>(JsonListGet<"beatmap_id"_HU>(MapData));

					if (beatmap_id && MD5.size() == 32){

						float diff_size = STN<float>(JsonListGet<"diff_size"_HU>(MapData)),
							  diff_overall = STN<float>(JsonListGet<"diff_overall"_HU>(MapData)),
							  diff_approach = STN<float>(JsonListGet<"diff_approach"_HU>(MapData));

						int Length = STN<int>(JsonListGet<"hit_length"_HU>(MapData)),
							RankedStatus = STN<int>(JsonListGet<"approved"_HU>(MapData)),
							MaxCombo = STN<int>(JsonListGet<"max_combo"_HU>(MapData)),
							BPM = STN<int>(JsonListGet<"bpm"_HU>(MapData));

						const byte mode = StringToNum(byte, JsonListGet<"mode"_HU>(MapData));

						std::string Title = std::string(JsonListGet<"artist"_HU>(MapData)) +
							" - " + std::string(JsonListGet<"title"_HU>(MapData)) +
							" [" + std::string(JsonListGet<"version"_HU>(MapData)) + "]";

						FileNameClean(Title);
						ReplaceAll(Title, "'", "''");

						enum {osuapi_graveyard = -2, osuapi_WIP = -1, osuapi_pending = 0, osuapi_ranked = 1, osuapi_approved = 2, osuapi_qualified = 3, osuapi_loved = 4};

						if (RankedStatus == osuapi_ranked || RankedStatus == osuapi_qualified || RankedStatus == osuapi_approved)
							RankedStatus = RANKED;
						else if (RankedStatus == osuapi_loved)
							RankedStatus = LOVED;

						auto AlreadyThere = SQLCon->ExecuteQuery("SELECT id, ranked FROM beatmaps WHERE beatmap_id = " + std::to_string(beatmap_id) + " LIMIT 1");						

						if (AlreadyThere && AlreadyThere->next()) {

							const int oldStatus = AlreadyThere->getInt(2);

							if (oldStatus == RANKED) RankedStatus = RANKED;//Never unrank a beatmap.
							else if (oldStatus == LOVED) RankedStatus = LOVED;
							
							SQLCon->ExecuteUPDATE(SQL_SETUPDATE("beatmaps", {
								_SQLKey("beatmap_md5", _M(MD5)),
								_SQLKey("song_name", _M(Title)),
								_SQLKey("ar",std::to_string(diff_approach)),
								_SQLKey("od",std::to_string(diff_overall)),
								_SQLKey("max_combo",MaxCombo),
								_SQLKey("hit_length",Length),
								_SQLKey("bpm",BPM),
								_SQLKey("ranked",RankedStatus)
							}, "id=" + AlreadyThere->getString(1) + " LIMIT 1"));
							
						}else
							SQLCon->ExecuteUPDATE(SQL_INSERT("beatmaps", {
								_SQLKey("beatmap_id", beatmap_id),
								_SQLKey("beatmapset_id", SetID),
								_SQLKey("beatmap_md5", _M(MD5)),
								_SQLKey("song_name", _M(Title)),
								_SQLKey("ar", std::to_string(diff_approach)),
								_SQLKey("od", std::to_string(diff_overall)),
								_SQLKey("mode", std::to_string(mode)),
								_SQLKey("difficulty_std", 0),
								_SQLKey("difficulty_taiko", 0),
								_SQLKey("difficulty_ctb", 0),
								_SQLKey("difficulty_mania", 0),
								_SQLKey("max_combo", MaxCombo),
								_SQLKey("hit_length",Length),
								_SQLKey("bpm", BPM),
								_SQLKey("ranked", RankedStatus),
								_SQLKey("latest_update", 0),
								_SQLKey("ranked_status_freezed", 0)
							}));

						DeleteAndNull(AlreadyThere);

						AddedMaps.push_back(beatmap_id);
					}

				}

				std::string NewMaps;

				for (const DWORD bID : AddedMaps)
					NewMaps += std::to_string(bID) + ",";

				int DeletedDiffs = 0;

				if (NewMaps.size()) {
					NewMaps.pop_back();
					DeletedDiffs = SQLCon->ExecuteUPDATE("DELETE FROM beatmaps WHERE beatmapset_id = " + std::to_string(SetID) +
														" AND beatmap_id NOT IN (" + NewMaps + ")");
				}else DeletedDiffs = SQLCon->ExecuteUPDATE("DELETE FROM beatmaps WHERE beatmapset_id = " + std::to_string(SetID));


				const std::string CountString = std::to_string(DeletedDiffs) + " diffs removed.";
				LogMessage(CountString.c_str(), ModuleName);

				LogMessage("Got data. Attempting grab again.", ModuleName);

				if (GetMapData(SQLCon)) LogMessage("Grab success.", ModuleName);
				else     LogMessage("Grab fail.", ModuleName);

			}else{

				printf("SetID %i has been removed from the osu server\n", SetID);
				
				Set->Deleted = 1;

				/*

				//The set does not exist on the osu server. Cache this fact.
				auto it = MapSet->find(SetID);

				if (it == MapSet->end())
					MapSet->insert(std::pair<const DWORD, _BeatmapSet*>(SetID, &BeatmapSetDeleted));
				else (*it).second->ID = 0;

				//TODO: Might want to unrank beatmaps in this state.
				*/
			}

		}

	}

	return Set;
}

_BeatmapData BeatmapNotSubmitted(RankStatus::UNKNOWN);


bool CheckMapUpdate(_BeatmapData *BD, _SQLCon* SQL){

	if (!BD || (!BD->SetID || BD->SetID >= aSize(BeatmapSet_Cache)) || BD->Hash.size() != 32 || !SQL)
		return 0;

	_BeatmapSet *BS = BeatmapSet_Cache[BD->SetID];
	
	if (!BS)
		return 0;

	const DWORD cTime = clock_ms();

	if (BS->LastUpdate + 14400000 > cTime)
		return 0;

	BS->LastUpdate = cTime;
	GetBeatmapSetFromSetID(BD->SetID, SQL, 1);

	return 1;
}


_BeatmapData* GetBeatmapCache(const DWORD SetID, const DWORD BID,const std::string_view MD5, std::string &&DiffName, _SQLCon* SQL, _BeatmapDataRef &REF){

	ExtractDiffName(DiffName);

	bool ValidMD5 = (MD5.size() == 32);
	bool DiffNameGiven = (DiffName.size() > 0);

	_BeatmapSet *BS = (SetID && SetID < aSize(BeatmapSet_Cache)) ? BeatmapSet_Cache[SetID] : 0;

	if (BS && BS->Deleted)
		return 0;

	if (!BS)
		BS = GetBeatmapSetFromSetID((SetID) ? SetID : getSetID_fHash(std::string(MD5), SQL), SQL);

	if (BS){

	int Tries = 0;

	while (Tries < 2){
		if (!BS->ID)
			return &BeatmapNotSubmitted;

		s_mlock (BS->MapUpdateLock){

			for (auto& Map : BS->Maps){
				if ((ValidMD5 && Map.Hash == MD5) || (BID && Map.BeatmapID == BID) ||
					(DiffNameGiven && Map.DiffName == DiffName)){//TODO: check if the servers md5 is out of date.
					Map.rCount.v++;//Should be safe
					REF.Ref = &Map;
					return &Map;
				}				
			}
		}

		const u32 cTime = clock_ms();

		if (BS->LastUpdate + 14400000 < cTime) {//Rate limit to every 4 hours
			BS->LastUpdate = cTime;

			LogMessage("Possible update to beatmap set.", ModuleName);

			BS = GetBeatmapSetFromSetID(SetID, SQL,1);
			if (!BS)
				break;
		}
		Tries++;
	}

	}else printf("Could not find the map at all.\n");

	//Worst case scenario. The beatmap is not in the database and the osuapi can not be reached.

	printf("Map Fail\n");

	return 0;
}


std::mutex AriaThreadLock[ARIA_THREAD_COUNT];
_SQLCon AriaSQL[ARIA_THREAD_COUNT];

std::string unAesString(const std::string &Input, const std::string &K, const std::string &IV) {

	if (!Input.size() || !IV.size() || !K.size())return "";

	char Key[32];
	char Iv[32];
	ZeroArray(Key);
	ZeroArray(Iv);

	memcpy(Key, &K[0], (32 <= K.size()) ? 32 : K.size());
	memcpy(Iv, &IV[0], (32 <= IV.size()) ? 32 : IV.size());

	std::string rString(Input.size(), '\0');

	try {
		CRijndael rj;
		rj.MakeKey(Key, Iv, 32, 32);
		rj.Decrypt((char*)&Input[0], (char*)&rString[0], Input.size());
	}
	catch(...){
		printf(KRED "AES FAILED\n" KRESET);
		return "";
	}

	return rString;
}

_inline std::string GetParam(const std::string& s, const std::string&& param) {

	size_t Off = s.find(param);
	if (Off == std::string::npos)return "";
	Off += param.size();

	DWORD End = s.size();

	for (DWORD i = Off + 1; i < s.size(); i++) {
		if (s[i] == '&') {
			End = i;
			break;
		}
	}

	return std::string(s.begin() + Off, s.begin() + End);
}

struct _GetParams {

	std::vector<std::pair<int/*Key Hash*/, std::string_view/*Value*/>> Params;

	template<const u32 Key>
	std::string_view get_pop() {

		for (size_t i = 0; i < Params.size(); ++i) {

			if (Params[i].first != Key)
				continue;

			std::string_view Ret = Params[i].second;

			std::swap(Params[i], Params.back());
			Params.pop_back();
			return Ret;
		}
		return std::string_view();
	}

	template<const u32 Key>
	std::string_view get()const{

		for (auto [K, Value] : Params){
			if (K != Key)continue;
			return Value;
		}
		return std::string_view();
	}

	_GetParams(const std::string_view URL) {

		size_t Start = URL.find('?');

		if (Start != std::string::npos) {
			Start++;

			bool Name = 1;

			u32 TempNameHash = 0;

			for (DWORD i = Start; i < URL.size(); i++) {

				if (Name) {
					if (URL[i] != '=')
						continue;
					TempNameHash = WeakStringToInt(std::string_view((const char*)&URL[Start], i - Start));
					Start = i + 1;
					Name = 0;
				}
				else{
					const bool Last = i == (URL.size() - 1) && ++i;//Special case for the last

					if (!Last && URL[i] != '&')
						continue;

					Params.emplace_back(
						TempNameHash,
						std::string_view((const char*)&URL[Start],i - Start)
						);
					Start = i + 1;

					Name = 1;
				}
			}
		}
	}

	_GetParams(std::vector<std::pair<int, std::string_view>>&& Params) : Params(_M(Params)) {}
};

void SendAria404(_Con s){

	s.SendData(ConstructResponse(200, Empty_Headers, STACK("<HTML><img src=\"https://cdn.discordapp.com/attachments/385279293007200258/570910676428652544/Kanzaki.png\"><br><b>404</b> - Aria does not know this page.</HTML>")));
	s.Dis();
}

void TryScoreAgain(_Con s){
	s.SendData(ConstructResponse(408, Empty_Headers, Empty_Byte));
	s.Dis();
}
void ScoreFailed(_Con s){
	s.SendData(ConstructResponse(408, Empty_Headers, STACK("error: no")));
	s.Dis();
}

Achievement GetAchievementsFromScore(const _Score &s, const float StarDiff) {
	
	Achievement Ret;

	u32& Bytes = Ret.General[al_clamp((int)s.GameMode,0, GM_MAX)];

	if (s.MaxCombo >= 500)
		Bytes |= Achievement::Combo500;
	if (s.MaxCombo >= 750)
		Bytes |= Achievement::Combo750;
	if (s.MaxCombo >= 1000)
		Bytes |= Achievement::Combo1000;
	if (s.MaxCombo >= 2000)
		Bytes |= Achievement::Combo2000;

	int StarCount = al_min(int(StarDiff), 8);

	while (StarCount > 0){
		if (s.FullCombo)
			Bytes += 1 << (11 + StarCount);
		Bytes += 1 << (3 + StarCount);

		StarCount--;
	}

	return Ret;
}

void ScoreServerHandle(const _HttpRes &res, _Con s){

	if (unlikely(res.GetHeaderValue("Host") != " osu.ppy.sh")){

		s.SendData(ConstructResponse(200, Empty_Headers, STACK("error: no")));
		s.Dis();
		return;
	}

	const char* mName = "Aria";

	int FailTime = -1;
	int Quit = -1;

	std::string score,
				score_bmk,//Beatmap hash - used to detect changing the map as it is loading.
				score_c1,
				score_osuver,
				score_s,//client hash
				score_i,//Used if osu thinks the person is flashlight cheating.
				score_iv,ReplayFile;

	_MD5 pass;

	const auto RawScoreData = Explode_View_Multi(res.Body,"-------------------------------28947758029299\r\n",16);

	if (RawScoreData.size() < 10){

		printf("RawScoreData was under 10(%zi)\n",RawScoreData.size());

		return TryScoreAgain(s);
	}else {
		bool FirstScoreParam = 1;
		for (DWORD i = 0; i < RawScoreData.size(); i++){

			if (!MEM_CMP_START(RawScoreData[i], "Content-Disposition: form-data; name=\""))
				continue;

			const char* End = &RawScoreData[i][38];
			const char* Start = &RawScoreData[i][38];
			const size_t DataSize = RawScoreData[i].size() - 39;

			if (DataSize == size_t(-1) || !DataSize)
				continue;

			bool QuickExit = 0;

			while (1) {
				End++;
				if (*End == '"')break;
				if (size_t(End - Start) >= DataSize) {
					QuickExit = 1;
					break;
				}
			}if (QuickExit)continue;

			const std::string Name(Start, End);

			Start = End + 5;
			End = &RawScoreData[i][RawScoreData[i].size() - 2];
			if (Start >= End)continue;


			#define EXTRACT(s) if(Name == #s)score_##s = std::string(Start, End)

			if (Name == "ft")
				FailTime = MemToNum<int>(Start, DWORD(End - Start));
			else if (Name == "x")
				Quit = MemToNum<int>(Start, DWORD(End - Start));
			else if (Name == "score") {
				if (FirstScoreParam)
					score = std::string(Start, End);
				else {
					Start += 58;
					End -= _strlen_("-------------------------------28947758029299--");
					if (Start >= End)continue;
					ReplayFile = std::string(Start, End);
				}
				FirstScoreParam = 0;
			}
			else EXTRACT(iv);
			else EXTRACT(bmk);
			else EXTRACT(c1);
			else if (Name == "pass")
				pass = _MD5(std::string(Start, End));
			else EXTRACT(osuver);
			else EXTRACT(s);
			else EXTRACT(i);
#undef EXTRACT
		}

		if (!score_iv.size() || !score_s.size() || !score_osuver.size() || !score.size()){//something very important is missing.
				LogError("Failed score.","Aria");
				return TryScoreAgain(s);
		}
		const bool OldClient = (FailTime == -1 || Quit == -1);

		if (FailTime == -1)
			FailTime = 0;
		if (Quit == -1)
			Quit = 0;

		score_iv = base64_decode(score_iv);
		
		const std::string Key = "osu!-scoreburgr---------" + score_osuver;

		_Score sData;
		{
			const std::string AES = unAesString(base64_decode(score), Key, score_iv);

			const auto ScoreData = Explode_View(AES, ':', 18);

			if (ScoreData.size() != 18) {
				const std::string Error = "Score sent with wrong ScoreData length (" + std::to_string(ScoreData.size()) + ")\0";
				LogError(Error.data(), "Aria");
				return ScoreFailed(s);
			}

			enum scoreOffset {
				score_FileCheckSum,
				score_PlayerName,
				score_onlineCheckSum,
				score_Count300,
				score_Count100,
				score_Count50,
				score_CountGeki,
				score_CountKatu,
				score_CountMiss,
				score_totalScore,
				score_maxCombo,
				score_Perfect,
				score_Rank,
				score_Mods,
				score_Pass,
				score_playMode,
				score_Date,
				Score_Version
			};

			sData.Mods = StringToNum(DWORD,ScoreData[score_Mods]);

			if (sData.Mods & (Mods::Relax2 | Mods::Autoplay | (1 << 29)
			#ifdef NO_RELAX
				| Mods::Relax
			#endif
				))
				return ScoreFailed(s);

			sData.BeatmapHash = REMOVEQUOTES(std::string(IT_COPY(ScoreData[scoreOffset::score_FileCheckSum])));

			sData.UserName = ScoreData[scoreOffset::score_PlayerName];
			sData.count300 = StringToNum(USHORT,ScoreData[score_Count300]);
			sData.count100 = StringToNum(USHORT, ScoreData[score_Count100]);
			sData.count50 = StringToNum(USHORT, ScoreData[score_Count50]);
			sData.countGeki = StringToNum(USHORT, ScoreData[score_CountGeki]);
			sData.countKatu = StringToNum(USHORT, ScoreData[score_CountKatu]);
			sData.countMiss = StringToNum(USHORT, ScoreData[score_CountMiss]);
			sData.Score = StringToNum(int, ScoreData[score_totalScore]);
			sData.MaxCombo = StringToNum(USHORT, ScoreData[score_maxCombo]);
			sData.FullCombo = (ScoreData[score_Perfect] == "True") ? 1 : 0;
			sData.GameMode = StringToNum(byte, ScoreData[score_playMode]);if (sData.GameMode > 3)sData.GameMode = 3;

			printf("AC: %i", std::count_if(begin(ScoreData[Score_Version]), end(ScoreData[Score_Version]), [](const char v) {return v == ' '; }));
			
		}//Score Data is ready to read.

		if (sData.UserName.size() && sData.UserName[sData.UserName.size()-1] == ' ')
			sData.UserName.pop_back();//Pops off supporter client check.
		
		_UserRef u(GetUserFromNameSafe(USERNAMESAFE(sData.UserName)),1);

		if (!u.User || !u.User->choToken){
			//They might still be logging in. Just abort the connection to let their client (hopefully) attempt again.
			//Hopefully once they have an active ruri connection.
			printf("%s> is not online?\n", sData.UserName.c_str());
			return TryScoreAgain(s);
		}

		const DWORD UserID = u.User->UserID;
		if (!(u.User->Password == pass)){
			printf("%s> password wrong\n", sData.UserName.c_str());
			return TryScoreAgain(s);
		}

		const DWORD sOffset =
		#ifndef NO_RELAX
			sData.Mods & Relax ? sData.GameMode + 4 :
		#endif
			sData.GameMode;

		{//Update total_score
		#ifdef NO_RELAX
			const std::string TableName = "users_stats";
		#else
			const std::string TableName = sData.Mods & Relax ? "rx_stats" : "users_stats";
		#endif
			const std::string Mode = [](const byte GM){
				switch (GM) {
				default:
					case 0: return "std";
					case 1: return "taiko";
					case 2: return "ctb";
					case 3: return "mania";				
				}
			}(sData.GameMode);

			SQLExecQue.AddQue("UPDATE " + TableName + " SET playcount_" + Mode + "=playcount_" + Mode + "+1,total_score_" + Mode +
				" = total_score_" + Mode + "+" + std::to_string(sData.Score) + " WHERE id = " + std::to_string(UserID) + " LIMIT 1");
		}

		if (const byte TrueGameMode =
		#ifndef NO_RELAX
			(sData.Mods & Relax) ? sData.GameMode + 4 :
		#endif			
			sData.GameMode;
			(u.User->privileges & (u32)Privileges::Visible) && !FailTime && !Quit && ReplayFile.size() > 250){

			_BeatmapDataRef MapRef;

			_BeatmapData *BD = GetBeatmapCache(0, 0, sData.BeatmapHash, "", &AriaSQL[s.ID], MapRef);

			if (!BD){
				printf(KRED"(%s) ScoreSubmit map failure\n" KRESET,sData.BeatmapHash.c_str());
				return TryScoreAgain(s);
			}
			float PP = 0.f;
			float MapStars = 0.f;

			const bool Loved = (BD->RankStatus == LOVED);

			if (BD->RankStatus >= RANKED){

				u.User->Stats[sOffset].PlayCount++;
				u.User->Stats[sOffset].tScore += sData.Score;

				if (sData.GameMode < 2){
				
					ezpp_t ez = ezpp_new();

					if (!ez) {
						LogError("Failed to load ezpp" "Aria");
						return TryScoreAgain(s);
					}

					ezpp_set_mods(ez, sData.Mods);
					ezpp_set_nmiss(ez, sData.countMiss);
					ezpp_set_accuracy(ez, sData.count100, sData.count50);
					ezpp_set_combo(ez, sData.MaxCombo);
					ezpp_set_mode(ez, sData.GameMode);

					if (!OppaiCheckMapDownload(ez, BD->BeatmapID)){
						printf("Could not download\n");
						return TryScoreAgain(s);
					}
				
					PP = ezpp_pp(ez);

					if (!Loved && PP < 30000.f){
						if (((sData.Mods & Relax) && PP > 1400.f) || (!(sData.Mods & Relax) && PP > 700.f)){

							UpdateQue.emplace_back(UserID, (u32)0, _UserUpdate::Restrict, (size_t)new std::string("Restricted for too much pp in a single play: " + std::to_string(PP)));

						}
					}

					 MapStars = (sData.Mods & (NoFail | Relax | Relax2)) ? 0.f : ezpp_stars(ez);

				}else{
					constexpr auto b = PacketBuilder::CT::String_Packet(Packet::Server::notification, "That gamemode is currently not supported for pp.\nYour score will still be saved for future calculations.");
					u->addQueArray(b);
					goto SENDSCORE;
				}
			}
			_ScoreCache sc(sData,u.User->UserID,PP);
			
			sc.Loved = (BD->RankStatus == LOVED);

			std::string ClientScoreUpdate;

			bool NewBest = BD->AddScore(TrueGameMode, sc, &AriaSQL[s.ID],&ClientScoreUpdate);

			std::string Charts =
				"beatmapId: " + std::to_string(BD->BeatmapID) +
				"|beatmapSetId: " + std::to_string(BD->SetID) +
				"|beatmapPlaycount: 1"
				"|beatmapPasscount: 1"
				"|approvedDate: 0\n";

			if (ClientScoreUpdate.size() != 0){
				Charts += "chartId: beatmap"
					"|chartUrl: https://osu.ppy.sh/b/" + std::to_string(BD->BeatmapID) +
					"|chartName: Beatmap Ranking" + ClientScoreUpdate;
				
				//TODO: might want to add overall stats
				std::string achievements;
				
				Achievement New = GetAchievementsFromScore(sData, MapStars);

				CalculateAchievement(New, u->Ach, sData.GameMode, achievements);//TODO Add ach to DB.
				u->Ach = New;

				if (achievements.size()){
					SQLExecQue.AddQue("UPDATE users SET achievements_0=" +
						std::to_string(u64(New.General[0]) + (u64(New.General[1]) << 32)) + ",achievements_1=" + std::to_string(u64(New.General[2]) + (u64(New.General[3]) << 32)) +
						" WHERE id=" + std::to_string(u->UserID) + " LIMIT 1;");

					Charts += "|achievements-new: " + achievements + "\n";
				}else Charts += "\n";
			}
			
			s.SendData(ConstructResponse(200, Empty_Headers, Charts));
			s.Dis();

			if (const DWORD Off = u->GetStatsOffset(); !Loved && NewBest && UpdateUserStatsFromDB(&AriaSQL[s.ID], UserID, TrueGameMode, u.User->Stats[TrueGameMode]))
				PacketBuilder::Build<Packet::Server::userStats, 'm','i','b','a','5','i','b','i','l','i','i','l','i','w'>(u->QueBytes,&u->qLock,u->UserID,u->actionID,u->ActionText,u->ActionMD5,u->actionMods,u->GameMode,u->BeatmapID,
					u->Stats[Off].rScore,*(int*)&u->Stats[Off].Acc, u->Stats[Off].pp > USHORT(-1) ? u->Stats[Off].pp : u->Stats[Off].PlayCount, u->Stats[Off].tScore, u->Stats[Off].getRank(Off, u->UserID), USHORT(u->Stats[Off].pp));

			if (NewBest && sc.ScoreID && ReplayFile.size())
				WriteAllBytes(ReplayPath + std::to_string(sc.ScoreID) + ".osr",&ReplayFile[0], ReplayFile.size());

			return;
		}
		SENDSCORE:
		s.SendData(ConstructResponse(200, Empty_Headers, STACK("error: no")));
		s.Dis();

		AriaSQL[s.ID].ExecuteUPDATE(SQL_INSERT(
			Score_Table_Name[RELAX_MODE && (sData.Mods & Relax)],
		{
		_SQLKey("beatmap_md5",std::string(sData.BeatmapHash)),
		_SQLKey("userid",UserID),
		_SQLKey("score",sData.Score),
		_SQLKey("max_combo",sData.MaxCombo),
		_SQLKey("full_combo",sData.FullCombo),
		_SQLKey("mods",sData.Mods),
		_SQLKey("300_count",sData.count300),
		_SQLKey("100_count",sData.count100),
		_SQLKey("50_count",sData.count50),
		_SQLKey("katus_count",sData.countKatu),
		_SQLKey("gekis_count",sData.countGeki),
		_SQLKey("misses_count",sData.countMiss),
		_SQLKey("time",time(0)),
		_SQLKey("play_mode",sData.GameMode),
		_SQLKey("completed",0),
		_SQLKey("accuracy",std::to_string(sData.GetAcc())),
		_SQLKey("pp",0)
		}));

		return;
	}

}

enum RankingType
{
	Local,
	Top,
	SelectedMod,
	Friends,
	Country
};

std::string urlDecode(const std::string_view SRC){

	const size_t Size = SRC.size();
	std::string ret(Size, '\0');
	size_t Off = 0;
	for (size_t i = 0; i < Size; i++)
		ret[Off++] =
		SRC[i] == '%' && i + 2 < Size
		? (CharHexToDecimal(SRC[i+++1]) << 4) + CharHexToDecimal(SRC[i+++1])
		: SRC[i] == '+' ? ' ' : SRC[i];
	ret.resize(Off);

	return ret;
}


#define PACK(Name,Type)pack<Type, ""#Name##_HU>{}
template<typename T, u32 Hash_Value>
struct pack {
	static constexpr u32 Hash = Hash_Value;
	static constexpr T type{};
};

template<typename ...T>
auto get_param_sbind(_GetParams &a, const T& ...Output){

	return std::tuple<typename std::decay<decltype(T::type)>::type...>{
		([&]{
			if constexpr (std::is_same<std::string_view, typename std::decay<decltype(T::type)>::type>::value)
				return a.get_pop<T::Hash>();
			else if constexpr (1) {
				const auto T_Value = a.get_pop<T::Hash>();

				typename std::decay<decltype(T::type)>::type V{};

				std::from_chars((const char* const)T_Value.data(), (const char* const)(T_Value.data() + T_Value.size()), V);

				return V;
			}
			return decltype(T::type){};
		}()
	)...};
}

void osu_getScores(const _HttpRes& http, _Con s){

	auto [LeaderBoardVersion, LType, BeatmapMD5, FileName, Mode,
		  SetID, Mods, UserName_View, Password_View] = get_param_sbind(_GetParams(http.Host),
			PACK("vv", int), PACK("v", int),
			PACK("c", std::string_view),
			PACK("f", std::string_view),
			PACK("m", u32), PACK("i", int),
			PACK("mods", u32),
			PACK("us", std::string_view),
			PACK("ha", std::string_view)
	);

	if (unlikely(LeaderBoardVersion != 4 || BeatmapMD5.size() != 32 || http.GetHeaderValue("Host") != " osu.ppy.sh"))
		return SendAria404(s);

	_UserRef u(GetUserFromName(urlDecode(UserName_View)), 1);

	if (!u || !(_MD5(Password_View) == u->Password))
		return SendAria404(s);

	if constexpr (RELAX_MODE){

		if ((u->actionMods ^ Mods) & Mods::Relax) {

			/*
				If the selected mods are desynced compared to their last natural status update
				we will update their mode status and send their new ranking status to their client.			
			*/

			u->actionMods = Mods;

			_User* tP = u.User;

			const DWORD Off = tP->GetStatsOffset();

			PacketBuilder::Build<Packet::Server::userStats, 'm',
				'i', 'b', 'a', '5', 'i', 'b', 'i', 'l', 'i', 'i', 'l', 'i', 'w'>(tP->QueBytes, &tP->qLock,
					tP->UserID, tP->actionID, tP->ActionText, tP->ActionMD5, tP->actionMods, tP->GameMode,
					tP->BeatmapID, tP->Stats[Off].rScore, *(int*)&tP->Stats[Off].Acc,
					tP->Stats[Off].pp > USHORT(-1) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount,
					tP->Stats[Off].tScore, tP->Stats[Off].getRank(Off, tP->UserID), USHORT(tP->Stats[Off].pp)
					);
		}
		Mode = (Mods & Relax) ? Mode + 4 : Mode;
	}

	if (Mode > GM_MAX)
		Mode = 0;

	if (u->actionID != bStatus::sPlaying && BeatmapMD5.size() == 32)
		memcpy(&u->ActionMD5[0], &BeatmapMD5[0], 32);

	_BeatmapDataRef MapRef;

	_BeatmapData* const BeatData = GetBeatmapCache(SetID, 0, BeatmapMD5, std::string(FileName), &AriaSQL[s.ID], MapRef);

	if (!BeatData || !BeatData->BeatmapID){
		s.SendData(ConstructResponse(200, Empty_Headers, STACK("-1|0")));
		return s.Dis();
	}

	const bool NeedUpdate = (BeatData && BeatData->Hash != BeatmapMD5);

	if (NeedUpdate)
		CheckMapUpdate(BeatData, &AriaSQL[s.ID]);//TODO: Should probably properly handle this instead of this hack.

	_LeaderBoardCache* const LeaderBoard = BeatData->GetLeaderBoard(Mode, &AriaSQL[s.ID]);

	const DWORD TotalScores = LeaderBoard ? LeaderBoard->ScoreCache.size() : 0;

	const bool Loved = (BeatData->RankStatus == LOVED);	

	std::string Response = NeedUpdate ? "1" : std::to_string(al_max(BeatData->RankStatus, 0));
	Response += "|false"//server osz
		"|" + std::to_string(BeatData->BeatmapID)//beatmap id
		+ "|" + std::to_string(BeatData->SetID)//set id
		+ "|" + std::to_string(TotalScores)//total records
		+ "\n0"//offset
		"\n" + BeatData->DisplayTitle//song name
		+ "\n" + std::to_string(BeatData->Rating) + "\n";//rating

	//Personal best
	if (!NeedUpdate && BeatData->RankStatus >= RANKED && LeaderBoard){

		DWORD Rank = LeaderBoard->GetRankByUID(u.User->UserID);

		if (Rank) {

			_ScoreCache s = LeaderBoard->GetScoreByRank(Rank);

			if (s.UserID != u.User->UserID)
				s = LeaderBoard->GetScoreByUID(u.User->UserID);
			if (s.UserID != 0){
				Response += std::to_string(s.ScoreID);//online id
				Response += "|" + u.User->Username;//player name
				Response += "|" + std::to_string((!Loved && Mode > 3) ? int(s.pp + 0.5f) : s.Score);//total score
				Response += "|" + std::to_string(s.MaxCombo);//max combo
				Response += "|" + std::to_string(s.count50);//count 50
				Response += "|" + std::to_string(s.count100);//count 100
				Response += "|" + std::to_string(s.count300);//count 300
				Response += "|" + std::to_string(s.countMiss);//count miss
				Response += "|" + std::to_string(s.countKatu);//count katu
				Response += "|" + std::to_string(s.countGeki);//count geki
				if (s.FullCombo)Response += "|1";//perfect
				else Response += "|0";//perfect
				Response += "|" + std::to_string(s.Mods);//mods
				Response += "|" + std::to_string(u.User->UserID);//userid
				Response += "|" + std::to_string(Rank);//online rank
				Response += "|" + std::to_string(s.Time);//date 
				Response += "|1";//online replay
			}
		}

		if (LeaderBoard->ScoreCache.size()) {

			std::shared_lock L(LeaderBoard->ScoreLock);

			u32 Rank = 0;

			for (size_t i = 0; i < LeaderBoard->ScoreCache.size(); i++) {

				if (Rank >= 250)
					break;//Not doing this for performance reasons of the server. Simply the client is not built to handle it.

				const _ScoreCache* lScore = &LeaderBoard->ScoreCache[i];

				if ((LType == RankingType::SelectedMod && lScore->Mods != Mods)
					|| (LType == RankingType::Friends && !u.User->isFriend(lScore->UserID)))
					continue;

				const int Score = (!Loved && (Mode > 3 || LType == RankingType::Country)) ? int(lScore->pp + 0.5f) : lScore->Score;

				if (Score < 0)
					continue;

				++Rank;

				if (u->isBlocked(lScore->UserID))
					continue;

				Response += "\n" + std::to_string(lScore->ScoreID)//online id
					+ "|" + GetUsernameFromCache(lScore->UserID)//player name
					+ "|" + std::to_string(Score)//total score
					+ "|" + std::to_string(lScore->MaxCombo)//max combo
					+ "|" + std::to_string(lScore->count50)//count 50
					+ "|" + std::to_string(lScore->count100)//count 100
					+ "|" + std::to_string(lScore->count300)//count 300
					+ "|" + std::to_string(lScore->countMiss)//count miss
					+ "|" + std::to_string(lScore->countKatu)//count katu
					+ "|" + std::to_string(lScore->countGeki)//count geki
					+ std::string((lScore->FullCombo) ? "|1" : "|0")
					+ "|" + std::to_string(lScore->Mods)//mods
					+ "|" + std::to_string(lScore->UserID)//userid
					+ "|" + std::to_string(Rank)//online rank
					+ "|" + std::to_string(lScore->Time)//date
					+ "|1";//online replay
			}

		}
	}

	s.SendData(ConstructResponse(200, Empty_Headers, Response));
	s.Dis();
}

std::vector<std::tuple<const u32,u32,std::string>> UpdateCache;std::shared_mutex UpdateCache_Lock;

void osu_checkUpdates(_GetParams&& Params,_Con s){

	const std::string_view action = Params.get<WSTI("action")>();

	if(action != "check" && action != "path")
		return SendAria404(s);

	bool AlreadyIn = 0;

	const u32 reqStream = WSTI<u32>(Params.get<WSTI("stream")>());

	s_mlock (UpdateCache_Lock)
		for (auto& [Stream,LastTime,Cache] : UpdateCache)
			if (reqStream == Stream){
				if (LastTime + 3600000 > clock_ms()){
					s.SendData(Cache);
					return s.Dis();
				}
				AlreadyIn = 1;
				break;
			}

	const std::string res = GET_WEB("old.ppy.sh", "web/check-updates.php?action=" + std::string(action)
												+ "&stream=" + std::string(Params.get<WSTI("stream")>())
												+ "&time=" + std::string(Params.get<WSTI("time")>()));

	s.SendData(res);
	s.Dis();

	std::scoped_lock L(UpdateCache_Lock);

	if (likely(AlreadyIn)){
		for (auto& [Stream, LastTime, Cache] : UpdateCache)
			if (reqStream == Stream) {
				LastTime = clock_ms();
				Cache = res;//dc about copy :)
				break;
			}
		
	}else UpdateCache.emplace_back(reqStream, clock_ms(), res);//dc about copy :)
}

const std::string directToApiStatus(const std::string &directStatus) {//thank you ripple
	if (!directStatus.size())
		return "";
	if (directStatus == "0" || directStatus == "7")
		return "1";
	if(directStatus == "8")
		return "4";
	if (directStatus == "3")
		return "3";
	if (directStatus == "2")
		return "0";
	if (directStatus == "5")
		return "-2";
	if (directStatus == "4")
		return "";

	return "1";
}

void GetReplay(const std::string_view URL, _Con s){

	std::string ID = std::string(_GetParams{URL}.get<WSTI("c")>());

	for (auto& c : ID)
		if (c == '.')
			c = '/';

	s.SendData(ConstructResponse(200, Empty_Headers, LOAD_FILE_RAW(ReplayPath + ID + ".osr")));

	return s.Dis();
}

void Thread_DownloadOSZ(const DWORD MapID, _Con s){

	s.SendData(GET_WEB(MIRROR_IP, "d/" + std::to_string(MapID)));

	return s.Dis();
}

static inline uint64_t UnixToDateTime(const int Unix){
	return 0x89F7FF5F7B58000 + (int64_t(Unix) * 10000000);
}

void Thread_WebReplay(const uint64_t ID, _Con s){

	auto res = SQL_BanchoThread[clock() & 3].ExecuteQuery("SELECT userid,play_mode,beatmap_md5,300_count,100_count,50_count,gekis_count,katus_count,misses_count,score,max_combo,full_combo,mods,time"
								 " FROM "+ Score_Table_Name[!(ID > 500000000u)] + " WHERE id=" + std::to_string(ID) + " LIMIT 1");

	if (const std::vector<byte>& Data = LOAD_FILE_RAW(ReplayPath + std::to_string(ID) + ".osr"); Data.size() && res && res->next())
	{
		std::vector<byte> Total;
		
		Total.reserve(256 + Data.size());

		Total.push_back(res->getInt(2));//PlayMode
		AddStream(Total, 20190101);//osu version
		AddString_SQL(Total, res->getString(3));//beatmap md5
		AddString(Total, GetUsernameFromCache(res->getInt(1)));//Username
		AddString_SQL(Total, res->getString(3));//checksum
		AddStream(Total, u16(res->getInt(4)));//count300
		AddStream(Total, u16(res->getInt(5)));//count100
		AddStream(Total, u16(res->getInt(6)));//count50
		AddStream(Total, u16(res->getInt(7)));//countGeki
		AddStream(Total, u16(res->getInt(8)));//countKatu
		AddStream(Total, u16(res->getInt(9)));//countMiss
		AddStream(Total, res->getUInt(10));//totalScore
		AddStream(Total, u16(res->getInt(11)));//MaxCombo
		Total.push_back(res->getBoolean(12));//Perfect
		AddStream(Total, res->getUInt(13));//Mods
		AddString(Total, "");//life
		AddStream(Total, UnixToDateTime(res->getUInt(14)));//Date
		AddStream(Total, u32(Data.size()));
		AddVector(Total, Data);
		AddStream(Total, ID);

		s.SendData(ConstructResponse(200, {
			{"Content-type","application/octet-stream"},
			{"Content-length",std::to_string(Total.size())},
			{"Content-Description","File Transfer"},
			{"Content-Disposition","attachment; filename=\"" + std::to_string(ID) + ".osr\""}
			}, Total));
	}

	DeleteAndNull(res);

	return s.Dis();
}

void Thread_UpdateOSU(const std::string URL, _Con s){

	s.SendData(GET_WEB("old.ppy.sh", URL));//Their osu clients will unchunk the data for us :)

	return s.Dis();
}


template<size_t Size>
struct rString {

	char Data[Size];

#pragma warning(suppress: 26495)
	template<typename Alloc> rString(Alloc A) noexcept {
		for (size_t i = 0; i < Size; i++)
			Data[i] = A();
	}

	constexpr size_t size() const noexcept { return Size; }

	void operator+(std::string& String)const noexcept {
		if constexpr (Size == 0)return;
		String.resize(String.size() + Size);
		memcpy((String.data() + String.size()) - Size, Data, Size);
	}

	std::string to_string()const noexcept{ return std::string((const char*)Data, Size); }

};

void UploadScreenshot(const _HttpRes &res, _Con s){

	if (res.Body.size() < 1000 || res.Body.size() > 2000000)
		return;

	#define SCREENSHOT_START "Content-Disposition: form-data; name=\"ss\"; filename=\"ss\"\r\nContent-Type: application/octet-stream\r\n\r\n"
	#define SCREENSHOT_END "\r\n-------------------------------28947758029299--"
	
	const static std::string Start = SCREENSHOT_START;

	auto it = std::search(
		res.Body.begin(), res.Body.end(),
		begin(Start), end(Start));

	if (it == end(res.Body))
		return s.Dis();

	it += CONSTX<size_t,_strlen_(SCREENSHOT_START)>::value;

	const auto end = res.Body.end() - CONSTX<size_t, _strlen_(SCREENSHOT_END)>::value;

	if (end > it){

		constexpr char CharList[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
									  'A', 'B', 'C', 'D', 'E', 'F', '_', 'a', 'b', 'c',
									  'd', 'e', 'f', '-', 'r', 'u', 'm', 'o', 'i', 'l', 'g', 'n' };

		const std::string Filename = rString<8>([&CharList]()->char {return CharList[BR::GetRand(0, sizeof(CharList) - 1)]; }).to_string() + ".png";

		WriteAllBytes("/home/ss/" + Filename, &*it, end - it);
		s.SendData(ConstructResponse(200, {}, Filename));
	}

	#undef SCREENSHOT_START
	#undef SCREENSHOT_END

	return s.Dis();	
}

namespace MIRROR {

	std::mutex MirrorAPILock;
	std::vector<std::pair<const std::string, _Con>> MirrorAPIQue;

	void HandleMirrorAPI(){

		std::vector<std::pair<const std::string, _Con>> QueCopy;

		while (1){

			if (MirrorAPIQue.size()){

				QueCopy.clear();

				mlock (MirrorAPILock){

					for (DWORD i = 0; i < MirrorAPIQue.size(); i++)
						QueCopy.emplace_back(_M(MirrorAPIQue[i].first), MirrorAPIQue[i].second);

					MirrorAPIQue.clear();
				}

				for (auto& [req,Con] : QueCopy){
					if constexpr (UsingRawMirror) {
						Con.SendData(GET_WEB(MIRROR_IP, req));
						Con.Dis();
					}
				}
			}

			Sleep(10);
		}
	}
}

void LastFM(_GetParams&& Params, _Con s){

	enum {
		RelifeRunning = 1 << 0,
		Console = 1 << 1,
		//1<<2 can false flag. Do not use.
		InvalidName = 1 << 3,
		InvalidFile = 1 << 4,
		RelifeLoaded = 1 << 5,
		SuspiciousFlags = RelifeLoaded | Console | InvalidName | InvalidFile | RelifeLoaded
	};

	const int Flags = [&Params]{

		int Value = 0;

		if (auto B = Params.get<"b"_HU>(); B.size() > 1 && unlikely(B[0] == 'a'))
			std::from_chars(B.data() + 1, B.data() + B.size(), Value);

		return Value;
	}();

	if (unlikely(Flags & SuspiciousFlags)){

		_UserRef u(GetUserFromNameSafe(USERNAMESAFE(std::string(Params.get<"us"_HU>()))), 1);

		if (likely(u.valid() && u->Password == _MD5(Params.get<"ha"_HU>())) && !u->is_banned()){

			u->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::RTX, '-'>(
				Flags == RelifeLoaded ? 
					STACK("Disable osu-relife or get banned") : 
					STACK("What did you think would happen?")
			));

			const auto ID = u->UserID;

			#define C(x) std::tuple<size_t,size_t, const char*>{x, std::integral_constant<size_t,_strlen_(""#x)>::value, ""#x}

			constexpr std::array Detections{
				C(RelifeRunning), C(Console), C(InvalidName), C(InvalidFile), C(RelifeLoaded)
			};

			#undef C

			std::string FlagString;

			for (const auto& [FlagValue, FlagNameSize, FlagName] : Detections)
				if (Flags & FlagValue){

					FlagString.resize(FlagNameSize + 1);

					char* Start = FlagString.data() + (FlagString.size() - FlagNameSize);
					*Start++ = '|';

					memcpy(Start, FlagName, FlagNameSize);
				}

			if (likely(FlagString.size())){

				printf(KYEL "%i> AC_FM Flags: %s\n" KRESET, ID, FlagString.c_str());

				if (!(u->privileges & (u32)Privileges::SuperAdmin) && Flags != RelifeLoaded)
					UpdateQue.emplace_back(ID, (u32)0, _UserUpdate::Restrict, (size_t)new std::string("Restricted for flags: " + FlagString));

			}
		}
	}

	s.SendData(STACK("HTTP/1.0 200 OK" MNL
			   "Content-Length: 2" MNL
				"Connection: close" DMNL
				"-3"));

	return s.Dis();
}

#define IS_NUM(s)!(s < '0' || s > '9')

#include "Web.h"

void HandleAria(_Con s){

	_HttpRes res;

	if (!s.RecvData(res)){
		s.Dis();
		return LogError("Connection Lost", ModuleName);
	}

	bool ThreadSpawned = 0;

	if (res.Host.size()){
		
		size_t Size = res.Host.size();

		if (size_t Pos = res.Host.find(".php"); Pos != std::string::npos)
			Size = Pos + 4;
		else for (; Size-- > 0;)// For direct access pages ignore the top level path.
			if (res.Host[Size] == '/' && ++Size)
				break;

		if (unlikely(MEM_CMP_START(res.Host, "/home/")))
			web::handle_web(res,s);
		else switch (WSTI<size_t>(std::string_view(res.Host.data(),Size))){

			case "/web/osu-submit-modular-selector.php"_HUST:
				ScoreServerHandle(res, s);
				break;

			case "/web/check-updates.php"_HUST:
				osu_checkUpdates(_GetParams(res.Host), s);
				break;

			case "/web/osu-osz2-getscores.php"_HUST:
				osu_getScores(res, s);
				break;

			case "/web/osu-search-set.php"_HUST:
				if (const DWORD SetID = StringToNum(DWORD, std::string_view(res.Host.data() + Size,res.Host.size() - Size)); SetID){

					ThreadSpawned = 1;

					std::scoped_lock L(MIRROR::MirrorAPILock);
					MIRROR::MirrorAPIQue.emplace_back("api/set?b=" + std::to_string(SetID), s);
				}
				break;

			case "/web/osu-search.php"_HUST:{

					size_t Start = 0;

					for (size_t i = 22; i < res.Host.size() - 2; i++) {
						if (*(USHORT*)&res.Host[i] == 0x7226) {//&r
							Start = i + 1;
							break;
						}
					}

					if (Start) {
						ThreadSpawned = 1;

						std::string u(res.Host.begin() + Start, res.Host.end());

						for (char& a : u)
							if (a == ' ')a = '+';

						std::scoped_lock L(MIRROR::MirrorAPILock);
						MIRROR::MirrorAPIQue.emplace_back("api/search?" + u, s);
					}
				}
				break;

			case "/web/osu-getreplay.php"_HUST:
				GetReplay(res.Host, s);
				break;

			case "/d/"_HUST:
				if (const DWORD ID = StringToNum(DWORD, std::string_view(res.Host.data() + Size, res.Host.size() - Size)); ID) {
					ThreadSpawned = 1;
					std::thread a(Thread_DownloadOSZ, ID, s);
					a.detach();
				}
				break;

			case "/web/maps/"_HUST :
				ThreadSpawned = 1;
				{
					std::thread a(Thread_UpdateOSU, std::string(res.Host.begin() + 1, res.Host.end()), s);
					a.detach();
				}
				break;

			case "/web/replays/"_HUST:
				ThreadSpawned = 1;
				{
					constexpr size_t rSize = _strlen_("/web/replays/");

					std::thread a(Thread_WebReplay, MemToNum<uint64_t>(&res.Host[rSize], res.Host.size() - rSize), s);
					a.detach();
				}
				break;

			case "/web/osu-screenshot.php"_HUST:
				UploadScreenshot(res, s);
				break;

			case "/web/lastfm.php"_HUST:
				LastFM(_GetParams(res.Host), s);
				break;

			default:
				SendAria404(s);
				break;
		}
	}

	if (!ThreadSpawned) { s.Dis(); }

}

template<typename T>
	struct _ConQue {

		int Count = 0;

		std::atomic<T> S = T();
		std::condition_variable cv;
		std::mutex Lock;

		_inline void notify() {
			std::lock_guard L(Lock);
			Count++;
			cv.notify_one();
		}

		_inline void wait() {
			std::unique_lock L(Lock);
			while (!Count)
				cv.wait(L);

			Count--;
		}

	};

template<size_t Thread_Count>
struct _SocketWorker{

	_ConQue<SOCKET> Slots[Thread_Count] = {};

	template<typename F>
	void work(const char* Socket, F WorkFunction, u16 PORT = 0){

		for (size_t i = 0; i < Thread_Count; i++){
			std::thread t(WorkFunction,i, std::ref(Slots[i]));
			t.detach();
		}

	#ifndef LINUX

		SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		sockaddr_in hint;
		ZeroMemory(&hint.sin_addr, sizeof(hint.sin_addr));
		hint.sin_family = AF_INET;
		hint.sin_port = htons(PORT);

		::bind(listening, (sockaddr*)& hint, sizeof(hint));

		listen(listening, SOMAXCONN);// Sets the socket to listen

		DWORD Time = 5000;
		DWORD MPL = MAX_PACKET_LENGTH;

		setsockopt(listening, SOL_SOCKET, SO_RCVTIMEO, (char*)& Time, 4);
		setsockopt(listening, SOL_SOCKET, SO_SNDTIMEO, (char*)& Time, 4);
		setsockopt(listening, SOL_SOCKET, SO_RCVBUF, (char*)& MPL, 4);

	#else

		SOCKET listening = socket(AF_UNIX, SOCK_STREAM, 0);

		struct sockaddr_un serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));

		serveraddr.sun_family = AF_UNIX;
		strcpy(serveraddr.sun_path, Socket);

		unlink(Socket);

		if (bind(listening, (struct sockaddr*) & serveraddr, SUN_LEN(&serveraddr)) < 0) {
			perror("bind() failed");
			return;
		}
		if (listen(listening, SOMAXCONN) < 0) {
			perror("listen() failed");
			return;
		}
		chmod(Socket, S_IRWXU | S_IRWXG | S_IRWXO);

	#endif

		u32 ID = 0;

		for (;;){

		#ifndef LINUX
			sockaddr_in client;
			ZeroMemory(&client, sizeof(client));
			int clientSize = sizeof(client);
			SOCKET s = accept(listening, (sockaddr*)&client, &clientSize);
		#else
			SOCKET s = accept(listening, 0, 0);
		#endif

			if (s == INVALID_SOCKET)
				continue;

			while (Slots[ID].S)
				ID = ++ID >= Thread_Count ? 0 : ID;

			Slots[ID].S = s;
			Slots[ID].notify();

			++COUNT_REQUESTS;

		}

	}

};

void AriaWork(const DWORD ID, _ConQue<SOCKET> &Slot){

	SOCKET conSocket = 0;

	for(;;){

		while (!(conSocket = Slot.S))
			Slot.wait();

		Slot.S = 0;

		HandleAria(_Con{ conSocket, ID });
	}

}

#include <fcntl.h>

bool ARIALOADED = 0;
void Aria_Main(){
	
	UpdateCache.reserve(16);//Really should only be a few anyway.

	for (size_t i = 0; i < ARIA_THREAD_COUNT; i++)
		AriaSQL[i].Connect();

	{
		std::thread a(MIRROR::HandleMirrorAPI);
		a.detach();
	}
	
	ARIALOADED = 1;

	_SocketWorker<ARIA_THREAD_COUNT> Worker;

	Worker.work(ARIA_UNIX_SOCKET, AriaWork, ARIAPORT);
}

#undef ModuleName
