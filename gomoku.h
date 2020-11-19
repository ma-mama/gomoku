#define BOARD_SIZE 15 // 盤面サイズ 15 * 15
#define STONE_SPACE 0 // 盤面にある石 なし
#define STONE_BLACK 1 // 盤面にある石 黒
#define STONE_WHITE 2 // 盤面にある石 白

#define NOKINJITE 0 // 禁じ手なし
#define SANSAN 1    // 三三の禁じ手
#define SHISHI 2    // 四四の禁じ手
#define CHOLEN 4    // 長連の禁じ手

extern int calcPutPos(int board[][BOARD_SIZE], int com, int *pos_x, int *pos_y);
extern int checkOutPos(int x, int y);
extern  int isSan(int board[][BOARD_SIZE], int x, int y);
extern void boardInit(int board[][BOARD_SIZE]);