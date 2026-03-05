#pragma once
#include "sqlite3.h"

			class ConnectDB {
			private:
				sqlite3* db;

			public:
				ConnectDB();  // Только заголовок конструктора
				~ConnectDB(); // Только заголовок деструктора
			};
