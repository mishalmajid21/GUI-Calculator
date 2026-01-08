#include <windows.h>
#include <cstdlib>
#include <cstdio>

HWND hDisplay;    // Display field
char currentInput[100] = "";

// Append a character to display
void appendChar(char c) {
    size_t len = strlen(currentInput);
    if (len < sizeof(currentInput) - 1) {
        currentInput[len] = c;
        currentInput[len + 1] = '\0';
        SetWindowText(hDisplay, currentInput);
    }
}

// Clear display
void clearDisplay() {
    currentInput[0] = '\0';
    SetWindowText(hDisplay, currentInput);
}

// Evaluate expression (supports only +,-,*,/)
double evaluateExpression() {
    double num1 = 0, num2 = 0;
    char op = 0;
    sscanf(currentInput, "%lf%c%lf", &num1, &op, &num2);
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': return num2 != 0 ? num1 / num2 : 0;
    }
    return num1;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        switch (wp) {
            // Number buttons 1-9
            case 101: appendChar('1'); break;
            case 102: appendChar('2'); break;
            case 103: appendChar('3'); break;
            case 104: appendChar('4'); break;
            case 105: appendChar('5'); break;
            case 106: appendChar('6'); break;
            case 107: appendChar('7'); break;
            case 108: appendChar('8'); break;
            case 109: appendChar('9'); break;
            case 100: appendChar('0'); break;

            // Operators
            case 201: appendChar('+'); break;
            case 202: appendChar('-'); break;
            case 203: appendChar('*'); break;
            case 204: appendChar('/'); break;

            // Clear
            case 301: clearDisplay(); break;

            // Equals
            case 302: {
                double res = evaluateExpression();
                char r[50];
                sprintf(r, "%g", res);
                SetWindowText(hDisplay, r);
                strcpy(currentInput, r);
            } break;
        }
    }

    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

// WinMain entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "FullCalcClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow("FullCalcClass", "Real GUI Calculator",
                             WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
                             300, 200, 260, 350,
                             NULL, NULL, hInst, NULL);

    // Display
    hDisplay = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
                            20, 20, 210, 30, hwnd, NULL, hInst, NULL);

    // Number buttons
    int numButtons[10] = {100,101,102,103,104,105,106,107,108,109};
    const char* labels[10] = {"0","1","2","3","4","5","6","7","8","9"};
    int x=20,y=60;
    int count=1;
    for(int i=1;i<=9;i++){
        CreateWindow("BUTTON", labels[i], WS_CHILD | WS_VISIBLE,
                     x, y, 50, 40, hwnd, (HMENU)numButtons[i], hInst, NULL);
        x+=55;
        if(count%3==0){ x=20; y+=45; }
        count++;
    }
    // Button 0
    CreateWindow("BUTTON", "0", WS_CHILD | WS_VISIBLE, 75, y, 50, 40, hwnd, (HMENU)100, hInst, NULL);

    // Operator buttons
    const char* ops[4] = {"+","-","*","/"};
    for(int i=0;i<4;i++)
        CreateWindow("BUTTON", ops[i], WS_CHILD | WS_VISIBLE, 180, 60+i*45, 50, 40, hwnd, (HMENU)(201+i), hInst, NULL);

    // Clear and Equals
    CreateWindow("BUTTON", "C", WS_CHILD | WS_VISIBLE, 20, y+45, 50, 40, hwnd, (HMENU)301, hInst, NULL);
    CreateWindow("BUTTON", "=", WS_CHILD | WS_VISIBLE, 135, y+45, 95, 40, hwnd, (HMENU)302, hInst, NULL);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

