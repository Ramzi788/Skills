#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define max(a,b) (((a) > (b)) ? (a) : (b)) // Finds the max between two elements
#define min(a,b) (((a) < (b)) ? (a) : (b)) //  Find the min between two elements

//Defining game board 
int gameBoard_NORZAK[6][7];
int ROWS_NORZAK = 6;
int COLS_NORZAK = 7;

//User Input Global Variables
const int name_length_NORZAK = 50; 
char name_NORZAK[name_length_NORZAK];
char line_NORZAK[name_length_NORZAK];

//Indicators for Player and AI Bot
const int PLAYER_NORZAK = 1;
const int BOT_NORZAK = 2;
int turn_NORZAK = 0; 
int identifyPlayer_NORZAK;// Identify whos turn it is
int maxDepth_NORZAK = 5;
bool gameOver_NORZAK = false;// Bool Flag for when the game is Over. Either a tie or the PLAYER_NORZAK/BOT has won. 

void playGame_NORZAK();
void printBoard_NORZAK(int* board);
int getScore_NORZAK(int playerTurn, int botTurn, int freeSpot);
int scoreSet_NORZAK(int array[], int move);
void Turn_NORZAK(int* board, int col, int move);
int trackScore_NORZAK(int * board, int move);
bool Won_NORZAK(int * board , int move);
int * CopyBoard_NORZAK(int*cboard, int *board);
int* minimax_NORZAK(int * board, int depth, int alpha, int beta, int move);
int aiTurn_NORZAK();
bool isDigit_NORZAK(char string[name_length_NORZAK]);
int coinFlip();
void userInput_NORZAK(int *cboard);

int main(){
    playGame_NORZAK();
}
    

/*
Requires: No arguments required
Effect: Begins the connect4 game while also allowing the user to pick their desired difficulty for the bot. 
*/
void playGame_NORZAK(){
    printf("Enter Name: ");
    scanf("%s", name_NORZAK);
    int result = coinFlip(); 
    identifyPlayer_NORZAK = result; // result of the coin flip (1 or 2)
    switch (identifyPlayer_NORZAK)
    {
        case 1: printf("Player starts\n");
        break;

        case 2: printf("Bot starts\n");
        break;
    }
    
    printf("Please select your desired level: ");
    printf("\n Easy: 1 \n Medium: 2 \n Hard: 3\n ");
    int selectLevel;
    scanf("%d",&selectLevel);
    switch (selectLevel)
    {
    case 1: 
        maxDepth_NORZAK = 1; // Easy Difficulty
        break;
    case 2: 
        maxDepth_NORZAK = 5; // Medium Difficulty 
        break;
    
    case 3: 
        maxDepth_NORZAK = 10; // Hard Difficulty
    }
    if (identifyPlayer_NORZAK == PLAYER_NORZAK){
        printf("Player starts..\n");
    }
    else if (identifyPlayer_NORZAK == BOT_NORZAK)
    {
        printf("BOT starts..\n");
    }


    int *newBoard = &gameBoard_NORZAK[0][0]; // Creates a new board to send to print board
    printBoard_NORZAK(newBoard);
    while(!gameOver_NORZAK){ // Loops until a tie or a winning condition occurs
        if(identifyPlayer_NORZAK == PLAYER_NORZAK){ 
            userInput_NORZAK(newBoard);
        }
        else if(identifyPlayer_NORZAK == BOT_NORZAK){
            Turn_NORZAK(newBoard, aiTurn_NORZAK(), BOT_NORZAK);
        }
        else if(turn_NORZAK == COLS_NORZAK*ROWS_NORZAK) // Number of turns has reached its limit
            gameOver_NORZAK = true;

        gameOver_NORZAK = Won_NORZAK(&gameBoard_NORZAK[0][0], identifyPlayer_NORZAK); // Checks for the winner 
        identifyPlayer_NORZAK = (identifyPlayer_NORZAK== 1) ? 2 : 1; // If 1, it is players move, if 2 it is BOTS move
        turn_NORZAK++;
        printBoard_NORZAK(newBoard);


    }
    if(turn_NORZAK == COLS_NORZAK*ROWS_NORZAK) // Board is full
        printf("No more moves allowed, game ends in a draw. \n");
    else{
        if(identifyPlayer_NORZAK == PLAYER_NORZAK)
            printf("Bot Wins.\n");
        else
            printf("%s,  has won.\n", name_NORZAK);
    }
}
    /*
    Requires: pointer to board
    Effect: prints the board 
    */
    void printBoard_NORZAK(int* board){
        printf("---------------\n");
        for(int i = 0; i < ROWS_NORZAK; i++){
            for(int j = 0; j < COLS_NORZAK; j++)
                printf("|%d", *(board + COLS_NORZAK * (ROWS_NORZAK - i - 1) + j));
            printf("|\n");
        }
        printf("---------------\n");
    }

    /*
    Requires: pointer to a copy of the board, and a pointer to the original board
    Effect: The point of this method is for the ai to simulate a move before actually performing it in the minimax algorithm. 
    This will allow the bot check if their move will put them in an advantage. However, the Bot's analysis of the 
    efficiency of the move of course depends on the difficulty the player chose initially. 
    */
    int * CopyBoard_NORZAK(int*cboard, int *board) {
        int count = 0;
        for (int i = 0; i < ROWS_NORZAK; i++){
            for(int j = 0; j < COLS_NORZAK; j++)
                *(&cboard[i + ROWS_NORZAK * count] + j) = *(&board[i + ROWS_NORZAK * count] + j);
            count++;
        }
        return cboard;
    }


    /*
    Requires: Player's turn, bot's turn, and a freeSpot variable that checks for empty spots in the board.
    Effect: Gets the current score for the player. If it is positive, the player is currently in a more a advanced stage in the game, if negative the bot is ahead. 
    */
    int getScore_NORZAK(int playerTurn, int botTurn, int freeSpot){
        int current_score = 0;
        if(playerTurn == 4)
            current_score += 100001;
        else if(playerTurn == 3 && freeSpot== 1)
            current_score += 1000;
        else if(playerTurn == 2 && freeSpot == 2)
            current_score += 100;
        else if(botTurn == 2 && freeSpot == 2)
            current_score -= 101;
        else if(botTurn == 3 && freeSpot == 1)
            current_score -= 1001;
        else if(botTurn == 4)
            current_score -= 100000;
        return current_score;
    }

    /*
    Requires: array containing the moves, and an integer move
    Effect: Returns the moves of the player, bot and the empty spots available. It then calls the getScore_NORZAK
    to calculate the score.  
    */
    int scoreSet_NORZAK(int array[], int move){
        int playerTurn = 0;
        int botTurn = 0;
        int empty = 0;
        for(int i = 0; i < 4; i++){
            if(array[i] == move)
                playerTurn++;
            if(array[i] == PLAYER_NORZAK|| array[i] == BOT_NORZAK)
                botTurn++;
            if(array[i] == 0)
                empty++;
        }
        botTurn -= playerTurn;
        return getScore_NORZAK(playerTurn, botTurn, empty);
    }


/*
Requires: pointer to game board, column integer variable, and move int variale
Effect: places the move entered at the least possible row (down).

Test Case: 
1. If move = 2, the function will place the move in the least possible row of column 2. 
*/
void Turn_NORZAK(int* board, int col, int move){
    for(int i = 0; i < ROWS_NORZAK; i++){
        if(*(board + COLS_NORZAK * i + col) == 0) {
            *(board + COLS_NORZAK * i + col) = move;
            break;
        }
    }
}

/*
Requires: pointer to board
Effect: Allows the bot to check if there is a possible 4 in a row in any direction. It can also allow the bot to
know that the user has a possible 4 in a row and thus will block the player.
*/
int trackScore_NORZAK(int * board, int move) {
    int score = 0;
    int rows[7];
    int cols[6];
    int set[4];

    //Check for 4 in a row Horizontally 
    for (int i = 0; i < ROWS_NORZAK; i++) {
        for (int j = 0; j < COLS_NORZAK; j++) {
            rows[j] = *(board + i * COLS_NORZAK + j);
        }
        for (int j = 0; j < COLS_NORZAK - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = rows[j + k];
            }
            score += scoreSet_NORZAK(set, move);
        }
    }
    //Check for 4 in a row Diagonally 
    for (int i = 0; i < COLS_NORZAK; i++) {
        for (int j = 0; j < ROWS_NORZAK; j++) {
            cols[j] = *(board + j * COLS_NORZAK + i);
        }
        for (int j = 0; j < ROWS_NORZAK - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = cols[j + k];
            }
            score += scoreSet_NORZAK(set, move);
        }
    }
    //Check for 4 in a row Diagonally 
    for (int i = 0; i < ROWS_NORZAK - 3; i++) {
        for (int j = 0; j < COLS_NORZAK; j++) {
            rows[j] = *(board + i * COLS_NORZAK + j);
        }
        for (int j = 0; j < COLS_NORZAK - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = *(board + COLS_NORZAK * (i + k) + j + k);
            }
            score += scoreSet_NORZAK(set, move);
        }
    }
    for (int i = 0; i < ROWS_NORZAK - 3; i++) {
        for (int j = 0; j < COLS_NORZAK; j++) {
            rows[j] = *(board + i * COLS_NORZAK + j);
        }
        for (int j = 0; j < COLS_NORZAK - 3; j++) {
            for (int k = 0; k < 4; k++) {
                set[k] = *(board + COLS_NORZAK * (i + 3 - k) + j + k);
            }
            score += scoreSet_NORZAK(set, move);
        }
    }
    return score;
}
    /*
    Requires: pointer to board, move int
    Effect: Checks if there is 4 in a row to determine the winner of the game. 
    */
    bool Won_NORZAK(int * board , int move) {
        int count = 0;
        //Check Horizontal
        for (int i = 0; i < COLS_NORZAK - 3; i++) {
            for (int j = 0; j < ROWS_NORZAK; j++) {
                for (int k = 0; k < 4; k++) {
                    if (*(board + COLS_NORZAK * j + i + k) == move) 
                        count++;
                    if (count == 4)
                        return true;
                }
                count = 0;
            }
        }
        //Vertical
        for (int i = 0; i < COLS_NORZAK; i++) {
            for (int j = 0; j < ROWS_NORZAK - 3; j++) {
                for (int k = 0; k < 4; k++) {
                    if (*(board + COLS_NORZAK * (j + k) + i) == move)
                        count++;
                    if (count == 4)
                        return true;
                }
                count = 0;
            }
        }
        //Diagonals
        for (int i = 0; i < COLS_NORZAK - 3; i++) {
            for (int j = 3; j < ROWS_NORZAK; j++) {
                for (int k = 0; k < 4; k++) {
                    if (*(board + COLS_NORZAK * (j - k) + i + k) == move)
                        count++;
                    if (count == 4)
                        return true;
                }
                count = 0;
            }
        }
        for (int i = 0; i < COLS_NORZAK - 3; i++) {
            for (int j = 3; j < ROWS_NORZAK - 3; j++) {
                for (int k = 0; k < 4; k++) {
                    if (*(board + COLS_NORZAK * (j + k) + i + k) == move) 
                        count++;
                    if (count == 4)
                        return true;
                }
                count = 0;
            }
        }
        return false;
    }
    /*
    Requires: No arguments required
    Effect: prints that the bot is thinking of a move, then calls the minimax algorithm. 
    */
    int aiTurn_NORZAK(){
        printf("Bot is thinking of a move\n");
        return minimax_NORZAK(&gameBoard_NORZAK[0][0], maxDepth_NORZAK, INT32_MIN, INT32_MAX, BOT_NORZAK)[1];
    }
    /*
    Requires: board pointer, depth variable to be used recursively, alpha and beta variables for pruning, and move. 
    Effect: Minimax is a decision-making algorithm, typically used in a turn-based, two player games. 
    The goal of the algorithm is to find the optimal next move. The algorithm is recursive and uses a tree 
    to find the bot's next optimal move. The algorithm must first reach the lowest possible level in the tree. 
    Every node of the tree represents a possible position for the bot. At each level above the external nodes,
    the node will pick either the max or min value from leaf level and the selection of min and max will alternate every level. 
    Furthermore, to save time, the idea of pruning is involved. This idea will be shown through test cases.

    Test Cases: 
    1.

                min(y)-->        4
                max(x)-->    4       >=5
                Leafs:     3   4    5   ?

                In this example, since we knew the left child of the second max chooser will be 5, the other 
                value of the right child will not matter. This is because the parent y ,will know that the left 
                child is 4 and that the right child is at least 5. Thus, y will choose 4. This is the idea of 
                pruning. 
    */

    int* minimax_NORZAK(int * board, int depth, int alpha, int beta, int move) {
        int array[2];
        if (depth == 0 || depth >= (ROWS_NORZAK*COLS_NORZAK) - turn_NORZAK) {
            array[0] = trackScore_NORZAK(board, BOT_NORZAK);
            array[1]  = -1;
            return &array[0];
        }
        if(move == BOT_NORZAK){
            array[0] = INT32_MIN;
            array[1] = -1;
            if(Won_NORZAK(board, PLAYER_NORZAK))
                return &array[0];
            for(int i = 0; i < COLS_NORZAK; i++){
                if(*(&board[35] + i) == 0){
                    int temp[6][7];
                    int *p = &temp[0][0];
                    p = CopyBoard_NORZAK(p, board);
                    Turn_NORZAK(p, i, move);
                    int score = (minimax_NORZAK(p, depth - 1,alpha, beta, PLAYER_NORZAK))[0];
                    if(score > array[0]){
                        array[0] = score;
                        array[1] = i;
                    }
                    alpha = max(alpha, array[0]);
                    if(alpha >= beta) // pruning
                        break;
                }
            }
            return array;
        }
        else {
            array[0] = INT32_MAX;
            array[1] = -1;
            if (Won_NORZAK(board, BOT_NORZAK))
                return &array[0];
            for (int i = 0; i < COLS_NORZAK; i++) {
                if (*(&board[35] + i) == 0) {
                    int temp[6][7];
                    int *p = &temp[0][0];
                    p = CopyBoard_NORZAK(p, board);
                    Turn_NORZAK(p, i, move);
                    int score = minimax_NORZAK(p, depth - 1, alpha, beta, BOT_NORZAK)[0];
                    if (score < array[0]) {
                        array[0] = score;
                        array[1] = i;
                    }
                    beta = min(beta, array[0]); 
                    if (alpha >= beta) //pruning
                        break;
                }
            }
            return array;
        }
    }

    
    /*
    Requires: User input variable that is a string.
    Effect: Checks if the string can be used in the user_Input_NORZAK algorithm. 
    */
    bool isDigit_NORZAK(char user_input[name_length_NORZAK]){
        if(strlen(user_input) == 0)
            return false;

        for(int i = 0; i < strlen(user_input); i++){
            if(isdigit(user_input[i]) == 0)
                return false;
        }
        return true; 
    }
    /*
    Requires: No Arguments Required
    Effect: Returns the value of coin flip, 1 for player, 2 for bot. 
    */
    int coinFlip(){
        srand (time(NULL));
        int result = rand()%2 + 1;
        return result;
    }

    /*
    Requires: a pointer to the board
    Effects: Checks if the user input is valid, i.e. an integer value, then places that move into the board. 

    Test Cases: 
    1. Input: y -->Output: Invalid input, please enter an integer between 1 and 7: 
    2. Input: 12y --> Output: Invalid input, please enter an integer between 1 and 7:
    3. Input: -1 --> Output: Enter a number between 1 & 7: 
    4. Input: 2 and Column 2 is full --> No more free spaces in column, please try again:
    */
    void userInput_NORZAK(int *cboard){
        int c = -1;
        char user_input[name_length_NORZAK]; // Make input as a string to check if valid input then convert to integer. 
        printf("%s", "Enter column: ", name_NORZAK);
        scanf("%s", user_input);
        while (c == -1) {
            while (!isDigit_NORZAK(user_input)) {   // String must contain only digits
                printf("Invalid input, please enter an integer between 1 and 7: ");
                scanf("%s", user_input);  
                fgets(line_NORZAK, name_length_NORZAK, stdin); // Stores 
            }
            c = atoi(user_input) -1; 
            while (c < 0 || c > 6 || gameBoard_NORZAK[ROWS_NORZAK - 1][c] != 0) {
                if (c < 0 || c > 6|| !isDigit_NORZAK(user_input)) // Could be a number out of bounds or the user inputted a string that doesn't only contain digits. 
                    printf("Enter a number between 1 & 7: ");
                else if (gameBoard_NORZAK[ROWS_NORZAK - 1][c] != 0) // No more free spots in column
                    printf("No more free spaces in column, please try again: ");
                    scanf("%s", user_input);  
                    fgets(line_NORZAK, name_length_NORZAK, stdin);
            
            }
        }
        Turn_NORZAK(cboard, c, PLAYER_NORZAK);
    }
