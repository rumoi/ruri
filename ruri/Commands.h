#pragma once

std::string_view TrimString_View(std::string_view s) {

	if (s.size()){

		size_t Start(0),Size(s.size());

		for (size_t i = 0; i < s.size(); i++) {
			if (s[i] != ' ')break;
			Start++;
		}
		for (size_t i = s.size(); i-- > 0;) {
			if (s[i] != ' ')break;
			Size = i;
		}

		s = std::string_view((const char*)&s[Start], Size - Start);

	}

	return s;
}

bool Fetus(const std::string &Target){

	_UserRef u(GetUserFromNameSafe(Target),0);

	if (!u)
		return 0;
	
	using namespace PacketBuilder::CT;

	constexpr auto fPacket = PopulateHeader(Concate(PacketHeader(Packet::Server::sendMessage), Number<byte>(0), Number<byte>(0), Number<byte>(0), Number<int>(0)));

	u->qLock.lock();

	for (DWORD i = 0; i < 16; i++)
		u->addQueArray<0>(fPacket);

	u->qLock.unlock();

	return 1;
}
/*
std::string CFGExploit(const std::string &Target, std::string NewCFGLine){

	_UserRef u(GetUserFromNameSafe(Target),1);
	if (!u.User)return "Could not find user";

	ReplaceAll(NewCFGLine, "\\n", "\n\n");
	u.User->qLock.lock();
	u.User->addQueNonLocking(_BanchoPacket(OPac::server_getAttention));
	for(DWORD i=0;i<10;i++)
		u.User->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess, "# \r\n" + NewCFGLine + "\r\nChatChannels = #osu" + std::to_string(i)));

	u.User->addQueNonLocking(bPacket::Message("BanchoBot",Target,"Do fish blink",0));

	u.User->addQueDelayNonLocking(_DelayedBanchoPacket(1, bPacket::GenericString(OPac::server_channelKicked, "BanchoBot")));

	const _BanchoPacket fPacket = bPacket::Message("", "", "", 0);
	for(DWORD i=0;i<16;i++)
		u.User->AddQueArray<0>(_DelayedBanchoPacket(2, fPacket));
	u.User->qLock.unlock();

	return "Done.";
}*/

int getBeatmapID_fHash(const std::string &H, _SQLCon* c);

bool IsBeatmapIDRanked(const DWORD ID, _SQLCon &SQL){

	if (!ID)return 0;

	sql::ResultSet *res = SQL.ExecuteQuery("SELECT ranked FROM beatmaps where beatmap_id = " + std::to_string(ID) + " LIMIT 1");

	if (!res)return 0;

	const bool Ret = !(!res || !res->next() || res->getInt(1) < 2);

	if (res) delete res;
	return Ret;
}

void RecalcSingleUserPP(const DWORD ID, _SQLCon &SQL){//This is relatively expensive. Obviously if there was an entire pp recalc a per beatmap solution would be much better (and will be made).
	
	if (!ID)return;
	
	//TODO add other modes
	for (DWORD i = 0; i < GM_MAX; i += 4) {

			const DWORD OppaiMode = i % 4;

			sql::ResultSet *res = SQL.ExecuteQuery("SELECT beatmap_md5,max_combo,mods,misses_count,accuracy,id FROM " + std::string(i > 3 ? "scores_relax" : "scores") + " WHERE userid = " + std::to_string(ID) + " AND completed = 3 AND play_mode = " + std::to_string(OppaiMode), 1);

			while (res && res->next()) {

				const DWORD BeatmapID = getBeatmapID_fHash(res->getString(1), &SQL);

				if (!IsBeatmapIDRanked(BeatmapID, SQL))
					continue;

				ezpp_t ez = ezpp_new();

				ezpp_set_mode(ez, OppaiMode);

				ezpp_set_combo(ez, res->getUInt(2));
				ezpp_set_mods(ez, res->getUInt(3));
				ezpp_set_nmiss(ez, res->getInt(4));
				ezpp_set_accuracy_percent(ez, res->getDouble(5));

				if (!OppaiCheckMapDownload(ez, BeatmapID)) {
					ezpp_free(ez);
					continue;
				}

				const float PP = ezpp_pp(ez);
				ezpp_free(ez);

				SQL.ExecuteUPDATE("UPDATE " + std::string(i > 3 ? "scores_relax" : "scores") + " SET pp = " + std::to_string(PP) + " WHERE id = " + std::to_string(res->getInt64(6)), 1);
			}DeleteAndNull(res);

			_UserStats blank;
			blank.Acc = -1.f;
			blank.pp = -1;

			UpdateUserStatsFromDB(&SQL, ID, i, blank);
			printf("TotalEndPP: %i\n", blank.pp);
	}
}

void unRestrictUser(_User* Caller, const std::string UserName, DWORD ID) {

	if (!Caller || !(Caller->privileges & Privileges::AdminManageUsers) || (UserName.size() == 0 && !ID))
		return;

	_SQLCon SQL;

	const auto Respond = [=,&SQL](const std::string&& Mess)->void {
		SQL.Disconnect();
		
		return
			PacketBuilder::Build<Packet::Server::notification, 'm', 's'>(Caller->QueBytes, &Caller->qLock, &Mess);
	};

	if (!SQL.Connect())
		return Respond("Could not connect to the SQL");

	DWORD BanPrivs = -1;

	if (!ID) {

		sql::ResultSet *res = SQL.ExecuteQuery("SELECT id, privileges FROM users WHERE username_safe = '" + UserName + "' LIMIT 1", 1);

		if (!res || !res->next()) {
			DeleteAndNull(res);
			return Respond("Failed to find a user with that name");
		}
		ID = res->getInt(1);
		BanPrivs = res->getUInt(2);
		delete res;
	}
	if (BanPrivs == -1){

		sql::ResultSet *res = SQL.ExecuteQuery("SELECT privileges FROM users WHERE id = " + std::to_string(ID) + " LIMIT 1", 1);

		if (!res || !res->next()) {
			DeleteAndNull(res);
			return Respond("Failed to find a user with that ID");
		}
		BanPrivs = res->getUInt(1);
		delete res;
	}

	if(BanPrivs & Privileges::UserPublic)
		return Respond("They do not appear to be banned.");

	BanPrivs = BanPrivs | Privileges::UserPublic;
	SQL.ExecuteUPDATE("UPDATE users SET privileges = " + std::to_string(BanPrivs) + " WHERE id = " + std::to_string(ID), 1);

	{
		_UserRef UnBanned(GetUserFromID(ID),1);
		if (UnBanned.User) {
			UnBanned->privileges = BanPrivs;
			UnBanned->choToken = 0;
		}
	}

	printf("Doing Full PP recalc on %i\n", ID);
	const int sTime = clock_ms();
	
	RecalcSingleUserPP(ID, SQL);

	printf("PP Recalc time: %i\n", clock_ms() - sTime);

	return Respond("They have been unrestricted.");
}

void RestrictUser(_User* Caller, const std::string UserName, DWORD ID, std::string Reason){
	
	if (!UserName.size() && !ID)
		return;
	
	_SQLCon SQL;

	const auto Respond = [=, &SQL](const std::string& Mess)->void {
		SQL.Disconnect();
		if(Caller)
			PacketBuilder::Build<Packet::Server::notification, 'm', 's'>(Caller->QueBytes, &Caller->qLock, &Mess);
	};

	if (!SQL.Connect())
		return Respond("Could not connect to the SQL");

	DWORD BanPrivs = -1;

	if (!ID){

		sql::ResultSet *res = SQL.ExecuteQuery("SELECT id, privileges FROM users WHERE username_safe = '" + UserName + "' LIMIT 1",1);

		if (!res || !res->next()){
			DeleteAndNull(res);
			return Respond("Failed to find a user with that name");
		}
		ID = res->getInt(1);
		BanPrivs = res->getUInt(2);
		DeleteAndNull(res);
	}
	if (BanPrivs == -1){

		sql::ResultSet *res = SQL.ExecuteQuery("SELECT privileges FROM users WHERE id = " + std::to_string(ID) + " LIMIT 1",1);

		if (!res || !res->next()) {
			DeleteAndNull(res);
			return Respond("Failed to find a user with that ID");
		}
		BanPrivs = res->getUInt(1);
		DeleteAndNull(res);
	}

	if(BanPrivs & Privileges::AdminDev)
		return Respond("Developers can only be demoted directly through the SQL.");

	if(Caller && (BanPrivs & Privileges::AdminManageUsers) && !(Caller->privileges & Privileges::AdminDev) || ID < USERID_START)
		return Respond("You do not have the perms to restrict that user.");

	if (BanPrivs & Privileges::UserPublic){
		BanPrivs = Privileges::UserNormal | (BanPrivs & (Privileges::UserDonor | Privileges::Premium));//Evict them of any special standing, except donor perks.
		
		//These could be done in one. But im pretty sure its better for SQL to do multiple smaller commands than one large one.

		SQL.ExecuteUPDATE("UPDATE users SET privileges = " + std::to_string(BanPrivs) + ", ban_datetime = " + std::to_string(time(0)) + " WHERE id = " + std::to_string(ID),1);
		SQL.ExecuteUPDATE("UPDATE scores SET pp = 0 WHERE completed = 3 AND userid = " + std::to_string(ID),1);
	#ifndef NO_RELAX
		SQL.ExecuteUPDATE("UPDATE scores_relax SET pp = 0 WHERE completed = 3 AND userid = " + std::to_string(ID),1);
		SQL.ExecuteUPDATE("UPDATE rx_stats SET pp_std = 0, pp_taiko = 0, pp_ctb = 0, pp_mania = 0 WHERE id = " + std::to_string(ID), 1);
	#endif
		SQL.ExecuteUPDATE("UPDATE users_stats SET pp_std = 0, pp_taiko = 0, pp_ctb = 0, pp_mania = 0 WHERE id = " + std::to_string(ID), 1);
		
		for(DWORD i=0;i <= GM_MAX;i++)
			UpdateRank(ID, i, 1);
		{
			_UserRef Banned(GetUserFromID(ID), 1);
			//If the user is online reconnect them to update their new status :)
			if (!!Banned) {
				Banned->privileges = BanPrivs;
				Banned->choToken = 0;
			}
		}

		for (size_t i = 0; i < aSize(BeatmapSet_Cache); i++)//This is very expensive. So dont restrict people willy nilly :)
			if (auto* Set = BeatmapSet_Cache[i]; Set){				
				std::shared_lock<std::shared_mutex> L(Set->MapUpdateLock);
				for (auto& Map : Set->Maps) {
					for (auto& Leaderboard : Map.lBoard) {

						if (!Leaderboard)continue;

						std::scoped_lock<std::shared_mutex> lLock(Leaderboard->ScoreLock);

						for (auto it = Leaderboard->ScoreCache.begin(); it != Leaderboard->ScoreCache.end(); ++it) {
							if (it->UserID == ID) {
								std::rotate(it, it + 1, Leaderboard->ScoreCache.end());
								Leaderboard->ScoreCache.pop_back();
								break;
							}
						}
					}
				}
			}

	}else return Respond("They already appear to be restricted.");
	
	if (Reason.size()){

		if (Caller)
			Reason = "\n"+ Caller->Username_Safe + "> " +Reason;
		else
			Reason = "\nSERVER> " + Reason;


		ReplaceAll(Reason, "\'", "\'\'"); 
			   
		//std::string TotalReason = std::chrono::date::format("%F %T\n", time_point_cast<milliseconds>(system_clock::now()));


		SQL.ExecuteUPDATE("UPDATE users SET notes=CONCAT(COALESCE(notes, ''),'" + Reason +"') WHERE id = " + std::to_string(ID) + " LIMIT 1");
	}

	return Respond("The deed is done.");
}

std::string CombineAllNextSplit(DWORD INDEX, const VEC(std::string_view) &SPLIT){

	std::string comString = (SPLIT.size() > INDEX) ? std::string(begin(SPLIT[INDEX]),end(SPLIT[INDEX])) : "";

	for (DWORD i = INDEX + 1; i < SPLIT.size(); i++){
		comString.push_back(' ');
		comString.append(SPLIT[i]);
	}

	return comString;
}

std::string MapStatusUpdate(_User* u, const DWORD RankStatus, DWORD SetID, const DWORD BeatmapID){

	if (!SetID && !BeatmapID)
		return "No BID or SID were given.";
	
	const char NewStatus = [&]{
		switch(RankStatus){
			case _WeakStringToInt_("rank"):
				return RankStatus::RANKED;
			case _WeakStringToInt_("unrank"):
				return RankStatus::PENDING;
			case _WeakStringToInt_("love"):
				return RankStatus::LOVED;
			case _WeakStringToInt_("lock"):
				if(u->privileges & Privileges::AdminDev)
					return RankStatus::RANK_LOCKED;
			default:
				return RankStatus::UNKNOWN;
		}
	}();

	if(NewStatus == RankStatus::UNKNOWN)
		return "Invalid status";

	const std::string StatusString = std::to_string(NewStatus);
	
	_SQLCon *const SQL = &SQL_BanchoThread[clock() & 3];//Rat a random SQL connection from the bancho threads

	sql::ResultSet *res = 0;
	
	if(!BeatmapID)
		 res = SQL->ExecuteQuery("SELECT beatmap_id, song_name FROM beatmaps WHERE beatmapset_id = " + std::to_string(SetID) + ((u->privileges & Privileges::AdminDev) ? "" : " AND ranked != -3"));
	else res = SQL->ExecuteQuery("SELECT beatmapset_id, song_name FROM beatmaps WHERE beatmap_id = " + std::to_string(BeatmapID) + ((u->privileges & Privileges::AdminDev) ? "" : " AND ranked != -3") + " LIMIT 1");
	
	if (!res || !res->next()){
		DeleteAndNull(res);
		return "Could not find any valid maps.";
	}

	if (BeatmapID)
		SetID = res->getUInt(1);
	else SQL->ExecuteUPDATE("UPDATE beatmaps SET ranked = " + StatusString + " WHERE beatmapset_id = " + std::to_string(SetID));

	std::string Announcement = u->ProfileLink() + " has " + [=](){
		if (NewStatus == RankStatus::RANKED)
			return "ranked\n";
		if (NewStatus == RankStatus::PENDING)
			return "unranked\n";
		if (NewStatus == RankStatus::LOVED)
			return "loved\n";
		return "locked\n";
		}();

	if (BeatmapID){
		const std::string sBeatmapID = std::to_string(BeatmapID);
		Announcement += "[https://osu.ppy.sh/b/" + sBeatmapID + " " + res->getString(2) + "]\n";

		SQL->ExecuteUPDATE("UPDATE beatmaps SET ranked = " + StatusString + " WHERE beatmap_id = " + sBeatmapID + " LIMIT 1");

	}else do
		Announcement += "[https://osu.ppy.sh/b/" + res->getString(1) + " " + res->getString(2) + "]\n";
	while (res->next());
	
	DeleteAndNull(res);

	chan_Announce.Bot_SendMessage(Announcement);

	_BeatmapSet* bData = GetBeatmapSetFromSetID(SetID, 0);

	if (bData){
		
		std::shared_lock<std::shared_mutex> L(bData->MapUpdateLock);

		if (BeatmapID){
			for (DWORD i = 0; i < bData->Maps.size(); i++) {
				if (bData->Maps[i].BeatmapID == BeatmapID) {
					bData->Maps[i].RankStatus = NewStatus;
					break;
				}
			}
		}else
			for (DWORD i = 0; i < bData->Maps.size(); i++)
				bData->Maps[i].RankStatus = NewStatus;
			
		

	}

	return "Done.";
}

std::string BlockUser(_User* u, const std::string_view Target, const bool UnBlock){

	auto res = SQL_BanchoThread[clock() & 3].ExecuteQuery("SELECT id from users where username_safe = '" + USERNAMESQL_Ref(std::string(IT_COPY(Target))) + "' LIMIT 1");

	DWORD UserID = 0;

	if (res && res->next())
		UserID = res->getUInt(1);

	DeleteAndNull(res);

	if (UserID < USERID_START)
		return "User not found.";

	if (u->UserID == UserID)
		return "You can not block your self";

	if (UnBlock){
		for (byte i = 0; i < 32; i++)
			if (u->Blocked[i] == UserID)
				u->Blocked[i] = 0;

		return "UserID " + std::to_string(UserID) + " is no longer blocked.";	
	}

	u->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::userLogout,'i'>(UserID));

	return u->AddBlock(UserID) ? "UserID " + std::to_string(UserID) + " is now blocked." : "You can only block 32 players.";
}

void UpdateAllUserStatsinGM(_User* Caller, const DWORD GM){
	
	_SQLCon SQL;

	const auto Respond = [=, &SQL](const std::string&& Mess)->void {
		SQL.Disconnect();
		if (Caller)
			PacketBuilder::Build<Packet::Server::notification, 'm', 's'>(Caller->QueBytes, &Caller->qLock, &Mess);

	};

	if (!SQL.Connect())
		return Respond("Could not connect to the SQL");


	auto res = SQL.ExecuteQuery("SELECT id FROM users WHERE 1");

	while (res->next()){

		const DWORD UID = res->getInt(1);

		UpdateUserStatsFromDB(&SQL, UID, GM, RecalculatingStats);
	}

	ReSortRank(GM);

	return Respond("Finished");
}

//Should be ran only once if a server is switching to ruri.
void FixLoved() {

	_SQLCon SQL;

	SQL.Connect();

	std::vector<std::pair<DWORD, std::string>> LovedMaps;


	auto res = SQL.ExecuteQuery("SELECT beatmap_id, beatmap_md5 FROM beatmaps WHERE ranked = 5",1);


	while (res && res->next())
		LovedMaps.emplace_back(res->getUInt(1), res->getString(2));

	chan_General.Bot_SendMessage(std::to_string(LovedMaps.size()) + " Loved map leaderboards being updated");

	for (const auto& [ID,MD5] : LovedMaps){

		const u32 OkayClang = ID;

		const auto UpdateTable = [OkayClang,&SQL](const std::string&& TableName, const std::string& MODE, const std::string &MD5){

			SQL.ExecuteUPDATE("UPDATE " + TableName + " SET pp = 0.001, score = 0 WHERE beatmap_md5 = '" + MD5 + "'",1);

			auto Score = SQL.ExecuteQuery("SELECT max_combo,mods,misses_count,accuracy,id FROM " + TableName + " WHERE beatmap_md5 = '" + MD5 + "' AND completed = 3 AND play_mode = " + MODE, 1);

			if (!Score || !Score->next()) {
				DeleteAndNull(Score);
				return;
			}
			do {

				ezpp_t ezpp = ezpp_new();
				ezpp_set_mode(ezpp, 0);
				ezpp_set_combo(ezpp, Score->getUInt(1));
				ezpp_set_mods(ezpp, Score->getUInt(2));
				ezpp_set_nmiss(ezpp, Score->getUInt(3));
				ezpp_set_accuracy_percent(ezpp, Score->getDouble(4));

				if (OppaiCheckMapDownload(ezpp, OkayClang))
					SQL.ExecuteUPDATE("UPDATE " + TableName + " SET score = " + std::to_string(int(ezpp->pp + 0.5f)) + " WHERE id = " + Score->getString(5), 1);
				//else { ezpp_free(ezpp); break; }
				ezpp_free(ezpp);

			} while (Score->next());
			DeleteAndNull(Score);
		};
		UpdateTable("scores", "0", MD5);
#ifndef NO_RELAX
		UpdateTable("scores_relax", "0", MD5);
#endif

	}
	DeleteAndNull(res);

	chan_General.Bot_SendMessage("Loved map leaderboards updated");


}

//If for what ever reason scores get out of sync (merging/backing up scores table)
void reCalcScore_Playcount(){

	_SQLCon SQL;
	SQL.Connect();

	VEC(DWORD) Users_ID;
	Users_ID.reserve(UsernameCache.size());

	{
		auto res = SQL.ExecuteQuery("SELECT id FROM users WHERE 1");

		while (res && res->next())
			Users_ID.push_back(res->getInt(1));

		DeleteAndNull(res);
	}

	for (const DWORD UserID : Users_ID){
		
		const std::string stringUID = std::to_string(UserID);
		chan_DevLog.Bot_SendMessage(stringUID);

		const std::string GM[] = { "std","taiko","ctb","mania"};

		for (DWORD i = 0; i < GM_MAX;i++){

			auto res = SQL.ExecuteQuery("SELECT score FROM "+ std::string((i > 3) ? "scores_relax" : "scores") +" WHERE play_mode="+std::to_string(i % 4)+"&&userid=" + stringUID);
			int Total = 0;
			int Count = 0;
			while (res && res->next()){
				Count++;
				Total += res->getInt(1);
			}
			DeleteAndNull(res);
			const std::string tName = (i > 3) ? "rx_stats" : "user_stats";
			SQL.ExecuteUPDATE("UPDATE " + tName +  " SET total_score_"+GM[i % 4]+"=" + std::to_string(Total) + ",playcount_"+GM[i % 4]+"=" + std::to_string(Count));
		}

	}

}

void FullRecalcPP(const std::string GM){

	_SQLCon SQL;

	if (!SQL.Connect())
		return;
	SQL.ExecuteQuery("UPDATE scores SET pp = 0 WHERE completed = 3 AND pp > 0 AND play_mode = " + GM,1);
	chan_General.Bot_SendMessage("Set scores to 0");
#ifndef NO_RELAX
	SQL.ExecuteQuery("UPDATE scores_relax SET pp = 0 WHERE completed = 3 AND pp > 0 AND play_mode = " + GM,1);
	chan_General.Bot_SendMessage("Set scores_relax to 0");
#endif

	struct SD { const std::string Hash; const DWORD BID; };

	std::vector<SD> Maps;

	Maps.reserve(USHORT(-1));
	{
		auto res = SQL.ExecuteQuery("SELECT beatmap_md5, beatmap_id FROM beatmaps WHERE ranked = 2", 1);

		while (res && res->next())
			Maps.push_back({ res->getString(1),res->getUInt(2) });
		DeleteAndNull(res);
	}

	chan_General.Bot_SendMessage("Going through " + std::to_string(Maps.size()) + "@"+GM+" beatmaps.");

	DWORD Count = 1;

	for (const auto& Map : Maps) {

		const std::string MapPath = BeatmapPath + std::to_string(Map.BID) + ".osu";

		Count++;
		if (Count % 1000 == 0)
			chan_General.Bot_SendMessage("Score recalculation is " + RoundTo2((float(Count) / float(Maps.size())) * 100) + "% complete.");

		const auto UpdateTable = [](const std::string&& TableName, _SQLCon &SQL,const SD& MAP, const std::string& MODE){
			auto Score = SQL.ExecuteQuery("SELECT max_combo,mods,misses_count,accuracy,id FROM "+ TableName +" WHERE beatmap_md5 = '" + MAP.Hash + "' AND completed = 3 AND pp = 0 AND play_mode = " + MODE, 1);

			if (!Score || !Score->next()){
				DeleteAndNull(Score);
				return;
			}
			do {

				ezpp_t ezpp = ezpp_new();
				ezpp_set_mode(ezpp, 0);
				ezpp_set_combo(ezpp, Score->getUInt(1));
				ezpp_set_mods(ezpp, Score->getUInt(2));
				ezpp_set_nmiss(ezpp, Score->getUInt(3));
				ezpp_set_accuracy_percent(ezpp, Score->getDouble(4));

				if (OppaiCheckMapDownload(ezpp, MAP.BID))
					SQL.ExecuteUPDATE("UPDATE "+ TableName +" SET pp = " + std::to_string(ezpp->pp) + " WHERE id = " + Score->getString(5),1);
				else{ezpp_free(ezpp);break;}
				ezpp_free(ezpp);

			} while (Score->next());
			DeleteAndNull(Score);
		};
		UpdateTable("scores",SQL, Map,GM);
	#ifndef NO_RELAX
		UpdateTable("scores_relax", SQL, Map,GM);
	#endif
	}
	chan_General.Bot_SendMessage("Finished recalculating PP. Removing PP from banned users.");

	{

		auto res = SQL.ExecuteQuery("SELECT id from users WHERE NOT privileges & 1",1);

		while (res && res->next()){
			const std::string UserID = res->getString(1);
			SQL.ExecuteUPDATE("UPDATE scores SET pp = 0 WHERE completed = 3 AND pp > 0 AND userid = " + UserID,1);
		#ifndef NO_RELAX
			SQL.ExecuteUPDATE("UPDATE scores_relax SET pp = 0 WHERE completed = 3 AND pp > 0 AND userid = " + UserID,1);
			SQL.ExecuteUPDATE("UPDATE rx_stats SET pp_std = 0 AND pp_taiko = 0 AND pp_ctb = 0 AND pp_mania = 0 WHERE id = " + UserID, 1);
		#endif
			SQL.ExecuteUPDATE("UPDATE users_stats SET pp_std = 0 AND pp_taiko = 0 AND pp_ctb = 0 AND pp_mania = 0 WHERE id = " + UserID, 1);
			
		}
		DeleteAndNull(res);		
	}
	SQL.Disconnect();

	chan_General.Bot_SendMessage("Updating ranks");
	UpdateAllUserStatsinGM(0, StringToNum(DWORD, GM));

	chan_General.Bot_SendMessage("Fully Completed :)");
}

const std::string ProcessCommand(_User* u,const std::string_view Command, DWORD &PrivateRes){

	if (!u || Command.size() == 0 || Command[0] != '!')
		return "";

	const DWORD Priv = u->privileges;

	PrivateRes = 1;

	const auto Split = Explode_View(Command, ' ', 8);

	const int CommandHash = WeakStringToInt(Split[0]);
	//User Commands
	{
		switch (CommandHash) {

		case _WeakStringToInt_("!roll"):
			PrivateRes = 0;
				return u->ProfileLink() + " rolled " + std::to_string(BR::GetRand64(0, Split.size() > 1 ? StringToNum(uint64_t, Split[1]) : 100));
		case _WeakStringToInt_("!priv"):
			return std::to_string(Priv);
		case _WeakStringToInt_("!reconnect"):
			u->choToken = 0;
			return "";
		case _WeakStringToInt_("!block"):
			return Split.size() > 1 ? BlockUser(u, Split[1],0) : "!block <username>";
		case _WeakStringToInt_("!unblock"):
			return Split.size() > 1 ? BlockUser(u, Split[1], 1) : "!unblock <username>";
		default:
			break;
		}
	}

	if (u->privileges & Privileges::AdminManageBeatmaps) {
		//BAT Commands
		if (CommandHash == _WeakStringToInt_("!map"))
			return Split.size() > 2 ? MapStatusUpdate(u, WeakStringToInt(Split[1]), StringToNum(DWORD, Split[2]), (Split.size() > 3) ? StringToNum(DWORD, Split[3]) : 0)
			: "!map <rank/love/unrank> <setid> optional<beatmapid>";

	}
	if (!(u->privileges & Privileges::AdminManageUsers))
		return "That is not a command.";

	//Admin Commands
	{
		switch (CommandHash) {

		case _WeakStringToInt_("!restrict"):
		case _WeakStringToInt_("!restrictid"):{
			if (Split.size() < 3)
				return "!restrict(id) <name / id> <reason>";

			const bool RestrictWithName = (Split[0].size() == _strlen_("!restrict"));
			const std::string RestrictName = (RestrictWithName) ? USERNAMESQL(Split[1]) : "";
			const DWORD RestrictID = (!RestrictWithName) ? StringToNum(DWORD,Split[1]) : 0;

			{
				std::thread t(RestrictUser, u, RestrictName, RestrictID,std::string(CombineAllNextSplit(2,Split)));
				t.detach();
			}

			return "";
		}

		case _WeakStringToInt_("!unrestrict"):
		case _WeakStringToInt_("!unrestrictid"):{
			if (Split.size() < 2)
				return "!unrestrict(id) <name / id>";

			const bool RestrictWithName = (Split[0].size() == _strlen_("!unrestrict"));
			const std::string RestrictName = (RestrictWithName) ? USERNAMESQL(Split[1]) : "";
			const DWORD RestrictID = (!RestrictWithName) ? StringToNum(DWORD, Split[1]) : 0;

			{
				std::thread t(unRestrictUser, u, RestrictName, RestrictID);
				t.detach();
			}

			return "";

		}

		case _WeakStringToInt_("!silence"):{
			if (Split.size() != 3)
				return "!silence <username> <time>";

			_UserRef t(GetUserFromNameSafe(USERNAMESQL(Split[1])),1);

			if (!t)
				return "User not found.";

			const int Length = StringToNum(int, Split[2]);

			t->silence_end = time(0) + Length;
			t->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::silenceEnd,'i'>(Length));

			SQLExecQue.AddQue("UPDATE users SET silence_end = " + std::to_string(t.User->silence_end) + " WHERE id = " + std::to_string(t->UserID));

			return "User has been silenced";
		}

		default:
			break;
		}
	}

	if (!(u->privileges & Privileges::AdminDev))
		return "That is not a command.";

	//Dev Commands
	{
		switch (CommandHash) {

		case _WeakStringToInt_("!fetus"):
			return Fetus(USERNAMESAFE(CombineAllNextSplit(1, Split))) ? "deletus." : "Not completus. The name you should collatus.";

		case _WeakStringToInt_("!alert"):
		case _WeakStringToInt_("!alertuser"): {

			const bool SingleTarget = Split[0].size() == _strlen_("!alertuser");
			VEC(byte) Packet;

			const std::string& Noti = CombineAllNextSplit(SingleTarget + 1, Split);

			PacketBuilder::Build<Packet::Server::notification, 's'>(Packet, &Noti);

			if (!SingleTarget) {
				for (auto& User : Users)
					if (User.choToken)
						User.addQueVector(Packet);
			}
			else if (Split.size() > 1) {
				_UserRef Target(GetUserFromNameSafe(USERNAMESQL(Split[1])),1);
				if (!!Target)
					Target.User->addQueVector(Packet);
			}

			return (!SingleTarget) ? "Alerted all online users." : "User has been alerted";
		}

		case _WeakStringToInt_("!rtx"): {

			if (Split.size() < 2)return "No target given";

			_UserRef Target(GetUserFromNameSafe(USERNAMESQL(Split[1])),1);

			if (!Target.User || !Target.User->choToken)
				return "User not found.";
			
			const std::string& Noti = CombineAllNextSplit(2, Split);

			PacketBuilder::Build<Packet::Server::RTX, 'm','s'>(Target->QueBytes,&Target->qLock, &Noti);

			return "You monster.";
		}

		case _WeakStringToInt_("!b"):
			PrivateRes = 0;
			return CombineAllNextSplit(1, Split);

		//case _WeakStringToInt_("!fcfg"):
			//return (Split.size() > 2) ? CFGExploit(USERNAMESQL(Split[1]), CombineAllNextSplit(2, Split)) : "!fcfg <username> <config lines>";

		case _WeakStringToInt_("!cbomb"): {

			if (Split.size() != 3)
				return "!cbomb <username> <count>";

			_UserRef t(GetUserFromNameSafe(USERNAMESQL(Split[1])),1);

			if (t.User) {
				const USHORT Count = StringToNum(USHORT, Split[2]);

				t.User->qLock.lock();

				//for (USHORT i = 0; i < Count; i++)
					//t.User->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess, "#" + std::to_string(i)));

				t.User->qLock.unlock();
			}

			return "okay";
		}

		case _WeakStringToInt_("!sql"):{			
			SQL_BanchoThread[0].ExecuteUPDATE(CombineAllNextSplit(1,Split));
			return "Executed";
		}
		case _WeakStringToInt_("!recalcusers"): {
			
			if(Split.size() == 2){
				std::thread t(UpdateAllUserStatsinGM, u, StringToNum(DWORD, Split[1]));
				t.detach();
			}

			return "running";
		}
		case _WeakStringToInt_("!fullrecalcpp"): {

			chan_General.Bot_SendMessage("A full pp recalc has been started by " + u->ProfileLink());
			{
				std::thread t(FullRecalcPP,"0");
				t.detach();
			}

			return "running";
		}
		case _WeakStringToInt_("!fixloved"): {

			chan_General.Bot_SendMessage(u->Username + " is fixing loved xd");
			{
				std::thread t(FixLoved);
				t.detach();
			}
			return "yes";
		}
		case _WeakStringToInt_("!fixplaycount"): {

			{
				std::thread t(reCalcScore_Playcount);
				t.detach();
			}
			return "yes";
		}
		case _WeakStringToInt_("!rCount"):{
			return std::to_string(u->ref) + "| Slot: " + std::to_string(GetIndex(Users,u)) + "| Name: " + u->Username;
		}
		case _WeakStringToInt_("!top100"):{
			return [&]()->std::string{

				std::string Return;

				for (DWORD i = 0; i < al_min(100, RankList[0].List.size()); i++)
					Return += std::to_string(i + 1) + " - " + GetUsernameFromCache(RankList[0].List[i].ID) + ": " + std::to_string(RankList[0].List[i].PP) + "\n";

				return Return;
			}();
		}
		case _WeakStringToInt_("!setpp"): {
			if (Split.size() != 2)
				return "!cbomb <pp>";

			u->Stats[0].pp = StringToNum(DWORD,Split[1]);
			UpdateRank(u->UserID, 0, u->Stats[0].pp);

			return "Set";
		}
		case _WeakStringToInt_("!test"): {


			using namespace PacketBuilder::CT;

			u->addQueArray(
				PopulateHeader(Concate(PacketHeader(Packet::Server::updateMatch),
					Number<USHORT>(0), Number<USHORT>(0), Number<int>(0)
					, Number<byte>(0xb), Number<byte>(255), Number<byte>(255), Number<byte>(255), Number<byte>(255), Number<byte>(255)
					, Number<USHORT>((USHORT)Packet::Server::ping), Number<byte>(0), Number<int>(0)))
			);

			return "Done";
		}

		default:
			break;
		}
	}

	return "That is not a command.";
}