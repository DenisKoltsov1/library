#pragma once // Важно! Защита от двойного подключения
#include <filesystem>
#include <string>
#include "connect.h" // Подключаем твой класс базы

class ParsingBook {
private:
    std::filesystem::path bookPath;
    std::string title;
    std::string author;
    int year;
public:
    ParsingBook(std::filesystem::path p);
    void parse(ConnectDB& database); // Передаем базу по ссылке
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    int getYear() const { return year; }
};
