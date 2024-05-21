#include <NewPing.h>

// ����� �����
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 200

#define MOTOR_LEFT_FORWARD 3
#define MOTOR_LEFT_BACKWARD 4
#define MOTOR_RIGHT_FORWARD 5
#define MOTOR_RIGHT_BACKWARD 6

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// ����� ����� �Ș�
const int gridWidth = 10;
const int gridHeight = 10;
bool grid[gridWidth][gridHeight];

// ������ � ��� ����
int posX = 0, posY = 0;
int direction = 0; // 0 = ���� 1 = ��ޡ 2 = ���ȡ 3 = ���

void setup() {
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  Serial.begin(9600);

  // �������� ����� �Ș�
  for (int x = 0; x < gridWidth; x++) {
    for (int y = 0; y < gridHeight; y++) {
      grid[x][y] = false; // ���� ���� ����
    }
  }
}

void loop() {
  // ���� ���� ���� ����
  grid[posX][posY] = true;

  // ����� ���� ���� ���� ���� ���� �� ������� �� ������� flood fill
  bool found = false;
  for (int x = 0; x < gridWidth; x++) {
    for (int y = 0; y < gridHeight; y++) {
      if (!grid[x][y]) {
        moveTo(x, y);
        found = true;
        break;
      }
    }
    if (found) break;
  }

  // ǐ� ��� ������ ���� �����ϡ ���� ����� ���
  if (!found) {
    stopRobot();
    while (true);
  }
}

void moveTo(int x, int y) {
  while (posX != x || posY != y) {
    unsigned int distance = sonar.ping_cm();
    if (distance > 0 && distance < 20) {
      stopRobot();
      moveBackward();
      delay(500);
      turnRight();
      delay(500);
    } else {
      int targetDirection = getDirectionTo(x, y);
      while (direction != targetDirection) {
        turnRight();
        delay(500);
      }
      moveForward();
      delay(1000); // ���� ���� �ј� � ����
      updatePosition();
    }
  }
}

int getDirectionTo(int x, int y) {
  if (posX < x) return 1; // �ј� �� ���
  if (posX > x) return 3; // �ј� �� ���
  if (posY < y) return 2; // �ј� �� ����
  return 0; // �ј� �� ����
}

void updatePosition() {
  switch (direction) {
    case 0: posY--; break;
    case 1: posX++; break;
    case 2: posY++; break;
    case 3: posX--; break;
  }

  // ����� ���� �� ������ �Ș� ���� �����
  if (posX < 0) posX = 0;
  if (posX >= gridWidth) posX = gridWidth - 1;
  if (posY < 0) posY = 0;
  if (posY >= gridHeight) posY = gridHeight - 1;
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  direction = (direction + 1) % 4;
}

void stopRobot() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}
