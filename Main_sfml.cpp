#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "polka_book.h"

#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main() {
    // 1. Инициализация окна (950x950)
    InitWindow(950, 950, "Library Shelves - Rainbow Edition");
    SetTargetFPS(60);

    // 2. Создание объектов и настройка данных
    Book book1("war_of_peace", 1895, "Tolstoy");
    book1.setCount(8); // Устанавливаем 8 книг через ваш исправленный метод

    CountShelf myShelf(5);
    int totalShelves = myShelf.getSize();
    int shelfSpacing = 150; // Расстояние между полками (высота пролета)

    // Палитра цветов для книг
    Color palette[] = { RED, ORANGE, YELLOW, GREEN, BLUE, MAGENTA, GOLD, LIME };
    int paletteSize = 8;

    // 3. Основной игровой цикл
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- ОТРИСОВКА СТЕЛЛАЖА (Вертикальные стойки) ---
        DrawRectangle(90, 100, 10, 750, DARKGRAY);  // Левая стойка
        DrawRectangle(700, 100, 10, 750, DARKGRAY); // Правая стойка

        // --- ОТРИСОВКА ПОЛОК (Коричневые доски) ---
        for (int i = 0; i < totalShelves; i++) {
            int shelfY = 100 + (i * shelfSpacing);
            // Рисуем доску полки
            DrawRectangle(100, shelfY, 600, 15, BROWN);
            // Номер полки (выровнен по правому краю стеллажа)
            DrawText(TextFormat("Shelf #%i", i + 1), 720, shelfY - 5, 18, GRAY);
        }

        // --- ОТРИСОВКА РАЗНОЦВЕТНЫХ КНИГ НА ВТОРОЙ ПОЛКЕ ---
        int count_book = book1.getCount();
        int secondShelfY = 100 + (1 * shelfSpacing); // Координата Y второй полки

        int bookWidth = 60;   // Большая ширина
        int bookHeight = 115; // Большая высота (почти до верхней полки)

        for (int i = 0; i < count_book; i++) {
            // Рассчитываем X для каждой книги (ширина книги + зазор 5 пикселей)
            int bookX = 110 + (i * (bookWidth + 5));

            // Выбираем цвет из палитры по кругу
            Color currentBookColor = palette[i % paletteSize];

            // 1. Рисуем тело книги
            // (secondShelfY - bookHeight) — чтобы книга стояла НА доске
            DrawRectangle(bookX, secondShelfY - bookHeight, bookWidth, bookHeight, currentBookColor);

            // 2. Рисуем контур книги (черный), чтобы они не сливались
            DrawRectangleLines(bookX, secondShelfY - bookHeight, bookWidth, bookHeight, BLACK);

            // 3. Рисуем надпись "BOOK" (черным цветом для контраста с яркими цветами)
            DrawText("BOOK", bookX + 8, secondShelfY - (bookHeight / 2) - 5, 15, BLACK);
        }

        // Статистика в верхнем углу
        DrawText(TextFormat("Total Books: %i", count_book), 20, 20, 25, RED);

        EndDrawing();
    }

    // 4. Завершение работы программы
    CloseWindow();
    return 0;
}
