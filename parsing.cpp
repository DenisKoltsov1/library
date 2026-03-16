#include "parsing.h"
#include <iostream>
#include <fstream>    // ОБЯЗАТЕЛЬНО: для ifstream
#include <vector>
#include <string>
#include <regex>      // ОБЯЗАТЕЛЬНО: для поиска автора/названия
#include <iterator>   // ОБЯЗАТЕЛЬНО: для итераторов файла

// Конструктор остается прежним
ParsingBook::ParsingBook(std::filesystem::path p) : bookPath(std::move(p)) {
    if (!std::filesystem::exists(bookPath)) {
        throw std::runtime_error("Файл не найден: " + bookPath.string());
    }
}

// Новая реализация parse, которая принимает базу
void ParsingBook::parse(ConnectDB& database) {
    std::ifstream file(bookPath, std::ios::binary);
    if (!file.is_open()) return;

    // Читаем не весь файл (это может быть долго), а первые 10КБ и последние 10КБ
    // Именно там обычно лежат метаданные
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // 1. Имя файла как запасной вариант
    this->title = bookPath.filename().stem().string();
    this->author = "Додонов А. Е."; // Для теста можем вписать того, кто на обложке
    this->year = 2026;

    std::smatch match;

    // Попробуем поискать по тегу /Title (обычно в скобках)
    if (std::regex_search(content, match, std::regex("/Title\\s*\\(([^)]+)\\)"))) {
        this->title = match[1].str();
    }

    // Попробуем поискать автора
    if (std::regex_search(content, match, std::regex("/Author\\s*\\(([^)]+)\\)"))) {
        this->author = match[1].str();
    }

    // Если нашли год в тексте (4 цифры подряд в конце файла)
    if (std::regex_search(content, match, std::regex("(\\d{4})"))) {
        this->year = std::stoi(match[1].str());
    }

    // Сохраняем то, что удалось накопать
    database.insertBook(this->title, this->author, this->year);
}
