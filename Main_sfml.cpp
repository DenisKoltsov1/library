#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "polka_book.h"
#include "connect.h"
#include "parsing.h" // Твой новый класс для парсинга
#include <vector>
#include <iostream>

#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main() {
    // 1. Инициализация базы данных (создаст таблицу, если её нет)
    ConnectDB myDb;
    std::cout << "!!! База данных инициализирована !!!" << std::endl;

    // 2. Инициализация окна Raylib
    InitWindow(950, 950, "Library: Drag & Drop PDF to Shelf");
    SetTargetFPS(60);

    // Список книг на полке (загружаем начальные данные)
    std::vector<Book> myBooks;
    myBooks.push_back(Book("War and Peace", 1869, "Tolstoy"));
    myBooks.push_back(Book("1984", 1949, "Orwell"));

    CountShelf myShelf(5);
    int totalShelves = myShelf.getSize();
    int shelfSpacing = 150;

    Color palette[] = { RED, ORANGE, YELLOW, GREEN, BLUE, MAGENTA, GOLD, LIME };
    const char* statusText = "Drag a PDF file here to add it!";

    while (!WindowShouldClose()) {
        // --- ЛОГИКА DRAG & DROP ---
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();

            for (unsigned int i = 0; i < droppedFiles.count; i++) {
                try {
                    // Создаем объект парсера для пути, который дал Raylib
                    ParsingBook parser(droppedFiles.paths[i]);

                    // Парсим и сохраняем в SQLite
                    parser.parse(myDb);

                    // Сразу добавляем книгу в визуальный список, чтобы она появилась на полке
                    // Мы берем данные, которые парсер вытащил из PDF (через геттеры)
                    myBooks.push_back(Book(parser.getTitle(), parser.getYear(), parser.getAuthor()));

                    statusText = "Book added successfully to DB and Shelf!";
                }
                catch (const std::exception& e) {
                    statusText = "Error: Not a valid PDF or file busy";
                    std::cerr << "Parsing error: " << e.what() << std::endl;
                }
            }
            UnloadDroppedFiles(droppedFiles); // Обязательно очищаем память Raylib
        }

        // --- ЛОГИКА ВЗАИМОДЕЙСТВИЯ (Твой код) ---
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < myBooks.size(); i++) {
            int bookWidth = 65;
            int bookHeight = 110;
            // Рассчитываем позицию (для примера все на 2-й полке)
            int bookX = 110 + (i * (bookWidth + 5));
            int shelfY = 100 + (1 * shelfSpacing);
            Rectangle bookRect = { (float)bookX, (float)shelfY - bookHeight, (float)bookWidth, (float)bookHeight };

            if (CheckCollisionPointRec(mousePos, bookRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (auto& b : myBooks) b.isSelected = false;
                myBooks[i].isSelected = true;
                statusText = "Selected: Press E to Read";
            }
        }

        // --- РИСОВАНИЕ ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Рисуем полки
        for (int i = 0; i < totalShelves; i++) {
            int shelfY = 100 + (i * shelfSpacing);
            DrawRectangle(100, shelfY, 600, 15, BROWN);
            DrawRectangle(90, 100, 10, 750, DARKGRAY);  // Стойка левая
            DrawRectangle(700, 100, 10, 750, DARKGRAY); // Стойка правая
        }

        // Рисуем книги из вектора
        for (int i = 0; i < myBooks.size(); i++) {
            int bookWidth = 65;
            int bookHeight = 110;
            int bookX = 110 + (i * (bookWidth + 5));
            int drawY = (100 + 1 * shelfSpacing) - bookHeight;

            if (myBooks[i].isSelected) drawY += 15;

            DrawRectangle(bookX, drawY, bookWidth, bookHeight, palette[i % 8]);
            DrawRectangleLines(bookX, drawY, bookWidth, bookHeight, BLACK);

            // Пишем первые 5 букв названия на корешке
            DrawText(myBooks[i].name.substr(0, 5).c_str(), bookX + 5, drawY + 45, 12, BLACK);
        }

        DrawText(statusText, 20, 20, 20, DARKBLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
