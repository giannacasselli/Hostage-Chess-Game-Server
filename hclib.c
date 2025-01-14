/*
CIS 2750 F24 Assignment 2
Gianna Casselli
1083289
October 22 2024, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hclib.h"

//new board function
exboard_t *newboard(){
    //check if malloc fails
    exboard_t *newboard = malloc(sizeof(exboard_t));
    if(newboard==NULL){
        return NULL;
    }
    //initialize all currently empty strings with null terminator
    memset(newboard->bprison, '\0', sizeof(newboard->bprison));
    memset(newboard->bairfield, '\0', sizeof(newboard->bairfield));
    //white piece characters
    newboard->board[0][0]='R';
    newboard->board[0][1]='N';
    newboard->board[0][2]='B';
    newboard->board[0][3]='Q';
    newboard->board[0][4]='K';
    newboard->board[0][5]='B';
    newboard->board[0][6]='N';
    newboard->board[0][7]='R';
    int i,j;
    //pawns
    for(i=0;i<8;i++){
        newboard->board[1][i]='P';
        for(j=2;j<6;j++){
            newboard->board[j][i]=32;
        }
        //opposite side of the board is same ascii chars but lower case - i.e. -32
        newboard->board[6][i]='p';
        newboard->board[7][i]=newboard->board[0][i]+32;
    }
    memset(newboard->wprison, '\0', sizeof(newboard->wprison));
    memset(newboard->wairfield, '\0', sizeof(newboard->wairfield));
    return newboard;
}

//copy board function
exboard_t *copyboard(exboard_t *board){
    //check if board to be copied is null
    if(board==NULL){
        return NULL;
    }
    exboard_t *cpyboard = malloc(sizeof(exboard_t));
    //check if malloc fails
    if(cpyboard==NULL){
        return NULL;
    }
    //copy memory
    memcpy(cpyboard,board,sizeof(exboard_t));
    return cpyboard;
}

//func to print board
char *stringboard(exboard_t *board){
    //check if board is NULL
    if(board==NULL){
        return NULL;
    }
    //allocate memory for 161 chars + Null terminator
    char *strboard=malloc(sizeof(char)*162);
    if(strboard == NULL){
        return NULL;
    }
    //initialize full board of spaces
    memset(strboard, ' ', 161);
    //last char is null termonator
    strboard[161]='\0';
    int i;
    //set every 9th char to new line
    //char 162 at [161] is supposed to be newline and null terminator ?
    //as per wording in the assignment
    for(i=8; i<162; i+=9){
        strboard[i]='\n';
    }
    strboard[161]='\0';
    //set rest of string chars
    int bp_flag = 0;
    int ba_flag = 0;
    int wp_flag = 0;
    int wa_flag = 0;
    for(i=0; i<8; i++){
        if(bp_flag == 0){
            if(board->bprison[i] != '\0'){
                strboard[i]=board->bprison[i];
            }
            else{
                bp_flag = 1;
            }
        }
        if(bp_flag == 0){
            if(board->bprison[i+8]!= '\0'){
                strboard[i+9]=board->bprison[i+8];
            }
            else{
                bp_flag = 1;
            }
        }
        if(ba_flag == 0){
            if(board->bairfield[i] != '\0'){
                strboard[i+18]=board->bairfield[i];
            }
            else{
                ba_flag = 1;
            }
        }
        if(ba_flag == 0){
            if(board->bairfield[i+8] != '\0'){
                strboard[i+27]=board->bairfield[i+8];
            }
            else{
                ba_flag = 1;
            }
        }
        if(wa_flag == 0){
            if(board->wairfield[i] != '\0'){
                strboard[i+126]=board->wairfield[i];
            }
            else{
                wa_flag = 1;
            }
        }
        if(wa_flag == 0){
            if(board->wairfield[i+8] != '\0'){
                strboard[i+135]=board->wairfield[i+8];
            }
            else{
                wa_flag = 1;
            }
        }
        if(wp_flag == 0){
            if(board->wprison[i] != '\0'){
                strboard[i+144]=board->wprison[i];
            }
            else{
                wp_flag = 1;
            }
        }
        if(wp_flag == 0){
            if(board->wprison[i+8] != '\0'){
                strboard[i+153]=board->wprison[i+8];
            }
            else{
                wp_flag = 1;
            }
        }
        //black side of board is exact same as white minus 32
        strboard[i+36]='-';
        strboard[i+45]=board->board[7][i];
        strboard[i+54]=board->board[6][i];
        strboard[i+63]=board->board[5][i];
        strboard[i+72]=board->board[4][i];
        strboard[i+81]=board->board[3][i];
        strboard[i+90]=board->board[2][i];
        strboard[i+99]=board->board[1][i];
        strboard[i+108]=board->board[0][i];
        strboard[i+117]='-';
    }
    return strboard;

}
//func to send a piece to white prison
exboard_t *to_wprison(exboard_t *board, move_t *move){
    int i;
    for(i=0;i<15;i++){
        if(board->wprison[i]=='\0'){
            board->wprison[i]=board->board[move->to_i][move->to_j];
            board->board[move->to_i][move->to_j]=' ';
            board->wprison[i+1]='\0';
            break;
        }
    }
    return board;
}
//func to send a piece to black prison
exboard_t *to_bprison(exboard_t *board, move_t *move){
    int i;
    for(i=0;i<15;i++){
        if(board->bprison[i]=='\0'){
            board->bprison[i]=board->board[move->to_i][move->to_j];
            board->board[move->to_i][move->to_j]=' ';
            board->bprison[i+1]='\0';
            break;
        }
    }
    return board;
}
//func that returns the piece char of the given coordinates
char piece(board_t *board, int to_i, int to_j){
    char p;
    p=(*board)[to_i][to_j];
    return p;

}
//func that returns an int (bool) based on a piece being at the loc or not
 int ispiece(board_t *board, int to_i, int to_j){
    int i;
    if((*board)[to_i][to_j]==' '){
        i=0;
    }
    else{
        i=1;
    }
    return i;

}
//func to check if the given location is a valid board location
int isboard(board_t *board, int to_i, int to_j){
    int i;
    if(((to_i>-1)&&(to_i<8))&&((to_j>-1)&&(to_j<8))){
        i=1;
    }
    else{
        i=0;
    }
    return i;
}
//apply the move
exboard_t *apply_move(exboard_t *board, move_t *move){
    if((move->from_i == -1) || (move->from_i==8)){
        //airfield
        if(move->from_i == -1){
            //white airfield
            int i;
            for(i=0;i<15;i++){
                //assume the promoted piece to be placed on the board is in the airfield
                if(board->wairfield[i]==move->promotion){
                    int j=i;
                    while(board->wairfield[j]!='\0'){
                        board->wairfield[j]=board->wairfield[j+1];
                        j++;
                    }
                    break;
                }
            }   
        }
        else{
            int i;
            for(i=0;i<15;i++){
                if(board->bairfield[i]==move->promotion){
                    int j=i;
                    while(board->bairfield[j]!='\0'){
                        board->bairfield[j]=board->bairfield[j+1];
                        j++;
                    }
                    break;
                }
            }

        }
        board->board[move->to_i][move->to_j]=move->promotion;
    }
    else if((move->from_i== -2)||(move->from_i==9)){
        //prison
        if(move->from_i==-2){
            //white prison
            //black piece in white's prison
            int i;
            for(i=0;i<15;i++){
                if(move->promotion==board->wprison[i]){
                    int j=i;
                    while(board->wprison[j]!='\0'){
                        board->wprison[j]=board->wprison[j+1];
                        j++;
                    }
                    break;
                }
            }
            for(i=0;i<15;i++){
                if(move->hostage==board->bprison[i]){
                    int j=i;
                    while(board->bprison[j]!='\0'){
                        board->bprison[j]=board->bprison[j+1];
                        j++;
                    }
                    break;
                }
            }
            for(i=0;i<15;i++){
                if(board->wairfield[i]=='\0'){
                    board->wairfield[i]=move->hostage;
                    board->wairfield[i+1]='\0';
                    break;
                }
            }
        }
        else{
            //white piece in black's prison
            int i;
            for(i=0;i<15;i++){
                if(move->promotion==board->bprison[i]){
                    int j=i;
                    while(board->bprison[j]!='\0'){
                        board->bprison[j]=board->bprison[j+1];
                        j++;
                    }
                    break;
                }
            }
            for(i=0;i<15;i++){
                if(move->hostage==board->wprison[i]){
                    int j=i;
                    while(board->wprison[j]!='\0'){
                        board->wprison[j]=board->wprison[j+1];
                        j++;
                    }
                    break;
                }
            }
            for(i=0;i<15;i++){
                if(board->bairfield[i]=='\0'){
                    board->bairfield[i]=move->hostage;
                    board->bairfield[i+1]='\0';
                    break;
                }
            }
        }
        board->board[move->to_i][move->to_j]=move->promotion;
    }
    else{
        if(move->promotion==' '){
            //castling
            if(((board->board[move->from_i][move->from_j]=='K')||(board->board[move->from_i][move->from_j]=='k'))&&(abs(move->from_j - move->to_j)>1)){
                board->board[move->to_i][move->to_j]=board->board[move->from_i][move->from_j];
                board->board[move->from_i][move->from_j]=' ';
                if((move->from_j - move->to_j)>0){
                    board->board[move->to_i][move->to_j+1]=board->board[move->to_i][move->to_j-1];
                    board->board[move->to_i][move->to_j-1]=' ';
                }
                else{
                    board->board[move->to_i][move->to_j-1]=board->board[move->to_i][move->to_j+1];
                    board->board[move->to_i][move->to_j+1]=' ';
                }
            }
            else if(((board->board[move->from_i][move->from_j]=='P')||(board->board[move->from_i][move->from_j]=='p'))&&(abs(move->from_j - move->to_j)>0)&&(board->board[move->to_i][move->to_j]==' ')){
                //en passant
                //int i;
                int temp_to_i = move->to_i;
                move->to_i = move->from_i;
                if(board->board[move->from_i][move->from_j]=='P'){
                    board = to_wprison(board, move);
                }
                else{
                    board = to_bprison(board, move);
                }
                move->to_i = temp_to_i;
                board->board[move->to_i][move->to_j]=board->board[move->from_i][move->from_j];
                board->board[move->from_i][move->from_j]=' ';
            }
            else{
                //regular move with no promotion
                if(board->board[move->to_i][move->to_j]!=' '){
                    if(board->board[move->to_i][move->to_j]>96){
                        board = to_wprison(board, move);
                    }
                    else{
                        board = to_bprison(board, move);
                    }
                }
                board->board[move->to_i][move->to_j]=board->board[move->from_i][move->from_j];
                board->board[move->from_i][move->from_j]=' ';
            }
        }
        else{
            //moves with promotion char
            board->board[move->to_i][move->to_j]=move->promotion;
            board->board[move->from_i][move->from_j]=' ';
        }
    }
    return board;
}

//move selection
move_t **moves(board_t *board, int from_i, int from_j){
    char c = piece(board, from_i, from_j);
    int colour;
    if(c<91){
        colour = 0;
    }
    else{
        colour = 1;
    }
    move_t **mymoves;
    //choose move based on piece
    if(c=='N' || c=='n'){
        mymoves=knightmoves(board, from_i, from_j, colour);
    }
    else if(c=='B' || c=='b'){
        mymoves=bishopmoves(board, from_i, from_j, colour);
    }
    else if(c=='R' || c=='r'){
        mymoves=rookmoves(board, from_i, from_j, colour);
    }
    else if(c=='Q' || c=='q'){
        mymoves=queenmoves(board, from_i, from_j, colour);
    }
    else if(c=='K' || c=='k'){
        mymoves=king_moves(board, from_i, from_j, colour);
    }
    else if(c=='P' || c=='p'){
        mymoves=pawn_moves(board, from_i, from_j, colour);
    }
    else{
        return NULL;
    }

    if(mymoves == NULL){
        return NULL;
    }
    return mymoves;

}

move_t **knightmoves(board_t *board, int from_i, int from_j, int colour){
    //8 possible moves + null terminator
    move_t **kmoves =malloc(sizeof(move_t *)*9);
    //check is malloc fails
    if((board==NULL)||(kmoves==NULL)){
        return NULL;
    }
    //empty variable for move count for realloc later
    int i;
    int count=0;
    //generate array from i and j coordinates
    int to_i[8];
    int to_j[8];
    //var to store piece colour
    int to_piece_colour;
    //generate all possible moves
    to_i[0]=from_i-2;
    to_j[0]=from_j-1;
    to_i[1]=from_i-2;
    to_j[1]=from_j+1;
    to_i[2]=from_i-1;
    to_j[2]=from_j-2;
    to_i[3]=from_i-1;
    to_j[3]=from_j+2;
    to_i[4]=from_i+1;
    to_j[4]=from_j-2;
    to_i[5]=from_i+1;
    to_j[5]=from_j+2;
    to_i[6]=from_i+2;
    to_j[6]=from_j-1;
    to_i[7]=from_i+2;
    to_j[7]=from_j+1;
    //loop through all possible moves
    for(i=0;i<8;i++){
        //to start, set the 'to' space's piece colour to that opposite of the current piece's colour
        to_piece_colour=!colour;
        //if the board exists at the to location
        if(isboard(board, to_i[i],to_j[i])){
            //if there is a piece at the to location, check the colour
            if(ispiece(board, to_i[i], to_j[i])){
                char p = piece(board, to_i[i], to_j[i]);
                if(p<91){
                    to_piece_colour=0;
                }
                else{
                    to_piece_colour=1;
                }
            }
            //if the to location piece has the same colour as current piece, move cannot be created
            if(to_piece_colour != colour){
                kmoves[count]=malloc(sizeof(move_t));
                //check is malloc fails and free everything if it does
                if(kmoves[count]==NULL){
                    for(int k=0; k<count; k++){
                        free(kmoves[k]);
                    }
                    free(kmoves);
                    return NULL;
                }
                //create move info
                kmoves[count]->from_i=from_i;
                kmoves[count]->from_j=from_j;
                kmoves[count]->to_i=to_i[i];
                kmoves[count]->to_j=to_j[i];
                kmoves[count]->promotion='\0';
                kmoves[count]->hostage='\0';
                //update the move counter
                count++;
            }
        }
    }
    //reallocate memory for move count
    move_t **r_kmoves=realloc(kmoves,(count+1)*sizeof(move_t *));
    //check if realloc fails and free everything if it does
    if(r_kmoves==NULL){
        for(i=0;i<count;i++){
            free(kmoves[i]);
        }
        free(kmoves);
        return NULL;
    }
    //set last move to NULL
    r_kmoves[count]=NULL;
    // for(i=0; i<count; i++){
    //     printf("%d %d\n", r_kmoves[i]->to_i, r_kmoves[i]->to_j);
    // }
    return r_kmoves;
}

move_t **bishopmoves(board_t *board, int from_i, int from_j, int colour){
    //max 13 possible moves
    move_t **bmoves = malloc(sizeof(move_t *)*15);
    //check if malloc fails
    if((board==NULL)||(bmoves==NULL)){
        return NULL;
    }
    //initialize flags
    //if bishop has encountered a piece in its path, it cannot move farther down that path
    int i;
    int count = 0;
    int to_piece;
    int downright=0;
    int upleft=0;
    int upright=0;
    int downleft=0;
    //max 7 possible move on any side of bishop
    for(i=1;i<8;i++){
        //check if board exists and check if top right piece flag is 0
        //i.e. make sure the path is clea
        if(isboard(board, from_i + i, from_j +i) && upright==0){
            //if there is a piece at the to location
            if(ispiece(board, from_i +i, from_j +i)){
                //check the colour
                char p = piece(board, from_i+i, from_j+i);
                if(p<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                //update the path flag
                upright=1;
            }
            else{
                to_piece=-1;
            }
            //only create move is the to location piece is a different colour than the current piece
            if(to_piece != colour){
                bmoves[count]=malloc(sizeof(move_t));
                //check if malloc failds
                if(bmoves[count]==NULL){
                    for(int k=0;k<count;k++){
                        free(bmoves[k]);
                    }
                    free(bmoves);
                    return NULL;
                }
                bmoves[count]->from_i = from_i;
                bmoves[count]->from_j = from_j;
                bmoves[count]->to_i = from_i+i;
                bmoves[count]->to_j = from_j+i;
                bmoves[count]->promotion = '\0';
                bmoves[count]->hostage = '\0';
                //update move counter
                count++;
            }
        }
        //same process as above, but for top left path
        if(isboard(board, from_i + i, from_j -i)&& upleft==0){
            if(ispiece(board, from_i +i, from_j -i)){
                char p = piece(board, from_i+i, from_j-i);
                if(p<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                upleft=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                bmoves[count]=malloc(sizeof(move_t));
                if(bmoves[count]==NULL){
                    for(int k=0;k<count;k++){
                        free(bmoves[k]);
                    }
                    free(bmoves);
                    return NULL;
                }
                bmoves[count]->from_i = from_i;
                bmoves[count]->from_j = from_j;
                bmoves[count]->to_i = from_i+i;
                bmoves[count]->to_j = from_j-i;
                bmoves[count]->promotion = '\0';
                bmoves[count]->hostage = '\0';
                count++;
            }
        }
        //process for bottom right path
        if(isboard(board, from_i - i, from_j +i)&& downright==0){
            if(ispiece(board, from_i -i, from_j +i)){
                char p = piece(board, from_i-i, from_j+i);
                if(p<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                downright=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                bmoves[count]=malloc(sizeof(move_t));
                if(bmoves[count]==NULL){
                    for(int k=0;k<count;k++){
                        free(bmoves[k]);
                    }
                    free(bmoves);
                    return NULL;
                }
                bmoves[count]->from_i = from_i;
                bmoves[count]->from_j = from_j;
                bmoves[count]->to_i = from_i-i;
                bmoves[count]->to_j = from_j+i;
                bmoves[count]->promotion = '\0';
                bmoves[count]->hostage = '\0';
                count++;
            }
        }
        //process for bottom left path
        if(isboard(board, from_i - i, from_j -i)&& downleft==0){
            if(ispiece(board, from_i -i, from_j -i)){
                char p = piece(board, from_i-i, from_j-i);
                if(p<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                downleft=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                bmoves[count]=malloc(sizeof(move_t));
                if(bmoves[count]==NULL){
                    for(int k=0;k<count;k++){
                        free(bmoves[k]);
                    }
                    free(bmoves);
                    return NULL;
                }
                bmoves[count]->from_i = from_i;
                bmoves[count]->from_j = from_j;
                bmoves[count]->to_i = from_i-i;
                bmoves[count]->to_j = from_j-i;
                bmoves[count]->promotion = '\0';
                bmoves[count]->hostage = '\0';
                count++;
            }
        }
    }
    //reallocate mem based on actual move count
    move_t **r_bmoves=realloc(bmoves,(count+1)*sizeof(move_t *));
    if(r_bmoves==NULL){
        for(i=0; i<count; i++){
            free(bmoves[i]);
        }
        free(bmoves);
        return NULL;
    }
    r_bmoves[count]=NULL;
    return r_bmoves;   
}

move_t **rookmoves(board_t *board, int from_i, int from_j, int colour){
    move_t **rmoves=malloc(sizeof(move_t *)*15);
    //max 14 possible moves + null
    //check if malloc fails
    if(rmoves==NULL || board==NULL){
        return NULL;
    }
    //initialize move count and path flags
    int i;
    int count=0;
    int up=0;
    int down=0;
    int left=0;
    int right=0;
    int to_piece;
    for(i=1;i<8;i++){
        //if the board exists at the to location and the path is clear
        if(isboard(board, from_i + i, from_j)&&up==0){
            if(ispiece(board,from_i + i, from_j)){
                char c=piece(board, from_i + i, from_j);
                //check colour of any piece in the path
                if(c<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                up=1;
            }
            else{
                to_piece=-1;
            }
            //only create move if piece colours are different
            if(to_piece != colour){
                rmoves[count]=malloc(sizeof(move_t));
                if(rmoves[count]==NULL){
                    for(int k=0; k<count; k++){
                        free(rmoves[count]);
                    }
                    free(rmoves);
                    return NULL;
                }
                rmoves[count]->from_i = from_i;
                rmoves[count]->from_j = from_j;
                rmoves[count]-> to_i = from_i+i;
                rmoves[count]->to_j = from_j;
                rmoves[count]->promotion = '\0';
                rmoves[count]->hostage = '\0';
                count++;
            }
        }
        //same process for bottom path
        if(isboard(board, from_i - i, from_j)&&down==0){
            if(ispiece(board,from_i - i, from_j)){
                char c=piece(board, from_i - i, from_j);
                if(c<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                down=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                rmoves[count]=malloc(sizeof(move_t));
                if(rmoves[count]==NULL){
                    for(int k=0; k<count; k++){
                        free(rmoves[count]);
                    }
                    free(rmoves);
                    return NULL;
                }
                rmoves[count]->from_i = from_i;
                rmoves[count]->from_j = from_j;
                rmoves[count]-> to_i = from_i-i;
                rmoves[count]->to_j = from_j;
                rmoves[count]->promotion = '\0';
                rmoves[count]->hostage = '\0';
                count++;
            }
        }
        //same process for right path
        if(isboard(board, from_i, from_j + i)&&right==0){
            if(ispiece(board,from_i, from_j + i)){
                char c=piece(board, from_i, from_j + i);
                if(c<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                right=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                rmoves[count]=malloc(sizeof(move_t));
                if(rmoves[count]==NULL){
                    for(int k=0; k<count; k++){
                        free(rmoves[count]);
                    }
                    free(rmoves);
                    return NULL;
                }
                rmoves[count]->from_i = from_i;
                rmoves[count]->from_j = from_j;
                rmoves[count]-> to_i = from_i;
                rmoves[count]->to_j = from_j+i;
                rmoves[count]->promotion = '\0';
                rmoves[count]->hostage = '\0';
                count++;
            }
        }
        //same process for left path
        if(isboard(board, from_i, from_j - i)&&left==0){
            if(ispiece(board,from_i, from_j - i)){
                char c=piece(board, from_i, from_j - i);
                if(c<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
                left=1;
            }
            else{
                to_piece=-1;
            }
            if(to_piece != colour){
                rmoves[count]=malloc(sizeof(move_t));
                if(rmoves[count]==NULL){
                    for(int k=0; k<count; k++){
                        free(rmoves[count]);
                    }
                    free(rmoves);
                    return NULL;
                }
                rmoves[count]->from_i = from_i;
                rmoves[count]->from_j = from_j;
                rmoves[count]-> to_i = from_i;
                rmoves[count]->to_j = from_j - i;
                rmoves[count]->promotion = '\0';
                rmoves[count]->hostage = '\0';
                count++;
            }
        }
    }
    //realloc move array and check if it fails
    move_t **r_rmoves=realloc(rmoves,(count+1)*sizeof(move_t *));
    if(r_rmoves==NULL){
        for(i=0; i<count; i++){
            free(rmoves[i]);
        }
        free(rmoves);
        return NULL;
    }
    r_rmoves[count]=NULL;
    return r_rmoves;
}

move_t **queenmoves(board_t *board, int from_i, int from_j, int colour){
    move_t **qmoves = malloc(sizeof(move_t *)*30);
    //max num moves for queen is 26, round up to 30 just to be safe :P
    if(qmoves==NULL || board==NULL){
        return NULL;
    }
    int i;
    int k=0;
    int count=0;
    int bcount=0;
    int rcount=0;
    //concactenate rook and bishop moves, remove any duplicates
    move_t **bmoves = bishopmoves(board, from_i, from_j, colour);
    move_t **rmoves = rookmoves(board, from_i, from_j, colour);
    if(rmoves == NULL || bmoves == NULL){
        return NULL;
    }
    while(bmoves[k] != NULL){
        bcount++;
        k++;
    }
    k=0;
    while(rmoves[k] != NULL){
        rcount++;
        k++;
    }
    for(i =0; i<bcount; i++){
        if(bmoves[i]!= NULL){
            qmoves[count]=malloc(sizeof(move_t));
            if(qmoves[count]==NULL){
                for(int a=0; a<count; a++){
                    free(qmoves[count]);
                }
                for(int b=0; b<bcount; b++){
                    free(bmoves[b]);
                }
                for(int r=0; r<rcount; r++){
                    free(rmoves[r]);
                }
                free(qmoves);
                free(bmoves);
                free(rmoves);
                return NULL;
            }
            memcpy(qmoves[count], bmoves[i], sizeof(move_t));
            free(bmoves[i]);
            count++;
        }
    }
    free(bmoves);
    for(i = 0; i<rcount; i++){
        if(rmoves[i]!=NULL){
            qmoves[count]=malloc(sizeof(move_t));
            if(qmoves[count]==NULL){
                for(int a=0; a<count; a++){
                    free(qmoves[count]);
                }
                for(int b=0; b<bcount; b++){
                    free(bmoves[b]);
                }
                for(int r=0; r<rcount; r++){
                    free(rmoves[r]);
                }
                free(qmoves);
                free(bmoves);
                free(rmoves);
                return NULL;
            }
            memcpy(qmoves[count], rmoves[i], sizeof(move_t));
            free(rmoves[i]);
            count++;
        }
    }
    free(rmoves);
    //remove duplicates
    int count2 = 0;
    int duplicate = 0;
    move_t **newqmoves = malloc(sizeof(move_t *)*30);
    if(newqmoves == NULL){
        for(i=0; i<count; i++){
            free(qmoves[i]);
        }
        free(qmoves);
        return NULL;
    }
    for(i=0; i<count; i++){
        for(int j=i+1; j<count; j++){
            if((qmoves[i]->to_i==qmoves[j]->to_i) && (qmoves[i]->to_j==qmoves[j]->to_j)){
                duplicate=1;
                break;
            }
        }
        if(duplicate == 0){
            newqmoves[count2] = malloc(sizeof(move_t));
            memcpy(newqmoves[count2], qmoves[i], sizeof(move_t));
            count2++;
        }
        duplicate = 0;
        free(qmoves[i]);
    }
    free(qmoves);
    //reallocate based on non duplicate move number
    move_t **r_qmoves=realloc(newqmoves,(count2+1)*sizeof(move_t *));
    if(r_qmoves==NULL){
        for(i=0; i<count2; i++){
            free(newqmoves[i]);
        }
        free(newqmoves);
        return NULL;
    }
    r_qmoves[count2]=NULL;
    return r_qmoves;

}

move_t **king_moves(board_t *board, int from_i, int from_j, int colour){
    move_t **Kmoves = malloc(sizeof(move_t *)*9);
    //8 max possible moves + null
    //check if malloc fails
    if(Kmoves == NULL || board == NULL){
        return NULL;
    }
    //initialize move array
    int count=0;
    int to_i[8];
    int to_j[8];
    to_i[0] = from_i + 1;
    to_j[0] = from_j;
    to_i[1] = from_i + 1;
    to_j[1] = from_j + 1;
    to_i[2] = from_i;
    to_j[2] = from_j + 1;
    to_i[3] = from_i - 1;
    to_j[3] = from_j + 1;
    to_i[4] = from_i - 1;
    to_j[4] = from_j;
    to_i[5] = from_i - 1;
    to_j[5] = from_j - 1;
    to_i[6] = from_i;
    to_j[6] = from_j - 1;
    to_i[7] = from_i + 1;
    to_j[7] = from_j - 1;
    int i;
    int to_piece;
    for(i=0; i<8; i++){
        //same type of process
        //check if board exists, if there is a piece, check the colour
        //only generate valid move if piece colours do not match
        //always check if malloc fails
        if(isboard(board, to_i[i], to_j[i])){
            if(ispiece(board, to_i[i], to_j[i])){
                char c = piece(board, to_i[i], to_j[i]);
                if(c<91){
                    to_piece=0;
                }
                else{
                    to_piece=1;
                }
            }
            else{
                to_piece = -1;
            }
            if(to_piece != colour){
                Kmoves[count] = malloc(sizeof(move_t));
                if(Kmoves == NULL){
                    for(int k=0; k<count; k++){
                        free(Kmoves[count]);
                    }
                    free(Kmoves);
                    return NULL;
                }
                Kmoves[count]->from_i = from_i;
                Kmoves[count]->from_j = from_j;
                Kmoves[count]->to_i = to_i[i];
                Kmoves[count]->to_j = to_j[i];
                Kmoves[count]->promotion = '\0';
                Kmoves[count]->hostage = '\0';
                count++;
            }
        }
    }
    //realloc move array
    move_t **r_Kmoves=realloc(Kmoves,(count+1)*sizeof(move_t *));
    if(r_Kmoves==NULL){
        for(i=0; i<count; i++){
            free(Kmoves[i]);
        }
        free(Kmoves);
        return NULL;
    }
    r_Kmoves[count]=NULL;
    return r_Kmoves;
}

move_t **pawn_moves(board_t *board, int from_i, int from_j, int colour){
    move_t **pmoves = malloc(sizeof(move_t *)*5);
    //max possible moves is 4 + null
    //check if malloc fails
    if(pmoves == NULL || board == NULL){
        return NULL;
    }
    //initialize move count
    int count = 0;
    int to_piece;
    int i;
    int adder;
    //adder dictates the direction the pawn must move in
    if(colour == 0){
        adder = 1;
    }
    else{
        adder = -1;
    }
    //check if the board exists, check the colour of any pieces in the way
    //only generate move if piece colours are opposite
    if(isboard(board, from_i + adder, from_j)){
        if(!ispiece(board, from_i + adder, from_j)){
            pmoves[count] = malloc(sizeof(move_t));
            if(pmoves == NULL){
                for(int p =0; p<count; p++){
                    free(pmoves[count]);
                }
                free(pmoves);
                return NULL;
            }
            pmoves[count]->from_i = from_i;
            pmoves[count]->from_j = from_j;
            pmoves[count]->to_i = from_i + adder;
            pmoves[count]->to_j = from_j;
            pmoves[count]->promotion = '\0';
            pmoves[count]->hostage = '\0';
            count++;
        }
    }
    if(isboard(board, from_i + 2*adder, from_j)){
        //if pawn is in its home row and there are no pieces in the way
        if(!ispiece(board, from_i + 2*adder, from_j) && !ispiece(board, from_i + adder, from_j) && (((from_i == 6)&& colour ==1)||((from_i==1)&& colour==0))){
            pmoves[count] = malloc(sizeof(move_t));
            if(pmoves == NULL){
                for(int p =0; p<count; p++){
                    free(pmoves[count]);
                }
                free(pmoves);
                return NULL;
            }
            pmoves[count]->from_i = from_i;
            pmoves[count]->from_j = from_j;
            pmoves[count]->to_i = from_i + 2*adder;
            pmoves[count]->to_j = from_j;
            pmoves[count]->promotion = '\0';
            pmoves[count]->hostage = '\0';
            count++;
        }
    }//capturing move
    if(isboard(board, from_i + adder, from_j + 1)){
        if(ispiece(board, from_i + adder, from_j + 1)){
            char c = piece(board, from_i + adder, from_j + 1);
            if(c<91){
                to_piece = 0;
            }
            else{
                to_piece = 1;
            }
            if(to_piece != colour){
                pmoves[count] = malloc(sizeof(move_t));
                if(pmoves == NULL){
                    for(int p =0; p<count; p++){
                        free(pmoves[count]);
                    }
                    free(pmoves);
                    return NULL;
                }
                pmoves[count]->from_i = from_i;
                pmoves[count]->from_j = from_j;
                pmoves[count]->to_i = from_i + adder;
                pmoves[count]->to_j = from_j + 1;
                pmoves[count]->promotion = '\0';
                pmoves[count]->hostage = '\0';
                count++;
            }
        }
    }
    //capturing move
    if(isboard(board, from_i + adder, from_j - 1)){
        if(ispiece(board, from_i + adder, from_j - 1)){
            char c = piece(board, from_i + adder, from_j - 1);
            if(c<91){
                to_piece = 0;
            }
            else{
                to_piece = 1;
            }
            if(to_piece != colour){
                pmoves[count] = malloc(sizeof(move_t));
                if(pmoves == NULL){
                    for(int p =0; p<count; p++){
                        free(pmoves[count]);
                    }
                    free(pmoves);
                    return NULL;
                }
                pmoves[count]->from_i = from_i;
                pmoves[count]->from_j = from_j;
                pmoves[count]->to_i = from_i + adder;
                pmoves[count]->to_j = from_j - 1;
                pmoves[count]->promotion = '\0';
                pmoves[count]->hostage = '\0';
                count++;
            }
        }
    }
    move_t **r_pmoves=realloc(pmoves,(count+1)*sizeof(move_t *));
    if(r_pmoves==NULL){
        for(i=0; i<count; i++){
            free(pmoves[i]);
        }
        free(pmoves);
        return NULL;
    }
    r_pmoves[count]=NULL;
    return r_pmoves;
}
char *fen1(exboard_t *board, char *active, char *castling, char *enpassant, int half, int full){
    //initialize empty string to store board pieces
    char pieces[200];
    int empty = 0;
    int index=0;
    int i,j;
    int wa1 = 0;
    int wa2 = 0;
    int wp1 = 0;
    int wp2 = 0;
    int ba1 = 0;
    int ba2 = 0;
    int bp1 = 0;
    int bp2 = 0;
    //append black prison pieces
    //go through string in two sections of 8
    for(i=0; i<8; i++){
        //if the char at index i is not a null terminator
        //append it to the fen string
        //increment fen index
        if(board->bprison[i]!= '\0' && wp1 == 0){
            pieces[index] = board->bprison[i];
            index++;
        }
        else{
            //if it is a null terminator, increase empty count
            wp1 = 1;
            empty++;
        }
    }
    if(empty!=0){
        //if there is a value for empty
        //i.e. the airifeld is not entirely full
        //append empty count to fen string and increment fen index
        pieces[index] = empty+48;
        index++;
    }
    //append slash to separate rows and increment index
    pieces[index] = '/';
    index++;
    //reset empty count to 0
    empty=0;

//repeat for second row of black prison
    for(i=8; i<16; i++){
        if(board->bprison[i]!= '\0' && wp2 == 0){
            pieces[index] = board->bprison[i];
            index++;
        }
        else{
            wp2 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;
//repeat for first row of black airfield
    for(i=0; i<8; i++){
        if(board->bairfield[i]!= '\0' && wa1 == 0){
            pieces[index] = board->bairfield[i];
            index++;
        }
        else{
            wa1 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;
//repeat for second row of black airfield
    for(i=8; i<16; i++){
        if(board->bairfield[i]!= '\0' && wa2 == 0){
            pieces[index] = board->bairfield[i];
            index++;
        }
        else{
            wa2 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;
//for the actual board
//reset empty count to 0 after every row
    for(i=7; i>-1; i--){
        empty=0;
        for(j=0; j<8; j++){
            //check the piece value and store it in p
            char p = piece(&board->board, i, j);
            //if a space, increment the empty count
            if(p == ' '){
                empty++;
            }
            //if not a space, i.e. there is a piece
            else{
                //if there was an existing empty count, append it to fen string and increment fen index
                if(empty>0){
                    pieces[index] = empty+48;
                    index++;
                }
                //append the piece and increment index
                pieces[index] = p;
                index++;
                //reset empty to 0
                empty = 0;
            }
        }
        //append the final empty space count
        if(empty>0){
            pieces[index] = empty+48;
            index++;
        }
        //add the slash 
        pieces[index] = '/';
        index++;
    }
    //repeat process for white airfield and prison
    //flip counters
    for(i=0; i<8; i++){
        if(board->wairfield[i]!= '\0' && ba2 == 0){
            pieces[index] = board->wairfield[i];
            index++;
        }
        else{
            ba2 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;

    for(i=8; i<16; i++){
        if(board->wairfield[i]!= '\0' && ba1 == 0){
            pieces[index] = board->wairfield[i];
            index++;
        }
        else{
            ba1 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;

    for(i=0; i<8; i++){
        if(board->wprison[i]!= '\0' && bp2 == 0){
            pieces[index] = board->wprison[i];
            index++;
        }
        else{
            bp2 = 1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }
    pieces[index] = '/';
    index++;
    empty=0;

    for(i=8; i<16; i++){
        if(board->wprison[i]!= '\0' && bp1 == 0){
            pieces[index] = board->wprison[i];
            index++;
        }
        else{
            bp1 =1;
            empty++;
        }
    }
    if(empty!=0){
        pieces[index] = empty+48;
        index++;
    }

    pieces[index] = '\0';
    int half_len = snprintf(NULL, 0, "%d", half);
    int full_len = snprintf(NULL, 0, "%d", full);
    int len = strlen(pieces) + 1 + strlen(active) + 1 + strlen(castling) + 1 + strlen(enpassant) + 1 + half_len + 1 + full_len + 1;
    char *fen_str = malloc(sizeof(char)*len);
    if(fen_str == NULL){
        printf("Could not make fen notation\n");
        return NULL;
    }
    snprintf(fen_str, len, "%s %s %s %s %d %d", pieces, active, castling, enpassant, half, full);
    return(fen_str);
}

char *fen(exboard_t *board, char *active, char *castling, char *enpassant, int half, int full){
    char pieces[200]={0};
    int empty = 0;
    int index=0;
    int i,j;
    //for the actual board
    //reset empty count to 0 after every row
    for(i=7; i>-1; i--){
        empty=0;
        for(j=0; j<8; j++){
            //check the piece value and store it in p
            char p = piece(&board->board, i, j);
            //if a space, increment the empty count
            if(p == ' '){
                empty++;
            }
            //if not a space, i.e. there is a piece
            else{
                //if there was an existing empty count, append it to fen string and increment fen index
                if(empty>0){
                    pieces[index] = empty+48;
                    index++;
                }
                //append the piece and increment index
                pieces[index] = p;
                index++;
                //reset empty to 0
                empty = 0;
            }
        }
        //append the final empty space count
        if(empty>0){
            pieces[index] = empty+48;
            index++;
        }
        //add the slash 
        if(i!=0){
            pieces[index] = '/';
        }
        index++;
    }
    pieces[index] = '\0';
    int half_len = snprintf(NULL, 0, "%d", half);
    int full_len = snprintf(NULL, 0, "%d", full);
    int len = strlen(pieces) + 1 + strlen(active) + 1 + strlen(castling) + 1 + strlen(enpassant) + 1 + half_len + 1 + full_len + 1;
    char *fen_str = malloc(sizeof(char)*len);
    if(fen_str == NULL){
        printf("Could not make fen notation\n");
        return NULL;
    }
    snprintf(fen_str, len, "%s %s %s %s %d %d", pieces, active, castling, enpassant, half, full);
    return(fen_str);

}

exboard_t *boardstring( char *string ){
    exboard_t *board = newboard();
    int i,j;
    int c = 0;
    for(i=0; i<16; i++){
        if(string[c]==' '){
            board->bprison[i]='\0';
        }
        else{
            board->bprison[i] = string[c];
        }
        if(string[c+9] == ' '){
            board->bairfield[i] = '\0';
        }
        else{
            board->bairfield[i] = string[c+9];
        }
        if(string[c+126]==' '){
            board->wairfield[i]='\0';
        }
        else{
            board->wairfield[i] = string[c+126];
        }
        if(string[c+144]==' '){
            board->wprison[i]='\0';
        }
        else{
            board->wprison[i] = string[c+144];
        }
        c++;
        if((c+1)%9 == 0 && c!= 161){
            c++;
        }        
    }
    c = 45;
    for(i=7; i>-1; i--){
        for(j=0; j<8; j++){
           board->board[i][j] = string[c];
           c++;
            if((c+1)%9 == 0){
                c++;
            } 
        }
    }
    return(board);
}



