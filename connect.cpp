#include "connect.h"
#include <iostream>
#include <string>

// 1. Конструктор: открывает базу и создает таблицу
ConnectDB::ConnectDB() : db(nullptr) {
    int result = sqlite3_open("test.db", &db);

    if (result != SQLITE_OK) {
        std::cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        std::cout << "База успешно подключена!" << std::endl;

        const char* sql = "CREATE TABLE IF NOT EXISTS BOOKS ("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "NAME TEXT NOT NULL, "
            "AUTHOR TEXT, "
            "AGE INTEGER, "
            "IS_SELECTED INTEGER DEFAULT 0);";

        char* errorMessage = nullptr;
        int execResult = sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage);

        if (execResult != SQLITE_OK) {
            std::cerr << "Ошибка создания таблицы: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
    }
} // Скобка, закрывающая конструктор

// 2. Деструктор: закрывает соединение (ЕГО НЕ ХВАТАЛО)
ConnectDB::~ConnectDB() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Соединение с БД закрыто." << std::endl;
    }
}
