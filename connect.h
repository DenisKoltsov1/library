#pragma once
#include "sqlite3.h"
#include <string>
#include "book.h"

			class ConnectDB {
			private:
				sqlite3* db;

			public:
				void insertBook(const std::string& name, const std::string& author, int age);
				std::vector<Book> showBook();
				static int countCallback(void* data, int argc, char** argv, char** azColName);

				ConnectDB();  // Только заголовок конструктора
				~ConnectDB(); // Только заголовок деструктора
			};
