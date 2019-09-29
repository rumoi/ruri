#pragma once

void RecalcSingleUserPP(const u32 ID, _SQLCon& SQL);

namespace User {

	#define Respond(s) {constexpr auto p = PacketBuilder::CT::String_Packet(Packet::Server::notification, s); return Invoker->addQueArray(p);}

	void Restrict(const u32 TargetID, _User* const Invoker, _SQLCon& SQL, std::string* ReasonRaw) noexcept{

		std::unique_ptr<std::string> Reason{ ReasonRaw };

		if ((TargetID < USERID_START) || !Invoker || !(Invoker->privileges & Privileges::AdminManageUsers))
			return;

		const std::string ID_String = std::to_string(TargetID);

		std::unique_ptr<sql::ResultSet> res { SQL.ExecuteQuery("SELECT privileges FROM users WHERE id = " + ID_String + " LIMIT 1") };

		if(!res || !res->next())
			Respond("Could not find the user.");

		u32 Priv = res->getInt(1);

		if(!(Priv & Privileges::UserPublic))
			Respond("They already appear to be restricted.");
		if ((Priv & Privileges::AdminManageUsers) && !(Invoker->privileges & Privileges::AdminDev))
			Respond("Insufficient permission to restrict that user.");

		Priv &= Privileges::UserNormal | Privileges::UserDonor | Privileges::Premium;//Evict them of any special standing, except donor perks.

		//These could be done in one. But it's better to do multiple smaller commands than one large one.

		SQL.ExecuteUPDATE("UPDATE users SET privileges = " + std::to_string(Priv) + ", ban_datetime = " + std::to_string(time(0)) + " WHERE id = " + ID_String);
		SQL.ExecuteUPDATE("UPDATE scores SET pp = 0 WHERE completed = 3 AND userid = " + ID_String);

		if constexpr (RELAX_MODE){
			SQL.ExecuteUPDATE("UPDATE scores_relax SET pp = 0 WHERE completed = 3 AND userid = " + ID_String);
			SQL.ExecuteUPDATE("UPDATE rx_stats SET pp_std = 0, pp_taiko = 0, pp_ctb = 0, pp_mania = 0 WHERE id = " + ID_String);
		}

		SQL.ExecuteUPDATE("UPDATE users_stats SET pp_std = 0, pp_taiko = 0, pp_ctb = 0, pp_mania = 0 WHERE id = " + ID_String);


		for (size_t i = 0; i <= GM_MAX; i++)
			UpdateRank(TargetID, i, 1);

		if(_UserRef Banned(GetUserFromID(TargetID), 1); Banned.User){//Check if the user is online, if so reconnect them to update their new status.
			Banned->privileges = Priv;
			Banned->choToken = 0;
		}
		//We need to clean up leaderboards brought into memory. This is very expensive; luckily restricting people should be relatively rare.

		for (size_t i = 0; i < ARRAY_SIZE(BeatmapSet_Cache); i++)
			if (auto* Set = BeatmapSet_Cache[i]; Set) {

				std::shared_lock L(Set->MapUpdateLock);

				for (auto& Map : Set->Maps){

					for (auto* Leaderboard : Map.lBoard){

						if (!Leaderboard)
							continue;

						std::shared_lock L2(Leaderboard->ScoreLock);

						for (auto it = Leaderboard->ScoreCache.begin(); it != Leaderboard->ScoreCache.end(); ++it)
							if (it->UserID == TargetID) {
								std::rotate(it, it + 1, Leaderboard->ScoreCache.end());
								Leaderboard->ScoreCache.pop_back();
								break;
							}
					}
				}
			}

		
		if (Reason) {

			std::string R = Invoker->Username_Safe + "> " + *Reason;
			ReplaceAll(R, "\'", "\'\'");

			SQL.ExecuteUPDATE("UPDATE users SET notes=CONCAT(COALESCE(notes, ''),'" + R + "') WHERE id = " + ID_String + " LIMIT 1");

		}

		Respond("The deed is done.");
	}

	void UnRestrict(const u32 TargetID, _User* const Invoker, _SQLCon& SQL, std::string* ReasonRaw) noexcept{

		if (ReasonRaw)
			delete ReasonRaw;

		if (!Invoker || !(Invoker->privileges & Privileges::AdminManageUsers))
			return;

		const std::string ID_String = std::to_string(TargetID);

		std::unique_ptr<sql::ResultSet> res{ SQL.ExecuteQuery("SELECT privileges FROM users WHERE id = " + ID_String + " LIMIT 1") };

		if (!res || !res->next())
			Respond("Could not find the user.");

		u32 Priv = res->getInt(1);

		if (Priv & Privileges::UserPublic)
			Respond("User is already not restricted.");

		Priv |= Privileges::UserPublic;

		SQL.ExecuteUPDATE("UPDATE users SET privileges = " + std::to_string(Priv) + " WHERE id = " + ID_String);

		if (_UserRef UnBanned(GetUserFromID(TargetID), 1); UnBanned.User) {
			UnBanned->privileges = Priv;
			UnBanned->choToken = 0;
		}

		RecalcSingleUserPP(TargetID, SQL);

		Respond("User has been unrestricted.");
	}

	u32 ResolveID(const std::string &Username_safe, _SQLCon& SQL){
		std::unique_ptr<sql::ResultSet> res{SQL.ExecuteQuery("SELECT id FROM users WHERE username_safe = '" + Username_safe + "' LIMIT 1")};
		return res && res->next() ? res->getInt(1) : 0;
	}

	#undef Respond

}