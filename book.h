#pragma once
#include <iostream>
#include <vector>
#include <string>


class Book {
private:
	unsigned short age;
	std::string name;
	std::string author;
	// Пример буфера, если он нужен внутри класса
	std::vector<int> buffer;

public:
	// Конструктор
	Book(std::string name_, unsigned short age_, std::string author_);


	// Метод возвращает ссылку на объект Book
	Book& open_book(Book& object);
	Book& take_book(Book& object);
};


