#include "hclib.h"
int main(){
    // printf("hello wrld\n");
    exboard_t *myboard=newboard();
    exboard_t *newboard=copyboard(myboard);
    free(myboard);
    int i;
    char *sb=stringboard(newboard);
    for(i=0; i<162; i++){
        printf("%c",sb[i]);
    }
    //free(sb);
    // const char *fp = "stringboard.txt";
    // FILE *file = fopen(fp, "w");
    // if (file == NULL) {
    //     printf("Failed to open the file for writing.\n");
    //     free(sb); // Free the memory for the stringboard
    // }

    // // Write the stringboard to the file
    // fprintf(file, "%s", sb);

    // // Close the file
    // fclose(file);
    exboard_t *sboard = boardstring(sb);

    char *bs = stringboard(sboard);
    for(i=0; i<162; i++){
        printf("%c",bs[i]);
    }
    //free(sboard);
    free(bs);
    char *active = "w";
    char *castling = "-";
    char *enpassant = "-";

    int ind=0;
    // char *fenstr = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr[ind] != '\0'){
    //     printf("%c",fenstr[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr);
    // free(sb);
    // move_t *mymove = malloc(sizeof(move_t));
    // mymove->from_i=1;
    // mymove->from_j=3;
    // mymove->to_i=3;
    // mymove->to_j=3;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }

    // exboard_t *sboard1 = boardstring(sb);

    // char *bs1 = stringboard(sboard1);
    // for(i=0; i<162; i++){
    //     printf("%c",bs1[i]);
    // }
    // free(sboard1);
    // free(bs1);

    // char *fenstr1 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr1[ind] != '\0'){
    //     printf("%c",fenstr1[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr1);
    // free(sb);

    // mymove->from_i=6;
    // mymove->from_j=2;
    // mymove->to_i=4;
    // mymove->to_j=2;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    char *fenstr2 = fen(sboard, active, castling, enpassant, 0, 0);
    while(fenstr2[ind] != '\0'){
        printf("%c",fenstr2[ind]);
        ind++;
    }
    ind=0;
    free(fenstr2);

    // mymove->from_i=4;
    // mymove->from_j=2;
    // mymove->to_i=3;
    // mymove->to_j=2;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr3 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr3[ind] != '\0'){
    //     printf("%c",fenstr3[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr3);

    // mymove->from_i=3;
    // mymove->from_j=2;
    // mymove->to_i=2;
    // mymove->to_j=3;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr4 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr4[ind] != '\0'){
    //     printf("%c",fenstr4[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr4);

    // mymove->from_i=1;
    // mymove->from_j=4;
    // mymove->to_i=3;
    // mymove->to_j=4;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr5 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr5[ind] != '\0'){
    //     printf("%c",fenstr5[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr5);

    // mymove->from_i=0;
    // mymove->from_j=3;
    // mymove->to_i=3;
    // mymove->to_j=7;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr6 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr6[ind] != '\0'){
    //     printf("%c",fenstr6[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr6);

    // mymove->from_i=2;
    // mymove->from_j=3;
    // mymove->to_i=1;
    // mymove->to_j=2;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr7 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr7[ind] != '\0'){
    //     printf("%c",fenstr7[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr7);

    // mymove->from_i=3;
    // mymove->from_j=7;
    // mymove->to_i=6;
    // mymove->to_j=4;
    // mymove->promotion=' ';
    // newboard=apply_move(newboard, mymove);
    // sb=stringboard(newboard);
    // for(i=0; i<162; i++){
    //     printf("%c",sb[i]);
    // }
    // free(sb);

    // char *fenstr8 = fen(newboard, active, castling, enpassant, 0, 0);
    // while(fenstr8[ind] != '\0'){
    //     printf("%c",fenstr8[ind]);
    //     ind++;
    // }
    // ind=0;
    // free(fenstr8);

//     mymove->from_i=7;
//     mymove->from_j=4;
//     mymove->to_i=6;
//     mymove->to_j=4;
//     mymove->promotion=' ';
//     newboard=apply_move(newboard, mymove);
//     sb=stringboard(newboard);
//     for(i=0; i<162; i++){
//         printf("%c",sb[i]);
//     }
//     free(sb);

//     fenstr = fen(newboard, active, castling, enpassant, 0, 0);
//     while(fenstr[ind] != '\0'){
//         printf("%c",fenstr[ind]);
//         ind++;
//     }
//     ind=0;
//     free(fenstr);

//     mymove->from_i=0;
//     mymove->from_j=1;
//     mymove->to_i=1;
//     mymove->to_j=2;
//     mymove->promotion=' ';
//     newboard=apply_move(newboard, mymove);
//     sb=stringboard(newboard);
//     for(i=0; i<162; i++){
//         printf("%c",sb[i]);
//     }
//     free(sb);

//     fenstr = fen(newboard, active, castling, enpassant, 0, 0);
//     while(fenstr[ind] != '\0'){
//         printf("%c",fenstr[ind]);
//         ind++;
//     }
//     ind=0;
//     free(fenstr);

//     mymove->from_i=7;
//     mymove->from_j=3;
//     mymove->to_i=0;
//     mymove->to_j=2;
//     mymove->promotion=' ';
//     newboard=apply_move(newboard, mymove);
//     sb=stringboard(newboard);
//     for(i=0; i<162; i++){
//         printf("%c",sb[i]);
//     }
//     free(sb);

//     fenstr = fen(newboard, active, castling, enpassant, 0, 0);
//     while(fenstr[ind] != '\0'){
//         printf("%c",fenstr[ind]);
//         ind++;
//     }
//     ind=0;
//     free(fenstr);


//     mymove->from_i=0;
//     mymove->from_j=0;
//     mymove->to_i=2;
//     mymove->to_j=3;
//     mymove->promotion=' ';
//     newboard=apply_move(newboard, mymove);
//     sb=stringboard(newboard);
//     move_t **pmoves = moves(newboard,0,2);
//     for(i=0; i<162; i++){
//         printf("%c",sb[i]);
//     }
//     i=0;
    // while(pmoves[i]!=NULL){
    //     free(pmoves[i]);
    //     i++;
    // }
    // free(pmoves);
    //free(sb);
    // free(mymove);
    // free(newboard);
     return 0;
}
