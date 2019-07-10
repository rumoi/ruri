
enum Privileges{
	UserBanned = 0,
	UserPublic = 1,
	UserNormal = 2 << 0,
	UserDonor = 2 << 1,
	AdminAccessRAP = 2 << 2,
	AdminManageUsers = 2 << 3,
	AdminBanUsers = 2 << 4,
	AdminSilenceUsers = 2 << 5,
	AdminWipeUsers = 2 << 6,
	AdminManageBeatmaps = 2 << 7,
	AdminManageServers = 2 << 8,
	AdminManageSettings = 2 << 9,
	AdminManageBetaKeys = 2 << 10,
	AdminManageReports = 2 << 11,
	AdminManageDocs = 2 << 12,
	AdminManageBadges = 2 << 13,
	AdminViewRAPLogs = 2 << 14,
	AdminManagePrivileges = 2 << 15,
	AdminSendAlerts = 2 << 16,
	AdminChatMod = 2 << 17,
	AdminKickUsers = 2 << 18,
	UserPendingVerification = 2 << 19,
	UserTournamentStaff = 2 << 20,
	AdminDev = 2 << 21,
	Premium = 2 << 22,
	AdminQAT = 2 << 23
};
enum UserType
{
	None = 0,
	Normal = 1,
	BAT = 2,
	Supporter = 4,
	Friend = 8,
	Peppy = 16,
	Tournament = 32
};
enum bStatus
{
	sIdle,
	sAfk,
	sPlaying,
	sEditing,
	sModding,
	sMultiplayer,
	sWatching,
	sUnknown,
	sTesting,
	sSubmitting,
	sPaused,
	sLobby,
	sMultiplaying,
	sOsuDirect
};

enum Mods
{
	noMod = 0,
	NoFail = 1 << 0,
	Easy = 1 << 1,
	//NoVideo              = 1 << 2,
	Hidden = 1 << 3,
	HardRock = 1 << 4,
	SuddenDeath = 1 << 5,
	DoubleTime = 1 << 6,
	Relax = 1 << 7,
	HalfTime = 1 << 8,
	Nightcore = 1 << 9,
	Flashlight = 1 << 10,
	Autoplay = 1 << 11,
	SpunOut = 1 << 12,
	Relax2 = 1 << 13,
	Perfect = 1 << 14,
	Key4 = 1 << 15,
	Key5 = 1 << 16,
	Key6 = 1 << 17,
	Key7 = 1 << 18,
	Key8 = 1 << 19,
	FadeIn = 1 << 20,
	Random = 1 << 21,
	Cinema = 1 << 22,
	Target = 1 << 23,
	Key9 = 1 << 24,
	KeyCoop = 1 << 25,
	Key1 = 1 << 26,
	Key3 = 1 << 27,
	Key2 = 1 << 28,
	LastMod = 1 << 29,
	KeyMod = Key1 | Key2 | Key3 | Key4 | Key5 | Key6 | Key7 | Key8 | Key9 | KeyCoop,
	FreeModAllowed = NoFail | Easy | Hidden | HardRock | SuddenDeath | Flashlight | FadeIn | Relax | Relax2 | SpunOut | KeyMod,
	ScoreIncreaseMods = Hidden | HardRock | DoubleTime | Flashlight | FadeIn,
	TimeAltering = DoubleTime | HalfTime | Nightcore

};

enum RankStatus {
	RANK_LOCKED = -3,
	UNKNOWN = -2,
	NOT_SUBMITTED = -1,
	PENDING = 0,
	NEED_UPDATE = 1,
	RANKED = 2,
	APPROVED = 3,
	QUALIFIED = 4,
	LOVED = 5
};

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KGRY  "\x1B[37m"
#define KRESET "\033[0m"
#define BOT_LOCATION 38

#define MIRROR_IP "5.9.151.156"
#define MIRROR_PORT 80
#define OSU_IP "1.1.1.1"

#define MAX_USER_COUNT 256
#define MAX_USERNAME_LENGTH 19
#define MAX_MULTI_COUNT 64

#define al_min(a, b) ((a) < (b) ? (a) : (b))
#define al_max(a, b) ((a) > (b) ? (a) : (b))

#define MUTEX_LOCKGUARD(a) std::scoped_lock<std::mutex> LOCKGUARD(a)
#define S_MUTEX_SHARED_LOCKGUARD(a) std::shared_lock<std::shared_mutex> LOCKGUARD(a)
#define S_MUTEX_LOCKGUARD(a) std::scoped_lock<std::shared_mutex> LOCKGUARD(a)

#define CHO_VERSION 19

#define _M(a) std::move(a)

const int PING_TIMEOUT_OSU = 80000;//yes thanks peppy
const int FREE_SLOT_TIME = 1800000;
const bool USER_TEST = 0;

enum OPac {
	NULL_PACKET = 1000,
	client_changeAction = 0,
	client_sendPublicMessage = 1,
	client_logout = 2,
	client_requestStatusUpdate = 3,
	client_pong = 4,
	server_userID = 5,
	server_commandError = 6,
	server_sendMessage = 7,
	server_ping = 8,
	server_handleIRCUsernameChange = 9,
	server_handleIRCQuit = 10,
	server_userStats = 11,
	server_userLogout = 12,
	server_spectatorJoined = 13,
	server_spectatorLeft = 14,
	server_spectateFrames = 15,
	client_startSpectating = 16,
	client_stopSpectating = 17,
	client_spectateFrames = 18,
	server_versionUpdate = 19,
	client_errorReport = 20,
	client_cantSpectate = 21,
	server_spectatorCantSpectate = 22,
	server_getAttention = 23,
	server_notification = 24,
	client_sendPrivateMessage = 25,
	server_updateMatch = 26,
	server_newMatch = 27,
	server_disposeMatch = 28,
	client_partLobby = 29,
	client_joinLobby = 30,
	client_createMatch = 31,
	client_joinMatch = 32,
	client_partMatch = 33,
	server_matchJoinSuccess = 36,
	server_matchJoinFail = 37,
	client_matchChangeSlot = 38,
	client_matchReady = 39,
	client_matchLock = 40,
	client_matchChangeSettings = 41,
	server_fellowSpectatorJoined = 42,
	server_fellowSpectatorLeft = 43,
	client_matchStart = 44,
	AllPlayersLoaded = 45,
	server_matchStart = 46,
	client_matchScoreUpdate = 47,
	server_matchScoreUpdate = 48,
	client_matchComplete = 49,
	server_matchTransferHost = 50,
	client_matchChangeMods = 51,
	client_matchLoadComplete = 52,
	server_matchAllPlayersLoaded = 53,
	client_matchNoBeatmap = 54,
	client_matchNotReady = 55,
	client_matchFailed = 56,
	server_matchPlayerFailed = 57,
	server_matchComplete = 58,
	client_matchHasBeatmap = 59,
	client_matchSkipRequest = 60,
	server_matchSkip = 61,
	server_unauthorised = 62,
	client_channelJoin = 63,
	server_channelJoinSuccess = 64,
	server_channelInfo = 65,
	server_channelKicked = 66,
	server_channelAvailableAutojoin = 67,
	client_beatmapInfoRequest = 68,
	server_beatmapInfoReply = 69,
	client_matchTransferHost = 70,
	server_supporterGMT = 71,
	server_friendsList = 72,
	client_friendAdd = 73,
	client_friendRemove = 74,
	server_protocolVersion = 75,
	server_mainMenuIcon = 76,
	client_matchChangeTeam = 77,
	client_channelPart = 78,
	client_receiveUpdates = 79,
	server_topBotnet = 80,
	server_matchPlayerSkipped = 81,
	client_setAwayMessage = 82,
	server_userPanel = 83,
	IRC_only = 84,
	client_userStatsRequest = 85,
	server_restart = 86,
	client_invite = 87,
	server_invite = 88,
	server_channelInfoEnd = 89,
	client_matchChangePassword = 90,
	server_matchChangePassword = 91,
	server_silenceEnd = 92,
	client_specialMatchInfoRequest = 93,
	server_userSilenced = 94,
	server_userPresenceSingle = 95,
	server_userPresenceBundle = 96,
	client_userPresenceRequest = 97,
	client_userPresenceRequestAll = 98,
	client_userToggleBlockNonFriendPM = 99,
	server_userPMBlocked = 100,
	server_targetIsSilenced = 101,
	server_versionUpdateForced = 102,
	server_switchServer = 103,
	server_accountRestricted = 104,
	server_jumpscare = 105,
	client_matchAbort = 106,
	server_switchTourneyServer = 107,
	client_specialJoinMatchChannel = 108,
	client_specialLeaveMatchChannel = 109
};

enum LoginReply {
	Login_Failed = -1
};

const unsigned int MAX_PACKET_LENGTH = 2816;

#ifndef LINUX
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

#define clock_ms clock

#define RURIPORT 420
#define ARIAPORT 421

#define likely(x) x 
#define unlikely(x) x 

#define _inline __forceinline
#else

#include "Linux.h"


#define RURI_UNIX_SOCKET "/tmp/ruri.sock"
#define ARIA_UNIX_SOCKET "/tmp/aria.sock"

#define likely(x)      __builtin_expect(!!(x), 1) 
#define unlikely(x)    __builtin_expect(!!(x), 0) 
#define _inline __attribute__((always_inline))

#endif


#include <thread>

typedef unsigned char byte;

WSADATA wsData;

#include <iostream>
#include <vector>
#include <string>
#include <array>



#ifndef NO_RELAX

#define GM_MAX 7

#else

#define GM_MAX 3

#endif



const static std::string GameModeNames[] = {
	"osu!","osu!taiko","osu!catch","osu!mania"
	"osu!relax","osu!taiko(relax)","osu!catch(relax)","osu!mania(relax)"
};

constexpr size_t _strlen_(const char* s)noexcept{
	return *s ? 1 + _strlen_(s + 1) : 0;
}

#define FastVByteAlloc(x)[&]{const char*const a = x; const std::vector<byte> b(a,a + _strlen_(a) + 1); return b;}()

#define MemToData(TYPE, LOC,LEN)\
	[](const size_t mem, const DWORD size)->TYPE{\
		if(!mem || !size)return 0;\
		TYPE ret = 0;\
		const char*const c = (char*)mem;\
		for(DWORD i=0;i<size;i++){\
			if(c[i] < '0' || c[i] > '9')\
				continue;\
			ret = (ret * 10) + (c[i] - '0');\
		}\
		return (c[0] != '-') ? ret : -ret;\
	}((size_t)LOC,LEN)

#define MemToInt32(LOC,LEN)MemToData(int, LOC, LEN)
#define MemToUInt32(LOC,LEN)MemToData(DWORD, LOC, LEN)

#define MemToUInt64(LOC,LEN)MemToData(uint64_t, LOC, LEN)
#define MemToInt64(LOC,LEN)MemToData(int64_t, LOC, LEN)

#define StringToInt32(s) MemToInt32(&s[0],s.size())
#define StringToUInt32(s) MemToUInt32(&s[0],s.size())
#define StringToUInt64(s) MemToUInt64(&s[0],s.size())

#define DeleteAndNull(s)if(s)delete s;s=0

#define MEM_CMP_START(VECT, STR)\
	[&]()->bool{\
		const char*const rS = STR;\
		const size_t rS_Size = _strlen_(STR);\
		if(rS_Size > VECT.size())return 0;\
\
		for(size_t ii=0; ii < rS_Size;ii++)\
			if(rS[ii] != VECT[ii])return 0;\
\
		return 1;\
	}()
#define MEM_CMP_OFF(VECT, STR, OFF)\
	[&]()->bool{\
		const char*const rS = STR;\
		const size_t rS_Size = _strlen_(rS);\
		if(rS_Size + OFF > VECT.size())return 0;\
\
		for(size_t ii=0; ii < rS_Size;ii++)\
			if(rS[ii] != VECT[OFF + ii])return 0;\
\
		return 1;\
	}()

#define LOAD_FILE(FILENAME)													\
	[&](){																	\
		std::ifstream file(std::string(FILENAME), std::ios::binary | std::ios::ate);		\
		if (!file.is_open())												\
			return std::vector<byte>();										\
																			\
		const size_t pos = file.tellg();									\
		if(!pos)return std::vector<byte>();									\
		std::vector<byte> rVec(pos + 1);									\
																			\
		file.seekg(0, std::ios::beg);										\
		file.read((char*)&rVec[0], pos);									\
		file.close();														\
		rVec[rVec.size()-1] = 0;											\
																			\
		return rVec;														\
	}()
#define LOAD_FILE_RAW(FILENAME)													\
	[&](){																	\
		std::ifstream file(std::string(FILENAME), std::ios::binary | std::ios::ate);		\
		if (!file.is_open())												\
			return std::vector<byte>();										\
																			\
		const size_t pos = file.tellg();									\
		if(!pos)return std::vector<byte>();									\
		std::vector<byte> rVec(pos);										\
																			\
		file.seekg(0, std::ios::beg);										\
		file.read((char*)&rVec[0], pos);									\
		file.close();														\
																			\
		return rVec;														\
	}()


#define VEC(s) std::vector<s>
#define PAIR(a,b) std::pair<a,b>
#define IT_COPY(a) begin(a),end(a)

#define SS_PAIR_VEC VEC(PAIR(std::string, std::string))

const std::string BOT_NAME = "ruri";
const std::string FAKEUSER_NAME = []{const byte a[] = { 226,128,140,226,128,141,0 }; return std::string((char*)a); }();

#include <time.h>

#include <mutex>
#include <shared_mutex>

template<typename T, typename T2>
_inline size_t GetIndex(const T& Start, const T2& End) {
	return (size_t(&End) - size_t(&Start[0])) / sizeof(Start[0]);
}


template<typename T, typename KeyType = DWORD>
struct _LTable {

#define BUCKET_COUNT 64
#define BUCKET_SIZE 32768
	//Failure rate is about ~2 million

#define GETOFF const DWORD OFF = (*(DWORD*)&Key) % BUCKET_COUNT

	std::shared_mutex Lock[BUCKET_COUNT];
	DWORD TableCount[BUCKET_COUNT];
	VEC(T) Table[BUCKET_COUNT];


	_inline T* push_back(const T&& A, const bool Locking = 1) {

		const auto Key = A.Key();

		static_assert((sizeof(Key) >= 4), "_LTable::Key must be at least 4 bytes");

		GETOFF;

		if (Locking)Lock[OFF].lock();
		if (TableCount[OFF] == BUCKET_SIZE) {
			printf("_LTable overflow\n");
			TableCount[OFF] = 0;
		}

		Table[OFF][TableCount[OFF]] = _M(A);

		T* Ret = &Table[OFF][TableCount[OFF]];
		TableCount[OFF]++;
		if (Locking)Lock[OFF].unlock();
		return Ret;
	}

	_inline std::shared_mutex& getMutex(const KeyType& Key){
		GETOFF;
		return Lock[OFF];
	}

	_inline T* get(const KeyType& Key, const bool Locking = 1) {

		GETOFF;

		T* ret = 0;

		if (Locking)Lock[OFF].lock_shared();

		const DWORD Size = TableCount[OFF];

		for (DWORD i = 0; i != Size; i++) {
			if (Table[OFF][i].Key() == Key) {
				ret = &Table[OFF][i];
				break;
			}
		}
		if (Locking)Lock[OFF].unlock_shared();

		return ret;
	}
	_LTable() {

		ZeroMemory(&TableCount[0], BUCKET_COUNT * 4);
		for (DWORD i = 0; i < BUCKET_COUNT; i++)
			Table[i].resize(BUCKET_SIZE);
	}


//#undef BUCKET_COUNT
#undef BUCKET_SIZE
#undef GETOFF
};


#include "SQL.h"
#define BANCHO_THREAD_COUNT 4
_SQLCon SQL_BanchoThread[BANCHO_THREAD_COUNT];

#include "BCrypt/BCrypt.hpp"

#include <time.h>
#include <random>


std::string GET_WEB(const std::string &&HostName, const std::string &&Page) {

	SOCKET Socket_WEB = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!Socket_WEB)
		return "";
	
	struct hostent *hp = gethostbyname(HostName.c_str());

	if (!hp){
		closesocket(Socket_WEB);
		return "";
	}

	SOCKADDR_IN SockAddr_WEB;
	SockAddr_WEB.sin_family = AF_INET;
	SockAddr_WEB.sin_port = htons(80);
	SockAddr_WEB.sin_addr.s_addr = *(unsigned long*)hp->h_addr;
		
	if (connect(Socket_WEB, (SOCKADDR*)(&SockAddr_WEB), sizeof(SockAddr_WEB))){
		closesocket(Socket_WEB);
		return "";
	}

	const std::string pData = "GET /" + Page + " HTTP/1.1\r\nHost: " + HostName + "\r\nConnection: close\r\n\r\n";

	if (send(Socket_WEB, (char*)&pData[0], pData.size(), 0) == SOCKET_ERROR){
		closesocket(Socket_WEB); return "";
	}
	
	int tSize = 0; std::string Return_WEB(MAX_PACKET_LENGTH, '\0'); \
		while (1) {
			const int rByte = recv(Socket_WEB, &Return_WEB[tSize], MAX_PACKET_LENGTH, 0);
			if (rByte == 0)
				break;
			if (rByte < 0)
				return "";

			Return_WEB.resize(Return_WEB.size() + rByte);
			tSize += rByte;
		}

	closesocket(Socket_WEB);
	Return_WEB.resize(tSize);

	return Return_WEB;
}

int CharHexToDecimal(const char c) {

	if (c >= '0' && c <= '9')return c - '0';

	if (c >= 'A' && c <= 'F')return 10 + (c - 'A');

	if (c >= 'a' && c <= 'f')return 10 + (c - 'a');

	return 0;
}
_inline int DecimalToHex(const char c) {
	if (c <= 0x9)return '0' + c;
	if (c <= 0xF)return 'a' + (c - 10);

	return ' ';
}

std::string GET_WEB_CHUNKED(const std::string &&HostName, const std::string &&Page){

	const std::string rp = GET_WEB(_M(HostName), _M(Page));

	if (rp.size() == 0)
		return "";

	std::string p; p.reserve(rp.size());
	DWORD Start = rp.find("\r\n\r\n");
	if (Start == std::string::npos)
		return "";

	const USHORT rn = *(USHORT*)"\r\n";

	for (DWORD i = Start + 4; i < rp.size(); i++){
		Start = i;
		while (*(USHORT*)&rp[i] != rn)i++;
		DWORD ChunkSize = 0;
		for (DWORD z = Start; z < i; z++){
			ChunkSize = ChunkSize << 4;
			ChunkSize += CharHexToDecimal(rp[z]);
		}

		if (ChunkSize == 0)break;

		i += 2;

		if (ChunkSize + i > rp.size())
			return "";

		p.resize(p.size() + ChunkSize); memcpy(&p[p.size() - ChunkSize], &rp[i], ChunkSize);
		i += ChunkSize + 1;
	}

	return p;
}



#define WeakStringToInt(s)\
	[&](){\
		int Return = 0;\
		for (DWORD i = 0; i < s.size(); i++)\
			 Return += (s[i] ^ i) << (((i << 4) % 32));\
		return Return;\
	}()

constexpr int _WeakStringToInt_(const char* c,DWORD O = 0, int CurrentValue = 0) noexcept{	
	return (c[O]) ? _WeakStringToInt_(c,O+1, CurrentValue + ((c[O] ^ O) << (((O << 4) % 32)))) : CurrentValue;
}


struct _Timer{

	const char*const Name;
	DWORD Count;
	std::chrono::steady_clock::time_point sTime;
	
	_Timer(const char*const Name) :Name(Name) { Count = 0; };

	void Start(){
		sTime = std::chrono::steady_clock::now();
	}
	void Stop(const DWORD Num = 0){


		const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
		printf(KYEL "%s%i> " KRESET "%fms\n",Name, (!Num) ? Count : Num, double(double(TTime) / 1000000.0));
		if(!Num)Count++;
		Start();
	}

};

std::vector<std::string> UsernameCache;

std::shared_mutex UsernameCacheLock;

void UpdateUsernameCache(_SQLCon *SQL){

	UsernameCacheLock.lock();

	sql::ResultSet* res(SQL->ExecuteQuery("SELECT id,username FROM users WHERE id >= " + std::to_string(UsernameCache.size() + 1000) + " ORDER BY id DESC"));
	DWORD Count = 0;
	if (res && res->next()){
		Count = 1;

		const DWORD HighestID = res->getUInt(1);
		if (HighestID >= 1000) {

			UsernameCache.resize(HighestID - 999);
			UsernameCache[HighestID-1000] = res->getString(2);

			while (res->next()) {
				const DWORD ID = res->getUInt(1);
				if (ID < 1000)break;
				Count++;
				UsernameCache[ID - 1000] = res->getString(2);
			}
		}
	}
	DeleteAndNull(res);
	
	UsernameCacheLock.unlock();

	printf("Updated %i usernames in the cache\n", Count);
}
//This will return [clan]username if clans are enabled.
std::string GetUsernameFromCache(const DWORD UID){

	
	std::string Res = "";

	if (UID >= 1000){
		UsernameCacheLock.lock_shared();
		if ((UID - 1000) < UsernameCache.size()) {
			Res = UsernameCache[UID - 1000];
		}
		UsernameCacheLock.unlock_shared();
	}
	return Res;
}
void UsernameCacheUpdateName(const DWORD UID, const std::string &s, _SQLCon *SQL){

	if (UID < 1000)
		return;

	UsernameCacheLock.lock_shared();

	if (UID - 1000 >= UsernameCache.size()){
		UsernameCacheLock.unlock_shared();
		return UpdateUsernameCache(SQL);
	}
	UsernameCache[UID - 1000] = s;

	UsernameCacheLock.unlock_shared();
}

std::string BEATMAP_PATH;
std::string REPLAY_PATH;
std::string osu_API_KEY;
std::string osu_API_BEATMAP;

struct _RankList {
	DWORD ID;
	DWORD PP;
	_RankList() {
		ID = 0;
		PP = 0;
	}
	_RankList(DWORD I, DWORD P) {
		ID = I;
		PP = P;
	}
}; std::vector<_RankList> RankList[GM_MAX + 1];
DWORD RankListVersion[] = { 1,1,1,1,1,1,1,1 };
std::shared_mutex RankUpdate[GM_MAX + 1];

void ReSortAllRank() {

	for (DWORD i = 0; i < 8; i++){

		RankUpdate[i].lock();
		RankListVersion[i]++;

		std::sort(RankList[i].begin(), RankList[i].end(),
			[](const _RankList& A, const _RankList &B){
				return A.PP > B.PP;
			});

		RankUpdate[i].unlock();

	}

}
void ReSortRank(const DWORD i) {

	if (i >= 8)
		return;

	RankUpdate[i].lock();
	RankListVersion[i]++;

	std::sort(RankList[i].begin(), RankList[i].end(),
		[](const _RankList& A, const _RankList &B) {
		return A.PP > B.PP;
	});

	RankUpdate[i].unlock();

}
struct _SQLQue{
	std::mutex CommandLock;
	std::vector<std::string> Commands;


	void AddQue(const std::string &s){
		CommandLock.lock();		
		Commands.push_back(s);
		CommandLock.unlock();
	}
	void AddQue(const std::string&& s) {
		CommandLock.lock();
		Commands.push_back(_M(s));
		CommandLock.unlock();
	}

}; _SQLQue SQLExecQue;


DWORD GetRank(const DWORD UserID, const DWORD GameMode){//Could use the cached pp to tell around where its supposed to be?

	if (UserID < 1000 || GameMode > GM_MAX || RankList[GameMode].size() == 0)
		return 0;

	DWORD Rank = 0;
	
	RankUpdate[GameMode].lock_shared();

	for (DWORD i = 0; i < RankList[GameMode].size(); i++){
		if (RankList[GameMode][i].ID == UserID){
			Rank = i + 1;
			break;
		}
	}

	RankUpdate[GameMode].unlock_shared();

	return Rank;
}

void UpdateRank(const DWORD UserID, const DWORD GameMode, const DWORD PP){

	if (!PP || UserID < 1000 || GameMode >= 8)return;

	RankUpdate[GameMode].lock();
	
	std::vector<_RankList> &cRankList = RankList[GameMode];


	enum { Nothing, Forward, Backward, NoUpdate };

	byte Direction = 0;
	DWORD OrigPos = 0;

	const _RankList n = { UserID,PP };

	for (DWORD i = 0; i < cRankList.size(); i++) {
		if (cRankList[i].ID == UserID) {
			Direction = (cRankList[i].PP > PP) ? Backward : Forward;
			if (cRankList[i].PP == PP)
				Direction = NoUpdate;
			OrigPos = i;
			break;
		}
	}

	if (Direction == Forward || Direction == Backward){

		DWORD NewPos = 0;
		if (Direction == Forward){
			if (OrigPos){
				for (DWORD i = OrigPos - 1; i > 0; i--)
					if (cRankList[i].PP > PP){
						NewPos = i + 1;
						break;
					}
				if (NewPos != OrigPos){
					memcpy(&cRankList[NewPos + 1], &cRankList[NewPos],(OrigPos - NewPos) * sizeof(_RankList));
					cRankList[NewPos] = n;
					RankListVersion[GameMode]++;
				}else cRankList[OrigPos] = n;

			}else cRankList[0] = n;//Rank one improving themself requires no extra calculations.

		}else if(Direction == Backward){
			if (PP){
				for (DWORD i = OrigPos; i < cRankList.size(); i++)
					if (cRankList[i].PP < PP) {
						NewPos = i - 1;
						break;
					}
			}else NewPos = cRankList.size() - 1;

			if (!NewPos)
				NewPos = cRankList.size() - 1;

			if (NewPos != OrigPos){
				memcpy(&cRankList[OrigPos], &cRankList[OrigPos + 1], (NewPos - OrigPos) * sizeof(_RankList));
				cRankList[NewPos] = n;
				RankListVersion[GameMode]++;
			}else cRankList[OrigPos] = n;

			for (DWORD i = cRankList.size(); i > 0; i--){//Pop off 0 pp players
				if (cRankList[i - 1].PP)break;
				cRankList.pop_back();
			}

		}
	}else if (Direction == Nothing && PP){//User is not currently in the rank cache.
		cRankList.push_back(n);
		std::sort(cRankList.begin(), cRankList.end(), [](const _RankList &i, const _RankList &j) {return (i.PP > j.PP); });//I am lazy.
		RankListVersion[GameMode]++;
	}
	RankUpdate[GameMode].unlock();
	printf("Updated ranks gm:%i\n",GameMode);
}


namespace BR {
	std::random_device randomDeviceInit;	
	std::mt19937 mersenneTwister = std::mt19937(randomDeviceInit());

	int GetRand(int min = INT_MIN, int max = INT_MAX) {
		return std::uniform_int_distribution<int>{min, max}(mersenneTwister);
	}
	uint64_t GetRand64(uint64_t min = 0, uint64_t max = uint64_t(-1)) {
		return std::uniform_int_distribution<uint64_t>{min, max}(mersenneTwister);
	}
};


void LogError(int i){
	printf(KRED "ERROR CODE: %i\n" KRESET, i);
}

void LogError(const char * t, const char* f = 0){
	if(f)return (void)printf(KMAG "%s> " KRED "ERROR: %s\n" KRESET,f, t);
	printf(KRED "ERROR: %s\n" KRESET, t);
}

void LogMessage(const char* t, const char* f = 0) {
	if (f)return (void)printf(KMAG "%s> " KRESET "%s\n" KRESET, f, t);
	printf(KMAG "Log: %s\n" KRESET, t);
}

void LogMessage(std::string t) {
	printf(KMAG "Log: %s\n" KRESET, t.c_str());
}

void ReplaceAll(std::string &str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos){
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}
template<typename T>
	_inline void AddStream(VEC(byte)&v, const T V){
		v.resize(v.size() + sizeof(V));
		*(T*)&v[v.size() - sizeof(V)] = V;
	}

template<typename T>
	_inline void AddVector(VEC(byte) &v, const T& Value){
		if (unlikely(Value.size() == 0))
			return;
		const size_t Size = Value.size() * sizeof(Value[0]);
		v.resize(v.size() + Size);
		memcpy(&v[v.size() - Size], &Value[0], Size);
	}

_inline void AddMem(std::vector<byte> &v, const void* Value, const DWORD Size) {
	if (Size == 0)return;
	v.resize(v.size() + Size);
	memcpy(&v[v.size() - Size], Value, Size);
}

void AddUleb(VEC(byte) &v, DWORD s) {

	if (s) {
		uint64_t ret = 2 + (0x0b << 8) + ((s & 0x7f) << 16);

		while (unlikely(*&s >>= 7)) {
			ret += (uint64_t(0x80) << ((ret & 0xff) << 3)) + 1;
			ret += (uint64_t(s & 0x7f) << ((ret & 0xff) << 3));
		}

		v.resize(v.size() + 8);
		*(uint64_t*)(&v[v.size() - 8]) = ret >> 8;
		v.resize(v.size() - (8 - (ret & 0xff)));

	}
	else v.push_back(0);

}

_inline void AddString(std::vector<byte> &v, const std::string_view Value){
	const DWORD Size = Value.size();
	AddUleb(v, Size);
	if (Size) {
		v.resize(v.size() + Size);
		memcpy(&v[v.size() - Size], &Value[0], Size);
	}
}
_inline void AddString_SQL(std::vector<byte>& v, const sql::SQLString &&Value) {
	const DWORD Size = Value.length();
	AddUleb(v, Size);
	if (Size){
		v.resize(v.size() + Size);
		memcpy(&v[v.size() - Size], &Value[0], Size);
	}
}

struct _BanchoPacket {
	short Type;
	byte Compression;
	std::vector<byte> Data;

	std::vector<byte> GetBytes() const{
		const DWORD DataSize = Data.size();
		std::vector<byte> Bytes(DataSize + 7);

		byte* D = &Bytes[0];
		
		*(USHORT*)(D) = Type;
		*(byte*)(D + 2) = Compression;
		*(DWORD*)(D + 3) = DataSize;

		if (DataSize)
			memcpy(D + 7, &Data[0], DataSize);

		return Bytes;
	}

	const size_t GetSize()const {
		return 7 + Data.size();
	}
	void GetBytes(std::vector<byte> &Bytes) const{

		Bytes.reserve(Bytes.size() + 7 + Data.size());
		byte D[7];
		
		*(USHORT*)D = Type;
		D[2] = Compression;
		*(DWORD*)(D + 3) = Data.size();
		AddMem(Bytes, D, 7);
		AddVector(Bytes, Data);
	}
	void GetBytesRaw(byte* &D) const{
		
		const DWORD DataSize = Data.size();
		*(USHORT*)(D) = Type;
		*(byte*)(D + 2) = Compression;
		*(DWORD*)(D + 3) = DataSize;

		if (DataSize)
			memcpy(D + 7, &Data[0], DataSize);

		D += 7 + DataSize;
	}

	_BanchoPacket() {
		Type = 0;
		Compression = 0;
	}

	template<typename T>
	_BanchoPacket(const short ID, T&& v) : Data(std::forward<T>(v)) {
		Type = ID;
		Compression = 0;
	}

	_BanchoPacket(const short ID, const DWORD vSize = 0) {
		Type = ID;
		Compression = 0;
		Data.resize(vSize);
	}


};
const _BanchoPacket erPacket(NULL_PACKET);

struct _DelayedBanchoPacket{
	int Time;
	_BanchoPacket b;

	_DelayedBanchoPacket() {
		Time = 0;
	}
	_DelayedBanchoPacket(const int t, const _BanchoPacket a) {
		Time = t;
		b = a;
	}

};

struct _UserStats {

private:
	DWORD RankVersion;
	DWORD GameRank;
public:

	unsigned long long tScore;
	unsigned long long rScore;
	int pp;
	float Acc;
	int PlayCount;

	DWORD getRank(const DWORD GameMode, const DWORD UID){
		if (GameMode >= 8)
			return 0;

		const DWORD cVersion = RankListVersion[GameMode];

		if (cVersion != RankVersion){
			RankVersion = cVersion;
			GameRank = GetRank(UID, GameMode);
		}

		return GameRank;
	}

	_UserStats() {
		tScore = 0;
		rScore = 0;
		pp = 0;
		Acc = 0;
		PlayCount = 0;
		GameRank = 0;
		RankVersion = 0;
	}
	void reset(){
		RankVersion = 0;
	}
};

#define EXPLODE_MULTI(TYPE,DATA,LEN,DELMNT)[](const char*const d,const size_t Size)->VEC(TYPE){\
	VEC(TYPE) Output;\
	if(!d || !Size)\
		return Output;\
	const char* Deli = DELMNT;\
	const size_t DeliSize = _strlen_(Deli);\
	const char* Start = d;\
	for(size_t i = 0; i< Size - (DeliSize - 1);i++){\
		for(size_t z = 0; z < DeliSize; z++){\
			if(d[i + z] != Deli[z])break;\
			if(z != DeliSize - 1)continue;\
			Output.push_back(TYPE(Start,d + i));\
			i += DeliSize - 1;\
			Start = d + i + 1;\
		}\
	}\
	if(Start != d + Size)\
		Output.push_back(TYPE(Start,d + Size));\
	return Output;\
	}((const char*const)DATA,LEN)

template<typename T>
const VEC(std::string_view) Explode_View(const T& Input, const char Delim, const DWORD ExpectedSize) {

	VEC(std::string_view) Views;

	if (Input.size()) {/*
					  This branch has to be here.
					  If a std::string_view is passed into Explode_View and the reference operator is called (&)
					  with a square bracket array ([i]) It will offset the reference to the reference and NOT the original reference point.
					  This will crash on the first &Input[0] if the size is 0 as well - it will try to read unintalized memory;
					  */
		Views.reserve(ExpectedSize);

		size_t Start = size_t(&Input[0]);

		for (const auto& c : Input) {//This needs to be auto. Setting it to anything else could cause an implicit cast - making the reference a local.

			if (c == Delim) {
				Views.push_back(std::string_view((const char*)Start, size_t(&c) - size_t(Start)));
				Start = size_t(&c) + 1;
			}

		}

		if (size_t End = size_t(&Input[0]) + Input.size(); Start != End)
			Views.push_back(std::string_view((const char*)Start, End - Start));
	}
	return Views;
}

#define _READINT32(s) [](const char* sP){					\
				if(!sP)return 0;							\
				const bool Negative = (*sP == '-');			\
				if(Negative)sP++;							\
															\
				int r = 0;									\
															\
				while(*sP >= '0' && *sP <= '9'){			\
					r = (r * 10) + (*sP - '0');				\
					sP++;									\
				}											\
				return (Negative) ? -r : r;					\
			}(s)

#define MD5CMP(x,y) (memcmp(x,y,32) == 0)

_inline std::string MenuClickButton(const std::string &&Command, const std::string &&Desc) {
	return "[osump://0/" + Command + "|" + FAKEUSER_NAME + " " + Desc + "]";
}


_inline void AddStringToVector(std::vector<byte> &v, const std::string &s){
	if (s.size() == 0)return;
	v.resize(v.size() + s.size());
	memcpy(&v[v.size() - s.size()], &s[0], s.size());
}

#include "Connection.h"


const SS_PAIR_VEC Empty_Headers = {};

const std::vector<byte> Empty_Byte;


_UserStats RecalculatingStats;

bool UpdateUserStatsFromDB(_SQLCon *SQL,const DWORD UserID, DWORD GameMode, _UserStats &stats){

	if (GameMode >= 8)return 0;
	const DWORD RawGameMode = GameMode;
	const std::string ScoreTableName = (GameMode >= 4) ? "scores_relax" : "scores";
	const std::string StatsTableName = (GameMode >= 4) ? "rx_stats" : "users_stats";
	
	const static std::string AccColName[] = {"avg_accuracy_std","avg_accuracy_taiko","avg_accuracy_ctb","avg_accuracy_mania"};
	const static std::string ppColName[] = { "pp_std","pp_taiko","pp_ctb","pp_mania" };

	GameMode = GameMode % 4;

	sql::ResultSet *res = SQL->ExecuteQuery("SELECT accuracy,pp FROM " + ScoreTableName + " WHERE userid = " + std::to_string(UserID) + " AND play_mode = " + std::to_string(GameMode) + " AND completed = 3 AND pp > 0 ORDER BY pp DESC LIMIT 100");
	DWORD Count = 0;
	double TotalAcc = 0.;
	double TotalPP = 0.;

	while (res && res->next()){
		if(Count < 50)
			TotalAcc += res->getDouble(1);
		TotalPP += res->getDouble(2) * std::pow(0.95,double(Count));
		Count++;
	}
	DeleteAndNull(res);

	const double ACC = (!Count) ? 0.f : (TotalAcc / double((Count > 50) ? 50 : Count));
	
	const float acc = (!Count) ? 0.f : ACC / 100.f;
	const int pp = (int)round(TotalPP);

	if (acc != stats.Acc || pp != stats.pp || &stats == &RecalculatingStats){

		if (pp != stats.pp && &stats != &RecalculatingStats)
			UpdateRank(UserID, RawGameMode, pp);

		stats.pp = pp;
		stats.Acc = acc;

		SQLExecQue.AddQue("UPDATE " + StatsTableName + " SET " + AccColName[GameMode] + " = " + std::to_string(ACC) + ", "
			+ ppColName[GameMode] + " = " + std::to_string(TotalPP)
			+ " WHERE id = " + std::to_string(UserID) + " LIMIT 1");
	}

	return 1;
}

enum MenuStates {
	Menu_None,
	Menu_ChooseScore

};

struct _Menu {
	DWORD State;
	int PreviousMap;
	int ChangePage;
	int LastPage;
	int aux1;
	std::string saux1;
	bool BanchoLoaded;
	bool ForceReDraw;

	_Menu() {
		State = 0;
		BanchoLoaded = 0;
		PreviousMap = 0;
		aux1 = 0;
		LastPage = 0;
		ChangePage = 0;
		ForceReDraw = 0;
	}
};

#include "Achievement.h"

#define MAX_CHAN_COUNT 32


void DisconnectUser(size_t Pointer);


struct _MD5 {

	char MD5[16];

	void Zero(){ ZeroMemory(MD5, sizeof(MD5)); }

	_MD5() { Zero(); }
	_MD5(std::string&& Input) {
		if (unlikely(Input.size() != 32)) {
			ZeroMemory(MD5, sizeof(MD5));
		}
		else for (size_t i = 0; i < 16; i++)
			MD5[i] = CharHexToDecimal(Input[i]) + (CharHexToDecimal(Input[16 + i]) << 4);//It does not really matter if the data is rearranged.
	}
	_MD5(const std::string_view Input) {
		if (unlikely(Input.size() != 32)) {
			ZeroMemory(MD5, sizeof(MD5));
		}
		else for (size_t i = 0; i < 16; i++)
			MD5[i] = CharHexToDecimal(Input[i]) + (CharHexToDecimal(Input[16 + i]) << 4);//It does not really matter if the data is rearranged.
	}

	const bool operator==(const _MD5& x) const {
		if (*(uint64_t*)& x.MD5[0] != *(uint64_t*)& this->MD5[0]
			|| *(uint64_t*)& x.MD5[8] != *(uint64_t*)& this->MD5[8])
			return 0;
		return 1;
	}

	std::string to_string()const {

		std::string ret;

		if (*(uint64_t*)& this->MD5[0] || *(uint64_t*)& this->MD5[8]) {
			ret.resize(32);

			for (size_t i = 0; i < 16; i++) {
				ret[i] = DecimalToHex(MD5[i] & 0xf);
				ret[i + 16] = DecimalToHex((MD5[i] & 0xf0) >> 4);
			}
		}

		return ret;
	}
};

struct _User{

	uint64_t choToken;
	DWORD UserID;
	DWORD privileges;
	int silence_end;
	std::string Username;
	std::string Username_Safe;
	_MD5 Password;
	char ActionMD5[32];
	DWORD actionMods;
	int BeatmapID;
	byte timeOffset;
	byte country;
	byte GameMode;
	byte actionID;
	float lat;
	float lon;

	std::vector<_BanchoPacket> Que;std::mutex qLock;

	std::mutex StatsAdd;

	_UserStats Stats[GM_MAX + 1];//4 normal modes + 4 more relax ones

	std::string ActionText;
	int LastPacketTime;
	_User* CurrentlySpectating;
	
	int LoginTime;
	bool SendToken;
	bool inLobby;
	bool HyperMode;
	bool FriendsOnlyChat;
	std::mutex SpecLock;
	std::vector<_User*> Spectators;
	//std::mutex MultiLock;
	USHORT CurrentMatchID;
	
	int LastSentBeatmap;
	//_Menu Menu;
	DWORD Friends[256];

	DWORD Blocked[32];	

	_Achievement Ach;//TODO: thread this.
	
	std::vector<_DelayedBanchoPacket> dQue;
	size_t ActiveChannels[MAX_CHAN_COUNT];//There is no way to resolve the actual size without restructuring xdxdxd

	
	std::string c1Check;

	std::mutex RefLock;
	DWORD ref;

	bool AddBlock(const DWORD UID){
		if (UID < 1000)return 0;

		for (byte i = 0; i < 32; i++) {
			if (!Blocked[i] || Blocked[i] == UID) {
				Blocked[i] = UID;
				return 1;
			}
		}

		return 0;
	}

	void AddChannel(const size_t C){
		for (DWORD i = 0; i < MAX_CHAN_COUNT; i++){
			if (!ActiveChannels[i]) {
				ActiveChannels[i] = C;
				return;
			}
		}
	}
	void RemoveChannel(const size_t C) {
		for (DWORD i = 0; i < MAX_CHAN_COUNT; i++) {
			if (ActiveChannels[i] == C) {
				ActiveChannels[i] = 0;
				return;
			}
		}
	}

	void addRef() {
		RefLock.lock();
		ref++;
		RefLock.unlock();
	}
	void removeRef() {
		RefLock.lock();
		ref--;
		RefLock.unlock();
	}

	DWORD GetStatsOffset()const{

		if (GameMode > 3)return 3;

		return
		#ifndef NO_RELAX
		(actionMods & Mods::Relax) ? GameMode + 4 :
		#endif
		GameMode;		
	}

	void SendToSpecs(const _BanchoPacket &b) {

		if (!Spectators.size())
			return;

		SpecLock.lock();

		for (_User*const Spec : Spectators)
			if (Spec)
				Spec->addQue(b);

		SpecLock.unlock();
	}
	bool isFriend(const DWORD ID) const{

		if (UserID == ID)
			return 1;//Always friends with your self :)

		for (DWORD i = 0; i < 256; i++)
			if (Friends[i] == ID)
				return 1;

		return 0;
	}
	bool isBlocked(const DWORD ID) const{
		if (UserID < 1000 || ID == UserID)
			return 0;

		for (DWORD i = 0; i < 32; i++)
			if (Blocked[i] == ID)
				return !isFriend(ID);

		return 0;
	}

	bool isSilenced()const{
		return silence_end ? (time(0) <= silence_end) : 0;
	}
void reset() {
		DisconnectUser(size_t(this));
		choToken = 0;
		UserID = 0;
		SendToken = 0;
		privileges = 0;
		Username.clear();
		Username_Safe.clear();
		Password.Zero();
		ZeroMemory(ActionMD5, 32);
		actionMods = 0;
		BeatmapID = 0;
		timeOffset = 0;
		country = 0;
		GameMode = 0;
		lat = 0.f;
		lon = 0.f;
		ActionText.clear();
		LoginTime = 0;
		CurrentlySpectating = 0;
		CurrentMatchID = 0;
		inLobby = 0;
		Spectators.clear();
		Que.clear();
		dQue.clear();
		HyperMode = 0;
		LastSentBeatmap = 0;
		FriendsOnlyChat = 0;
		ZeroMemory(&Friends[0], 256 * sizeof(Friends[0]));
		ZeroMemory(&Blocked[0], 32 * sizeof(Blocked[0]));
		c1Check.clear();
		silence_end = 0;
	}
	_User(){
		reset();
		Spectators.reserve(16);
		Que.reserve(512);
		ref = 0;
		LastPacketTime = INT_MIN;
	}
	
	template<typename... T>
	void addQue(T&& ... b) {
		if (!choToken)
			return;
		qLock.lock();
		Que.emplace_back(std::forward<T>(b)...);
		qLock.unlock();
	}

	template<typename T>
	void addQueDelay(T&& b) {
		if (b.b.Type == NULL_PACKET || b.Time == 0 || !choToken)
			return;
		qLock.lock();
		dQue.push_back(std::forward<T>(b));
		qLock.unlock();
	}

	void addQue(const VEC(_BanchoPacket) &b){
		if (choToken && b.size()){
			qLock.lock();
			for (auto& p : b){
				if (p.Type != NULL_PACKET)
					Que.push_back(p);
			}
			qLock.unlock();
		}
	}

	void addQue(std::initializer_list<_BanchoPacket>&& bundle) {
		if (choToken && bundle.size()){

			qLock.lock();

			for (auto& b : bundle)
				Que.emplace_back(_M(b));
			
			qLock.unlock();
		}
	}


	template<typename T>
	void addQueNonLocking(T&& b){
		if (b.Type == NULL_PACKET || !choToken)return;
		Que.push_back(std::forward<T>(b));
	}

	template<typename T>
	void addQueDelayNonLocking(T&& b){
		if (b.b.Type == NULL_PACKET || b.Time == 0 || !choToken)return;
		dQue.push_back(std::forward<T>(b));
	}

	void doQue(_Con s){

		std::vector<byte> SendBytes;

		if (Que.size() == 0 && dQue.size() == 0)
			goto FEEDALIVE;

		qLock.lock();

		if (Que.size()){
					
			SendBytes.resize([&](){
				size_t S = 0;
				for (const auto& q : Que)
					S += q.GetSize();
				return S;
			}());

			byte* cPos = (byte*)&SendBytes[0];

			for (const auto& q : Que)
				q.GetBytesRaw(cPos);

			Que.clear();
		}

		if (dQue.size() != 0){

			std::sort(dQue.begin(), dQue.end(), [](const _DelayedBanchoPacket i, const _DelayedBanchoPacket j) { return (i.Time < j.Time); });

			const int cTime = clock_ms();
			bool NotEnd = 0;
			for (size_t i = dQue.size() - 1; i < dQue.size(); i--){
				if (dQue[i].Time == 1){//means do the next bancho frame.
					dQue[i].Time = 0;
					NotEnd = 1;
					continue;
				}
				if (dQue[i].Time == 2) {
					dQue[i].Time = 1;
					NotEnd = 1;
					continue;
				}
				if(dQue[i].Time <= cTime){
					dQue[i].b.GetBytes(SendBytes);

					if(!NotEnd)dQue.pop_back();
					else{
						dQue[i] = dQue[dQue.size() - 1];
						dQue.pop_back();//Dont really care about it being slightly off. The next sort will correct this.
					}

				}else break;
			}
		}
		qLock.unlock();

		if (SendBytes.size())
			s.SendData(ConstructResponse(200, (!SendToken) ? Empty_Headers : SS_PAIR_VEC{{"cho-token", std::to_string(choToken)}}, SendBytes));
		else{
		FEEDALIVE:
			s.SendData(ConstructResponse(200, {}, SendBytes));
		}

	}

}; std::array<_User, MAX_USER_COUNT> Users;

void DisconnectUser(_User *u);

void DisconnectUser(size_t Pointer){
	return (Pointer) ? DisconnectUser((_User*)Pointer) : void();
}

void debug_LogOutUser(_User *p);


struct _UserRef {
	_User* User;

	_UserRef(){User = 0;}
	_UserRef(_User* U, bool AlreadyDoneRef): User(U){
		if (!User || AlreadyDoneRef)return;

		User->addRef();
	}
	~_UserRef() {
		if (!User)
			return;
		User->removeRef();
		User = 0;
	}

	void Reset(_User* NewUser, bool AlreadyDoneRef){
		if (User)
			User->removeRef();

		User = NewUser;

		if (User && !AlreadyDoneRef)
			User->addRef();
	}

	_User* operator->() {
		return User;
	}

	const bool operator!() {
		return !(User);
	}

};

#define UserDoubleCheck(s) if(!(s))continue;MUTEX_LOCKGUARD(User.RefLock);if(!(s))continue;User.ref++;


_User* GetPlayerSlot_Safe(const std::string &UserName){

	if (!UserName.size())return 0;


	for (auto& User : Users){

		UserDoubleCheck(User.Username_Safe == UserName)
		return &User;
	}

	//User has not logged in before. Opt for a free slot instead

	for (auto& User : Users){

		UserDoubleCheck(User.ref == 0 && User.LastPacketTime == INT_MIN)
		User.reset();

		User.LastPacketTime = 0;

		return &User;
	}

	return 0;
}



_User* GetUserFromID(DWORD ID) {

	if (int(ID) < 1000)return 0;
	
	for (auto& User : Users) {
		UserDoubleCheck(User.UserID == ID)
		return &User;
	}
	return 0;
}

_User* GetUserFromToken(const uint64_t Token) {

	if (!Token)return 0;
	
	for (auto& User : Users) {
		UserDoubleCheck(User.choToken == Token)
		return &User;
	}

	return 0;
}

template<typename T>
	_User* GetUserFromName(const T& Name, const bool Force = 0){

		if (Name.size() == 0)return 0;

		for (auto& User : Users){
			UserDoubleCheck((User.choToken || Force) && Name == User.Username)
			return &User;
		}

		return 0;
	}

_User* GetUserFromNameSafe(const std::string &Name, const bool Force = 0) {

	if (Name.size() == 0)return 0;

	for (auto& User : Users){

		UserDoubleCheck((User.choToken || Force) && Name == User.Username_Safe)

		return &User;
	}

	return 0;
}

DWORD GetIDFromName(const std::string &Name) {

	if (Name.size() == 0)return 0;

	for (auto& User : Users) {

		if (User.Username != Name)
			continue;

		return User.UserID;
	}

	return 0;
}


#define GetUserType(a)[](byte x)->byte{ \
			if (x & Privileges::AdminDev)return UserType::Peppy; \
			if (x & Privileges::AdminBanUsers)return UserType::Friend; \
			if (x & (Privileges::AdminChatMod | Privileges::AdminManageBeatmaps | Privileges::AdminQAT))return UserType::BAT; \
			if (x & (Privileges::Premium | Privileges::UserDonor))return UserType::Supporter; \
			return UserType::Normal; \
		}(a)


namespace bPacket {

	_BanchoPacket Message(const std::string_view senderName, const std::string_view targetname, const std::string_view message, const int senderId) {

		_BanchoPacket b(OPac::server_sendMessage);

		AddString(b.Data, senderName);
		AddString(b.Data, message);
		AddString(b.Data, targetname);
		AddStream(b.Data, senderId);

		return b;
	}

	_BanchoPacket BotMessage_NonDefault(const std::string_view Target, const std::string_view message, const std::string_view Name , const DWORD ID = 999) {

		_BanchoPacket b(OPac::server_sendMessage);

		b.Data.reserve(10 + Name.size() + message.size() + Target.size());

		AddString(b.Data, Name);
		AddString(b.Data, message);
		AddString(b.Data, Target);
		AddStream(b.Data, ID);

		return b;
	}

	_BanchoPacket BotMessage(const std::string_view Target, const std::string_view message) {

		_BanchoPacket b(OPac::server_sendMessage);

		b.Data.reserve(10 + BOT_NAME.size() + message.size() + Target.size());

		AddString(b.Data, BOT_NAME);
		AddString(b.Data, message);
		AddString(b.Data, Target);
		AddStream(b.Data,999);

		return b;
	}

}

#include "Channel.h"


const _BanchoPacket BOT_STATS = [] {
	_BanchoPacket b(OPac::server_userStats);
	AddStream(b.Data, 999);
	b.Data.push_back(0);//actionID
	AddString(b.Data, "");//actiontext
	AddString(b.Data, "");//md5
	AddStream(b.Data, 0);//mods
	b.Data.push_back(0);//gamemode
	AddStream(b.Data, 0);//beatmapid
	AddStream(b.Data, (long long)0);//rankedscore
	AddStream(b.Data, 0);//acc
	AddStream(b.Data, 0);//playcount
	AddStream(b.Data, (long long)0);//total score
	AddStream(b.Data, 0);//gamerank
	AddStream(b.Data, short(0));//pp
	return b;
}();

namespace bPacket {

	_inline _BanchoPacket Notification(const std::string_view Mes) {

		_BanchoPacket b(OPac::server_notification);

		AddString(b.Data, Mes);

		return b;
	}
	_BanchoPacket ChannelInfo(const _Channel* c) {

		_BanchoPacket b(OPac::server_channelInfo);

		AddString(b.Data, c->ChannelName);
		AddString(b.Data, c->ChannelDesc);
		AddStream(b.Data, short(c->ChannelCount));

		return b;
	}
	_BanchoPacket ChannelInfo(const std::string Name, const std::string Desc, USHORT Count) {

		_BanchoPacket b(OPac::server_channelInfo);

		AddString(b.Data, Name);
		AddString(b.Data, Desc);
		AddStream(b.Data, short(Count));

		return b;
	}

	#define bPacket4Byte(ID, VALUE) _BanchoPacket(ID,VEC(byte){\
												static_cast<byte>(VALUE),\
												static_cast<byte>(VALUE >> 8),\
												static_cast<byte>(VALUE >> 16),\
												static_cast<byte>(VALUE >> 24)})
	
	_inline _BanchoPacket RawData(const short ID, const byte* Value, const DWORD Size) {

		_BanchoPacket b(ID);

		b.Data.resize(Size);

		memcpy(&b.Data[0], Value, Size);

		return b;
	}
	


	_inline _BanchoPacket GenericString(const short ID, const std::string_view Value){
		_BanchoPacket b(ID);
		AddString(b.Data, Value);
		return b;
	}

	_inline _BanchoPacket GenericInt32List(const short ID, const std::vector<int> &Value, const bool BigHeader) {

		_BanchoPacket b(ID);

		if (BigHeader)
			AddStream(b.Data, DWORD(Value.size()));
		else
			AddStream(b.Data, USHORT(Value.size()));

		for (const auto i : Value)
			AddStream(b.Data, i);

		return b;
	}
	_inline _BanchoPacket GenericDWORDList(const short ID, const std::vector<DWORD> &Value, const bool BigHeader){

		_BanchoPacket b(ID);

		if(BigHeader)
			AddStream(b.Data, DWORD(Value.size()));
		else
			AddStream(b.Data, USHORT(Value.size()));

		for (const auto i : Value)
			AddStream(b.Data, i);

		return b;
	}


	_BanchoPacket UserPanel(const DWORD UserID, const DWORD AskerID) {

		if (UserID < 1000){
			_BanchoPacket b(OPac::server_userPanel);
			AddStream(b.Data, UserID);

			switch (UserID){
			case 999:
				AddString(b.Data, BOT_NAME);
				break;
			case 998:
				AddString(b.Data, FAKEUSER_NAME);
				break;
			default:
				AddString(b.Data, "Not Set");
				break;
			}

			b.Data.push_back(0);//timezone
			b.Data.push_back(BOT_LOCATION);//country
			b.Data.push_back(UserType::Peppy);//userank
			AddStream(b.Data, 0);//long
			AddStream(b.Data, 0);//lat
			AddStream(b.Data, 0);//gamerank
			return b;
		}

		_UserRef tP(GetUserFromID(UserID),1);

		if (!tP || !tP->UserID || (UserID != AskerID && !(tP->privileges & UserPublic)))return erPacket;

		_BanchoPacket b(OPac::server_userPanel);

		AddStream(b.Data, UserID);
		AddString(b.Data, tP->Username);
		b.Data.push_back(24 + tP->timeOffset);
		b.Data.push_back(tP->country);
		b.Data.push_back((UserID != AskerID) ? GetUserType(tP->privileges) : UserType::Supporter);
		AddStream(b.Data, tP->lon);
		AddStream(b.Data, tP->lat);
		const DWORD sOff = tP->GetStatsOffset();
		AddStream(b.Data, tP->Stats[sOff].getRank(sOff,tP->UserID));

		return b;
	}
	_BanchoPacket UserPanel(_User *tP) {

		_BanchoPacket b(OPac::server_userPanel);

		AddStream(b.Data, tP->UserID);
		AddString(b.Data, tP->Username);
		b.Data.push_back(24 + tP->timeOffset);
		b.Data.push_back(tP->country);
		b.Data.push_back(GetUserType(tP->privileges));
		AddStream(b.Data, tP->lon);
		AddStream(b.Data, tP->lat);
		const DWORD sOff = tP->GetStatsOffset();
		AddStream(b.Data, tP->Stats[sOff].getRank(sOff,tP->UserID));

		return b;
	}

#define USER_STATS(UID, AID)\
	[](const DWORD UserID, const DWORD AskerID)->const _BanchoPacket{\
		if (UserID == 999)return BOT_STATS;\
		if(UserID < 999){\
			_BanchoPacket b = BOT_STATS;\
			*(DWORD*)&b.Data[0] = UserID;\
			return b;\
		}\
		_UserRef tP(GetUserFromID(UserID),1);\
		if (!tP || !tP->choToken || (!(tP->privileges & UserPublic) && UserID != AskerID))\
			return bPacket4Byte(OPac::server_userLogout, UserID);\
		const DWORD Off = tP->GetStatsOffset();\
\
		_BanchoPacket b(OPac::server_userStats);\
		b.Data.reserve(128);\
		AddStream(b.Data, UserID);\
		b.Data.push_back(tP->actionID);\
\
		AddString(b.Data, tP->ActionText);\
		if (tP->ActionMD5[0] == 0)AddString(b.Data, "");\
		else{\
			AddUleb(b.Data,32);\
			AddMem(b.Data, tP->ActionMD5, 32);\
		}\
		AddStream(b.Data, tP->actionMods);\
		b.Data.push_back(tP->GameMode);\
		AddStream(b.Data, tP->BeatmapID);\
		AddStream(b.Data, tP->Stats[Off].rScore);\
		AddStream(b.Data, tP->Stats[Off].Acc);\
		AddStream(b.Data, (tP->Stats[Off].pp > USHORT(-1)) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount);\
		AddStream(b.Data, tP->Stats[Off].tScore);\
		AddStream(b.Data, tP->Stats[Off].getRank(Off, tP->UserID));\
		AddStream(b.Data, USHORT(tP->Stats[Off].pp));\
\
		return b;\
	}(UID,AID)

	_BanchoPacket UserStats(_User *tP) {

		if (!tP || !tP->choToken || !(tP->privileges & UserPublic))
			return bPacket4Byte(OPac::server_userLogout, tP->UserID);

		const DWORD Off = tP->GetStatsOffset();

		_BanchoPacket b(OPac::server_userStats);
		b.Data.reserve(128);

		AddStream(b.Data, tP->UserID);
		b.Data.push_back(tP->actionID);
		AddString(b.Data, tP->ActionText);
		if (tP->ActionMD5[0] == 0)AddString(b.Data, "");
		else{
			AddUleb(b.Data, 32);
			AddMem(b.Data, tP->ActionMD5, 32);
		}
		AddStream(b.Data, tP->actionMods);
		b.Data.push_back(tP->GameMode);
		AddStream(b.Data, tP->BeatmapID);
		AddStream(b.Data, tP->Stats[Off].rScore);
		AddStream(b.Data, tP->Stats[Off].Acc);
		AddStream(b.Data, (tP->Stats[Off].pp > USHORT(-1)) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount);
		AddStream(b.Data, tP->Stats[Off].tScore);
		AddStream(b.Data, tP->Stats[Off].getRank(Off, tP->UserID));
		AddStream(b.Data, USHORT(tP->Stats[Off].pp));

		return b;
	}


}


void debug_SendOnlineToAll(_User *p){

	if (!(p->privileges & UserPublic))
		return;//Do not even care about restricted users for now.

	const _BanchoPacket Panel = bPacket::UserPanel(p->UserID, 0);
	const _BanchoPacket Stats = USER_STATS(p->UserID, 0);


	for (auto& User : Users) {
		if (!User.choToken || &User == p)
			continue;

		User.qLock.lock();

		User.addQueNonLocking(Panel);
		User.addQueNonLocking(Stats);

		User.qLock.unlock();

	}
}

void Event_client_stopSpectating(_User *u){
	
	u->addQue(bPacket::GenericString(OPac::server_channelKicked, "#spectator"));

	_User *const SpecTarget = u->CurrentlySpectating;
	
	if (!SpecTarget)return;

	u->CurrentlySpectating = 0;

	bool AllEmptySpecs = 1;

	_BanchoPacket b = bPacket4Byte(OPac::server_fellowSpectatorLeft, u->UserID);

	{
		SpecTarget->SpecLock.lock();

		for (_User*& Spec : SpecTarget->Spectators){

			if (Spec == u){
				Spec = 0;
				break;
			}
		}
		for (_User*const Spec : SpecTarget->Spectators) {
			if (!Spec)continue;

			Spec->addQue(b);

			AllEmptySpecs = 0;
		}

		if (AllEmptySpecs)
			SpecTarget->Spectators.clear();

		SpecTarget->SpecLock.unlock();
	}
	b.Type = OPac::server_spectatorLeft;
	SpecTarget->addQue(b);

	if (AllEmptySpecs)
		SpecTarget->addQue(bPacket::GenericString(OPac::server_channelKicked,"#spectator"));
}

void Event_client_cantSpectate(_User *tP) {

	_User *SpecHost = tP->CurrentlySpectating;

	if (!SpecHost)
		return;

	const _BanchoPacket b = bPacket4Byte(OPac::server_spectatorCantSpectate, tP->UserID);
	SpecHost->SendToSpecs(b);
	SpecHost->addQue(_M(b));
}

DWORD COUNT_CURRENTONLINE = 0;
DWORD COUNT_REQUESTS = 0;
DWORD COUNT_MULTIPLAYER = 0;

void RenderHTMLPage(_Con s, const _HttpRes &&res){

	std::vector<byte> Body;

	AddStringToVector(Body, "<HTML><img src=\"https://cdn.discordapp.com/attachments/385279293007200258/567292020104888320/unknown.png\">"
		"<br> Online users: "+std::to_string(COUNT_CURRENTONLINE) + " | " + std::to_string(COUNT_REQUESTS) + " total connections handled."
		"<br>"+ std::to_string(COUNT_MULTIPLAYER) + " currently active multiplayer games.</HTML>");

	s.SendData(ConstructResponse(405, { {"Content-Type", "text/html; charset=utf-8"} }, Body));
}

uint64_t GenerateChoToken(){
	/*
	for (DWORD i = 0; i < MAX_USER_COUNT; i++)
		if (Token == User[i].choToken)goto RETRY;*///One in 18 quintillion lol... I think I will take my bets that this will never collide.

	return BR::GetRand64();
}


_inline DWORD ReadUlebSize(size_t &O, const size_t Max) {

	if (O + 2 >= Max)
		return 0;

	if (!*(byte*)O) {
		O++;
		return 0;
	}

	O++;

	DWORD Size = *(byte*)O & 0x7f;
	byte Shift = 7;
	while ((*(byte*)O) & 0x80 && O + 1 < Max) {
		O++;
		Size |= (*(byte*)O & 0x7f) << Shift;
		Shift += 7;
	}O++;

	return (size_t(O) + Size <= Max) ? Size : 0;
}

_inline std::string ReadUleb(size_t &O, const size_t Max) {

	const DWORD Size = ReadUlebSize(O, Max);

	const char* S = (char*)O;
	O += Size;

	return std::string(S, Size);
}
_inline std::string_view ReadUleb_View(size_t& O, const size_t Max){

	const DWORD Size = ReadUlebSize(O, Max);
	const char*const S = (char*)O;
	O += Size;
	return std::string_view(S, Size);
}

_inline void SkipUleb(size_t &O, const size_t Max){
	O += ReadUlebSize(O, Max);
}


void Event_client_channelJoin(_User *tP,const byte* const Packet, const DWORD Size){

	if (Size <= 2)
		return;

	size_t O = (size_t)&Packet[0];

	const std::string ChannelName = ReadUleb(O,O + Size);

	_Channel *c = GetChannelByName(ChannelName);

	/*if(!c && tP->CurrentlySpectating && ChannelName != "#spectator") {
		tP->addQue(bPacket::GenericString(OPac::server_channelJoinSuccess, ChannelName.c_str()));
		return;
	}*/
	
	if (!c) {
		tP->addQue(bPacket::GenericString(OPac::server_channelKicked, ChannelName));
		return;
	}

	c->JoinChannel(tP);

	tP->addQue(bPacket::GenericString(OPac::server_channelJoinSuccess, ChannelName));

}
void Event_client_channelPart(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size <= 2)return;

	size_t O = (size_t)&Packet[0];

	const std::string ChannelName = ReadUleb(O,O + Size);

	_Channel *c = GetChannelByName(ChannelName);

	if (!c)return;

	c->PartChannel(tP);
}

void Event_client_userStatsRequest(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size <= 2)return;

	const USHORT Count = al_min(*(USHORT*)&Packet[0],64);

	if ((Count << 2) + 2 > Size)return;

	tP->qLock.lock();

	for (DWORD i = 0; i < Count; i++) {

		const DWORD UID = *(DWORD*)&Packet[2 + (i << 2)];

		tP->addQueNonLocking(USER_STATS(UID, tP->UserID));
	}
	tP->qLock.unlock();

}
void Event_client_changeAction(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size < 12)return;

	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;

	const byte n_actionID = *(byte*)O; O++;
	const std::string n_ActionText = ReadUleb(O, End);
	const std::string n_CheckSum = REMOVEQUOTES(ReadUleb(O, End));
	if (O + 4 > End)return;
	const DWORD n_Mods = *(DWORD*)O; O += 4;
	if (O + 1 > End)return;
	const byte n_GameMode = *(byte*)O; O++;
	if (O + 4 > End)return;
	const int n_BeatmapID = *(int*)O;

	tP->actionID = n_actionID;
	tP->ActionText = n_ActionText;

	if (n_CheckSum.size() == 32)
		memcpy(tP->ActionMD5, &n_CheckSum[0], 32);

	if(n_actionID != bStatus::sAfk)//Stop relax flickering.
		tP->actionMods = n_Mods;
	tP->GameMode = n_GameMode;
	if(n_BeatmapID)tP->BeatmapID = n_BeatmapID;

	tP->addQue(bPacket::UserStats(tP));

}

#include <fstream>


#ifndef LINUX
#include <filesystem>
_inline DWORD GetFileSize(const std::string &filename){	
	std::error_code er;
	if (const DWORD Size = std::filesystem::file_size(filename, er); !er)
		return Size;
	return 0;
}
#else
_inline DWORD GetFileSize(const std::string& filename) {
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	if (ifs.is_open()) {
		ifs.close();

		return ifs.tellg();
	}
	return 0;
}


#endif

_inline bool WriteAllBytes(const std::string &filename, const void* data, const DWORD Size);
bool DownloadMapFromOsu(const int ID) {

	if (ID < 0 || ID > 6000000)
		return 0;

	const std::string bFile = GET_WEB_CHUNKED("old.ppy.sh", std::string("osu/" + std::to_string(ID)));

	if (bFile.size() == 0 || bFile.find("[HitObjects]") == std::string::npos)return 0;

	//ReplaceAll(bFile, "\r\n", "\n");//Could save diskspace

	WriteAllBytes(BEATMAP_PATH + std::to_string(ID) + ".osu", (void*)&bFile[0], bFile.size());

	return 1;
}

#include "oppai.h"
//#include "pp/pp_base.h"
static const std::string String_Space = " ";


bool OppaiCheckMapDownload(ezpp_t ez, const DWORD BID) {

	const std::string MapPath = BEATMAP_PATH + std::to_string(BID) + ".osu";	

	const DWORD Size = GetFileSize(MapPath);

	if (!Size && !DownloadMapFromOsu(BID)) {
		printf(KRED "Failed to download %i.osu\n" KRESET, BID);

		WriteAllBytes(MapPath, &String_Space[0], String_Space.size());//Stop it from trying it over and over again.
		return 0;
	}
	if (Size < 100)
		return 0;

	const int res = ezpp(ez, (char*)MapPath.c_str());

	if (res < 0) {
		printf(KMAG "oppai> " KRED "Failed with errorcode %i\n" KRESET, res);
		return 0;
	}

	return 1;
}

bool OppaiCheckMapDownload_New(ezpp_t ez, const DWORD BID, const std::string &File) {

	if (!GetFileSize(File) && !DownloadMapFromOsu(BID)) {
		printf(KRED "Failed to download %i.osu\n" KRESET, BID);
		WriteAllBytes(File, &String_Space[0], String_Space.size());//Stop it from trying it over and over again.
		return 0;
	}

	free_owned_map(ez);
	ez->map = (char*)File.c_str();

	return 1;
}

#include "Match.h"

/*
inline bool WriteAllBytes(const std::string &filename, const std::string &res) {

	if (!res.size())
		return 0;
	std::ofstream ifs;
	ifs.open(filename);

	if (!ifs.is_open())return 0;

	ifs.write((char*)&res[0], res.size());
	ifs.close();

	return 1;
}*/

_inline bool WriteAllBytes(const std::string &filename, const void* data, const DWORD Size){

	std::ofstream ifs;
	ifs.open(filename,std::ofstream::binary);

	if (!ifs.is_open())return 0;
	
	ifs.write((char*)data, Size);
	ifs.close();

	return 1;
}


int getSetID_fHash(const std::string &H, _SQLCon* c){//Could combine getbeatmapid and getsetid into one

	if (H.size() != 32)
		return 0;

	if (c){
		sql::ResultSet* res = c->ExecuteQuery("SELECT beatmapset_id FROM beatmaps WHERE beatmap_md5 = '" + H + "' LIMIT 1");

		if (res && res->next()) {
			const int ID = res->getInt(1);
			delete res;
			return ID;
		}

		if (res)
			delete res;
	}

	const std::string BeatmapData = GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "h=" + H);

	if (BeatmapData.size() <= 25)return 0;

	DWORD Off = BeatmapData.find("\"beatmapset_id\"");

	if (Off == std::string::npos)return 0;

	Off += 17;

	std::string t;//TODO: Dont.

	while (BeatmapData[Off] != '"') {
		t.push_back(BeatmapData[Off]);
		Off++;
	}

	//TODO add to database
	return StringToInt32(t);
}


int getBeatmapID_fHash(const std::string &H, _SQLCon* c) {

	if (H.size() != 32)return 0;


	if (c){

		sql::ResultSet* res = c->ExecuteQuery("SELECT beatmap_id FROM beatmaps WHERE beatmap_md5 = '" + H + "' LIMIT 1");

		if (res && res->next()){
			const int ID = res->getInt(1);
			delete res;
			return ID;
		}
		if (res)
			delete res;

	}
	const std::string &BeatmapData = GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "h=" + H);

	if (BeatmapData.size() <= 25)return 0;

	DWORD Off = BeatmapData.find("\"beatmap_id\"");

	if (Off == std::string::npos)return 0;

	Off += 14;

	std::string t;

	while (BeatmapData[Off] != '"') {
		t.push_back(BeatmapData[Off]);
		Off++;
	}

	//TODO add to database

	return StringToInt32(t);
}

#define AddDeci(s,o) if (*(byte*)(s + o) == 0) { *(USHORT*)(s + o - 1) = *(USHORT*)(s + o - 2); *(byte*)(s + o - 2) = '.'; return s;}

std::string RoundTo2(const float Input) {

	char s[16];
	*(uint64_t*)(s) = 0;
	*(uint64_t*)(s + 8) = 0;

	snprintf(s, 16, "%d", int(Input * 100.f));

	return std::string([&]{
		if (s[0] == '0')return s;
		AddDeci(s, 2)
		AddDeci(s, 3)
		AddDeci(s, 4)
		AddDeci(s, 5)
		AddDeci(s, 6)
		AddDeci(s, 7)
		AddDeci(s, 8)
		AddDeci(s, 9)
		AddDeci(s, 10)
		AddDeci(s, 11)
		AddDeci(s, 12)
		AddDeci(s, 13)
		AddDeci(s, 14)
		return s;
	}());
}


float ezpp_NewAcc(ezpp_t ez, const float Acc) {
	ezpp_set_accuracy_percent(ez, Acc);
	calc(ez);
	return ezpp_pp(ez);
}



#include "Aria.h"
#include "Commands.h"

void BotMessaged(_User *tP, const std::string_view Message){
	
	if (Message.size() == 0)return;

	if (Message[0] == '!'){

		if (!MEM_CMP_START(Message, "!with") && !MEM_CMP_START(Message, "!acc")) {

			DWORD Unused;

			const std::string Res = ProcessCommand(tP, Message, Unused);

			if (Res.size())
				tP->addQue(bPacket::BotMessage(tP->Username, Res));

			return;
		}
	}

	const auto BotMessage = Explode_View(Message, ' ', 8);
	
	if (BotMessage.size() < 2)
		return;

	int mapID = 0;
	int Mods = 0;
	float Acc = 0.f;
	int Combo = 0;

	if (BotMessage.size() >= 5) {

		if (BotMessage[0] == "\x1""ACTION"){

			const DWORD BeatmapOffset = (BotMessage[2] == "listening") ? 4:3;

			const DWORD mSize = BotMessage[BeatmapOffset].size();

			if(BotMessage[BeatmapOffset].find("/s/") != std::string::npos || mSize < 5)//mSize check is just to make it safe.
				return tP->addQue(bPacket::BotMessage(tP->Username, "This appears to be a Beatmapset - Which is not currently supported sorry!"));

			char* Temp = new char[mSize]();

			size_t pos = BotMessage[BeatmapOffset].find_last_of('/');

			if (pos != std::string::npos && pos != mSize - 1)
				memcpy(Temp, &BotMessage[BeatmapOffset][pos + 1], mSize - (pos + 1));

			Temp[(mSize > 0) ? mSize - 1 : 0] = 0;//Just incase there is some mallice.

			mapID = MemToInt32(Temp,strlen(Temp));
			delete[] Temp;
		}

		if (mapID){

			#define MODREAD(s) if(MEM_CMP_OFF(BotMessage[i],#s,1)){ Mods |= s;continue; }

			for (DWORD i = BotMessage.size() - 1; i > 5; i--){

				if (BotMessage[i].size() <= 1)continue;

				MODREAD(DoubleTime);
				MODREAD(Nightcore);
				MODREAD(Hidden);
				MODREAD(HardRock);
				MODREAD(Flashlight);
				MODREAD(SpunOut);
				MODREAD(Easy);
				MODREAD(NoFail);
				MODREAD(HalfTime);
				MODREAD(Relax);

			}

			if (Mods & Nightcore)
				Mods |= DoubleTime;


			#undef MODREAD
		}

	}

	if (!mapID){
		if (BotMessage.size() >= 2){

			if (BotMessage[0] == "!acc")
				return tP->addQue(bPacket::BotMessage(tP->Username, "This is now in !with. Here are some examples of the usage.\n!with HDDT 95\n!with 98.5 HD HR"));

			if (BotMessage[0] == "!with"){

				Mods = 0;
				std::string AllMods;
				for (DWORD i = 1; i < BotMessage.size(); i++){
					AllMods += BotMessage[i];
					if (Acc == 0.f || Combo == 0){
						byte Com = 0;
						std::string tNum;

						for (const char C : BotMessage[i]){
							if ((C >= '0' && C <= '9'))
								tNum.push_back(C);
							else if (C == '.' && tNum.size() != 0 && Com != 2){
								Com = 2;
								tNum.push_back(C);
							}else if ((C == 'x' || C == 'X') && Com != 2)
								Com = 1;
						}

						if (tNum.size()){
							if (Com == 1)Combo = std::stoi(tNum);
							else Acc = std::stof(tNum);
						}
					}
				}

				for (char& C :  AllMods)
					if (C >= 'A' && C <= 'Z')
						C += 'a' - 'A';

				if (AllMods.find("hd") != std::string::npos)
					Mods += Hidden;
				if (AllMods.find("dt") != std::string::npos || BotMessage[1].find("nc") != std::string::npos)
					Mods += DoubleTime;
				if (AllMods.find("fl") != std::string::npos)
					Mods += Flashlight;
				if (AllMods.find("ez") != std::string::npos)
					Mods += Easy;
				if (AllMods.find("ht") != std::string::npos)
					Mods += HalfTime;
				if (AllMods.find("rx") != std::string::npos)
					Mods += Relax;
				if (AllMods.find("so") != std::string::npos)
					Mods += SpunOut;
				if (AllMods.find("hr") != std::string::npos)
					Mods += HardRock;

				mapID = tP->LastSentBeatmap;

				if (!mapID)return;

				goto WITHMODS;
			}
		}
		 return;
	}

WITHMODS:

	/*
	_RawBeatmap s;


	std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();
	
	pp_GetRawMapData(mapID, s);

	const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
	printf("Time: %f\n%i\n", double(double(TTime) * .000001),s.Notes.size());
	*/
	
	ezpp_t ez = ezpp_new();

	if (!ez)return;

	tP->addQue(bPacket::BotMessage(tP->Username, [&]()->std::string{
		ezpp_set_mods(ez, Mods);
		ezpp_set_accuracy_percent(ez, (Acc == 0.f) ? 95.f : Acc);

		if (Combo)ezpp_set_combo(ez, Combo);

		if (!OppaiCheckMapDownload(ez, mapID))
			return "Sorry. I failed to get that map.";

		tP->LastSentBeatmap = mapID;

		const float PP = ezpp_pp(ez);

		if (Acc != 0.f || Combo != 0)
			return RoundTo2(PP) + " pp";

		const float PPValues[3] = { ezpp_NewAcc(ez,98.f),ezpp_NewAcc(ez,99.f),ezpp_NewAcc(ez,100.f) };

		return "95%: " + RoundTo2(PP) + " pp\n98%: "
			+ RoundTo2(PPValues[0]) + " pp\n99%: " + RoundTo2(PPValues[1]) + " pp\n100%: "
			+ RoundTo2(PPValues[2]) + " pp";
	}()));

	ezpp_free(ez);
}

void Event_client_sendPrivateMessage(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size < 7 || tP->isSilenced() || !(tP->privileges & Privileges::UserPublic))
		return;

	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;

	SkipUleb(O, End);//Skip sender. It is useless

	const std::string_view Message = ReadUleb_View(O, End);
	const std::string_view Target = ReadUleb_View(O, End);

	if (O + 4 > End)return;
	const int ID = *(int*)O;

	if (Message.size() == 0)return;

	if (Target == BOT_NAME)
		return BotMessaged(tP, Message);

	_UserRef u(GetUserFromName(Target),1);

	if (unlikely(!u))
		return tP->addQue(bPacket4Byte(OPac::server_userLogout, ID));
	
	if (u->isBlocked(tP->UserID))
		return;

	if(u->FriendsOnlyChat && !u->isFriend(tP->UserID))
		tP->addQue([&] {//hmm yes im peppy this is a great system!!1m!1! I AM GEN I US PEPPPYYY
		auto b = bPacket::Message(tP->Username, u->Username, "", tP->UserID);
		b.Type = OPac::server_userPMBlocked;
		return b;
		}());
	else
		u->addQue(bPacket::Message(tP->Username, Target, Message, tP->UserID));
}

void Event_client_sendPublicMessage(_User *tP, const byte* const Packet, const DWORD Size) {

	if (Size < 7 || tP->isSilenced())return;

	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;

	SkipUleb(O, End);//Skip sender. It is useless

	const std::string Message = [&]{
		std::string Why_can_MSVC_compile_and_work_fine_with_no_locals_but_GCC_eats_massive_dog_shit__Thanks_GCC = ReadUleb(O, End);
		return TRIMSTRING(Why_can_MSVC_compile_and_work_fine_with_no_locals_but_GCC_eats_massive_dog_shit__Thanks_GCC);
	}();

	const std::string_view Target = ReadUleb_View(O, End);

	if (Message.size() == 0 || Target == "#highlight" || Target == "#userlog")
		return;

	_Channel *c = 0;
	
	if (Target == "#multiplayer"){
		const USHORT MultiID = tP->CurrentMatchID;
		if (MultiID){
			_Match *const m = getMatchFromID(MultiID);
			if (m){
				DWORD notVisible = 0;
				const std::string s = ProcessCommandMultiPlayer(tP, Message, notVisible, m);

				if (s.size() == 0){
					m->Lock.lock();
					m->sendUpdate(bPacket::Message(tP->Username, Target, Message, tP->UserID), tP);
					m->Lock.unlock();
				}else{

					if(notVisible)
						tP->addQue(bPacket::BotMessage("#multiplayer", s));
					else{
						m->Lock.lock();
						m->sendUpdate(bPacket::BotMessage("#multiplayer", s), 0);
						m->Lock.unlock();
					}
				}
			}
		}
		return;
	}else if ((tP->CurrentlySpectating || tP->Spectators.size()) && Target == "#spectator"){

		_User *const SpecHost = (tP->Spectators.size()) ? tP : tP->CurrentlySpectating;

		if (SpecHost){

			const _BanchoPacket b = bPacket::Message(tP->Username, Target, Message, tP->UserID);

			SpecHost->SpecLock.lock();

			for (_User *const s : SpecHost->Spectators)
				if (s && s != tP)
					s->addQue(b);

			SpecHost->SpecLock.unlock();

			if (SpecHost != tP)
				SpecHost->addQue(b);
		}

		return;
	}else c = GetChannelByName(Target);

	if (!c)
		return tP->addQue(bPacket::GenericString(OPac::server_channelKicked, Target));

	if (Message[0] == '!'){

		DWORD notVisible = 0;
		const std::string Res = ProcessCommand(tP, Message, notVisible);

		if (!(tP->privileges & Privileges::UserPublic))
			notVisible = 1;

		if (Res.size()){
			if (notVisible)
				tP->addQue(bPacket::BotMessage(c->ChannelName, Res));
			else
				c->Bot_SendMessage(Res);
		}

		return;
	}

	if (tP->privileges & Privileges::UserPublic)
		c->SendPublicMessage(tP, bPacket::Message(tP->Username, Target, Message, tP->UserID));
}

void Event_client_startSpectating(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size < 4)return;

	const DWORD ID = *(DWORD*)&Packet[0];

	if (ID < 1000)	
		return tP->addQue(bPacket::Notification("What are you doing spectating bots?"));
	

	_UserRef SpecTarget(GetUserFromID(ID),1);

	if (!SpecTarget || tP == SpecTarget.User){
		tP->addQue(bPacket::Notification("Failed to find user."));
		return;
	}

	if (tP->CurrentlySpectating)
		Event_client_stopSpectating(tP);

	bool Add = (tP->CurrentlySpectating != SpecTarget.User);

	tP->CurrentlySpectating = SpecTarget.User;

	tP->addQue(bPacket::GenericString(OPac::server_channelJoinSuccess,"#spectator"));

	const _BanchoPacket b = bPacket4Byte(OPac::server_fellowSpectatorJoined, tP->UserID);
	
	SpecTarget->SpecLock.lock();

	if(SpecTarget->Spectators.size() == 0)
		SpecTarget->addQue(bPacket::GenericString(OPac::server_channelJoinSuccess, "#spectator"));

	for (auto& fSpec : SpecTarget->Spectators){

		if (!fSpec) {
			if(Add)fSpec = tP;
			Add = 0;
			continue;
		}

		if (fSpec != tP){
			fSpec->addQue(b);
			tP->addQue(bPacket4Byte(OPac::server_fellowSpectatorJoined, fSpec->UserID));
		}else Add = 0;

	}

	if (Add)
		SpecTarget->Spectators.push_back(tP);
	
	SpecTarget->SpecLock.unlock();

	SpecTarget->addQue({
		bPacket::UserPanel(tP),
		bPacket4Byte(OPac::server_spectatorJoined, tP->UserID)
	});

}


struct _ReplayFrame {
	int time;
	float x, y;
	byte ButtonState;
	byte bt;
};

void Event_client_spectateFrames(_User *tP, const byte* const Packet, const DWORD Size){

	if (tP->Spectators.size() == 0)return;

	const _BanchoPacket b = bPacket::RawData(OPac::server_spectateFrames, Packet, Size);
	const _BanchoPacket u = bPacket::UserStats(tP);

	tP->SpecLock.lock();
	for (_User *s :  tP->Spectators){

		if (!s)continue;

		s->qLock.lock();

		s->addQueNonLocking(u);
		s->addQueNonLocking(b);

		s->qLock.unlock();
	}
	tP->SpecLock.unlock();
}

void ReadMatchData(_Match *m, const byte* const Packet,const DWORD Size, bool Safe = 0){//todo make better

	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;

	/*m->MatchId = *(USHORT*)O; O += 2;*/
	/*m->inProgress = *(byte*)O; O++;*/

	O += 3;

	if (O + 1 > End)return;
	m->Settings.MatchType = *(byte*)O; O++;
	if (O + 4 > End)return;
	m->Settings.Mods = *(DWORD*)O; O += 4;
	m->Settings.Name = ReadUleb(O,End);
	if (!Safe){
		m->Settings.Password = ReadUleb(O,End);
		ReplaceAll(m->Settings.Password," ","_");
		ReplaceAll(m->Settings.Password, "//private", "");
	}
	else SkipUleb(O,End);
	m->Settings.BeatmapName = ReadUleb(O,End);
	if (O + 4 > End)return;
	m->Settings.BeatmapID = *(DWORD*)O; O += 4;
	m->Settings.BeatmapChecksum = ReadUleb(O,End);

	byte tSlotStatus[NORMALMATCH_MAX_COUNT];
	byte tSlotTeam[NORMALMATCH_MAX_COUNT];
	if (O + NORMALMATCH_MAX_COUNT > End)return;
	if (!Safe)memcpy(tSlotStatus, (void*)O, NORMALMATCH_MAX_COUNT); O += NORMALMATCH_MAX_COUNT;
	if (O + NORMALMATCH_MAX_COUNT > End)return;
	if (!Safe)memcpy(tSlotTeam, (void*)O, NORMALMATCH_MAX_COUNT); O += NORMALMATCH_MAX_COUNT;
	if (!Safe)m->HostID = *(DWORD*)O; O += 4;
	
	if (!Safe){
		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++) {
			m->Slots[i].SlotStatus = tSlotStatus[i];
			m->Slots[i].SlotTeam = tSlotTeam[i];
		}
	}

	for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
		if (m->Slots[i].SlotStatus & SlotStatus::HasPlayer)
			O += 4;
	
	if (O + 4 > End)return;

	m->Settings.PlayMode = *(byte*)O; O++;
	m->Settings.ScoringType = *(byte*)O; O++;
	m->Settings.TeamType = *(byte*)O; O++;
	m->Settings.FreeMod = *(byte*)O; O++;

	if (m->Settings.FreeMod){
		if (O + (NORMALMATCH_MAX_COUNT * 4) > End)return;
		int tCurrentMods[NORMALMATCH_MAX_COUNT];
		memcpy(tCurrentMods, (void*)O, NORMALMATCH_MAX_COUNT * 4);

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			m->Slots[i].CurrentMods = tCurrentMods[i];		

		O += NORMALMATCH_MAX_COUNT * 4;
	}
	if (O + 4 > End)return;
	m->Seed = *(DWORD*)O;
}

void Event_client_createMatch(_User *tP, const byte* const Packet, const DWORD Size){

	if (tP->CurrentMatchID || !(tP->privileges & UserPublic))//already in a match? Might want to kick them from the old one.
		return tP->addQue(_BanchoPacket(OPac::server_matchJoinFail));


	_Match *const m = getEmptyMatch();

	if(unlikely(!m))
		return tP->addQue(_BanchoPacket(OPac::server_matchJoinFail));

	tP->inLobby = 0;

	ReadMatchData(m, Packet,Size);

	m->HostID = tP->UserID;
	m->inProgress = 0;
	m->PlayersLoading = 0;
	m->Settings.Mods = 0;
	m->PlayerCount = 1;
	m->Slots[0].SlotStatus = SlotStatus::NotReady;
	m->Slots[0].User = tP;
	tP->CurrentMatchID = m->MatchId;
	m->Tournament = 0;


	tP->qLock.lock();
	tP->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess,"#multiplayer"));
	tP->addQueNonLocking(bPacket::bMatch(OPac::server_matchJoinSuccess,m,1));
	tP->qLock.unlock();
}

_inline void SendMatchList(_User *tP, const bool New) {

	if(!tP->inLobby)return;
	
	for (DWORD i = 0; i < MAX_MULTI_COUNT; i++){

		if (Match[i].PlayerCount && !Match[i].Tournament) {
			if (Match[i].LobbyCache.Type == 0) {//TODO might want to improve this. Could cause pipeline issues.
				Match[i].Lock.lock();
				tP->addQue(bPacket::bMatch((New) ? OPac::server_newMatch : OPac::server_updateMatch, &Match[i], 0));
				Match[i].Lock.unlock();
			}
			else tP->addQue(Match[i].LobbyCache);
		}
	}
}

void Event_client_partMatch(_User *tP){
	if (!tP->CurrentMatchID)return;
	_Match *m = getMatchFromID(tP->CurrentMatchID);
	if (!m)return;
	m->Lock.lock();
	m->removeUser(tP, 0);
	m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
	m->Lock.unlock();
}

void Event_client_matchChangeSlot(_User *const tP, const byte* const Packet, const DWORD Size){

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m && Size == 4){

		const DWORD NewSlot = al_min(*(DWORD*)& Packet[0], MULTI_MAXSIZE - 1);
		byte OldSlot = MULTI_MAXSIZE;

		m->Lock.lock();
		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				OldSlot = GetIndex(m->Slots, Slot);
				break;
			}

		_Slot& Old = m->Slots[OldSlot];
		_Slot& New = m->Slots[NewSlot];

		if (OldSlot != MULTI_MAXSIZE && OldSlot != NewSlot && !New.User &&
			New.SlotStatus != SlotStatus::Locked && Old.SlotStatus != SlotStatus::Ready) {
			New = Old;
			Old.reset();
			m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
		}

		m->Lock.unlock();

	}

}

void Event_client_joinLobby(_User *tP){

	_Match *const m = getMatchFromID(tP->CurrentMatchID);

	if (m){
		m->Lock.lock();
		m->removeUser(tP, 0);
		m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));//tell the other people that they left.
		m->Lock.unlock();
	}

	tP->inLobby = 1;
}

void Event_client_matchChangeSettings(_User *tP, const byte* const Packet, const DWORD Size){

	if (!tP->CurrentMatchID)return;

	_Match *m = getMatchFromID(tP->CurrentMatchID);

	if (!m)return;

	m->Lock.lock();

	if (m->HostID != tP->UserID){//Non host trying to change settings
		m->Lock.unlock();
		return;
	}

	const byte PreviousFree = m->Settings.FreeMod;
	const int PreviousBeatmapID = m->Settings.BeatmapID;
	const int PreviousMods = m->Settings.Mods;

	const byte pMatchType = m->Settings.MatchType;
	const byte pScoringType = m->Settings.ScoringType;
	const byte pTeamType = m->Settings.TeamType;

	ReadMatchData(m, Packet,Size, 1);

	const bool unReadyUsers = (PreviousMods != m->Settings.Mods || pMatchType != m->Settings.MatchType || pScoringType != m->Settings.ScoringType ||
						pTeamType != m->Settings.TeamType || PreviousFree != m->Settings.FreeMod || PreviousBeatmapID != m->Settings.BeatmapID);

	if (PreviousFree != m->Settings.FreeMod){

		const DWORD TimeMods = m->Settings.Mods & TimeAltering;
		const DWORD NonTimeMods = m->Settings.Mods - TimeMods;

		if (m->Settings.FreeMod){
			for (DWORD i = 0; i < 16; i++){
				if (m->Slots[i].User)m->Slots[i].CurrentMods = NonTimeMods;
				else m->Slots[i].CurrentMods = 0;
			}
			m->Settings.Mods = TimeMods;
		}
		else {
			for (DWORD i = 0; i < 16; i++) {
				if (m->Slots[i].User == tP)
					m->Settings.Mods = m->Slots[i].CurrentMods + TimeMods;
				
				m->Slots[i].CurrentMods = 0;
			}
		}
	}

	if (PreviousBeatmapID != m->Settings.BeatmapID){
		if (m->Settings.BeatmapID != -1){
			m->sendUpdate(bPacket::BotMessage("#multiplayer","(Bloodcat)[https://bloodcat.com/osu?q=" + std::to_string(m->Settings.BeatmapID) + "]"));
		}
	}

	if (unReadyUsers){
		m->ClearPlaying();
		m->UnreadyUsers();
	}

	m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));

	m->Lock.unlock();
}

void Event_client_matchLock(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size != 4)return;

	const DWORD& SlotID = Packet[0];

	if(_Match * m = getMatchFromID(tP->CurrentMatchID);
					m && SlotID < MULTI_MAXSIZE){

		m->Lock.lock();

		if (m->HostID == tP->UserID){

			if (_Slot& Slot = m->Slots[SlotID]; Slot.SlotStatus & SlotStatus::HasPlayer) {
				if (Slot.User != tP)
					m->removeUser(Slot.User, 1);
			}else
				Slot.SlotStatus = (Slot.SlotStatus == SlotStatus::Open) ? SlotStatus::Locked : SlotStatus::Open;

			m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
		}

		m->Lock.unlock();
	}

}

void Event_client_matchChangeMods(_User *tP, const byte* const Packet, const DWORD Size){

	if (_Match * m = getMatchFromID(tP->CurrentMatchID);m && Size == 4){

		const DWORD Mods = *(DWORD*)&Packet[0];

		MUTEX_LOCKGUARD(m->Lock);

		const bool isHost = (m->HostID == tP->UserID);

		if (!m->Settings.FreeMod){
			if (!isHost)return;
			m->Settings.Mods = Mods;
		}else{

			int SlotID = MULTI_MAXSIZE;

			for(auto& Slot : m->Slots)
				if (Slot.User == tP) {
					SlotID = GetIndex(m->Slots,Slot);
					break;
				}

			if (SlotID != MULTI_MAXSIZE){
				if (isHost)
					m->Settings.Mods = Mods & TimeAltering;

				m->Slots[SlotID].CurrentMods = Mods - (Mods & TimeAltering);
			}
		}

		m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
	}

	
}

void Event_client_joinMatch(_User *tP, const byte* const Packet, const DWORD Size) {


	if(!(tP->privileges & UserPublic))
		return tP->addQue(_BanchoPacket(OPac::server_matchJoinFail));

	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;
	
	if (O + 4 > End)return;

	const USHORT MatchID = USHORT(*(DWORD*)O); O += 4;
	const std::string_view Password = ReadUleb_View(O,End);

	if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {
		m->Lock.lock();
		m->removeUser(tP, 0);
		m->Lock.unlock();
	}

	bool Failed = 1;

	if (_Match * m = getMatchFromID(MatchID); m){

		m->Lock.lock();

		if (m->Settings.Password.size() == 0 || m->Settings.Password == Password && m->PlayerCount) {//Dont let players join empty matchs.

			for (auto& Slot : m->Slots){

				if (Slot.SlotStatus == SlotStatus::Open && !Slot.User){

					m->PlayerCount++;
					Slot.reset();
					Slot.SlotStatus = SlotStatus::NotReady;
					Slot.User = tP;

					tP->CurrentMatchID = m->MatchId;

					tP->addQue({
						bPacket::GenericString(OPac::server_channelJoinSuccess, "#multiplayer"),
						bPacket::bMatch(OPac::server_matchJoinSuccess, m, 1)
						});
					m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1), tP);
					Failed = 0;
					break;
				}
			}
		}

		m->Lock.unlock();
	}
	
	if (Failed)
		tP->addQue({bPacket4Byte(OPac::server_disposeMatch, MatchID),_BanchoPacket(OPac::server_matchJoinFail)});
	else tP->inLobby = 0;
}

void Event_client_matchChangeTeam(_User *tP){

	if (_Match* m = getMatchFromID(tP->CurrentMatchID); m){

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP){
				Slot.SlotTeam = !Slot.SlotTeam;
				m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
				break;
			}

		m->Lock.unlock();

	}
}

void Event_client_matchNoBeatmap(_User *tP){

	if (_Match* m = getMatchFromID(tP->CurrentMatchID); m){

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP){
				if (Slot.SlotStatus != SlotStatus::NoMap){
					Slot.SlotStatus = SlotStatus::NoMap;
					m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
				}
				break;
			}

		m->Lock.unlock();
	}

}

void Event_client_matchHasBeatmap(_User *tP){

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				if (Slot.SlotStatus == SlotStatus::NoMap) {
					Slot.SlotStatus = SlotStatus::NotReady;
					m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
				}break;
			}

		m->Lock.unlock();
	}
}

void Event_client_matchTransferHost(_User *tP, const byte* const Packet, const DWORD Size){


	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m && Size == 4) {

		const DWORD SlotID = al_min(*(DWORD*)&Packet[0],MULTI_MAXSIZE-1);

		m->Lock.lock();

		_Slot& Slot = m->Slots[SlotID];

		if (m->HostID == tP->UserID && Slot.User && Slot.User->choToken){

			m->HostID = Slot.User->UserID;

			Slot.User->addQue(_BanchoPacket(OPac::server_matchTransferHost));

			m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
		}

		m->Lock.unlock();

	}

}

void Event_client_matchReady(_User *tP) {

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP){
				Slot.SlotStatus = SlotStatus::Ready;
				m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
				break;
			}

		m->Lock.unlock();
	}
}

void Event_client_matchNotReady(_User *tP){

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				Slot.SlotStatus = SlotStatus::NotReady;
				m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
				break;
			}

		m->Lock.unlock();
	}
}

void Event_client_matchStart(_User *tP) {

	if (!tP->CurrentMatchID)return;//not in a match currently

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {
		m->Lock.lock();

		if (m->HostID != tP->UserID || m->Settings.BeatmapID == -1) {
			m->Lock.unlock();
			return;
		}

		m->ClearPlaying();

		for (auto& Slot : m->Slots)
			if (Slot.SlotStatus == SlotStatus::Ready || Slot.SlotStatus == SlotStatus::NotReady){
				Slot.resetPlaying();
				Slot.SlotStatus = SlotStatus::Playing;
			}

		m->sendUpdate(bPacket::bMatch(OPac::server_matchStart, m, 1));

		m->Lock.unlock();
	}

}

void Event_client_matchLoadComplete(_User *tP){

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		MUTEX_LOCKGUARD(m->Lock);

		bool AllLoaded = 1;

		for (auto& Slot : m->Slots)
			if (Slot.User == tP)
				Slot.Loaded = 1;
			else if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Loaded)
				AllLoaded = 0;

		if (AllLoaded)
			m->sendUpdates({
				_BanchoPacket(OPac::server_matchAllPlayersLoaded),
				bPacket::bMatch(OPac::server_updateMatch, m, 1)
				});
	}
}

void Event_client_matchScoreUpdate(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size < 10)return;
	if (!tP->CurrentMatchID)return;//not in a match currently

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m && Size >= 10) {

		_BanchoPacket b(OPac::server_matchScoreUpdate);
		AddMem(b.Data, Packet, Size);

		m->Lock.lock();
		
		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				b.Data[4] = (byte)GetIndex(m->Slots, Slot);
				m->sendUpdate(_M(b));
				break;
			}

		m->Lock.unlock();

	}
}

void Event_client_matchComplete(_User *tP) {

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP)
				Slot.Completed = 1;

		bool AllFinished = 1;

		for (auto& Slot : m->Slots)
			if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Completed) {
				AllFinished = 0;
				break;
			}

		if (AllFinished){

			m->ClearPlaying();

			m->sendUpdates({
				_BanchoPacket(OPac::server_matchComplete),
				bPacket::bMatch(OPac::server_updateMatch, m, 1)
				});
		}

		m->Lock.unlock();
	}
}

void Event_client_matchFailed(_User *tP) {

	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				m->sendUpdate(bPacket4Byte(OPac::server_matchPlayerFailed, GetIndex(m->Slots, Slot)));
				break;
			}

		m->Lock.unlock();
	}
}

void Event_client_matchSkipRequest(_User* tP) {
	if (_Match * m = getMatchFromID(tP->CurrentMatchID); m) {

		m->Lock.lock();

		for (auto& Slot : m->Slots)
			if (Slot.User == tP) {
				Slot.Skipped = 1;
				m->sendUpdate(bPacket4Byte(OPac::server_matchPlayerSkipped, GetIndex(m->Slots, Slot)));
				break;
			}

		bool AllPlayersSkipped = 1;

		for (auto& Slot : m->Slots)
			if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Skipped) {
				AllPlayersSkipped = 0;
				break;
			}

		if (AllPlayersSkipped)
			m->sendUpdate(_BanchoPacket(OPac::server_matchSkip));

		m->Lock.unlock();
	}
}

void Event_client_invite(_User *tP, const byte* const Packet, const DWORD Size){

	if (Size != 4)
		return;

	_Match *m = getMatchFromID(tP->CurrentMatchID);

	if (!m)return;//not in a match currently

	const DWORD TID = *(DWORD*)&Packet[0];

	if (TID < 1000)
		return tP->addQue(bPacket::BotMessage("#multiplayer", "Why would a bot want to join your match? You dirty shaved monkey."));;

	_UserRef Target(GetUserFromID(TID),1);

	if (!Target)
		return tP->addQue(bPacket::BotMessage("#multiplayer", "Could not find player."));
	

	tP->addQue(bPacket::BotMessage("#multiplayer", "Invited " + Target->Username + " to your match."));


	if (Target->isBlocked(tP->UserID))
		return;

	_BanchoPacket b = bPacket::Message(tP->Username, Target->Username,"I have invited you to the multiplayer lobby \"[osump://" + std::to_string(m->MatchId) + "/" + m->Settings.Password +" " + m->Settings.Name + "]\".",tP->UserID);
	//Users could fuck with the client [] url construction but its not dangerous in anyway so who cares.

	b.Type = OPac::server_invite;

	Target->addQue(b);
}

void Event_client_friendAdd(_User *tP, const byte* const Packet, const DWORD Size) {

	if (Size != 4)return;

	const DWORD ID = *(DWORD*)Packet;

	enum { ListFull, Added, AlreadyIn };

	char Status = ListFull;

	for (USHORT i = 0; i < 256; i++){
		if (tP->Friends[i] == ID){
			Status = AlreadyIn;
			break;
		}
		if (!tP->Friends[i]){
			Status = Added;
			tP->Friends[i] = ID;
			break;
		}
	}
	if (Status == ListFull)
		tP->addQue(bPacket::Notification("You may only have a maximum of 256 friends."));
	else if (Status == Added)
		SQLExecQue.AddQue(SQL_INSERT("users_relationships",{
			_SQLKey("user1", tP->UserID),
			_SQLKey("user2", ID)
		}));
}

void Event_client_friendRemove(_User *tP, const byte* const Packet, const DWORD Size) {

	if (Size != 4)return;

	const DWORD ID = *(DWORD*)Packet;

	enum { NotRemoved, Removed};

	char Status = NotRemoved;

	for (USHORT i = 0; i < 256; i++) {
		if (tP->Friends[i] == ID){
			tP->Friends[i] = 0;
			Status = Removed;
			break;
		}
	}
	
	if (Status == Removed)
		SQLExecQue.AddQue("DELETE FROM users_relationships WHERE user1 = " + std::to_string(tP->UserID) + " AND user2 = " + std::to_string(ID));
	
}



_inline void debug_LogOutUser(_User *tP){

	const DWORD UID = tP->UserID;

	DisconnectUser(tP);

	if (!UID)return;

	const _BanchoPacket b = bPacket4Byte(OPac::server_userLogout, UID);
	
	for (auto& User : Users)
		if (User.choToken)
			User.addQue(b);
}

/*
void IngameMenu(_User* u, _Con s){

	if (!u->Menu.State || !u->choToken)return;

	std::string Res;

	if (u->Menu.State == MenuStates::Menu_ChooseScore) {


		if (u->Menu.aux1) {
			const int ID = u->Menu.aux1;
			std::string bMD5 = u->Menu.saux1;
			u->Menu.aux1 = 0;
			u->Menu.saux1.clear();

			PrepareSQLString(bMD5);

			SQL_BanchoThread[s.ID].ExecuteUPDATE("UPDATE scores SET completed = 2 WHERE userid = " + std::to_string(u->UserID) + " AND beatmap_md5 = '" + bMD5 + "' AND completed = 3 LIMIT 1");//TODO see if i can just slap a ; and do two in one
			SQL_BanchoThread[s.ID].ExecuteUPDATE("UPDATE scores SET completed = 3 WHERE id = " + std::to_string(ID) + " AND userid = " + std::to_string(u->UserID) + " LIMIT 1");

		}

		DWORD BID = u->BeatmapID;

		if (BID && (u->Menu.ForceReDraw || BID != u->Menu.PreviousMap)) {
			//TODO add relax
			const std::string bMD5 = std::string(u->ActionMD5, 32);

			sql::ResultSet *res = SQL_BanchoThread[s.ID].ExecuteQuery("SELECT id,score,max_combo,mods,accuracy,pp,completed FROM scores WHERE userid = "
				+ std::to_string(u->UserID) + " AND beatmap_md5 = '" + bMD5 + "' AND completed = 2 ORDER BY pp DESC");

			DWORD Count = 0;
			Res += "This will also affect your total pp.\n";

			while (res && res->next()) {
				if (Count > 8)break;
				
				Res += MenuClickButton("iaux1=" + res->getString(1) + "|saux1=" + bMD5, "[Set as Top]");

				Res += "   Score: " + res->getString(2) + " | Acc: " + RoundTo2(res->getDouble(5)) + " | pp: " + RoundTo2(res->getDouble(6)) + "\n";

				Count++;
			}
			delete res;
			u->Menu.PreviousMap = BID;

			if (Count == 0)
				Res += "You do not appear to have any scores on this map\n";

			Res += MenuClickButton("exit", "Exit Menu");
		}
	}
	if (u->Menu.ForceReDraw)Res += " ";
	u->Menu.ForceReDraw = 0;
	if (Res.size()){
		if (!u->Menu.BanchoLoaded) {
			u->Menu.BanchoLoaded = 1;
			u->addQueNonLocking(bPacket::UserPanel(998, 0));
			u->addQueNonLocking(bPacket::UserStats(998, 0));
		}else u->addQue(bPacket::GenericInt32(94, 998));

		u->addQueDelay(_DelayedBanchoPacket(1, bPacket::BotMessage(u->Username, Res,FAKEUSER_NAME, 998)));
	}

}*/

void Event_client_requestStatusUpdate(_User *const tP){
	tP->addQue(bPacket::UserStats(tP));
}

void DoBanchoPacket(_Con s,const uint64_t choToken,const std::vector<byte> &PacketBundle){

	_UserRef tP(GetUserFromToken(choToken),1);

	if (!tP){//No user online with that token

		s.SendData(ConstructResponse(200, Empty_Headers, bPacket4Byte(OPac::server_restart,1).GetBytes()));
		//printf("Request relogin\n");
		return;
	}

	tP->LastPacketTime = clock_ms();

	DWORD In = 0;

	while (In <= PacketBundle.size() - 7){

		const USHORT PacketID = *(USHORT*)&PacketBundle[In];
		const DWORD PacketSize = *(DWORD*)&PacketBundle[In + 3];

		In += 7;

		if (In + PacketSize > PacketBundle.size()) {
			printf("%s> Out of range packet %i|%i|%llu\n", tP->Username.c_str(), PacketID, (In + PacketSize), PacketBundle.size());
			tP->choToken = 0;
			return;
		}

		const byte* const Packet = (byte*)&PacketBundle[In];
		In += PacketSize;

		switch (PacketID){
			
		case 68://Client asking if certain beatmaps are ranked.
		case 79://Client asking for stats updates. ruri does all this automatically so its useless :)
		case OPac::client_pong:
			break;

		case OPac::client_requestStatusUpdate:
			Event_client_requestStatusUpdate(tP.User);
			break;

		case OPac::client_changeAction:
			Event_client_changeAction(tP.User, Packet, PacketSize);
			break;

		case OPac::client_userStatsRequest:
			Event_client_userStatsRequest(tP.User, Packet, PacketSize);
			break;

		case OPac::client_channelJoin:
			Event_client_channelJoin(tP.User, Packet, PacketSize);
			break;

		case OPac::client_channelPart:
			Event_client_channelPart(tP.User, Packet, PacketSize);
			break;

		case OPac::client_sendPublicMessage:
			Event_client_sendPublicMessage(tP.User, Packet, PacketSize);
			break;

		case OPac::client_sendPrivateMessage:
			Event_client_sendPrivateMessage(tP.User, Packet, PacketSize);
			break;

		case OPac::client_logout:
			if (tP->LoginTime + 5000 > tP.User->LastPacketTime)break;
			debug_LogOutUser(tP.User);
			break;

		case OPac::client_startSpectating:
			Event_client_startSpectating(tP.User, Packet, PacketSize);
			break;

		case OPac::client_stopSpectating:
			Event_client_stopSpectating(tP.User);
			break;
		case OPac::client_cantSpectate:
			Event_client_cantSpectate(tP.User);
			break;

		case OPac::client_spectateFrames:
			Event_client_spectateFrames(tP.User,Packet,PacketSize);
			break;

		case OPac::client_createMatch:
			Event_client_createMatch(tP.User, Packet, PacketSize);
			break;

		case OPac::client_partMatch:
			Event_client_partMatch(tP.User);
			break;

		case OPac::client_matchChangeSlot:
			Event_client_matchChangeSlot(tP.User,Packet, PacketSize);
			break;

		case OPac::client_joinLobby:
			Event_client_joinLobby(tP.User);
			break;

		case OPac::client_partLobby:
			tP->inLobby = 0;
			break;

		case OPac::client_matchChangeSettings:
			Event_client_matchChangeSettings(tP.User,Packet, PacketSize);
			break;

		case OPac::client_matchLock:
			Event_client_matchLock(tP.User, Packet, PacketSize);
			break;

		case OPac::client_matchChangeMods:
			Event_client_matchChangeMods(tP.User, Packet, PacketSize);
			break;

		case OPac::client_joinMatch:
			Event_client_joinMatch(tP.User, Packet, PacketSize);
			break;

		case client_matchChangeTeam:
			Event_client_matchChangeTeam(tP.User);
			break;

		case client_matchNoBeatmap:
			Event_client_matchNoBeatmap(tP.User);
			break;

		case client_matchHasBeatmap:
			Event_client_matchHasBeatmap(tP.User);
			break;

		case client_matchTransferHost:
			Event_client_matchTransferHost(tP.User, Packet, PacketSize);
			break;
		
		case client_matchReady:
			Event_client_matchReady(tP.User);
			break;

		case client_matchNotReady:
			Event_client_matchNotReady(tP.User);
			break;

		case client_matchStart:
			Event_client_matchStart(tP.User);
			break;

		case client_matchLoadComplete:
			Event_client_matchLoadComplete(tP.User);
			break;

		case client_matchScoreUpdate:
			Event_client_matchScoreUpdate(tP.User, Packet, PacketSize);
			break;

		case client_matchComplete:
			Event_client_matchComplete(tP.User);
			break;

		case client_matchFailed:
			Event_client_matchFailed(tP.User);
			break;

		case client_matchSkipRequest:
			Event_client_matchSkipRequest(tP.User);
			break;

		case client_invite:
			Event_client_invite(tP.User, Packet, PacketSize);
			break;
		case client_friendAdd:
			Event_client_friendAdd(tP.User, Packet, PacketSize);
			break;
		case client_friendRemove:
			Event_client_friendRemove(tP.User, Packet, PacketSize);
			break;


		default:
			printf("PacketID:%i | Length:%i\n", PacketID, PacketSize);
			break;
		}

		//const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();

		//if(PacketID == 0 || PacketID == 4 || PacketID == 85)
			//printf("P%i: %fms\n", PacketID, double(double(TTime) / 1000000.0));
		
	}

	SendMatchList(tP.User, 0);//Sends multiplayer data if they are in the lobby.
	//IngameMenu(tP,s);//Handles all the cool new clickable menus

	tP->LastPacketTime = clock_ms();

	tP->doQue(s);

}

const std::vector<byte> PingPacket = _BanchoPacket(OPac::server_ping).GetBytes();

const char* countryCodes[] = {
	"--","AP","EU","AD","AE","AF","AG","AI","AL","AM","AN","AO","AQ","AR",
	"AS","AT","AU","AW","AZ","BA","BB","BD","BE","BF","BG","BH","BI","BJ",
	"BM","BN","BO","BR","BS","BT","BV","BW","BY","BZ","CA","CC","CD","CF",
	"CG","CH","CI","CK","CL","CM","CN","CO","CR","CU","CV","CX","CY","CZ",
	"DE","DJ","DK","DM","DO","DZ","EC","EE","EG","EH","ER","ES","ET","FI",
	"FJ","FK","FM","FO","FR","FX","GA","GB","GD","GE","GF","GH","GI","GL",
	"GM","GN","GP","GQ","GR","GS","GT","GU","GW","GY","HK","HM","HN","HR",
	"HT","HU","ID","IE","IL","IN","IO","IQ","IR","IS","IT","JM","JO","JP",
	"KE","KG","KH","KI","KM","KN","KP","KR","KW","KY","KZ","LA","LB","LC",
	"LI","LK","LR","LS","LT","LU","LV","LY","MA","MC","MD","MG","MH","MK",
	"ML","MM","MN","MO","MP","MQ","MR","MS","MT","MU","MV","MW","MX","MY",
	"MZ","NA","NC","NE","NF","NG","NI","NL","NO","NP","NR","NU","NZ","OM",
	"PA","PE","PF","PG","PH","PK","PL","PM","PN","PR","PS","PT","PW","PY",
	"QA","RE","RO","RU","RW","SA","SB","SC","SD","SE","SG","SH","SI","SJ",
	"SK","SL","SM","SN","SO","SR","ST","SV","SY","SZ","TC","TD","TF","TG",
	"TH","TJ","TK","TM","TN","TO","TL","TR","TT","TV","TW","TZ","UA","UG",
	"UM","US","UY","UZ","VA","VC","VE","VG","VI","VN","VU","WF","WS","YE",
	"YT","RS","ZA","ZM","ME","ZW","A1","A2","O1","AX","GG","IM","JE","BL",
	"MF" };

_inline byte getCountryNum(const USHORT isoCode){

	for (byte i = 0; i < 253; i++){
		if (isoCode == *(USHORT*)countryCodes[i])
			return i;
	}	
	return 0;
}

const std::vector<byte> PACKET_INCORRECTLOGIN = []{return _BanchoPacket(OPac::server_userID, VEC(byte){0xff,0xff,0xff,0xff}).GetBytes();}();
const std::vector<byte> PACKET_SERVERFULL = [] {
	std::vector<byte> packet = _BanchoPacket(OPac::server_userID, VEC(byte){ 0xff,0xff,0xff,0xff }).GetBytes();
	AddVector(packet, _M(bPacket::Notification("Server is currently full").GetBytes()));
	return packet;
}();
const std::vector<byte> PACKET_CLIENTOUTOFDATE = [] {
	std::vector<byte> packet = _BanchoPacket(OPac::server_userID, VEC(byte){ 0xff,0xff,0xff,0xff }).GetBytes();
	AddVector(packet, _M(bPacket::Notification("Your client is out of date!\nPlease update it.").GetBytes()));
	return packet;
}();

void BanchoIncorrectLogin(_Con s){
	s.SendData(ConstructResponse(200, { {"cho-token", "0"} }, PACKET_INCORRECTLOGIN));
}
void BanchoServerFull(_Con s) {
	LogError("Server Full");
	s.SendData(ConstructResponse(200, { {"cho-token", "0"} }, PACKET_SERVERFULL));
}

void HandleBanchoPacket(_Con s, const _HttpRes &&res,const uint64_t choToken) {

	if (res.Body.size() <= 1)
		return;

	if (!choToken){//No token sent - Assume its the login request which only ever comes in once

		std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();

		const auto LoginData = Explode_View(res.Body, '\n',3);

		if (LoginData.size() != 3)
			return BanchoIncorrectLogin(s);

		std::string Username = USERNAMESQL(LoginData[0]);

		const _MD5 cPassword(USERNAMESQL(LoginData[1]));

		const auto ClientData = Explode_View(LoginData[2],'|', 5);

		if (ClientData.size() != 5 || Username.size() > MAX_USERNAME_LENGTH)
			return BanchoIncorrectLogin(s);

		const std::string Username_Safe = Username;

		const bool VersionFailed = [&]{

			return 0;
			/*
			const size_t Start = (size_t)&ClientData[0][0];
			const size_t End = (size_t)&ClientData[0][ClientData[0].size()] - 4;

			for (size_t i = Start; i < End; i++)
				if (*(USHORT*)i == *(USHORT*)"20" && MemToInt32(i, 4) >= 2019)
					return 0;
		
			return 1;*/
		}();

		if(VersionFailed)
			return (void)s.SendData(ConstructResponse(200, { {"cho-token", "0"} }, PACKET_CLIENTOUTOFDATE));

		int UserID = 0;
		int Priv = 0;
		int SilenceEnd = 0;
		byte CountryCode = 0;
		bool NewLogin = 0;

		_UserRef u(GetUserFromNameSafe(Username_Safe, 1),1);

		if (u.User && u->Password == cPassword){
			UserID = u->UserID;
			Priv = u->privileges;//TODO make this able to be updated.
			SilenceEnd = u->silence_end;
			CountryCode = u->country;
		}else u.User = 0;

		if(!u){
			NewLogin = 1;
			_SQLCon *const con = &SQL_BanchoThread[s.ID];

			sql::ResultSet *res = con->ExecuteQuery("SELECT id, password_md5, username, privileges,silence_end FROM users WHERE username_safe = '" + Username_Safe + "' LIMIT 1");

			if (!res || !res->next()){
				DeleteAndNull(res);
				return BanchoIncorrectLogin(s);
			}

			UserID = res->getInt(1);

			if (bcrypt_checkpw(cPassword.to_string().c_str(), res->getString(2).c_str())){
				DeleteAndNull(res);
				return BanchoIncorrectLogin(s);//Might want to add a brute force lock out
			}
			if (!u) {

				u.Reset(GetPlayerSlot_Safe(Username_Safe),1);
				if (!u) {
					DeleteAndNull(res);
					return BanchoServerFull(s);
				}

			}

			Username = res->getString(3);//get the database captialization for consistencies sake
			Priv = res->getInt(4);
			SilenceEnd = res->getInt(5);

			DeleteAndNull(res);

			if (Username != GetUsernameFromCache(UserID))
				UsernameCacheUpdateName(UserID, Username, &SQL_BanchoThread[s.ID]);

			const std::string TableName[] = {"users_stats","rx_stats"};

			for(byte z = 0; z < 2; z++){

				res = con->ExecuteQuery("SELECT ranked_score_std, playcount_std, total_score_std, avg_accuracy_std,pp_std,"
					"ranked_score_taiko, playcount_taiko, total_score_taiko, avg_accuracy_taiko, pp_taiko,"
					"ranked_score_ctb, playcount_ctb, total_score_ctb, avg_accuracy_ctb, pp_ctb,"
					"ranked_score_mania, playcount_mania, total_score_mania, avg_accuracy_mania, pp_mania, country FROM " + TableName[z] + " WHERE id = " + std::to_string(UserID) + " LIMIT 1");

				if (res && res->next()){


					if (z == 0) {

						const std::string Country = res->getString(21);

						if (Country.size() == 2)
							CountryCode = getCountryNum(*(USHORT*)&Country[0]);

					}
					DWORD Offset = 0;

					for (byte i = 0; i < 4; i++){

						const DWORD Slot = i + (z * 4);

						u->Stats[Slot].rScore = res->getInt64(++Offset);
						u->Stats[Slot].PlayCount = res->getInt(++Offset);
						u->Stats[Slot].tScore = res->getInt64(++Offset);
						u->Stats[Slot].Acc = res->getDouble(++Offset) * 0.01f;
						u->Stats[Slot].pp = res->getInt(++Offset);

					}
				}
				DeleteAndNull(res);
			}

			res = con->ExecuteQuery("SELECT user2 FROM users_relationships WHERE user1 = " + std::to_string(UserID) + " LIMIT 256");
			DWORD fCount = 0;
			while (res && res->next()){
				u->Friends[fCount] = res->getInt(1);
				fCount++;
			}

			DeleteAndNull(res);

			//Todo HWID
		}

		//chan_DevLog.Bot_SendMessage(Username + " Logged in.");

		{
			if (!u){

				u.Reset(GetPlayerSlot_Safe(Username_Safe),1);

				if (!u)
					return BanchoServerFull(s);
			}

			DisconnectUser(u.User);

			u->privileges = Priv;

			u->c1Check = [&]()->std::string{
				const auto cHash = Explode_View(ClientData[3], ':', 5);

				if (cHash.size() < 5)return "";

				return std::string(cHash[3]) + "|" + std::string(cHash[4]);
			}();

			u->FriendsOnlyChat = (ClientData[4] == "1");

			u->timeOffset = MemToInt32(&ClientData[1][0], ClientData[1].size());			

			u->country = CountryCode;// getCountryNum(res.GetHeaderValue("CF-IPCountry")); the nature of private servers disallow this.. i guess bancho could use this :(
						   // Best course of action would to resolve it from cloudflare on registration, not verification.

			u->LoginTime = clock_ms();
			u->LastPacketTime = u->LoginTime;
			u->UserID = UserID;
			u->silence_end = SilenceEnd;

			if(NewLogin)
				u->Username = Username;

			u->Username_Safe = Username_Safe;
			u->Password = cPassword;

			u->choToken = GenerateChoToken();

			u->qLock.lock();

			u->addQueNonLocking(bPacket::Notification("Welcome to ruri.\nBuild: " __DATE__ " " __TIME__));

			const int SilenceDelta = (SilenceEnd && SilenceEnd > time(0)) ? SilenceEnd - time(0) : 0;

			if (!SilenceDelta)u->silence_end = 0;

			u->addQueNonLocking(bPacket4Byte(OPac::server_silenceEnd, SilenceDelta));

			u->addQueNonLocking(bPacket4Byte(OPac::server_userID, UserID));
			u->addQueNonLocking(bPacket4Byte(OPac::server_protocolVersion, CHO_VERSION));
			u->addQueNonLocking(bPacket4Byte(OPac::server_supporterGMT, Supporter));
			u->addQueNonLocking(bPacket::UserPanel(u.User));
			u->addQueNonLocking(bPacket::UserStats(u.User));
			
			u->addQueNonLocking(bPacket4Byte(OPac::server_channelInfoEnd, 0));//Sending this after loading the channels fucks with the desired order. So it is here instead.

			const int IRC_LEVEL = GetMaxPerm(u->privileges);
			
			for (DWORD i = 0; i < ChannelListSize(); i++){

				if (ChannelList[i]->ViewLevel > IRC_LEVEL)
					continue;

				u->addQueNonLocking(bPacket::ChannelInfo(ChannelList[i]));
				
				if (ChannelList[i]->AutoJoin){
					u->addQueNonLocking(bPacket::GenericString(OPac::server_channelJoinSuccess, ChannelList[i]->ChannelName));
					ChannelList[i]->JoinChannel(u.User);
				}
			}

			u->addQueNonLocking(bPacket::GenericString(OPac::server_channelKicked, "#osu"));

			std::vector<DWORD> FriendsList;
			FriendsList.reserve(256);
			FriendsList.push_back(999);//Bot is always their friend.
			FriendsList.push_back(UserID);//Shows self in the friend ranking.

			for (DWORD i = 0; i < 256; i++) {
				if (u->Friends[i])
					FriendsList.push_back(u->Friends[i]);
				else break;
			}

			u->addQueNonLocking(bPacket::GenericDWORDList(OPac::server_friendsList, FriendsList, 0));

			u->addQueNonLocking(bPacket::UserPanel(999, 0));
			u->addQueNonLocking(USER_STATS(999, 0));
			
			for (auto& gUser : Users) {
				if (!gUser.choToken || &gUser == u.User || u->isBlocked(gUser.UserID))
					continue;
				u->addQueNonLocking(bPacket::UserPanel(gUser.UserID, UserID));
				u->addQueNonLocking(USER_STATS(gUser.UserID, UserID));
			}

			u->SendToken = 1;
			u->qLock.unlock();

			const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
			printf("LoginTime: %fms\n", double(double(TTime) / 1000000.0));
			;
			u->doQue(s);

			debug_SendOnlineToAll(u.User);
		}

		return;
	}
	else DoBanchoPacket(s,choToken,res.Body);

}


void DisconnectUser(_User *u){

	if (!u || !u->UserID || !u->choToken)return;

	for (DWORD i = 0; i < MAX_CHAN_COUNT; i++) {

		_Channel*const C = (_Channel*)u->ActiveChannels[i];

		if (!C)continue;

		C->PartChannel(u);
	}

	u->choToken = 0;
	u->inLobby = 0;

	if (u->CurrentMatchID){

		_Match *m = getMatchFromID(u->CurrentMatchID);

		if (m) {
			m->Lock.lock();
			m->removeUser(u, 0);
			m->sendUpdate(bPacket::bMatch(OPac::server_updateMatch, m, 1));
			m->Lock.unlock();
		}
		u->CurrentMatchID = 0;
	}

	Event_client_stopSpectating(u);

	for (byte i = 0; i < 8; i++)
		u->Stats[i].reset();

	u->Spectators.clear();

	u->qLock.lock();
	u->Que.clear();
	u->dQue.clear();
	u->qLock.unlock();
}

void HandlePacket(_Con s){

	 _HttpRes res;

	if (!s.RecvData(res)){
		LogMessage(KRED "Connection lost");
		return s.Dis();
	}
	
	const std::string& UserAgent = res.GetHeaderValue("User-Agent");

	const uint64_t choToken = StringToUInt64(res.GetHeaderValue("osu-token"));

	if (UserAgent.size() == 0){

		const std::string& UserAgent2 = res.GetHeaderValue("user-agent");


		if (!UserAgent2.size()) {
			LogMessage("No user agent set.");
			s.SendData(ConstructResponse(200, Empty_Headers, bPacket::Notification("If there is anything that seems wrong make sure to contact rumoi.").GetBytes()));
			s.Dis();
			return;
		}
		if (UserAgent2 != "osu!" && !choToken) {//If it is not found
			RenderHTMLPage(s, _M(res));
			LogMessage("HTML page served");
			return s.Dis();
		}
	}

	HandleBanchoPacket(s, _M(res), choToken);

	return s.Dis();
}

//Used to logout dropped users and other house keeping
void LazyThread(){
	printf("LazyThread running.\n");

	_SQLCon lThreadSQL;
	lThreadSQL.Connect();

	while (1){

		Sleep(500);

		const int cTime = clock_ms();
		
		DWORD PCount = 0;

		for (auto& User : Users){

			if (User.LastPacketTime == INT_MIN)
				continue;

			if (User.choToken){
				PCount++;
				if (User.LastPacketTime + PING_TIMEOUT_OSU < cTime) {
					_UserRef UF(&User, 0);
					debug_LogOutUser(UF.User);
				}
			}else if (User.LastPacketTime + FREE_SLOT_TIME < cTime)//Free slots after 30 minutes of logging out.
				User.LastPacketTime = INT_MIN;


		}
		COUNT_CURRENTONLINE = PCount;

		Sleep(500);

		DWORD ActiveLobbies = 0;

		for (DWORD i = 0; i < MAX_MULTI_COUNT; i++)
			if (Match[i].PlayerCount)
				ActiveLobbies++;

		COUNT_MULTIPLAYER = ActiveLobbies;

		if (SQLExecQue.Commands.size()){

			SQLExecQue.CommandLock.lock();
			{
				for (std::string& Command : SQLExecQue.Commands)
					lThreadSQL.ExecuteUPDATE(_M(Command), 1);

				SQLExecQue.Commands.clear();
			}
			SQLExecQue.CommandLock.unlock();

		}


	}
}


const std::string PPColNames[] = { "pp_std","pp_taiko","pp_ctb","pp_mania" };
const std::string UserTableNames[] = { "users_stats","rx_stats" };

void DoFillRank(DWORD I, bool TableName){

	RankList[I].reserve(USHORT(-1));

	sql::ResultSet *res = SQL_BanchoThread[I].ExecuteQuery("SELECT id, " + PPColNames[I] + " FROM " + UserTableNames[TableName] + " WHERE " + PPColNames[I] + " > 0 ORDER BY " + PPColNames[I] + " DESC");

	DWORD cOffset = 0;

	if (TableName)
		I += 4;
	while (res && res->next()) {
		RankList[I].emplace_back(res->getUInt(1), res->getUInt(2));
		cOffset++;
	}
	if (res)delete res;
}

void PushUsers() {

	{

		struct CacheUsers {

			VEC(DWORD) PrivTable;
			VEC(byte) UserData;

			void push_User(const _User& u){

				VEC(byte) Data;

				AddStream(Data, u.choToken ^ size_t(u.privileges));
				AddStream(Data, u.UserID);
				AddStream(Data, u.privileges);
				AddStream(Data, u.silence_end);
				AddString(Data, u.Username);
				AddString(Data, u.Username_Safe);
				AddMem(Data, &u.Password.MD5[0], 16);
				AddMem(Data, &u.ActionMD5[0], 32);
				AddStream(Data, u.actionMods);
				AddStream(Data, u.BeatmapID);
				Data.push_back(u.timeOffset);
				Data.push_back(u.country);
				Data.push_back(u.GameMode);
				Data.push_back(u.actionID);
				AddString(Data, u.ActionText);

				if (!u.CurrentlySpectating)
					AddStream(Data, 0);
				else AddStream(Data, u.CurrentlySpectating->UserID);

				{

					AddStream(Data, u.Spectators.size());

					for (const auto Spec : u.Spectators)//Spectators changing at the very instant the server restarts is astronomically rare.
						Spec ? AddStream(Data, Spec->UserID) : AddStream(Data, 0);

				}

				AddStream(Data, u.CurrentMatchID);
				AddStream(Data, u.LastSentBeatmap);

				AddMem(Data, &u.Friends[0], sizeof(u.Friends));
				AddMem(Data, &u.Blocked[0], sizeof(u.Blocked));
				Data.push_back(u.FriendsOnlyChat);

				for (DWORD i = 0; i < sizeof(u.ActiveChannels) / sizeof(u.ActiveChannels[0]); i++)
					u.ActiveChannels[i] ? AddStream(Data, ((_Channel*)u.ActiveChannels[i])->NameSum) : AddStream(Data, 0);

				AddString(Data, u.c1Check);

				PrivTable.push_back(u.privileges);
				AddStream(UserData, Data.size());
				AddVector(UserData, _M(Data));

			}

		};



		CacheUsers Cached;

		VEC(_User) UserTable;

		for (const auto& User : Users) {

			_UserRef u((_User*)& User, 0);

			if (!(User.privileges & 1))//Banned users can feel the pain :)
				continue;

			Cached.push_User(User);
		}

	}

}


void FillRankCache(){

		printf("Filling rank cache (std)\n");
		{			
			std::array<std::thread, 4> threads = { std::thread(DoFillRank,0,0),std::thread(DoFillRank,1,0),std::thread(DoFillRank,2,0),std::thread(DoFillRank,3,0) };
			for (auto& t : threads)
				t.join();
		}

		#ifndef NO_RELAX

		printf("Filling rank cache (relax)\n");
		{
			std::array<std::thread, 4> threads = { std::thread(DoFillRank,0,1),std::thread(DoFillRank,1,1),std::thread(DoFillRank,2,1),std::thread(DoFillRank,3,1) };
			for (auto& t : threads)
				t.join();
		}
		#endif
		printf("Completed.\n");
}


std::mutex BanchoWorkLock[BANCHO_THREAD_COUNT];
std::vector<_Con> BanchoConnectionQue[BANCHO_THREAD_COUNT];
void BanchoWork(const DWORD ID){
	while(1){

		size_t Count = 0;
		_Con* Req = 0;

		Count = BanchoConnectionQue[ID].size();

		if (Count){
			BanchoWorkLock[ID].lock();

			Count = BanchoConnectionQue[ID].size();

			Req = new _Con[Count];

			if(Req)memcpy(&Req[0], &BanchoConnectionQue[ID][0], Count * sizeof(_Con));

			BanchoConnectionQue[ID].clear();

			BanchoWorkLock[ID].unlock();
		}
		for (DWORD i = 0; i < Count; i++)
			HandlePacket(Req[i]);

		if(Req)delete[] Req;

		Sleep(1);//It being 0 literally hogs an entire core so.. No?
	}
}

#include "ruri_API.h"

void receiveConnections(){

	printf("Starting BanchoThreads\n");

	{
		for (DWORD i = 0; i < BANCHO_THREAD_COUNT; i++){
			BanchoConnectionQue[i].reserve(64);
			printf("BanchoThread%i: %i\n", i, int(SQL_BanchoThread[i].Connect()));
			std::thread a(BanchoWork, i);
			a.detach();
		}
	}

	FillRankCache();

	UpdateUsernameCache(&SQL_BanchoThread[0]);
	
	{
		std::thread a(LazyThread);
		a.detach();
	}

	{
		std::thread a(Aria_Main);
		a.detach();
	}
#if defined(LINUX) && defined(API)
	{
		std::thread a(ruri_API);
		a.detach();
	}
#endif

	while (!ARIALOADED)
		Sleep(100);

#ifndef LINUX
	SOCKET listening = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listening == INVALID_SOCKET)
	{
		printf("Failed to load listening socket.\n");
		return;
	}
	sockaddr_in hint;
	ZeroMemory(&hint.sin_addr, sizeof(hint.sin_addr));
	hint.sin_family = AF_INET;
	hint.sin_port = htons(RURIPORT);

	::bind(listening, (sockaddr*)&hint, sizeof(hint));
	::listen(listening, SOMAXCONN);

	sockaddr_in client;

	DWORD Time = 2000;
	DWORD MPL = MAX_PACKET_LENGTH;

	setsockopt(listening, SOL_SOCKET, SO_RCVTIMEO, (char*)&Time, 4);
	setsockopt(listening, SOL_SOCKET, SO_SNDTIMEO, (char*)&Time, 4);
	setsockopt(listening, SOL_SOCKET, SO_RCVBUF, (char*)&MPL, 4);
#else

	struct sockaddr_un serveraddr;

	SOCKET listening = socket(AF_UNIX, SOCK_STREAM, 0);

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, RURI_UNIX_SOCKET);

	unlink(RURI_UNIX_SOCKET);

	if (bind(listening, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr)) < 0) {
		perror("bind() failed");
		return;
	}
	if (listen(listening, SOMAXCONN) < 0){
		perror("listen() failed");
		return;
	}
	chmod(RURI_UNIX_SOCKET, S_IRWXU | S_IRWXG | S_IRWXO);
	
#endif

	std::printf(KGRN "Listening to socket\n" KRESET);

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
		
		COUNT_REQUESTS++;

		BanchoWorkLock[ID].lock();

		BanchoConnectionQue[ID].push_back(_Con(s,ID));

		BanchoWorkLock[ID].unlock();

		ID++;
		if (ID >= BANCHO_THREAD_COUNT)ID = 0;

	}
}



std::string ExtractConfigValue(const std::vector<byte> &Input){

	DWORD Start = 0;
	DWORD End = 0;

	for (DWORD i = 0; i < Input.size(); i++){
		if (!Start){
			if (Input[i] == '"')
				Start = i+1;
		}
		else if (Input[i] == '"') {
			End = i;
			break;
		}
	}


	if (!Start || !End)
		return "";

	return std::string(Input.begin() + Start, Input.begin() + End);
}

int main(){


	const std::vector<byte> ConfigBytes = LOAD_FILE("config.ini");

	if (!ConfigBytes.size()){
		printf("\nconfig.ini missing.\n");
		return 0;
	}

	for (const auto& Config : Explode_View(ConfigBytes, '\n',1)){

	#define V ExtractConfigValue(std::vector<byte>(Config.cbegin(),Config.cend()))

		if (MEM_CMP_START(Config, "osu_API_Key"))
			osu_API_KEY = V;
		else if (MEM_CMP_START(Config, "SQL_Password"))
			SQL_Password = V;
		else if (MEM_CMP_START(Config, "SQL_Username"))
			SQL_Username = V;
		else if (MEM_CMP_START(Config, "SQL_Schema"))
			SQL_Schema = V;
		else if (MEM_CMP_START(Config, "BeatmapPath"))
			BEATMAP_PATH = V;
		else if (MEM_CMP_START(Config, "ReplayPath"))
			REPLAY_PATH = V;

	#undef V

	}

	static_assert((BANCHO_THREAD_COUNT >= 4 && ARIA_THREAD_COUNT >= 4),
		"BANCHO_THREAD_COUNT or ARIA_THREAD_COUNT can not be below 4");

	if (osu_API_KEY.size() == 0)
		printf("No api key was given. Some features will not work.\n");
	else osu_API_BEATMAP = "api/get_beatmaps?k=" + osu_API_KEY + "&";

	if (WSAStartup(MAKEWORD(2, 2), &wsData)) {
		printf("Failed to load WSA.\n");
		return 0;
	}
	receiveConnections();
	
	return 1;
}
