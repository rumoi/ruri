#pragma once


#define MNL "\r\n"
#define DMNL "\r\n\r\n"


struct _HttpHeader{

	const std::string Text;
	const std::string Value;

	_HttpHeader(const std::string &Text,const std::string &Value): Text(Text),Value(Value){};

};

struct _HttpRes {
	std::vector<byte> Body;
	std::vector<byte> Host;
	std::vector<_HttpHeader> Headers;

	const std::string GetHeaderValue(const std::string Name){

		

		for (DWORD i = 0; i < Headers.size(); i++)
			if (Headers[i].Text == Name)
				return Headers[i].Value;

		return "";
	}

};

const std::string ConstructResponse(const DWORD Code, const std::vector<_HttpHeader> &Headers,const std::vector<byte> &Body){

	std::string Return = [=]{

		if(Code == 200)return "HTTP/1.0 200 OK" MNL;
		if (Code == 404)return "HTTP/1.0 404 Not Found" MNL;
		if (Code == 405)return "HTTP/1.0 405 Method Not Allowed" MNL;
		if (Code == 408)return "HTTP/1.0 408 Request Timeout" MNL;
		
		return "HTTP/1.0 200 OK" MNL;
	}();

	for (DWORD i = 0; i < Headers.size(); i++)
		Return += Headers[i].Text + ": " + Headers[i].Value + MNL;
	

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

	bool RecvData(_HttpRes &res) {

		if (!s)return 0;

		res = _HttpRes();

		res.Headers.reserve(32);

		DWORD pSize = 0;
		std::vector<byte> p;
		p.reserve(USHORT(-1));

		std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();

		int pLength = MAX_PACKET_LENGTH;
		do{
			p.resize(pSize + MAX_PACKET_LENGTH);

			pLength = recv(s, (char*)&p[pSize], MAX_PACKET_LENGTH, 0);

			if (pLength == SOCKET_ERROR)break;
			pSize += pLength;

		} while (pLength == MAX_PACKET_LENGTH);

		const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
		printf("recv %fms\n", double(double(TTime) / 1000000.0));

		if (pSize == 0)return 0;

		const auto temp = EXPLODE(VEC(byte),&p[0], pSize, '\r');
		
		if (!temp.size() || !temp[0].size())return 0;

		{
			const auto PageName = EXPLODE_VEC(VEC(byte), temp[0], ' ');

			if (PageName.size() > 1)
				res.Host = std::move(PageName[1]);
			
		}

		int CurrentOffset = 1;

		{//Headers
			for (DWORD i = CurrentOffset; i < temp.size(); i++) {

				CurrentOffset = i;

				if (temp[i].size() <= 1)
					break;

				const auto Head = EXPLODE(std::string,&temp[i][1],temp[i].size()-1,':');

				if (Head.size() < 2 || !Head[0].size() || Head[1].size() <= 1)continue;

				res.Headers.push_back(_HttpHeader(std::move(Head[0]), std::move(Head[1].substr(1,Head[1].size()-1))));
			}
		}

		{//Body
			
			bool FirstBody = 1;

			for (DWORD i = CurrentOffset + 1; i < temp.size(); i++){

				if(FirstBody){
					FirstBody = 0;
					res.Body = std::vector<byte>(temp[i].begin() + 1, temp[i].end());
				}
				else {
					res.Body.push_back('\r');
					res.Body.resize(res.Body.size() + temp[i].size());
					memcpy(&res.Body[res.Body.size() - temp[i].size()], &temp[i][0], temp[i].size());
				}
			}

		}
		
		return 1;
	}

	bool SendData(const std::string &Data) {;
		if (!s)return 0;


		std::chrono::steady_clock::time_point sTime = std::chrono::steady_clock::now();

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

		const unsigned long long TTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::steady_clock::now() - sTime).count();
		printf("Send%fms\n", double(double(TTime) / 1000000.0));

		return 1;
	}

	void Dis() {

		if (!s)return;
		//printf("Closing Connection\n");
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