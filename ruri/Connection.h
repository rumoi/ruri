#pragma once


#define MNL "\r\n"
#define DMNL "\r\n\r\n"


const std::string Empty_string = "";

struct _HttpRes {

	std::vector<byte> Body;
	std::vector<byte> Host;
	SS_TUPLE_VEC Headers;

	__inline const std::string& GetHeaderValue(const std::string &&Name)const{

		for (const auto& [Text, Value] : Headers)
			if (Text == Name)
				return Value;

		return Empty_string;
	}
	
	_HttpRes(){}
};

const std::string ConstructResponse(const DWORD Code, const SS_TUPLE_VEC&Headers,const std::vector<byte> &Body){

	std::string Return = [=]{

		if(Code == 200)return "HTTP/1.0 200 OK" MNL;
		if (Code == 404)return "HTTP/1.0 404 Not Found" MNL;
		if (Code == 405)return "HTTP/1.0 405 Method Not Allowed" MNL;
		if (Code == 408)return "HTTP/1.0 408 Request Timeout" MNL;
		
		return "HTTP/1.0 200 OK" MNL;
	}();

	for (const auto& [Text, Value] : Headers)
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

		int pLength = MAX_PACKET_LENGTH;
		do {
			p.resize(pSize + MAX_PACKET_LENGTH);

			pLength = recv(s, (char*)&p[pSize], MAX_PACKET_LENGTH, 0);

			if (pLength <= 0)break;

			pSize += pLength;

		} while (pLength == MAX_PACKET_LENGTH);

		if (pSize == 0)
			return 0;

		p.resize(pSize);

		const auto DATA = Explode_View(p,'\r',64);

		if (!DATA.size() || !DATA[0].size())
			return 0;


		{
			const auto PageName = Explode_View(DATA[0], ' ', 8);
			res.Host = (PageName.size() > 1) ? VEC(byte)(begin(PageName[1]), end(PageName[1])) : VEC(byte) {};
		}

		int CurrentOffset = 1;

		res.Headers.reserve(32);

		{//Headers
			for (DWORD i = CurrentOffset; i < DATA.size(); i++) {

				CurrentOffset = i;

				if (DATA[i].size() <= 1)
					break;

				const auto Head = Explode_View(DATA[i],':',2);

				if (Head.size() < 2 || Head[0].size() < 2 || Head[1].size() < 2)
					continue;

				res.Headers.push_back(
				TUPLE(std::string,std::string){
					Head[0].substr(1,Head[0].size() - 1),
					Head[1].substr(1,Head[1].size() - 1)
				});
			}
		}

		{//Body
			
			bool FirstBody = 1;

			for (DWORD i = CurrentOffset + 1; i < DATA.size(); i++){
				if(FirstBody){
					FirstBody = 0;
					res.Body = std::vector<byte>(DATA[i].begin() + 1, DATA[i].end());
				}
				else{
					if (!DATA[i].size()){
						res.Body.push_back('\r');
						continue;
					}
					res.Body.resize(res.Body.size() + DATA[i].size() + 1);
					res.Body[res.Body.size() - (DATA[i].size() + 1)] = '\r';
					memcpy(&res.Body[res.Body.size() - DATA[i].size()], &DATA[i][0], DATA[i].size());
				}
			}
		}

		return 1;
	}

	bool SendData(const std::string &&Data){

		if (!s)
			return 0;

		DWORD Count = 0;
		const DWORD Size = Data.size();
		while (Count < Data.size()){

			int SendSize = Size - Count;

			if (SendSize > MAX_PACKET_LENGTH)SendSize = MAX_PACKET_LENGTH;

			if (send(s, (char*)&Data[Count], SendSize, 0) == SOCKET_ERROR) {
				return 0;
			}

			Count += SendSize;

		}

		return 1;
	}

	void Dis(){
		if (!s)return;

		closesocket(s);
		s = 0;

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