#pragma once
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <string>
#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"


class Book {
private:
    // Оставляем здесь только то, что реально должно быть скрыто
    std::vector<int> buffer;
    static int value;

public:
    // ПЕРЕНЕСЛИ СЮДА, чтобы main.cpp мог их читать:
    unsigned short age;
    std::string name;
    std::string author;
    bool isSelected = false;

    // Конструктор
    Book(std::string name_, unsigned short age_, std::string author_);

    // Методы
    Book& open_book(Book& object);
    Book& take_book(Book& object);
    static int getCount();
    void setCount(int n) { value = n; }
    // Добавь в public:
    void draw(int x, int y, Color color);

    std::vector<Book> showBook(sqlite3* db);
    static int countCallback(void* data, int argc, char** argv, char** azColName);
};


