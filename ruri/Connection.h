#pragma once


#define MNL "\r\n"
#define DMNL "\r\n\r\n"


const std::string Empty_string = "";

struct _HttpRes{

	std::vector<byte> Body;
	std::vector<byte> Host;
	SS_PAIR_VEC Headers;

	__inline const std::string& GetHeaderValue(const std::string &&Name)const{

		for (const auto& [Text, Value] : Headers)
			if (Text == Name)
				return Value;

		return Empty_string;
	}
	
	_HttpRes(){}
};


template<typename T>
	const std::string ConstructResponse(const DWORD Code, const std::vector<std::pair<std::string, std::string>> &Headers, const T& Body) {

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
			memcpy(&Return[Return.size() - Body.size()], &Body[0], Body.size());
		}

		return Return;
	}

struct _Con{

	SOCKET s;
	DWORD ID;

	const bool RecvData(_HttpRes &res)const{

		DWORD pSize = 0;
		std::vector<byte> p;
		p.reserve(USHORT(-1));

		while (1){
			p.resize(MAX_PACKET_LENGTH + pSize);
			
			if (int pLength = recv(s, (char*)&p[pSize], MAX_PACKET_LENGTH, 0);
				pLength > 0)
				pSize += pLength;
			else break;
		}

		if (!pSize)
			return 0;

		p.resize(pSize);

		const auto DATA = Explode_View(p,'\r',64);

		if (!DATA.size() || !DATA[0].size())
			return 0;

		if(const auto PageName = Explode_View(DATA[0], ' ', 8); PageName.size() > 1)
			res.Host = VEC(byte)(begin(PageName[1]), end(PageName[1]));

		int CurrentOffset = 1;

		res.Headers.reserve(32);

		//Headers
		for (DWORD i = CurrentOffset; i < DATA.size(); i++){

			if (CurrentOffset = i; DATA[i].size() <= 1)
				break;
			
			if (const auto Head = Explode_View(DATA[i], ':', 2);
				Head.size() < 2 || Head[0].size() < 2 || Head[1].size() < 2)
				continue;
			else
				res.Headers.push_back({
					std::string(Head[0].substr(1, Head[0].size() - 1)),
					std::string(Head[1].substr(1, Head[1].size() - 1))
				});
		}

		//Body
		{

			{
				int TotalSize = 0;
				CurrentOffset++;

				for (DWORD i = CurrentOffset; i < DATA.size(); i++)
					TotalSize += DATA[i].size() + 1;

				res.Body.reserve(TotalSize);

				if (DWORD i = CurrentOffset; i < DATA.size()){
					if (size_t Size = DATA[i].size() - 1; DATA[i].size()) {
						res.Body.resize(Size);
						memcpy(res.Body.data(), &DATA[1], Size);
					}
				}

				CurrentOffset++;
			}

			for (DWORD i = CurrentOffset; i < DATA.size(); i++){
				if (unlikely(!DATA[i].size())){
					res.Body.push_back('\r');
					continue;
				}
				res.Body.resize(res.Body.size() + DATA[i].size() + 1);
				res.Body[res.Body.size() - (DATA[i].size() + 1)] = '\r';
				memcpy(&res.Body[res.Body.size() - DATA[i].size()], DATA[i].data(), DATA[i].size());
			}
		}

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
	bool SendData(const T &Data){

		if (DWORD Count = 0; likely(s)){

			const DWORD Size = Data.size();

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