#pragma once

#include "aes.h"
#include "Base64.h"

#define ARIA_THREAD_COUNT 8

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
	bool FullCombo;
	byte GameMode;
	float pp;

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
		Time = time(0);
	}

	_ScoreCache() {
		UserID = 0;
		ScoreID = 0;
		GameMode = 0;
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

	DWORD GetRankByUID(const DWORD UID, bool DontLock = 0){

		DWORD Rank = 0;

		if(!DontLock)ScoreLock.lock_shared();

		for (DWORD i = 0; i < ScoreCache.size(); i++){
			if (ScoreCache[i].UserID == UID) {
				Rank = i + 1;
				break;
			}
		}
		if (!DontLock)ScoreLock.unlock_shared();

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

	bool AddScore(_ScoreCache &s, const DWORD BID, const std::string &MapName, std::string MD5, _SQLCon *SQL = 0, std::string *Ret = 0) {
		
		PlayCount++;
		PassCount++;//dont really care about this getting malformed..

		bool Done = 0;
		bool NewTop = 0;

		MD5 = REMOVEQUOTES(MD5);
		
	#ifndef NO_RELAX
		#define TableName std::string(s.Mods & Relax ? "scores_relax" : "scores")
	#else
		#define TableName std::string("scores")
	#endif


		if (MD5.size() != 32){
			printf("AddScore Incorrect md5 length\n");//should never happen
			return 0;
		}

		ScoreLock.lock();
		
		int LastRank = 0;
		_ScoreCache LastScore;

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
			_SQLKey("pp", std::to_string(s.pp))
		};

		for (DWORD i = 0; i < ScoreCache.size(); i++){
			if (ScoreCache[i].UserID == s.UserID){
				LastScore = ScoreCache[i];
				Done = 1;
				if (ScoreCache[i].pp < s.pp){

					if (SQL){

						ScoreInsert.push_back(_SQLKey("completed", 3));

						SQL->Lock.lock();

						SQL->ExecuteUPDATE("UPDATE " + TableName +  " SET completed = 2 WHERE id = " + std::to_string(ScoreCache[i].ScoreID) + " LIMIT 1",1);
						
						SQL->ExecuteUPDATE(SQL_INSERT(TableName,_M(ScoreInsert)),1);

						auto res = SQL->ExecuteQuery("SELECT LAST_INSERT_ID() FROM " + TableName,1);

						SQL->Lock.unlock();
						
						if (res && res->next())
							s.ScoreID = res->getInt64(1);
						
						DeleteAndNull(res);

					}
					ScoreCache[i] = s;
					NewTop = 1;
					LastRank = i + 1;
				}else{
					ScoreInsert.push_back(_SQLKey("completed", 2));
					SQL->ExecuteUPDATE(SQL_INSERT(TableName, _M(ScoreInsert)));
				}
				break;
			}
		}
		if (!Done){

			ScoreInsert.push_back(_SQLKey("completed", 3));

			SQL->Lock.lock();

			SQL->ExecuteUPDATE(SQL_INSERT(TableName, _M(ScoreInsert)), 1);

			auto res = SQL->ExecuteQuery("SELECT LAST_INSERT_ID() FROM " + TableName, 1);

			SQL->Lock.unlock();

			if (res && res->next())
				s.ScoreID = res->getUInt64(1);

			DeleteAndNull(res);

			ScoreCache.push_back(s);
			NewTop = 1;
		}
		#undef TableName

		if(NewTop)
			std::sort(ScoreCache.begin(), ScoreCache.end() ,
		#ifndef NO_RELAX
			(s.Mods & Relax) ? SortScoreCacheByPP :
			#endif				
				SortScoreCacheByScore);//Could optimize this by bounding it. Will do it if this ever becomes a problem (doubt it)

		const DWORD NewRank = (!NewTop) ? 0 : GetRankByUID(s.UserID,1);
		
		ScoreLock.unlock();

		if (NewRank == 1 && ScoreCache.size() > 5)
			chan_Announce.Bot_SendMessage("[https://osu.ppy.sh/u/" + std::to_string(s.UserID) + " " + GetUsernameFromCache(s.UserID) +
				"] has achieved #1 on [https://osu.ppy.sh/b/" + std::to_string(BID) +" "+ MapName +"] ( " + RoundTo2(s.pp) + "pp )");

		printf("ScoreID: %llu\n", s.ScoreID);

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
	_LeaderBoardCache* lBoard[GM_MAX + 1];
	int RankStatus;

	~_BeatmapData() {

		for (DWORD i = 0; i < GM_MAX + 1; i++){
			DeleteAndNull(lBoard[i]);
		}

	}

	_BeatmapData() {
		BeatmapID = 0;
		SetID = 0;
		RankStatus = RankStatus::UNKNOWN;
		Rating = 0.f;
		lBoard[0] = 0;
		lBoard[1] = 0;
		lBoard[2] = 0;
		lBoard[3] = 0;
		lBoard[4] = 0;
		lBoard[5] = 0;
		lBoard[6] = 0;
		lBoard[7] = 0;
	}
	_BeatmapData(const int Status) {
		BeatmapID = 0;
		SetID = 0;
		RankStatus = Status;
		Rating = 0.f;
		lBoard[0] = 0;
		lBoard[1] = 0;
		lBoard[2] = 0;
		lBoard[3] = 0;
		lBoard[4] = 0;
		lBoard[5] = 0;
		lBoard[6] = 0;
		lBoard[7] = 0;
	}

	_LeaderBoardCache* GetLeaderBoard(const DWORD Mode, _SQLCon *const SQL){
		if (Mode >= 8)return 0;

		if (lBoard[Mode])
			return lBoard[Mode];
		
		if (SQL){

			const std::string TableName = (Mode >= 4) ? "scores_relax" : "scores";
			const std::string OrderBy = (Mode >= 4) ? "pp" : "score";

			_LeaderBoardCache* L = new _LeaderBoardCache();

			auto *res = SQL->ExecuteQuery("SELECT id,score,max_combo,50_count,100_count,300_count,misses_count,katus_count,gekis_count,full_combo,mods,userid,pp,time FROM " + TableName + " WHERE completed = 3 AND beatmap_md5 = '" + Hash + "' AND play_mode = " + std::to_string(Mode % 4) + " AND pp > 0 ORDER BY "+ OrderBy +" DESC");
			const DWORD GM = Mode % 4;
			while (res && res->next())
				L->ScoreCache.push_back(_ScoreCache(res, GM));
			
			DeleteAndNull(res);

			if (!lBoard[Mode])
				lBoard[Mode] = L;
			else delete L;//Another request beat us to it.

			return lBoard[Mode];
		}
		return 0;
	}

	bool AddScore(const DWORD Mode, _ScoreCache &s, _SQLCon *Con, std::string* Ret) {

		if (RankStatus < RankStatus::RANKED|| s.pp == 0.f)return 0;

		_LeaderBoardCache *L = GetLeaderBoard(Mode, Con);

		if (!L)return 0;

		return L->AddScore(s, BeatmapID, DisplayTitle, Hash, Con,Ret);
	}

};

struct _BeatmapSet{
	DWORD ID;
	DWORD LastUpdate;
	std::vector<_BeatmapData> Maps;
	bool Deleted;

	_BeatmapSet(){
		LastUpdate = INT_MIN;
		ID = 0;
		Deleted = 0;
		Maps.reserve(16);
	}
	_BeatmapSet(const DWORD SetID){
		ID = SetID;
		LastUpdate = INT_MIN;
		Deleted = 0;
		Maps.reserve(16);
	}
	_BeatmapSet(const DWORD SetID, const bool Del) {
		ID = SetID;
		LastUpdate = INT_MIN;
		Deleted = Del;
		Maps.reserve(16);
	}

	~_BeatmapSet(){
		ID = 0;
		LastUpdate = 0;
		Deleted = 0;
	}

	const inline DWORD Key()const noexcept {
		return ID;
	}

};

_LTable<_BeatmapSet> BeatmapSet_Cache;

std::string GetJsonValue(const std::string &Input, const std::string &Param) {
	
	DWORD Start = Input.find("\"" + Param + "\":\"");

	if (Start == std::string::npos)
		return "";

	Start += Param.size() + 4;

	DWORD End = Input.size();
	for (DWORD i = Start; i < Input.size(); i++) {
		if (Input[i] == '\"'){
			End = i;
			break;
		}
	}
	return std::string(Input.begin() + Start, Input.begin() + End);
}

int64_t GetJsonValueInt64(const std::string &Input, const std::string &Param) {

	DWORD Start = Input.find("\"" + Param + "\":\"");

	if (Start == std::string::npos)
		return 0;

	Start += Param.size() + 4;
	DWORD End = Input.size();
	for (DWORD i = Start; i < Input.size(); i++) {
		if ((Input[i] < '0' || Input[i] > '9') && Input[i] != '-') {
			End = i;
			break;
		}
	}

	return MemToInt64(&Input[Start],End - Start);
}


std::vector<std::string> JsonListSplit(const std::string& Input){
	
	VEC(std::string) Return;

	if (Input.size() > 2){

		std::string Current;
		Current.reserve(Input.size());

		int Count = 0;
		bool inQuote = 0;

		for (DWORD i = 1; i < Input.size() - 1; i++) {
			bool Add = 0;
			if (Input[i] == '{') {

				if (inQuote)
					Add = 1;
				else {
					Count++;
					Add = (Count > 1);
				}
			}
			else if (Input[i] == '}') {

				if (inQuote)
					Add = 1;
				else if (Count > 0) {
					Count--;
					if (!Count) {
						Return.push_back(Current);
						Current.clear();
					}

				}
			}
			else if (Count > 0 && Input[i] == '\"') {
				if (Count > 0) {
					inQuote = !inQuote;
					Add = 1;
				}
			}
			else Add = (Count > 0);

			if (Add)
				Current.push_back(Input[i]);
		}

		if (Current.size())
			Return.push_back(Current);

	}
	return Return;
}

std::string ExtractDiffName(const std::string &SRC){

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

	if (!Start || !End)return "";

	return std::string(SRC.begin() + Start, SRC.begin() + End);
}


std::string FileNameClean(const std::string &S){
	std::string Return;
	Return.reserve(S.size());
	
	const char Banned[] = { '/','\\',':','*','?','"', '<','>','|' };
	
	for (DWORD i = 0; i < S.size();i++) {

		bool Add = 1;

		for (DWORD z = 0; z < 9; z++) {
			if (S[i] == Banned[z]) {
				Add = 0;
				break;
			}
		} 

		if (Add)
			Return.push_back(S[i]);
	}

	return Return;
}


//Calling GetBeatmapSetFromSetID can collect setID information from the osu API if it misses in the database
_BeatmapSet *GetBeatmapSetFromSetID(const DWORD SetID, _SQLCon* SQLCon, _BeatmapSet* SET = 0 , bool ForceUpdate = 0){
	const char* mName = "Aria";

	if (!SetID)return 0;

	if(!SET && !ForceUpdate){

		auto Set = BeatmapSet_Cache.get(SetID);
		if (Set)
			return Set;
	}

	if (!SQLCon && !ForceUpdate)
		return 0;

	//Set ID is not in the cache. We need to add it.

	std::shared_mutex& bMutex = BeatmapSet_Cache.getMutex(SetID);

	S_MUTEX_LOCKGUARD(bMutex);

	//Attempt to get it again. Just incase there was another request that already handled it.
	if(!SET && !ForceUpdate){
		auto Set = BeatmapSet_Cache.get(SetID,0);
		if (Set)
			return Set;
	}

	const auto GetMapData = [&](_SQLCon* SQL)->_BeatmapSet*{

		auto res = (SQL) ? SQL->ExecuteQuery("SELECT ranked, beatmap_id, song_name, rating, beatmap_md5 FROM beatmaps WHERE beatmapset_id = " + std::to_string(SetID))
				: 0;

		_BeatmapSet *Set = 0;

		if (res && res->next()){
					
			_BeatmapSet TempSet(SetID);

			Set = SET ? SET : &TempSet;

			if (SET)//TODO: Solve threading issue
				Set->Maps.clear();
			
			int Count = 0;

			do {
				_BeatmapData l;
				l.SetID = SetID;
				l.RankStatus = res->getInt(1);
				l.BeatmapID = res->getInt(2);

				l.DisplayTitle = res->getString(3);
				l.DiffName = FileNameClean(ExtractDiffName(l.DisplayTitle));
				ReplaceAll(l.DisplayTitle, "\n", "|");

				l.Rating = res->getDouble(4);
				l.Hash = res->getString(5);

				if (l.Hash.size() != 32)
					continue;

				Count++;
				if (SET) {
					if (Count == Set->Maps.capacity())//:(
						break;

					if (Count <= Set->Maps.size())
						Set->Maps[Count-1] = l;
					else Set->Maps.push_back(l);

				}
				Set->Maps.push_back(l);

			} while (res && res->next());

			if (!SET)
				Set = BeatmapSet_Cache.push_back(_M(*Set), 0);
			else Set->Maps.resize(Count);
		}

		DeleteAndNull(res);

		return Set;
	};

	_BeatmapSet* Set = (!SET && !ForceUpdate) ? GetMapData(SQLCon) : 0;

	if (!Set && SQLCon){//Failed - Or requesting an updated version.

		LogMessage("Getting beatmap data from the osu!API", mName);

		const std::string ApiRes = GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "s=" + std::to_string(SetID));

		if (ApiRes.size()){
			
			const auto BeatmapData = JsonListSplit(ApiRes);

			if (BeatmapData.size() != 0){

				VEC(DWORD) AddedMaps;
				AddedMaps.reserve(BeatmapData.size());

				for (const auto& Maps : BeatmapData) {

					const std::string MD5 = REMOVEQUOTES(GetJsonValue(Maps, "file_md5"));
					const DWORD beatmap_id = StringToUInt32(GetJsonValue(Maps, "beatmap_id"));

					if (beatmap_id && MD5.size() == 32){

						float diff_size = 0.f, diff_overall = 0.f, diff_approach = 0.f;
						int Length = 0, RankedStatus = 0, MaxCombo = 0, BPM = 0;
						byte mode = 0;

						//"C++ needs exceptions!" - noone

						try { diff_size = std::stof(GetJsonValue(Maps, "diff_size")); }catch (...) {}
						try { diff_overall = std::stof(GetJsonValue(Maps, "diff_overall")); }catch (...) {}
						try { diff_approach = std::stof(GetJsonValue(Maps, "diff_approach")); }catch (...) {}
						try { Length = std::stoi(GetJsonValue(Maps, "hit_length")); }catch (...) {}
						try { RankedStatus = std::stoi(GetJsonValue(Maps, "approved")); }catch (...) {}
						try { MaxCombo = std::stoi(GetJsonValue(Maps, "max_combo")); }catch (...) {}
						try { BPM = std::stoi(GetJsonValue(Maps, "bpm")); }catch (...) {}

						std::string Title = FileNameClean(GetJsonValue(Maps, "artist") + " - " + GetJsonValue(Maps, "title") + " [" + GetJsonValue(Maps, "version") + "]");
						ReplaceAll(Title, "'", "''");

						enum {osuapi_graveyard = -2, osuapi_WIP = -1, osuapi_pending = 0, osuapi_ranked = 1, osuapi_approved = 2, osuapi_qualified = 3, osuapi_loved = 4};

						if (RankedStatus == osuapi_ranked)
							RankedStatus = RANKED;
						else if (RankedStatus == osuapi_qualified)
							RankedStatus = QUALIFIED;

						auto AlreadyThere = SQLCon->ExecuteQuery("SELECT id, ranked FROM beatmaps WHERE beatmap_id = " + std::to_string(beatmap_id) + " LIMIT 1");
						

						if (AlreadyThere && AlreadyThere->next()){

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
				LogMessage(CountString.c_str(), mName);

				LogMessage("Got data. Attempting grab again.", mName);
				Set = GetMapData(SQLCon);

				if (Set) LogMessage("Grab success.", mName);
				else     LogMessage("Grab fail.", mName);
				
			}else{
				LogMessage("SetID has been removed from the osu server\n", mName);


				_BeatmapSet* B = BeatmapSet_Cache.get(SetID, 0);

				if (B)B->Deleted = 1;
				else 
					BeatmapSet_Cache.push_back(_BeatmapSet(SetID,1), 0);
				
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


bool CheckMapUpdate(_BeatmapData *BD, _SQLCon* SQL) {

	if (!BD || !BD->SetID || BD->Hash.size() != 32 || !SQL)
		return 0;

	_BeatmapSet *BS = BeatmapSet_Cache.get(BD->SetID);
	
	if (!BS)
		return 0;

	const DWORD cTime = clock_ms();

	if (BS->LastUpdate + 14400000 > cTime)
		return 0;

	BS->LastUpdate = cTime;
	GetBeatmapSetFromSetID(BD->SetID, SQL, BS, 1);

	return 1;
}


_BeatmapData* GetBeatmapCache(const DWORD SetID, const DWORD BID,const std::string_view MD5, const std::string &&DiffName, _SQLCon* SQL){

	//if (GameMode >= 8)return 0;

	bool ValidMD5 = (MD5.size() == 32);
	bool DiffNameGiven = (DiffName.size() > 0);

	_BeatmapSet *BS = (SetID) ? BeatmapSet_Cache.get(SetID) : 0;

	if (BS && BS->Deleted)
		return 0;

	if(!BS)
		BS = GetBeatmapSetFromSetID((SetID) ? SetID : getSetID_fHash(std::string(MD5), SQL), SQL);

	if (BS){

	int Tries = 0;

	while (Tries < 2) {
		if (!BS->ID)
			return &BeatmapNotSubmitted;

		{

			//S_MUTEX_SHARED_LOCKGUARD(*BS->Lock);

			for (auto& Map : BS->Maps){

				if ((ValidMD5 && Map.Hash == MD5) ||
					(DiffNameGiven && Map.DiffName == DiffName))//TODO: check if the servers md5 is out of date.
					return &Map;
			}

		}
		const DWORD cTime = clock_ms();

		if (BS->LastUpdate + 14400000 < cTime) {//Rate limit to every 4 hours
			BS->LastUpdate = cTime;

			LogMessage("Possible update to beatmap set.", "Aria");

			BS = GetBeatmapSetFromSetID(SetID, SQL, BS);
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
std::vector<_Con> AriaConnectionQue[ARIA_THREAD_COUNT];

std::string unAesString(const std::string &Input, const std::string &K, const std::string &IV) {

	if (!Input.size() || !IV.size() || !K.size())return "";

	char Key[32];
	char Iv[32];
	ZeroMemory(Key, 32);
	ZeroMemory(Iv, 32);

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

_inline std::string GetParam(const std::string& s, const std::string&& param) {

	DWORD Off = s.find(param);
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

	std::vector<std::pair<int, std::string_view>> Params;

	std::string_view get(const int Key)const{

		for (const auto [K, Value] : Params) {

			if (K != Key)continue;

			return Value;
		}

		return std::string_view();
	}

	_GetParams(const std::string_view URL) {

		DWORD Start = URL.find('?');

		if (Start != std::string::npos) {
			Start++;

			bool Name = 1;

			std::string_view TempName;

			for (DWORD i = Start; i < URL.size(); i++) {

				if (Name) {
					if (URL[i] != '=')
						continue;
					TempName = std::string_view((const char*)&URL[Start], i - Start);
					Start = i + 1;
					Name = 0;
				}
				else {

					const bool Last = i == (URL.size() - 1) && ++i;//Special case for the last

					if (!Last && URL[i] != '&')
						continue;

					Params.push_back({
						WeakStringToInt(TempName),
						std::string_view((const char*)&URL[Start],i - Start)
						});
					Start = i + 1;

					Name = 1;
				}
			}
		}
	}

	_GetParams(std::vector<std::pair<int, std::string_view>>&& Params) : Params(_M(Params)) {}
};



void SendAria404(_Con s){

	s.SendData(ConstructResponse(200, Empty_Headers, FastVByteAlloc("<HTML><img src=\"https://cdn.discordapp.com/attachments/385279293007200258/570910676428652544/Kanzaki.png\"><br><b>404</b> - Aria does not know this page.</HTML>")));
	s.Dis();
}

void TryScoreAgain(_Con s){
	s.SendData(ConstructResponse(408, Empty_Headers, Empty_Byte));
	s.Dis();
}
void ScoreFailed(_Con s){
	s.SendData(ConstructResponse(408, Empty_Headers, FastVByteAlloc("error: no")));
	s.Dis();
}

_Achievement GetAchievementsFromScore(const _Score &s, const float StarDiff) {

	_Achievement Ret;
		
	DWORD *const Gen = [&](){
		if (s.GameMode == 0)
			return &Ret.osuGeneral;
		else if (s.GameMode == 1)
			return &Ret.taikoGeneral;
		else if (s.GameMode == 2)
			return &Ret.ctbGeneral;
		return &Ret.maniaGeneral;
	}();

	if (s.MaxCombo >= 500)
		*Gen += AchGeneral::Combo500;
	if (s.MaxCombo >= 750)
		*Gen += AchGeneral::Combo750;
	if (s.MaxCombo >= 1000)
		*Gen += AchGeneral::Combo1000;
	if (s.MaxCombo >= 2000)
		*Gen += AchGeneral::Combo2000;

	int StarCount = al_min(int(StarDiff), 8);

	while (StarCount > 0){
		if (s.FullCombo)
			*Gen += 1 << (11 + StarCount);
		*Gen += 1 << (3 + StarCount);

		StarCount--;
	}

	return Ret;
}

void ScoreServerHandle(const _HttpRes &res, _Con s){
	

	/*std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();
	const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
	printf("%fms\n", double(double(TTime) / 1000000.0));*/

	const char* mName = "Aria";

	//LogMessage("Start reading score.", mName);

	int FailTime = -1;
	int Quit = -1;

	std::string score,
				bmk,//Beatmap hash - used to detect changing the map as it is loading.
				c1,
				osuver,
				clienthash,
				Image,//Used if osu thinks the person is flash light cheating.
				iv,ReplayFile;

	_MD5 pass;

	const auto RawScoreData = EXPLODE_MULTI(std::string, &res.Body[0], res.Body.size(), "-------------------------------28947758029299\r\n");

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

			if (Name == "ft")
				FailTime = MemToInt32(Start, DWORD(End - Start));
			else if (Name == "x")
				Quit = MemToInt32(Start, DWORD(End - Start));
			else if (Name == "iv")
				iv = std::string(Start, End);
			else if (Name == "score"){
				if(FirstScoreParam)
					score = std::string(Start, End);
				else {
					Start += 58;
					End -= _strlen_("-------------------------------28947758029299--");
					if (Start >= End)continue;
					ReplayFile = std::string(Start, End);
				}
				FirstScoreParam = 0;
			}else if (Name == "bmk")
				bmk = std::string(Start, End);
			else if (Name == "c1")
				c1 = std::string(Start, End);
			else if (Name == "bmk")
				bmk = std::string(Start, End);
			else if (Name == "pass")
				pass = _MD5(std::string(Start, End));
			else if (Name == "osuver")
				osuver = std::string(Start, End);
			else if (Name == "s")
				clienthash = std::string(Start, End);
			else if (Name == "i")
				Image = std::string(Start, End);

		}

		if (!iv.size() || !clienthash.size() || !osuver.size() || !score.size()){//something very important is missing.
				LogError("Failed score.","Aria");
				return TryScoreAgain(s);
		}
		const bool OldClient = (FailTime == -1 || Quit == -1);

		if (FailTime == -1)
			FailTime = 0;
		if (Quit == -1)
			Quit = 0;



		iv = base64_decode(iv);
		
		const std::string Key = "osu!-scoreburgr---------" + osuver;

		_Score sData;
		{
			const std::string AES = unAesString(base64_decode(score), Key, iv);

			const auto ScoreData = Explode_View(AES, ':', 18);

			if (ScoreData.size() != 18) {
				const std::string Error = "Score sent with wrong ScoreData length (" + std::to_string(ScoreData.size()) + ")\0";
				LogError(&Error[0], "Aria");
				return ScoreFailed(s);
			}

			sData.Mods = StringToUInt32(ScoreData[score_Mods]);

			if (sData.Mods & (Mods::Relax2 | Mods::Autoplay | (1 << 29)
			#ifdef NO_RELAX
				| Mods::Relax
			#endif
				))
				return ScoreFailed(s);

			sData.BeatmapHash = REMOVEQUOTES(std::string(IT_COPY(ScoreData[scoreOffset::score_FileCheckSum])));

			sData.UserName = ScoreData[scoreOffset::score_PlayerName];
			sData.count300 = StringToInt32(ScoreData[score_Count300]);
			sData.count100 = StringToInt32(ScoreData[score_Count100]);
			sData.count50 = StringToInt32(ScoreData[score_Count50]);
			sData.countGeki = StringToInt32(ScoreData[score_CountGeki]);
			sData.countKatu = StringToInt32(ScoreData[score_CountKatu]);
			sData.countMiss = StringToInt32(ScoreData[score_CountMiss]);
			sData.Score = StringToUInt32(ScoreData[score_totalScore]);
			sData.MaxCombo = StringToInt32(ScoreData[score_maxCombo]);
			sData.FullCombo = (ScoreData[score_Perfect] == "True") ? 1 : 0;
			sData.GameMode = al_min(StringToInt32(ScoreData[score_playMode]),GM_MAX);
			
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

		u.User->Stats[
		#ifndef NO_RELAX
			sData.Mods & Relax ? sData.GameMode + 4 :
		#endif
			sData.GameMode].PlayCount++;


		if (const byte TrueGameMode =
		#ifndef NO_RELAX
			(sData.Mods & Relax) ? sData.GameMode + 4 :
		#endif			
			sData.GameMode;
			(u.User->privileges & UserPublic) && !FailTime && !Quit && ReplayFile.size() > 250){

			_BeatmapData *BD = GetBeatmapCache(0, 0, sData.BeatmapHash, "", &AriaSQL[s.ID]);

			if (!BD) {
				printf(KRED"(%s) ScoreSubmit map failure\n" KRESET,sData.BeatmapHash.c_str());
				return TryScoreAgain(s);
			}
			float PP = 0.f;
			float MapStars = 0.f;
			if (BD->RankStatus == RANKED){
				
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
						if (!OppaiCheckMapDownload(ez, BD->BeatmapID)) {
							printf("Could not download\n");
							return TryScoreAgain(s);
						}
				
					PP = ezpp_pp(ez);

				 MapStars = (sData.Mods & (NoFail | Relax | Relax2)) ? 0.f : ezpp_stars(ez);
				}else u->addQue(bPacket::Notification("That gamemode is currently not supported for pp.\nYour score will still be saved for future calculations."));
			}
			_ScoreCache sc(sData,u.User->UserID,PP);
			
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
				
				_Achievement New = u->Ach;// GetAchievementsFromScore(sData, MapStars);

				CalculateAchievement(New, u->Ach, sData.GameMode, &achievements);//TODO Add ach to DB.
				u->Ach = New;
				if(achievements.size())
					Charts += "|achievements-new: " + achievements + "\n";
				else Charts += "\n";
			}
			
			s.SendData(ConstructResponse(200, Empty_Headers, std::vector<byte>(Charts.begin(), Charts.end())));
			s.Dis();

			if(NewBest && UpdateUserStatsFromDB(&AriaSQL[s.ID], UserID, TrueGameMode, u.User->Stats[TrueGameMode]))
				u.User->addQue(bPacket::UserStats(u.User));

			if (NewBest && sc.ScoreID && ReplayFile.size())
				WriteAllBytes(REPLAY_PATH +std::to_string(sc.ScoreID) + ".osr",&ReplayFile[0], ReplayFile.size());

			return;
		}

		s.SendData(ConstructResponse(200, Empty_Headers, FastVByteAlloc("error: no")));
		s.Dis();

		AriaSQL[s.ID].ExecuteUPDATE(SQL_INSERT(
		#ifndef NO_RELAX
			sData.Mods & Relax ? "scores_relax" :
		#endif
			"scores",
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
		_SQLKey("completed","0"),
		_SQLKey("accuracy",std::to_string(sData.GetAcc())),
		_SQLKey("pp","0")
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

std::string urlDecode(const std::string &SRC) {
	std::string ret;
	ret.reserve(SRC.size());
	DWORD i, ii;
	for (i = 0; i<SRC.length(); i++) {
		if (SRC[i] == '%') {
			if (sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii) != EOF) {
				ret.push_back(char(ii));
				i = i + 2;
			}
		}
		else {
			ret += SRC[i];
		}
	}
	ReplaceAll(ret, "+", " ");

	return ret;
}

void osu_getScores(const _HttpRes &http, _Con s){
	const char* mName = "Aria";


	const auto Params = _GetParams(std::string_view((const char*)&http.Host[0], http.Host.size()));

	const std::string_view BeatmapMD5 = Params.get(_WeakStringToInt_("c"));

	const DWORD SetID = StringToUInt32(Params.get(_WeakStringToInt_("i")));

	if (BeatmapMD5.size() != 32)
		return SendAria404(s);

	_UserRef u(GetUserFromName(urlDecode(std::string(Params.get(_WeakStringToInt_("us"))))),1);//Might want to look into a new urldecode..

	if (!u.User ||  !(_MD5(Params.get(_WeakStringToInt_("ha"))) == u.User->Password))
		return SendAria404(s);

	const DWORD Mods = StringToUInt32(Params.get(_WeakStringToInt_("mods")));
	DWORD Mode = StringToUInt32(Params.get(_WeakStringToInt_("m")));
	const bool CustomClient = (Params.get(_WeakStringToInt_("vv")) != "4");
	const DWORD LType = StringToUInt32(Params.get(_WeakStringToInt_("v")));

#ifndef NO_RELAX

	const std::string ScoreTableName = (Mods & Relax) ? "scores_relax" : "scores";

	if ((u.User->actionMods & Relax) != (Mods & Relax)) {//actionMods is outdated.
		u.User->actionMods = Mods;
		u.User->addQue(bPacket::UserStats(u.User));//Send the updates stats back to the client.
	}

	if (Mods & Relax)Mode += 4;
	if (Mode >= 8)Mode = 0;

#else

	const std::string ScoreTableName = "scores";

#endif
	
	if (u.User->actionID != bStatus::sPlaying && BeatmapMD5.size() == 32)
		memcpy(&u.User->ActionMD5[0], &BeatmapMD5[0], 32);

	_BeatmapData*const BeatData = GetBeatmapCache(SetID,0, BeatmapMD5,
		ExtractDiffName(urlDecode(std::string(Params.get(_WeakStringToInt_("f")))))
		,&AriaSQL[s.ID]);

	if (!BeatData || !BeatData->BeatmapID){

		const std::string BeatmapFailed = "-1|0";

		s.SendData(ConstructResponse(200, Empty_Headers, std::vector<byte>(BeatmapFailed.begin(), BeatmapFailed.end())));
		return s.Dis();
	}


	const bool NeedUpdate = (BeatData && BeatData->Hash != BeatmapMD5);

	if (NeedUpdate)
		CheckMapUpdate(BeatData, &AriaSQL[s.ID]);//TODO: Should probably properly handle this instead of this hack.


	_LeaderBoardCache *const LeaderBoard = BeatData->GetLeaderBoard(Mode, &AriaSQL[s.ID]);

	const DWORD TotalScores = (LeaderBoard) ? LeaderBoard->ScoreCache.size() : 0;
	
	std::string Response = (NeedUpdate) ? "1" : std::to_string(al_max(BeatData->RankStatus,0));
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

		if (Rank){

			_ScoreCache s = LeaderBoard->GetScoreByRank(Rank);

			if (s.UserID != u.User->UserID)
				s = LeaderBoard->GetScoreByUID(u.User->UserID);
			if (s.UserID != 0) {
				Response += std::to_string(s.ScoreID);//online id
				Response += "|" + u.User->Username;//player name
				Response += "|" + std::to_string((Mode > 3) ? int(s.pp + 0.5f) : s.Score);//total score
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

			LeaderBoard->ScoreLock.lock_shared();

			DWORD Rank = 0;

			for (DWORD i = 0; i < LeaderBoard->ScoreCache.size(); i++) {

				if (Rank >= 250)
					break;//Not doing this for performance reasons of the server. Simply the client is not built to handle it.

				const _ScoreCache *lScore = &LeaderBoard->ScoreCache[i];
				
				if (LType == RankingType::SelectedMod && lScore->Mods != Mods)
					continue;

				if (LType == RankingType::Friends && !u.User->isFriend(lScore->UserID))
					continue;

				Rank++;

				if (u.User->isBlocked(lScore->UserID))
					continue;

				Response += "\n" + std::to_string(lScore->ScoreID)//online id
				         + "|" + GetUsernameFromCache(lScore->UserID)//player name
				         + "|" + std::to_string((Mode > 3 || LType == RankingType::Country) ? int(lScore->pp + 0.5f) : lScore->Score)//total score
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

			LeaderBoard->ScoreLock.unlock_shared();
		}
	}

	s.SendData(ConstructResponse(200, Empty_Headers, std::vector<byte>(Response.begin(), Response.end())));
	s.Dis();
}

struct _UpdateCache{
	int Stream;
	DWORD LastTime;
	std::vector<byte> Cache;
	_UpdateCache() {
		Stream = 0;
		LastTime = 0;
	}
}; std::vector<_UpdateCache> UpdateCache;

void osu_checkUpdates(const std::vector<byte> &Req,_Con s){

	if (!MEM_CMP_OFF(Req, "check",_strlen_("/web/check-updates.php?action=")) && !MEM_CMP_OFF(Req, "path", _strlen_("/web/check-updates.php?action=")))
		return SendAria404(s);
	int CacheOffset = -1;

	const std::string URL(Req.begin() + 1, Req.end());

	const int Stream = WeakStringToInt(GetParam(URL, "stream="));
	
	for (DWORD i = 0; i < UpdateCache.size(); i++){

		if (UpdateCache[i].Stream == Stream){

			if (UpdateCache[i].LastTime + 3600000 > clock_ms()){
				s.SendData(ConstructResponse(200, Empty_Headers, UpdateCache[i].Cache));
				return s.Dis();
			}

			CacheOffset = i;
			break;
		}

	}

	const std::string res = GET_WEB_CHUNKED("old.ppy.sh",std::string(Req.begin() + 1, Req.end()));

	s.SendData(ConstructResponse(200, Empty_Headers, std::vector<byte>(res.begin(), res.end())));
	s.Dis();

	_UpdateCache c;

	c.Stream = Stream;
	c.LastTime = clock_ms();
	c.Cache = std::vector<byte>(res.begin(),res.end());

	if (CacheOffset == -1)
		UpdateCache.push_back(c);
	else UpdateCache[CacheOffset] = c;
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

void GetReplay(const std::string &URL, _Con s){

	const uint64_t ScoreID = StringToUInt64(GetParam(URL, "&c="));

	if (!ScoreID)
		return s.Dis();

	const std::vector<byte> Data = LOAD_FILE_RAW(REPLAY_PATH + std::to_string(ScoreID) + ".osr");

	if (!Data.size())
		return s.Dis();

	s.SendData(ConstructResponse(200, Empty_Headers, Data));
	
	return s.Dis();
}

void Thread_DownloadOSZ(const DWORD MapID, _Con s){

	s.SendData(GET_WEB(MIRROR_IP, "d/" + std::to_string(MapID)));

	return s.Dis();
}

static inline uint64_t UnixToDateTime(const int Unix) {	
	return 0x89F7FF5F7B58000 + (int64_t(Unix) * 10000000);
}

void Thread_WebReplay(const uint64_t ID, _Con s) {

	if (!ID)
		return s.Dis();

	_SQLCon *SQL = &SQL_BanchoThread[clock() & 3];

	auto res = SQL->ExecuteQuery("SELECT userid,play_mode, beatmap_md5,300_count, 100_count, 50_count, gekis_count,katus_count, misses_count, score, max_combo, full_combo, mods, time FROM scores" + std::string(ID > 500000000u ? "" : "_relax")
		+ " WHERE id = " + std::to_string(ID));

	if (!res || !res->next()){
		DeleteAndNull(res);
		return s.Dis();
	}
	const std::vector<byte> Data = LOAD_FILE_RAW(REPLAY_PATH + std::to_string(ID) + ".osr");

	if (!Data.size()) {
		DeleteAndNull(res);
		return s.Dis();
	}

	const std::vector<byte> Total = [&] {

		std::vector<byte> Ret;
		Ret.reserve(256 + Total.size());
		
		Ret.push_back(res->getInt(2));//PlayMode
		AddInt(Ret, 20190101);//osu version
		AddString_SQL(Ret, res->getString(3));//beatmap md5
		AddString(Ret, GetUsernameFromCache(res->getInt(1)));//Username
		AddString_SQL(Ret, res->getString(3));//checksum
		AddShort(Ret, res->getInt(4));//count300
		AddShort(Ret, res->getInt(5));//count100
		AddShort(Ret, res->getInt(6));//count50
		AddShort(Ret, res->getInt(7));//countGeki
		AddShort(Ret, res->getInt(8));//countKatu
		AddShort(Ret, res->getInt(9));//countMiss
		AddInt(Ret, res->getUInt(10));//totalScore
		AddShort(Ret, res->getInt(11));//MaxCombo
		Ret.push_back(res->getBoolean(12));//Perfect
		AddInt(Ret, res->getUInt(13));//Mods
		AddString(Ret, "");//life
		AddLong(Ret, UnixToDateTime(res->getUInt(14)));//Date
		AddInt(Ret,Data.size());
		AddVector(Ret, Data, byte);

		AddLong(Ret, ID);

		return Ret;
	}();

	DeleteAndNull(res);

	s.SendData(ConstructResponse(200, {
		{"Content-type","application/octet-stream"},
		{"Content-length",std::to_string(Total.size())},
		{"Content-Description","File Transfer"},
		{"Content-Disposition","attachment; filename=\"" + std::to_string(ID) + ".osr\""}
	}, Total));

	return s.Dis();
}

void Thread_UpdateOSU(const std::string URL, _Con s){

	s.SendData(GET_WEB("old.ppy.sh", _M(URL)));//Their osu clients will unchunk the data for us :)

	return s.Dis();
}

std::string RandomString(const DWORD Count){
	static const char CharList[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
									 'A', 'B', 'C', 'D', 'E', 'F', '_', 'a', 'b', 'c',
									 'd', 'e', 'f', '-', 'r', 'u', 'm', 'o', 'i', 'y', 'g', 'n' };
	std::string rString(Count,'\0');

	for (char& c : rString)
		c = CharList[BR::GetRand(0, 31)];
	
	return rString;
}

#define IT_ADD(s) size_t(&*s)

void UploadScreenshot(const _HttpRes &res, _Con s){
	
	if (res.Body.size() < 1000 || res.Body.size() > 2000000)
		return;

	#define SCREENSHOT_START "Content-Disposition: form-data; name=\"ss\"; filename=\"ss\"\r\nContent-Type: application/octet-stream\r\n\r\n"
	#define SCREENSHOT_END "\r\n-------------------------------28947758029299--"
	
	static const std::string Start = SCREENSHOT_START;

	auto it = std::search(
		std::cbegin(res.Body), std::cend(res.Body),
		std::cbegin(Start), std::cend(Start));

	if (it == std::cend(res.Body))
		return s.Dis();

	it += _strlen_(SCREENSHOT_START);
	const auto end = res.Body.end() - _strlen_(SCREENSHOT_END);

	if (end < it)
		return s.Dis();
	
	const std::string Filename = RandomString(8) + ".png";

	WriteAllBytes("/home/ss/" + Filename, &*it, end - it);

	s.SendData(ConstructResponse(200, {}, VEC(byte)(Filename.begin(), Filename.end())));

	return s.Dis();
}

namespace MIRROR {

	std::mutex MirrorAPILock;
	std::vector<std::pair<const std::string, _Con>> MirrorAPIQue;

	void HandleMirrorAPI(){

		while (1){

			if (MirrorAPIQue.size()){

				MirrorAPILock.lock();

				auto QueCopy = MirrorAPIQue;

				MirrorAPIQue.clear();
				MirrorAPILock.unlock();

				for (auto& [req,Con] : QueCopy){
					Con.SendData(GET_WEB(MIRROR_IP, _M(req)));
					Con.Dis();
				}
			}
			Sleep(10);
		}
	}
}


void RestrictUser(_User* Caller, const std::string UserName, DWORD ID, std::string Reason);

void LastFM(_GetParams&& Params, _Con s){

	enum {
		RelifeRunning = 1 << 0,
		Console = 1 << 1,
		//1<<2 can false flag. Do not use.
		InvalidName = 1 << 3,
		InvalidFile = 1 << 4,
		RelifeLoaded = 1 << 5
	};

	const int Flags = [&]{

		auto B = Params.get(_WeakStringToInt_("b"));
		
		return (B.size() && B[0] == 'a') ? StringToInt32(B) : 0;
	}();


	if (Flags & (RelifeLoaded | Console | InvalidName | InvalidFile | RelifeLoaded)){

		_UserRef u(GetUserFromNameSafe(USERNAMESAFE(std::string(Params.get(_WeakStringToInt_("us"))))), 1);

		if (!(!u) && u->Password == _MD5(Params.get(_WeakStringToInt_("ha"))) && u->privileges & UserPublic){
			
			for(DWORD i=0;i<16;i++)
				u->addQue(bPacket::GenericString(0x69, "What did you think would happen?"));

			const DWORD ID = u->UserID;

			std::string FlagString;

		#define FlagPrint(s) if(Flags & s)FlagString += "|"#s
			FlagPrint(RelifeRunning);
			FlagPrint(Console);
			FlagPrint(InvalidName);
			FlagPrint(InvalidFile);
			FlagPrint(RelifeLoaded);
		#undef FlagPrint

			printf(KYEL "%i> Flags: %s\n" KRESET, ID, FlagString.c_str());

			if (!(u->privileges & AdminDev) && Flags != RelifeLoaded){
				std::thread t(RestrictUser, (_User*)0, "", ID, "Restricted for flags: " + FlagString);
				t.detach();
			}
		}
	}

	s.SendData("HTTP/1.0 200 OK" MNL
			   "Content-Length: 2" MNL
				"Connection: close" DMNL
				"-3");

	return s.Dis();
}

#define IS_NUM(s)!(s < '0' || s > '9')

void HandleAria(_Con s){

	const char* mName = "Aria";

	_HttpRes res;

	if (!s.RecvData(res)){
		s.Dis();
		return LogError("Connection Lost", mName);
	}

	bool DontCloseConnection = 0;
	if (MEM_CMP_START(res.Host, "/web/osu-submit-modular-selector.php")) {

		const int sTime = clock_ms();

		ScoreServerHandle(res, s);

		printf("Score Done in %ims\n", clock_ms() - sTime);

	}
	else if (MEM_CMP_START(res.Host, "/web/check-updates.php")) {
		osu_checkUpdates(res.Host, s);
	}
	else if (MEM_CMP_START(res.Host, "/web/osu-osz2-getscores.php"))
		osu_getScores(res, s);
	else if (MEM_CMP_START(res.Host, "/web/osu-search-set.php")) {

		std::string v;
		v.reserve(8);
		for (DWORD i = res.Host.size() - 1; i < res.Host.size(); i--) {
			if (IS_NUM(res.Host[i]))
				v.push_back(res.Host[i]);
			else break;
		}
		std::reverse(v.begin(), v.end());

		const DWORD SetID = StringToUInt32(v);

		if (SetID) {

			MIRROR::MirrorAPILock.lock();
			MIRROR::MirrorAPIQue.push_back({ "api/set?b=" + std::to_string(SetID), s });
			MIRROR::MirrorAPILock.unlock();

			DontCloseConnection = 1;
		}
	}
	else if (MEM_CMP_START(res.Host, "/web/osu-search.php")) {

		const USHORT Key = *(USHORT*)"&r";
		DWORD Start = 0;

		for (DWORD i = _strlen_("/web/osu-search.php"); i < res.Host.size() - 2; i++) {
			if (*(USHORT*)& res.Host[i] == Key) {
				Start = i + 1;
				break;
			}
		}

		if (Start) {

			MIRROR::MirrorAPILock.lock();
			MIRROR::MirrorAPIQue.push_back({ "api/search?" + std::string(res.Host.begin() + Start, res.Host.end()),s });
			MIRROR::MirrorAPILock.unlock();

			DontCloseConnection = 1;
		}
	}
	else if (MEM_CMP_START(res.Host, "/web/osu-getreplay.php"))
		GetReplay(std::string(res.Host.begin(), res.Host.end()), s);
	else if (MEM_CMP_START(res.Host, "/d/")) {

		std::string v;
		v.reserve(8);

		for (DWORD i = 3; i < res.Host.size(); i++)
			if (IS_NUM(res.Host[i]))
				v.push_back(res.Host[i]);
			else break;

		const DWORD ID = StringToUInt32(v);

		if (ID) {
			std::thread a(Thread_DownloadOSZ, ID, s);
			a.detach();
			DontCloseConnection = 1;
		}
	}
	else if (MEM_CMP_START(res.Host, "/web/maps/")) {//used when updating a single maps .osu
		std::thread a(Thread_UpdateOSU, std::string(res.Host.begin() + 1, res.Host.end()), s);
		a.detach();
		DontCloseConnection = 1;
	}
	else if (MEM_CMP_START(res.Host, "/web/replays/")) {//used when updating a single maps .osu
		std::thread a(Thread_WebReplay, MemToUInt64(&res.Host[_strlen_("/web/replays/")], res.Host.size() - _strlen_("/web/replays/")), s);
		a.detach();
		DontCloseConnection = 1;
	}
	else if (MEM_CMP_START(res.Host, "/web/osu-screenshot.php"))
		UploadScreenshot(res, s);
	else if (MEM_CMP_START(res.Host, "/web/lastfm.php"))
		LastFM(_GetParams(std::string_view((const char*)&res.Host[0],res.Host.size())),s);
	else SendAria404(s);

	if (!DontCloseConnection){
		s.Dis();
		/*const unsigned long long Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - begin).count();
		
		printf(KMAG"Aria> " KRESET "%fms\n", double(double(Time) / 1000000.0));*/
	}
}


void AriaWork(const DWORD ID){

	while (1){

		DWORD Count = 0;
		_Con* Req = 0;

		Count = AriaConnectionQue[ID].size();

		if (Count) {

			AriaThreadLock[ID].lock();

			Count = AriaConnectionQue[ID].size();

			Req = new _Con[Count];

			if (Req)
				memcpy(&Req[0], &AriaConnectionQue[ID][0], Count * sizeof(_Con));

			AriaConnectionQue[ID].clear();

			AriaThreadLock[ID].unlock();
		}

		for (DWORD i = 0; i < Count; i++)
			HandleAria(Req[i]);

		if (Req)delete[] Req;

		Sleep(1);//It being 0 literally hogs an entire core so.. No?
	}

}
bool ARIALOADED = 0;
void Aria_Main(){
	
	UpdateCache.reserve(16);//Really should only be a few anyway.

	const char * mName = "Aria";

	LogMessage("Starting", mName);

	{
		for (DWORD i = 0; i < ARIA_THREAD_COUNT; i++) {
			AriaConnectionQue[i].reserve(64);
			printf("	Aria%i: %i\n", i, int(AriaSQL[i].Connect()));
			std::thread a(AriaWork, i);
			a.detach();
		}
		{
			std::thread a(MIRROR::HandleMirrorAPI);
			a.detach();
		}
	}
	ARIALOADED = 1;


#ifndef LINUX
	SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (listening == INVALID_SOCKET)
		return LogError("Failed to load socket", mName);

	sockaddr_in hint; 
	ZeroMemory(&hint.sin_addr, sizeof(hint.sin_addr));
	hint.sin_family = AF_INET;
	hint.sin_port = htons(ARIAPORT);
	
	::bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);// Sets the socket to listen

	sockaddr_in client;

	DWORD Time = 2500;
	DWORD MPL = MAX_PACKET_LENGTH;

	setsockopt(listening, SOL_SOCKET, SO_RCVTIMEO, (char*)&Time, 4);
	setsockopt(listening, SOL_SOCKET, SO_SNDTIMEO, (char*)&Time, 4);
	setsockopt(listening, SOL_SOCKET, SO_RCVBUF, (char*)&MPL, 4);
#else

	struct sockaddr_un serveraddr;

	SOCKET listening = socket(AF_UNIX, SOCK_STREAM, 0);

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, ARIA_UNIX_SOCKET);

	unlink(ARIA_UNIX_SOCKET);

	if (bind(listening, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr)) < 0) {
		perror("bind() failed");
		return;
	}
	if (listen(listening, SOMAXCONN) < 0) {
		perror("listen() failed");
		return;
	}
	chmod(ARIA_UNIX_SOCKET, S_IRWXU | S_IRWXG | S_IRWXO);

#endif
	DWORD ID = 0;

	while(1){
		
#ifndef LINUX
		int clientSize = sizeof(client);
		ZeroMemory(&client, clientSize);

		SOCKET s = accept(listening, (sockaddr*)&client, &clientSize);
#else
		SOCKET s = accept(listening, 0, 0);
#endif

		if (s == INVALID_SOCKET)continue;

		AriaThreadLock[ID].lock();

		AriaConnectionQue[ID].push_back(_Con(s,ID));

		AriaThreadLock[ID].unlock();

		ID++;
		if (ARIA_THREAD_COUNT >= ID)
			ID = 0;
	}
}