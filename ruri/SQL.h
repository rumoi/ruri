#pragma once

#pragma comment (lib, "mysqlcppconn.lib")

#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

__forceinline void PrepareSQLString(char* s){

	DWORD O = 0;
	while(s[O] != 0){
		if (s[O] == '\"' || s[O] == '\'')
			s[O] = '_';
		O++;
	}
}


#define REMOVEQUOTES(STR)\
	[](std::string &const s)->const std::string{\
		const size_t l = s.size();\
		for(size_t i = 0; i < l ; i++)\
			if(s[i] == '\'')\
				s[i] = '_';\
		return s;\
	}(STR)

#define USERNAMESAFE(STR)[](std::string &const s)->std::string{\
		const size_t l = s.size();\
		for(size_t i = 0; i < l ; i++){\
			if(s[i] == ' ')s[i] = '_';\
			else if (s[i] >= 'A' && s[i] <= 'Z')\
				s[i] += 'a' - 'A';\
		}\
		return s;\
}(STR)

/*
__forceinline void PrepareSQLString(std::string &s){
	for (DWORD i = 0; i < s.size(); i++)
		if (s[i] == '\"' || s[i] == '\'')
			s[i] = '_';
}*/

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

	sql::ResultSet *ExecuteQuery(const std::string Query, const bool DontLock = 0){
		if (!DontLock)Lock.lock();

		sql::ResultSet *ret = 0;
		sql::Statement* s = 0;

		try {

			CheckIsValid();
			s = con->createStatement();

			if (!s) {
				if (!DontLock)Lock.unlock();
				LastMessage = clock();
				return 0;
			}
			ret = s->executeQuery(Query);

		}catch (sql::SQLException &e) {
			printf("SQLERROR:%i\n", e.getErrorCode());
		}
		if(s)delete s;


		if (!DontLock)Lock.unlock();
		LastMessage = clock();
		return ret;
	}
	bool ExecuteSQL(const std::string Query){

		Lock.lock();

		bool ret = 0;
		sql::Statement* s = 0;

		try {
			CheckIsValid();
			s = con->createStatement();
			if (!s) {
				Lock.unlock();
				LastMessage = clock();
				return 0;
			}
			ret = s->execute(Query);

		}
		catch (sql::SQLException &e) {
			printf("SQLERROR:%i\n", e.getErrorCode());
		}
		if (s)delete s;

		Lock.unlock();
		LastMessage = clock();

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
				LastMessage = clock();
				return 0;
			}
			ret = s->executeUpdate(Query);

		}
		catch (sql::SQLException &e) {
			printf("SQLERROR:%i (%s)\n", e.getErrorCode(), Query.c_str());
		}
		delete s;

		if (!DontLock)Lock.unlock();

		LastMessage = clock();
		return ret;
	}

	bool Connect(){
		Lock.lock();

		bool Suc = 1;

		try {
			driver = get_driver_instance();

			if (!driver) {
				Lock.unlock();
				return 0;
			}
			con = driver->connect("localhost", SQL_Username, SQL_Password);
			if (!con) {
				Lock.unlock();
				return 0;
			}
			con->setSchema(SQL_Schema);
		}catch (...) {
			Suc = 0;
		}
		LastMessage = clock();

		Lock.unlock();
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
	}

};