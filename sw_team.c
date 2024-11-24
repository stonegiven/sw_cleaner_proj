#include <stdio.h>
#include <stdbool.h>
// #include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "sw_team.h"

#define Sleep(ms) usleep((ms) * 100000)
// #define MOVE_DELAY 0
// #define ROTATE_DELAY 0
#define MOVE_DELAY 300
#define ROTATE_DELAY 1

//맵은 3차원 배열로, 1, 2차원까지는 위치 좌표고 3차원부턴
//[x, y]
//x-> 0 : 공백 1 : 벽 2 : 로봇
//y-> 0 : 먼지 없음 1 : 먼지 있음

// 맵을 구성하는 cell간 거리는 모터가 움직일 수 있는 최소단위라 생각해주세요.
//int map[8][8][2] = {{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}},
//				{{1,0}, {0,0}, {0,0}, {0,0}, {0,1}, {0,0}, {0,0}, {1,0}},
//				{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}, {1,0}},
//				{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//				{{1,0}, {0,0}, {2,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//				{{1,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//				{{1,0}, {0,0}, {1,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//				{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}}};

 int map[20][20][2] = {
 	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}, {0,1}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
 	{{1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,1}, {0,0}, {0,0}, {0,1}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
 	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {2,1}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}, {1,0}, {1,0}},
 	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {1,0}},
 	{{1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {0,1}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {0,1}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}, {1,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {0,0}, {1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}, {1,0}},
 	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}}
 };

//int map[20][20][2] = {
//	{{1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}},
//	{{1,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {1,0}}
//};



// struct sensor {
// 	bool (*sense)(int);
// };

// struct motor {
// 	bool mf; // move foward
// 	bool mb; // move backward
// 	bool tl; // turn left
// 	bool tr; // turn right
// 	void (*move)(bool mf, bool mb, bool tl, bool tr); // 0: forward 1: turn_left 2: turn_right 3: backward
// };

// struct cleaner {
// 	bool on;
// 	bool turbo;
// 	void (*clean)(bool on, bool turbo);
// };

// struct robot {
// 	int state; // 0:INIT 1: MOVE_FORWARD 2: TURN_LEFT 3: TURN_RIGHT 4: BACKWARD 5: TURBO
// 	int tick;

// 	struct sensor front_sensor;
// 	struct sensor right_sensor;
// 	struct sensor left_sensor;
// 	struct sensor dust_sensor;
// 	struct motor motor;
// 	struct cleaner cleaner;

// 	void(*control)(struct robot, max_tick);
// };

int robot_location[2];
int robot_rotation; // 0~359	0->up,  90->right,  180->down,  270->left
int state_change_count = 0; // state에 유의미한 차이를 줬을때 ++
bool map_print_flag = true;

// 모든 sleep문은 디버깅용으로, 제거하여도 정상작동합니다.

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

int map_check(int* location, int idx) {
	return map[location[0]][location[1]][idx];
}

void printing(struct robot rb, bool* ol, bool de) {
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
					printf("# ");
				break;
			}
		}
		printf("\n");
	}
}

void move_robot_to(int* new_location) {
	// 벽박음!
	if (map[new_location[0]][new_location[1]][0] == 1) {
		printf(".");
		return;
	}

	map[robot_location[0]][robot_location[1]][0] = 0;
	robot_location[0] = new_location[0];
	robot_location[1] = new_location[1];
	map[robot_location[0]][robot_location[1]][0] = 2;
}

void move(bool mf, bool mb, bool tl, bool tr) {
	if (mf & mb) {
		printf("error!"); exit(0);
	}

	if (tl & tr) {
		printf("error!"); exit(0);
	}

	if (mf) {
		move_robot_to(calc_location(0));
	}

	if (mb) {
		move_robot_to(calc_location(3));
	}

	if (tl) {
		robot_rotation--;
		(robot_rotation == -1) ? robot_rotation = 359 : 0;
	}

	if (tr) {
		robot_rotation++;
		(robot_rotation == 360) ? robot_rotation = 0 : 0;
	}
}

void clean(bool on, bool turbo) {
	if (on) {
		if (turbo) {
			// if (rand() % 4 == 0) // 확률적으로 제거
			map[robot_location[0]][robot_location[1]][1] = 0;
		}
	}
}

bool sense(int target) { // 먼지든 벽이든 둘다 1임
	if (1 == target) return true;
	else return false;
}

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

bool determine_dust_existance(struct robot rb) {
	bool d = rb.dust_sensor.sense(map_check(robot_location, 1));
	return d;
}

void control(struct robot *rb, int max_state_change_count) {
	while (state_change_count < max_state_change_count) {
		rb->tick++;
		rb->motor.move(rb->motor.mf, rb->motor.mb, rb->motor.tl, rb->motor.tr);
		rb->cleaner.clean(rb->cleaner.on, rb->cleaner.turbo);

		bool* ol = determine_obstacle_location(*rb); // obstacle_location
		bool de = determine_dust_existance(*rb); // dust_existance

		if (map_print_flag && robot_rotation % 30 == 0) printing(*rb, ol, de); // 디버깅용
		switch (rb->state) {
		case 0: // INIT	
			usleep(MOVE_DELAY);
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
			usleep(MOVE_DELAY);
			if (de) { // dust detected
				rb->motor.mf = false;
				rb->cleaner.turbo = true;
				rb->state = 5; 
			}
			else if ((ol[0] & ol[1] & ol[2])) { // backward
				rb->motor.mf = false;
				rb->motor.mb = true;
				rb->cleaner.on = false;
				rb->state = 4; 
			}
			else if (!ol[1]) { // turn_left
				rb->motor.mf = false;
				rb->motor.tl = true;
				rb->cleaner.on = false;
				rb->state = 2; 
			}
			else if ((ol[0] & !ol[2])) { // turn_right
				rb->motor.mf = false;
				rb->motor.tr = true;
				rb->cleaner.on = false;
				rb->state = 3; 
			}
			state_change_count++;
			break;

		case 2: // TURN_LEFT
			usleep(ROTATE_DELAY);
			if (robot_rotation % 90 == 0) {
				rb->motor.mf = true;
				rb->motor.tl = false;
				rb->cleaner.on = true;
				rb->state = 1;
				state_change_count++;
			}
			break;

		case 3: // TURN_RIGHT
			usleep(ROTATE_DELAY);
			if (robot_rotation % 90 == 0) {
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
			usleep(MOVE_DELAY);
			if (!ol[2] | !ol[1]) {
				rb->motor.mb = false;
				rb->motor.tr = true;
				rb->state = 3;
			}
			state_change_count++;
			break;

		case 5: // TURBO
			usleep(MOVE_DELAY);
			if (!de) {
				rb->motor.mf = true;
				rb->cleaner.turbo = false;
				rb->state = 1;
				state_change_count++;
			}
			break;
		}
	}
}

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

//int main() { // robot은 한번에 최대 1개 생성가능...
//	srand(time(NULL));
//
//	struct robot rb = generate_robot(0);
//	rb.control(&rb, 100);
//}