#include <locale.h>
#include <ncursesw/curses.h>
#include <vector>
#include <string>
#include <fstream>

#include <locale>
#include <cstdlib>
using namespace std;

// Hàm load ASCII/Unicode art từ file
vector<wstring> loadFrame(const char* filename) {
    setlocale(LC_ALL, ""); // hỗ trợ UTF-8
    wifstream fin("image_ascii.txt");
    fin.imbue(locale(""));
    fin.imbue(locale("")); // để đọc Unicode
    vector<wstring> frame;
    wstring line;
    while (getline(fin, line)) {
        frame.push_back(line);
    }
    return frame;
}

struct MenuItem {
    wstring title;
    vector<MenuItem> submenu;
    bool isForm = false;
};

void drawBox(int y, int x, int h, int w, bool highlight) {
    if (highlight) attron(COLOR_PAIR(2));
    else attron(COLOR_PAIR(1));

    mvaddwstr(y, x, L"┌");
    mvaddwstr(y, x+w, L"┐");
    mvaddwstr(y+h, x, L"└");
    mvaddwstr(y+h, x+w, L"┘");

    for (int i = 1; i < w; i++) {
        mvaddwstr(y, x+i, L"─");
        mvaddwstr(y+h, x+i, L"─");
    }
    for (int j = 1; j < h; j++) {
        mvaddwstr(y+j, x, L"│");
        mvaddwstr(y+j, x+w, L"│");
    }

    // Highlight nội dung
    if (highlight) {
        for (int j = 1; j < h; j++) {
            mvhline(y+j, x+1, ' ', w-1);
        }
    }

    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
}

void drawMenu(const vector<MenuItem>& menu, int sel, int startY=2, int startX=4) {
    int h = 3, w = 20;
    for (size_t i = 0; i < menu.size(); i++) {
        drawBox(startY + i*(h+1), startX, h, w, i == sel);
        mvaddwstr(startY + i*(h+1) + h/2, startX + 2, menu[i].title.c_str());
    }
}

wstring inputForm(int y, int x, const wstring& prompt) {
    echo();
    curs_set(1);
    mvprintw(y, x, "%ls: ", prompt.c_str());
    wint_t buffer[100];
    wgetn_wstr(stdscr, buffer, 100);
    noecho();
    curs_set(0);

    wstring value;
    for (int i = 0; buffer[i] != L'\0'; i++)
        value += buffer[i];
    return value;
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK); // bình thường
        init_pair(2, COLOR_BLACK, COLOR_CYAN);  // highlight
    }

    vector<MenuItem> mainMenu = {
        {L"Start Game"},
        {L"Options", {{L"Sound"}, {L"Graphics"}}},
        {L"Form Input", {}, true},
        {L"Exit"}
    };

    vector<vector<MenuItem>> menuStackItems; // lưu menu cha
    vector<int> menuStackIndex;              // lưu index menu cha
    vector<MenuItem> currentMenu = mainMenu;
    int sel = 0;
    vector<wstring> bg = loadFrame("image_ascii.txt");
    
    while (true) {
        clear();
        system("clear");
        system("cat image_ascii.txt");
        // ---- VẼ NỀN ASCII/Unicode từ file ----
        // for(size_t i = 0; i < bg.size(); i++)
        //     mvaddwstr(i, 0, bg[i].c_str());


        drawMenu(currentMenu, sel);
        mvprintw(20, 4, "Arrow keys to move, Enter select, Left back, q quit");
        refresh();

        int ch = getch();
        if (ch == 'q') break;

        if (ch == KEY_UP && sel > 0) sel--;
        if (ch == KEY_DOWN && sel < (int)currentMenu.size()-1) sel++;

        if (ch == '\n' || ch == KEY_ENTER) {
            MenuItem& item = currentMenu[sel];
            if (!item.submenu.empty()) {
                // Lưu menu cha
                menuStackItems.push_back(currentMenu);
                menuStackIndex.push_back(sel);
                currentMenu = item.submenu;
                sel = 0;
            } else if (item.isForm) {
                wstring value = inputForm(15, 4, L"Enter your name");
                mvprintw(17, 4, "You entered: %ls", value.c_str());
                getch();
            } else if (item.title == L"Exit") {
                break;
            }
        }

        if (ch == KEY_LEFT && !menuStackItems.empty()) {
            // Quay lại menu cha
            sel = menuStackIndex.back();
            menuStackIndex.pop_back();
            currentMenu = menuStackItems.back();
            menuStackItems.pop_back();
        }
    }

    endwin();
    return 0;
}
