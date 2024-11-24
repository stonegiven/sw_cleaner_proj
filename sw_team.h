#ifndef SW_TEAM_H
#define SW_TEAM_H

#include <stdbool.h>

// 상수 정의
#define MOVE_FORWARD 0
#define TURN_LEFT 1
#define TURN_RIGHT 2
#define MOVE_BACKWARD 3

#define POWER_ON 1
#define POWER_OFF 0
#define POWER_UP 2
// #define MOVE_DELAY 300
// #define ROTATE_DELAY 1

// 구조체 정의
struct sensor {
    bool (*sense)(int target);
};

struct motor {
    bool mf;  
    bool mb;  
    bool tl;  
    bool tr;  

    void (*move)(bool mf, bool mb, bool tl, bool tr);
};

struct cleaner {
    bool on;    
    bool turbo; 

    void (*clean)(bool on, bool turbo);
};

struct robot {
    int state;         
    int tick;         

    struct sensor front_sensor;
    struct sensor right_sensor;
    struct sensor left_sensor;
    struct sensor dust_sensor;

    struct motor motor;
    struct cleaner cleaner;

    bool one_tick;

    void (*control)(struct robot*, int);
};

// 전역 변수 선언
extern int map[20][20][2];
extern int robot_location[2];
extern int robot_rotation;
extern int state_change_count;
extern bool map_print_flag;

// 함수 선언
struct robot generate_robot(int state);
void control(struct robot *rb, int max_state_change_count);
void move(bool mf, bool mb, bool tl, bool tr);
void clean(bool on, bool turbo);
int map_check(int* location, int idx);
bool* determine_obstacle_location(struct robot rb);
bool determine_dust_existance(struct robot rb);
int* calc_location(int direction);
void move_robot_to(int* new_location);
void printing(struct robot rb, bool* ol, bool de);
bool sense(int target);

#endif // SW_TEAM_H
