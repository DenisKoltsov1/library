#pragma once
#include <iostream>
#include <vector>
#include <string>

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
};


