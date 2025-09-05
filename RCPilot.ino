// --- TB6612FNG motor driver pins (Elegoo Smart Car V4.0 shield) ---
const int PWMA = 5;   // Right motor speed (PWM)
const int AIN1 = 7;   // Right motor direction  (HIGH = forward, LOW = reverse)
const int PWMB = 6;   // Left motor speed (PWM)
const int BIN1 = 8;   // Left motor direction   (HIGH = forward, LOW = reverse)
const int STBY = 3;   // Standby (enable)

// --- Line sensors ---
const int leftIR  = A2;
const int midIR   = A1;
const int rightIR = A0;

// --- Thresholds ---
// White ≈ low (~40), Black ≈ higher (>50)
int thL = 100;  
int thM = 100;
int thR = 100;

// --- Speed settings ---
int baseSpeed = 50;   // straight forward
int turnSpeed = 80;   // boost on turns
int searchSpeed = 40; // slow speed while wiggling

// --- Wiggle settings ---
unsigned long lastSeenTime = 0;     // time when line was last detected
int wiggleTime = 300;               // base wiggle duration (ms)
int maxWiggleTime = 800;            // max wiggle time
int pauseTime = 500;                // pause between wiggles
bool wiggleLeft = true;             // alternate wiggle direction

// --- Forward burst settings ---
bool justFoundLine  = false;    // flag to trigger bursts
int maxBursts       = 3;        // number of bursts when line is found
int burstDuration   = 200;      // each burst length (ms)
int burstPause      = 150;      // pause between bursts (ms)

// ------------------ Motor Control Helpers ------------------
void setRightMotor(int speed, bool forward) {
  if (speed == 0) {
    analogWrite(PWMA, 0);
    return;
  }
  digitalWrite(AIN1, forward ? HIGH : LOW);
  analogWrite(PWMA, speed);
}

void setLeftMotor(int speed, bool forward) {
  if (speed == 0) {
    analogWrite(PWMB, 0);
    return;
  }
  digitalWrite(BIN1, forward ? HIGH : LOW);
  analogWrite(PWMB, speed);
}

void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

// ------------------ Setup ------------------
void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(leftIR, INPUT);
  pinMode(midIR, INPUT);
  pinMode(rightIR, INPUT);

  digitalWrite(STBY, HIGH); // Enable motor driver
  Serial.begin(9600);
}

// ------------------ Main Loop ------------------
void loop() {
  // Read sensors
  int L = analogRead(leftIR);
  int M = analogRead(midIR);
  int R = analogRead(rightIR);

  bool leftBlack  = L > thL;
  bool midBlack   = M > thM;
  bool rightBlack = R > thR;

  // Debugging output
  Serial.print("L:"); Serial.print(L); Serial.print(leftBlack ? "[B] " : "[W] ");
  Serial.print(" M:"); Serial.print(M); Serial.print(midBlack ? "[B] " : "[W] ");
  Serial.print(" R:"); Serial.print(R); Serial.println(rightBlack ? "[B]" : "[W]");

  // ------------------ Line following logic ------------------
  // check if line is lost (all white or all black)
  bool lostLine = (!leftBlack && !midBlack && !rightBlack)   // all white
               || (leftBlack && midBlack && rightBlack);     // all black

  if (!lostLine) {
    // Normal Line Following
    if (!justFoundLine) {
      // Just reacquired the line → do short forward bursts
      for (int i = 0; i < maxBursts; i++)
      {
        setRightMotor(baseSpeed, true);
        setLeftMotor(baseSpeed, true);
        delay(burstDuration);               // move forward briefly
        stopMotors();
        delay(burstPause);                  // pause between bursts
      }
      justFoundLine = true;                 // only once until line lost again
    }

    lastSeenTime = millis();        // reset lost timer

    // --- Correction Logic ---
    if (!leftBlack && midBlack && !rightBlack) {
      // Straight
      setRightMotor(baseSpeed, true);
      setLeftMotor(baseSpeed, true);
    }
    else if (leftBlack && !midBlack && !rightBlack) {
      // Hard Left Pivot
      setRightMotor(turnSpeed, true);
      setLeftMotor(turnSpeed, false);
    }
    else if (leftBlack && midBlack && !rightBlack) {
      // Curve left
      setRightMotor(turnSpeed, true);
      setLeftMotor(baseSpeed, true);
    }
    else if (!leftBlack && !midBlack && rightBlack) {
      // Hard Right Pivot
      setRightMotor(turnSpeed, false);
      setLeftMotor(turnSpeed, true);
    }
    else if (!leftBlack && midBlack && rightBlack) {
      // Curve right
      setRightMotor(baseSpeed, true);
      setLeftMotor(turnSpeed, true);
    }
  }

  // ------------------ Lost line → wiggle search ------------------
  else {
    unsigned long lostTime = millis() - lastSeenTime;
    int currentWiggle = wiggleTime + lostTime / 5;        // increase wiggle over time
    
    if (currentWiggle > maxWiggleTime){
      currentWiggle = maxWiggleTime;
    } 
    
    unsigned long startTime = millis();

    // Wiggle left WHILE checking sensors while moving
    if (wiggleLeft) {
      while (millis() - startTime < currentWiggle) {
        setRightMotor(searchSpeed, true);
        setLeftMotor(0, true);

        // Recheck sensors mid-wiggle
        int L = analogRead(leftIR);
        int M = analogRead(midIR);
        int R = analogRead(rightIR);
        if ((L > thL) || (M > thM) || (R > thR)) {
          stopMotors();
          justFoundLine = false; // allow bursts again
          return; // break early → line reacquired
        }
      }
    } 
    // Wiggle right WHILE checking sensors while moving
    else {
      while (millis() - startTime < currentWiggle) {
        setRightMotor(0, true);
        setLeftMotor(searchSpeed, true);

        // Recheck sensors mid-wiggle
        int L = analogRead(leftIR);
        int M = analogRead(midIR);
        int R = analogRead(rightIR);
        if ((L > thL) || (M > thM) || (R > thR)) {
          stopMotors();
          justFoundLine = false;
          return; 
        }
      }
    }

    // Finished wiggle attempt without finding line
    stopMotors();
    delay(pauseTime);           // pause before next wiggle
    wiggleLeft = !wiggleLeft;   // aternate direction
    justFoundLine = false;      // reset so bursts happen next time
  }

  delay(50);
}
