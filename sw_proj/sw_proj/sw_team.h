#ifndef SW_TEAM_H
#define SW_TEAM_H

#include <stdbool.h>

// 상수 정의
#define MOVE_FORWARD 0           // 전진
#define TURN_LEFT 1              // 좌회전
#define TURN_RIGHT 2             // 좌회전
#define MOVE_BACKWARD 3          // 후진

#define POWER_ON 1               // 클리너 전원 켜기
#define POWER_OFF 0              // 클리너 전원 끄기
#define POWER_UP 2               // 클리너 터보 모드
#define MOVE_DELAY 300
#define ROTATE_DELAY 1


// 구조체 정의
struct sensor {
    /**
     * 주어진 값을 기준으로 감지 상태를 반환합니다.
     * @param target 감지 대상 값
     * @return 감지 여부
     */
    bool (*sense)(int target);
};

struct motor {
    bool mf;  // Move forward
    bool mb;  // Move backward
    bool tl;  // Turn left
    bool tr;  // Turn right

    /**
     * 로봇의 모터를 제어하여 이동합니다.
     * @param mf move forward
     * @param mb move backward
     * @param tl turn left
     * @param tr turn right
     */
    void (*move)(bool mf, bool mb, bool tl, bool tr);
};

struct cleaner {
    bool on;            // 청소기 작동 여부
    bool turbo;         // 터보 모드 여부

    /**
    * 로봇의 청소기를 제어하여 먼지를 제거합니다.
    * @param on 청소기 작동 여부
    * @param turbo 터보 모드 여부
    */
    void (*clean)(bool on, bool turbo);
};

struct robot {
    int state;          // 로봇 상태: 0=INIT, 1=MOVE_FORWARD, 2=TURN_LEFT, ...
    int tick;           // 상태 지속 시간

    struct sensor front_sensor; // 전방 센서
    struct sensor right_sensor; // 우측 센서
    struct sensor left_sensor;  // 좌측 센서
    struct sensor dust_sensor;  // 먼지 센서

    struct motor motor;         // 모터
    struct cleaner cleaner;     // 청소기

    bool one_tick; // 한 틱만 실행할지 여부를 나타내는 플래그

    /**
     * 로봇의 동작 상태를 제어합니다. (메인 루프)
     * @param rb 로봇 구조체
     * @param max_state_change_count state가 바뀔 수 있는 최대 수 (디버깅용)
     */
    void (*control)(struct robot*, int);
};

// 전역 변수 선언
extern int map[20][20][2];     // 맵 데이터: [x, y][0:공백/벽/로봇, 1:먼지]
extern int robot_location[2];  // 로봇 위치 [row, col]
extern int robot_rotation;     // 로봇 회전 (0=위, 90=오른쪽, 180=아래, 270=왼쪽)
extern int state_change_count;
extern bool map_print_flag;

// 함수 선언

/**
 * 로봇의 초기 상태를 설정합니다.
 * @return 초기화된 로봇 구조체
 * @param state 로봇 의 초기 state 설정
 */
struct robot generate_robot(int state);

/**
 * 로봇의 동작 상태를 제어합니다. (메인 루프)
 * @param rb 로봇 구조체의 주솟값
 * @param max_state_change_count state가 바뀔 수 있는 최대 수 (디버깅용)
 */
void control(struct robot *rb, int max_state_change_count);

/**
 * 로봇의 모터를 제어하여 이동합니다.
 * @param mf move forward
 * @param mb move backward
 * @param tl turn left
 * @param tr turn right
 */
void move(bool mf, bool mb, bool tl, bool tr);

/**
 * 로봇의 청소기를 제어하여 먼지를 제거합니다.
 * @param on 청소기 작동 여부
 * @param turbo 터보 모드 여부
 */
void clean(bool on, bool turbo);

/**
 * 주어진 좌표의 맵 데이터를 반환합니다.
 * @param location 좌표 [row, col]
 * @param idx 데이터 인덱스 (0:공백/벽/로봇, 1:먼지)
 * @return 맵 데이터
 */
int map_check(int* location, int idx);

/**
 * 로봇 주변의 장애물 상태를 확인합니다.
 * @param rb 로봇 구조체
 * @return 장애물 상태 배열 [front, left, right]
 */
bool* determine_obstacle_location(struct robot rb);

/**
 * 현재 로봇 위치에서 먼지가 존재하는지 확인합니다.
 * @param rb 로봇 구조체
 * @return 먼지 존재 여부
 */
bool determine_dust_existance(struct robot rb);

/**
 * 주어진 방향으로 이동한 좌표를 계산합니다.
 * @param direction 0=front, 1=left, 2=right, 3=back
 * @return 계산된 좌표
 */
int* calc_location(int direction);

/**
 * 로봇을 새로운 위치로 이동합니다.
 * @param new_location 이동할 좌표 [row, col]
 */
void move_robot_to(int* new_location);

/**
 * 로봇 상태와 맵 정보를 출력합니다. (디버깅용)
 * @param rb 로봇 구조체
 * @param ol 장애물 상태
 * @param de 먼지 존재 여부
 */
void printing(struct robot rb, bool* ol, bool de);

/**
 * 주어진 값을 기준으로 감지 상태를 반환합니다.
 * @param target 감지 대상 값
 * @return 감지 여부
 */
bool sense(int target);

#endif // SW_TEAM_H
#pragma once
