#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "connect.h"
#include "parsing.h" 
#include <vector>
#include <iostream>
#include <string>

// Подключение библиотек
#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main() {
    // 1. Инициализация базы и окна
    ConnectDB myDb;
    InitWindow(950, 950, "Library: [D] Take/Put | [E] Read");
    SetTargetFPS(60);

    // 2. Загрузка данных
    Book tempBook("temp", 0, "temp");
    std::vector<Book> myBooks = tempBook.showBook(myDb.getDb());
    Book* bookInHand = nullptr;

    // Состояния программы
    bool isReading = false;
    int shelfSpacing = 150;
    Color palette[] = { RED, ORANGE, YELLOW, GREEN, BLUE, MAGENTA, GOLD, LIME };
    const char* statusText = "Click to Select | [D] Take/Put | [E] Read";

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // --- ЛОГИКА ВЗАИМОДЕЙСТВИЯ ---

        // 1. КЛАВИША E - ПЕРЕКЛЮЧЕНИЕ ЧТЕНИЯ (Очистка экрана)
        if (IsKeyPressed(KEY_E)) {
            if (isReading) {
                isReading = false;
            }
            else {
                // Проверяем, выбрана ли книга для чтения
                for (auto& b : myBooks) {
                    if (b.isSelected) {
                        isReading = true;
                        statusText = "Reading mode. Press [E] to return.";
                        break;
                    }
                }
            }
        }

        // Логика шкафа работает, только если мы НЕ в режиме чтения
        if (!isReading) {
            // 2. ФАЙЛЫ (Drag & Drop)
            if (IsFileDropped()) {
                FilePathList droppedFiles = LoadDroppedFiles();
                for (unsigned int i = 0; i < droppedFiles.count; i++) {
                    try {
                        ParsingBook parser(droppedFiles.paths[i]);
                        parser.parse(myDb);
                        myBooks.push_back(Book(parser.getTitle(), parser.getYear(), parser.getAuthor()));
                    }
                    catch (...) {}
                }
                UnloadDroppedFiles(droppedFiles);
            }

            // 3. ВЫБОР МЫШКОЙ
            if (bookInHand == nullptr) {
                for (int i = 0; i < (int)myBooks.size(); i++) {
                    int bookX = 110 + (i * 70);
                    int shelfY = 100 + (1 * shelfSpacing);
                    Rectangle bookRect = { (float)bookX, (float)shelfY - 110, 65.0f, 110.0f };

                    if (CheckCollisionPointRec(mousePos, bookRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        for (auto& b : myBooks) b.isSelected = false;
                        myBooks[i].isSelected = true;
                    }
                }
            }

            // 4. КЛАВИША D - ВЗЯТЬ / ПОЛОЖИТЬ
            if (IsKeyPressed(KEY_D)) {
                if (bookInHand == nullptr) {
                    for (int i = 0; i < (int)myBooks.size(); i++) {
                        if (myBooks[i].isSelected) {
                            bookInHand = new Book(myBooks[i]);
                            myBooks.erase(myBooks.begin() + i);
                            break;
                        }
                    }
                }
                else {
                    bookInHand->isSelected = false;
                    myBooks.push_back(*bookInHand);
                    delete bookInHand;
                    bookInHand = nullptr;
                }
            }
        }

        // --- РИСОВАНИЕ ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (isReading) {
            // --- ЭКРАН ЧТЕНИЯ (Полок нет) ---
            DrawRectangle(50, 50, 850, 850, WHITE);
            DrawRectangleLines(50, 50, 850, 850, BLACK);

            for (auto& b : myBooks) {
                if (b.isSelected) {
                    DrawText("ОТКРЫТАЯ КНИГА", 350, 80, 25, DARKGRAY);
                    DrawText(b.name.c_str(), 100, 150, 30, BLACK);
                    DrawText(b.author.c_str(), 100, 200, 20, GRAY);
                    DrawText("Текст книги из базы данных...", 100, 300, 20, DARKBLUE);
                }
            }
            DrawText("Нажмите [E], чтобы закрыть", 330, 880, 20, RED);

        }
        else {
            // --- ЭКРАН ШКАФА ---
            DrawText(statusText, 20, 20, 20, DARKBLUE);

            // Стойки
            DrawRectangle(90, 100, 10, 750, DARKGRAY);
            DrawRectangle(700, 100, 10, 750, DARKGRAY);

            // Полки
            for (int i = 0; i < 5; i++) {
                DrawRectangle(100, 100 + (i * shelfSpacing), 600, 15, BROWN);
            }

            // Книги (вызывают свой метод draw)
            for (int i = 0; i < (int)myBooks.size(); i++) {
                int bookX = 110 + (i * 70);
                int shelfBaseY = (100 + 1 * shelfSpacing) - 110;
                myBooks[i].draw(bookX, shelfBaseY, palette[i % 8]);
            }

            // Книга в руках
            if (bookInHand != nullptr) {
                DrawText("В РУКАХ", 750, 370, 20, DARKBLUE);
                bookInHand->draw(755, 405, GOLD);
            }
        }

        EndDrawing();
    }

    if (bookInHand) delete bookInHand;
    CloseWindow();
    return 0;
}
