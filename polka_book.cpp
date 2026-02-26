#include "polka_book.h"

// Реализация конструктора со списком инициализации
CountShelf::CountShelf(int count_val) : shelfSize(count_val) {
    countShelf.resize(shelfSize);
}

// Реализация метода
int CountShelf::getSize() {
    return shelfSize;
}
