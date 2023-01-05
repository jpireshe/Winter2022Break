#include "libtt.h"

void tt_init() {
    // malloc space for the board rows
    board = (char**) malloc(BOARD_ROWS*sizeof(char*));  

    // now malloc space for each col in the row         
    for(int i = 0; i < BOARD_ROWS; i++) {
        board[i] = (char*) malloc(BOARD_COLS*sizeof(char));

        // and populate it!
        for(int j = 0; j < BOARD_COLS; j++) {
            board[i][j] = '#';
        }
    }
    /*
        # # #
        # # #
        # # #
    */
}

void tt_reset() {
    for(int i = 0; i < BOARD_ROWS; i++) {
        memset(board[i], '#', BOARD_COLS);
    }
}

void tt_end() {
    for(int i = 0; i < BOARD_ROWS; i++) free(board[i]);
    free(board);
}

void print_board(cursor* c) {
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            if(c != NULL) {
                if(i == c->y && j == c->x) printf("%s%c%s ", c->color, c->c, "\x1B[0m");
                else printf("%c ", board[i][j]);
            }
            else printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void get_player(cursor* c, int player_number, int* server_rc_p, int server_cl) {
    // networking stuff
    int buflen = 100;
    char* buf = malloc(buflen*sizeof(char));
    memset(buf, 0, buflen);

    // flag to check if your player has put valid data
    int valid = 0;

    // get user input
    char received[50];
    char user_input;

    while(!valid && *server_rc_p > 0) {
        // show board
        // system("clear");
        send_board(c, server_rc_p, server_cl);
        
        // prompt according to player number:
        if(player_number) {
            sprintf(buf, "Player 1 (X), input position (move with WASD, set with P): ");
            *server_rc_p = write(server_cl, buf, strlen(buf));
            memset(buf, 0, buflen);
        } 
        else {
            sprintf(buf, "Player 2 (O), input position (move with WASD, set with P): ");
            *server_rc_p = write(server_cl, buf, strlen(buf));
            memset(buf, 0, buflen);
        }

        // this really was not the best method, but
        // i could not think of anything better..
        *server_rc_p = read(server_cl, received, 50);
        user_input = received[0];

        // use player inputs
        if(user_input == 'W' || user_input == 'w')
            {if (c->y > 0) c->y--;}
        
        else if(user_input == 'S' || user_input == 's')
            {if (c->y < BOARD_ROWS - 1) c->y++;}

        else if(user_input == 'A' || user_input == 'a')
            {if (c->x > 0) c->x--;}

        else if(user_input == 'D' || user_input == 'd')
            {if (c->x < BOARD_COLS - 1) c->x++;}

        else if(user_input == 'P' || user_input == 'p')
            {if (board[c->y][c->x] == '#') valid = 1;}
        
        else continue;
    }

    board[c->y][c->x] = c->c;

    free(buf);
}

int check_win() {
    // check for horizontal wins
    for(int i = 0; i < BOARD_ROWS; i++) {
        if((board[i][0] == board[i][1]) && (board[i][0] == board[i][2])) {
            if (board[i][0] == 'X') return 1;
            else if (board[i][0] == 'O') return 2;
        }
    }

    // check for vertical wins
    for(int i = 0; i < BOARD_COLS; i++) {
        if((board[0][i] == board[1][i]) && (board[0][i] == board[2][i])) {
            if (board[0][i] == 'X') return 1;
            else if (board[0][i] == 'O') return 2;
        }
    }

    // check for diagonal wins (only 2 possibilities)
    // if I change the board dimension this is one of the few parts that need change
    if ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2])) {
        if(board[0][0] == 'X') return 1;
        else if (board[0][0] == 'O') return 2;
    }

    if ((board[0][2] == board[1][1]) && (board[0][2] == board[2][0])) {
        if(board[0][2] == 'X') return 1;
        else if (board[0][2] == 'O') return 2;
    }

    // check for NO WINNER
    int no_winner = 1;
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            if (board[i][j] == '#') {no_winner = 0; break;}
        }
        if (no_winner == 0) break;
    }
    if(no_winner) return 3;

    return 0;
}

int play_again(int player_number, int* server_rc_p, int server_cl) {
    char server_returning_buf[100];
    char received[50] = "a";
    while(received[0] != '1' && received[0] != '0' && *server_rc_p > 0) {
        sprintf(server_returning_buf, "Player %d, do you wish to play again? Input 1 for yes, 0 for no: ", player_number);
        *server_rc_p = write(server_cl, server_returning_buf, strlen(server_returning_buf));
        memset(server_returning_buf, 0, 100);

        memset(received, 0, 50);
        *server_rc_p = read(server_cl, received, 50);
    }
    int ret = atoi(&received[0]);

    return ret;
}

void server_handle_sigchild(int sig) {
    int ch = wait(NULL);
}

void send_board(cursor* c, int* server_rc_p, int server_cl) {
    int buflen = 50;
    char* buf = malloc(buflen*sizeof(char));
    memset(buf, 0, buflen);

    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            if(c != NULL) {
                if(i == c->y && j == c->x) {
                    sprintf(buf, "%s%c%s ", c->color, c->c, "\x1B[0m");
                    *server_rc_p = write(server_cl, buf, strlen(buf));
                    memset(buf, 0, buflen);
                }
                else {
                    sprintf(buf, "%c ", board[i][j]);
                    *server_rc_p = write(server_cl, buf, strlen(buf));
                    memset(buf, 0, buflen);
                }
            }
            else {
                sprintf(buf, "%c ", board[i][j]);
                *server_rc_p = write(server_cl, buf, strlen(buf));
                memset(buf, 0, buflen);
            }
        }
        sprintf(buf, "\n");
        *server_rc_p = write(server_cl, buf, strlen(buf));
        memset(buf, 0, buflen);
    }
    sprintf(buf, "\n");
    *server_rc_p = write(server_cl, buf, strlen(buf));
    memset(buf, 0, buflen);

    free(buf);
}

void server_handle_incomming_connection(int server_cl) {
    int server_buflen = 256;
    // READ: server incoming buf
    char* server_incoming_buf = malloc(server_buflen*sizeof(char));
    memset(server_incoming_buf, 0, server_buflen);
    // WRITE: server return buf
    char* server_returning_buf = malloc(server_buflen*sizeof(char));
    memset(server_returning_buf, 0, server_buflen);
    // for connection purposes
    int server_rc = 1;

//////**** MAKE THE GAME HAPPEN HERE ****//////

    // control variable for the winner
    int winner = 0;
    // player cursor
    cursor c1 = {0, 0, 'X', RED};
    cursor c2 = {0, 0, 'O', BLUE};
    // initiate board
    tt_init();
    // print initial menu
    strcpy(server_returning_buf, "Tic Tac Toe Game!\n\n");
    write(server_cl, server_returning_buf, strlen(server_returning_buf));
    memset(server_returning_buf, 0, server_buflen);

    while(server_rc > 0) {
        while(winner == 0 && server_rc > 0) {
            // player 1 input 
            get_player(&c1, 1, &server_rc, server_cl);
            winner = check_win();                                   // what if we made the player's pc check for the win?

            // reset p1 cursor
            c1.x = 0; c1.y = 0;
            if(winner) break;

            // player 2 input
            get_player(&c2, 0, &server_rc, server_cl);
            winner = check_win();
            
            // reset p2 cursor
            c2.x = 0; c2.y = 0;
        }

        // print results
        if(winner == 3) {
            sprintf(server_returning_buf, "No winner!\n");
            server_rc = write(server_cl, server_returning_buf, strlen(server_returning_buf));
            memset(server_returning_buf, 0, server_buflen);
        }
        else {
            sprintf(server_returning_buf, "Player %d wins!\n", winner);
            server_rc = write(server_cl, server_returning_buf, strlen(server_returning_buf));
            memset(server_returning_buf, 0, server_buflen);
        }

        // reset board
        tt_reset();
        winner = 0;
        
        // prompt to play again
        if(!play_again(1, &server_rc, server_cl) || !play_again(2, &server_rc, server_cl)) break;
    }

    // end and close program
    tt_end();
    free(server_incoming_buf);
    free(server_returning_buf);
    close(server_cl);
}