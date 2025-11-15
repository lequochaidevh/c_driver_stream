g++ table2.cpp -o table -lncurses


lncursesw

sudo apt install libncurses5-dev libncursesw5-dev


        // timeout(60); // Good

        //nodelay(stdscr, TRUE);
        // timeout(60); // Good
        int ch = getch();
        if (ch == 'q') break;

chafa 1.jpeg --symbols braille
ls


screen -S Session-name -t Window-title
Ctrl-a + |  and Ctrl-a + S

Ctrl-a + tab and Ctrl-a + C.

screen -X quit
Ctrl-a + c – create a new window.
Ctrl-a + p and n – move to the previous and next window.
Ctrl-a + w – display the Terminal window list.
Ctrl-a + A – rename the current window.
Ctrl-a + : – enter the command mode.
Ctrl-a + x – close the current window.
Ctrl-a + a – cancel the initial Ctrl-a shortcut.

ffmpeg -i 3.png -vf "scale=1920:1080:force_original_aspect_ratio=decrease" resized.png