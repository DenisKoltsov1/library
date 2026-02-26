#ifndef POLKA_BOOK_H
#define POLKA_BOOK_H

#include <vector>

class CountShelf {
private:
    const int shelfSize;
    std::vector<int> countShelf;

public:
    // Только объявляем конструктор
    CountShelf(int count_val);

    // Только объявляем метод
    int getSize();
};

#endif
