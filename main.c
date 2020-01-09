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

// 아래부터 수빈
#define K_SHEET_SIZE 80
#define K_UNIT_TIME 0.25
#define K_JUDGE_DENSITY 10

int k_note_num = 25;			// 떴다떴다비행기의 note 수 == 25
int k_note_cnt = 0;				// note는 최소 박자 단위
int k_seg_cnt = 0;				// seg는 (note / K_JUDGE_DENSITY)
int k_sw = 0;
int k_noinput_cnt = 5;

double k_unit_score;
double k_now_score;
double k_score_max = 100;			// 100점 만점
double k_score_bad;
double k_score_exc;
double k_score_sum = 0;

int k_sheet[] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 2,0,0,4, 8,0,4,0, 2,0,2,0, 2,0,0,0, 4,0,4,0, 4,0,0,0, 2,0,2,0, 2,0,0,0, 2,0,0,4, 8,0,4,0, 2,0,2,0, 2,0,0,0, 4,0,4,0, 2,0,4,0, 8,0,0,0};
						//  악보 data                              떴            다     떴         다         비       행            기                     날       아          라                     날         아         라                     떴             다       떴     다           비        행          기                     우       리           비        행          기

// 아래부터 유리


int cols;
int s_point = 0;

// 아래부터 정민
int keyDir[4][3] = {{4,7, RED}, {5, 7, GREEN}, {6, 7, BLUE}, {7, 7, YELLOW}};
int scores_j[2] = {3, 1};
int buzzList[4] = {C1, D1, E1, F1};

int dir_j, oldDir_j = 0;
int keyBaseColor = VIOLET;
int totalScore = 0;
int lives = 60;
int isGameOver = 0;

char keyInput_j;
int keyInputToCompare;
int lock_j = 0;
int colorNote[4] = {RED, GREEN, BLUE, YELLOW};
int isSongEnded = 0;

int currentNote = 0;
int lifex = 7, lifey = 4;

// =================함수

// 아래부터 수빈


void judge_note(){
	k_seg_cnt++;
	k_noinput_cnt ++;
	k_sw = 0;

	switch(keyInput_j) {
		case 'a': keyInputToCompare = 8; break;
		case 's': keyInputToCompare = 4; break;
		case 'd': keyInputToCompare = 2; break;
		case 'f': keyInputToCompare = 1; break;
	}

	if(k_seg_cnt == K_JUDGE_DENSITY) { k_note_cnt++; s_point++; k_seg_cnt = 0; k_sw = 1;}

	// exc, bad 판정.
	if((k_seg_cnt >= K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt+1] == keyInputToCompare && keyInput_j != 0) || (k_seg_cnt <= K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt] == keyInputToCompare && keyInput_j != 0)) {
		k_now_score = k_score_exc;
		k_score_sum += k_now_score;
		Uart2_Printf("good! %d 현재점수: %.2lf\n", k_seg_cnt,k_score_sum);
		k_noinput_cnt = 0;
	}

	else if((int)keyInput_j != 0 && (k_seg_cnt < K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt+1] == keyInput_j && k_sheet[k_note_cnt] != keyInputToCompare) || (k_seg_cnt > K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt] == keyInputToCompare && k_sheet[k_note_cnt+1] != keyInputToCompare)) {
//		k_now_score = k_score_bad;
		decreaseLife();
		Uart2_Printf("BAD! %d 현재점수: %.2lf\n", k_seg_cnt,k_score_sum);
		Uart2_Printf("keypressed: %d\n", keyInput_j);
		k_noinput_cnt = 0;
	}
//
//	if(k_noinput_cnt == 10 && k_sheet[k_note_cnt] != 0){
////		k_now_score = k_score_bad;
//		decreaseLife();
//		Uart2_Printf("MISS! %d 현재점수: %.2lf\n", k_seg_cnt,k_score_sum);
//		k_noinput_cnt = 0;
//	}

	return;
}

void play_game(){
	k_unit_score = (double)k_score_max / (double)k_note_num;
	k_score_bad = 0;		// 모든 note를 bad를 받으면, 0점이 나오도록.
	k_score_exc = k_unit_score;		// 모든 note를 exc를 받으면, k_score_max점이 나오도록.

	for(;;){
	// 노래가 끝날 때까지 loop
		//key
		keyInput_j = Uart2_Get_Pressed();

		if (keyInput_j != 0 && lock_j == 0) {
			Uart2_Printf("keyInput_j: %c\n", keyInput_j);
			showKeyPressed(keyInput_j);
		}

		if((lock_j == 1) && (keyInput_j == 0))
		{
			lock_j = 0;
		}

		// sheet_display(); // 유리님 job
			// now_time % K_UNIT_TIME??              // TIMER 작동 방식을 알아야 로직 구현 가능할 듯.
			// ""K_UNIT_TIME마다"" display 내림
			// 즉, now_time이 K_UNIT_TIME만큼씩 지날 때마다, 한 줄 아래로 내림.
			// TIMER 오차로 인해 여러 줄 내려가는 것 방지 위해, 한 줄 내린 후 display_lock = 1로 ON.
			// now_time이 K_UNIT_TIME의 반주기가 지나면, display_lock = 0으로 OFF.
		TIM_Delay_ms(K_UNIT_TIME * 1000 / K_JUDGE_DENSITY);
		judge_note();
			// 입력을 판정한다
			// ""K_UNIT_TIME의 1/N 시간마다"" 입력을 판정.
			// K_JUDGE_DENSITY로 N값을 define. 현재는 10.

//		TIM_Delay_ms(K_UNIT_TIME * 1000);
//		jm_judge_note();

		// note 내려오기
		if (k_sw == 1) draw_note();

		keyInput_j = NULL;

	// 노래가 끝나면 loop 종료
		if (isSongEnded == 1) return 0;
		if (isGameOver == 1) return 0;
	}
	// 노래 종료 후 점수 출력. main()에서 print_score() 호출.
	return;
}

// 아래부터 유리
void draw_note ()
{

   int count;
   int colorx;
   int colory=8;
   if (s_point == 75) {
	   isSongEnded =1;
	   Uart2_Printf("Song Ended!\n TOTAL SCORE : %.0lf", k_score_sum);
	   return;
   }

   for (count = s_point; count < s_point+8 /*ex*/; count++)
   {
	   switch(k_sheet[count])
	   {

	   case 8:
		   colorx=0;
		   break;
	   case 4:
		   colorx=1;
		   break;
	   case 2:
		   colorx=2;
		   break;
	   case 0:
		   colorx=30;
		   break;

	   }

	   for (int j=0;j<4;j++)
	   {
			if(j==colorx)
			{
				RGB_Put_Pixel(j,colory,colorNote[j]);
			}
			else
			{
				RGB_Put_Pixel(j,colory,BLACK);
			}
	   }

	   colory--;
   }
   return;
}


// 아래부터 정민
// 키보드 입력
void showKeyPressed(char key) {
   RGB_Put_Pixel(keyDir[oldDir_j][0], keyDir[oldDir_j][1], keyBaseColor);

    switch (key) {
        case 'a': dir_j = 0;  break;
        case 's': dir_j = 1; break;
        case 'd': dir_j = 2; break;
        case 'f': dir_j = 3; break;

    }

    RGB_Put_Pixel(keyDir[dir_j][0], keyDir[dir_j][1], keyDir[dir_j][2]);
    Buzzer_Beep(buzzList[dir_j], N16);
    oldDir_j = dir_j;
}


int showTotalScore(void) {
	RGB_Clr_Screen(BLACK);
	TIM_Delay_ms(500);
	IMG_Num_Disp_2digit(0,0,k_score_sum, YELLOW, BLACK);
}

// 목숨관련 로직
int decreaseLife(void) {
    lives -= 1;
    if (lives % 4 == 0) {
        Uart2_Printf("life: %d\n", lives/4);
        RGB_Put_Pixel(lifex, lifey, BLACK);

        if(lifex < 5) {
        	lifex = 7;
        	lifey -= 1;
        } else {
        	lifex -= 1;
        }
    }
    if (lives == -1) showGameOver();
}

void showGameOver(void) {
    Uart2_Printf("GAME OVER!");
    Buzzer_Beep(C1,N1);
    RGB_Draw_Rect(0, 0, 7, 7, BLACK);
    isGameOver = 1;
}


int main(void) {


    // 초기 LED 세팅
    for (int i = 0; i<4; i++) {
        RGB_Put_Pixel(keyDir[i][0], keyDir[i][1], keyBaseColor);
    }
    for (int j =0; j < 4; j++) {
        for (int i = 0; i < 3; i++) {
            RGB_Put_Pixel(5+i, j, RED);
        }
    }

    play_game();
    showTotalScore();



}

#endif
