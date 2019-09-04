#pragma once

#pragma comment (lib, "mysqlcppconn.lib")

#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

_inline void PrepareSQLString(char* s){

	DWORD O = 0;
	while(s[O] != 0){
		if (s[O] == '\"' || s[O] == '\'')
			s[O] = '_';
		O++;
	}
}

#define DO_REMOVEQUOTES(s) for (char& c : s) { if (c == '\'')c = '_'; }

#define DO_USERNAMESAFE(s)\
	for (char& c : s){\
		if (c == ' ')\
			c = '_';\
		else if(c >= 'A' && c <= 'Z')\
			c += 'a' - 'A';\
	}

_inline std::string USERNAMESQL(std::string s){
	DO_REMOVEQUOTES(s);
	DO_USERNAMESAFE(s)
	return s;
}

_inline std::string USERNAMESQL_Ref(std::string&& s) {
	DO_REMOVEQUOTES(s);
	DO_USERNAMESAFE(s)
	return s;
}

_inline std::string USERNAMESQL(std::string_view s){
	return USERNAMESQL_Ref(std::string(IT_COPY(s)));
}

_inline std::string REMOVEQUOTES(std::string s){
	DO_REMOVEQUOTES(s);
	return s;
}

_inline std::string USERNAMESAFE(std::string s) {
	DO_USERNAMESAFE(s);
	return s;
}

//#define USERNAMESAFE(STR) [&]{std::string TempString(STR); DO_USERNAMESAFE(TempString); return TempString;}()
//#define REMOVEQUOTES(STR) [&]{std::string TempString(STR); DO_REMOVEQUOTES(TempString); return TempString;}()

std::string SQL_Password;
std::string SQL_Username;
std::string SQL_Schema;

struct _SQLCon {

	sql::Driver* driver;
	sql::Connection* con;

	int LastMessage;
	std::mutex Lock;

	void CheckIsValid(){
		if (!con || !driver || (!con->isValid() && !con->reconnect())) {

			printf("SQL has done the death.\n");

			if (con)delete con;
			if (!driver)driver = get_driver_instance();

			con = driver->connect("localhost", SQL_Username, SQL_Password);
			con->setSchema(SQL_Schema);
		}
	}

	sql::ResultSet *ExecuteQuery(const std::string& Query, const bool DontLock = 0){


		if (!DontLock)Lock.lock();

		sql::ResultSet *ret = 0;
		sql::Statement* s = 0;

		try {

			CheckIsValid();
			s = con->createStatement();

			if (!s) {
				if (!DontLock)Lock.unlock();
				LastMessage = clock_ms();
				return 0;
			}
			ret = s->executeQuery(_M(Query));

		}catch (sql::SQLException &e) {
			printf("SQLERROR:%i\n", e.getErrorCode());
		}
		if(s)delete s;


		if (!DontLock)Lock.unlock();
		LastMessage = clock_ms();
		return ret;
	}

	int ExecuteUPDATE(const std::string &Query, const bool DontLock = 0) {
		if(!DontLock)Lock.lock();

		int ret = 0;
		sql::Statement* s = 0;

		try {

			CheckIsValid();

			s = con->createStatement();

			if (!s) {
				if (!DontLock)Lock.unlock();
				LastMessage = clock_ms();
				return 0;
			}
			ret = s->executeUpdate(Query);

		}
		catch (sql::SQLException &e) {
			printf("SQLERROR:%i (%s)\n", e.getErrorCode(), Query.c_str());
		}
		delete s;

		if (!DontLock)Lock.unlock();

		LastMessage = clock_ms();
		return ret;
	}

	bool Connect(){

		std::scoped_lock<std::mutex> L(Lock);

		bool Suc = 1;

		try {
			driver = get_driver_instance();

			if (!driver){
				return 0;
			}
			con = driver->connect("localhost", SQL_Username, SQL_Password);
			if (!con)
				return 0;
			con->setSchema(SQL_Schema);
		}catch (...) {
			Suc = 0;
		}
		LastMessage = clock_ms();
		return Suc;
	}

	void Disconnect() {
		Lock.lock();
		if (con){
			con->close();
			delete con;
			con = 0;
		}
		driver = 0;//I seem to remember reading somewhere that the driver should not be freed by us.
		Lock.unlock();
	}

	_SQLCon() {
		driver = 0;
		con = 0;
		LastMessage = 0;
	}

};

struct _SQLKey{

	const std::string Key;
	std::string Value;
	const bool Text;

	template<typename T>
	_SQLKey(T&& Key, const std::string&& Value) : Key(std::forward<T>(Key)), Value(_M(Value)), Text(1) {}

	template<typename T>
	_SQLKey(T&& Key, const int64_t Value) : Key(std::forward<T>(Key)), Value(_M(std::to_string(Value))), Text(0) {}

};



const std::string SQL_INSERT(const std::string& Table, const VEC(_SQLKey)& Values) {
	return "INSERT INTO " + Table + " (" + [&] {
		std::string Return;
		for (const _SQLKey& v : Values)
			Return += v.Key + ",";
		if (Return.size())
			Return.pop_back();
		return Return;
	}() + ")VALUES(" + [&] {
		std::string Return;
		for (const _SQLKey& v : Values)
			if(v.Text)Return += "'" + v.Value + "',";
			else Return += v.Value + ",";
		if (Return.size())
			Return.pop_back();
		return Return;
	}() + ");";
}

const std::string SQL_SETUPDATE(const std::string& Table, const VEC(_SQLKey) && Values, const std::string& Condition) {
	return "UPDATE " + Table + " SET " + [&]{
		std::string Return;
		for (const _SQLKey& v : Values)
			if (v.Text)Return += v.Key + "='" + v.Value + "',";
			else Return += v.Key + "=" + v.Value+ ",";
		if (Return.size())
			Return.pop_back();
		return Return;
	}() + " WHERE " + Condition;
}