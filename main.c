/* 절대 이부분은 삭제하거나 수정하지 마시오 */

#include "m0_cube_lib.h"
#include <stdlib.h>

#define 	BLACK	0x000000
#define 	RED 	0xFF0000
#define 	GREEN	0x00FF00
#define 	BLUE	0x0000FF
#define 	YELLOW	0xFFFF00
#define 	CYAN	0x00FFFF
#define 	VIOLET	0xFF00FF
#define 	WHITE	0xFFFFFF

unsigned int COLOR[] = {BLACK, RED, GREEN, BLUE, YELLOW, CYAN, VIOLET, WHITE};
unsigned int icon[][2] = {{0x0066FFFF, 0xFF7E3C18},{0xFFC3A599,0x99A5C3FF}};

#define BASE  (500)

enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};

/***********************************************************/
// 프로젝트! 즐거운 DDR
/***********************************************************/
#if 1
int keyDir[4][3] = {{6,6, RED}, {6, 7, GREEN}, {5, 7, BLUE}, {7, 7, YELLOW}};
int scores_j[2] = {3, 1};

int dir_j, oldDir_j = 0;
int keyBaseColor = WHITE;
int totalScore = 0;
int lives = 3;
int isGameOVer = 0;

// 키보드 입력
int showKeyPressed(char key) {
   RGB_Put_Pixel(keyDir[oldDir_j][0], keyDir[oldDir_j][1], keyBaseColor);

    switch (key) {
        case 'w': dir_j = 0; break;
        case 's': dir_j = 1; break;
        case 'a': dir_j = 2; break;
        case 'd': dir_j = 3; break;

        case '0':
        case '1':
        case '2': sumScores(key); break;
    }


    RGB_Put_Pixel(keyDir[dir_j][0], keyDir[dir_j][1], keyDir[dir_j][2]);
    oldDir_j = dir_j;
}

// 점수관련 로직
void sumScores(char score) {
    // excellent = 2; good = 1; miss = 0;
    if (score == '2') totalScore += scores_j[0];
    if (score == '1') totalScore += scores_j[1];
    if (score == '0') decreaseLife();
    Uart2_Printf("Total Score Now: %d\n", totalScore);
}

int showTotalScore(void) {}

// 목숨관련 로직
int decreaseLife(void) {
    lives -= 1;
    RGB_Put_Pixel(5+lives, 0, BLACK);
    if (lives == -1) showGameOver();
}

void showGameOver(void) {
    Uart2_Printf("GAME OVER!");
    Buzzer_Beep(C1,N1);
    RGB_Draw_Rect(0, 0, 7, 7, BLACK);
    isGameOVer = 1;
}


int main(void) {
	char keyInput_j;
	int lock_j = 0;

    // 초기 LED 세팅
    for (int i = 0; i<4; i++) {
        RGB_Put_Pixel(keyDir[i][0], keyDir[i][1], keyBaseColor);
    }
    for (int i = 0; i < 3; i++) {
        RGB_Put_Pixel(5+i, 0, RED);
    }

	// key 입력
    for (;;) {
		keyInput_j = Uart2_Get_Pressed();

		if (keyInput_j != 0 && lock_j == 0) {
			Uart2_Printf("keyInput_j: %c\n", keyInput_j);
			showKeyPressed(keyInput_j);
		}

		if((lock_j == 1) && (keyInput_j == 0))
		{
			lock_j = 0;
		}

		if (isGameOVer == 1) return 0;

	}

}

#endif
