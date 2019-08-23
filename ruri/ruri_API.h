#pragma once

extern "C" {
#include "LZ4/lz4.h"
}

enum class API_ID {
	Stats_Get,//
	GlobalLeaderBoard_All,//{byte GM}
	GlobalLeaderBoard_Range,//{byte GM, int Start, int End}
	MapLeaderboard_Get,//{int SID, int BID, byte gm, int Start, int End}
	MapLeaderboard_GetBest,//{int SID, int BID, byte gm, int UserId}
	User_SetPrivileges,//{int UID, int newpriv}
	User_SetSilence,//{int UID, int SilenceEndDate}
	User_SetUsername,//{int UID,}
	User_GetStatus,//{int UID}
	User_GetRank,//{int UID, byte gamemode}
	User_GetAllOnline,//
	Match_GetAll,//
	Match_GetSingle,//{USHORT ID}
	IRC_Send,//{int FromID, string TargetChannel, string Message}
	IRC_RecvUpdates//{int LastTimeStamp, string TargetChannel}
};

void API_MapLeaderboard_Get(int &Count, const char* p, VEC(byte) &Res, _SQLCon *SQL){

	int SID = *(int*)&p[Count]; Count += 4;
	const int BID = *(int*)&p[Count]; Count += 4;
	const byte GM = p[Count];Count++;
	const int Start = *(int*)&p[Count]; Count += 4;
	const int EndRange = *(int*)&p[Count]; Count += 4;

	if (!SID){

		auto res = SQL->ExecuteQuery("SELECT beatmapset_id FROM beatmaps WHERE beatmap_id = " + std::to_string(BID) + " LIMIT 1", 1);

		if (res && res->next())
			SID = res->getUInt(1);
			
		DeleteAndNull(res);

		if(!SID)return AddStream(Res, 0);
	}

	_BeatmapDataRef MapRef;

	_BeatmapData* const BD = GetBeatmapCache(SID, BID, "", "", SQL, MapRef);

	if (BD){

		if (_LeaderBoardCache * const LeaderBoard = BD->GetLeaderBoard(GM, SQL); LeaderBoard) {

			LeaderBoard->ScoreLock.lock_shared();

			const int End = al_min(LeaderBoard->ScoreCache.size(), EndRange);

			Start > End ? AddStream(Res, 0) : AddStream(Res, End - Start);

			for (int i = Start; i < End; i++){

				const _ScoreCache* const Score = &LeaderBoard->ScoreCache[i];

				AddStream(Res, Score->ScoreID);
				AddStream(Res, Score->UserID);
				AddStream(Res, Score->Time);
				AddStream(Res, Score->pp);
				AddStream(Res, Score->Mods);
				AddStream(Res, Score->Score);
				AddStream(Res, Score->count300);
				AddStream(Res, Score->count100);
				AddStream(Res, Score->count50);
				AddStream(Res, Score->countGeki);
				AddStream(Res, Score->countKatu);
				AddStream(Res, Score->countMiss);
				Res.push_back(Score->FullCombo);

			}
			LeaderBoard->ScoreLock.unlock_shared();

		}
	}else return AddStream(Res, 0);
}

void API_GlobalLeaderBoard_Range(int& Count, const char* p, VEC(byte)& Res, _SQLCon* SQL){

	const byte GM = al_min(p[Count],GM_MAX); Count++;
	const int Start = *(int*)&p[Count]; Count += 4;
	const int EndRange = *(int*)&p[Count]; Count += 4;

	RankList[GM].lock_shared();

	const int End = al_min(RankList[GM].List.size(), EndRange);

	Start > End ? AddStream(Res, 0) : AddStream(Res, End - Start);

	for (DWORD i = Start; i < End; i++){
		AddStream(Res, RankList[GM].List[i].ID);
		AddStream(Res, RankList[GM].List[i].PP);
	}

	RankList[GM].unlock_shared();

}

void WorkAPI(_Con s, _SQLCon *SQL){

	DWORD pSize = 0;
	std::vector<byte> p;
	p.reserve(USHORT(-1));

	int pLength = MAX_PACKET_LENGTH;
	do {
		p.resize(pSize + MAX_PACKET_LENGTH);

		pLength = recv(s.s, (char*)& p[pSize], MAX_PACKET_LENGTH, 0);

		if (pLength <= 0)break;

		pSize += pLength;

	} while (pLength == MAX_PACKET_LENGTH);

	if (pSize == 0){
		printf(KRED "API recv failed" KRESET);
		return;
	}

	p.resize(pSize);

	VEC(byte) Res;
	Res.reserve(256);
	
	_UserRef Ref;

	{
		int Count = 0;

		while (Count < p.size()){

			const byte PacketID = *(byte*)&p[Count]; Count++;

			switch (API_ID(PacketID)){

			case API_ID::Stats_Get:
				AddStream(Res, COUNT_CURRENTONLINE);
				AddStream(Res, COUNT_MULTIPLAYER);
				AddStream(Res, COUNT_REQUESTS);

				break;
			case API_ID::GlobalLeaderBoard_Range:
				API_GlobalLeaderBoard_Range(Count, (const char*)& p[0], Res, SQL);
				break;

			case API_ID::MapLeaderboard_Get:
				API_MapLeaderboard_Get(Count, (const char*)&p[0], Res, SQL);
				break;

			case API_ID::User_GetStatus:

				Ref.Reset(GetUserFromID(*(DWORD*)&p[Count]),1);

				Count+=4;

				if (!Ref){
					Res.push_back(-1);
					break;
				}
				Res.push_back(Ref->actionID);
				Res.push_back(Ref->GameMode);
				AddStream(Res, Ref->actionMods);
				AddStream(Res, Ref->BeatmapID);
				AddString(Res, Ref->ActionText);
				if (_User* Spec = Ref->CurrentlySpectating; Spec)
					AddStream(Res, Spec->UserID);
				else AddStream(Res, 0);
				AddStream(Res, Ref->Spectators.size());//TODO: Could send who is spectating them instead of just a number
				AddStream(Res, Ref->CurrentMatchID);
				break;

			case API_ID::User_GetRank:
				AddStream(Res, GetRank(*(int*)&p[Count], p[Count + 4]));
				Count += 5;
				break;

			case API_ID::User_GetAllOnline:
				if (VEC(DWORD) UserIDs;1){
					UserIDs.reserve(MAX_USER_COUNT);

					for (const auto& User : Users)
						if (User.choToken)
							UserIDs.push_back(User.choToken);

					AddStream(Res, int(UserIDs.size()));
					AddVector(Res, _M(UserIDs));

				}
				break;
			default:
				break;
			}		

		}
	}

	if (Res.size()){

		const int MaxSize = LZ4_compressBound(Res.size());

		char* Comp = new char[MaxSize];

		const int compressed_data_size = LZ4_compress_default((const char*)&Res[0], Comp, Res.size(), MaxSize);

		if (compressed_data_size > 0)
			s.SendRawData(Comp, compressed_data_size);
		else
			printf(KRED"LZ4 error\n" KRESET);

		delete[] Comp;
	}
	s.Dis();
}


#if defined(LINUX) && defined(API)

#define API_UNIX_SOCKET "/tmp/ruri_api.sock"

void ruri_API(){

	_SQLCon SQL;
	SQL.Connect();

	struct sockaddr_un serveraddr;

	SOCKET listening = socket(AF_UNIX, SOCK_STREAM, 0);

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, API_UNIX_SOCKET);

	unlink(API_UNIX_SOCKET);

	if (bind(listening, (struct sockaddr*) & serveraddr, SUN_LEN(&serveraddr)) < 0) {
		perror("bind() failed");
		return;
	}
	if (listen(listening, SOMAXCONN) < 0) {
		perror("listen() failed");
		return;
	}
	chmod(API_UNIX_SOCKET, S_IRWXU | S_IRWXG | S_IRWXO);

	printf(KGRN "API Loaded\n" KRESET);

	while (1) {

		SOCKET s = accept(listening, 0, 0);
		if (s == INVALID_SOCKET)continue;


		WorkAPI(_Con(s, 0),&SQL);

	}

}
#endif
