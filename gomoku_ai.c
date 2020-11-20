//------------------------------------------------------------------------
// 五目ならべAI
//  ここを作る．
//  下記のサンプルは，直前に自分が置いた石の8近傍のどこかにランダムに石を置くだけ
//
//  引数説明
//    board[BOARD_SIZE][BORARD_SIZE]：五目並べの盤
//    com ： コンピュータが白石(STONE_WHITE)か黒石(STONE_BLACK)かを表す．
//    pos_x, pos_y：石を置く場所のx座標，y座標 両方出力用
//------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gomoku.h"

int calcPutPos(int board[][BOARD_SIZE], int com, int *pos_x, int *pos_y)
{
    static int pre[2] = {
        // 直前の石の場所を覚えておくための変数 適当に初期化
        BOARD_SIZE / 2,
        BOARD_SIZE / 2,
    };
    static int flag = 0;
    int count = 0;
    int enem[BOARD_SIZE] = {0};
    int eva[BOARD_SIZE][BOARD_SIZE];
    int index = 0;
    int maxEva[2] = {0};
    boardInit(eva);

    // コンピュータが最初の1手の場合は，(7, 7)に石を置く
    if (flag == 0)
    {
        srand((unsigned int)time(NULL));
        flag = 1;
        if (com == STONE_BLACK)
        {
            *pos_y = BOARD_SIZE / 2;
            *pos_x = BOARD_SIZE / 2;
            return 0;
        }
    }
    while (1)
    {
        //乱数で前に置いた場所の8近傍に置く
        if (count > 2)
        { // 2回置こうとして，どこにも置けなくなった場合はどこでもよいからおいてみる
            *pos_y = (int)(rand() % 15);
            *pos_x = (int)(rand() % 15);
            printf("%d\n", count);
        }
        else
        {
            //周辺のコマが相手のかどうか判定し、そのコマとの反対には置かない(先攻)(ボードの評価を0に)
            if(com == STONE_BLACK){
                if(checkBoardNow(board, pre[0], pre[1], com, enem) != com){
                    eva[pre[0] + (0 - (enem[0]- pre[0]))][pre[1] + (0 - (enem[1] - pre[1]))] = 10;
                }
            }
            
            evaluate_board(board, eva, com, maxEva);
            *pos_y = maxEva[0];
            *pos_x = maxEva[1];
        }
        if (checkOutPos(*pos_x, *pos_y))
        {
            if (board[*pos_y][*pos_x] == STONE_SPACE)
            {
                break;
            }
            count++;
        }
        else
        { // bug出し
            printf("out of board(%d %d)\n", *pos_x, *pos_y);
            count++;
        }
    }
    pre[0] = *pos_y;
    pre[1] = *pos_x;

    return 0;
}

//周辺の相手のコマを調べ
int checkBoardNow(int board[][BOARD_SIZE], int pre_x, int pre_y, int com, int enem[])
{
    int i, j;
    int dx[] = {-1, 0, 1};
    int dy[] = {-1, 0, 1};
    int index = 0;
    int check = STONE_SPACE;
    for(j = 0; j < 3; j++){
        for(i = 0; i < 3; i++){
            if(board[pre_x + dx[i]][pre_y + dy[j]] != com && board[pre_x + dx[i]][pre_y + dy[j]] != STONE_SPACE){
                enem[index++] = pre_x + dx[i];
                enem[index++] = pre_y + dy[j];
                check = board[pre_x + dx[i]][pre_y + dy[j]];
            }
            
            
            
        }
    }
    return check;
}

void evaluate_board(int board[][BOARD_SIZE], int eva[][BOARD_SIZE], int com, int maxEva[]){
    int i, j;
    int count = 0;
    int max = 0;
    for(j = 0; j < BOARD_SIZE; j++){
        for(i = 0; i < BOARD_SIZE; i++){
            if(com != STONE_BLACK && count == 0){
                if(board[i][j] == STONE_BLACK){
                    eva[i - 1][j + 1] = 5;
                    eva[i][j + 1] = 5;
                    eva[i + 1][j + 1] = 5;
                    eva[i - 1][j] = 5;
                    eva[i + 1][j] = 5;
                    eva[i - 1][j - 1] = 5;
                    eva[i][j - 1] = 5;
                    eva[i + 1][j - 1] = 5;
                }
            }
            if(board[i][j] != com){
                if(board[i - 1][j + 1] != com && board[i - 1][j + 1] != STONE_SPACE){
                    eva[i + 1][j - 1] = 10;
                    eva[i - 2][j - 2] = 10;
                }if(board[i][j + 1] != com && board[i][j + 1] != STONE_SPACE){
                    eva[i][j - 1] = 10;
                    eva[i][j + 2] = 10;
                }if(board[i + 1][j + 1] != com && board[i + 1][j + 1] != STONE_SPACE){
                    eva[i - 1][j - 1] = 10;
                    eva[i + 2][j + 2] = 10;
                }if(board[i - 1][j] != com && board[i - 1][j] != STONE_SPACE){
                    eva[i + 1][j] =10;
                    eva[i - 2][j] = 10;
                }if(board[i + 1][j] != com && board[i + 1][j] != STONE_SPACE){
                    eva[i - 1][j] = 10;
                    eva[i + 2][j] = 10;
                }if(board[i - 1][j - 1] != com && board[i - 1][j - 1] != STONE_SPACE){
                    eva[i + 1][j+1] =10;
                    eva[i - 2][j - 2] = 10;
                }if(board[i][j - 1] != com && board[i][j - 1] != STONE_SPACE){
                    eva[i][j - 2] = 10;
                    eva[i][j + 1] = 10;
                }if(board[i + 1][j - 1] != com && board[i + 1][j - 1] != STONE_SPACE){
                    eva[i - 1][j + 1] = 10;
                    eva[i + 2][j - 2] = 10;
                }
            }
        }
    }

    for(j = 0; j < BOARD_SIZE; j++){
        for(i = 0; i < BOARD_SIZE; i++){
            if(max < eva[i][j]){
                maxEva[0] = i;
                maxEva[1] = j;
                max = eva[i][j];
            }
        }
    }
}