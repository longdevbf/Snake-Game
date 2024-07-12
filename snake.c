#include <stdio.h>
#include <conio.h>
#include <windows.h>

enum state { INGAME, MENU, SETTING, GAMEOVER, INFO } state;

int vx = 0, vy = -1;
int snake[1000];
int ct = 0;
int length;
int fruit = 12010;
int score = 0;

int getX(int a) {
    return (a - 10000) / 100;
}

int getY(int a) {
    return a % 100;
}

// Hàm random đưa fruit ngẫu nhiên xuất hiện trên màn hình
int RANDOM(int a, int b) {
    return a + rand() % (b - a + 1);
}

void creatFruit() {
    int bl = 0;
    for (;;) {
        fruit = 10000 + RANDOM(1, 117) * 100;
        fruit += RANDOM(1, 27);
        for (int i = 0; i < length; i++) {
            if (fruit == snake[i]) bl = 1;
        }
        if (!bl) break;
    }
}

void initgame() {
    length = 2;
    snake[0] = 11010;
    snake[1] = 11011;
}

void gotoxy(int x, int y) {
    static HANDLE h = NULL;
    if (!h) {
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}

void menu() {
    system("cls");
    printf("-------MENU------\n");
    printf("1. Vao choi\n");
    printf("2. Cai dat\n");
    printf("3. Thoat game\n");

    while (1) {
        if (kbhit()) {
            int key = getch();
            if (key == '1') {
                state = INGAME;
                return;
            } else if (key == '2') {
                state = SETTING;
                return;
            } else if (key == '3') {
                exit(0);
            }
        }
        Sleep(100); // Ngủ 100ms để giảm tải CPU
    }
}

void printScore() {
    int tg = score;
    int a[5];
    for (int i = 0; i < 5; i++) {
        a[i] = tg % 10;
        tg /= 10;
    }
    gotoxy(119, 20);
    printf("%d", a[0]);
    gotoxy(119, 19);
    printf("%d", a[1]);
    gotoxy(119, 18);
    printf("%d", a[2]);
    gotoxy(119, 17);
    printf("%d", a[3]);
    gotoxy(119, 16);
    printf("%d", a[4]);
}

void printBox() {
    for (int i = 0; i < 119; i++) {
        printf("=");
    }
    for (int i = 0; i < 28; i++) {
        printf("\n");
        for (int j = 0; j < 119; j++) {
            if (j == 0 || j == 118) {
                printf("=");
            } else {
                printf(" ");
            }
        }
    }
    printf("\n");
    for (int i = 0; i < 119; i++) {
        printf("=");
    }
    gotoxy(119, 10);
    printf("s");
    gotoxy(119, 11);
    printf("c");
    gotoxy(119, 12);
    printf("o");
    gotoxy(119, 13);
    printf("r");
    gotoxy(119, 14);
    printf("e");
}

void ingame() {
    int key = 0;
    initgame();
    printBox();
    gotoxy(getX(snake[0]), getY(snake[0]));
    printf("#");
    for (int j = 1; j < length - 1; j++) {
        gotoxy(getX(snake[j]), getY(snake[j]));
        printf("o");
    }
    while (state == INGAME) {
        if (kbhit()) {
            key = getch();
            switch (key) {
                case 72: // nut len
                    if (vx != 0) {
                        vy = -1;
                        vx = 0;
                    }
                    break;
                case 80: // nut xuong
                    if (vx != 0) {
                        vy = 1;
                        vx = 0;
                    }
                    break;
                case 75: // nut trai
                    if (vy != 0) {
                        vx = -1;
                        vy = 0;
                    }
                    break;
                case 77: // nut phai
                    if (vy != 0) {
                        vx = 1;
                        vy = 0;
                    }
                    break;
            }
        }

        if ((ct % 5000) == 0) {
            gotoxy(getX(snake[length - 1]), getY(snake[length - 1]));
            printf(" ");
            for (int i = length - 1; i > 0; i--) {
                snake[i] = snake[i - 1];
            }
            snake[0] += vy;
            snake[0] += vx * 100;

            // Xử lý khi con rắn vượt ra khỏi màn hình
            if (getY(snake[0]) == 0) snake[0] += 28;
            if (getY(snake[0]) == 29) snake[0] -= 28;
            if (getX(snake[0]) == 0) snake[0] += 11500;
            if (getX(snake[0]) == 118) snake[0] -= 11500;

            gotoxy(getX(snake[0]), getY(snake[0]));
            printf("<");
            gotoxy(getX(fruit), getY(fruit));
            printf("o");
            if (snake[0] == fruit) { // ăn được chữ O thì += #
                gotoxy(getX(fruit), getY(fruit));
                printf("#");
                length++;
                creatFruit();
                score += 5;
                printScore();
            }
            gotoxy(getX(snake[0]), getY(snake[0]));
            printf("#");
            gotoxy(getX(snake[1]), getY(snake[1]));
            printf("o");
            gotoxy(getX(fruit), getY(fruit));
            printf("O");
            for (int i = 1; i < length; i++) {
                if (snake[0] == snake[i])
                    state = GAMEOVER; // so sánh liên tục nếu cắn phải thân thì over game
            }
        }
        ct++;
    }
}

void setting() {
    system("cls");
    printf("Setting\n");
    printf("Press any key to return to menu...\n");
    getch(); // Chờ người dùng nhấn một phím để trở về menu
    state = MENU; // Thiết lập trạng thái là MENU để quay lại vòng lặp chính
}

void gameover() {
    system("cls");
    printf("GAME OVER\n");
    printf("Press any key to return to menu...\n");
    getch(); // Chờ người dùng nhấn một phím để trở về menu
    state = MENU; // Thiết lập trạng thái là MENU để quay lại vòng lặp chính
}

void info() {
    system("cls");
    printf("Info\n");
    printf("Press any key to return to menu...\n");
    getch(); // Chờ người dùng nhấn một phím để trở về menu
    state = MENU; // Thiết lập trạng thái là MENU để quay lại vòng lặp chính
}

int main() {
    srand(time(NULL));

    state = MENU; // Thiết lập trạng thái ban đầu là MENU

    while (1) {
        switch (state) {
            case MENU:
                menu();
                break;
            case SETTING:
                setting();
                break;
            case INGAME:
                ingame();
                break;
            case GAMEOVER:
                gameover();
                break;
            case INFO:
                info();
                break;
        }
    }

    return 0;
}
