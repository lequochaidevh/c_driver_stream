#include <locale.h>
#include <ncursesw/curses.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <codecvt>
#include <cstdlib> // Required for std::system

using namespace std;
#include <boost/locale.hpp>

#include <memory>
#include <stdexcept>
#include <array>

// A reusable function to execute a command and capture its output
std::string exec_cli(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    // Use std::unique_ptr to automatically call pclose() when it goes out of scope
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void remove_newlines(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Tiếp tục tìm kiếm sau chuỗi đã được thay thế
    }
}

std::string ws2s(const std::wstring& w) {
    return boost::locale::conv::utf_to_utf<char>(w);
}
// ================= Menu struct =================
struct Menu {
    string title;
    vector<string> items;
    vector<Menu*> children;
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

std::vector<std::string> splitString(const std::string& s) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string line;
    while (std::getline(ss, line)) {
        result.push_back(line);
    }
    return result;
}

// Hàm drawBox đã được nâng cấp để hỗ trợ nhiều dòng
void drawBox_endl(int y, int x, int h, int w, bool highlight, const std::string& text) {
    // ... Phần vẽ khung không đổi ...
    mvaddwstr(y, x, L"┌");
    mvaddwstr(y, x + w, L"┐");
    mvaddwstr(y + h, x, L"└");
    mvaddwstr(y + h, x + w, L"┘");
    for (int i = 1; i < w; i++) {
        mvaddwstr(y, x + i, L"─");
        mvaddwstr(y + h, x + i, L"─");
    }
    for (int j = 1; j < h; j++) {
        mvaddwstr(y + j, x, L"│");
        mvaddwstr(y + j, x + w, L"│");
    }

    std::vector<std::string> lines = splitString(text);

    int start_y = y + 1;
    int max_lines = h - 2; // Số dòng tối đa có thể in trong khung
    int current_line = 0;

    for (const auto& line : lines) {
        if (current_line >= max_lines) break; // Ngừng in nếu vượt quá khung

        if (highlight) {
            attron(A_REVERSE);
            mvprintw(start_y + current_line, x + 2, "%s", line.c_str());
            attroff(A_REVERSE);
        } else {
            mvprintw(start_y + current_line, x + 2, "%s", line.c_str());
        }
        current_line++;
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
wstring inputBox(int y, int x, int w, int h, const wstring& prompt)
{
    // ===== Draw Box =====
    mvadd_wch(y,   x,   WACS_ULCORNER);
    mvadd_wch(y,   x+w, WACS_URCORNER);
    mvadd_wch(y+h, x,   WACS_LLCORNER);
    mvadd_wch(y+h, x+w, WACS_LRCORNER);

    // Collums
    for(int i=x+1;i<x+w;i++){
        mvadd_wch(y,   i, WACS_HLINE);
        mvadd_wch(y+h, i, WACS_HLINE);
    }

    // Rows
    for(int i=y+1;i<y+h;i++){
        mvadd_wch(i, x,   WACS_VLINE);
        mvadd_wch(i, x+w, WACS_VLINE);
    }

    // ===== PROMPT =====
    mvaddwstr(y+1, x+2, prompt.c_str());
    refresh();

    // ===== Get INPUT =====
    wstring result;
    int cx = x + 2 + prompt.size() + 1;  // cursor X
    int cy = y + 1;

    move(cy, cx);
    curs_set(1);

    while (1) {
        wint_t wc;
        get_wch(&wc);

        if (wc == '\n') break;        // Enter → OK

        if (wc == KEY_BACKSPACE || wc == 127) {
            if (!result.empty()) {
                result.pop_back();
                mvaddwstr(cy, cx + result.size(), L" ");
                move(cy, cx + result.size());
            }
        }
        else {
            if ((int)result.size() < w - 4) {
                result.push_back((wchar_t)wc);
                mvaddwstr(cy, cx, result.c_str());
            }
        }
        refresh();
    }
    curs_set(0);
    return result;
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

    // ================= Create MENU tree =================
    Menu root{"Main Menu", {"Start","Options","Commander","Exit"}};
    Menu cli{"Commander", {"Input Command","Back"}};
    Menu opt{"Options", {"Change Name","Graphics","Back"}};
    Menu gfx{"Graphics", {"Low","Medium","High","Back"}};
    // set hierarchy
    root.children = {nullptr, &opt, &cli, nullptr};
    opt.parent = &root;
    opt.children = {nullptr, &gfx, &root};
    cli.parent = &root;
    cli.children = {nullptr, &root};
    gfx.parent = &opt;
    gfx.children = {nullptr,nullptr,nullptr,&opt};
    Menu* current = &root;
    int sel = 0;

    // ------------------- Init 256 color pairs -------------------
    int pair_index = 1;
    for(int r=0;r<6;r++)
        for(int g=0;g<6;g++)
            for(int b=0;b<6;b++){
                init_pair(pair_index,16 + r*36 + g*6 + b,-1);
                pair_index++;
            }

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
                wstring gradient = L"▰ ▰❚▮◀▰▶◆▮❚░▓";
                int lum = (p.r + p.g + p.b)/3;
                wchar_t ch = gradient[lum*gradient.size()/256];
                mvaddwstr(y,x,&ch);
                attroff(COLOR_PAIR(color_pair));
            }
        }

        // ---------- Menu ----------
        int startY = 6, startX = 18, cellH = 2, cellW = 128;
        for(int i=0;i < current->items.size();i++){
            drawBox(startY+i*(cellH+1),startX,cellH,cellW,sel==i,current->items[i]);
        }

        mvprintw(term_h-1, 90,"---------- Use arrows to move, Enter to select, q to quit. ----------");
        refresh();

        // -------- Input ----------
        int ch = getch();
        if(ch=='q') break;
        if(ch==KEY_UP && sel>0) sel--;
        if(ch==KEY_DOWN && sel < (int)current->items.size()-1) sel++;

        if(ch == KEY_LEFT){
            if(current->parent){
                current = current->parent;
                sel = 0;
            }
        }

        if(ch=='\n'){
            // Exit
            if(current == &root && sel==(root.items.size()-1)) break;

            // Option: Name
            if(current == &opt && sel==0){

                wstring name = inputBox(startY+3*(cellH+1), startX+30, cellW-40, 3, L"Enter Name:");
                string reply = "Hello " + ws2s(name); 
                drawBox(startY+4*(cellH+1)+1,startX+30,cellH+3,cellW-40,true,reply);
                
                getch();
            }

            if(current == &cli && sel==0){

                wstring input_cli = inputBox(startY+2*(cellH+1), startX+10, cellW-20, 3, L"Input command:");
                string linux_output = exec_cli(ws2s(input_cli).c_str());
                // remove_newlines(linux_output);
                // replaceAll(linux_output, "\n", "\r ");
                drawBox_endl(startY+3*(cellH+1)+1,startX+10,cellH+10,cellW-20,true,linux_output);
                getch();
            }

            // Navigate to child menu
            if(current->children.size() > sel && current->children[sel] != nullptr){
                current = current->children[sel];
                sel = 0;
            }
        }
    }

    endwin();
    return 0;
}
