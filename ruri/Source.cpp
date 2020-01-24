/*#define USERID_START 1000
#define M_BOT_NAME "ruri"
#define BOT_LOCATION 0*/
#define USERID_START 10
#define M_BOT_NAME "Chloe"
#define BOT_LOCATION 111

#define BANCHO_THREAD_COUNT 4
#define ARIA_THREAD_COUNT 4

constexpr bool UsingRawMirror = 1;

enum class Privileges : unsigned int {

	Visible = 1 << 0,
	Verified = 1 << 1,
	Tournament_Manager = 1 << 2,

	Donor = 1 << 6,
	Premium = 1 << 7,
	Alumni = 1 << 8,

	Mod_General = 1 << 12,//Can silence people
	Mod_Nominator = 1 << 13,//Can rank or love a map
	Mod_MapManager = 1 << 14,//Can rank/unrank/love or change the locked status of a map

	Admin_General = 1 << 21,//Can restrict people and read reports.
	Admin_ManageUsers = 1 << 22,//Can manage users name/email and other personal data.
	Admin_Alert = 1 << 23,
	Admin_Wipe = 1 << 24,//Basically allows the user to evoke/invoke scores for accounts

	SuperAdmin = 1 << 30,//Highest permission. Manages users and admins alike.

	Banned = 0,

	CanRankMaps = Mod_Nominator | Mod_MapManager,

	Name_Yellow = Donor | Premium | Alumni,
	Name_Orange = Mod_General | Mod_Nominator | Mod_MapManager,
	Name_Periwinkle = Admin_General | Admin_ManageUsers | Admin_Alert | Admin_Wipe,
	Name_Blue = SuperAdmin
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
	TouchScreen = 1 << 2,
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
#define KGRY  "\033[37m"
#define KRESET "\033[0m"


constexpr char* COMPILER_NAME[] = {
	"clang++","g++","MSVC","UNKNOWN"
};
#define IS_CLANG   0
#define IS_GPP     1
#define IS_MSVC    2
#define IS_UNKNOWN 3



#if defined(__clang__)
#define COMPILER_VERSION IS_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_VERSION IS_G++
#elif defined(_MSC_VER)
#define COMPILER_VERSION IS_MSVC
#else
#define COMPILER_VERSION IS_UNKNOWN
#endif

#define LOGO\
  COL2  "                    _\n"\
		"                   (_)\n"\
		COL1" _ __ " COL2" _   _ " COL1" _ __  " COL2"_\n"\
		COL1"| '__|" COL2"| | | |" COL1"| '__|" COL2"| |\n"\
		COL1"| |   " COL2"| |_| |" COL1"| |   " COL2"| |\n"\
		COL1"|_|   " COL2" \\__,_|" COL1"|_|   " COL2"|_|   " KRESET

#define TAG "\n   > Compiled with " KMAG "%s" KRESET" on " __DATE__" " __TIME__".\n\n"

#define MIRROR_IP "34.94.215.186"
#define MIRROR_PORT 80

#define MAX_USER_COUNT 256
#define MAX_USERNAME_LENGTH 19
#define MAX_MULTI_COUNT 64

#define CHO_VERSION 19
#define _M(a) std::move(a)
#define RURIPORT 420
#define ARIAPORT 421

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

const int PING_TIMEOUT_OSU = 120000;//yes thanks peppy
const int FREE_SLOT_TIME = 1800000;

namespace Packet{
	enum class Server : unsigned short{
		userID = 5,
		commandError = 6,
		sendMessage = 7,
		ping = 8,
		handleIRCUsernameChange = 9,
		handleIRCQuit = 10,
		userStats = 11,
		userLogout = 12,
		spectatorJoined = 13,
		spectatorLeft = 14,
		spectateFrames = 15,
		versionUpdate = 19,
		spectatorCantSpectate = 22,
		getAttention = 23,
		notification = 24,
		updateMatch = 26,
		newMatch = 27,
		disposeMatch = 28,
		matchJoinSuccess = 36,
		matchJoinFail = 37,
		fellowSpectatorJoined = 42,
		fellowSpectatorLeft = 43,
		AllPlayersLoaded_unused = 45,
		matchStart = 46,
		matchScoreUpdate = 48,
		matchTransferHost = 50,
		matchAllPlayersLoaded = 53,
		matchPlayerFailed = 57,
		matchComplete = 58,
		matchSkip = 61,
		unauthorised = 62,
		channelJoinSuccess = 64,
		channelInfo = 65,
		channelKicked = 66,
		channelAvailableAutojoin = 67,
		beatmapInfoReply = 69,
		supporterGMT = 71,
		friendsList = 72,
		protocolVersion = 75,
		mainMenuIcon = 76,
		ratTheirPC = 80,
		matchPlayerSkipped = 81,
		userPanel = 83,
		IRC_only = 84,
		restart = 86,
		invite = 88,
		channelInfoEnd = 89,
		matchChangePassword = 91,
		silenceEnd = 92,
		userSilenced = 94,
		userPresenceSingle = 95,
		userPresenceBundle = 96,
		userPMBlocked = 100,
		targetIsSilenced = 101,
		versionUpdateForced = 102,
		switchServer = 103,
		accountRestricted = 104,
		RTX = 105,
		switchTourneyServer = 107
	};
	namespace Client{
		enum : unsigned short {
			changeAction = 0,
			sendPublicMessage = 1,
			logout = 2,
			requestStatusUpdate = 3,
			pong = 4,
			startSpectating = 16,
			stopSpectating = 17,
			spectateFrames = 18,
			errorReport = 20,
			cantSpectate = 21,
			sendPrivateMessage = 25,
			partLobby = 29,
			joinLobby = 30,
			createMatch = 31,
			joinMatch = 32,
			partMatch = 33,
			matchChangeSlot = 38,
			matchReady = 39,
			matchLock = 40,
			matchChangeSettings = 41,
			matchStart = 44,
			AllPlayersLoaded_unused = 45,
			matchScoreUpdate = 47,
			matchComplete = 49,
			matchChangeMods = 51,
			matchLoadComplete = 52,
			matchNoBeatmap = 54,
			matchNotReady = 55,
			matchFailed = 56,
			matchHasBeatmap = 59,
			matchSkipRequest = 60,
			channelJoin = 63,
			beatmapInfoRequest = 68,
			matchTransferHost = 70,
			friendAdd = 73,
			friendRemove = 74,
			matchChangeTeam = 77,
			channelPart = 78,
			receiveUpdates = 79,
			setAwayMessage = 82,
			IRC_only = 84,
			userStatsRequest = 85,
			matchInvite = 87,
			matchChangePassword = 90,
			specialMatchInfoRequest = 93,
			userPresenceRequest = 97,
			userPresenceRequestAll = 98,
			userToggleBlockNonFriendPM = 99,
			matchAbort = 106,
			specialJoinMatchChannel = 108,
			specialLeaveMatchChannel = 109
		};

	}

}

enum LoginReply {
	Login_Failed = -1
};

#ifndef LINUX
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

#define clock_ms clock

#define likely(x) x 
#define unlikely(x) x

#define _inline __forceinline

#define MM_ALIGN __declspec(align(16))

#else

#include "Linux.h"

#define likely(x)      __builtin_expect(!!(x), 1) 
#define unlikely(x)    __builtin_expect(!!(x), 0) 
#define _inline __attribute__((always_inline))

#define MM_ALIGN __attribute__((aligned(16)))

#endif

constexpr size_t MAX_PACKET_LENGTH = 2816;

#define RURI_UNIX_SOCKET "/tmp/ruri.sock"
#define ARIA_UNIX_SOCKET "/tmp/aria.sock"
#define API_UNIX_SOCKET "/tmp/ruri_api.sock"

#define al_min(a, b) ((a) < (b) ? (a) : (b))
#define al_max(a, b) ((a) > (b) ? (a) : (b))
template<typename T>
_inline T al_clamp(const T v, const T minv, const T maxv) {
	if (v < minv)return minv;
	if (v > maxv)return maxv;
	return v;
}

template<typename... T>
struct r_struct {

	template<size_t N, typename TT, typename... types>struct get_Nth_type{using type = typename get_Nth_type<N - 1, types...>::type;};
	template<typename TT, typename... types>struct get_Nth_type<0, TT, types...>{using type = TT;};

	u8* Data;
	r_struct(void* P) noexcept : Data((u8*)P) {}
	~r_struct() { Data = 0; }

	template<size_t i>
	[[nodiscard]] constexpr auto& get() const noexcept{

		using T2 = typename get_Nth_type<i, T...>::type;

		constexpr size_t Offset = []{size_t c = 0; return (((c++ < i) ? sizeof(T) : 0) + ...);}();

		return (T2&)Data[Offset];
	}
};

#include <thread>

typedef unsigned char byte;

WSADATA wsData;

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <nmmintrin.h>
#include <condition_variable>
#include <atomic>
#include <cmath>
#include <time.h>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <type_traits>

#ifndef NO_RELAX
constexpr bool RELAX_MODE = 1;
#define GM_MAX 7
#else
const bool RELAX_MODE = 0;
#define GM_MAX 3
#endif

const std::string GameModeNames[] = {
	"osu!","osu!taiko","osu!catch","osu!mania"
	"osu!relax","osu!taiko(relax)","osu!catch(relax)","osu!mania(relax)"
};
const std::string Stats_Table_Name[] = {
	"users_stats",
	"rx_stats"
};
const std::string Score_Table_Name[] = {
	"scores",
	"scores_relax"
};
const std::string PPColNames[] = { "pp_std","pp_taiko","pp_ctb","pp_mania" };

const std::string Acc_Col_Name[] = { "avg_accuracy_std","avg_accuracy_taiko","avg_accuracy_ctb","avg_accuracy_mania" };
const std::string PP_Col_Name[] = { "pp_std","pp_taiko","pp_ctb","pp_mania" };

constexpr size_t _strlen_(const char* s)noexcept{
	size_t V = 0;
	while (s[V++]);
	return V--;
}
template<typename T, size_t size>
	constexpr size_t aSize(const T(&)[size]) noexcept{ return size; }

#define ARRAY_SIZE(s) (sizeof(s) / sizeof(s[0]))
#define ZeroArray(s) memset(s,0,sizeof(s))

#define SafeRead(Var)if(Current + sizeof(Var) > End)return;Var = *(decltype(Var)*)Current;Current += sizeof(Var)

template<const size_t nSize>
	_inline const auto STACK(const char(&String)[nSize]) {

		constexpr size_t Size = nSize - 1;

		static_assert((nSize != 0 && nSize != 1), "STACK() can not be called with an empty string");

		std::array<char, Size> a;

		memcpy(a.data(), String, nSize - 1);
		/*
		for (size_t i = 0; (i + 4) <= Size; i+=4)
			*(DWORD*)&a[i] = *(DWORD*)&String[i];

		if constexpr (constexpr size_t Rem = Size % 4; Rem)
			for (size_t i = Size - Rem; i < Size; i++)
				a[i] = String[i];*/
		return a;
	}

template<typename T>
	_inline T MemToNum(const void* P, const size_t Size) {
		if (auto C = (const char*)P; P && Size) {
			T Return = T();
			if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) {

				int i = 0;

				for (; i < Size; i++) {
					const byte c = C[i] - '0';
					if (c == 254) {//.
						i++;
						break;
					}
					if (c <= 9)
						Return = (Return * T(10)) + (c);
				}
				if (i < Size) {

					T e = T();
					int count = 0;

					for (; i < Size; i++) {
						const byte c = C[i] - '0';
						if (c <= 9) {
							count++;
							e = (e * T(10)) + (c);
						}
					}
					Return += e / std::pow(T(10), count);
				}
				return (Size && *(char*)P == '-') ? -Return : Return;
			}

			for (size_t i = 0; i < Size; i++) {
				const byte c = C[i] - '0';
				if (c <= 9)
					Return = (Return * 10) + c;
			}
			return (Size && *(char*)P == '-') ? -Return : Return;
		}
		return T();
	}

#include <charconv>

#define StringToNum(TYPE, STRING) [](const std::string_view S){return MemToNum<TYPE>(S.data(), S.size());}(STRING)
#define DeleteAndNull(s)if(s)delete s;s=0

template<typename T, typename T2>
	_inline T STN(const T2& String){

	#if COMPILER_VERSION != IS_MSVC
		return [](const std::string_view S) {return MemToNum<T>(S.data(), S.size()); }(std::string_view((const char*)String.data(), String.size()));
	#else
		T v;
		return std::from_chars(String.data(), String.data() + String.size(), v).ptr != String.data() ? v : T();
	#endif
	}


#define mlock(s) if constexpr(std::scoped_lock L(s);1 == 1)
#define s_mlock(s) if constexpr(std::shared_lock L(s);1)

template<typename T, const size_t Size>
	_inline bool MEM_CMP_START(const T& VECT, const char (&STR)[Size]){

		if (const size_t InputSize = VECT.size();
			InputSize >= Size - 1) {

			for (DWORD i = 0; i < Size - 1; i++)
				if (static_cast<char>(VECT[i]) != STR[i])
					return 0;

			return 1;
		}

		return 0;
	}

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

#define LOAD_FILE(FILENAME)\
	[&](){\
		std::ifstream file(std::string(FILENAME), std::ios::binary | std::ios::ate);\
		if (!file.is_open())\
			return std::vector<byte>();\
\
		const size_t pos = file.tellg();\
		if(!pos)return std::vector<byte>();\
		std::vector<byte> rVec(pos + 1);\
\
		file.seekg(0, std::ios::beg);\
		file.read((char*)&rVec[0], pos);\
		file.close();\
		rVec[rVec.size()-1] = 0;\
\
		return rVec;\
	}()

#define VEC(s) std::vector<s>
#define PAIR(a,b) std::pair<a,b>
#define IT_COPY(a) begin(a),end(a)

const std::string BOT_NAME = M_BOT_NAME;
const std::string FAKEUSER_NAME = []{const byte a[] = { 226,128,140,226,128,141,0 }; return std::string((char*)a); }();

template<typename T, typename T2>
size_t GetIndex(const T& Start, const T2& End) {
	return (size_t(&End) - size_t(&Start[0])) / sizeof(Start[0]);
}

const auto Space_To_Underscore = [](char& c) {if (c == ' ')c = '_';};

const auto Convert_Username_Safe = [](char& c){
	if (c == ' ') c = '_';
	else if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
};


template<typename T, T V> struct CONSTX { constexpr static T value = V;};


template<size_t Size_T>
struct c_string_view {
	char* data;
	static constexpr size_t Size = Size_T;
};

template<const size_t nSize>
constexpr auto _VIEW(const char(&String)[nSize]) {
	return c_string_view<nSize - 1>{(char*)String};
}

template<typename T>
struct T_to_string_max {

	static constexpr size_t value = [] {

		if constexpr (std::is_same<typename std::decay<T>::type, double>::value)
			return 24;
		if constexpr (std::is_same<typename std::decay<T>::type, float>::value)
			return 16;

		switch (sizeof(std::decay<T>::type)) {
			case 1: return 4;
			case 2: return 6;
			case 4: return 11;
			case 8: return 20;
		};

		return 0;
	}();

};

template<typename>
struct is_c_string_view : std::false_type {};

template<size_t Size>
struct is_c_string_view<c_string_view<Size>> : std::true_type {};

template<typename ...T>
auto PRINTF(T... Params) {

	constexpr size_t Size = (([]()->size_t {
		if constexpr (is_c_string_view<T>::value) return T::Size;
		return T_to_string_max<T>::value;
		}()) + ...);

	std::array<char, Size> Ret;

	char* Current = (char*)Ret.data(), * End = Current + Size;

	(([&Current, &Params, End] {
		if constexpr (is_c_string_view<T>::value) {
			memcpy((void*)Current, Params.data, T::Size);
			Current += T::Size;
		}
		else Current = std::to_chars(Current, End, Params).ptr;

		}()), ...);

	return std::pair{ (size_t)Current - (size_t)Ret.data(), Ret };
}


#include "SQL.h"
#include "Json.h"

_SQLCon SQL_BanchoThread[BANCHO_THREAD_COUNT];

#include "BCrypt/BCrypt.hpp"

std::string GET_WEB(const std::string& HostName, const std::string& Page) {

	SOCKET Socket = 0;
	addrinfo* result(0), * rp(0);

	std::string Ret;

	for (;;) {

		if (Socket)closesocket(Socket);

		Socket = 0;

		addrinfo ainfo;
		ZeroMemory(&ainfo, sizeof(ainfo));

		ainfo.ai_family = AF_UNSPEC;
		ainfo.ai_socktype = SOCK_STREAM;

		if (getaddrinfo(HostName.c_str(), "80", &ainfo, &result) != 0)
			break;

		for (rp = result; rp; rp = rp->ai_next) {
			if ((Socket = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol)) == SOCKET_ERROR)continue;

			if (connect(Socket, rp->ai_addr, rp->ai_addrlen) == SOCKET_ERROR){
				closesocket(Socket);
				Socket = 0;
				continue;
			}

			break;
		}

		if (rp == NULL)
			break;


		const std::string pData = "GET /" + Page + " HTTP/1.1\r\nHost: " + HostName + "\r\nConnection: close\r\n\r\n";

		if (send(Socket, pData.data(), pData.size(), 0) == SOCKET_ERROR)
			break;

		size_t Total = 0;
		for (;;) {

			constexpr int BufferSize = 1024;

			Ret.resize(Total + BufferSize);

			const int Size = recv(Socket, &Ret[Total], BufferSize, 0);

			if (Size == -1) {
				Ret.clear();
				break;
			}
			if (Size == 0)break;
			Ret.resize((Total += Size));
		}

		break;
	}

	if (result)freeaddrinfo(result);
	if (Socket)closesocket(Socket);

	return Ret;
}

int CharHexToDecimal(const char c){
	return c >= '0' && c <= '9'
		? c - '0'
		: c >= 'A' && c <= 'F'
		? c - ('A' - 10)
		: c >= 'a' && c <= 'f'
		? c - ('a' - 10)
		: 0;
}

_inline int DecimalToHex(const char c){
	return c <= 0x9 ? '0' + c : c <= 0xF ? ('a' - 10) + c : ' ';
}

std::string GET_WEB_CHUNKED(const std::string& HostName, const std::string& Page) {

	const auto h2d = [](const char c)->int {
		return c >= '0' && c <= '9'
			? c - '0'
			: c >= 'A' && c <= 'F'
			? c - ('A' - 10)
			: c >= 'a' && c <= 'f'
			? c - ('a' - 10)
			: 0;
	};

	const std::string& rp = GET_WEB(HostName, Page);

	if (rp.size() == 0)
		return "";

	std::string p; p.reserve(rp.size());
	size_t Start = rp.find("\r\n\r\n");

	if (Start == std::string::npos)
		return "";


	const size_t End = rp.size();

	for (size_t i = Start + 4; i < End; i++) {
		Start = i;
		while (i < End && *(USHORT*)&rp[i] != 0x0a0d)i++;//\r\n
		if (i == End)break;

		size_t ChunkSize = 0;

		for (DWORD z = Start; z < i; z++) {
			ChunkSize <<= 4;
			ChunkSize += h2d(rp[z]);
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

struct _Timer{

	const std::chrono::steady_clock::time_point sTime;
	
	_Timer(): sTime(std::chrono::steady_clock::now()){};

	~_Timer() {

		printf("%fms\n",double(double(u64(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - sTime).count())) / 1000000.0));

	}
};

std::vector<std::string> UsernameCache;

std::shared_mutex UsernameCacheLock;

void UpdateUsernameCache(_SQLCon *SQL){

	u32 Count = 0;
	
	mlock (UsernameCacheLock){

		std::unique_ptr<sql::ResultSet> res{SQL->ExecuteQuery("SELECT id,username FROM users WHERE id >= " + std::to_string(UsernameCache.size() + USERID_START) + " ORDER BY id DESC")};

		if (res && res->next()){
			Count = 1;

			const u32 HighestID = res->getUInt(1);
			if (HighestID >= USERID_START) {

				UsernameCache.resize(HighestID - (USERID_START - 1));
				UsernameCache[HighestID - USERID_START] = res->getString(2);

				while (res->next()){
					//if (ID < USERID_START)
						//break;
					Count++;
					UsernameCache[res->getUInt(1) - USERID_START] = res->getString(2);
				}
			}
		}

	}

	printf("Updated %i usernames in the cache\n", Count);
}

std::string GetUsernameFromCache(const DWORD UID){
	
	const int ID = int(UID) - USERID_START;

	if (ID >= 0)
		if (std::shared_lock L(UsernameCacheLock);  ID < UsernameCache.size())
			return UsernameCache[ID];
	
	return "";
}
void UsernameCacheUpdateName(const DWORD UID, const std::string &s, _SQLCon *SQL){

	if (UID >= USERID_START){

		UsernameCacheLock.lock_shared();

		if (UID - USERID_START >= UsernameCache.size()) {
			UsernameCacheLock.unlock_shared();
			return UpdateUsernameCache(SQL);
		}
		UsernameCache[UID - USERID_START] = s;

		UsernameCacheLock.unlock_shared();
	}
}

std::string BeatmapPath;
std::string ReplayPath;
std::string osu_API_Key;
std::string osu_API_BEATMAP;

struct _RankList {
	u32 ID;
	u32 PP;
	_RankList() {
		ID = 0;
		PP = 0;
	}
	_RankList(u32 I, u32 P){
		ID = I;
		PP = P;
	}
	bool operator ==(const _RankList A)const noexcept{return ID == A.ID;}

};
struct _rList{
	DWORD Version = 1;
	VEC(_RankList) List;
	std::shared_mutex UpdateLock;
	_inline void lock() { UpdateLock.lock(); }
	_inline void unlock() { UpdateLock.unlock(); }
	_inline void lock_shared() { UpdateLock.lock_shared(); }
	_inline void unlock_shared() { UpdateLock.unlock_shared(); }


}; _rList RankList[GM_MAX + 1];

void ReSortAllRank(){
	for (auto& rList : RankList){
		rList.lock();

		rList.Version++;
		std::sort(rList.List.begin(), rList.List.end(),
			[](const _RankList& A, const _RankList& B){
				return A.PP > B.PP;
			});

		rList.unlock();
	}
}
void ReSortRank(const DWORD i) {

	if (i > GM_MAX)
		return;

	RankList[i].lock();

	RankList[i].Version++;
	std::sort(RankList[i].List.begin(), RankList[i].List.end(),
		[](const _RankList& A, const _RankList &B){
		return A.PP > B.PP;
	});

	RankList[i].unlock();

}
struct _SQLQue{
	std::mutex CommandLock;
	std::vector<std::string> Commands;


	void AddQue(const std::string &s){
		std::scoped_lock<std::mutex> L(CommandLock);
		Commands.push_back(s);
	}
	void AddQue(const std::string&& s) {
		std::scoped_lock<std::mutex> L(CommandLock);
		Commands.push_back(_M(s));
	}

}; _SQLQue SQLExecQue;


DWORD GetRank(const DWORD UserID, const DWORD GameMode){//Could use the cached pp to tell around where its supposed to be?

	if (UserID < USERID_START || GameMode > GM_MAX || RankList[GameMode].List.size() == 0)
		return 0;

	std::shared_lock L(RankList[GameMode]);

	const auto& List = RankList[GameMode].List;

	return (std::find(begin(List), end(List), _RankList(UserID, 0)) - List.begin()) + 1;
}

void UpdateRank(const DWORD UserID, const DWORD GameMode, const DWORD PP){

	if (!PP || UserID < USERID_START || GameMode > GM_MAX)
		return;
	{

		std::scoped_lock<std::shared_mutex> L(RankList[GameMode].UpdateLock);

		const _RankList N = { UserID,PP };

		auto& List = RankList[GameMode].List;

		auto PreviousRank = std::find(begin(List), end(List), N);

		if (PreviousRank != end(List) && List.size()){

			const DWORD OldPP = PreviousRank->PP;
			PreviousRank->PP = N.PP;
			const size_t Origin = PreviousRank - List.begin();		

			if (size_t NewPosition = 0; OldPP < N.PP){
				for (size_t i = Origin; i-- > 0;)
					if (List[i].PP > N.PP) {
						NewPosition = i + 1;
						break;
					}

				if (NewPosition != Origin) {
					memcpy(&List[NewPosition + 1], &List[NewPosition], (Origin - NewPosition) * sizeof(_RankList));
					List[NewPosition] = N;
					RankList[GameMode].Version++;
				}

			}
			else if (OldPP > N.PP){

				NewPosition = List.size() - 1;

				if (PP) {
					for (size_t i = Origin + 1; i < List.size(); i++)
						if (List[i].PP < N.PP) {
							NewPosition = i - 1;
							break;
						}
				}
				else NewPosition = List.size() - 1;

				if (NewPosition != Origin) {
					memcpy(&List[Origin], &List[Origin + 1], (NewPosition - Origin) * sizeof(_RankList));
					List[NewPosition] = N;
					RankList[GameMode].Version++;
				}

				for (size_t i = List.size(); i-- > 0;) {
					if (List[i].PP)break;
					List.pop_back();
				}
			}
		}
		else {
			List.push_back(N);
			std::sort(begin(List), end(List),
				[](const _RankList& i, const _RankList& j) {
					return (i.PP > j.PP);
				});
			RankList[GameMode].Version++;
		}
	}

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

	void random_bytes(void* dest, const size_t Size){

		const auto Ran = []{return (size_t)std::uniform_int_distribution<size_t>{ 0, size_t(-1)}(mersenneTwister);};

		for (size_t i(0),b(Ran()); i < Size; i++)
			*(u8*)((size_t)dest + i) = (likely(b >>= 8) ? b : b = Ran()) & 0xff;
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

void AddUleb(VEC(byte) &v, size_t s){

	if (s){

		u64 ret = 0x0b02ull + ((s & 0x7f) << 16);

		while(unlikely(s = (s >> 7)))
			ret += (u64(0x80) << ((ret++ & 0xff) << 3)) + (u64(s & 0x7f) << ((ret & 0xff) << 3));

		v.resize(v.size() + 8);
		*(u64*)(&v[v.size() - 8]) = ret >> 8;
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
		if (GameMode > GM_MAX)
			return 0;

		const DWORD cVersion = RankList[GameMode].Version;

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

template<typename T>
const VEC(std::string_view) Explode_View(const T& Input, const char Delim, const DWORD ExpectedSize) {

	VEC(std::string_view) Views;

	if (Input.size()) {

		Views.reserve(ExpectedSize);

		size_t Start = size_t(&Input[0]);

		for (auto& c : Input){
			if (static_cast<char>(c) == Delim) {
				Views.emplace_back((const char*)Start, size_t(&c) - size_t(Start));
				Start = size_t(&c) + 1;
			}
		}

		if (size_t End = size_t(&Input[0]) + Input.size(); Start != End)
			Views.emplace_back((const char*)Start, End - Start);
	}
	return Views;
}

template<typename T, size_t Size>
const VEC(std::string_view) Explode_View_Multi(const T& Input, const char (&Delim)[Size], const DWORD ExpectedSize) {

	VEC(std::string_view) Views;

	if (Input.size() && Size) {
		Views.reserve(ExpectedSize);

		if (Input.size() > (Size - 1)) {

			size_t Start = 0;

			for (size_t i = 0; i < Input.size() - (Size-1); i++) {

				for (size_t z = 0; z < (Size-1); z++) {

					if (Input[i + z] != Delim[z])
						break;

					if (z != (Size-1) - 1)
						continue;

					Views.emplace_back((const char*)& Input[Start], i - Start);
					i += z;
					Start = i + 1;
				}

			}

			if (Start != Input.size())
				Views.emplace_back((const char*)& Input[Start], Input.size() - Start);

		}
		else Views.emplace_back((const char*)& Input[0], Input.size());

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

std::vector<std::pair<std::string, std::string>> Empty_Headers;

const std::vector<byte> Empty_Byte;

_UserStats RecalculatingStats;

bool UpdateUserStatsFromDB(_SQLCon *SQL,const DWORD UserID, const DWORD GameMode, _UserStats &stats){

	if (GameMode > GM_MAX)
		return 0;

	const DWORD MMode = GameMode % 4;

	DWORD Count = 0;
	double TotalAcc = 0.;
	double TotalPP = 0.;

	if (auto res = SQL->ExecuteQuery("SELECT accuracy,pp FROM " + Score_Table_Name[GameMode >= 4] + " WHERE userid=" + std::to_string(UserID) + "&&play_mode=" + std::to_string(MMode) + "&&completed=3&&pp>0 ORDER BY pp DESC LIMIT 100")
			;res){

		while(res->next()){
			if(Count < 50)
				TotalAcc += res->getDouble(1);
			TotalPP += res->getDouble(2) * std::pow(0.95, double(Count));
			Count++;
		}

		delete res;
	}

	float acc = 0.f;
	const int pp = (int)round(TotalPP);

	if (Count)
		acc = double(TotalAcc / double(Count > 50 ? 50. : Count)) / 100.;

	if (acc != stats.Acc || pp != stats.pp || &stats == &RecalculatingStats){

		if (pp != stats.pp && &stats != &RecalculatingStats)
			UpdateRank(UserID, GameMode, pp);

		stats.pp = pp;
		stats.Acc = acc;

		SQLExecQue.AddQue("UPDATE " + Stats_Table_Name[GameMode >= 4] + " SET " + Acc_Col_Name[MMode] + "=" + std::to_string(acc * 100.) + "," + PP_Col_Name[MMode] + " = " + std::to_string(TotalPP)
						+ " WHERE id = " + std::to_string(UserID) + " LIMIT 1");
	}

	return 1;
}


namespace PacketBuilder {

	namespace CT {
		template<size_t Size>
			constexpr auto _GetUleb_();
	}

	constexpr size_t PreAlloc(const char v) { return (v == 's' || v == 'v') ? 64 : v == '5' ? 32 : v == 'l' ? 8 : v == '!' ? 7 : v == 'i' ? 4 : v == 'w' ? 2 : 1; }

	template<Packet::Server ID,char... Format,typename... T>
		void Build(std::vector<byte>& c, T... A){

			constexpr size_t Reserve = []{
				size_t R = 7;
				((R = R + (Format == 'a' ? sizeof(T) + 6 : Format == '-' ? (sizeof(CT::_GetUleb_<sizeof(T)>()) + sizeof(T)) : PreAlloc(Format))) | ...);
				return R;
			}();

			(([&] {
				if constexpr (Format == 'm')
					A->lock();
				return 0; }()) | ...);

			size_t StartID = c.size();		

			c.reserve(StartID + Reserve);
			c.resize(StartID + 7);
			*(USHORT*)&c[StartID] = (USHORT)ID;
			
			(([&]{
				if constexpr (Format == 's'){
					const std::string* v = A;
					const size_t Size = v->size();
					AddUleb(c, Size);
					if (Size) {
						c.resize(c.size() + Size);
						memcpy(&c[c.size() - Size], v->data(), Size);
					}
					return 0;
				}
				if constexpr (Format == 'v'){
					const std::string_view* v = A;
					const size_t Size = v->size();
					AddUleb(c, Size);
					if (Size) {
						c.resize(c.size() + Size);
						memcpy(&c[c.size() - Size], v->data(), Size);
					}
					return 0;
				}
				if constexpr (Format == '5'){

					if (A[0]){//Assume it is at least 32 bytes long.
						const size_t cI = c.size();
						c.resize(c.size() + 34);
						*(USHORT*)&c[cI] = 0x200b;
						*(uint64_t*)&c[cI + 2]  = *(uint64_t*)(A);
						*(uint64_t*)&c[cI + 10] = *(uint64_t*)(A + 8);
						*(uint64_t*)&c[cI + 18] = *(uint64_t*)(A + 16);
						*(uint64_t*)&c[cI + 26] = *(uint64_t*)(A + 24);
					}else c.push_back(0);

					return 0;
				}
				if constexpr (Format == '!') {
					*(DWORD*)&c[StartID + 3] = c.size() - (StartID + 7);
					StartID = c.size();
					c.resize(StartID + 7);
					*(USHORT*)&c[StartID] = (USHORT)A;
				}
				if constexpr (Format == '-') {

					constexpr auto ULEB = CT::_GetUleb_<sizeof(T)>();

					const size_t Off = c.size();

					c.resize(c.size() + sizeof(ULEB) + sizeof(T));

					*(std::array<byte, sizeof(ULEB)>*)&c[Off] = ULEB;
					*(T*)&c[Off + sizeof(ULEB)] = A;

					return 0;
				}
				if constexpr (Format == 'l') {
					c.resize(c.size() + 8);
					*(long long*)&c[c.size() - 8] = A;
					return 0;
				}
				if constexpr (Format == 'i'){
					c.resize(c.size() + 4);
					*(int*)&c[c.size() - 4] = A;
					return 0;
				}
				if constexpr (Format == 'b') {
					c.push_back(byte(A));
					return 0;
				}
				if constexpr (Format == 'w'){
					c.resize(c.size() + 2);
					*(short*)&c[c.size() - 2] = A;
					return 0;
				}
				if constexpr (Format == 'a') {

					size_t Size = sizeof(T);

					for (size_t i = 0; i < sizeof(T); i++) {
						if (A[i] == 0) {
							Size = i;
							break;
						}
					}

					AddUleb(c, Size);
					const size_t s = c.size();
					c.resize(s + Size);
					memcpy(&c[s], A.data(), Size);

					return 0;
				}

				return 0; }()
				) | ...);

			*(DWORD*)&c[StartID + 3] = c.size() - (StartID + 7);

			(([&] {
				if constexpr (Format == 'm')
					A->unlock();
				return 0; }()) | ...);

		}

	template<Packet::Server ID, char... Format, typename... T>
		auto Fixed_Build(T... A){

			constexpr size_t Size = [] {
				size_t R = 7;
				((R = R + ((Format == '-') ? (sizeof(CT::_GetUleb_<sizeof(T)>()) + sizeof(T)) : PreAlloc(Format))) | ...);
				return R;
			}();

			std::array<byte, Size> Ret;

			size_t Start = 7;

			*(USHORT*)&Ret[0] = USHORT(ID);Ret[2] = 0;

			size_t Off = 7;
			
			(([&]{
				if constexpr (Format == 'i'){
					*(int*)&Ret[Off] = A;
					Off += 4;
					return 0;
				}
				if constexpr (Format == 'l') {
					*(long long*)&Ret[Off] = A;
					Off += 8;
					return 0;
				}
				if constexpr (Format == 'w') {
					*(short*)&Ret[Off] = A;
					Off += 2;
					return 0;
				}
				if constexpr (Format == 'b') {
					Ret[Off++] = A;
					return 0;
				}
				if constexpr (Format == '!'){
					*(DWORD*)&Ret[Start - 4] = Off - Start;
					*(USHORT*)&Ret[Off] = USHORT(A);
					Ret[Off + 2] = 0;
					Off += 7;
					Start = Off;
					return 0;
				}
				if constexpr (Format == '-'){
					constexpr auto ULEB = CT::_GetUleb_<sizeof(T)>();
					*(std::array<byte, sizeof(ULEB)>*)&Ret[Off] = ULEB; Off += sizeof(ULEB);
					*(T*)&Ret[Off] = A; Off += sizeof(T);
					return 0;
				}

				return 0;
			}()) | ...);

			*(DWORD*)&Ret[Start - 4] = Off - Start;

			return Ret;
		}

	namespace CT {

		template<size_t Size>
		constexpr auto _GetUleb_() {

			constexpr byte HeaderSize = [] {
				if (Size == 0)return 1;
				if (Size >> 7 == 0)return 2;
				if (Size >> 14 == 0)return 3;
				if (Size >> 21 == 0)return 4;
				if (Size >> 28 == 0)return 5;
				return 6;
			}();

			if constexpr (HeaderSize == 1)
				return std::array<byte, 1>{0};
			if constexpr (HeaderSize == 2)
				return std::array<byte, 2>{0xb, byte(Size)};
			if constexpr (HeaderSize == 3)
				return std::array<byte, 3>{0xb, byte((Size) | 0x80), byte((Size >> 7))};
			if constexpr (HeaderSize == 4)
				return std::array<byte, 4>{0xb, byte((Size) | 0x80), byte((Size >> 7) | 0x80), byte((Size >> 14))};
			if constexpr (HeaderSize == 5)
				return std::array<byte, 5>{0xb, byte((Size) | 0x80), byte((Size >> 7) | 0x80), byte((Size >> 14) | 0x80), byte((Size >> 21))};
			if constexpr (HeaderSize == 6)
				return std::array<byte, 6>{0xb, byte((Size) | 0x80), byte((Size >> 7) | 0x80), byte((Size >> 14) | 0x80), byte((Size >> 21) | 0x80), byte((Size >> 28))};

			return std::array<byte, HeaderSize>{0};
		}

		template<size_t Size>
		constexpr auto String(const char(&S)[Size]) {

			constexpr auto ULEB = _GetUleb_<Size - 1>();

			std::array<byte, ULEB.size() + Size - 1> Ret = { 0 };

			for (size_t i = 0; i < ULEB.size(); i++)
				Ret[i] = ULEB[i];
			for (size_t i = 0; i < Size - 1; i++)
				Ret[i + ULEB.size()] = S[i];

			return Ret;
		}

		template<typename T>
		constexpr std::array<byte, sizeof(T)> Number(const T Num) {
			std::array<byte, sizeof(T)> Ret{ 0 };
			for (size_t i = 0; i < sizeof(T); i++)
				Ret[i] = byte(Num >> (8 * i));
			return Ret;
		}

		constexpr std::array<byte, 7> PacketHeader(Packet::Server ID) {
			std::array<byte, 7> Ret{ 0 };
			Ret[0] = byte(USHORT(ID));
			Ret[1] = byte(USHORT(ID) >> 8);
			return Ret;
		}
		template<size_t Size>
			constexpr std::array<byte, Size> PopulateHeader(std::array<byte, Size> Packet) {
				Packet[3] = byte((Size - 7));
				Packet[4] = byte((Size - 7) >> 8);
				Packet[5] = byte((Size - 7) >> 16);
				Packet[6] = byte((Size - 7) >> 24);
				return Packet;
			}

		template<size_t DestSize, size_t SrcSize>
			constexpr size_t Write(std::array<byte,DestSize> &Dest, std::array<byte,SrcSize>& Src, size_t Offset){
				for (size_t i = 0; i < SrcSize; i++)
					Dest[Offset + i] = Src[i];
				return SrcSize;
				}

		template<typename... T>
			constexpr auto Concate(T... Arr){

				constexpr size_t TotalBytes = [&]{
					size_t Total = 0; ((Total = Total + sizeof(Arr)) && ...); return Total;	
				}();

				std::array<byte, TotalBytes> Ret = {0};
				size_t Offset = 0;
				((Offset = (Offset + Write(Ret, Arr, Offset))) | ...);

				return Ret;
			}

		template<const size_t nSize>
			constexpr auto _STACK(const char(&String)[nSize]){
				constexpr size_t Size = nSize - 1;
				static_assert((nSize != 0 && nSize != 1), "STACK() can not be called with an empty string");
				std::array<byte, Size> a{};
				for (size_t i = 0; i < Size; i++)
					a[i] = String[i];
				return a;
			}

		template<size_t Size>
			constexpr auto String_Packet(Packet::Server ID, const char(&S)[Size]){
				return PopulateHeader(Concate(PacketHeader(ID), String(S)));
			}
		template<typename T>
			constexpr auto Number_Packet(Packet::Server ID, T Num) {
				return PopulateHeader(Concate(PacketHeader(ID), Number(Num)));
			}

			template<size_t Number>
				constexpr auto IntToArray(){

					constexpr size_t DigitCount = [] {size_t n(Number), c(1); while (n /= 10)c++; return c; }();
					constexpr auto iPow = [](size_t i, size_t e) {size_t r = i; while (--e)r *= i; return r; };

					std::array<byte, DigitCount> Ret{};

					for (size_t i = DigitCount; i-- > 0;)
						Ret[(DigitCount - 1) - i] = '0' + (!i ? Number % 10 : (Number / iPow(10, i)) % 10);

					return Ret;
				}

			template<USHORT Code,size_t Size, size_t Size2>
				constexpr auto HTTPBytes(const std::array<byte, Size> Body,std::array<byte, Size2> Header){

					constexpr auto resCode = [=] {
						if constexpr(Code == 200)return _STACK("HTTP/1.0 200 OK");
						if constexpr(Code == 404)return _STACK("HTTP/1.0 404 Not Found");
						if constexpr(Code == 405)return _STACK("HTTP/1.0 405 Method Not Allowed");
						if constexpr(Code == 408)return _STACK("HTTP/1.0 408 Request Timeout");
					}();
					constexpr auto ContentLength = Concate(_STACK("Connection: close\r\nContent-Length: "), IntToArray<sizeof(Body)>(),_STACK("\r\n\r\n"));

					return Concate(resCode,Header, ContentLength ,Body);
				}

	}

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

	void Zero(){ ZeroArray(MD5); }

	_MD5() { Zero(); }
	_MD5(std::string&& Input) {
		if (unlikely(Input.size() != 32))
			ZeroArray(MD5);
		else for (size_t i = 0; i < 16; i++)
			MD5[i] = CharHexToDecimal(Input[i]) + (CharHexToDecimal(Input[16 + i]) << 4);//It does not really matter if the data is rearranged.
	}
	_MD5(const std::string_view Input) {
		if (unlikely(Input.size() != 32))
			ZeroArray(MD5);
		else for (size_t i = 0; i < 16; i++)
			MD5[i] = CharHexToDecimal(Input[i]) + (CharHexToDecimal(Input[16 + i]) << 4);//It does not really matter if the data is rearranged.
	}

	const bool operator==(const _MD5& x) const noexcept{
		if (*(uint64_t*)&x.MD5[0] != *(uint64_t*)& this->MD5[0]
			|| *(uint64_t*)&x.MD5[8] != *(uint64_t*)& this->MD5[8])
			return 0;
		return 1;
	}

	std::string to_string()const {

		std::string ret;

		if (*(uint64_t*)&this->MD5[0] || *(uint64_t*)&this->MD5[8]) {
			ret.resize(32);

			for (size_t i = 0; i < 16; i++) {
				ret[i] = DecimalToHex(MD5[i] & 0xf);
				ret[i + 16] = DecimalToHex((MD5[i] & 0xf0) >> 4);
			}
		}

		return ret;
	}
};

#define WriteStringToArray(Array,String){const size_t sSize = al_min(sizeof(Array)-1,String.size());Array[sSize] = 0;memcpy(Array.data(), String.data(), sSize);}

u32 LogoutOffset = 0;

struct USER_SERVER {};

struct _User{

	uint64_t choToken;
	u32 UserID;
	u32 privileges;
	int silence_end;
	char ActionMD5[32];
	u32 actionMods;
	int BeatmapID;
	u32 LogOffset : 16, timeOffset : 5, SpamLevel : 5, GameMode : 3, SendToken : 1, inLobby : 1, FriendsOnlyChat : 1;

	_MD5 Password;
	std::string Username;
	std::string Username_Safe;
	std::array<byte, 64> ActionText;
	u32 country : 8, actionID : 8;
	float lat;
	float lon;

	std::mutex qLock;
	VEC(byte) QueBytes;

	_UserStats Stats[GM_MAX + 1];//4 normal modes + 4 more relax ones

	int LastPacketTime;
	int LoginTime;

	_User* CurrentlySpectating;

	std::shared_mutex SpecLock;
	VEC(_User*) Spectators;

	u16 CurrentMatchID;
	
	int LastSentBeatmap;

	Achievement Ach;
	
	size_t ActiveChannels[MAX_CHAN_COUNT];//There is no way to resolve the actual size without restructuring xdxdxd
	
	std::string c1Check;

	std::mutex RefLock;
	u32 ref;

	/*MM_ALIGN */u32 Friends[256];
	/*MM_ALIGN */u32 Blocked[32];

	const bool is_banned()const noexcept{
		return (privileges & (u32)Privileges::Visible) == 0;
	}

	bool AddBlock(const DWORD UID){
		if (UID >= USERID_START)
			for (size_t i = 0; i < aSize(Blocked); i++) {
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
				break;
			}
		}
	}

	void RemoveChannel(const size_t C) {
		for (DWORD i = 0; i < MAX_CHAN_COUNT; i++) {
			if (ActiveChannels[i] == C) {
				ActiveChannels[i] = 0;
				break;
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

	std::string ProfileLink()const{return "(" + Username + ")[https://osu.ppy.sh/u/" + std::to_string(UserID) + "]";}

	DWORD GetStatsOffset()const{
		if constexpr(RELAX_MODE)
			return al_min(actionMods & Mods::Relax ? GameMode + 4 : GameMode, GM_MAX);
		else 
			return GameMode;
	}

	void SendToSpecs(const VEC(byte) &b){
		if (Spectators.size()){
			std::shared_lock<std::shared_mutex> L(SpecLock);
			for (_User* const Spec : Spectators)
				if (Spec)
					Spec->addQueVector(b);
		}
	}

	template<size_t Size>
		void SendToSpecs(const std::array<byte, Size> &b) {
			if (Spectators.size()) {
				std::shared_lock<std::shared_mutex> L(SpecLock);
				for (_User* const Spec : Spectators)
					if (Spec)
						Spec->addQueArray(b);
			}
		}

	bool isFriend(const DWORD ID) const{
		if (UserID == ID)
			return 1;//Always friends with your self :)

		for (DWORD i = 0; i < aSize(Friends); i++)
			if (Friends[i] == ID)
				return 1;

		return 0;
	}

	bool isBlocked(const DWORD ID) const {
		if (UserID < USERID_START || ID == UserID)
			return 0;

		for (DWORD i = 0; i < aSize(Blocked); i++)
			if (Blocked[i] == ID)
				return !isFriend(ID);

		return 0;
	}

	bool isSilenced()const{
		return silence_end ? (time(0) <= silence_end) : 0;
	}
void reset() {
		DisconnectUser((size_t)this);
		choToken = 0;
		UserID = 0;
		SendToken = 0;
		privileges = 0;
		Username.clear();
		Username_Safe.clear();
		Password.Zero();
		ZeroArray(ActionMD5);		
		ZeroMemory(ActionText.data(), ActionText.size());
		actionMods = 0;
		BeatmapID = 0;
		timeOffset = 0;
		country = 0;
		GameMode = 0;
		lat = 0.f;
		lon = 0.f;
		
		LoginTime = 0;
		CurrentlySpectating = 0;
		CurrentMatchID = 0;
		inLobby = 0;
		Spectators.clear();
		QueBytes.clear();
		SpamLevel = 0;
		LastSentBeatmap = 0;
		FriendsOnlyChat = 0;
		ZeroArray(Friends);
		ZeroArray(Blocked);
		c1Check.clear();
		silence_end = 0;
		LogOffset = LogoutOffset;
	}
	_User(){
		reset();
		Spectators.reserve(16);
		QueBytes.reserve(2048);
		ref = 0;
		LastPacketTime = INT_MIN;
	}
	_User(USER_SERVER){

		//Username = "SERVER";
		Username_Safe = "SERVER";

		choToken = 0;
		UserID = 0;
		privileges = -1;
	}

	template<const bool Locked = true>
		void addQueVector(const VEC(byte) &b){
			if (choToken && b.size()){

				if constexpr (Locked)
					qLock.lock();

				QueBytes.resize(QueBytes.size() + b.size());
				memcpy(&QueBytes[QueBytes.size() - b.size()], b.data(), b.size());

				if constexpr (Locked)
					qLock.unlock();
			}
		}

	template<const bool Locked = true, size_t Size>
		_inline void addQueArray(const std::array<byte, Size>& Array) {

			if constexpr (Locked)qLock.lock();
			if (choToken) {
				QueBytes.resize(Size + QueBytes.size());
				*(std::array<byte, Size>*)&QueBytes[QueBytes.size() - Size] = Array;
			}
			if constexpr (Locked)qLock.unlock();
		}

	void doQue(_Con s){

		if (QueBytes.size()){

			mlock (qLock){

				if (unlikely(SendToken)) {
					SendToken = 0;
					const std::string HTTPHeader = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: " + std::to_string(QueBytes.size()) + "\r\ncho-token: " + std::to_string(choToken) + "\r\n\r\n";
					::send(s.s, HTTPHeader.data(), HTTPHeader.size(), 0);
				}else{
					const std::string HTTPHeader = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: " + std::to_string(QueBytes.size()) + "\r\n\r\n";
					::send(s.s, HTTPHeader.data(), HTTPHeader.size(), 0);
				}

				::send(s.s, (const char*)QueBytes.data(), QueBytes.size(), 0);

				QueBytes.clear();
			}

		}else{
			constexpr auto Empty = PacketBuilder::CT::_STACK("HTTP/1.0 200 OK\r\nContent-Length: 0\r\nConnection: close\r\n\r\n");

			s.SendData(Empty);
		}
	}

};

template<typename T, size_t Size>
	struct locked_array{

		std::array<T, Size> Data;//clang hates this as an inheritance :(

		std::shared_mutex Lock;

		void replace(T From, T To){

			if (std::find(begin(Data), end(Data), From) != end(Data)){

				std::scoped_lock L(Lock);

				for (auto& v : Data)
					if (v == From){
						v = To;
						std::remove(begin(Data), end(Data), T());
						break;
					}
			}
		}

		T& operator [](const size_t Index)noexcept {
			return Data[Index];
		}
	};

template<typename T, size_t Reserve = 16>
	struct locked_vector{

		std::shared_mutex Lock;
		std::vector<T> Vec;

		template<typename ... P>
		void emplace_back(P... Input){
			std::scoped_lock L(Lock);
			Vec.emplace_back(Input ...);
		}

		template<typename F>
		void pop_for_each(F Func) {

			if (!Vec.size())
				return;

			Lock.lock();

			std::vector<T> Temp = std::move(Vec);
			Vec.clear();Vec.reserve(Reserve);

			Lock.unlock();

			for(auto& t : Temp)Func(t);
		}

		T find_copy_default(const T Lambda, const T Default){

			s_mlock(Lock)
				for (const auto& v : Vec)
				if (Lambda(v))
					return v;

			return Default;
		}

		locked_vector() {
			Vec.reserve(Reserve);
		}
	};

std::array<_User, MAX_USER_COUNT> Users;

locked_array<_User*, ARRAY_SIZE(Users)> LobbyUsers = {};
/*
	I decided that updating needlessly to each and every user is useless.
	Updates will only be sent on important information updates.
	Like a player leaving/joining and room metadata updates.
*/
void SendToLobby(const std::vector<byte> &Data){

	for (size_t i = 0; i < ARRAY_SIZE(LobbyUsers.Data); i++) {
		if (_User* u = LobbyUsers[i]; unlikely(u)){
			u->addQueVector(Data);
		}else break;
	}

	/*
	std::for_each_n(begin(LobbyUsers), ARRAY_SIZE(LobbyUsers), [&Data](_User* u) {
			if (unlikely(u)) {
				std::scoped_lock L(u->qLock);
				u->addQueVector(Data);
			}
		});*/
}

void DisconnectUser(_User *u);

void DisconnectUser(size_t Pointer){
	return Pointer ? DisconnectUser((_User*)Pointer) : void();
}

struct _UserRef{

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

	_User* operator->()const noexcept {return User;}

	const inline bool valid()const noexcept {
		return User != 0;
	}
	const bool operator!()const noexcept {
		return !(User);
	}

};

#define UserDoubleCheck(s) if(!(s))continue;std::scoped_lock L(User.RefLock);if(unlikely(!(s)))continue;User.ref++;

_User* GetPlayerSlot_Safe(const std::string &UserName){

	if (unlikely(!UserName.size()))
		return 0;

	for (auto& User : Users){
		UserDoubleCheck(User.Username_Safe == UserName)
		return &User;
	}

	for (auto& User : Users){//User has not logged in before. Opt for a free slot instead

		UserDoubleCheck(User.ref == 0 && User.LastPacketTime == INT_MIN)

		User.reset();

		User.LastPacketTime = 0;

		return &User;
	}

	return 0;
}

_User* GetUserFromID(DWORD ID){

	if (ID >= USERID_START)
		for (auto& User : Users) {
			UserDoubleCheck(User.UserID == ID)
			return &User;
		}

	return 0;
}

_User* GetUserFromToken(const uint64_t Token){

	if(Token)
		for (auto& User : Users){
			UserDoubleCheck(User.choToken == Token)
			return &User;
		}

	return 0;
}

template<typename T>
	_User* GetUserFromName(const T& Name, const bool Force = 0){

		if (Name.size())
			for (auto& User : Users){
				UserDoubleCheck((User.choToken || Force) && Name == User.Username)
				return &User;
			}

		return 0;
	}

_User* GetUserFromNameSafe(const std::string &Name, const bool Force = 0){

	if (Name.size())
		for (auto& User : Users){
			UserDoubleCheck((User.choToken || Force) && Name == User.Username_Safe)
			return &User;
		}

	return 0;
}

#undef UserDoubleCheck

_inline byte GetUserType(const u32 p){

	if (p & (u32)Privileges::Name_Blue)
		return UserType::Peppy;
	if (p & (u32)Privileges::Name_Periwinkle)
		return UserType::Friend;
	if (p & (u32)Privileges::Name_Orange)
		return UserType::BAT;
	if (p & (u32)Privileges::Name_Yellow)
		return UserType::Supporter;

	return UserType::Normal;
}

#include "Channel.h"

namespace PacketBuilder::CT::BotInfo {

	//using namespace PacketBuilder::CT;

	constexpr const auto Bot_Panel = PopulateHeader(Concate(
		PacketHeader(Packet::Server::userPanel),
		Number<int>(USERID_START - 1), String(M_BOT_NAME), Number<byte>(0), Number<byte>(BOT_LOCATION), Number<byte>(UserType::Peppy),
		Number<int>(0), Number<int>(0), Number<int>(0)
	));

	constexpr const auto Bot_Stats = PopulateHeader(
		Concate(
			PacketHeader(Packet::Server::userStats),
			Number<int>(USERID_START - 1), Number<byte>(0), Number<byte>(0), Number<byte>(0), Number<int>(0), Number<byte>(0), Number<int>(0), Number<long long>(0),
			Number<int>(0), Number<int>(0), Number<long long>(0), Number<int>(0), Number<short>(0)
		));
}

void debug_SendOnlineToAll(_User *u){

	if (u->privileges & (u32)Privileges::Visible){

		VEC(byte) b;

		const DWORD Off = u->GetStatsOffset();
		
		PacketBuilder::Build<Packet::Server::userPanel,'i','s','b','b','b','i','i','i',
			'!','i','b','a','5','i','b','i','l','i','i','l','i','w'>
			(b,u->UserID,&u->Username, 24 + u->timeOffset,u->country, GetUserType(u->privileges), *(int*)&u->lon, *(int*)&u->lat, u->Stats[Off].getRank(Off, u->UserID),
			Packet::Server::userStats,u->UserID,u->actionID,u->ActionText,u->ActionMD5,u->actionMods,u->GameMode,u->BeatmapID, u->Stats[Off].rScore, *(int*)&u->Stats[Off].Acc,
				u->Stats[Off].pp > USHORT(-1) ? u->Stats[Off].pp : u->Stats[Off].PlayCount, u->Stats[Off].tScore, u->Stats[Off].getRank(Off, u->UserID), USHORT(u->Stats[Off].pp));
				
		for (auto& User : Users)
			if (User.choToken && &User != u)
				User.addQueVector(b);
	}
}

void Event_client_stopSpectating(_User *u){

	constexpr auto specLeave = PacketBuilder::CT::String_Packet(Packet::Server::channelKicked, "#spectator");

	u->addQueArray(specLeave);

	if (_User* const SpecTarget = u->CurrentlySpectating; SpecTarget){

		u->CurrentlySpectating = 0;

		bool AllEmptySpecs = 1;


		auto b = PacketBuilder::Fixed_Build<Packet::Server::fellowSpectatorLeft, 'i'>(u->UserID);

		s_mlock (SpecTarget->SpecLock){

			for (auto& Spec : SpecTarget->Spectators){
				if (Spec == u)
					Spec = 0;
				else if (Spec){
					Spec->addQueArray(b);
					AllEmptySpecs = 0;
				}
			}

			if (AllEmptySpecs)
				SpecTarget->Spectators.clear();
		}

		*(USHORT*)b.data() = USHORT(Packet::Server::spectatorLeft);

		AllEmptySpecs?
			SpecTarget->addQueArray(PacketBuilder::CT::Concate(b, specLeave))//Pretty gross.
			: SpecTarget->addQueArray(b);
	}
}

void Event_client_cantSpectate(_User *tP){

	if (_User* const SpecHost = tP->CurrentlySpectating; SpecHost){

		const auto b = PacketBuilder::Fixed_Build<Packet::Server::spectatorCantSpectate, 'i'>(tP->UserID);

		SpecHost->SendToSpecs(b);
		SpecHost->addQueArray(b);
	}
}

DWORD COUNT_CURRENTONLINE = 0;
DWORD COUNT_REQUESTS = 0;
DWORD COUNT_MULTIPLAYER = 0;

void RenderHTMLPage(_Con s, const _HttpRes &res){
	s.SendData(ConstructResponse(200, { {"Content-Type", "text/html; charset=utf-8"} },
		"<HTML><img src=\"https://cdn.discordapp.com/attachments/385279293007200258/567292020104888320/unknown.png\">"
		"<br> Online users: " + std::to_string(COUNT_CURRENTONLINE) + " | " + std::to_string(COUNT_REQUESTS) + " total connections handled."
		"<br>" + std::to_string(COUNT_MULTIPLAYER) + " currently active multiplayer games.</HTML>"));
}

uint64_t GenerateChoToken(){
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

template<typename T = std::string>
_inline T ReadUleb(size_t &O, const size_t Max) {

	const DWORD Size = ReadUlebSize(O, Max);

	const char* S = (char*)O;
	O += Size;

	return T(S, Size);
}

_inline void SkipUleb(size_t &O, const size_t Max){
	O += ReadUlebSize(O, Max);
}

#include <fstream>

std::vector<byte> LOAD_FILE_RAW(const std::string& FILENAME) {

	std::ifstream f(FILENAME, std::ios::binary | std::ios::ate);

	VEC(byte) Ret;

	if (f.is_open()) {
		const size_t pos = (size_t)f.tellg();
		Ret.resize(pos);
		f.seekg(0, std::ios::beg);
		f.read((char*)&Ret[0], pos);
		f.close();
	}

	return Ret;
}

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

_inline bool WriteAllBytes(const std::string& filename, const void* data, const DWORD Size){

	std::ofstream ifs;
	ifs.open(filename, std::ofstream::binary);

	if (!ifs.is_open())
		return 0;

	ifs.write((char*)data, Size);
	ifs.close();

	return 1;
}

bool DownloadMapFromOsu(const int ID) {

	if (ID < 0 || ID > 6000000)
		return 0;

	const std::string bFile = GET_WEB_CHUNKED("old.ppy.sh", std::string("osu/" + std::to_string(ID)));

	if (bFile.size() == 0 || bFile.find("[HitObjects]") == std::string::npos)return 0;

	//ReplaceAll(bFile, "\r\n", "\n");//Could save diskspace

	WriteAllBytes(BeatmapPath + std::to_string(ID) + ".osu", (void*)&bFile[0], bFile.size());

	return 1;
}

#include "oppai.h"

bool OppaiCheckMapDownload(ezpp_t ez, const DWORD BID) {

	const std::string MapPath = BeatmapPath + std::to_string(BID) + ".osu";

	const DWORD Size = GetFileSize(MapPath);
	bool MapDownloaded = 0;

	if (!Size){

		MapDownloaded = DownloadMapFromOsu(BID);

		if (!MapDownloaded){
			printf(KRED "Failed to download %i.osu\n" KRESET, BID);
			WriteAllBytes(MapPath, " ", 1);//Stop it from trying it over and over again.
			return 0;
		}
	}
	if (Size < 100 && !MapDownloaded)
		return 0;

	const int res = ezpp(ez, (char*)MapPath.c_str());

	if (res < 0) {
		printf(KMAG "oppai> " KRED "Failed with errorcode %i\n" KRESET, res);
		return 0;
	}

	return 1;
}

#include "Match.h"

int getSetID_fHash(const std::string &H, _SQLCon* c){//Could combine getbeatmapid and getsetid into one

	if (c){
		std::unique_ptr<sql::ResultSet> res{c->ExecuteQuery("SELECT beatmapset_id FROM beatmaps WHERE beatmap_md5 = '" + H + "' LIMIT 1")};
		if (res && res->next())
			return res->getInt(1);
	}

	return StringToNum(int, JSON::_Json(GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "h=" + H)).GetString<WSTI("beatmapset_id")>());
}


int getBeatmapID_fHash(const std::string &H, _SQLCon* c) {

	if (H.size() != 32)return 0;

	if (c){
		std::unique_ptr<sql::ResultSet> res{c->ExecuteQuery("SELECT beatmap_id FROM beatmaps WHERE beatmap_md5 = '" + H + "' LIMIT 1")};

		if (res && res->next())
			return res->getInt(1);
	}
	return StringToNum(int, JSON::_Json(GET_WEB_CHUNKED("old.ppy.sh", osu_API_BEATMAP + "h=" + H)).GetString<WSTI("beatmap_id")>());
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


struct _UserUpdate{

	enum : u32{ Restrict, UnRestrict };

	u32 Target : 32, Caller : 28, Action : 4;
	size_t Data;

	_UserUpdate(u32 Target, u32 Caller, u32 Action, size_t Data) : Target(Target),Caller(Caller), Action(Action), Data(Data){}

};

locked_vector<_UserUpdate> UpdateQue;

#include "Aria.h"
#include "User.h"
#include "Commands.h"

void BotMessaged(_User *tP, const std::string_view Message){
	
	if (Message.size() == 0)return;

	if (Message[0] == '!'){

		if (!MEM_CMP_START(Message, "!with") && !MEM_CMP_START(Message, "!acc")) {
			
			DWORD Unused;
			if (const std::string& Res = ProcessCommand(tP, Message, Unused); Res.size())
				PacketBuilder::Build<Packet::Server::sendMessage, 'm','-','s','s','i'>(tP->QueBytes,&tP->qLock,STACK(M_BOT_NAME),&Res,&tP->Username,USERID_START-1);

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
				return PacketBuilder::Build<Packet::Server::sendMessage, 'm', '-', '-', 's', 'i'>(tP->QueBytes, &tP->qLock, STACK(M_BOT_NAME),
					STACK("This appears to be a Beatmapset - Which is not currently supported sorry!"), &tP->Username, USERID_START - 1);

			char* Temp = new char[mSize]();

			size_t pos = BotMessage[BeatmapOffset].find_last_of('/');

			if (pos != std::string::npos && pos != mSize - 1)
				memcpy(Temp, &BotMessage[BeatmapOffset][pos + 1], mSize - (pos + 1));

			Temp[(mSize > 0) ? mSize - 1 : 0] = 0;//Just incase there is some mallice.

			mapID = MemToNum<int>(Temp,strlen(Temp));
			delete[] Temp;
		}

		if (mapID){

			#define READ(s) case WSTI(#s): Mods |= s;break;

			for (size_t i = 3; i < BotMessage.size(); i++){

				if (BotMessage[i].size() < 6)
					continue;

				switch (WSTI(std::string_view(BotMessage[i].data() + 1, BotMessage.size() - 1))){

					READ(DoubleTime)
					READ(Nightcore)
					READ(Hidden)
					READ(HardRock)
					READ(Flashlight)
					READ(SpunOut)
					READ(Easy)
					READ(NoFail)
					READ(HalfTime)
					READ(Relax)

					default:break;
				}

			}

			Mods |= (Mods & Nightcore) ? DoubleTime : 0;

			#undef READ
		}

	}

	if (!mapID){
		if (BotMessage.size() >= 2){
			
			if (BotMessage[0] == "!acc")
				return PacketBuilder::Build<Packet::Server::sendMessage,'m','-','-','s','i'>(tP->QueBytes,&tP->qLock,STACK(M_BOT_NAME),
					STACK("This is now in !with. Here are some examples of the usage.\n!with HDDT 95\n!with 98.5 HD HR"),&tP->Username, USERID_START-1);

			if (BotMessage[0] == "!with" && (mapID = tP->LastSentBeatmap)){

				std::string Combined;

				for (DWORD i = 1; i < BotMessage.size(); i++) {
					Combined += BotMessage[i];

					if (Acc == 0.f || Combo == 0){
						byte Com = 0;
						std::string tNum;

						for (const char C : BotMessage[i]) {
							if ((C >= '0' && C <= '9'))
								tNum.push_back(C);
							else if (C == '.' && tNum.size() != 0 && Com != 2) {
								Com = 2;
								tNum.push_back(C);
							}
							else if ((C == 'x' || C == 'X') && Com != 2)
								Com = 1;
						}

						if (tNum.size()) {
							if (Com == 1)Combo = std::stoi(tNum);
							else Acc = std::stof(tNum);
						}
					}
				}

				std::transform(begin(Combined), end(Combined), begin(Combined), [](u8 c){return std::tolower(c);});

				constexpr std::pair<const char*, u32> ModValues[] = { {"hd",Hidden},{"dt",DoubleTime}, {"fl",Flashlight},{"ez",Easy} ,{"ht",HalfTime} ,{"rx",Relax} ,{"so",SpunOut}, {"hr", HardRock} };

				for(size_t i = 0; i < ARRAY_SIZE(ModValues);i++)
						Mods |= Combined.find(ModValues[i].first) != std::string::npos ? ModValues[i].second : 0;

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

	if (ez){

		const std::string& Res = [&]()->std::string{

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
		}();

		PacketBuilder::Build<Packet::Server::sendMessage, 'm', '-', 's', 's', 'i'>(tP->QueBytes, &tP->qLock, STACK(M_BOT_NAME), &Res, &tP->Username, USERID_START - 1);

		ezpp_free(ez);
	}
}

void ReadMatchData(_Match &m, const byte* const Packet,const DWORD Size, bool Safe = 0){//todo make better
	/*
	size_t O = (size_t)&Packet[0];
	const size_t End = O + Size;

	O += 3;//Skip MatchId and inProgress.

	SafeRead(m.Settings.MatchType);
	SafeRead(m.Settings.Mods);
	m.Settings.Name = ReadUleb(O,End);

	if (!Safe){
		m.Settings.Password = ReadUleb(O,End);
		ReplaceAll(m.Settings.Password," ","_");
		ReplaceAll(m.Settings.Password, "//private", "");
	}
	else SkipUleb(O,End);

	m.Settings.BeatmapName = ReadUleb(O,End);
	SafeRead(m.Settings.BeatmapID);
	m.Settings.BeatmapChecksum = ReadUleb(O,End);

	byte tSlotStatus[NORMALMATCH_MAX_COUNT];
	byte tSlotTeam[NORMALMATCH_MAX_COUNT];
	if (O + NORMALMATCH_MAX_COUNT > End)return;
	if (!Safe)memcpy(tSlotStatus, (void*)O, NORMALMATCH_MAX_COUNT); O += NORMALMATCH_MAX_COUNT;
	if (O + NORMALMATCH_MAX_COUNT > End)return;
	if (!Safe)memcpy(tSlotTeam, (void*)O, NORMALMATCH_MAX_COUNT); O += NORMALMATCH_MAX_COUNT;
	if (!Safe)m.HostID = *(DWORD*)O; O += 4;

	for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
		if (m.Slots[i].SlotStatus & SlotStatus::HasPlayer)
			O += 4;
	
	if (O + 4 > End)return;

	m.Settings.PlayMode = *(byte*)O; O++;
	m.Settings.ScoringType = *(byte*)O; O++;
	m.Settings.TeamType = *(byte*)O; O++;
	m.Settings.FreeMod = *(byte*)O; O++;

	if (m.Settings.FreeMod){
		if (O + (NORMALMATCH_MAX_COUNT * 4) > End)return;
		int tCurrentMods[NORMALMATCH_MAX_COUNT];
		memcpy(tCurrentMods, (void*)O, NORMALMATCH_MAX_COUNT * 4);

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			m.Slots[i].CurrentMods = tCurrentMods[i];		

		O += NORMALMATCH_MAX_COUNT * 4;
	}
	SafeRead(m.Seed);*/
}

void New_ReadMatchData(_Match& m, const u8* const Packet, const u32 Size, bool FullRead = 0) {//todo make better


	const u8* Current = Packet;
	const u8* End = Current + Size;

	Current += 3;//Skip MatchId and inProgress.

	SafeRead(m.Settings.MatchType);
	SafeRead(m.Settings.Mods);

	m.Settings.Name = ReadUleb((size_t&)Current, (size_t)End);

	if (!FullRead) {

		m.Settings.Password = ReadUleb((size_t&)Current, (size_t)End);

		std::for_each(begin(m.Settings.Password), end(m.Settings.Password), Space_To_Underscore);

		//ReplaceAll(m.Settings.Password, "//private", "");

	}else SkipUleb((size_t&)Current, (size_t)End);

	m.Settings.BeatmapName = ReadUleb((size_t&)Current, (size_t)End);
	SafeRead(m.Settings.BeatmapID);
	m.Settings.BeatmapChecksum = ReadUleb((size_t&)Current, (size_t)End);

	byte tSlotStatus[NORMALMATCH_MAX_COUNT];
	byte tSlotTeam[NORMALMATCH_MAX_COUNT];
	if (Current + NORMALMATCH_MAX_COUNT > End)return;
	if (!FullRead)memcpy(tSlotStatus, (void*)Current, NORMALMATCH_MAX_COUNT); Current += NORMALMATCH_MAX_COUNT;
	if (Current + NORMALMATCH_MAX_COUNT > End)return;
	if (!FullRead)memcpy(tSlotTeam, (void*)Current, NORMALMATCH_MAX_COUNT); Current += NORMALMATCH_MAX_COUNT;
	if (!FullRead) {
		m.HostID = *(DWORD*)Current; Current += 4;
	}
	/*
	if (!Safe){
		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++) {
			m.Slots[i].SlotStatus = tSlotStatus[i];
			m.Slots[i].SlotTeam = tSlotTeam[i];
		}
	}*/

	for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
		if (m.Slots[i].SlotStatus & SlotStatus::HasPlayer)
			Current += 4;

	SafeRead(m.Settings.PlayMode);
	SafeRead(m.Settings.ScoringType);
	SafeRead(m.Settings.TeamType);
	SafeRead(m.Settings.FreeMod);


	if (m.Settings.FreeMod) {
		if (Current + (NORMALMATCH_MAX_COUNT * 4) > End)return;
		int tCurrentMods[NORMALMATCH_MAX_COUNT];
		memcpy(tCurrentMods, (void*)Current, NORMALMATCH_MAX_COUNT * 4);

		for (DWORD i = 0; i < NORMALMATCH_MAX_COUNT; i++)
			m.Slots[i].CurrentMods = tCurrentMods[i];

		Current += NORMALMATCH_MAX_COUNT * 4;
	}
	SafeRead(m.Seed);
}

/*
_inline void SendMatchList(_User *tP){

	//make subscribers

	constexpr DWORD mSize = Match.size();

	for (DWORD i = 0; i < mSize; i++)
		if (Match[i].PlayerCount && !Match[i].Tournament){		
			if (auto& [Lock, UpdateTime, Cache] = LobbyCache[i];
					Match[i].LastUpdate != UpdateTime){

				std::scoped_lock<std::shared_mutex> L1(Lock);
				std::scoped_lock<std::mutex> L2(Match[i].Lock);
				UpdateTime = Match[i].LastUpdate;
				tP->addQueVector(Cache = bPacket::bMatch<0>(&Match[i]));
			}else{
				std::shared_lock<std::shared_mutex> L(Lock);
				tP->addQueVector(Cache);
			}
		}	
}*/

std::shared_mutex LogOutLock;
u32 LogoutLog[32] = {};

void LogOutUser(_User* tP){

	mlock (LogOutLock)
		LogoutLog[LogoutOffset++ & 0x1f] = tP->UserID;

	DisconnectUser(tP);
}

_inline void LogoutUpdates(_User* tP){

	if (tP->LogOffset != LogoutOffset){

		u32 Arr[aSize(LogoutLog)];
		int Start(tP->LogOffset), End(tP->LogOffset);

		s_mlock (LogOutLock){
			memcpy(Arr, LogoutLog, sizeof(Arr));
			End = (tP->LogOffset = LogoutOffset) & 0x1f;
		}
		do tP->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::userLogout, 'i'>(Arr[Start]));
		while ((++Start & 0x1f) != End);

	}
}

template<typename ...T>
[[nodiscard]] std::tuple<T...> deserialize(const u8* Start, const size_t Size)noexcept {

	constexpr size_t Static_Size = ((std::is_same<T, std::string_view>::value ? 2 : sizeof(T)) + ...);

	const size_t EndPointer = (size_t)Start + Size;

	if (Static_Size > Size)
		return { T{}... };

	return {
		([&Start,&EndPointer]{

			T V = T();

			if constexpr (std::is_same<T, std::string_view>::value)
				V = ReadUleb<std::string_view>((size_t&)Start, EndPointer);
			else if ((size_t)Start + sizeof(T) < EndPointer) {
				V = *(T*)Start;
				Start += sizeof(T);
			}

			return V;
		}())...
	};
}

namespace Bancho_Event{

	namespace Client_Event{

		#define DefaultRead const u8* const Packet, const u32 Size

		void requestStatusUpdate(_User* const tP) {

			const DWORD Off = tP->GetStatsOffset();

			PacketBuilder::Build<Packet::Server::userStats, 'm',
				'i', 'b', 'a', '5', 'i', 'b', 'i', 'l', 'i', 'i', 'l', 'i', 'w'>(tP->QueBytes, &tP->qLock,
					tP->UserID, tP->actionID, tP->ActionText, tP->ActionMD5, tP->actionMods, tP->GameMode,
					tP->BeatmapID, tP->Stats[Off].rScore, *(int*)&tP->Stats[Off].Acc,
					tP->Stats[Off].pp > u16(-1) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount,
					tP->Stats[Off].tScore, tP->Stats[Off].getRank(Off, tP->UserID), u16(tP->Stats[Off].pp)
					);
		}

		void changeAction(_User* tP, DefaultRead) {

			const auto& [new_ActionID, new_ActionText, new_CheckSum, new_Mods, new_GameMode, new_BeatmapID] = deserialize<u8, std::string_view, std::string_view, u32, byte, int>(Packet, Size);

			WriteStringToArray(tP->ActionText, new_ActionText);

			if (new_CheckSum.size() == 32) {

				std::array<char, 32> Buffer;
				memcpy(Buffer.data(), new_CheckSum.data(), 32);

				for (auto& c : Buffer)
					if (c == '\'')
						c = '_';

				memcpy(tP->ActionMD5, Buffer.data(), 32);
			}

			if (new_ActionID != bStatus::sAfk)tP->actionMods = new_Mods; //Stop relax flickering.

			tP->GameMode = new_GameMode;

			if (new_BeatmapID)tP->BeatmapID = new_BeatmapID;

			VEC(byte) Stats;
			const DWORD Off = tP->GetStatsOffset();

			PacketBuilder::Build<Packet::Server::userStats,
				'i', 'b', 'a', '5', 'i', 'b', 'i', 'l', 'i', 'i', 'l', 'i', 'w'>(Stats,
					tP->UserID, tP->actionID, tP->ActionText, tP->ActionMD5, tP->actionMods, tP->GameMode,
					tP->BeatmapID, tP->Stats[Off].rScore, *(int*)&tP->Stats[Off].Acc,
					tP->Stats[Off].pp > USHORT(-1) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount,
					tP->Stats[Off].tScore, tP->Stats[Off].getRank(Off, tP->UserID), USHORT(tP->Stats[Off].pp)
					);

			tP->addQueVector(Stats);

			if (tP->Spectators.size()) {
				std::shared_lock l(tP->SpecLock);

				for (auto Spec : tP->Spectators)
					if (Spec)Spec->addQueVector(Stats);
			}

		}

		void userStatsRequest(_User* u, DefaultRead) {

			if (unlikely(Size <= 2))
				return;

			const size_t Count = al_min(*(USHORT*)Packet, 64);

			if (unlikely((Count * 4) + 2 > Size))
				return;

			std::scoped_lock l(u->qLock);

			const u32* UserID = (u32*)(Packet + 2);

			for (const u32* End = UserID + Count; UserID != End; ++UserID){
				if (*UserID < USERID_START) {
					u->addQueArray<0>(PacketBuilder::CT::BotInfo::Bot_Stats);
					continue;
				}

				_UserRef tP(GetUserFromID(*UserID), 1);
				if (!tP || !tP->choToken || (tP->is_banned() && u != tP.User)) {
					//u->addQueArray<0>(PacketBuilder::Fixed_Build<Packet::Server::userLogout,'i'>(uID));
					continue;
				}

				const DWORD Off = tP->GetStatsOffset();

				PacketBuilder::Build<Packet::Server::userStats,
					'i', 'b', 'a', '5', 'i', 'b', 'i', 'l', 'i', 'i', 'l', 'i', 'w'>(u->QueBytes,
						tP->UserID, tP->actionID, tP->ActionText, tP->ActionMD5, tP->actionMods, tP->GameMode,
						tP->BeatmapID, tP->Stats[Off].rScore, *(int*)&tP->Stats[Off].Acc,
						tP->Stats[Off].pp > USHORT(-1) ? (tP->Stats[Off].pp) : tP->Stats[Off].PlayCount,
						tP->Stats[Off].tScore, tP->Stats[Off].getRank(Off, tP->UserID), USHORT(tP->Stats[Off].pp)
						);
				
			}
			
		}

		void channelJoin(_User* tP, DefaultRead) {

			if (unlikely(Size <= 2))
				return;		

			const auto& [ChannelName] = deserialize<std::string_view>(Packet, Size);

			if (_Channel* c = GetChannelByName(WSTI(ChannelName)); c) {
				  c->JoinChannel(tP);
				  PacketBuilder::Build<Packet::Server::channelJoinSuccess, 'm', 'v'>(tP->QueBytes, &tP->qLock, &ChannelName);
			}else PacketBuilder::Build<Packet::Server::channelKicked, 'm', 'v'>(tP->QueBytes, &tP->qLock, &ChannelName);

			
		}

		void channelPart(_User* tP, DefaultRead) {

			if (unlikely(Size <= 2))
				return;

			const auto& [ChannelName] = deserialize<std::string_view>(Packet, Size);

			if (_Channel* c = GetChannelByName(WSTI(ChannelName)); c)
				c->PartChannel(tP);
			
		}
		
		void sendPublicMessage(_User* tP, DefaultRead) {

			if (Size < 7 || tP->isSilenced())
				return;

			const auto& [Sender, Message, Target_Channel] = deserialize<std::string_view, std::string_view, std::string_view>(Packet, Size);

			if (Message.size() == 0)
				return;

			const u32 Target = WSTI<u32>(Target_Channel);

			if ((Target == std::integral_constant<u32, "#highlight"_HU>::value) | (Target == std::integral_constant<u32, "#userlog"_HU>::value))
				return;

			_Channel* c = 0;

			if (Target == std::integral_constant<u32, "#multiplayer"_HU>::value) {

				if (_Match* const m = getMatchFromID(tP->CurrentMatchID); m) {

					DWORD notVisible = 0;

					const std::string& s = ProcessCommandMultiPlayer(tP, Message, notVisible, m);

					if (VEC(byte) Pack; !s.size()) {

						PacketBuilder::Build<Packet::Server::sendMessage, 's', 'v', '-', 'i'>(Pack, &tP->Username, &Message, STACK("#multiplayer"), tP->UserID);

						std::scoped_lock L(m->Lock);
						m->sendUpdateVector(Pack, tP);

					}else {

						PacketBuilder::Build<Packet::Server::sendMessage, '-', 's', '-', 'i'>(Pack, STACK(M_BOT_NAME), &s, STACK("#multiplayer"), USERID_START - 1);

						if (!notVisible) {
							  std::scoped_lock L(m->Lock);
							  m->sendUpdateVector(Pack, 0);
						}else tP->addQueVector(Pack);
					}
				}

				return;
			}
			else if ((tP->CurrentlySpectating || tP->Spectators.size()) && Target == CONSTX<u32, WSTI<u32>("#spectator")>::value) {

				_User* const SpecHost = (tP->Spectators.size()) ? tP : tP->CurrentlySpectating;

				if (SpecHost) {

					VEC(byte) b;
					PacketBuilder::Build<Packet::Server::sendMessage, 's', 'v', 'v', 'i'>(b, &tP->Username, &Message, &Target_Channel, tP->UserID);

					s_mlock(SpecHost->SpecLock) {
						for (_User* const s : SpecHost->Spectators) {
							if (s && s != tP)
								s->addQueVector(b);
						}
					}
					if (SpecHost != tP)
						SpecHost->addQueVector(b);
				}

				return;
			}
			else c = GetChannelByName(Target);

			if (!c)
				return PacketBuilder::Build<Packet::Server::channelKicked, 'm', 'v'>(tP->QueBytes, &tP->qLock, &Target_Channel);

			if (Message[0] == '!') {

				DWORD notVisible = 0;
				const std::string& Res = ProcessCommand(tP, Message, notVisible);

				if (!(tP->privileges & (u32)Privileges::Visible))
					notVisible = 1;

				if (Res.size()) {
					if (notVisible)
						PacketBuilder::Build<Packet::Server::sendMessage, 'm', '-', 's', 'v', 'i'>(tP->QueBytes, &tP->qLock, STACK(M_BOT_NAME), &Res, &Target_Channel, USERID_START - 1);
					else
						c->Bot_SendMessage(Res);
				}

				return;
			}

			if (tP->privileges & (u32)Privileges::Visible) {
				VEC(byte) Pack;
				PacketBuilder::Build<Packet::Server::sendMessage, 's', 'v', 'v', 'i'>(Pack, &tP->Username, &Message, &Target_Channel, tP->UserID);

				c->SendPublicMessage(tP, Pack);
			}
		}

		void sendPrivateMessage(_User* tP, DefaultRead) {

			if (Size < 7 || tP->isSilenced() || tP->is_banned())
				return;

			const auto& [Sender, Message, Target, ID] = deserialize<std::string_view, std::string_view, std::string_view, int>(Packet, Size);

			if (likely(Message.size())){

				if (Target == BOT_NAME)
					return BotMessaged(tP, Message);

				_UserRef u(GetUserFromName(Target), 1);

				if (unlikely(!u))
					return tP->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::userLogout, 'i'>(ID));

				if (u->isBlocked(tP->UserID))
					return;

				if (u->FriendsOnlyChat && !u->isFriend(tP->UserID))
					 PacketBuilder::Build<Packet::Server::userPMBlocked, 'm', 's', 'b', 's', 'i'>(tP->QueBytes, &tP->qLock, &tP->Username, 0, &u->Username, tP->UserID);
				else PacketBuilder::Build<Packet::Server::sendMessage, 'm', 's', 'v', 'v', 'i'>(u->QueBytes, &u->qLock, &tP->Username, &Message, &Target, tP->UserID);

			}

		}

		void startSpectating(_User* tP, DefaultRead) {

			if (Size < 4)return;

			const u32 ID = *(u32*)Packet;

			if (ID < USERID_START || ID == tP->UserID) {
				constexpr auto cPack = PacketBuilder::CT::String_Packet(Packet::Server::notification, "You can not spectate that user.");
				return tP->addQueArray(cPack);
			}

			_UserRef SpecTarget(GetUserFromID(ID), 1);

			if (!SpecTarget) {
				constexpr auto cPack = PacketBuilder::CT::String_Packet(Packet::Server::notification, "Failed to find user.");
				return tP->addQueArray(cPack);
			}

			if (tP->CurrentlySpectating)
				Event_client_stopSpectating(tP);

			bool Add = (tP->CurrentlySpectating != SpecTarget.User);

			tP->CurrentlySpectating = SpecTarget.User;

			{

				const auto b = PacketBuilder::Fixed_Build<Packet::Server::fellowSpectatorJoined, 'i'>(tP->UserID);

				std::scoped_lock L(SpecTarget->SpecLock);

				if (SpecTarget->Spectators.size() == 0)
					SpecTarget->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::channelJoinSuccess, '-'>(STACK("#spectator")));

				VEC(byte) Fellow; Fellow.reserve(SpecTarget->Spectators.size() * 11);

				for (auto& fSpec : SpecTarget->Spectators) {
					if (!fSpec) {
						if (Add)
							fSpec = tP;
						Add = 0;
					}
					else if (fSpec != tP) {
						fSpec->addQueArray(b);
						PacketBuilder::Build<Packet::Server::fellowSpectatorJoined, 'i'>(Fellow, fSpec->UserID);
					}
					else Add = 0;
				}

				tP->addQueVector(Fellow);

				if (Add)
					SpecTarget->Spectators.push_back(tP);

			}

			{
				const DWORD sOff = tP->GetStatsOffset();

				PacketBuilder::Build<Packet::Server::userPanel, 'm', 'i', 's', 'b', 'b', 'b', 'i', 'i', 'i'
					, '!', 'i'>
					(SpecTarget->QueBytes, &SpecTarget->qLock,
						tP->UserID, &tP->Username, 24 + tP->timeOffset, tP->country, GetUserType(tP->privileges), *(int*)&tP->lon, *(int*)&tP->lat, tP->Stats[sOff].getRank(sOff, tP->UserID),
						Packet::Server::spectatorJoined, tP->UserID);

			}

		}

		void spectateFrames(_User* tP, DefaultRead) {

			if (tP->Spectators.size() == 0)
				return;

			VEC(byte) Frame((size_t)Size + 7);

			*(USHORT*)Frame.data() = (USHORT)Packet::Server::spectateFrames;
			//Frame[2] = 0;
			*(DWORD*)(Frame.data() + 3) = Size;

			memcpy(Frame.data() + 7, Packet, Size);

			std::shared_lock<std::shared_mutex> L(tP->SpecLock);

			for (_User* s : tP->Spectators)
				if (s) s->addQueVector(Frame);

		}

		void createMatch(_User* tP, DefaultRead) {

			constexpr auto Failed = PacketBuilder::CT::PacketHeader(Packet::Server::matchJoinFail);

			if (tP->CurrentMatchID || tP->is_banned())//already in a match? Might want to kick them from the old one.
				return tP->addQueArray(Failed);

			_Match* const m = getEmptyMatch();

			if (unlikely(!m))
				return tP->addQueArray(Failed);

			if (tP->inLobby) {
				LobbyUsers.replace(tP, 0);
				tP->inLobby = 0;
			}

			ReadMatchData(*m, Packet, Size);//Might want to clean this up.

			m->HostID = tP->UserID;
			m->inProgress = 0;
			m->PlayersLoading = 0;
			m->Settings.Mods = 0;
			m->PlayerCount = 1;
			m->Slots[0].SlotStatus = SlotStatus::NotReady;
			m->Slots[0].User = tP;
			tP->CurrentMatchID = m->MatchId;
			m->Tournament = 0;

			SendToLobby(bPacket::bMatch<0>(m));

			auto MatchData = bPacket::bMatch(m);
			*(USHORT*)MatchData.data() = (USHORT)Packet::Server::matchJoinSuccess;

			PacketBuilder::Build<Packet::Server::channelJoinSuccess, '-'>(MatchData, STACK("#multiplayer"));

			tP->addQueVector(MatchData);

		}

		void partMatch(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);
				m->removeUser(tP, 0);
				m->sendUpdateVector(bPacket::bMatch(m));
				SendToLobby(bPacket::bMatch<0>(m));
			}

		}

		void matchChangeSlot(_User* tP, DefaultRead) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m && Size == 4) {

				std::scoped_lock L(m->Lock);

				_Slot* OldSlot = 0;

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						OldSlot = &Slot;
						break;
					}

				_Slot& New = m->Slots[al_min(*(u32*)Packet, MULTI_MAXSIZE - 1)];

				if (OldSlot && OldSlot != &New && !New.User && (New.SlotStatus & (SlotStatus::Locked | SlotStatus::Ready)) == 0){
					New = *OldSlot;
					*OldSlot = _Slot();
					m->sendUpdateVector(bPacket::bMatch(m));
				}

			}

		}

		void joinLobby(_User* tP) {

			_Match* const m = getMatchFromID(tP->CurrentMatchID);

			if (m) {
				std::scoped_lock L(m->Lock);
				m->removeUser(tP, 0);
				m->sendUpdateVector(bPacket::bMatch(m));//tell the other people that they left.
			}

			if (!tP->inLobby) {
				LobbyUsers.replace(0, tP);
				tP->inLobby = 1;
				//Might want to send all match data here.
			}
		} 

		void matchChangeSettings(_User* tP, DefaultRead) {

			_Match* m = getMatchFromID(tP->CurrentMatchID);

			if (!m)return;

			std::scoped_lock L(m->Lock);

			if (m->HostID != tP->UserID)//Non host trying to change settings
				return;

			const auto OldFlags = m->Settings.StatusFlags();

			ReadMatchData(*m, Packet, Size, 1);

			const bool unReadyUsers = (OldFlags.Total != m->Settings.StatusFlags().Total);

			if (OldFlags.FreeMod != m->Settings.FreeMod) {

				const u32 TimeMods(m->Settings.Mods & TimeAltering),
					NonTimeMods(m->Settings.Mods & (~TimeAltering));

				if (m->Settings.FreeMod) {

					m->Settings.Mods = TimeMods;
					for (auto& Slot : m->Slots)
						Slot.CurrentMods = Slot.User ? NonTimeMods : 0;
				}
				else {
					for (auto& Slot : m->Slots) {
						if (unlikely(Slot.User == tP))
							m->Settings.Mods = Slot.CurrentMods | TimeMods;//This slot is the host so we need to update the global mods to their personal mods.

						Slot.CurrentMods = 0;
					}
				}
			}

			if (unReadyUsers) {
				m->ClearPlaying();
				m->UnreadyUsers();
			}

			/*VEC(byte) Update = bPacket::bMatch(m);
			if (MapUpdated){
				std::string Mes = "(Bloodcat)[https://bloodcat.com/osu?q=" + std::to_string(m->Settings.BeatmapID) + "]";
				PacketBuilder::Build<Packet::Server::sendMessage, '-', 's', '-', 'i'>(Update, STACK(M_BOT_NAME), &Mes, STACK("#multiplayer"), USERID_START - 1);
			}*/
			m->sendUpdateVector(bPacket::bMatch(m));
			SendToLobby(bPacket::bMatch<0>(m));
		}

		void friendAdd(_User* tP, DefaultRead) {

			const u32 ID = Size == 4 ? *(u32*)Packet : 0;

			if (!ID || ID >= INT_MAX)return;

			enum : u8{ ListFull, Added, AlreadyIn };

			u8 Status = ListFull;

			for (size_t i = 0; i < aSize(tP->Friends); ++i) {
				if (tP->Friends[i] == ID) {
					Status = AlreadyIn;
					break;
				}
				if (!tP->Friends[i]) {
					Status = Added;
					tP->Friends[i] = ID;
					break;
				}
			}

			if (Status == ListFull) {
				constexpr auto b = PacketBuilder::CT::String_Packet(Packet::Server::notification, "You may only have a maximum of 256 friends.");
				tP->addQueArray(b);
			}else if (Status == Added)
				SQLExecQue.AddQue(SQL_INSERT("users_relationships", {
					_SQLKey("user1", tP->UserID),
					_SQLKey("user2", ID)
					}));
		}

		void friendRemove(_User* tP, DefaultRead) {

			const u32 ID = Size == 4 ? *(u32*)Packet : 0;

			if (!ID || ID >= INT_MAX)return;

			enum : u8{ NotRemoved, Removed };

			u8 Status = NotRemoved;

			for (size_t i = 0; i < aSize(tP->Friends); ++i)
				if (unlikely(tP->Friends[i] == ID)){

					bool f = 0;

					for (USHORT z = aSize(tP->Friends); --z > i;) {
						if (tP->Friends[z]) {
							f = 1;
							tP->Friends[i] = tP->Friends[z];
							tP->Friends[z] = 0;
						}
					}
					if (!f)
						tP->Friends[i] = 0;

					Status = Removed;
					break;
				}

			if (Status == Removed){



				SQLExecQue.AddQue("DELETE FROM users_relationships WHERE user1 = " + std::to_string(tP->UserID) + " AND user2 = " + std::to_string(ID));
			}
		}

		void matchLock(_User* tP, DefaultRead) {

			if (Size != 4)return;

			const u32 SlotID = *(u32*)Packet;

			if (_Match* m = getMatchFromID(tP->CurrentMatchID);
				m && SlotID < MULTI_MAXSIZE) {

				if (std::scoped_lock L(m->Lock); m->HostID == tP->UserID) {

					_Slot& Slot = m->Slots[SlotID];

					if (Slot.User)m->removeUser(Slot.User, 1);
					else Slot.SlotStatus = Slot.SlotStatus == SlotStatus::Open ? SlotStatus::Locked : SlotStatus::Open;

					m->sendUpdateVector(bPacket::bMatch(m));
					SendToLobby(bPacket::bMatch<0>(m));
				}
			}

		}

		void matchChangeMods(_User* tP, DefaultRead) {

			if (Size != 4)return;

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				const u32 Mods = *(u32*)Packet;

				std::scoped_lock L(m->Lock);

				const bool isHost = (m->HostID == tP->UserID);

				if (!m->Settings.FreeMod) {

					if (isHost) m->Settings.Mods = Mods;
					else return;

				}else for (auto& Slot : m->Slots)
					if (Slot.User == tP) {

						if (isHost) m->Settings.Mods = Mods & TimeAltering;

						m->Slots[GetIndex(m->Slots, Slot)].CurrentMods = Mods & (~TimeAltering);

						break;
					}

				m->sendUpdateVector(bPacket::bMatch(m));
			}

		}

		void joinMatch(_User* tP, DefaultRead) {

			if (tP->is_banned()){
				constexpr auto b = PacketBuilder::CT::PacketHeader(Packet::Server::matchJoinFail);
				return tP->addQueArray(b);
			}

			const auto& [MatchID, Password] = deserialize<u32, std::string_view>(Packet, Size);

			if (!MatchID)return;

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {
				std::scoped_lock L(m->Lock);
				m->removeUser(tP, 0);
			}

			if (_Match* m = getMatchFromID(MatchID); m) {

				std::scoped_lock L(m->Lock);

				if (m->Settings.Password.size() == 0 || m->Settings.Password == Password && m->PlayerCount) {//Dont let players join empty matchs.

					for (auto& Slot : m->Slots) {

						if (Slot.SlotStatus == SlotStatus::Open && !Slot.User) {

							m->PlayerCount++;
							Slot.reset();
							Slot.SlotStatus = SlotStatus::NotReady;
							Slot.User = tP;

							tP->CurrentMatchID = m->MatchId;

							auto Pack = bPacket::bMatch(m);
							m->sendUpdateVector(Pack, tP);

							PacketBuilder::Build<Packet::Server::channelJoinSuccess, '-'>(Pack, STACK("#multiplayer"));
							*(USHORT*)Pack.data() = (USHORT)Packet::Server::matchJoinSuccess;
							tP->addQueVector(Pack);

							if (tP->inLobby) {
								LobbyUsers.replace(tP, 0);
								tP->inLobby = 0;
							}

							SendToLobby(bPacket::bMatch<0>(m));
							return;
						}
					}
				}
			}

			tP->addQueArray(PacketBuilder::Fixed_Build<Packet::Server::disposeMatch, 'i', '!'>(MatchID, Packet::Server::matchJoinFail));
		}

		void matchChangeTeam(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						Slot.SlotTeam = ~Slot.SlotTeam;
						m->sendUpdateVector(bPacket::bMatch(m));
						break;
					}
			}
		}

		void matchNoBeatmap(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						if (Slot.SlotStatus != SlotStatus::NoMap) {
							Slot.SlotStatus = SlotStatus::NoMap;
							m->sendUpdateVector(bPacket::bMatch(m));
						}
						break;
					}
			}

		}

		void matchHasBeatmap(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						if (Slot.SlotStatus == SlotStatus::NoMap) {
							Slot.SlotStatus = SlotStatus::NotReady;
							m->sendUpdateVector(bPacket::bMatch(m));
						}break;
					}
			}
		}

		void matchTransferHost(_User* tP, DefaultRead) {

			if (Size != 4)return;

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				_Slot& Target = m->Slots[al_min(*(u32*)Packet, MULTI_MAXSIZE - 1)];

				if (m->HostID == tP->UserID && Target.User && Target.User->choToken) {

					m->HostID = Target.User->UserID;

					constexpr auto HostPacket = PacketBuilder::CT::PacketHeader(Packet::Server::matchTransferHost);

					Target.User->addQueArray(HostPacket);

					m->sendUpdateVector(bPacket::bMatch(m));
				}

			}

		}

		void matchReady(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						if (likely(Slot.SlotStatus != SlotStatus::Ready)) {
							Slot.SlotStatus = SlotStatus::Ready;
							m->sendUpdateVector(bPacket::bMatch(m));
						}
						break;
					}
			}
		}

		void matchNotReady(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						if (likely(Slot.SlotStatus != SlotStatus::NotReady)) {
							Slot.SlotStatus = SlotStatus::NotReady;
							m->sendUpdateVector(bPacket::bMatch(m));
						}
						break;
					}
			}
		}

		void matchStart(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				if (m->HostID == tP->UserID && m->Settings.BeatmapID != -1) {

					m->ClearPlaying();

					for (auto& Slot : m->Slots)
						if (Slot.SlotStatus & (SlotStatus::Ready | SlotStatus::NotReady)) {
							Slot.resetPlaying();
							Slot.SlotStatus = SlotStatus::Playing;
						}

					auto MatchData = bPacket::bMatch(m);
					*(u16*)MatchData.data() = (u16)Packet::Server::matchStart;

					m->sendUpdateVector(MatchData);
					SendToLobby(bPacket::bMatch<0>(m));
				}
			}

		}

		void matchLoadComplete(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {				
				
				bool AllLoaded = 1;

				std::scoped_lock L(m->Lock);				

				for (auto& Slot : m->Slots)
					if (unlikely(Slot.User == tP))
						Slot.Loaded = 1;
					else if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Loaded)
						AllLoaded = 0;

				if (AllLoaded) {
					auto Packet = bPacket::bMatch(m);
					PacketBuilder::Build<Packet::Server::matchAllPlayersLoaded, '.'>(Packet, 0);
					m->sendUpdateVector(Packet);
				}
			}
		}

		void matchScoreUpdate(_User* tP, DefaultRead) {

			if (tP->CurrentMatchID)
				if (_Match* m = getMatchFromID(tP->CurrentMatchID); m && (Size == 29 || Size == 45/*Score V2*/)) {

					VEC(u8) b(7 + Size);

					*(u16*)b.data() = (u16)Packet::Server::matchScoreUpdate;
					b[2] = 0;
					*(u32*)&b[3] = Size;

					memcpy(7 + b.data(), Packet, Size);
					enum { b_time, id, count300, count100, count50, countGeki, countKatu, countMiss, totalScore, maxCombo, currentCombo, perfect, currentHp, tagByte, usingScoreVs, comboPortion, bonusPortion };
					r_struct<u32, u8, u16, u16, u16, u16, u16, u16, u32, u16, u16, u8, u8, u8, u8> MatchData(b.data() + 7);

					std::scoped_lock L(m->Lock);

					for (auto& Slot : m->Slots)
						if (Slot.User == tP) {

							MatchData.get<id>() = (u8)GetIndex(m->Slots, Slot);
							m->sendUpdateVector(b);
							break;
						}
				}
		}

		void matchComplete(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						Slot.Completed = 1;
						break;
					}

				bool AllFinished = 1;

				for (auto& Slot : m->Slots){					
					if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Completed){
						AllFinished = 0;
						break;
					}
				}

				if (AllFinished) {

					m->sendUpdate(PacketBuilder::CT::PacketHeader(Packet::Server::matchComplete));

					m->ClearPlaying();

					m->sendUpdateVector(bPacket::bMatch(m));

				}

			}
		}

		void matchFailed(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						m->sendUpdate(PacketBuilder::Fixed_Build<Packet::Server::matchPlayerFailed, 'i'>(GetIndex(m->Slots, Slot)));
						break;
					}

			}
		}

		void matchSkipRequest(_User* tP) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m) {

				std::scoped_lock L(m->Lock);

				for (auto& Slot : m->Slots)
					if (Slot.User == tP) {
						Slot.Skipped = 1;
						m->sendUpdate(PacketBuilder::Fixed_Build<Packet::Server::matchPlayerSkipped, 'i'>(GetIndex(m->Slots, Slot)));
						break;
					}

				for (auto& Slot : m->Slots)
					if (Slot.SlotStatus == SlotStatus::Playing && !Slot.Skipped)
						return;

				m->sendUpdate(PacketBuilder::CT::PacketHeader(Packet::Server::matchSkip));
			}
		}

		void matchInvite(_User* tP, DefaultRead) {

			if (_Match* m = getMatchFromID(tP->CurrentMatchID); m && Size == 4) {
				const u32 InviteID = *(u32*)Packet;

				using namespace PacketBuilder::CT;

				if (InviteID < USERID_START) {
					constexpr auto b = PopulateHeader(Concate(PacketHeader(Packet::Server::sendMessage),
						String(M_BOT_NAME), String("Why would a bot want to join your match? You dirty shaved monkey."), String("#multiplayer"), Number<int>(USERID_START - 1)));

					return tP->addQueArray(b);
				}

				_UserRef Target(GetUserFromID(InviteID), 1);

				if (!Target) {

					constexpr auto b = PopulateHeader(Concate(PacketHeader(Packet::Server::sendMessage),
						String(M_BOT_NAME), String("Could not find player."), String("#multiplayer"), Number<int>(USERID_START - 1)));

					return tP->addQueArray(b);
				}

				{
					const std::string Mes = "Invited " + Target->ProfileLink() + " to your match.";

					PacketBuilder::Build<Packet::Server::sendMessage, 'm', '-', 's', '-', 'i'>(tP->QueBytes, &tP->qLock, STACK(M_BOT_NAME), &Mes, STACK("#multiplayer"), USERID_START - 1);
				}

				if (Target->isBlocked(tP->UserID))
					return;

				{
					const std::string Mes = "I have invited you to the multiplayer lobby \"[osump://" + std::to_string(m->MatchId) + "/" + m->Settings.Password + " " + m->Settings.Name + "]\".";

					//Users could fuck with the client [] url construction but its not dangerous in anyway so who cares.

					PacketBuilder::Build<Packet::Server::invite, 'm', 's', 's', 's', 'i'>(Target->QueBytes, &Target->qLock,
						&tP->Username, &Mes, &Target->Username, tP->UserID);
				}

			}
		}

		#undef DefaultRead

	}


}

void DoBanchoPacket(_Con s,const uint64_t choToken,const std::string_view PacketBundle){

	_UserRef tP(GetUserFromToken(choToken),1);

	if (!tP) {//No user online with that token

		using namespace PacketBuilder::CT;

		constexpr auto b = HTTPBytes<200>(Number_Packet(Packet::Server::restart, 1), _STACK("\r\n"));

		return (void)s.SendData(b);
	}

	tP->LastPacketTime = clock_ms();

	const u8* Pos = (u8*)PacketBundle.data();
	const u8* End = Pos + PacketBundle.size();

	while (Pos < End - 7){

		const struct {
			const size_t ID : 16, Size : 32;
		} PacketHeader{ *(u16*)Pos, *(u32*)(Pos += 3) };

		Pos += 4;

		if (Pos + PacketHeader.Size > End) {
			printf("%s> Out of range packet %llu|%llu\n", tP->Username.c_str(), PacketHeader.ID, PacketHeader.Size);
			tP->choToken = 0;
			return;
		}

		const u8* const Packet = Pos;
		
		Pos += PacketHeader.Size;

		using namespace Bancho_Event;
		using namespace Packet::Client;

		switch (PacketHeader.ID){

		case pong:
		case beatmapInfoRequest:
		case receiveUpdates://ruri does all this automatically so its useless :)
			break;

		case requestStatusUpdate:
			Client_Event::requestStatusUpdate(tP.User);
			break;

		case changeAction:
			Client_Event::changeAction(tP.User, Packet, PacketHeader.Size);
			break;

		case userStatsRequest:
			Client_Event::userStatsRequest(tP.User, Packet, PacketHeader.Size);
			break;

		case channelJoin:
			Client_Event::channelJoin(tP.User, Packet, PacketHeader.Size);
			break;

		case channelPart:
			Client_Event::channelPart(tP.User, Packet, PacketHeader.Size);
			break;

		case sendPublicMessage:
			Client_Event::sendPublicMessage(tP.User, Packet, PacketHeader.Size);
			break;

		case sendPrivateMessage:
			Client_Event::sendPrivateMessage(tP.User, Packet, PacketHeader.Size);
			break;

		case logout:
			if (tP->LoginTime + 5000 > tP.User->LastPacketTime)break;
			LogOutUser(tP.User);
			break;

		case startSpectating:
			Client_Event::startSpectating(tP.User, Packet, PacketHeader.Size);
			break;

		case stopSpectating:
			Event_client_stopSpectating(tP.User);
			break;
		case cantSpectate:
			Event_client_cantSpectate(tP.User);
			break;

		case spectateFrames:
			Client_Event::spectateFrames(tP.User,Packet, PacketHeader.Size);
			break;

		case createMatch:
			Client_Event::createMatch(tP.User, Packet, PacketHeader.Size);
			break;

		case partMatch:
			Client_Event::partMatch(tP.User);
			break;

		case matchChangeSlot:
			Client_Event::matchChangeSlot(tP.User,Packet, PacketHeader.Size);
			break;

		case joinLobby:
			Client_Event::joinLobby(tP.User);
			break;

		case partLobby:
			if (tP->inLobby){
				tP->inLobby = 0;
				LobbyUsers.replace(tP.User, 0);
			}
			break;

		case matchChangeSettings:
			Client_Event::matchChangeSettings(tP.User,Packet, PacketHeader.Size);
			break;

		case matchLock:
			Client_Event::matchLock(tP.User, Packet, PacketHeader.Size);
			break;

		case matchChangeMods:
			Client_Event::matchChangeMods(tP.User, Packet, PacketHeader.Size);
			break;

		case joinMatch:
			Client_Event::joinMatch(tP.User, Packet, PacketHeader.Size);
			break;

		case matchChangeTeam:
			Client_Event::matchChangeTeam(tP.User);
			break;

		case matchNoBeatmap:
			Client_Event::matchNoBeatmap(tP.User);
			break;

		case matchHasBeatmap:
			Client_Event::matchHasBeatmap(tP.User);
			break;

		case matchTransferHost:
			Client_Event::matchTransferHost(tP.User, Packet, PacketHeader.Size);
			break;
		
		case matchReady:
			Client_Event::matchReady(tP.User);
			break;

		case matchNotReady:
			Client_Event::matchNotReady(tP.User);
			break;

		case matchStart:
			Client_Event::matchStart(tP.User);
			break;

		case matchLoadComplete:
			Client_Event::matchLoadComplete(tP.User);
			break;

		case matchScoreUpdate:
			Client_Event::matchScoreUpdate(tP.User, Packet, PacketHeader.Size);
			break;

		case matchComplete:
			Client_Event::matchComplete(tP.User);
			break;

		case matchFailed:
			Client_Event::matchFailed(tP.User);
			break;

		case matchSkipRequest:
			Client_Event::matchSkipRequest(tP.User);
			break;

		case matchInvite:
			Client_Event::matchInvite(tP.User, Packet, PacketHeader.Size);
			break;

		case friendAdd:
			Client_Event::friendAdd(tP.User, Packet, PacketHeader.Size);
			break;

		case friendRemove:
			Client_Event::friendRemove(tP.User, Packet, PacketHeader.Size);
			break;

		default:
			printf("Unhandled Packet: ID:%i | Length:%i\n", PacketHeader.ID, PacketHeader.Size);
			break;
		}

	}

	//if (tP->inLobby)
		//SendMatchList(tP.User);//Sends multiplayer data if they are in the lobby.

	tP->LastPacketTime = clock_ms();

	LogoutUpdates(tP.User);

	tP->doQue(s);

}

MM_ALIGN u16 const countryCodes[] = { 0x2d2d, 0x5041, 0x5545, 0x4441, 0x4541, 0x4641, 0x4741, 0x4941, 0x4c41, 0x4d41, 0x4e41, 0x4f41, 0x5141, 0x5241, 0x5341, 0x5441,
									  0x5541, 0x5741, 0x5a41, 0x4142, 0x4242, 0x4442, 0x4542, 0x4642, 0x4742, 0x4842, 0x4942, 0x4a42, 0x4d42, 0x4e42, 0x4f42, 0x5242,
									  0x5342, 0x5442, 0x5642, 0x5742, 0x5942, 0x5a42, 0x4143, 0x4343, 0x4443, 0x4643, 0x4743, 0x4843, 0x4943, 0x4b43, 0x4c43, 0x4d43,
									  0x4e43, 0x4f43, 0x5243, 0x5543, 0x5643, 0x5843, 0x5943, 0x5a43, 0x4544, 0x4a44, 0x4b44, 0x4d44, 0x4f44, 0x5a44, 0x4345, 0x4545,
									  0x4745, 0x4845, 0x5245, 0x5345, 0x5445, 0x4946, 0x4a46, 0x4b46, 0x4d46, 0x4f46, 0x5246, 0x5846, 0x4147, 0x4247, 0x4447, 0x4547,
									  0x4647, 0x4847, 0x4947, 0x4c47, 0x4d47, 0x4e47, 0x5047, 0x5147, 0x5247, 0x5347, 0x5447, 0x5547, 0x5747, 0x5947, 0x4b48, 0x4d48,
									  0x4e48, 0x5248, 0x5448, 0x5548, 0x4449, 0x4549, 0x4c49, 0x4e49, 0x4f49, 0x5149, 0x5249, 0x5349, 0x5449, 0x4d4a, 0x4f4a, 0x504a,
									  0x454b, 0x474b, 0x484b, 0x494b, 0x4d4b, 0x4e4b, 0x504b, 0x524b, 0x574b, 0x594b, 0x5a4b, 0x414c, 0x424c, 0x434c, 0x494c, 0x4b4c,
									  0x524c, 0x534c, 0x544c, 0x554c, 0x564c, 0x594c, 0x414d, 0x434d, 0x444d, 0x474d, 0x484d, 0x4b4d, 0x4c4d, 0x4d4d, 0x4e4d, 0x4f4d,
									  0x504d, 0x514d, 0x524d, 0x534d, 0x544d, 0x554d, 0x564d, 0x574d, 0x584d, 0x594d, 0x5a4d, 0x414e, 0x434e, 0x454e, 0x464e, 0x474e,
									  0x494e, 0x4c4e, 0x4f4e, 0x504e, 0x524e, 0x554e, 0x5a4e, 0x4d4f, 0x4150, 0x4550, 0x4650, 0x4750, 0x4850, 0x4b50, 0x4c50, 0x4d50,
									  0x4e50, 0x5250, 0x5350, 0x5450, 0x5750, 0x5950, 0x4151, 0x4552, 0x4f52, 0x5552, 0x5752, 0x4153, 0x4253, 0x4353, 0x4453, 0x4553,
									  0x4753, 0x4853, 0x4953, 0x4a53, 0x4b53, 0x4c53, 0x4d53, 0x4e53, 0x4f53, 0x5253, 0x5453, 0x5653, 0x5953, 0x5a53, 0x4354, 0x4454,
									  0x4654, 0x4754, 0x4854, 0x4a54, 0x4b54, 0x4d54, 0x4e54, 0x4f54, 0x4c54, 0x5254, 0x5454, 0x5654, 0x5754, 0x5a54, 0x4155, 0x4755,
									  0x4d55, 0x5355, 0x5955, 0x5a55, 0x4156, 0x4356, 0x4556, 0x4756, 0x4956, 0x4e56, 0x5556, 0x4657, 0x5357, 0x4559, 0x5459, 0x5352,
									  0x415a, 0x4d5a, 0x454d, 0x575a, 0x3141, 0x3241, 0x314f, 0x5841, 0x4747, 0x4d49, 0x454a, 0x4c42, 0x464d, 0x0000, 0x0000, 0x0000};

_inline u8 getCountryNum(const u16 isoCode) {

	const auto Mask = _mm_set1_epi16(isoCode);

	for (size_t i = 0; i < 256; i += 8) {

		u32 res = _mm_movemask_epi8(_mm_cmpeq_epi16(Mask, _mm_load_si128((__m128i*)(countryCodes + i))));

		if (unlikely(res)){
			res = ~res;
			while (res & 3) { res >>= 2; ++i; }
			return i;
		}
	}

	return 0;
}

void BanchoIncorrectLogin(_Con s){

	using namespace PacketBuilder::CT;

	constexpr auto b = HTTPBytes<200>(Number_Packet(Packet::Server::userID, (int)-1), _STACK("\r\ncho-token: 0\r\n"));

	s.SendData(b);
}
void BanchoServerFull(_Con s) {
	LogError("Server Full");

	using namespace PacketBuilder::CT;

	constexpr auto b = HTTPBytes<200>(Concate(Number_Packet(Packet::Server::userID, (int)-1), String_Packet(Packet::Server::notification, "Server is currently full"))
									,_STACK("\r\ncho-token: 0\r\n"));

	s.SendData(b);
}

bool Outdated = 0;

void HandleBanchoPacket(_Con s, const _HttpRes &&RES,const uint64_t choToken) {

	if (RES.Body.size() <= 1)
		return;

	if (!choToken){//No token sent - Assume its the login request which only ever comes in once
		//std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();

		const auto LoginData = Explode_View(RES.Body, '\n',3);

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

		if (VersionFailed){

			using namespace PacketBuilder::CT;

			constexpr auto b = HTTPBytes<200>(Concate(Number_Packet(Packet::Server::userID, (int)-1),String_Packet(Packet::Server::notification, "Your client is out of date!\nPlease update your osu."))
											,_STACK("\r\ncho-token: 0\r\n"));

			return (void)s.SendData(b);
		}
		int UserID = 0;
		int Priv = 0;
		int SilenceEnd = 0;
		byte CountryCode = 0;
		bool NewLogin = 0;
		Achievement Ach;

		_UserRef u(GetUserFromNameSafe(Username_Safe, 1),1);

		if (u.User && u->Password == cPassword){
			UserID = u->UserID;
			Priv = u->privileges;//TODO make this able to be updated.
			SilenceEnd = u->silence_end;
			CountryCode = u->country;
			Ach = u->Ach;
		}else u.User = 0;

		if(!u){

			NewLogin = 1;
			_SQLCon *const con = &SQL_BanchoThread[s.ID];

			std::unique_ptr<sql::ResultSet> res{con->ExecuteQuery("SELECT id, password_md5, username, privileges,silence_end FROM users WHERE username_safe = '" + Username_Safe + "' LIMIT 1")};

			if (!res || !res->next())
				return BanchoIncorrectLogin(s);

			UserID = res->getInt(1);

			if (bcrypt_checkpw(cPassword.to_string().c_str(), res->getString(2).c_str()))
				return BanchoIncorrectLogin(s);//Might want to add a brute force lock out

			if (!u){
				u.Reset(GetPlayerSlot_Safe(Username_Safe),1);
				if (!u)
					return BanchoServerFull(s);

			}

			Username = res->getString(3);//get the database captialization for consistencies sake
			Priv = res->getInt(4);

			if ((Priv & (u32)Privileges::Visible) && (~Priv & (u32)Privileges::Verified)){

				DWORD NewPerms = 0;

				auto HWID = Explode_View(ClientData[3],':',5);

				if (HWID.size() == 5){

					const std::string MAC = std::string(HWID[2]);
					const std::string UID = std::string(HWID[3]);
					const std::string DID = std::string(HWID[4]);

					NewPerms = (u32)Privileges::Visible | (u32)Privileges::Verified;

					std::unique_ptr<sql::ResultSet> match {(sql::ResultSet*)0};

					//Check if the person is using wine. If they are only use the UID.
					if (unlikely(HWID[2] == "b4ec3c4334a0249dae95c284ec5983df" || HWID[4] == "ffae06fb022871fe9beb58b005c5e21d"))
						match.reset(con->ExecuteQuery("SELECT userid from hw_user WHERE unique_id='" + UID +
												  "'&&userid!=" + std::to_string(UserID) + " LIMIT 1;"));
					else
						match.reset(con->ExecuteQuery("SELECT userid from hw_user WHERE mac='"+ MAC +
												  "'&&unique_id='" + UID +
												  "'&&disk_id='"+ DID + "'&&userid!=" + std::to_string(UserID) + " LIMIT 1;"));

					if (!match || !match->next())
						con->ExecuteUPDATE(SQL_INSERT("hw_user",
							{
								_SQLKey("mac",_M(MAC)),
								_SQLKey("unique_id",_M(UID)),
								_SQLKey("disk_id",_M(DID)),
								_SQLKey("userid",UserID),
								_SQLKey("activated",1)
							}
						));
					else NewPerms = 0;
				}
				con->ExecuteUPDATE("UPDATE users SET privileges = " + std::to_string(NewPerms) + " WHERE id=" + std::to_string(UserID));
				Priv = NewPerms;
			}


			if (!Priv){

				using namespace PacketBuilder::CT;

				constexpr auto b = HTTPBytes<200>(Number_Packet(Packet::Server::userID, (int)-3), _STACK("\r\ncho-token: 0\r\n"));

				return (void)s.SendData(b);
			}
			

			SilenceEnd = res->getInt(5);

			if (Username != GetUsernameFromCache(UserID))
				UsernameCacheUpdateName(UserID, Username, &SQL_BanchoThread[s.ID]);

			{
				res.reset(con->ExecuteQuery("SELECT achievements_0,achievements_1 FROM users WHERE id=" + std::to_string(UserID)));

				if (res && res->next()){

					u64 Data[]{ res->getUInt64(1),res->getUInt64(2)};


					Ach.General[0] = Data[0] & u32(-1);Ach.General[1] = (Data[0] >> 32) & u32(-1);
					Ach.General[2] = Data[1] & u32(-1); Ach.General[3] = (Data[1] >> 32) & u32(-1);


				}
			}

			for(byte z = 0; z < 				
			#ifndef NO_RELAX
				2
			#else
				1
			#endif				
				; z++){

				res.reset(con->ExecuteQuery("SELECT ranked_score_std, playcount_std, total_score_std, avg_accuracy_std,pp_std,"
					"ranked_score_taiko, playcount_taiko, total_score_taiko, avg_accuracy_taiko, pp_taiko,"
					"ranked_score_ctb, playcount_ctb, total_score_ctb, avg_accuracy_ctb, pp_ctb,"
					"ranked_score_mania, playcount_mania, total_score_mania, avg_accuracy_mania, pp_mania, country FROM " + Stats_Table_Name[z] + " WHERE id = " + std::to_string(UserID) + " LIMIT 1"));

				if (res && res->next()){

					if (z == 0) {

						const std::string Country = res->getString(21);

						if (Country.size() == 2)
							CountryCode = getCountryNum(*(USHORT*)Country.data());

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
			}

			res.reset(con->ExecuteQuery("SELECT user2 FROM users_relationships WHERE user1 = " + std::to_string(UserID) + " LIMIT 256"));
			DWORD fCount = 0;
			while (res && res->next()){
				u->Friends[fCount] = res->getInt(1);
				fCount++;
			}

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

			u->timeOffset = StringToNum(byte, ClientData[1]);

			u->country = CountryCode;// getCountryNum(res.GetHeaderValue("CF-IPCountry")); the nature of private servers disallow this.. i guess bancho could use this :(
						   // Best course of action would to resolve it from cloudflare on registration, not verification.

			u->LoginTime = clock_ms();
			u->LastPacketTime = u->LoginTime;
			u->UserID = UserID;
			u->silence_end = SilenceEnd;

			u->Ach = Ach;

			if(NewLogin)
				u->Username = Username;

			u->Username_Safe = Username_Safe;
			u->Password = cPassword;

			u->choToken = GenerateChoToken();

			u->qLock.lock();

			if (~Priv & (u32)Privileges::Visible){
				constexpr auto b = PacketBuilder::CT::String_Packet(Packet::Server::notification, "Your account is currently restricted.");
				u->addQueArray<0>(b);
			}
			if (Outdated && Priv > (u32)Privileges::SuperAdmin)
				PacketBuilder::Build<Packet::Server::sendMessage, '-', '-', 's', 'i'>(u->QueBytes, STACK(M_BOT_NAME),
					STACK("This instance of ruri is out of date. (Consider updating.)[https://github.com/rumoi/ruri]"), &u->Username, USERID_START - 1);

			{				

				{
					u->addQueArray<0>(PacketBuilder::Fixed_Build<Packet::Server::userID, 'i', '!', 'w', 'i', 'i'>(UserID, Packet::Server::friendsList, 2, USERID_START - 1,UserID));
					size_t FriendCount = 0;

					while (FriendCount < (sizeof(Users[0].Friends) >> 2) && u->Friends[FriendCount])FriendCount++;

					if (FriendCount){

						const size_t ByteSize = FriendCount << 2;

						(u32&)u->QueBytes[u->QueBytes.size() - 14] += ByteSize;
						(u16&)u->QueBytes[u->QueBytes.size() - 10] += FriendCount;
						u->QueBytes.resize(u->QueBytes.size() + ByteSize);
						memcpy(u->QueBytes.data() + u->QueBytes.size() - ByteSize, u->Friends, ByteSize);

					}
				}

				using namespace PacketBuilder::CT;

				constexpr auto constPacket =
					Concate(
						Number_Packet<int>(Packet::Server::protocolVersion, CHO_VERSION),
						String_Packet(Packet::Server::notification, "Welcome to ruri.\nBuild: " __DATE__ " " __TIME__),
						Number_Packet<int>(Packet::Server::channelInfoEnd, 0),
						Number_Packet<int>(Packet::Server::supporterGMT, UserType::Supporter),
						String_Packet(Packet::Server::channelKicked, "#osu"),
						BotInfo::Bot_Panel, BotInfo::Bot_Stats
					);
				u->addQueArray<0>(constPacket);
			}

			{
				const int SilenceDelta = (SilenceEnd && SilenceEnd > time(0)) ? SilenceEnd - time(0) : 0;

				if (!SilenceDelta)u->silence_end = 0;

				PacketBuilder::Build<Packet::Server::silenceEnd, 'i',
					//'!', 'i',
					'!', 'i', 's', 'b', 'b','b','i','i','i',
					'!','i','b','a','5','i','b','i','l','i','i','l','i','w'>(u->QueBytes, SilenceDelta,
					//Packet::Server::userID, UserID,
					Packet::Server::userPanel, UserID, &u->Username, 24 + u->timeOffset, u->country, UserType::Supporter, *(int*)&u->lat, *(int*)& u->lon, u->Stats[0].getRank(0, UserID),
					Packet::Server::userStats, UserID, bStatus::sIdle,u->ActionText,u->ActionMD5,0,0,0,u->Stats[0].rScore,*(int*)&u->Stats[0].Acc,
						int(u->Stats[0].pp > USHORT(-1) ? (u->Stats[0].pp) : u->Stats[0].PlayCount), u->Stats[0].tScore, u->Stats[0].getRank(0, UserID), USHORT(u->Stats[0].pp)
				);

			}

			;

			u->addQueVector<0>(PublicChannelCache);

			if (const byte IRC_LEVEL = GetMaxPerm(u->privileges); IRC_LEVEL > IRC_Public){
				for (auto& Chan : ChannelTable){
					if (Chan.ViewLevel == IRC_Public)
						continue;

					if (Chan.ViewLevel <= IRC_LEVEL && !Chan.Hidden) {
						PacketBuilder::Build<Packet::Server::channelInfo, 's', 's', 'w'>(u->QueBytes, &Chan.ChannelName, &Chan.ChannelDesc, USHORT(Chan.ChannelCount));
						if (Chan.AutoJoin)
							PacketBuilder::Build<Packet::Server::channelJoinSuccess, 's'>(u->QueBytes, &Chan.ChannelName);
					}
				}
			}

			for (auto& gUser : Users){

				if (!gUser.choToken || !(gUser.privileges & (u32)Privileges::Visible) || &gUser == u.User || u->isBlocked(gUser.UserID))
					continue;

				const DWORD Off = gUser.GetStatsOffset();
				
				PacketBuilder::Build<Packet::Server::userPanel, 'i', 's', 'b', 'b', 'b', 'i', 'i', 'i',
					'!', 'i', 'b', 'a', '5', 'i', 'b', 'i', 'l', 'i', 'i', 'l', 'i', 'w'>
					(u->QueBytes, gUser.UserID, &gUser.Username, 24 + gUser.timeOffset, gUser.country, GetUserType(gUser.privileges), *(int*)&gUser.lon, *(int*)&gUser.lat, gUser.Stats[Off].getRank(Off, gUser.UserID),
						Packet::Server::userStats, gUser.UserID, gUser.actionID, gUser.ActionText, gUser.ActionMD5, gUser.actionMods, gUser.GameMode, gUser.BeatmapID, gUser.Stats[Off].rScore, *(int*)& gUser.Stats[Off].Acc,
						gUser.Stats[Off].pp > USHORT(-1) ? gUser.Stats[Off].pp : gUser.Stats[Off].PlayCount, gUser.Stats[Off].tScore, gUser.Stats[Off].getRank(Off, gUser.UserID), USHORT(gUser.Stats[Off].pp));
					
			}

			u->SendToken = 1;
			u->qLock.unlock();

			//const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
			//printf("LoginTime: %fms\n", double(double(TTime) / 1000000.0));

			u->doQue(s);

			debug_SendOnlineToAll(u.User);

			for (auto& Chan : ChannelTable)
				if (Chan.ViewLevel == IRC_Public && Chan.AutoJoin)
					Chan.JoinChannel(u.User);
		}
	}else DoBanchoPacket(s,choToken,RES.Body);

}


void DisconnectUser(_User *u){

	if (!u || !u->UserID || !u->choToken)return;

	for (DWORD i = 0; i < MAX_CHAN_COUNT; i++) {

		_Channel*const C = (_Channel*)u->ActiveChannels[i];

		if (!C)continue;

		C->PartChannel(u);
	}
	
	u->choToken = 0;

	if (u->inLobby){
		LobbyUsers.replace(u,0);
		u->inLobby = 0;
	}
	if (u->CurrentMatchID){
		if (_Match * m = getMatchFromID(u->CurrentMatchID); m){
			std::scoped_lock L(m->Lock);

			m->removeUser(u, 0);
			m->sendUpdateVector(bPacket::bMatch(m));
		}
		u->CurrentMatchID = 0;
	}

	if(u->CurrentlySpectating)
		Event_client_stopSpectating(u);

	for (size_t i = 0; i <= GM_MAX; i++)
		u->Stats[i].reset();

	{
		std::scoped_lock L(u->SpecLock);
		u->Spectators.clear();
	}
	{
		std::scoped_lock L(u->qLock);
		u->QueBytes.clear();
	}
}

void HandlePacket(_Con s){

	 _HttpRes res;

	if (!s.RecvData(res)){
		LogMessage(KRED "Connection lost");
		return s.Dis();
	}
	
	const auto UserAgent = res.GetHeaderValue("User-Agent");

	const uint64_t choToken = StringToNum(uint64_t, res.GetHeaderValue("osu-token"));

	if (UserAgent.size() == 0){

		const auto UserAgent2 = res.GetHeaderValue("user-agent");

		if (!UserAgent2.size()){
			LogMessage("No user agent set.");

			using namespace PacketBuilder::CT;

			constexpr auto b = HTTPBytes<200>(String_Packet(Packet::Server::notification, "An error occurred"), _STACK("\r\n"));

			s.SendData(b);

			return s.Dis();
		}
		if (UserAgent2 != "osu!" && !choToken) {//If it is not found
			RenderHTMLPage(s, res);
			//LogMessage("HTML page served");
			return s.Dis();
		}
	}

	HandleBanchoPacket(s, _M(res), choToken);

	return s.Dis();
}

//Used to logout dropped users and other house keeping
void LazyThread(){

	int LastUpdateCheck = 0;

	_SQLCon lThreadSQL;
	lThreadSQL.Connect();
	VEC(std::string) Que_Buffer;

	for (;;){

		Sleep(500);

		const int cTime = clock_ms();
		
		{
			DWORD PlayerCount = 0;

			for (auto& User : Users) {

				if (User.LastPacketTime == INT_MIN)
					continue;

				if (User.choToken){

					if (User.LastPacketTime + PING_TIMEOUT_OSU < cTime) {
						_UserRef UF(&User, 0);
						LogOutUser(UF.User);
					}else ++PlayerCount;
				}
				else if (User.LastPacketTime + FREE_SLOT_TIME < cTime)//Free slots after 30 minutes of logging out.
					User.LastPacketTime = INT_MIN;
			}
			COUNT_CURRENTONLINE = PlayerCount;
		}

		Sleep(500);

		{
			DWORD ActiveLobbyCount = 0;

			for(const auto& M : Match)
				if (M.PlayerCount > 0)
					++ActiveLobbyCount;

			COUNT_MULTIPLAYER = ActiveLobbyCount;
		}

		if (SQLExecQue.Commands.size()){

			{

				Que_Buffer.clear();

				std::scoped_lock L(SQLExecQue.CommandLock);

				for(auto& Command : SQLExecQue.Commands)
					Que_Buffer.emplace_back(_M(Command));

				SQLExecQue.Commands.clear();
			}

			for(auto& Command : Que_Buffer)
				lThreadSQL.ExecuteUPDATE(_M(Command), 1);
		}

		if (!Outdated && LastUpdateCheck < cTime){

			LastUpdateCheck = cTime + 15144840;
		}

		UpdateQue.pop_for_each([&lThreadSQL](const _UserUpdate& Update){
			switch (Update.Action){

			case _UserUpdate::UnRestrict:
			case _UserUpdate::Restrict:
				{
					_User OkayClang(USER_SERVER{});

					((void(*)(const u32, _User * const, _SQLCon&, std::string*) noexcept)(Update.Action == _UserUpdate::Restrict ? User::Restrict : User::UnRestrict))
						(Update.Target, Update.Caller ? _UserRef(GetUserFromID(Update.Caller), 1).User : &OkayClang, lThreadSQL,(std::string*)Update.Data);
				}
				break;

			default:
				break;
			}
		});

	}
}

void FillRankCache(){

	struct _Worker{

		std::vector<std::thread> Threads;

		void Sync(const char* Text){

			printf("%s:	0/%zi", Text,Threads.size());

			size_t Completed = 0;

			for (auto& t : Threads){
				t.join();
				printf("\r%s:	%zi/%zi", Text, ++Completed, Threads.size());
			}
			printf("\n");
			Threads.clear();
		}

		void operator += (std::thread&& t) noexcept{
			Threads.emplace_back(_M(t));
		}

	};

	std::array<_SQLCon,4> RankSQL;

	for (auto& s : RankSQL)
		s.Connect();


	const auto DoFillRank = [&RankSQL](size_t I){

		const size_t i = I & 0x1f;

		if (sql::ResultSet* res = RankSQL[i].ExecuteQuery(
			"SELECT id, " + PPColNames[i] + " FROM " + Stats_Table_Name[I >> 5] + " WHERE " + PPColNames[i] + " > 0 ORDER BY " + PPColNames[i] + " DESC");
			res){

			RankList[I = (i + ((I & 0x20) >> 3))].List.reserve(USHORT(-1));

			while (res->next())
				RankList[I].List.emplace_back(res->getUInt(1), res->getUInt(2));

			delete res;
		}
	};

	_Worker RankCacher;

	for (size_t i = 0; i < 4; i++)
		RankCacher += std::thread(DoFillRank, i);

	RankCacher.Sync("Caching ranks");

	if constexpr (RELAX_MODE){

		for (size_t i = 0; i < 4; i++)
			RankCacher += std::thread(DoFillRank, i | 0x20);

		RankCacher.Sync("Caching relax ranks");
	}

	for (auto& s : RankSQL)
		s.Disconnect();

}

void BanchoWork(const DWORD ID, _ConQue<SOCKET> &Slot){

	SOCKET ConnectionSocket = 0;

	for(;;){

		while (!(ConnectionSocket = Slot.S))
			Slot.wait();

		Slot.S = 0;

		HandlePacket(_Con{ ConnectionSocket,ID});
	}
}

#include "ruri_API.h"

void receiveConnections(){

	_ConQue<SOCKET> Slots[BANCHO_THREAD_COUNT] = {};

	{
		for (size_t i = 0; i < BANCHO_THREAD_COUNT; i++){

			if (!SQL_BanchoThread[i].Connect()) {

				printf(KRED "\nSQL connection error - Most likely incorrect credentials.\n\n" KRESET);
				return;
			}

			std::thread T(BanchoWork,i,std::ref(Slots[i]));
			T.detach();
		}
	}

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
		std::thread a(r_API::API_Main);
		a.detach();
	}
#endif

	Sleep(100);

	_SocketWorker<BANCHO_THREAD_COUNT> Worker;

	Worker.work(RURI_UNIX_SOCKET, BanchoWork, RURIPORT);
}

const char* Splashes[] = {	"Undefined definitively defined level of undefined behaviour",
							"Questionable coding ethics", "No refunds",
							"Guaranteed to aggravate all sides", "RTX on", "Harbours ambivalence in server owners",
							"Sometimes tested", "Fun for the whole family","Ominous",
							"We'll do it live", "Y2K compliant", "Built with slave labour",
							"JSON is the root of all evil", "Quixotic apprehension",
							"Abuses the instruction cache","Not sentient " KYEL"yet" KRESET};

int main(){

	#define COL1 KCYN
	#define COL2 KGRN

	printf(LOGO);

	#undef COL1
	#undef COL2

	printf("%s!\n",Splashes[time(0) % (sizeof(Splashes) / sizeof(char*))]);

	printf(TAG, COMPILER_NAME[COMPILER_VERSION % (IS_UNKNOWN + 1)]);

	const std::vector<byte> ConfigBytes = LOAD_FILE("config.json");

	if (!ConfigBytes.size()) {
		printf( KRED"\nconfig.json missing.\n" KRESET);
		return 0;
	}

	{
		JSON::_Json Config(ConfigBytes);

		#define ReadConfig(s) s = std::string(Config.GetString<WSTI(#s)>())

		ReadConfig(osu_API_Key);
		ReadConfig(SQL_Password);
		ReadConfig(SQL_Username);
		ReadConfig(SQL_Schema);
		ReadConfig(BeatmapPath);
		ReadConfig(ReplayPath);

		#undef ReadConfig

		if (const auto& GeneralName = Config.GetString<WSTI("GeneralName")>(); GeneralName.size()) {
			chan_General.ChannelName = GeneralName;
			chan_General.NameSum = WSTI(chan_General.ChannelName);
		}


		if (WSTI(Config.GetString<WSTI("New_Perms")>()) != WSTI("True")){

			printf( KRED"\n\nWarning:\n" KRESET
						"          ruri is now using a permission schema that differs from ripples.\n"
						"          Unless you want random users to have insane permission clearance you will want to sort this out before continuing.\n"
						"          Once all the permissions are cleaned up all you have to do is set \"New_Perms\" to \"True\" in the config.\n"
						"          Do not forget to remove newly registered accounts from obtaining the ripple UserPendingVerification flag.\n"
						"          Unless you want all users to pass the mod check.\n"
						"\n\n");


			return 0;
		}

	}

	static_assert((BANCHO_THREAD_COUNT >= 1 && ARIA_THREAD_COUNT >= 1),
		"BANCHO_THREAD_COUNT or ARIA_THREAD_COUNT can not be below 1");

	if (osu_API_Key.size() == 0)
		printf( KRED"\n-------- Warning! --------\nNo api key was given. Beatmap data can't update.\n\n" KRESET);
	else osu_API_BEATMAP = "api/get_beatmaps?k=" + osu_API_Key + "&";

	if (WSAStartup(MAKEWORD(2, 2), &wsData)) {
		printf("Failed to load WSA.\n");
		return 0;
	}

	for (auto& Chan : ChannelTable) {
		if (Chan.ViewLevel == IRC_Public && !Chan.Hidden) {
			PacketBuilder::Build<Packet::Server::channelInfo, 's', 's', 'w'>(PublicChannelCache,
				&Chan.ChannelName, &Chan.ChannelDesc, USHORT(Chan.ChannelCount));
		}
	}

	for (auto& Chan : ChannelTable)
		if (Chan.ViewLevel == IRC_Public && !Chan.Hidden && Chan.AutoJoin)
			PacketBuilder::Build<Packet::Server::channelJoinSuccess, 's'>(PublicChannelCache, &Chan.ChannelName);

	FillRankCache();

	receiveConnections();
	
	return 1;
}
