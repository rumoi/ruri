#pragma once


#define MNL "\r\n"
#define DMNL "\r\n\r\n"


struct _HttpHeader{
	char Text[64];
	char Value[64];

	_HttpHeader(){
		ZeroMemory(Text, 64);
		ZeroMemory(Value, 64);
	}
	void Clear(){
		ZeroMemory(Text, 64);
		ZeroMemory(Value, 64);
	}

	_HttpHeader(const char* t, const char* v){
		const int tRS = strlen(t);
		const int vRS = strlen(v);

		memcpy(Text, t, tRS + 1);
		memcpy(Value, v, vRS + 1);
	}

};
struct _HttpRes {
	_HttpHeader H[32];//32 should be more than enough
	std::vector<byte> Body;
	std::vector<byte> Host;

	DWORD GetHeaderOffset(const char* v, const char I = 0)const{
		for (DWORD i = 0; i < 32; i++){

			if (H[i].Text[0] == 0)break;

			if (I) {
				if (!_strcmpi(v, H[i].Text))return i;
			}else if (!strcmp(v, H[i].Text))return i;
		}
		return INT_MAX;
	}

	char* GetHeaderValue(const char* v, const char I = 0)const{
		DWORD O = GetHeaderOffset(v, I);

		if (O == INT_MAX)return "0";

		return (char*)H[O].Value;
	}

	void AddHeader(char* t, char* v){
		for (DWORD i = 0; i < 32; i++){
			if (H[i].Text[0] == 0){
				H[i] = _HttpHeader(t, v);
				return;
			}
		}
	}

};


std::string ConstructResponse(std::string Response, const std::vector<_HttpHeader> &Headers,const std::vector<byte> &Body){

	std::string Return = Response + MNL;
	
	for (DWORD i = 0; i < Headers.size(); i++){
		Return += Headers[i].Text;
		Return += ": ";
		Return += Headers[i].Value;
		Return += MNL;
	}

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

		DWORD pSize = 0;
		std::vector<byte> p;
		p.reserve(USHORT(-1));
		int pLength = MAX_PACKET_LENGTH;
		do{
			p.resize(pSize + MAX_PACKET_LENGTH);

			pLength = recv(s, (char*)&p[pSize], MAX_PACKET_LENGTH, 0);

			if (pLength == SOCKET_ERROR)break;
			pSize += pLength;

		} while (pLength == MAX_PACKET_LENGTH);

		

		if (pSize == 0)return 0;

		auto temp = Explode(&p[0], pSize, '\r');

		if (!temp.size() || !temp[0].size())return 0;

		{
			auto PageName = Explode(&temp[0][0], temp[0].size(), ' ');
			if (PageName.size() > 1){
				res.Host = PageName[1];
			}


		}

		int CurrentOffset = 1;

		{//Headers
			for (DWORD i = CurrentOffset; i < temp.size(); i++) {

				CurrentOffset = i;

				if (temp[i].size() <= 1)
					break;

				auto Head = Explode(&temp[i][1], temp[i].size()-1, ':', 1);

				if (Head.size() != 2)continue;

				if (Head[0].size() >= 64)Head[0].resize(63);
				if (Head[1].size() >= 64)Head[1].resize(63);

				Head[0].push_back(0);
				Head[1].push_back(0);

				res.AddHeader((char*)&Head[0][0], (char*)&Head[1][1]);

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

	bool SendData(std::string Data) {;
		if (!s)return 0;

		DWORD Count = 0;
		const DWORD Size = Data.size();
		while (Count < Data.size()){

			int ReadSize = Size - Count;

			if (ReadSize > MAX_PACKET_LENGTH)ReadSize = MAX_PACKET_LENGTH;

			if (send(s, (char*)&Data[Count], ReadSize, 0) == SOCKET_ERROR) {
				return 0;
			}

			Count += ReadSize;

		}
		return 1;
	}

	__inline void close() {

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