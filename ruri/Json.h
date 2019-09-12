#pragma once
#ifndef _H_JSON
#define _H_JSON

template<typename R = u32, typename T>
_inline constexpr R WeakStringToInt(const T& s) {

	static_assert(std::is_unsigned<R>::value,"WeakStringToInt is only allowed with unsigned types.");

	struct is_cString
		: public std::disjunction<
		std::is_same<char*, typename std::decay<T>::type>,
		std::is_same<const char*, typename std::decay<T>::type>
		> {};

	R Return = 0;

	#define H Return += R(s[i] ^ i) << ((i & (sizeof(R)-1)) << 3)

	if constexpr (is_cString::value)
		for (size_t i = 0; i < sizeof(T) - 1; i++) H;
	else for (size_t i = 0, Size(s.size()); i < Size; i++) H;

	#undef H

	return Return;
}

#define WSTI WeakStringToInt

namespace JSON{

	struct _JsonNode {

		enum class JsonType : u32 { Unknown, Int, Float, String,/* List, */Object, Continue };

		struct _JsonValue {
			u32 Name;
			JsonType Type;
			union {
				std::string_view Value_String;
				_JsonNode* Value_List;
			};

			~_JsonValue() {
				if ((Type == JsonType::Object || Type == JsonType::Continue) && Value_List)
					delete Value_List;

				Type = JsonType::Unknown;
				Name = 0;
				Value_List = 0;
			}

			_JsonValue() {
				Name = 0;
				Type = JsonType::Unknown;
				Value_List = 0;
			}
			_JsonValue(JsonType Type, std::string_view Name) : Type(Type), Name(WeakStringToInt(Name)), Value_List((_JsonNode*)0) {}
		};

		_JsonValue Values[64] = {};

		_JsonValue& emplace_back(JsonType Type, std::string_view Name) {

			constexpr size_t Size = sizeof(Values) / sizeof(Values[0]);

			auto& Back = Values[Size - 1];

			if (Back.Type == JsonType::Continue)
				return Back.Value_List->emplace_back(Type, Name);

			for (size_t i = 0; i < Size - 1; i++)
				if (Values[i].Type == JsonType::Unknown) {
					Values[i].Type = Type;
					Values[i].Name = WeakStringToInt(Name);
					return Values[i];
				}

			Back.Type = JsonType::Continue;
			Back.Value_List = new _JsonNode();

			return Back.Value_List->emplace_back(Type, Name);
		}

		_JsonValue& back() {

			constexpr size_t Size = sizeof(Values) / sizeof(Values[0]);

			auto& Back = Values[Size - 1];

			if (Back.Type == JsonType::Continue && Back.Value_List->Values[0].Type != JsonType::Unknown)
				return Back.Value_List->back();

			for (size_t i = 1; i < Size - 1; i++)
				if (Values[i].Type == JsonType::Unknown)
					return Values[i - 1];

			return Values[Size - 1];
		}

		void operator =(const _JsonNode& v) = delete;

		~_JsonNode() {

			constexpr size_t Size = sizeof(Values) / sizeof(Values[0]);

			if (Values[Size - 1].Type == JsonType::Continue)
				delete Values[Size - 1].Value_List;

			for (size_t i = 0; i < Size - 1; i++) {
				if (Values[i].Type == JsonType::Unknown)break;
				if (Values[i].Type == JsonType::Object && Values[i].Value_List)
					delete Values[i].Value_List;
			}

			ZeroMemory(this, sizeof(_JsonNode));
		}

	};

	[[nodiscard]] _JsonNode* ReadJson(const std::string_view Input) {

		_JsonNode* Alpha = new _JsonNode();

		std::vector<_JsonNode*> NodeTree(16, Alpha); NodeTree.resize(1);

		const auto SkipForward = [&Input](size_t& i) {
			for (; i < Input.size(); i++)
				if (const char c = Input[i]; c != ' ' && c != '\n' && c != '\r' && c != '	' && c != ':')
					return;
		};

		const auto ExtractString = [&Input](size_t& i, const bool Second = 0)->std::string_view {
			const size_t S = ++i;
			for (; i < Input.size(); i++)
				if (Input[i] == '\"' && Input[i - 1] != '\\')
					return std::string_view(Input.data() + S, (Second ? i : i++) - S);
			return std::string_view();
		};

		std::string_view ArrayName;

		for (size_t i = 1; i < Input.size(); i++) {

			switch (Input[i]) {

			case '{':case '[': {
			AddArray:
				auto& Node = NodeTree.back()->emplace_back(_JsonNode::JsonType::Object, ArrayName);
				NodeTree.emplace_back(Node.Value_List = new _JsonNode());
				ArrayName = std::string_view();
				break;
			}
			case ']':case '}':
				if (likely(NodeTree.size() > 1))
					NodeTree.pop_back();
				break;

			case '"': {

				ArrayName = ExtractString(i);

				SkipForward(i);

				if (Input[i] == '[')
					goto AddArray;

				std::string_view Value = Input[i] == '\"' ? ExtractString(i, 1) : [&Input, &NodeTree](size_t& i) {
					const size_t S = i;
					for (; i < Input.size(); i++) {

						if (const bool Closing = (Input[i] == '}' || Input[i] == ']'); Closing || Input[i] == ',') {
							if (Closing && likely(NodeTree.size() > 1))
								NodeTree.pop_back();

							return std::string_view(Input.data() + S, i - S);
						}
					}
					return std::string_view();
				}(i);

				NodeTree.back()->emplace_back(_JsonNode::JsonType::String, ArrayName).Value_String = Value;

				ArrayName = std::string_view();

			}
			default:break;
			}

		}

		return Alpha;
	}

	struct _Json {

		_JsonNode* Root = 0;
		_JsonNode* Current = 0;

		template<typename T>
		_Json(const T& Input) {
			Current = Root = ReadJson(std::string_view((const char*)Input.data(),Input.size()));
		}

		_Json(_JsonNode* NewHead) {
			Current = Root = NewHead;
		}

		~_Json() {
			if (Root)
				delete Root;
			Current = Root = 0;
		}

		template<u32 Name>
		_JsonNode* GetArray() {

			_JsonNode* Temp = Current;

			constexpr size_t Size = sizeof(_JsonNode::Values) / sizeof(_JsonNode::Values[0]);

		Search:if (!Temp)return 0;

			for (size_t i = 0; i < Size; i++) {

				const auto& Value = Temp->Values[i];

				switch (Value.Type) {

				case _JsonNode::JsonType::Object:
					if (Value.Name == Name)
						return Current = Temp->Values[i].Value_List;
					break;

				case _JsonNode::JsonType::Continue:
					Temp = Value.Value_List;
					goto Search;

				case _JsonNode::JsonType::Unknown:return 0;
				default: break;
				}

			}
			return 0;
		}

		template<u32 Name>
		std::string_view GetString() {

			_JsonNode* Temp = Current;

			constexpr size_t Size = sizeof(_JsonNode::Values) / sizeof(_JsonNode::Values[0]);

		Search:if (Temp) {
			for (size_t i = 0; i < Size; i++) {

				const auto& Value = Temp->Values[i];

				switch (Value.Type) {

				case _JsonNode::JsonType::String:
					if (Value.Name == Name)
						return Temp->Values[i].Value_String;
					break;

				case _JsonNode::JsonType::Continue:
					Temp = Value.Value_List;
					goto Search;

				case _JsonNode::JsonType::Unknown:return std::string_view();
				default: break;
				}

			}
		}
			return std::string_view();
		}

		void Reset() { Current = Root; }

	};

};

std::vector<std::vector<std::pair<int, std::string_view>>> JsonListSplit(const std::string_view Input, const size_t ExpectedSize) {

	std::vector<std::vector<std::pair<int, std::string_view>>> Return;
	Return.reserve(16);

	if (u32 BraceCount(0), NameHash(0); Input.size() > 2) {

		bool Body(0);
		const size_t iEnd = Input.size() - 1;
		for (size_t i = 1; i < iEnd; i++) {

			if (Input[i] == '{') {
				if (!BraceCount) {
					Body = 0;
					Return.emplace_back().reserve(ExpectedSize);
				}BraceCount++;
			}
			else if (Input[i] == '}') {
				if (BraceCount > 0)
					BraceCount--;
			}
			else if (BraceCount) {
				if (Input[i] == '\"') {
					const int startIndex = i + 1;
					for (; ++i < iEnd;) {
						if (Input[i] == '\"' && Input[i - 1] != '\\') {
							if (!Body)
								NameHash = WeakStringToInt(std::string_view((const char*)& Input[startIndex], i - startIndex));
							else {
								if (NameHash)
									Return.back().emplace_back(NameHash, std::string_view((const char*)& Input[startIndex], i - startIndex));
								NameHash = 0;
							}
							Body = !Body;
							break;
						}
					}
				}
				else if (Body && Input[i] == ',') {
					Body = 0;
				}
			}
		}
	}
	return Return;
}

#endif