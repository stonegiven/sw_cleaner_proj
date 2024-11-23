#ifndef SW_TEAM_H
#define SW_TEAM_H

#include <stdbool.h>

// ��� ����
#define MOVE_FORWARD 0           // ����
#define TURN_LEFT 1              // ��ȸ��
#define TURN_RIGHT 2             // ��ȸ��
#define MOVE_BACKWARD 3          // ����

#define POWER_ON 1               // Ŭ���� ���� �ѱ�
#define POWER_OFF 0              // Ŭ���� ���� ����
#define POWER_UP 2               // Ŭ���� �ͺ� ���
#define MOVE_DELAY 300
#define ROTATE_DELAY 1


// ����ü ����
struct sensor {
    /**
     * �־��� ���� �������� ���� ���¸� ��ȯ�մϴ�.
     * @param target ���� ��� ��
     * @return ���� ����
     */
    bool (*sense)(int target);
};

struct motor {
    bool mf;  // Move forward
    bool mb;  // Move backward
    bool tl;  // Turn left
    bool tr;  // Turn right

    /**
     * �κ��� ���͸� �����Ͽ� �̵��մϴ�.
     * @param mf move forward
     * @param mb move backward
     * @param tl turn left
     * @param tr turn right
     */
    void (*move)(bool mf, bool mb, bool tl, bool tr);
};

struct cleaner {
    bool on;            // û�ұ� �۵� ����
    bool turbo;         // �ͺ� ��� ����

    /**
    * �κ��� û�ұ⸦ �����Ͽ� ������ �����մϴ�.
    * @param on û�ұ� �۵� ����
    * @param turbo �ͺ� ��� ����
    */
    void (*clean)(bool on, bool turbo);
};

struct robot {
    int state;          // �κ� ����: 0=INIT, 1=MOVE_FORWARD, 2=TURN_LEFT, ...
    int tick;           // ���� ���� �ð�

    struct sensor front_sensor; // ���� ����
    struct sensor right_sensor; // ���� ����
    struct sensor left_sensor;  // ���� ����
    struct sensor dust_sensor;  // ���� ����

    struct motor motor;         // ����
    struct cleaner cleaner;     // û�ұ�

    bool one_tick; // �� ƽ�� �������� ���θ� ��Ÿ���� �÷���

    /**
     * �κ��� ���� ���¸� �����մϴ�. (���� ����)
     * @param rb �κ� ����ü
     * @param max_state_change_count state�� �ٲ� �� �ִ� �ִ� �� (������)
     */
    void (*control)(struct robot*, int);
};

// ���� ���� ����
extern int map[20][20][2];     // �� ������: [x, y][0:����/��/�κ�, 1:����]
extern int robot_location[2];  // �κ� ��ġ [row, col]
extern int robot_rotation;     // �κ� ȸ�� (0=��, 90=������, 180=�Ʒ�, 270=����)
extern int state_change_count;
extern bool map_print_flag;

// �Լ� ����

/**
 * �κ��� �ʱ� ���¸� �����մϴ�.
 * @return �ʱ�ȭ�� �κ� ����ü
 * @param state �κ� �� �ʱ� state ����
 */
struct robot generate_robot(int state);

/**
 * �κ��� ���� ���¸� �����մϴ�. (���� ����)
 * @param rb �κ� ����ü�� �ּڰ�
 * @param max_state_change_count state�� �ٲ� �� �ִ� �ִ� �� (������)
 */
void control(struct robot *rb, int max_state_change_count);

/**
 * �κ��� ���͸� �����Ͽ� �̵��մϴ�.
 * @param mf move forward
 * @param mb move backward
 * @param tl turn left
 * @param tr turn right
 */
void move(bool mf, bool mb, bool tl, bool tr);

/**
 * �κ��� û�ұ⸦ �����Ͽ� ������ �����մϴ�.
 * @param on û�ұ� �۵� ����
 * @param turbo �ͺ� ��� ����
 */
void clean(bool on, bool turbo);

/**
 * �־��� ��ǥ�� �� �����͸� ��ȯ�մϴ�.
 * @param location ��ǥ [row, col]
 * @param idx ������ �ε��� (0:����/��/�κ�, 1:����)
 * @return �� ������
 */
int map_check(int* location, int idx);

/**
 * �κ� �ֺ��� ��ֹ� ���¸� Ȯ���մϴ�.
 * @param rb �κ� ����ü
 * @return ��ֹ� ���� �迭 [front, left, right]
 */
bool* determine_obstacle_location(struct robot rb);

/**
 * ���� �κ� ��ġ���� ������ �����ϴ��� Ȯ���մϴ�.
 * @param rb �κ� ����ü
 * @return ���� ���� ����
 */
bool determine_dust_existance(struct robot rb);

/**
 * �־��� �������� �̵��� ��ǥ�� ����մϴ�.
 * @param direction 0=front, 1=left, 2=right, 3=back
 * @return ���� ��ǥ
 */
int* calc_location(int direction);

/**
 * �κ��� ���ο� ��ġ�� �̵��մϴ�.
 * @param new_location �̵��� ��ǥ [row, col]
 */
void move_robot_to(int* new_location);

/**
 * �κ� ���¿� �� ������ ����մϴ�. (������)
 * @param rb �κ� ����ü
 * @param ol ��ֹ� ����
 * @param de ���� ���� ����
 */
void printing(struct robot rb, bool* ol, bool de);

/**
 * �־��� ���� �������� ���� ���¸� ��ȯ�մϴ�.
 * @param target ���� ��� ��
 * @return ���� ����
 */
bool sense(int target);

#endif // SW_TEAM_H
#pragma once
