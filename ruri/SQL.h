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
__forceinline void PrepareSQLString(std::string &s){
	for (DWORD i = 0; i < s.size(); i++)
		if (s[i] == '\"' || s[i] == '\'')
			s[i] = '_';
}

__forceinline void UserNameSafe(std::string &s){
	for (DWORD i = 0; i < s.size(); i++)
		if (s[i] == '\"' || s[i] == '\'' || s[i] == ' ')
			s[i] = '_';
		else if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] += 'a' - 'A';
}

std::string SQL_Password;

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

			con = driver->connect("localhost", "root", SQL_Password);
			con->setSchema("ripple");
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

	int ExecuteUPDATE(const std::string Query, const bool DontLock = 0) {
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
			printf("SQLERROR:%i\n", e.getErrorCode());
		}
		delete s;

		if (!DontLock)Lock.unlock();

		LastMessage = clock();
		return ret;
	}

	bool Connect(){
		Lock.lock();
		driver = get_driver_instance();

		if (!driver) {
			Lock.unlock();
			return 0;
		}
		con = driver->connect("localhost", "root", SQL_Password);
		if (!con) {
			Lock.unlock();
			return 0;
		}
		con->setSchema("ripple");

		LastMessage = clock();

		Lock.unlock();
		return 1;
	}

	void Disconnect() {
		Lock.lock();
		con->close();
		delete con;
	}


	_SQLCon() {
		driver = 0;
		con = 0;
	}

};