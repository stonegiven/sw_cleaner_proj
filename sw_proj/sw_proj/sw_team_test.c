#include "unity.h"
#include <stdlib.h>
#include <time.h>
#include "sw_team.h" // ���� �ڵ� ��� ����

void setUp(void) {
    srand(time(NULL)); // �׽�Ʈ ȯ�� �ʱ�ȭ
}

typedef struct {
    bool front;     // F: Front ��ֹ� ���� ����
    bool left;      // L: Left ��ֹ� ���� ����
    bool right;     // R: Right ��ֹ� ���� ����
    bool dust;
} TestCase;

void setup_test_case(TestCase tc) {
    // �� �ʱ�ȭ
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            map[i][j][0] = 0; // Clear all walls
            map[i][j][1] = 0; // Clear all dust
        }
    }

    // �ܰ��� ��ֹ� ��ġ
    for (int i = 0; i < 20; i++) {
        map[0][i][0] = 1;
        map[19][i][0] = 1;
        map[i][0][0] = 1;
        map[i][19][0] = 1;
    }

    // �κ� �ʱ� ��ġ
    robot_location[0] = 10;
    robot_location[1] = 10;
    robot_rotation = 0; // Facing up
    map[10][10][0] = 2; // Place robot

    // TestCase ���� ���� �ֺ� ��ֹ�/���� ����
    if (tc.front) map[9][10][0] = 1; // ��ֹ�: ����
    if (tc.left)  map[10][9][0] = 1; // ��ֹ�: ����
    if (tc.right) map[10][11][0] = 1; // ��ֹ�: ����
    if (tc.dust)  map[10][10][1] = 1; // ����: ���� ��ġ
}


void tearDown(void) {}


// F = Front, L = Left, R = Right, Dust = ���� ���� ����

// Test0000 : F=0, L=0, R=0, Dust=0
void test_case_0000(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);
    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // F
    TEST_ASSERT_FALSE(obstacle[1]); // L
    TEST_ASSERT_FALSE(obstacle[2]); // R

    bool dust_detected = determine_dust_existance(rb);
    TEST_ASSERT_FALSE(dust_detected); // Dust
}

// Test0010 : F=0, L=0, R=1, Dust=0
void test_case_0010(void) {
    TestCase tc = { 0, 0, 1, 0 };
    setup_test_case(tc);
    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // F
    TEST_ASSERT_FALSE(obstacle[1]); // L
    TEST_ASSERT_TRUE(obstacle[2]);  // R

    bool dust_detected = determine_dust_existance(rb);
    TEST_ASSERT_FALSE(dust_detected); // Dust
}

// Test0100 : F=0, L=1, R=0, Dust=0
void test_case_0100(void) {
    TestCase tc = { 0, 1, 0, 0 };
    setup_test_case(tc);
    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // F
    TEST_ASSERT_TRUE(obstacle[1]);  // L
    TEST_ASSERT_FALSE(obstacle[2]); // R

    bool dust_detected = determine_dust_existance(rb);
    TEST_ASSERT_FALSE(dust_detected); // Dust
}

// Test1000 : F=1, L=0, R=0, Dust=0
void test_case_1000(void) {
    TestCase tc = { 1, 0, 0, 0 };
    setup_test_case(tc);
    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_TRUE(obstacle[0]);  // F
    TEST_ASSERT_FALSE(obstacle[1]); // L
    TEST_ASSERT_FALSE(obstacle[2]); // R

    bool dust_detected = determine_dust_existance(rb);
    TEST_ASSERT_FALSE(dust_detected); // Dust
}

// Test0110 : F=0, L=1, R=1, Dust=0
void test_case_0110(void) {
    TestCase tc = { 0, 1, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    bool* obstacle = determine_obstacle_location(rb);

    TEST_ASSERT_FALSE(obstacle[0]); // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_FALSE(determine_dust_existance(rb)); // Dust
}

// Test1100: F=1, L=1, R=0, Dust=0
void test_case_1100(void) {
    TestCase tc = { 1, 1, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    bool* obstacle = determine_obstacle_location(rb);

    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_FALSE(obstacle[2]); // Right
    TEST_ASSERT_FALSE(determine_dust_existance(rb)); // Dust
}

// Test1010: F=1, L=0, R=1, Dust=0
void test_case_1010(void) {
    TestCase tc = { 1, 0, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    bool* obstacle = determine_obstacle_location(rb);

    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_FALSE(obstacle[1]); // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_FALSE(determine_dust_existance(rb)); // Dust
}

// Test1110: F=1, L=1, R=1, Dust=0
void test_case_1110(void) {
    TestCase tc = { 1, 1, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    bool* obstacle = determine_obstacle_location(rb);

    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_FALSE(determine_dust_existance(rb)); // Dust
}

// Test0001: F=0, L=0, R=0, Dust=1
void test_case_0001(void) {
    TestCase tc = { 0, 0, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // Front
    TEST_ASSERT_FALSE(obstacle[1]); // Left
    TEST_ASSERT_FALSE(obstacle[2]); // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test0011: F=0, L=0, R=1, Dust=1
void test_case_0011(void) {
    TestCase tc = { 0, 0, 1, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // Front
    TEST_ASSERT_FALSE(obstacle[1]); // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test0101: F=0, L=1, R=0, Dust=1
void test_case_0101(void) {
    TestCase tc = { 0, 1, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_FALSE(obstacle[2]); // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test1001: F=1, L=0, R=0, Dust=1
void test_case_1001(void) {
    TestCase tc = { 1, 0, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_FALSE(obstacle[1]); // Left
    TEST_ASSERT_FALSE(obstacle[2]); // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test0111: F=0, L=1, R=1, Dust=1
void test_case_0111(void) {
    TestCase tc = { 0, 1, 1, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_FALSE(obstacle[0]); // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test1101: F=1, L=1, R=0, Dust=1
void test_case_1101(void) {
    TestCase tc = { 1, 1, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_FALSE(obstacle[2]); // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test1011: F=1, L=0, R=1, Dust=1
void test_case_1011(void) {
    TestCase tc = { 1, 0, 1, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_FALSE(obstacle[1]); // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

// Test1111 : F=1, L=1, R=1, Dust=1
void test_case_1111(void) {
    TestCase tc = { 1, 1, 1, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);

    bool* obstacle = determine_obstacle_location(rb);
    TEST_ASSERT_TRUE(obstacle[0]);  // Front
    TEST_ASSERT_TRUE(obstacle[1]);  // Left
    TEST_ASSERT_TRUE(obstacle[2]);  // Right
    TEST_ASSERT_TRUE(determine_dust_existance(rb)); // Dust
}

void test_Determine_Obstacle_Location_0(void) { // state : init
    // Case 0: ���� ��ֹ�, ����, ���� ����, ���� ���� �� TURN_RIGHT
    TestCase tc = { 1, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0); // init state(0)�� ������ �����θ� ���� state�̹Ƿ� forward state�� ����.
    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // TURN_RIGHT
}

void test_Determine_Obstacle_Location_1(void) { // state : forward
    // Case 1: ����, ����, ���� ��ֹ� ����, ���� ���� �� TURN_LEFT
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(1); // MOVE_FORWARD ����


    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tl);
    TEST_ASSERT_EQUAL(2, rb.state); // TURN_LEFT
}

void test_Determine_Obstacle_Location_2(void) { // state : forward
    // Case 2: ���� ��ֹ�, ����, ���� ����, ���� ���� �� TURN_LEFT
    TestCase tc = { 1, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    
    // state forward�� �켱 ������ ���� �� ���� state�� �����մϴ�.
    // ���� robot_location�� ��ĭ �ڷ� �Ű� �����մϴ�.

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tl);
    TEST_ASSERT_EQUAL(2, rb.state); // TURN_LEFT
}

void test_Determine_Obstacle_Location_3(void) { // state : forward
    // Case 3: ���� ��ֹ�, ����, ���� ����, ���� ���� �� MOVE_FORWARD ����
    TestCase tc = { 0, 1, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
     

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mf);
    TEST_ASSERT_EQUAL(1, rb.state); // MOVE_FORWARD
}

void test_Determine_Obstacle_Location_4(void) { // state : forward
    // Case 4: ���� ��ֹ�, ����, ���� ����, ���� ���� �� TURN_LEFT
    TestCase tc = {0, 0, 1, 0};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tl);
    TEST_ASSERT_EQUAL(2, rb.state);
}

void test_Determine_Obstacle_Location_5(void) { // state : forward
    // Case 5: ���� ��ֹ�, ���� ��ֹ�, ���� ���� �� TURN_RIGHT
    TestCase tc = {1, 1, 0, 0};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // TURN_RIGHT
}

void test_Determine_Obstacle_Location_6(void) { // state : forward
    // Case 6: ���� ��ֹ�, ���� ��ֹ�, ���� ���� �� TURN_LEFT
    TestCase tc = {1, 0, 1, 0};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tl);
    TEST_ASSERT_EQUAL(2, rb.state); // TURN_LEFT
}

void test_Determine_Obstacle_Location_7(void) { // state : forward
    // Case 7: ����, ���� ��ֹ� ����, ���� ���� �� MOVE_FORWARD ����
    TestCase tc = {0, 1, 1, 0};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mf);
    TEST_ASSERT_EQUAL(1, rb.state); // MOVE_FORWARD
}

void test_Determine_Obstacle_Location_8(void) { // state : forward
    // Case 8: ����, ����, ���� ��ֹ� ����, ���� ���� �� MOVE_BACKWARD
    TestCase tc = {1, 1, 1, 0};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mb);
    TEST_ASSERT_EQUAL(4, rb.state); // BACKWARD
}

void test_Determine_Obstacle_Location_9(void) { // state : forward
    // Case 9: ���� ����, ��ֹ� ���� �� TURBO
    TestCase tc = {0, 0, 0, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_10(void) { // state : forward
    // Case 10: ���� ����, ���� ��ֹ� �� TURBO
    TestCase tc = {1, 0, 0, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_11(void) { // state : forward
    // Case 11: ���� ����, ���� ��ֹ� �� TURBO
    TestCase tc = {0, 1, 0, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_12(void) { // state : forward
    // Case 12: ���� ����, ���� ��ֹ� �� TURBO
    TestCase tc = {0, 0, 1, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_13(void) { // state : forward
    // Case 13: ���� ����, ����, ���� ��ֹ� �� TURBO
    TestCase tc = {1, 1, 0, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_14(void) { // state : forward
    // Case 14: ���� ����, ����, ���� ��ֹ� �� TURBO
    TestCase tc = {1, 0, 1, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_15(void) { // state : forward
    // Case 15: ���� ����, ����, ���� ��ֹ� �� TURBO
    TestCase tc = {0, 1, 1, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_16(void) { // state : forward
    // Case 16: ���� ����, ����, ����, ���� ��ֹ� �� TURBO
    TestCase tc = {1, 1, 1, 1};
    setup_test_case(tc);

    struct robot rb = generate_robot(1);
    

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_17(void) { // state : turn_left
    // Case 17: ����, ����, ����, ���� ������
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(2);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mf);
    TEST_ASSERT_EQUAL(1, rb.state); // forward
}

void test_Determine_Obstacle_Location_18(void) { // state : turn_right
    // Case 18: ����, ����, ���� ������, ���� ��ֹ� �� turn_right
    TestCase tc = { 0, 0, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(3);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // turn_right
}

void test_Determine_Obstacle_Location_19(void) { // state : turn_right
    // Case 19: ����, ����, ���� ������, ���� ���� �� FORWARD
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(3);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mf);
    TEST_ASSERT_EQUAL(1, rb.state); // FORWARD
}

void test_Determine_Obstacle_Location_20(void) { // state : backward
    // Case 20: ����, ���� ������, ����, ���� ��ֹ� �� BACKWARD
    TestCase tc = { 0, 1, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(4);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.mb);
    TEST_ASSERT_EQUAL(4, rb.state); // BACKWARD
}

void test_Determine_Obstacle_Location_21(void) { // state : backward
    // Case 21: ����, ���� ������, ���� ��ֹ�, ���� ���� �� TURN_RIGHT
    TestCase tc = { 0, 1, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(4);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // TURN_RIGHT
}

void test_Determine_Obstacle_Location_22(void) { // state : backward
    // Case 22: ����, ���� ������, ����, ���� ���� �� TURN_RIGHT
    TestCase tc = { 0, 0, 1, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(4);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // TURN_RIGHT
}

void test_Determine_Obstacle_Location_23(void) { // state : backward
    // Case 23: ����, ���� ������, ����, ���� ���� �� TURN_RIGHT
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(4);

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.motor.tr);
    TEST_ASSERT_EQUAL(3, rb.state); // TURN_RIGHT
}

void test_Determine_Obstacle_Location_24(void) { // state : turbo
    // Case 24: ���� ����, ����, ����, ���� ������ �� TURBO
    TestCase tc = { 0, 0, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(5); // �ϴ� clean�ϰ� state�� ���ϱ⿡... Ȯ���ϱ� ��ƴ�...

    rb.control(&rb, 1);

    TEST_ASSERT_TRUE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(5, rb.state); // TURBO
}

void test_Determine_Obstacle_Location_25(void) { // state : turbo
    // Case 25: ���� ����, ����, ����, ���� ������ �� FORWARD
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(5);

    rb.control(&rb, 1);

    TEST_ASSERT_FALSE(rb.cleaner.turbo);
    TEST_ASSERT_EQUAL(1, rb.state); // FORWARD
}


// ���� �������̽� �׽�Ʈ
void test_Moter_Interface_1(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    int* expected_location = calc_location(0);

    rb.motor.mf = true;
    rb.motor.move(true, false, false, false); // ���� ��� ����

    TEST_ASSERT_EQUAL_INT(expected_location[0], robot_location[0]);
    TEST_ASSERT_EQUAL_INT(expected_location[1], robot_location[1]);
}

void test_Moter_Interface_2(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    rb.motor.mf = false;
    rb.motor.move(false, false, false, false); // ���� ��� ����

    TEST_ASSERT_EQUAL_INT(10, robot_location[0]);
    TEST_ASSERT_EQUAL_INT(10, robot_location[1]);
}

void test_Moter_Interface_3(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    int expected_rotation = (robot_rotation - 1 + 360) % 360; // 90�� ȸ������ �Ϸ��� ���� �ʿ�!!!!!!!!!!!!!!!!!!!!

    rb.motor.tl = true;
    rb.motor.move(false, false, true, false); // ��ȸ�� ��� ����

    TEST_ASSERT_EQUAL_INT(expected_rotation, robot_rotation);
}

void test_Moter_Interface_4(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    int expected_rotation = (robot_rotation + 1) % 360;

    rb.motor.tr = true;
    rb.motor.move(false, false, false, true); // ��ȸ�� ��� ����

    TEST_ASSERT_EQUAL_INT(expected_rotation, robot_rotation);
}

// �κ� ���� �׽�Ʈ
void test_Move_Forward(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(1); // state: forward���� ����
    int* expected_location = calc_location(0);
    int expected_x = expected_location[0];
    int expected_y = expected_location[1];

    rb.control(&rb, 1); // state ��ȭ 1������ ����

    TEST_ASSERT_EQUAL_INT(expected_x, robot_location[0]);
    TEST_ASSERT_EQUAL_INT(expected_y, robot_location[1]);
}

void test_Move_Backward(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(4);
    int* expected_location = calc_location(3); // state: backward���� ����
    int expected_x = expected_location[0];
    int expected_y = expected_location[1];

    rb.control(&rb, 1); // state ��ȭ 1������ ����

    TEST_ASSERT_EQUAL_INT(expected_x, robot_location[0]);
    TEST_ASSERT_EQUAL_INT(expected_y, robot_location[1]);
}

void test_Turn_Left(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(2);
    int expected_rotation = (robot_rotation - 90 + 360) % 360;

    rb.control(&rb, 1); // state ��ȭ 1������ ����

    TEST_ASSERT_EQUAL_INT(expected_rotation, robot_rotation);
}

void test_Turn_Right(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(3);
    int expected_rotation = (robot_rotation + 90 + 360) % 360;

    rb.control(&rb, 1); // state ��ȭ 1������ ����

    TEST_ASSERT_EQUAL_INT(expected_rotation, robot_rotation);
}

// Ŭ���� �������̽� �׽�Ʈ
void test_Cleaner_Interface_1(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    rb.cleaner.on = true;                           
    TEST_ASSERT_TRUE(rb.cleaner.on);
    TEST_ASSERT_FALSE(rb.cleaner.turbo);
}

void test_Cleaner_Control_1(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    rb.cleaner.clean(true, false); // ���� ����
    TEST_ASSERT_FALSE(map[robot_location[0]][robot_location[1]][1]); // ���� ���� Ȯ��
}

void test_Cleaner_Control_2(void) {
    TestCase tc = { 0, 0, 0, 1 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    map[robot_location[0]][robot_location[1]][1] = 1; // ���� ����
    rb.cleaner.clean(true, true); // Ŭ���� �ͺ� �۵�
    TEST_ASSERT_FALSE(map[robot_location[0]][robot_location[1]][1]); // ���� ���� Ȯ��
}

void test_Cleaner_Control_3(void) {
    TestCase tc = { 0, 0, 0, 0 };
    setup_test_case(tc);

    struct robot rb = generate_robot(0);
    map[robot_location[0]][robot_location[1]][1] = 0; // ���� ����
    rb.cleaner.clean(false, false); // Ŭ���� ����
    TEST_ASSERT_FALSE(rb.cleaner.on);
}


int main(void) {
    UNITY_BEGIN();

    map_print_flag = false;

    // ��ֹ� ���� �׽�Ʈ
    RUN_TEST(test_case_0000);
    RUN_TEST(test_case_0010);
    RUN_TEST(test_case_0100);
    RUN_TEST(test_case_1000);
    RUN_TEST(test_case_0110);
    RUN_TEST(test_case_1100);
    RUN_TEST(test_case_1010);
    RUN_TEST(test_case_1110);
    RUN_TEST(test_case_0001);
    RUN_TEST(test_case_0011);
    RUN_TEST(test_case_0101);
    RUN_TEST(test_case_1001);
    RUN_TEST(test_case_0111);
    RUN_TEST(test_case_1101);
    RUN_TEST(test_case_1011);
    RUN_TEST(test_case_1111);

    // ��ֹ� ���� �� ���� �׽�Ʈ
    RUN_TEST(test_Determine_Obstacle_Location_0);
    RUN_TEST(test_Determine_Obstacle_Location_1);
    RUN_TEST(test_Determine_Obstacle_Location_2);
    RUN_TEST(test_Determine_Obstacle_Location_3);
    RUN_TEST(test_Determine_Obstacle_Location_4);
    RUN_TEST(test_Determine_Obstacle_Location_5);
    RUN_TEST(test_Determine_Obstacle_Location_6);
    RUN_TEST(test_Determine_Obstacle_Location_7);
    RUN_TEST(test_Determine_Obstacle_Location_8);
    RUN_TEST(test_Determine_Obstacle_Location_9);
    RUN_TEST(test_Determine_Obstacle_Location_10);
    RUN_TEST(test_Determine_Obstacle_Location_11);
    RUN_TEST(test_Determine_Obstacle_Location_12);
    RUN_TEST(test_Determine_Obstacle_Location_13);
    RUN_TEST(test_Determine_Obstacle_Location_14);
    RUN_TEST(test_Determine_Obstacle_Location_15);
    RUN_TEST(test_Determine_Obstacle_Location_16);
    RUN_TEST(test_Determine_Obstacle_Location_17);
    RUN_TEST(test_Determine_Obstacle_Location_18);
    RUN_TEST(test_Determine_Obstacle_Location_19);
    RUN_TEST(test_Determine_Obstacle_Location_20);
    RUN_TEST(test_Determine_Obstacle_Location_21);
    RUN_TEST(test_Determine_Obstacle_Location_22);
    RUN_TEST(test_Determine_Obstacle_Location_23);
    RUN_TEST(test_Determine_Obstacle_Location_24);
    RUN_TEST(test_Determine_Obstacle_Location_25);

     // ���� �������̽� �׽�Ʈ
    RUN_TEST(test_Moter_Interface_1);
    RUN_TEST(test_Moter_Interface_2);
    RUN_TEST(test_Moter_Interface_3);
    RUN_TEST(test_Moter_Interface_4);

    // �κ� ���� �׽�Ʈ
    RUN_TEST(test_Move_Forward);
    RUN_TEST(test_Move_Backward);
    RUN_TEST(test_Turn_Left);
    RUN_TEST(test_Turn_Right);

    // Ŭ���� �������̽� �׽�Ʈ
    RUN_TEST(test_Cleaner_Interface_1);
    RUN_TEST(test_Cleaner_Control_1);
    RUN_TEST(test_Cleaner_Control_2);
    RUN_TEST(test_Cleaner_Control_3);

    return UNITY_END();
}
