#pragma once

#define MNL "\r\n"
#define DMNL "\r\n\r\n"

struct _HttpRes{

	std::vector<byte> Raw;
	std::string_view Body;
	std::string_view Host;

	std::vector<std::pair<std::string_view,std::string_view>> Headers;

	__inline const std::string_view GetHeaderValue(const std::string& Name)const noexcept{

		for (const auto& [Text, Value] : Headers)
			if (Text == Name)
				return Value;

		return std::string_view{};
	}
	
	_HttpRes(){}
};

template<typename T>
	const std::string ConstructResponse(const DWORD Code, const std::vector<std::pair<std::string, std::string>> &Headers, const T& Body) {

		std::string Return{
			[=]{
				switch (Code) {
					case 200:
					default:
						return "HTTP/1.0 200 OK" MNL;

					case 404:
						return "HTTP/1.0 404 Not Found" MNL;

					case 405:
						return "HTTP/1.0 405 Method Not Allowed" MNL;

					case 408:						
						return "HTTP/1.0 408 Request Timeout" MNL;
				}
			}()
		};
		
		Return.reserve(48 + Return.size() + Body.size());

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
		
		size_t pSize = 0;

		res.Raw.reserve(USHORT(-1));

		for (;;) {

			res.Raw.resize(pSize + MAX_PACKET_LENGTH);

			const auto pLength = recv(s, (char*)res.Raw.data() + pSize, MAX_PACKET_LENGTH, 0);

			if (pLength <= 0)
				break;

			pSize += pLength;

			if (pLength != MAX_PACKET_LENGTH)
				break;
		}

		if (pSize == 0)
			return 0;

		res.Raw.resize(pSize);

		const auto DATA = Explode_View_Multi(res.Raw, "\r\n", 64);

		if (!DATA.size() || !DATA[0].size())
			return 0;

		if(const auto PageName = Explode_View(DATA[0], ' ', 8); PageName.size() > 1)
			res.Host = std::string_view((const char*)PageName[1].data(), PageName[1].size());

		size_t Offset = 1;

		{//Headers

			res.Headers.reserve(32);

			for (; Offset < DATA.size(); Offset++){

				if (DATA[Offset].size() <= 1)
					break;

				if (const auto Head = Explode_View(DATA[Offset], ':', 2); Head.size() < 2 || !Head[0].size() || !Head[1].size())
					continue;
				else
					res.Headers.emplace_back(Head[0], Head[1]);
			}
		}

		if (++Offset < DATA.size() && !DATA[Offset - 1].size())
			res.Body = std::string_view(DATA[Offset].data(), size_t(res.Raw.data() + res.Raw.size()) - size_t(DATA[Offset].data()));

		return 1;
	}

	bool SendRawData(const void* Data, const size_t Size) noexcept{
		
		if (unlikely(!s))
			return 0;

		size_t Count = 0;

		while (Count < Size){

			const auto Sent = send(s, (const char*)Data + Count, al_min(Size - Count, MAX_PACKET_LENGTH), 0);

			if (unlikely(Sent == SOCKET_ERROR))
				return 0;

			Count += Sent;
		}

		return 1;
	}

template<typename T>
	bool SendData(const T& Data) noexcept{

		if (unlikely(!s))
			return 0;

		size_t Count = 0; const size_t Size = Data.size();

		const char* const DataStart = (char*)Data.data();

		while (Count < Size){

			const auto Sent = send(s, DataStart + Count, al_min(Size - Count, MAX_PACKET_LENGTH), 0);

			if (Sent == SOCKET_ERROR)
				return 0;

			Count += Sent;
		}

		return 1;
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