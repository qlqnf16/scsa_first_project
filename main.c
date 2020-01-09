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
// test_KSB :: 판정 logic 구현을 위한 함수 및 logic test
/***********************************************************/

#if 1
#define K_SHEET_SIZE 80
#define K_UNIT_TIME 0.25
#define K_JUDGE_DENSITY 10

int k_note_num = 25;			// 떴다떴다비행기의 note 수 == 25
int k_note_cnt = 0;				// note는 최소 박자 단위
int k_seg_cnt = 0;				// seg는 (note / K_JUDGE_DENSITY)

double k_unit_score;
int k_now_score;
int k_score_max = 100;			// 100점 만점
int k_score_bad;
int k_score_exc;
int k_score_sum = 0;

int k_sheet[K_SHEET_SIZE] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 2,0,0,4, 8,0,4,0, 2,0,2,0, 2,0,0,0, 4,0,4,0, 4,0,0,0, 2,0,2,0, 2,0,0,0, 2,0,0,4, 8,0,4,0, 2,0,2,0, 2,0,0,0, 4,0,4,0, 2,0,4,0, 8,0,0,0};
						//  악보 data                              떴            다     떴         다         비       행            기                     날       아          라                     날         아         라                     떴             다       떴     다           비        행          기                     우       리           비        행          기

//#define KEY_1 ## 이거 어떻게 쓸 생각이지?
//#define KEY_2 ##
//#define KEY_3 ##
//#define KEY_4 ##

/*
int check_start(){

	if(start key가 눌리면) return 1;
	return 0;
}
*/

void judge_note(){
	k_seg_cnt++;
	if(k_seg_cnt == K_JUDGE_DENSITY) k_note_cnt++; k_seg_cnt = 0;

	if((k_seg_cnt >= K_JUDGE_DENSITY*3/4 && k_sheet[k_note_cnt+1] == keyInput_j) || (k_seg_cnt <= K_JUDGE_DENSITY/4 && k_sheet[k_note_cnt] == keyInput_j)) k_now_score = k_score_exc; k_score_sum += k_now_score;
	if((k_seg_cnt < K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt+1] == keyInput_j && k_sheet[k_note_cnt] != keyInput_j) || (k_seg_cnt > K_JUDGE_DENSITY/2 && k_sheet[k_note_cnt] == keyInput_j && k_sheet[k_note_cnt+1] != keyInput_j)) k_now_score = k_score_bad; k_score_sum += k_now_score;
	// exc, bad 판정.
	return;
}

void play_game(){
	k_unit_score = (double)k_score_max / (double)k_note_num;
	k_score_bad = 0 * k_unit_score;		// 모든 note를 bad를 받으면, 0점이 나오도록.
	k_score_exc = 1 * k_unit_score;		// 모든 note를 exc를 받으면, k_score_max점이 나오도록.

	for(;;){
	// 노래가 끝날 때까지 loop
		// timer start
		// TIM_Delay_ms()를 쓰면 timer start는 안 해도 되는 듯.

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
	// 노래가 끝나면 loop 종료
	}
	// 노래 종료 후 점수 출력. main()에서 print_score() 호출.
	return;
}

/*
void print_score(){

	return;
}
*/

int main(void){
	/*
	for(;;){
		if (check_start() == 1) break;
	}
	*/
	play_game();
//	print_score();
	return 0;
}

#endif
