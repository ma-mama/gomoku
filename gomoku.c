// �ܖڕ��ׁi�A��j���͍���(7,7)�ɒu��
// 3��ނ̋ւ���̂ǂ�����[���Ƃ��邩�́Ccheck�ϐ��ŃR���p�C�����Ɏw�肷��D
// �ւ���̃��[���́C�����̃T�C�g�����č��������D
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

int check = 6; // 1:�O�O�̂� 2:�l�l�̂� 4:���A�̂� 3:�O�O�Ǝl�l 5:�O�O�ƒ��A 6:�l�l�ƒ��A 7:�S��

//=======================================================
// main
//=======================================================
int main()
{
    // �ϐ��錾
    int board[BOARD_SIZE][BOARD_SIZE];
    int which_turn;
    int com;

    // ��������
    if (gameInit(board, &which_turn, &com) == 1)
        return 1;

    boardPrint(board);

    //---- ���C�����[�v
    while (1)
    {
        //--- ���͏���
        if (inputPutPos(board, which_turn, com) == 1)
            return 1;

        //--- ��Ԍ�㏈��
        changeTurn(&which_turn);

        //--- �o�͏���
        boardPrint(board);

        //--- �I������
        if (gameEndProcess(board))
        {
            break;
        }
    }
    return 0;
}

//-------------------------------------------------
// �u���ꏊ����
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
        printf("%s", (which == 1) ? "��" : "��");
        printf("�̔Ԃł��B�ǂ��ɒu���܂��� x y �̏��ɓ��͂��Ă�������\n> ");
        while (1)
        {
            if (fgets(buf, sizeof buf, stdin) == NULL || buf[0] == '\n')
                return 1;
            if (sscanf(buf, "%d %d", &pos_x, &pos_y) != 2)
            {
                printf("�s���ȓ��͂ł�\n >");
                continue;
            }
            if (checkOutPos(pos_x, pos_y) && board[pos_y][pos_x] == STONE_SPACE)
            {
                break;
            }
            else
            {
                printf("�s���ȓ��͂ł�\n> ");
                continue;
            }
        }
    }
    board[pos_y][pos_x] = which;
    return 0;
}

//-------------------------------------------------
// ��Ԍ�㏈��
//-------------------------------------------------
void changeTurn(int *which_turn)
{
    *which_turn = (*which_turn == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
}

//-------------------------------------------------
// �͈͊O�`�F�b�N
//-------------------------------------------------
int checkOutPos(int x, int y)
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

//-------------------------------------------------
// �Q�[����񏉊���
//-------------------------------------------------
int gameInit(int board[][BOARD_SIZE], int *which_turn, int *com)
{
    int in = -1;
    char buf[256];
    do
    {
        printf("����I�т܂����H Yes=>1 No=>0\n> ");
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
// �Ֆʏ���
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
// �Ֆʏo��
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
                printf("�E");
                break;
            case STONE_BLACK:
                printf("��");
                break;
            case STONE_WHITE:
                printf("��");
                break;
            }
        }
        puts("");
    }
    puts("");
}

//-------------------------------------------------
// �Q�[���I�����菈��
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
                printf("���A�̋ւ���ł��B%s�̏����ł�\n", "��");
                return 1;
            }
            if ((check & SANSAN) && board[i][j] == STONE_BLACK && (num = isSan(board, j, i)))
            {
                n_san += num;
                if (n_san >= 2)
                {
                    printf("�O�O�̋ւ���ł��B%s�̏����ł�\n", "��");
                    return 1;
                }
            }
            if ((check & SHISHI) && board[i][j] == STONE_BLACK && (num = isShi(board, j, i)))
            {
                n_shi += num;
                if (n_shi >= 2)
                {
                    printf("�l�l�̋ւ���ł��B%s�̏����ł�\n", "��");
                    return 1;
                }
            }
            if (lenCheck(board, j, i))
            {
                printf("%s�̏����ł��B\n", (board[i][j] == STONE_BLACK) ? "��" : "��");
                return 1;
            }
        }
    }
    return 0;
}

//-------------------------------------------------
// 5�A�m�F
//-------------------------------------------------
int lenCheck(int board[][BOARD_SIZE], int x, int y)
{
    int i, j, len_flag;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};

    // i    0:[�_]���� 1:[��]���� 2:[��]���� 3:[�^]����

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
// �O ��������
//-------------------------------------------------
int isSan(int board[][BOARD_SIZE], int x, int y)
{
    int i, j;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};
    int sum_stone = 0;
    int sum_space = 0;
    int num = 0;

    // i    0:[�_]���� 1:[��]���� 2:[��]���� 3:[�^]����

    for (i = 0; i < 4; i++)
    {
        sum_stone = 0;
        sum_space = 0;
        // ��������ŁC�S�̘A������}�X��3���΂Ŏc��P����ŉE�[���󂩁C3�A������}�X�̂R���΂ŁC�E�[����
        if (!checkOutPos(x - dx[i], y - dy[i]) || (checkOutPos(x - dx[i], y - dy[i]) && board[y - dy[i]][x - dx[i]] != STONE_SPACE))
        {
            // ��������łȂ����C�Ղ̊O
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
                    if (sum_stone == 3) // 3�A��
                        break;
                }
                else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE) // ��юO�̉\������
                {
                    sum_space++;
                }
                else
                { // �Ⴄ�F�̐΂�����
                    break;
                }
            }
        }
        //printf("%d %d sum_stone %d sum_space %d (%d %d)\n", i, j, sum_stone, sum_space, board[y][x], board[y + j * dy[i]][x + j * dx[i]]);

        if (sum_stone == 3 && sum_space == 1)
        {
            // �E����Ȃ�
            if (checkOutPos(x + 4 * dx[i], y + 4 * dy[i]) && board[y + 4 * dy[i]][x + 4 * dx[i]] == STONE_SPACE)
            {
                printf("��юO(%d %d)\n", x, y);
                //��юO
                num++;
            }
        }
        else if (sum_stone == 3 && sum_space == 0)
        {
            // �E����Ȃ�
            if (checkOutPos(x + 3 * dx[i], y + 3 * dy[i]) && board[y + 3 * dy[i]][x + 3 * dx[i]] == STONE_SPACE)
            {
                // 3�A��
                printf("�O�A(%d %d)\n", x, y);
                num++;
            }
        }
    }
    return num;
}

//-------------------------------------------------
// �l ��������
//-------------------------------------------------
int isShi(int board[][BOARD_SIZE], int x, int y)
{
    int i, j;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};
    int sum_stone = 0;
    int sum_space = 0;
    int num = 0;

    // i    0:[�_]���� 1:[��]���� 2:[��]���� 3:[�^]����

    for (i = 0; i < 4; i++)
    {
        sum_stone = 0;
        sum_space = 0;
        // �T�̘A������}�X�̂S���΂Ŏc��P���󂩂S�̘A������}�X�̂S���΂ŁC���[�̂ǂ��炩�ɐ΂��Ȃ�
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
                { // ��юl�̉\������
                    sum_space++;
                }
                else // �Ⴄ�F�̐΂�����
                    break;
            }
        }
        if (sum_stone == 4 && sum_space == 1)
        {
            // ��юl
            num++;
            printf("��юl(%d %d)\n", x, y);
        }
        else if (sum_stone == 4 && sum_space == 0)
        {
            //�S�A
            if ((checkOutPos(x - dx[i], y - dy[i]) && board[y - dy[i]][x - dx[i]] == STONE_SPACE) ||
                (checkOutPos(x + 4 * dx[i], y + 4 * dy[i]) && board[y + 4 * dy[i]][x + 4 * dx[i]] == STONE_SPACE))
            {
                num++;
                printf("�l�A(%d %d)\n", x, y);
            }
        }
    }
    return num;
}

//-------------------------------------------------
// �U�A����������
//-------------------------------------------------
int isRoku(int board[][BOARD_SIZE], int x, int y)
{
    int i, j;
    int dx[] = {1, 0, 1, -1};
    int dy[] = {1, 1, 0, 1};
    int sum_stone = 0;
    int sum_space = 0;

    // i    0:[�_]���� 1:[��]���� 2:[��]���� 3:[�^]����
    for (i = 0; i < 4; i++)
    {
        sum_space = 0;
        sum_stone = 0;

        // 6�̘A������}�X��5���΂Ŏc��P����
        for (j = 0; j < 6; j++)
        {
            if (checkOutPos(x + j * dx[i], y + j * dy[i]))
            {
                if (board[y][x] == board[y + j * dy[i]][x + j * dx[i]])
                {
                    sum_stone++;
                    if (sum_stone == 5 && sum_space == 1)
                    { // �U�̘A������}�X�̂����C5�ɐ΂���
                        return true;
                    }
                }
                else if (board[y + j * dy[i]][x + j * dx[i]] == STONE_SPACE)
                { // �\������
                    sum_space++;
                }
                else // �Ⴄ�F�̐΂�����
                    break;
            }
        }
    }
    return false;
}