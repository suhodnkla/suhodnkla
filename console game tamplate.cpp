//C 헤더
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>

//C++ 라이브러리
#include <iostream>
#include <thread>

//윈도우, 콘솔 헤더
#include <windows.h>
#include <conio.h>

using namespace std;


void click(int* xx, int* yy, int* lr)
{
    INPUT_RECORD rec;
    DWORD dwNOER;
    HANDLE CIN = 0;
    while (1)
    {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER); // 콘솔창 입력을 받아들임.
        if (rec.EventType == MOUSE_EVENT) {// 마우스 이벤트일 경우

            if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) { // 좌측 버튼이 클릭되었을 경우
                int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
                int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴

                *xx = mouse_x; //x값을 넘김
                *yy = mouse_y; //y값을 넘김
                *lr = 1; //마우스 클릭상태를 넘김

                break;
            }
            else if (rec.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) { // 우측 버튼이 클릭되었을 경우
                int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
                int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴

                *xx = mouse_x; //x값을 넘김
                *yy = mouse_y; //y값을 넘김
                *lr = 2; //마우스 클릭상태를 넘김

                break;
            }
        }
    }
}

void color(int a) //색상을 지정할 수 있음. a 값은 0~255
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), a);
}

void gotoxy(int x, int y) //콘솔 내의 좌표값으로 이동
{
    COORD pos = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursorhide() //커서를 숨기는 함수
{
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

class GameObject
{
public:

    bool enable = true;

    char Look[32][32];

    int x_len = 1,y_len = 1;

    int Color = 7; // 0 ~ 255

    int x = 0, y = 0;

    int Print(void)
    {
        int i;
        color(Color);

        for (i = 0; i < y_len; i++)
        {

            cursorhide();
            gotoxy(x, y + i);
            printf(Look[i]);
        }

        return y_len;
    }

    int Erase(void)
    {
        int i, j;
        for (i = 0; i < y_len; i++)
        {
            for (j = 0; j < x_len; j++)
            {
                cursorhide();
                gotoxy(x+j, y + i);
                printf(" ");
            }

        }
    }

    int Set_Figure(char str[][32],int xlenIn, int ylenIn)
    {
        int i;
        for (i = 0; i < ylenIn; i++)
        {
            strcpy_s(Look[i],str[i]);
        }
        x_len = xlenIn;
        y_len = ylenIn;
        return 0;
    }

    int Set_Position(int xIn, int yIn)
    {
        x = xIn;
        y = yIn;

        return 0;
    }

    int Get_X(void)
    {
        return x;
    }

    int Get_Y(void)
    {
        return y;
    }
};

class Button : public GameObject
{
private:

    thread th;
    int (*execute)(void);

    void Action()
    {
        INPUT_RECORD rec;
        DWORD dwNOER;
        HANDLE CIN = 0;

        while (true)
        {
            SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

            if(enable)
            {
                ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);
                if (rec.EventType == MOUSE_EVENT) // 마우스 이벤트일 경우
                {
                    if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)  // 좌측 버튼이 클릭되었을 경우
                    {
                        int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X값 받아옴
                        int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y값 받아옴

                        if (mouse_x >= x && mouse_x < x + x_len)
                        {
                            if (mouse_y >= y && mouse_y < y + y_len && enable)
                            {
                                (*execute)();
                            }
                        }
                    }
                }
            }
        }
    }

public:

    Button(int (*executeIn)(void))
    {
        execute = executeIn;

        th = thread(&Button::Action, this);
    }


};

int f(void)
{
    static int cnt = 0;
    cnt++;
    gotoxy(10, 10);
    printf("Hello,World %d",cnt);
    return 0;
}

int f1(void)
{
    static int cnt = 0;
    cnt++;
    srand(time(NULL));
    color(rand()%16);
    gotoxy(10,11);
    printf("Goodbye, World %d",cnt);
}

int main()
{
    DWORD consoleModePrev;
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(handle, &consoleModePrev);
    SetConsoleMode(handle, consoleModePrev & ~ENABLE_QUICK_EDIT_MODE);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    Button obj = Button(f);
    char str[32][32] = { "Hello"};
    obj.Color = 7;
    obj.Set_Position(0, 0);
    obj.enable = true;
    obj.Set_Figure(str, 8, 1);
    obj.Print();

    Button obj2 = Button(f1);
    char str1[32][32] = { "Goodbye"};
    obj2.Color = 7;
    obj2.Set_Position(1, 10);
    obj2.enable = true;
    obj2.Set_Figure(str1, 8, 1);
    obj2.Print();

    while(1){}
}

