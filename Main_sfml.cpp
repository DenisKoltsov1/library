#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "polka_book.h"
#include "connect.h"
#include "parsing.h" 
#include <vector>
#include <iostream>
#include <string>

// Подключение библиотек для компилятора MSVC
#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main() {
    // 1. Инициализация базы данных
    ConnectDB myDb;

    // 2. Инициализация окна Raylib
    InitWindow(950, 950, "Library: [D] Take/Put | [E] Read");
    SetTargetFPS(60);

    // Загружаем существующие книги из БД
    std::vector<Book> myBooks = myDb.showBook();

    // Указатель на книгу, которую мы держим в руках
    Book* bookInHand = nullptr;

    int shelfSpacing = 150;
    Color palette[] = { RED, ORANGE, YELLOW, GREEN, BLUE, MAGENTA, GOLD, LIME };
    const char* statusText = "Click to Select | [D] Take/Put | [E] Read";

    while (!WindowShouldClose()) {
        // --- ЛОГИКА DRAG & DROP (Добавление книг) ---
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            for (unsigned int i = 0; i < droppedFiles.count; i++) {
                try {
                    ParsingBook parser(droppedFiles.paths[i]);
                    parser.parse(myDb);
                    // Создаем книгу (используем age, как в твоем классе)
                    myBooks.push_back(Book(parser.getTitle(), parser.getYear(), parser.getAuthor()));
                    statusText = "Book added to shelf!";
                }
                catch (...) {
                    statusText = "Error: Could not parse PDF";
                }
            }
            UnloadDroppedFiles(droppedFiles);
        }

        Vector2 mousePos = GetMousePosition();

        // --- ЛОГИКА ВЗАИМОДЕЙСТВИЯ ---

        // 1. ВЫБОР КНИГИ (только если в руках ничего нет)
        if (bookInHand == nullptr) {
            for (int i = 0; i < (int)myBooks.size(); i++) {
                int bookX = 110 + (i * 70);
                int shelfY = 100 + (1 * shelfSpacing);
                Rectangle bookRect = { (float)bookX, (float)shelfY - 110, 65.0f, 110.0f };

                if (CheckCollisionPointRec(mousePos, bookRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    for (auto& b : myBooks) b.isSelected = false;
                    myBooks[i].isSelected = true;
                    statusText = "Selected! [E] to Read, [D] to Take";
                }
            }
        }

        // 2. КЛАВИША E - ЧИТАТЬ (вывод данных в консоль)
        if (IsKeyPressed(KEY_E)) {
            for (auto& b : myBooks) {
                if (b.isSelected) {
                    statusText = "Reading... (Check Console)";
                    // Используем b.age, так как это имя в твоем классе Book
                    std::cout << ">>> КНИГА: " << b.name << " | ГОД: " << b.age << " | АВТОР: " << b.author << std::endl;
                }
            }
        }

        // 3. КЛАВИША D - ВЗЯТЬ / ПОЛОЖИТЬ
        if (IsKeyPressed(KEY_D)) {
            if (bookInHand == nullptr) {
                // Если руки пусты — пытаемся ВЗЯТЬ выделенную книгу
                for (int i = 0; i < (int)myBooks.size(); i++) {
                    if (myBooks[i].isSelected) {
                        bookInHand = new Book(myBooks[i]); // Копируем в "руки"
                        myBooks.erase(myBooks.begin() + i); // Удаляем с полки
                        statusText = "You took the book! [D] to put it back.";
                        break;
                    }
                }
            }
            else {
                // Если книга в руках — КЛАДЕМ её обратно на полку
                bookInHand->isSelected = false;
                myBooks.push_back(*bookInHand);
                delete bookInHand; // Очищаем память
                bookInHand = nullptr;
                statusText = "Book put back on shelf.";
            }
        }

        // --- РИСОВАНИЕ ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 1. Рисуем боковые стойки шкафа (рейки)
        DrawRectangle(90, 100, 10, 750, DARKGRAY);  // Левая
        DrawRectangle(700, 100, 10, 750, DARKGRAY); // Правая

        // 2. Рисуем горизонтальные полки
        for (int i = 0; i < 5; i++) {
            int shelfY = 100 + (i * shelfSpacing);
            DrawRectangle(100, shelfY, 600, 15, BROWN);
        }

        // 3. Рисуем книги на полке (все на 2-й полке для примера)
        for (int i = 0; i < (int)myBooks.size(); i++) {
            int bookX = 110 + (i * 70);
            int drawY = (100 + 1 * shelfSpacing) - 110;

            // Визуальный эффект выбора
            if (myBooks[i].isSelected) drawY += 15;

            DrawRectangle(bookX, drawY, 65, 110, palette[i % 8]);
            DrawRectangleLines(bookX, drawY, 65, 110, BLACK);

            // Название (первые 5 букв)
            std::string label = (myBooks[i].name.length() >= 5) ? myBooks[i].name.substr(0, 5) : myBooks[i].name;
            DrawText(label.c_str(), bookX + 5, drawY + 45, 12, BLACK);
        }

        // 4. Рисуем книгу "в руках" (справа от шкафа)
        if (bookInHand != nullptr) {
            DrawText("IN HAND", 750, 370, 20, DARKBLUE);
            DrawRectangle(755, 405, 65, 110, GOLD);
            DrawRectangleLines(755, 405, 65, 110, BLACK);
            std::string label = (bookInHand->name.length() >= 5) ? bookInHand->name.substr(0, 5) : bookInHand->name;
            DrawText(label.c_str(), 760, 450, 12, BLACK);
        }

        // 5. Статусный текст
        DrawText(statusText, 20, 20, 20, DARKBLUE);

        EndDrawing();
    }

    // Очистка при выходе
    if (bookInHand) delete bookInHand;
    CloseWindow();
    return 0;
}
