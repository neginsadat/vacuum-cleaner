#include <NewPing.h>

//  ⁄—Ì› ÅÌ‰ùÂ«
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define MAX_DISTANCE 200

#define MOTOR_LEFT_FORWARD 3
#define MOTOR_LEFT_BACKWARD 4
#define MOTOR_RIGHT_FORWARD 5
#define MOTOR_RIGHT_BACKWARD 6

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

//  ⁄—Ì› «»⁄«œ ‘»òÂ
const int gridWidth = 10;
const int gridHeight = 10;
bool grid[gridWidth][gridHeight];

// „Êﬁ⁄Ì  Ê ÃÂ  —»« 
int posX = 0, posY = 0;
int direction = 0; // 0 = ‘„«·° 1 = ‘—ﬁ° 2 = Ã‰Ê»° 3 = €—»

void setup() {
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  Serial.begin(9600);

  // „ﬁœ«—œÂÌ «Ê·ÌÂ ‘»òÂ
  for (int x = 0; x < gridWidth; x++) {
    for (int y = 0; y < gridHeight; y++) {
      grid[x][y] = false; // Â‰Ê“  „Ì“ ‰‘œÂ
    }
  }
}

void loop() {
  //  „Ì“ ò—œ‰ Œ«‰Â ›⁄·Ì
  grid[posX][posY] = true;

  // Ì«› ‰ Œ«‰Â »⁄œÌ »—«Ì  „Ì“ ò—œ‰ »« «” ›«œÂ «“ «·êÊ—Ì „ flood fill
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

  // «ê— Â„Â Œ«‰ÂùÂ«  „Ì“ ‘œÂù«‰œ° —»«  „ Êﬁ› ‘Êœ
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
      delay(1000); // “„«‰ »—«Ì Õ—ò  Ìò Œ«‰Â
      updatePosition();
    }
  }
}

int getDirectionTo(int x, int y) {
  if (posX < x) return 1; // Õ—ò  »Â ‘—ﬁ
  if (posX > x) return 3; // Õ—ò  »Â €—»
  if (posY < y) return 2; // Õ—ò  »Â Ã‰Ê»
  return 0; // Õ—ò  »Â ‘„«·
}

void updatePosition() {
  switch (direction) {
    case 0: posY--; break;
    case 1: posX++; break;
    case 2: posY++; break;
    case 3: posX--; break;
  }

  // »——”Ì «Ì‰òÂ «“ „ÕœÊœÂ ‘»òÂ Œ«—Ã ‰‘ÊÌ„
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
