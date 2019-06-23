#pragma once


#define MNL "\r\n"
#define DMNL "\r\n\r\n"


struct _HttpHeader{

	const std::string Text;
	const std::string Value;

	_HttpHeader(const std::string &&Text,const std::string &&Value): Text(_M(Text)),Value(_M(Value)){}
};

struct _HttpRes {

	const std::vector<byte> Body;
	const std::vector<byte> Host;
	const std::vector<_HttpHeader> Headers;

	const std::string GetHeaderValue(const std::string &&Name)const {

		for (DWORD i = 0; i < Headers.size(); i++)
			if (Headers[i].Text == Name)
				return Headers[i].Value;

		return "";
	}

	_HttpRes(const std::vector<byte> &&Host, const std::vector<_HttpHeader>&& Headers, const std::vector<byte> &&Body) :Host(_M(Host)), Headers(_M(Headers)), Body(_M(Body)) {}
	_HttpRes(){}
	//_HttpRes(_HttpRes&& a) : Body(_M(a.Body)), Host(_M(a.Host)), Headers(_M(a.Headers)) {}

};

const std::string ConstructResponse(const DWORD Code, const std::vector<_HttpHeader> &Headers,const std::vector<byte> &Body){

	std::string Return = [=]{

		if(Code == 200)return "HTTP/1.0 200 OK" MNL;
		if (Code == 404)return "HTTP/1.0 404 Not Found" MNL;
		if (Code == 405)return "HTTP/1.0 405 Method Not Allowed" MNL;
		if (Code == 408)return "HTTP/1.0 408 Request Timeout" MNL;
		
		return "HTTP/1.0 200 OK" MNL;
	}();

	for (const auto& H : Headers)
		Return += H.Text + ": " + H.Value + MNL;
	

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

	const _HttpRes RecvData(bool &Suc)const {
		Suc = 0;
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
			return _HttpRes();

		const auto DATA = EXPLODE(VEC(byte),&p[0], pSize, '\r');

		if (!DATA.size() || !DATA[0].size())
			return _HttpRes();

		const auto PageName = EXPLODE_VEC(VEC(byte), DATA[0], ' ');
		
		int CurrentOffset = 1;

		std::vector<_HttpHeader> Headers; Headers.reserve(32);
		std::vector<byte> Body;

		{//Headers
			for (DWORD i = CurrentOffset; i < DATA.size(); i++) {

				CurrentOffset = i;

				if (DATA[i].size() <= 1)
					break;

				const auto Head = EXPLODE(std::string, &DATA[i][1], DATA[i].size()-1,':');

				if (Head.size() < 2 || !Head[0].size() || Head[1].size() <= 1)continue;

				Headers.push_back(_HttpHeader(_M(Head[0]), Head[1].substr(1,Head[1].size()-1)));
			}
		}

		{//Body
			
			bool FirstBody = 1;

			for (DWORD i = CurrentOffset + 1; i < DATA.size(); i++){

				if(FirstBody){
					FirstBody = 0;
					Body = std::vector<byte>(DATA[i].begin() + 1, DATA[i].end());
				}
				else{
					Body.resize(Body.size() + DATA[i].size() + 1);
					memcpy(&Body[Body.size() - DATA[i].size()], &DATA[i][0], DATA[i].size());
					Body[Body.size() - (DATA[i].size() + 1)] = '\r';
				}
			}
		}
		Suc = 1;
		return _HttpRes((PageName.size() > 1) ? _M(PageName[1]) : std::vector<byte>{}, _M(Headers), _M(Body));
	}

	bool SendData(const std::string &&Data) {;
		if (!s)return 0;

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