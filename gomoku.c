// 五目並べ（連珠）先手は黒で(7,7)に置く
// 3種類の禁じ手のどれをルールとするかは，check変数でコンパイル時に指定する．
// 禁じ手のルールは，これらのサイトを見て作ったつもり．
//  http://iroha.poloa.net/ref/02_04.html
//  https://ja.wikipedia.org/wiki/%E4%BA%94%E7%9B%AE%E4%B8%A6%E3%81%B9

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "gomoku.h"

int inputPutPos(int board[][BOARD_SIZE], int which, int com);
void changeTurn(int *which_turn);
int checkOutPos(int x, int y);
int gameInit(int board[][BOARD_SIZE], int *which_turn, int *com);
void boardInit(int board[][BOARD_SIZE]);
void boardPrint(int board[][BOARD_SIZE]);
int gameEndProcess(int board[][BOARD_SIZE]);
int lenCheck(int board[][BOARD_SIZE], int x, int y);
int isSan(int board[][BOARD_SIZE], int x, int y);
int isShi(int board[][BOARD_SIZE], int x, int y);
int isRoku(int board[][BOARD_SIZE], int x, int y);

int check = 6; // 1:三三のみ 2:四四のみ 4:長連のみ 3:三三と四四 5:三三と長連 6:四四と長連 7:全部

//=======================================================
// main
//=======================================================
int main()
{
    // 変数宣言
    int board[BOARD_SIZE][BOARD_SIZE];
    int which_turn;
    int com;

    // 初期処理
    if (gameInit(board, &which_turn, &com) == 1)
        return 1;

    boardPrint(board);

    //---- メインループ
    while (1)
    {
        //--- 入力処理
        if (inputPutPos(board, which_turn, com) == 1)
            return 1;

        //--- 手番交代処理
        changeTurn(&which_turn);

        //--- 出力処理
        boardPrint(board);

        //--- 終了判定
        if (gameEndProcess(board))
        {
            break;
        }
    }
    return 0;
}

//-------------------------------------------------
// 置く場所入力
//-------------------------------------------------
int inputPutPos(int board[][BOARD_SIZE], int which, int com)
{
    int pos_x = -1, pos_y = -1;
    char buf[10];

    if (which == com)
    {
        calcPutPos(board, com, &pos_x, &pos_y);
    }
    else
    {
        printf("%s", (which == 1) ? "●" : "○");
        printf("の番です。どこに置きますか x y の順に入力してください\n> ");
        while (1)
        {
            if (fgets(buf, sizeof buf, stdin) == NULL || buf[0] == '\n')
                return 1;
            if (sscanf(buf, "%d %d", &pos_x, &pos_y) != 2)
            {
                printf("不正な入力です\n >");
                continue;
            }
            if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE)
            {
                break;
            }
            else
            {
                printf("不正な入力です\n> ");
                continue;
            }
        }
    }
    board[pos_y][pos_x] = which;
    return 0;
}

//-------------------------------------------------
// 手番交代処理
//-------------------------------------------------
void changeTurn(int *which_turn)
{
    *which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// 範囲外チェック
//-------------------------------------------------
int checkOutPos(int x, int y)
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// ゲーム情報初期化
//-------------------------------------------------
int gameInit(int board[][BOARD_SIZE], int *which_turn, int *com)
{
    int in = -1;
    char buf[256];
    do
    {
        printf("先手を選びますか？ Yes=>1 No=>0\n> ");
        if (fgets(buf, sizeof buf, stdin) == NULL || buf[0] == '\n')
            return 1;

        if (sscanf(buf, "%d", &in) != 1)
        {
            continue;
        }
    } while (in != 0 && in != 1);

    *com = (in == 1) ? STONE_WHITE : STONE_BLACK;

    boardInit(board);
    *which_turn = STONE_BLACK;
    return 0;
}

//-------------------------------------------------
// 盤面初期
//-------------------------------------------------
void boardInit(int board[][BOARD_SIZE])
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = STONE_SPACE;
        }
    }
}

//-------------------------------------------------
// 盤面出力
//-------------------------------------------------
void boardPrint(int board[][BOARD_SIZE])
{
    int i, j;

    printf("    ");
    for (i = 0; i < BOARD_SIZE; i++)
    {
        if (i / 10)
            printf("%d ", i / 10);
        else
            printf("  ");
    }
    puts("");
    printf("    ");
    for (i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i % 10);
    }
    puts("");
    for (i = 0; i < BOARD_SIZE; i++)
    {
        printf("%2d ", i);
        for (j = 0; j < BOARD_SIZE; j++)
        {
            switch (board[i][j])
            {
            case STONE_SPACE:
                printf("・");
                break;
            case STONE_BLACK:
                printf("●");
                break;
            case STONE_WHITE:
                printf("○");
                break;
            }
        }
        puts("");
    }
    puts("");
}

//-------------------------------------------------
// ゲーム終了判定処理
//-------------------------------------------------
int gameEndProcess(int board[][BOARD_SIZE])
{
    int i, j;
    int n_san = 0;
    int n_shi = 0;
    int num;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == STONE_SPACE)
            {
                continue;
            }
            if ((check & CHOLEN) && board[i][j] == STONE_BLACK && (isRoku(board, j, i) == true))
            {
                printf("長連の禁じ手です。%sの勝ちです\n", "○");
                return 1;
            }
            if ((check & SANSAN) && board[i][j] == STONE_BLACK && (num = isSan(board, j, i)))
            {
                n_san += num;
                if (n_san >= 2)
                {
                    printf("三三の禁じ手です。%sの勝ちです\n", "○");
                    return 1;
                }
            }
            if ((check & SHISHI) && board[i][j] == STONE_BLACK && (num = isShi(board, j, i)))
            {
                n_shi += num;
                if (n_shi >= 2)
                {
                    printf("四四の禁じ手です。%sの勝ちです\n", "○");
                    return 1;
                }
            }
            if (lenCheck(board, j, i))
            {
                printf("%sの勝ちです。\n", (board[i][j] == STONE_BLACK) ? "●" : "○");
                return 1;
            }
        }
    }
    return 0;
}

//-------------------------------------------------
// 5連確認
//-------------------------------------------------
int lenCheck(int board[][BOARD_SIZE], int x, int y)
{
    int i, j, len_flag;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};

    // i    0:[＼]方向 1:[│]方向 2:[─]方向 3:[／]方向

    for (i = 0; i < 4; i++)
    {
        for (j = 0, len_flag = 1; j < 5; j++)
        {
            if (!checkOutPos(x + j * dx[i], y + j * dy[i]) || board[y][x] != board[y + j * dy[i]][x + j * dx[i]])
            {
                len_flag = 0;
                break;
            }
        }
        if (len_flag == 1)
        {
            return 1;
        }
    }
    return 0;
}

//-------------------------------------------------
// 三 を見つける
//-------------------------------------------------
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

//-------------------------------------------------
// 四 を見つける
//-------------------------------------------------
int isShi(int board[][BOARD_SIZE], int x, int y)
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
        // ５つの連続するマスの４つが石で残り１つが空か４つの連続するマスの４つが石で，両端のどちらかに石がない
        for (j = 0; j < 5; j++)
        {
            if (checkOutPos(x + j * dx[i], y + j * dy[i]))
            {
                if (board[y][x] == board[y + j * dy[i]][x + j * dx[i]])
                {
                    sum_stone++;
                    if (sum_stone == 4)
                        break;
                }
                else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE)
                { // 飛び四の可能性あり
                    sum_space++;
                }
                else // 違う色の石がある
                    break;
            }
        }
        if (sum_stone == 4 && sum_space == 1)
        {
            // 飛び四
            num++;
            printf("飛び四(%d %d)\n", x, y);
        }
        else if (sum_stone == 4 && sum_space == 0)
        {
            //４連
            if ((checkOutPos(x - dx[i], y - dy[i]) && board[y - dy[i]][x - dx[i]] == STONE_SPACE) ||
                (checkOutPos(x + 4 * dx[i], y + 4 * dy[i]) && board[y + 4 * dy[i]][x + 4 * dx[i]] == STONE_SPACE))
            {
                num++;
                printf("四連(%d %d)\n", x, y);
            }
        }
    }
    return num;
}

//-------------------------------------------------
// ６連続を見つける
//-------------------------------------------------
int isRoku(int board[][BOARD_SIZE], int x, int y)
{
    int i, j;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};
    int sum_stone = 0;
    int sum_space = 0;

    // i    0:[＼]方向 1:[│]方向 2:[─]方向 3:[／]方向
    for (i = 0; i < 4; i++)
    {
        sum_space = 0;
        sum_stone = 0;

        // 6つの連続するマスの5つが石で残り１つが空
        for (j = 0; j < 6; j++)
        {
            if (checkOutPos(x + j * dx[i], y + j * dy[i]))
            {
                if (board[y][x] == board[y + j * dy[i]][x + j * dx[i]])
                {
                    sum_stone++;
                    if (sum_stone == 5 && sum_space == 1)
                    { // ６つの連続するマスのうち，5個に石あり
                        return true;
                    }
                }
                else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE)
                { // 可能性あり
                    sum_space++;
                }
                else // 違う色の石がある
                    break;
            }
        }
    }
    return false;
}