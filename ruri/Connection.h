#pragma once

#define MNL "\r\n"
#define DMNL "\r\n\r\n"

struct _HttpRes{

	std::vector<byte> Raw;
	std::string_view Body;
	std::string_view Host;

	std::vector<std::pair<std::string_view,std::string_view>> Headers;

	__inline const std::string_view GetHeaderValue(const std::string &&Name)const{

		for (const auto& [Text, Value] : Headers)
			if (Text == Name)
				return Value;

		return std::string_view();
	}
	
	_HttpRes(){}
};

template<typename T>
	const std::string (const DWORD Code, const std::vector<std::pair<std::string, std::string>> &Headers, const T& Body) {

		std::string Return = [=]{

			if(Code == 200)return "HTTP/1.0 200 OK" MNL;
			if (Code == 404)return "HTTP/1.0 404 Not Found" MNL;
			if (Code == 405)return "HTTP/1.0 405 Method Not Allowed" MNL;
			if (Code == 408)return "HTTP/1.0 408 Request Timeout" MNL;
		
			return "HTTP/1.0 200 OK" MNL;
		}(); Return.reserve(48 + Return.size() + Body.size());

		for (auto& [Text, Value] : Headers)
			Return += Text + ": " + Value + MNL;

		Return += "Content-Length: " + std::to_string(Body.size()) + MNL + "Connection: close" + DMNL;

		if (Body.size()){
			Return.resize(Return.size() + Body.size());
			memcpy(&Return[Return.size() - Body.size()], Body.data(), Body.size());
		}

		return Return;
	}

struct _Con{

	SOCKET s;
	DWORD ID;

	const bool RecvData(_HttpRes &res)const {
		
		DWORD pSize = 0;
		int pLength = MAX_PACKET_LENGTH;

		res.Raw.reserve(USHORT(-1));

		do{
			res.Raw.resize(pSize + MAX_PACKET_LENGTH);

			pLength = recv(s, (char*)&res.Raw[pSize], MAX_PACKET_LENGTH, 0);

			if (pLength <= 0)break;

			pSize += pLength;

		} while (pLength == MAX_PACKET_LENGTH);

		if (pSize == 0)
			return 0;

		res.Raw.resize(pSize);

		const auto DATA = Explode_View_Multi(res.Raw, "\r\n", 64);

		if (!DATA.size() || !DATA[0].size())
			return 0;

		if(const auto PageName = Explode_View(DATA[0], ' ', 8); PageName.size() > 1 && PageName[1].size())
			res.Host = std::string_view((const char*)&PageName[1][0], PageName[1].size());

		int CurrentOffset = 1;

		res.Headers.reserve(32);

		//Headers
		for (DWORD i = CurrentOffset; i < DATA.size(); i++){

			if (CurrentOffset = i; DATA[i].size() <= 1)
				break;
			
			if (const auto Head = Explode_View(DATA[i], ':', 2);
				Head.size() < 2 || !Head[0].size() || !Head[1].size())
				continue;
			else
				res.Headers.push_back({
					Head[0],
					Head[1]
				});
		}

		if (++CurrentOffset < DATA.size() && !DATA[CurrentOffset - 1].size())
			res.Body = std::string_view(DATA[CurrentOffset].data(), size_t(res.Raw.data() + res.Raw.size()) - size_t(DATA[CurrentOffset].data()));

		return 1;
	}

	bool SendRawData(const void* vData, const DWORD Size){

		const char* Data = (const char*)vData;

		if (DWORD Count = 0; likely(s)){

			while (Count < Size)
				if (int Sent = send(s, (char*)&Data[Count], al_min(Size - Count, MAX_PACKET_LENGTH), 0);
					Sent != SOCKET_ERROR)
					Count += Sent;
				else
					return 0;

			return 1;
		}
		return 0;
	}

template<typename T>
	bool SendData(const T& Data){

		if (DWORD Count = 0; likely(s)){

			const DWORD Size = Data.size();

			while (Count < Size){
				if (int Sent = send(s, (const char*)&Data[Count], al_min(Size - Count, MAX_PACKET_LENGTH), 0);
					Sent != SOCKET_ERROR)
					Count += Sent;
				else break;
			}

			return 1;
		}

		return 0;
	}

	void Dis(){
		if (s){
			closesocket(s); s = 0;
		}
	}

	_Con(SOCKET S) {
		s = S;
		ID = 0;
	}
	_Con(SOCKET S, DWORD I) {
		s = S;
		ID = I;
	}
	_Con() {
		s = 0;
		ID = 0;
	}
};