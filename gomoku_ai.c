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
    int enem[BOARD_SIZE][BOARD_SIZE];
    int index = 0;
    boardInit(enem);

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
            //周辺のコマが相手のかどうか判定し、そのコマとの反対には置かない(先攻)
            if(checkBoardNow(board, pre[0], pre[1], com, enem)){
                
            }
            
            *pos_y = pre[0] + (int)(rand() % 3 - 1);
            *pos_x = pre[1] + (int)(rand() % 3 - 1);
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

int isSan(int board[][BOARD_SIZE], int x, int y)
{
    int i, j;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};
    int sum_stone = 0;
    int sum_space = 0;
    int num = 0;

    // i    0:[＼]方向 1:[│]方向 2:[─]方向 3:[／]方向

    for (i = 0; i < 4; i++)
    {
        sum_stone = 0;
        sum_space = 0;
        // 左側が空で，４つの連続するマスの3つが石で残り１つが空で右端も空か，3連続するマスの３つが石で，右端も空
        if (!checkOutPos(x - dx[i], y - dy[i]) || (checkOutPos(x - dx[i], y - dy[i]) && board[y - dy[i]][x - dx[i]] != STONE_SPACE))
        {
            // 左側が空でないか，盤の外
            continue;
        }
        for (j = 0; j < 4; j++)
        {
            // printf("%d (%d %d) %d (%d %d) \n", board[y][x], x, y, board[y + j * dy[i]][x + j * dx[i]], x + j * dx[i], y + j * dy[i]);
            if (checkOutPos(x + j * dx[i], y + j * dy[i]))
            {
                if (board[y][x] == board[y + j * dy[i]][x + j * dx[i]])
                {
                    sum_stone++;
                    if (sum_stone == 3) // 3連続
                        break;
                }
                else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE) // 飛び三の可能性あり
                {
                    sum_space++;
                }
                else
                { // 違う色の石がある
                    break;
                }
            }
        }
        //printf("%d %d sum_stone %d sum_space %d (%d %d)\n", i, j, sum_stone, sum_space, board[y][x], board[y + j * dy[i]][x + j * dx[i]]);

        if (sum_stone == 3 && sum_space == 1)
        {
            // 右が空なら
            if (checkOutPos(x + 4 * dx[i], y + 4 * dy[i]) && board[y + 4 * dy[i]][x + 4 * dx[i]] == STONE_SPACE)
            {
                printf("飛び三(%d %d)\n", x, y);
                //飛び三
                num++;
            }
        }
        else if (sum_stone == 3 && sum_space == 0)
        {
            // 右が空なら
            if (checkOutPos(x + 3 * dx[i], y + 3 * dy[i]) && board[y + 3 * dy[i]][x + 3 * dx[i]] == STONE_SPACE)
            {
                // 3連続
                printf("三連(%d %d)\n", x, y);
                num++;
            }
        }
    }
    return num;
}

//周辺の相手のコマを調べその座標を返す関数
bool checkBoardNow(int board[][BOARD_SIZE], int pre_x, int pre_y, int com, int enem[])
{
    int i, j;
    int dx[] = {-1, 0, 1};
    int dy[] = {-1, 0, 1};
    int index = 0;
    bool check = false;
    for(j = 0; j < 3; j++){
        for(i = 0; i < 3; i++){
            if(board[pre_x + dx[i]][pre_y + dy[j]] != com && board[pre_x + dx[i]][pre_y + dy[j]] != STONE_PLACE){
                enem[index++] = pre_x + dx[i];
                enem[index++] = pre_y + dy[j];
                check = true;
            }
            
        }
    }
    return check;
}
