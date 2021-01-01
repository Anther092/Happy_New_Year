#include <iostream>
#include <ncurses.h>
#include <vector>
#include <unistd.h>
using namespace std;

typedef struct {
    int y;
    int x;

    int printY;
    int printX;

    int dy;
    int dx;

    int initX;
    int scope;

} Snowflake;

bool isFall(Snowflake sf, int maxY) {
    if (sf.y > maxY) {
        return true;
    }
    else
        return false;
}

void moveSF(Snowflake& sf) {
    sf.y += sf.dy;
    sf.x += sf.dx;
}

bool isBeyondScope(Snowflake sf) {
    if ( (sf.x) > (sf.initX + sf.scope) || (sf.x) < (sf.initX - sf.scope))
        return true;
    else
        return false;
}

bool isBeyondTerminal(Snowflake sf, int maxX) {
    if ( sf.x < 0 || sf.x > maxX)
        return true;
    else
        return false;
}

void printSF(Snowflake& sf) {
    move(sf.y, sf.x);
    addch('*');
    refresh();

    sf.printY = sf.y;
    sf.printX = sf.x;
}

void cleanOneSF(Snowflake sf) {
    move(sf.printY, sf.printX);
    addch(' ');
    refresh();
}

void spawnSF(vector<Snowflake*>& masSnowflakes, int maxX) {
    for (int i = 0; i < 20; ++i) {
        auto* sf = new Snowflake;
        sf->y = 0;
        sf->x = rand() % maxX;

        sf->printY = sf->y;
        sf->printX = sf->x;

        sf->dy = (rand() % 3) + 1;
        int sign = rand() % 10;
        if (sign > 5)
            sign = 1;
        else
            sign = -1;
        sf->dx = sign*((rand() % 3) + 1);

        sf->initX = sf->x;
        sf->scope = (rand() % 3) + 1;
        masSnowflakes.push_back(sf);
    }
}

int main(){
    initscr();
    srand(time(nullptr));

    int maxY = 0;
    int maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    --maxY;
    --maxX;

    //
    int y = 20;
    int x = 60;
    move(y, x);
    printw(" _   _                           _   _                __   __              \n");
    ++y;
    move(y, x);
    printw("| | | | __ _ _ __  _ __  _   _  | \\ | | _____      __ \\ \\ / /__  __ _ _ __ \n");
    ++y;
    move(y, x);
    printw("| |_| |/ _` | '_ \\| '_ \\| | | | |  \\| |/ _ \\ \\ /\\ / /  \\ V / _ \\/ _` | '__|\n");
    ++y;
    move(y, x);
    printw("|  _  | (_| | |_) | |_) | |_| | | |\\  |  __/\\ V  V /    | |  __/ (_| | |   \n");
    ++y;
    move(y, x);
    printw("|_| |_|\\__,_| .__/| .__/ \\__, | |_| \\_|\\___| \\_/\\_/     |_|\\___|\\__,_|_|   \n");
    ++y;
    move(y, x);
    printw("            |_|   |_|    |___/                                             ");

    y += 5;
    x += 29;
    move(y, x);
    printw("by Anther");

    refresh();
    usleep(5000000);
    //


    vector<Snowflake*> masSnowflakes;

    while(true) {

        spawnSF(masSnowflakes, maxX);

        for (int i = 0; i < masSnowflakes.size(); ++i) {
            printSF(*masSnowflakes[i]);

            moveSF(*masSnowflakes[i]);
            if (isBeyondScope(*masSnowflakes[i]) || isBeyondTerminal(*masSnowflakes[i], maxX)) {

                masSnowflakes[i]->dx = -masSnowflakes[i]->dx;

                while (isBeyondTerminal(*masSnowflakes[i], maxX)) {
                    moveSF(*masSnowflakes[i]);
                }
            }

            if (isFall(*masSnowflakes[i], maxY)) {
                cleanOneSF(*masSnowflakes[i]);
                free(masSnowflakes[i]);
                masSnowflakes.erase(masSnowflakes.begin() + i);
            }
        }

        usleep(500000);

        for (auto& masSnowflake : masSnowflakes) {
            cleanOneSF(*masSnowflake);
        }

        //break;
    }



    endwin();
    return 0;
}
