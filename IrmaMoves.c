//
//  IrmaMoves.c
//
//  Solomon Storts 7625
//  Data Structures
//  Florida Polytechnic University
//
//  Last updated: 2/5/2021
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IrmaMoves.h"

//Functional Prototypes

int convertToInt(Move *irmaMove);

int checkForLand(char** board, Move *irmaMove);

void setNewIrmaLocation(char** board, Move *irmaMove);

/*
 convertToInt converts the current_loc.col, which is a character, to an integer index used for the array.
 */
int convertToInt(Move *irmaMove) {
    //switch manually checks for each case
    switch (irmaMove->current_loc.col) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

/*
 checkForLand returns 1 if irma is on land, and 0 if irma is on water
 */
int checkForLand(char** board, Move *irmaMove) {
        if (board[irmaMove->current_loc.row][convertToInt(irmaMove)] != ' ') {
            return 1; //on land
        }
        return 0; //on water
}

/*
 setNewIrmaLocation has two while loops, moving irma first horizontally and then vertically.
 Each while loop checks which direction irma needs to move, moves irma, and then adjusts ws and wg accordingly.
 */
void setNewIrmaLocation(char** board, Move *irmaMove) {
    
    //move horizontally
    while(irmaMove->current_loc.col != irmaMove->to_loc.col) {
        
        if (irmaMove->current_loc.col > irmaMove->to_loc.col) {
            //move left
            irmaMove->current_loc.col -= sizeof(char);
        } else {
            //move right
            irmaMove->current_loc.col += sizeof(char);
        }
        
        if(checkForLand(board, irmaMove)) {
            irmaMove->irma.ws -= 15;
            irmaMove->irma.wg -= 10;
        } else {
            irmaMove->irma.ws += 10;
            irmaMove->irma.wg += 5;
        }
    }

    //move vertically
    while(irmaMove->current_loc.row != irmaMove->to_loc.row) {

        if (irmaMove->current_loc.row > irmaMove->to_loc.row) {
            //move up
            irmaMove->current_loc.row--;
        } else {
            //move down
            irmaMove->current_loc.row ++;
        }

        if(checkForLand(board, irmaMove)) {
            irmaMove->irma.ws -= 2;
            irmaMove->irma.wg -= 1;
        } else {

            irmaMove->irma.ws += 6;
            irmaMove->irma.wg += 3;
        }
    }
}

/*
 createMapBoard dynamically allocates space for a 2D char array with dimensions 8x8, and populates the array with the initial configuration of the map board
 */
char **createMapBoard(void) {
    char** board; //create map board
    board = malloc(sizeof(char*) * 8); //dynamically allocate space
    if (board == NULL) {
        return 0; //call to malloc failed
    }
    for(int i = 0; i < 8; i++) {
        board[i] = malloc(sizeof(char) * 8); //dynamically allocate space
        if (board[i] == NULL) {
            return 0; //call to malloc failed
        }
    }
    
    //populate array with initial configuration
    strncpy(board[0], "FF      ", 8);
    strncpy(board[1], " F      ", 8);
    strncpy(board[2], " FF     ", 8);
    strncpy(board[3], "  F     ", 8);
    strncpy(board[4], "  K     ", 8);
    strncpy(board[5], "C  B    ", 8);
    strncpy(board[6], " CC D   ", 8);
    strncpy(board[7], "  C  DD ", 8);
    
    return board; //return a pointer to dynamically allocated 2D array
}

/*
 destroyMapBoard frees all dynamically allocated memory associated with board
 */
char **destroyMapBoard(char **board) {
    
    for(int i = 0; i < 8; i++) {
        free(board[i]);
    }
    free(board);
    
    return NULL;
}

/*
 printMapBoard takes a pointer to an 8x8 char array and prints the map board represented by that array using the format described in the project file
 */
void printMapBoard(char **board) {
    printf("========\n");
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("========\n\n");
}

/*
 predictIrmaChange starts by printing the map board with Irma in its starting position.
 After printing the initial map board, this function should parse all of the algebraic notation strings passed in through making calls to the parseNotationString() function.
 The printMapBoard() function is called once after the Irma’s location has been identified, and another time after the board’s final configuration has been processed.
 */
char **predictIrmaChange (char* str, Move *irmaMove) {
    //create map board
    char** board = createMapBoard();
    if(board == NULL) {
        return NULL;
    }
    //print initial map board
    printMapBoard(board);
    
    //determine irma's location
    parseNotationString(str, irmaMove);
    irmaMove->current_loc.row = irmaMove->from_loc.row;
    irmaMove->current_loc.col = irmaMove->from_loc.col;
    
    //print updated board with irma's location
    char hold = board[irmaMove->current_loc.row][convertToInt(irmaMove)];
    board[irmaMove->current_loc.row][convertToInt(irmaMove)] = 'I';
    printMapBoard(board);
    board[irmaMove->current_loc.row][convertToInt(irmaMove)] = hold; //set irma's starting space back to it's original character, for the sake of visuals
    
    //print final configuration
    setNewIrmaLocation(board, irmaMove);
    board[irmaMove->current_loc.row][convertToInt(irmaMove)] = 'I';
    printMapBoard(board);
    
    return board;
}

/*
 parseNotationString parses str and extracts information about Irma moves encoded there, and populate all corresponding fields in the struct pointed to by irmaMove
 */
void parseNotationString(char* str, Move* irmaMove){
    
    //initialize to x or -1 for unknown values
    irmaMove->from_loc.col = 'x';
    irmaMove->current_loc.col = 'x';
    irmaMove->to_loc.col = 'x';
    irmaMove->from_loc.row = -1;
    irmaMove->current_loc.row = -1;
    irmaMove->to_loc.row = -1;
    
    //extract information from str
    irmaMove->from_loc.col = str[0];
    irmaMove->from_loc.row = str[1] - '0'; //use "- '0'" to convert to proper integer value
    irmaMove->irma.ws = ((str[3] - '0') * 100) + ((str[4] - '0') * 10) + (str[5] - '0'); //use coversions and multiplication in order to convert three characters to a 3-digit integer
    irmaMove->irma.wg = ((str[7] - '0') * 100) + ((str[8] - '0') * 10) + (str[9] - '0');
    irmaMove-> to_loc.col = str[11];
    irmaMove-> to_loc.row = str[12] - '0';
}

/*
 difficultyRating returns a double from 1.0 - 5.0 indicating the difficulty of this assignment
 */
double difficultyRating(void) {
    return 3.0;
    
    //It took me a little bit to understand how irma moved and how I was meant to relate my functions to that, but once I understood the code came quickly
}

/*
 hoursSpent returns a double indicating the number of hours I spent on this assignment
 */
double hoursSpent(void) {
    return 3.5;
}
