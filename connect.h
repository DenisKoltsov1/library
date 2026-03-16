#pragma once
#include "sqlite3.h"
#include <string>

			class ConnectDB {
			private:
				sqlite3* db;

			public:
				void insertBook(const std::string& name, const std::string& author, int age);

				ConnectDB();  // Только заголовок конструктора
				~ConnectDB(); // Только заголовок деструктора
			};
