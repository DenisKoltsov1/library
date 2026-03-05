#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "polka_book.h"
#include "connect.h"
#include <vector>

#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

int main() {
    ConnectDB myDb;
    std::cout << "!!! ПОПЫТКА СОЗДАТЬ БАЗУ ДАННЫХ !!!" << std::endl;
    // 1. Инициализация окна
    InitWindow(950, 950, "Library: Click & Read");
    SetTargetFPS(60);

    // 2. Создание списка уникальных книг (объектов)
    std::vector<Book> myBooks;
    myBooks.push_back(Book("War and Peace", 1869, "Tolstoy"));
    myBooks.push_back(Book("1984", 1949, "Orwell"));
    myBooks.push_back(Book("The Witcher", 1990, "Sapkowski"));
    myBooks.push_back(Book("Harry Potter", 1997, "Rowling"));
    myBooks.push_back(Book("Dracula", 1897, "Stoker"));

    CountShelf myShelf(5);
    int totalShelves = myShelf.getSize();
    int shelfSpacing = 150;

    // Цвета для книг
    Color palette[] = { RED, ORANGE, YELLOW, GREEN, BLUE, MAGENTA, GOLD, LIME };
    const char* statusText = "Click a book to select it";

    // 3. Основной цикл
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // --- ЛОГИКА ВЗАИМОДЕЙСТВИЯ ---
        for (int i = 0; i < myBooks.size(); i++) {
            int secondShelfY = 100 + (1 * shelfSpacing);
            int bookWidth = 65;
            int bookHeight = 110;
            int bookX = 110 + (i * (bookWidth + 5));

            // Прямоугольник для проверки столкновения с мышкой
            Rectangle bookRect = { (float)bookX, (float)secondShelfY - bookHeight, (float)bookWidth, (float)bookHeight };

            // Если кликнули мышкой по книге
            if (CheckCollisionPointRec(mousePos, bookRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Снимаем выбор со всех и выбираем текущую
                for (auto& b : myBooks) b.isSelected = false;
                myBooks[i].isSelected = true;
                statusText = "Book selected! Press E to Read, D to Put back";
            }

            // Обработка клавиш для выбранной книги
            if (myBooks[i].isSelected) {
                if (IsKeyPressed(KEY_E)) statusText = "Reading... (Title: 1984)";
                if (IsKeyPressed(KEY_D)) {
                    myBooks[i].isSelected = false;
                    statusText = "Book returned to shelf";
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // --- 1. РИСУЕМ БОКОВЫЕ СТОЙКИ СТЕЛЛАЖА ---
        DrawRectangle(90, 100, 10, 750, DARKGRAY);  // Левая
        DrawRectangle(700, 100, 10, 750, DARKGRAY); // Правая

        // --- 2. РИСУЕМ ПОЛКИ ---
        for (int i = 0; i < totalShelves; i++) {
            int shelfY = 100 + (i * shelfSpacing);
            DrawRectangle(100, shelfY, 600, 15, BROWN); // Доска полки
            DrawText(TextFormat("Shelf #%i", i + 1), 720, shelfY - 5, 18, GRAY);
        }

        // --- 3. РИСУЕМ КНИГИ ---
        int secondShelfY = 100 + (1 * shelfSpacing);
        int bookWidth = 65;
        int bookHeight = 110;

        for (int i = 0; i < myBooks.size(); i++) {
            int bookX = 110 + (i * (bookWidth + 5));
            int drawY = secondShelfY - bookHeight; // Стандартная позиция на полке

            // Если книга выбрана — выдвигаем её чуть НИЖЕ
            if (myBooks[i].isSelected) {
                drawY += 15;
                // Рисуем золотую рамку вокруг выбранной книги
                DrawRectangleLinesEx({ (float)bookX - 2, (float)drawY - 2, (float)bookWidth + 4, (float)bookHeight + 4 }, 2, GOLD);
            }

            // Рисуем тело книги
            DrawRectangle(bookX, drawY, bookWidth, bookHeight, palette[i % 8]);
            // Рисуем контур
            DrawRectangleLines(bookX, drawY, bookWidth, bookHeight, BLACK);
            // Надпись на книге
            DrawText("BOOK", bookX + 10, drawY + 45, 15, BLACK);
        }

        // --- 4. ИНТЕРФЕЙС ---
        DrawText(statusText, 20, 20, 22, DARKGRAY);
        DrawText("E - Read | D - Put back", 20, 50, 18, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
