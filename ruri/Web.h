#pragma once

#include "HTML.h"


struct md5{

};


namespace web{

	struct _session_token{

		u32 UserID, Privileges;
		std::array<u64,3> Token;

		_session_token() noexcept{ BR::random_bytes(Token.data(), sizeof(Token)); UserID = Privileges = 0;}
		~_session_token() noexcept{ZeroMemory(this, sizeof(_session_token));}

		constexpr _session_token(int) : UserID(0), Privileges(0), Token{}{}
		constexpr _session_token(const decltype(Token) Token) noexcept: UserID(0), Privileges(0), Token(Token){}

		#pragma warning(suppress: 26495)
		_session_token(const _session_token& src) noexcept{ memcpy(this, &src, sizeof(this)); }

		_session_token(const _session_token&&) = delete;

		void operator()(const std::pair<u32/*UserId*/, u32/*Privileges*/> D) noexcept { UserID = D.first; Privileges = D.second; }
		bool operator==(const _session_token& a)const noexcept{return memcmp(Token.data(), a.Token.data(), sizeof(Token));}

	};

	locked_vector<_session_token, MAX_USER_COUNT> session_tokens;

	void render_page(const std::string& Body, _Con s){

		std::string HTML = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"description\" content=\"A custom server for osu!\"><meta name=\"author\" content=\"jacksonisiah/Navisu Developers\"><title>Leaderboard - Navisu</title><script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/3.1.0/jquery.min.js\"></script><script src=\"https://code.jquery.com/jquery-migrate-3.0.0.js\"></script><link rel=\"stylesheet\" href=\"https://unpkg.com/@clr/ui/clr-ui.min.css\" /><link rel=\"stylesheet\" href=\"./assets/css/navisu-main.css\" /><link href=\"https://fonts.googleapis.com/css?family=Quicksand:700&display=swap\" rel=\"stylesheet\"></head><body><div class=\"main-container\" style=\"background-color: #23182d;\"><header class=\"header header-3\"><div class=\"branding\"><a href=\"/\" class=\"nav-link\"><span class=\"title\" style=\"font-family: 'Quicksand'; font-size: 25px; margin-left: 30px;\">Navisu</span></a></div><div class=\"header-nav\"><a href=\"/\" class=\"nav-link nav-text\">Home</a><a href=\"/stuff/lb.php\" class=\"nav-link nav-text\">Leaderboard</a></div><div class=\"header-actions\"><a href=\"/sure/index.php?=2\"</div></header><div class=\"content-container\"><div class=\"content-area\">";

		HTML += Body + "</div></div></div></div></body></html>";

		s.SendData(ConstructResponse(200, Empty_Headers, HTML));
	}

	inline bool check_perms(const decltype(_session_token::Token) Token, const u32 Priv){
		return 0;//return session_tokens.find_copy_default(_session_token(Token), default_token).Privileges & Priv;
	}

	
	void Leaderboard(u32 GameMode, _Con s){

		GameMode = GameMode > GM_MAX ? GM_MAX : GameMode;

		std::string res = "<table style=\"text-align:center;font-size:22px;color:#BBBBBB;margin-left:auto;margin-right:auto;border-spacing:8px;\"><tr><td>Rank</td><td>Name</td><td>pp</td></tr>";
			
		s_mlock(RankList[GameMode]){

			auto& List = RankList[GameMode].List;

			for (size_t i(0), size(List.size()); i < size; i++)
				res += "<tr><td>" + std::to_string(i + 1) + "</td><td><a href=\"/u/" + std::to_string(List[i].ID) + "\">" + GetUsernameFromCache(List[i].ID) + "</a></td><td>" + std::to_string(List[i].PP) + "</td></tr>";
		}
		res += "</table>";

		render_page(res, s);
	}

	void render_login(_Con s){

		const std::string res = "<form action=\"login\" method=\"POST\">Username: <input type=\"text\" name=\"FirstName\"><br>Password: <input type=\"text\" name=\"LastName\"><br><input type=\"submit\" value=\"Submit\"></form>";

		render_page(res, s);
	}


	void web_login(const _HttpRes& res, _Con s){


		
		if (res.Body.size() <= 5) {//If sufficient post data is not provided then render the login page.
		FailedLogin:
			return render_login(s);
		}

		/*
		0 - Username
		1 - Password Hash
		*/

		const _GetParams Params(res.Body);

		const auto Username = Params.get<WSTI("username")>();
		const auto Password = Params.get<WSTI("password")>();

		if (!Username.size() || !Password.size())
			goto FailedLogin;

		//const auto& Data = Explode_View(res.Body, '\n', 2);


		//std::string LoginButton;


		render_page("<meta http-equiv=\"Refresh\" content=\"0; url = select\" />", s);

	}


	void handle_web(const _HttpRes &res, _Con s){

		const auto Temp = Explode_View(std::string_view((const char* const)(res.Host.data()+1),res.Host.size()), '/', 4);

		if (Temp.size() > 1){
			if (Temp[1] == "login")
				return web_login(res, s);
			if (Temp[1] == "leaderboard")
				return Leaderboard((Temp.size() > 2 ? StringToNum(u32,Temp[2]) : 0),s);
		}

		return render_page("<centre><br><h1>404</h1><br>Page not found<br><br></centre>",s);
	}

}