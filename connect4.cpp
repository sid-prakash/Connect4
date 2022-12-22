#include <iostream>
#include <ncurses.h>
#include <stdlib.h>

#include "connect4.h"

using namespace std;

struct player_info;
struct drop_info;

int main()
{
	player_info player_one, player_two;
	char board[ROWS][COLUMN];

    io_init_terminal();

	intro_message(&player_one, &player_two);
	
    init_board(board);
	print_board(board);


    do {
        drop_info dropOne = player_drop(board, player_one);
        print_board(board);
        if(check_win(dropOne, board, player_one)) {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvprintw(10, 1, "%s Connected Four, You Win!", player_one.playerName.c_str());
            if(restart(board) == 1) {
                print_board(board);
                continue;
            }
            else {
                break;
            }
        }

        drop_info dropTwo = player_drop(board, player_two);
        print_board(board);
        if(check_win(dropTwo, board, player_two)) {
            attron(COLOR_PAIR(COLOR_WHITE));
            mvprintw(10, 1, "%s Connected Four, You Win!", player_two.playerName.c_str());
            if(restart(board) == 1) {
                print_board(board);
                continue;
            }
            else {
                break;
            }
        }

    } while(true);

    endwin();

    return 0;
}

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

std::string get_string()
{
    std::string input;

    nocbreak();
    echo();

    int ch = getch();
    while (ch != '\n')
    {
        input.push_back(ch);
        ch = getch();
    }

    cbreak();
    noecho();

    return input;
}

void intro_message (player_info *player_one, player_info *player_two) {
    mvprintw(1, 1, "Let's Play Connect 4");
	mvprintw(3, 1, "Player One please enter your name: ");
    refresh();
	(*player_one).playerName = get_string();
	(*player_one).playerID = 'X';

	mvprintw(4, 1, "Player Two please enter your name: ");
    refresh();
	(*player_two).playerName = get_string();
	(*player_two).playerID = 'O';
    refresh();
    getch();
    clear();
}

void init_board (char board[][COLUMN])
{
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMN; j++) {
            board[i][j] = '_';
        }
    }
}

void print_board (char board[][COLUMN])
{
	int i, j;

    clear();

    move(1, 1);
    printw(" ");
    for(j = 1; j <= COLUMN; j++)
    {
        attron(COLOR_PAIR(COLOR_GREEN));
        printw(" %d", j);
    }

    move(2, 1);
	for(i = 0; i < ROWS; i++)
	{
        attron(COLOR_PAIR(COLOR_BLUE));
        printw("| ");
		for(j = 0; j < COLUMN; j++)
		{
            if(board[i][j] == 'X') {
                attron(COLOR_PAIR(COLOR_YELLOW));
            }
            else if(board[i][j] == 'O') {
                attron(COLOR_PAIR(COLOR_RED));
            }
            else {
                attron(COLOR_PAIR(COLOR_BLUE));
            }
            printw("%c ", board[i][j]);
		}
        attron(COLOR_PAIR(COLOR_BLUE));
		printw("| ");
        move(i + 3, 1);
	}

    refresh();
}

int get_valid_column() {

    int dropChoice = getch();
    while(dropChoice < 49 || dropChoice > 55) {
        attron(COLOR_PAIR(COLOR_WHITE));
        move(11, 0);
        clrtoeol();
        mvprintw(11, 1, "Try Again");
        refresh();
        dropChoice = getch();
    }
    move(11, 0);
    clrtoeol();
    
    return dropChoice -= 49;
}

drop_info player_drop(char board[][COLUMN], player_info activePlayer)
{
    attron(COLOR_PAIR(COLOR_WHITE));
    mvprintw(10, 1, "%s's Turn. Please enter a number between 1 and 7: ", activePlayer.playerName.c_str());
    refresh();
    int dropChoice = get_valid_column();

    while (board[0][dropChoice] != '_')
    {
        attron(COLOR_PAIR(COLOR_WHITE));
        mvprintw(11, 1, "This row is full, please enter a new row: ");
        refresh();
        dropChoice = get_valid_column();
    }

    int i = ROWS - 1;
    while(board[i][dropChoice] != '_') i--;

    board[i][dropChoice] = activePlayer.playerID;

    drop_info drop;
    drop.y = i;
    drop.x = dropChoice;


    return drop;
}

bool check_win(drop_info lastDrop, char board[][COLUMN], player_info player) {
    int y = lastDrop.y;
    int x = lastDrop.x;
    char playerId = player.playerID;
    int count = 0;

    // horizontal
    for(int i = 0; i < COLUMN; i++) {
        if(board[y][i] == playerId) {
            count++;
            if(count == 4) {
                return true;
            }
        }
        else {
            count = 0;
        }
    }

    // vertical
    for(int i = 0; i < ROWS; i++) {
        if(board[i][x] == playerId) {
            count++;
            if(count == 4) {
                return true;
            }
        }
        else {
            count = 0;
        }
    }

    // diagonal top left - bottom right
    for (int i = 0; i < ROWS - 3; i++)
    {
        for (int j = 0; j < COLUMN - 3; j++)
        {
            if (board[i][j] == playerId && board[i][j] == board[i+1][j+1] && 
                board[i][j] == board[i+2][j+2] && board[i][j] == board[i+3][j+3])
            {
                return true;
            }
        }
    }

    // diagonal top right - bottom left
    for (int i = 0; i < ROWS - 3; i++)
    {
        for (int j = 3; j < COLUMN; j++)
        {
            if (board[i][j] == playerId && board[i][j] == board[i+1][j-1] && 
                board[i][j] == board[i+2][j-2] && board[i][j] == board[i+3][j-3])
            {
                return true;
            }
        }
    }

    return false;
}

bool is_full(char board[][COLUMN])
{
	int full = 0;
	for (int i = 0; i < COLUMN; ++i)
	{
		if (board[1][i] != '_')
			full++;
	}
    return full == 7;
}

int restart (char board[][COLUMN])
{
    attron(COLOR_PAIR(COLOR_WHITE));
	mvprintw(11, 1, "Would you like to restart? Yes(1) No(2): ");
    refresh();
	int restart = getch() - 48;
	if (restart == 1) init_board(board);
    else {
        clear();
        attron(COLOR_PAIR(COLOR_WHITE));
        mvprintw(1, 1, "Goodbye!");
        refresh();
        getch();
    }

    return restart;
}