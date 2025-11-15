#include <locale.h>
#include <ncursesw/curses.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// ================= Menu struct =================
struct Menu {
    string title;
    vector<string> items;
    vector<Menu*> children;   // con của menu
    Menu* parent = nullptr;
};

struct Pixel{
    int r,g,b;
};

// ---------------- Load image ----------------
vector<Pixel> loadImageRow(const string& filename,int& w,int& h){
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &n, 3);
    if(!data){
        cerr << "Failed to load image: " << filename << endl;
        exit(1);
    }
    vector<Pixel> img(w*h);
    for(int i=0;i<w*h;i++){
        img[i].r = data[i*3+0];
        img[i].g = data[i*3+1];
        img[i].b = data[i*3+2];
    }
    stbi_image_free(data);
    return img;
}

// ---------------- Menu ----------------
void drawBox(int y,int x,int h,int w,bool highlight,const string& text){
    mvaddwstr(y,x,L"┌");
    mvaddwstr(y,x+w,L"┐");
    mvaddwstr(y+h,x,L"└");
    mvaddwstr(y+h,x+w,L"┘");
    for(int i=1;i<w;i++){
        mvaddwstr(y,x+i,L"─");
        mvaddwstr(y+h,x+i,L"─");
    }
    for(int j=1;j<h;j++){
        mvaddwstr(y+j,x,L"│");
        mvaddwstr(y+j,x+w,L"│");
    }
    if(highlight){
        attron(A_REVERSE);
        mvprintw(y+h/2,x+2,"%s",text.c_str());
        attroff(A_REVERSE);
    } else {
        mvprintw(y+h/2,x+2,"%s",text.c_str());
    }
}

// ---------------- Form Input ----------------
wstring inputForm(int y, int x, int w, const wstring& prompt) {
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

int main(){
    setlocale(LC_ALL,"");
    initscr();
    start_color();
    use_default_colors();
    noecho();
    cbreak();
    keypad(stdscr,TRUE);
    curs_set(0);

    // ------------------- Terminal size -------------------
    int term_w, term_h;
    getmaxyx(stdscr, term_h, term_w);

    // ------------------- Load Image -------------------
    int img_w,img_h;
    vector<Pixel> img = loadImageRow("resized.png", img_w,img_h);
    
    // ------------------- Init 256 color pairs -------------------
    int pair_index = 1;
    for(int r=0;r<6;r++)
        for(int g=0;g<6;g++)
            for(int b=0;b<6;b++){
                init_pair(pair_index,16 + r*36 + g*6 + b,-1);
                pair_index++;
            }

    // ------------------- Main Loop -------------------
    int selY=0,selX=0;
    vector<string> menuItems = {"Start","Options","Exit"};

    while(true){
        clear();
        getmaxyx(stdscr, term_h, term_w);

        // ---------- Render image ---------- // ◀ ▶ ⬊ ▰ ♦ ❚ ▐ ▮
        double step_x = max(1.0, (double)img_w / term_w);
        double char_ratio = 1.0; // font height / width
        double step_y = max(1.0, (double)img_h / (term_h*char_ratio));
        for(int y=0;y<term_h && y<img_h/step_y;y++){
            for(int x=0;x<term_w && x<img_w/step_x;x++){
                int px = x*step_x;
                int py = y*step_y;
                Pixel p = img[py*img_w+px];
                int cr = p.r/51, cg=p.g/51, cb=p.b/51;
                int color_pair = 1 + cr*36 + cg*6 + cb;
                attron(COLOR_PAIR(color_pair));
                wstring gradient = L"  ❚▮◀▰▶◆▮❚░▓";
                int lum = (p.r + p.g + p.b)/3;
                wchar_t ch = gradient[lum*gradient.size()/256];
                mvaddwstr(y,x,&ch);
                attroff(COLOR_PAIR(color_pair));
            }
        }

        // ---------- Menu ----------
        int startY = 10, startX = 18, cellH = 2, cellW = 130;
        for(int i=0;i<menuItems.size();i++){
            drawBox(startY+i*(cellH+1),startX,cellH,cellW,selY==i,menuItems[i]);
        }

        mvprintw(term_h-2,2,"Use arrows to move, Enter to select, q to quit.");
        refresh();

        int ch = getch();
        if(ch=='q') break;
        if(ch==KEY_UP && selY>0) selY--;
        if(ch==KEY_DOWN && selY<(int)menuItems.size()-1) selY++;
        if(ch=='\n'){
            if(menuItems[selY]=="Exit") break;
            else if(menuItems[selY]=="Options"){
                wstring name = inputForm(term_h-4,2,20,L"Enter Name: ");
                mvprintw(term_h-3,2,"You entered: %ls",name.c_str());
                getch();
            }
        }
    }

    endwin();
    return 0;
}
