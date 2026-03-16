#include "connect.h"
#include <iostream>
#include <string>

// 1. Конструктор: открывает базу и создает таблицу
ConnectDB::ConnectDB() : db(nullptr) {
    // Вместо "test.db" напиши полный путь:
    int result = sqlite3_open("C:/book/book/test.db", &db);


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
    // ... твой текущий код (конструктор и деструктор) остается выше ...

// 3. Реализация метода вставки данных
    void ConnectDB::insertBook(const std::string & name, const std::string & author, int age) {
        // Используем sqlite3_mprintf — это безопасный способ собрать SQL-запрос.
        // %Q сам добавит кавычки и защитит от ошибок, если в названии книги есть апостроф.
        char* sql = sqlite3_mprintf(
            "INSERT INTO BOOKS (NAME, AUTHOR, AGE, IS_SELECTED) VALUES (%Q, %Q, %d, 0);",
            name.c_str(), author.c_str(), age
        );

        char* errorMessage = nullptr;

        // Выполняем запрос
        int result = sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage);

        if (result != SQLITE_OK) {
            std::cerr << "Ошибка записи в БД: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            std::cout << "БАЗА: Книга '" << name << "' успешно сохранена!" << std::endl;
        }

        // Обязательно освобождаем память, выделенную mprintf
        sqlite3_free(sql);
    }


