//von https://lastminuteengineers.com/drv8833-arduino-tutorial/
// Define the control inputs

//#define SECOND_MOTOR
//D1+D2 (M3) funktionieren noch nicht
//D7+D8 (M2) funktionieren!
//D3+D4 (M1) TODO noch ungetestet
#define MOT_A1_PIN D7//D1  //10
#define MOT_A2_PIN D8//D2  //9

#ifdef SECOND_MOTOR
#define MOT_B1_PIN 6
#define MOT_B2_PIN 5
#endif  //SECOND_MOTOR

void setup(void) {
  // Set all the motor control inputs to OUTPUT
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
#ifdef SECOND_MOTOR
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);
#endif  //SECOND_MOTOR

  // Turn off motors - Initial state
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
#ifdef SECOND_MOTOR
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);
#endif  //SECOND_MOTOR

  // Initialize the serial UART at 9600 baud
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
}

void loop(void) {
  // Generate a fixed motion sequence to demonstrate the motor modes.

  // Ramp speed up.
  for (int i = 0; i < 11; i++) {
    spin_and_wait(25 * i, 25 * i, 50);
  }
  // Full speed forward.
  spin_and_wait(255, 255, 200);

  // Ramp speed into full reverse.
  for (int i = 0; i < 21; i++) {
    spin_and_wait(255 - 25 * i, 255 - 25 * i, 50);
  }

  // Full speed reverse.
  spin_and_wait(-255, -255, 200);

  // Stop.
  spin_and_wait(0, 0, 200);

  // Full speed, forward, turn, reverse, and turn for a two-wheeled base.
  spin_and_wait(255, 255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(-255, 255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(-255, -255, 2000);
  spin_and_wait(0, 0, 1000);
  spin_and_wait(255, -255, 2000);
  spin_and_wait(0, 0, 1000);
}

/// Set the current on a motor channel using PWM and directional logic.
///
/// \param pwm    PWM duty cycle ranging from -255 full reverse to 255 full forward
/// \param IN1_PIN  pin number xIN1 for the given channel
/// \param IN2_PIN  pin number xIN2 for the given channel
void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN) {
  if (pwm < 0) {  // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);

  } else {  // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}

/// Set the current on both motors.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
void set_motor_currents(int pwm_A, int pwm_B) {
  set_motor_pwm(pwm_A, MOT_A1_PIN, MOT_A2_PIN);
#ifdef SECOND_MOTOR
  set_motor_pwm(pwm_B, MOT_B1_PIN, MOT_B2_PIN);
#endif  //SECOND_MOTOR

  // Print a status message to the console.
  Serial.print("Set motor A PWM = ");
  Serial.print(pwm_A);
#ifdef SECOND_MOTOR
  Serial.print(" motor B PWM = ");
  Serial.println(pwm_B);
#endif  //SECOND_MOTOR
}

/// Simple primitive for the motion sequence to set a speed and wait for an interval.
///
/// \param pwm_A  motor A PWM, -255 to 255
/// \param pwm_B  motor B PWM, -255 to 255
/// \param duration delay in milliseconds
void spin_and_wait(int pwm_A, int pwm_B, int duration) {
  set_motor_currents(pwm_A, pwm_B);
  delay(duration);
}