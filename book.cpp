#include "book.h"
#include <iostream>
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