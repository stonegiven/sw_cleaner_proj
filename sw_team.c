#include <stdio.h>
#include <stdbool.h>
// #include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "sw_team.h"

#define MOVE_DELAY 300000

//맵은 3차원 배열로, 1, 2차원까지는 위치 좌표고 3차원부턴
//[x, y]
//x-> 0 : 공백 1 : 벽 2 : 로봇
//y-> 0 : 먼지 없음 1 : 먼지 있음

// 맵을 구성하는 cell간 거리는 모터가 움직일 수 있는 최소단위라 생각해주세요.
// test진행을 위해 이 맵은 안쓰입니다.
int map[20][20][2] = {
	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}, {0,1}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
	{{1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,1}, {0,0}, {0,0}, {0,1}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {2,1}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}, {1,0}, {1,0}},
	{{1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
	{{1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}},
	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}}
};

int robot_location[2]; // (row, col)
int robot_rotation; // 0~359	0->up,  90->right,  180->down,  270->left
int state_change_count = 0; // state에 유의미한 차이를 줬을때 ++
bool map_print_flag = true; // 맵을 출력할지 결정

// 모든 sleep문은 디버깅용으로, 제거하여도 정상작동합니다.

// direction방향으로 한칸 이동했을때의 좌표를 계산합니다. (중요: 정적변수 반환)
int* calc_location(int direction) { // 0:front 1:left 2:right 3:back
	static int result[2];

	// 이동 방향에 따른 초기 adder 설정
	int adder[2] = { 0, 0 };
	if (direction == 0) { adder[0] = -1; adder[1] = 0; } // front
	if (direction == 1) { adder[0] = 0; adder[1] = -1; } // left
	if (direction == 2) { adder[0] = 0; adder[1] = 1; } // right
	if (direction == 3) { adder[0] = 1; adder[1] = 0; } // back

	// 로봇의 회전에 따른 방향 보정
	int adjusted_x = 0, adjusted_y = 0;
	if (robot_rotation / 90 == 0) {
		adjusted_x = adder[0];
		adjusted_y = adder[1];
	}
	else if (robot_rotation / 90 == 1) {
		adjusted_x = adder[1];
		adjusted_y = -adder[0];
	}
	else if (robot_rotation / 90 == 2) {
		adjusted_x = -adder[0];
		adjusted_y = -adder[1];
	}
	else if (robot_rotation / 90 == 3) {
		adjusted_x = -adder[1];
		adjusted_y = adder[0];
	}
	else {
		printf("critical error! rotation value: %d...!\n", robot_rotation);
		exit(1); // 회전 값 오류
	}

	result[0] = robot_location[0] + adjusted_x;
	result[1] = robot_location[1] + adjusted_y;

	return result;
}

// 맵의 locaiton좌표에 위치한 물체를 반환합니다.
// idx = 0 -> 0:공백 1:벽 2:로봇
// idx = 1 -> 0:공백 1:먼지
int map_check(int* location, int idx) {
	return map[location[0]][location[1]][idx];
}

// 현재까지 진행된 모든 변화를 기반으로 맵과 각종 변수들을 출력합니다.
void printing(struct robot rb, bool* ol, bool de) { // par: 로봇, obstacle_location, dust_existance
	if (!map_print_flag) return;

	printf("\033[H\033[J");
	char* state = "";
	char* cleaner_state = "";
	char* motor_state = "";

	switch (rb.state) {
	case 0:
		state = "init"; break;
	case 1:
		state = "forward"; break;
	case 2:
		state = "turnleft"; break;
	case 3:
		state = "turnright"; break;
	case 4:
		state = "backward"; break;
	case 5:
		state = "turbo"; break;
	}

	if (rb.cleaner.on) {
		if (rb.cleaner.turbo)
			cleaner_state = "turbo";
		else
			cleaner_state = "on";
	}
	else {
		cleaner_state = "off";
	}

	if (rb.motor.mf) { motor_state = "move_forward"; }
	else if (rb.motor.mb) { motor_state = "move_forward"; }
	else if (rb.motor.tl) { motor_state = "turn_left"; }
	else if (rb.motor.tr) { motor_state = "turn_right"; }
	else { motor_state = "off"; }

	printf("state :%s\n", state);
	printf("cleaner : %s\n", cleaner_state);
	printf("motor : %s\n", motor_state);
	printf("robot_location :%-2d,%-2d\n", robot_location[0], robot_location[1]);
	printf("robot_rotation : %d\n", robot_rotation);
	printf("f: %d l: %d r: %d d: %d\n", ol[0], ol[1], ol[2], de);
	for (int row = 0; row < sizeof(map) / sizeof(map[0]); row++) {
		for (int col = 0; col < sizeof(map[0]) / sizeof(map[0][0]); col++) {
			switch (map[row][col][0]) {
			case 0: // 공백
				if (map[row][col][1] == 0)
					printf("  ");
				else
					printf(". ");
				break;

			case 1: // 벽
				printf("o ");
				break;

			case 2: // 로봇
				if (map[row][col][1] == 0) {
					if (robot_rotation / 90 == 0) { printf("^ "); }
					else if (robot_rotation / 90 == 1) { printf("> "); }
					else if (robot_rotation / 90 == 2) { printf("v "); }
					else if (robot_rotation / 90 == 3) { printf("< "); }
				}
				else
					printf("# "); // 터보모드
				break;
			}
		}
		printf("\n");
	}
	usleep(MOVE_DELAY);
}

// 로봇을 새로운 좌표로 이동시킵니다. 실패시 아무것도 하지 않습니다.
void move_robot_to(int* new_location) {
	// 벽박음!
	if (map[new_location[0]][new_location[1]][0] == 1) {
		printf("crash!");
		return;
	}

	map[robot_location[0]][robot_location[1]][0] = 0;
	robot_location[0] = new_location[0];
	robot_location[1] = new_location[1];
	map[robot_location[0]][robot_location[1]][0] = 2;
}

// motor구조체가 가지는 함수로, motor의 지역변수를 받아와 알맞게 움직입니다.
void move(bool mf, bool mb, bool tl, bool tr) {
	if (mf & mb) { // 예외처리 : move_foward, move_backward가 같이 있음
		printf("error!"); exit(0);
	}

	if (tl & tr) { // 예외처리 : turn_left, turn_right가 같이 있음
		printf("error!"); exit(0);
	}

	if (mf) { // 앞으로 이동
		move_robot_to(calc_location(0));
	}

	if (mb) { // 뒤로 이동
		move_robot_to(calc_location(3));
	}

	if (tl) { // 1도 좌회전
		robot_rotation--;
		(robot_rotation == -1) ? robot_rotation = 359 : 0;
	}

	if (tr) { // 1도 우회전
		robot_rotation++;
		(robot_rotation == 360) ? robot_rotation = 0 : 0;
	}
}

// 구조체 cleaner가 가지는 함수로, cleaner의 지역변수를 기반으로 청소를 진행합니다.
void clean(bool on, bool turbo) {
	if (on) {
		if (turbo) {
			// if (rand() % 4 == 0) // 확률적으로 제거
			map[robot_location[0]][robot_location[1]][1] = 0;
		}
	}
}

// 구조체 sensor가 가지는 함수로, 1을 감지했다면 true를 반환합니다.
bool sense(int target) { // 먼지든 벽이든 둘다 1입니다.
	if (1 == target) return true;
	else return false;
}

// 센서 3개의 결과를 2진수 3자리로 표현합니다. obstacle_location(ol)입니다.
bool* determine_obstacle_location(struct robot rb) {
	static bool result[3];
	bool f = rb.front_sensor.sense(map_check(calc_location(0), 0));
	bool l = rb.left_sensor.sense(map_check(calc_location(1), 0));
	bool r = rb.right_sensor.sense(map_check(calc_location(2), 0));

	result[0] = f;
	result[1] = l;
	result[2] = r;
	return result;
}

// 센서 1개의 결과로 먼지의 유무를 저장합니다. dust_existence(de)입니다.
bool determine_dust_existance(struct robot rb) {
	bool d = rb.dust_sensor.sense(map_check(robot_location, 1));
	return d;
}

// 로봇을 움직이는 controller역할의 함수입니다. state변화가 한도에 다다를때 까지 무한반복합니다.
void control(struct robot *rb, int max_state_change_count) {
	while (state_change_count < max_state_change_count) {
		rb->tick++;

		// motor과 cleaner에게 작동 신호를 보냅니다. 각자 자신의 지역변수에 따른 움직임을 취합니다.
		rb->motor.move(rb->motor.mf, rb->motor.mb, rb->motor.tl, rb->motor.tr);
		rb->cleaner.clean(rb->cleaner.on, rb->cleaner.turbo);

		// 총 4개의 센서를 작동시켜 아래의 저장공간에 저장합니다.
		bool* ol = determine_obstacle_location(*rb); // obstacle_location
		bool de = determine_dust_existance(*rb); // dust_existance

		switch (rb->state) {
		case 0: // INIT	
			printing(*rb, ol, de); // 디버깅용
			if (ol[0]) {
				rb->motor.tr = true;
				rb->motor.mf = false;
				rb->cleaner.on = false;
				rb->state = 3;
				state_change_count++;
			}
			else {
				rb->motor.mf = true;
				rb->cleaner.on = true;
			}
			break;

		case 1: // MOVE_FORWARD
			printing(*rb, ol, de); // 디버깅용
			if (de) { // dust detected -> turbo
				rb->motor.mf = false;
				rb->cleaner.turbo = true;
				rb->state = 5; 
			}
			else if ((ol[0] & ol[1] & ol[2])) { // f & l & r -> backward
				rb->motor.mf = false;
				rb->motor.mb = true;
				rb->cleaner.on = false;
				rb->state = 4; 
			}
			else if (!ol[1]) { // !l -> turn_left
				rb->motor.mf = false;
				rb->motor.tl = true;
				rb->cleaner.on = false;
				rb->state = 2; 
			}
			else if ((ol[0] & !ol[2])) { // f & !r ->turn_right
				rb->motor.mf = false;
				rb->motor.tr = true;
				rb->cleaner.on = false;
				rb->state = 3; 
			}
			state_change_count++;
			break;

		case 2: // TURN_LEFT
			if (robot_rotation % 90 == 0) { // 회전을 마침 -> forward
				printing(*rb, ol, de); // 디버깅용
				rb->motor.mf = true;
				rb->motor.tl = false;
				rb->cleaner.on = true;
				rb->state = 1;
				state_change_count++;
			}
			break;

		case 3: // TURN_RIGHT
			if (robot_rotation % 90 == 0) { // 회전을 마침 -> forward
				printing(*rb, ol, de); // 디버깅용
				if (!ol[0]) {
					rb->motor.mf = true;
					rb->motor.tr = false;
					rb->cleaner.on = true;
					rb->state = 1;
				}
				state_change_count++;
			}
			break;

		case 4: // BACKWARD
			printing(*rb, ol, de); // 디버깅용
			if (!ol[2] | !ol[1]) { // !r | !l -> turn_right
				rb->motor.mb = false;
				rb->motor.tr = true;
				rb->state = 3;
			}
			state_change_count++;
			break;

		case 5: // TURBO
			printing(*rb, ol, de); // 디버깅용
			if (!de) { // 먼지 제거됨 -> forward
				rb->motor.mf = true;
				rb->cleaner.turbo = false;
				rb->state = 1;
				state_change_count++;
			}
			break;
		}
	}
}

// 로봇 구조체를 초기화합니다. 요구된 시작 state에 따라 내부 구조체들의 지역변수를 초기화합니다.
struct robot generate_robot(int state) {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (map[row][col][0] == 2) {
				robot_location[0] = row;
				robot_location[1] = col;
			}
		}
	}
	robot_rotation = 0;
	state_change_count = 0;

	struct robot rb;
	rb.state = state;
	rb.tick = 0;

	rb.front_sensor.sense = sense;
	rb.right_sensor.sense = sense;
	rb.left_sensor.sense = sense;
	rb.dust_sensor.sense = sense;

	rb.motor.mf = false;
	rb.motor.mb = false;
	rb.motor.tl = false;
	rb.motor.tr = false;
	rb.motor.move = move;

	rb.cleaner.on = false;
	rb.cleaner.turbo = false;
	rb.cleaner.clean = clean;

	rb.control = control;

	// 시작 state에 따른 내부 구조체 지역변수 초기화
	switch (state) {
	case 1:
		rb.motor.mf = true;
		rb.cleaner.on = true;
		robot_location[0]++;
		break;
	case 2:
		rb.motor.tl = true;
		break;
	case 3:
		rb.motor.tr = true;
		break;
	case 4:
		rb.motor.mb = true;
		robot_location[0]--;
		break;
	case 5:
		rb.cleaner.on = true;
		rb.cleaner.turbo = true;
		break;
	}

	return rb;
}

// sw_team_test.c의 main대신 사용할 시 로봇의 동작을 시각적으로 확인할 수 있습니다.
// int main() {
// 	srand(time(NULL));

// 	struct robot rb = generate_robot(0);
// 	rb.control(&rb, 10000);
// }