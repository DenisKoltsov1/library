// 1. Подключаем библиотеку с ПРАВИЛЬНЫМ путем к вашей распакованной папке
#include "C:/raylib/raylib-5.5_win64_msvc16/include/raylib.h"
#include "book.h"
#include "polka_book.h" 

// Путь к самому файлу библиотеки .lib
#pragma comment(lib, "C:/raylib/raylib-5.5_win64_msvc16/lib/raylib.lib")

// Системные библиотеки Windows, необходимые для raylib
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
int main() {
    // Увеличим высоту окна, чтобы влезло 10 полок
    InitWindow(950, 950, "Library Shelves");
    SetTargetFPS(60);

    int totalShelves = 5; // Количество полок
    int i = 1;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
      
       
        DrawText(TextFormat("Stelag: %i", i), 250, 20, 30, DARKGRAY);
        // Цикл для отрисовки 10 полок
        for (int i = 0; i < totalShelves; i++) {
            // Рассчитываем высоту каждой полки (Y)
            // 50 — отступ сверху, i * 80 — расстояние между полками
            int shelfY = 100 + (i * 180);

            // 1. Рисуем доску полки (коричневый прямоугольник)
            DrawRectangle(100, shelfY, 600, 15, BROWN);

            // 2. Рисуем боковые стойки (для красоты, чтобы это был стеллаж)
            DrawRectangle(90, 100, 10, 800, DARKGRAY); // Левая стойка
            DrawRectangle(700, 100, 10, 800, DARKGRAY); // Правая стойка

            // 3. Выводим номер полки (только английские буквы, чтобы не было '????')
            DrawText(TextFormat("Shelf #%i", i + 1), 720, shelfY, 15, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
