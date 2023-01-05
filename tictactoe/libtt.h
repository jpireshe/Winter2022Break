#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <curses.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// constants and globals
#define BOARD_ROWS 3
#define BOARD_COLS 3
#define BLUE  "\x1B[34m"
#define RED   "\x1B[31m"
char** board;

// structs
typedef struct cursor {
    int x;
    int y;
    char c;
    char color[10];
} cursor;

// functions
void tt_init();
void tt_end();
void tt_reset();
void server_handle_sigchild(int sig);
void server_handle_incomming_connection(int server_cl);
void print_board(cursor* c);
void send_board(cursor* c, int* server_rc_p, int server_cl);
void get_player(cursor* c, int player_number, int* server_rc_p, int server_cl);
int check_win();
int play_again(int player_number, int* server_rc_p, int server_cl);
int gamemain();