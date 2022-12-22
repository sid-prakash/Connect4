#ifndef CONNECT4_H
# define CONNECT4_H

#include <iostream>
#include <stdlib.h>
using namespace std;

struct player_info
{
	std::string playerName;
	char playerID;
};

struct drop_info
{
	int y, x;
};

const int ROWS = 6; // number of rows in the game board
const int COLUMN = 7; // number of columns in the game board

void io_init_terminal(void);
std::string get_string();
void intro_message (player_info *player_one, player_info *playerwo);
void init_board (char board[][COLUMN]);
void print_board (char board[][COLUMN]);
int get_valid_column();
drop_info player_drop(char board[][COLUMN], player_info activePlayer);
bool check_win (drop_info drop, char board[][COLUMN], player_info player);
bool is_full(char board[][COLUMN]);
int restart (char board[][COLUMN]);

#endif