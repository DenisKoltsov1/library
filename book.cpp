#include "book.h"

#include <iostream>
#include "connect.h"


Book::Book(std::string name_, unsigned short age_, std::string author_) : name(name_), age(age_), author(author_) { value++; }


Book& Book::open_book(Book& object)
{
	
		// Логика открытия
		// Логика: нажатие клавиши и открытие
		std::cout << "Книга открыта" << std::endl;
		return object;

}

Book& Book:: take_book(Book& object) {
	// Логика взятия
	// Логика: нажатие клавиши и открытие
	std::cout << "Книга взята" << std::endl;
	return object;
	}

int Book::value = 1;
int Book::getCount() {
	return value;
}



int Book::countCallback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " ";
    }
    std::cout << std::endl;
    return 0;
}

// В файле connect.cpp
std::vector<Book> Book::showBook(sqlite3* db) {
    std::vector<Book> books;
    // ВАЖНО: имена колонок и таблицы как в твоем CREATE TABLE
    const char* sql = "SELECT NAME, AGE, AUTHOR FROM BOOKS;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Индекс 0 = NAME (Text)
            const char* c_name = (const char*)sqlite3_column_text(stmt, 0);
            std::string name = c_name ? c_name : "Unknown";

            // Индекс 1 = AGE (Int)
            int year = sqlite3_column_int(stmt, 1);

            // Индекс 2 = AUTHOR (Text)
            const char* c_author = (const char*)sqlite3_column_text(stmt, 2);
            std::string author = c_author ? c_author : "Unknown";

            // Добавляем в вектор
            books.push_back(Book(name, year, author));
        }
    }
    else {
        std::cerr << "SQL Error: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return books;
}

void Book::draw(int x, int y, Color color) {
    // 1. Учитываем "подпрыгивание" при выборе прямо здесь!
    int drawY = isSelected ? y + 15 : y;

    // 2. Рисуем саму обложку
    DrawRectangle(x, drawY, 65, 110, color);
    DrawRectangleLines(x, drawY, 65, 110, BLACK);

    // 3. Логика обрезки текста (теперь она скрыта от main)
    std::string label = (name.length() >= 5) ? name.substr(0, 5) : name;

    // 4. Текст на книге
    DrawText(label.c_str(), x + 5, drawY + 45, 12, BLACK);

    // 5. Если книга выбрана, можно добавить рамку потолще или значок
    if (isSelected) {
        DrawRectangleLinesEx({ (float)x - 2, (float)drawY - 2, 69, 114 }, 2, GOLD);
    }
}
