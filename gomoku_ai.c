//------------------------------------------------------------------------
// �ܖڂȂ��AI
//  ���������D
//  ���L�̃T���v���́C���O�Ɏ������u�����΂�8�ߖT�̂ǂ����Ƀ����_���ɐ΂�u������
//
//  ��������
//    board[BOARD_SIZE][BORARD_SIZE]�F�ܖڕ��ׂ̔�
//    com �F �R���s���[�^������(STONE_WHITE)������(STONE_BLACK)����\���D
//    pos_x, pos_y�F�΂�u���ꏊ��x���W�Cy���W �����o�͗p
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
        // ���O�̐΂̏ꏊ���o���Ă������߂̕ϐ� �K���ɏ�����
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

    // �R���s���[�^���ŏ���1��̏ꍇ�́C(7, 7)�ɐ΂�u��
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
        //�����őO�ɒu�����ꏊ��8�ߖT�ɒu��
        if (count > 2)
        { // 2��u�����Ƃ��āC�ǂ��ɂ��u���Ȃ��Ȃ����ꍇ�͂ǂ��ł��悢���炨���Ă݂�
            *pos_y = (int)(rand() % 15);
            *pos_x = (int)(rand() % 15);
            printf("%d\n", count);
        }
        else
        {
            //���ӂ̃R�}������̂��ǂ������肵�A���̃R�}�Ƃ̔��΂ɂ͒u���Ȃ�(��U)(�{�[�h�̕]����0��)
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
        { // bug�o��
            printf("out of board(%d %d)\n", *pos_x, *pos_y);
            count++;
        }
    }
    pre[0] = *pos_y;
    pre[1] = *pos_x;

    return 0;
}

//���ӂ̑���̃R�}�𒲂�
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