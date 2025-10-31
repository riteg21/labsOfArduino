enum State { S_GREEN, S_YELLOW, S_RED, S_WARNING, NUM_STATES };
enum Event { E_NONE, E_TIMER, E_PED, E_EMERGENCY_ON, E_EMERGENCY_OFF, E_NIGHT_ON, E_NIGHT_OFF, NUM_EVENTS };

State currentState = S_GREEN;
unsigned long stateStart = 0;
unsigned long stateDur = 0;

// Переменные для кнопок
bool pedRequest = false;
bool emergency = false;
bool night_mode_toggle = false;

const int BUTTON_PIN = 2;
const int EMERGENCY_PIN = 3;
const int GREEN_PIN = 11;
const int YELLOW_PIN = 12;
const int RED_PIN = 13;

// Антидребезг
const uint32_t BTN_DEB = 50;
uint32_t buttonTmr = 0;
uint32_t emergencyTmr = 0;
uint32_t timerPressStart = 0;
const int NIGHT_TOGGLE_HOLD_TIME = 5000;

// Таймер мигания
unsigned long blinkStart = 0;
const unsigned long BLINK_INTERVAL = 500;

typedef void (*Handler)(void);
Handler fsmTable[NUM_STATES][NUM_EVENTS];

// --- Логирование переходов ---
void logTransition(State oldState, State newState, const char* reason) {
  Serial.print("[");
  Serial.print(millis());
  Serial.print("мс] ");

  // Слева — текущее состояние
  switch (oldState) {
    case S_GREEN: Serial.print("ЗЕЛЁНЫЙ"); break;
    case S_YELLOW: Serial.print("ЖЁЛТЫЙ"); break;
    case S_RED: Serial.print("КРАСНЫЙ"); break;
    case S_WARNING: Serial.print("ВНИМАНИЕ"); break;
  }

  Serial.print(" -> ");

  // Справа — новое состояние
  switch (newState) {
    case S_GREEN: Serial.print("ЗЕЛЁНЫЙ"); break;
    case S_YELLOW: Serial.print("ЖЁЛТЫЙ"); break;
    case S_RED: Serial.print("КРАСНЫЙ"); break;
    case S_WARNING: Serial.print("ВНИМАНИЕ"); break;
  }

  Serial.print(" | Причина: ");
  Serial.println(reason);
}

// --- Управление светодиодами ---
void setOutputsForState(State s) {
  switch (s) {
    case S_GREEN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      break;
    case S_YELLOW:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case S_RED:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      break;
    case S_WARNING:
      // стартуем с выключенного жёлтого — мигание будет управляться в loop()
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      break;
  }
}

// --- Обработчики событий ---
void onGreenTimer() {
  State oldState = currentState;
  currentState = S_YELLOW;
  stateDur = 3000;
  stateStart = millis();
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Timer expired");
}

void onGreenPed() {
  State oldState = currentState;
  currentState = S_YELLOW;
  stateDur = 3000;
  stateStart = millis();
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Pedestrian request");
}

void onYellowTimer() {
  State oldState = currentState;
  currentState = S_RED;
  stateDur = pedRequest ? 15000 : 10000;
  stateStart = millis();
  setOutputsForState(currentState);
  if (pedRequest) {
    pedRequest = false;
    logTransition(oldState, currentState, "Истёк таймер + запрос пешехода");
  } else {
    logTransition(oldState, currentState, "Истёк таймер");
  }
}

void onRedTimer() {
  State oldState = currentState;
  currentState = S_GREEN;
  stateDur = 10000;
  stateStart = millis();
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Истёк таймер");
}

void onAnyEmergencyOn() {
  State oldState = currentState;
  currentState = S_WARNING;
  stateDur = 0xFFFFFFFFUL;
  stateStart = millis();
  blinkStart = millis();
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Аварийный режим ВКЛ");
}

void onEmergencyOff() {
  State oldState = currentState;
  currentState = S_RED;
  stateDur = 10000;
  stateStart = millis();
  blinkStart = 0;
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Аварийный режим ВЫКЛ");
}

void onNightToggle() {
  State oldState = currentState;
  currentState = S_WARNING;
  stateDur = 0xFFFFFFFFUL;
  stateStart = millis();
  blinkStart = millis();
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Ночной режим ВКЛ");
}

void onNightToggleOff() {
  State oldState = currentState;
  currentState = S_RED;
  stateDur = 10000;
  stateStart = millis();
  blinkStart = 0;
  setOutputsForState(currentState);
  logTransition(oldState, currentState, "Ночной режим ВЫКЛ");
}

// --- Таблица переходов ---
void setupTable() {
  for (int s=0; s<NUM_STATES; ++s)
    for (int e=0; e<NUM_EVENTS; ++e)
      fsmTable[s][e] = NULL;

  fsmTable[S_GREEN][E_TIMER] = onGreenTimer;
  fsmTable[S_YELLOW][E_TIMER] = onYellowTimer;
  fsmTable[S_RED][E_TIMER] = onRedTimer;

  for (int s=0; s<NUM_STATES; ++s) {
    fsmTable[s][E_EMERGENCY_ON] = onAnyEmergencyOn;
    fsmTable[s][E_EMERGENCY_OFF] = onEmergencyOff;
  }

  // Ночной режим
  for (int s=0; s<NUM_STATES; ++s)
    fsmTable[s][E_NIGHT_ON] = onNightToggle;

  fsmTable[S_WARNING][E_NIGHT_OFF] = onNightToggleOff;
}

// --- Чтение кнопок ---
void readInputs() {
  // Кнопка пешехода / ночной режим
  if (millis() - buttonTmr >= BTN_DEB) {
    buttonTmr = millis();

    static bool buttonPState = false;
    bool buttonState = !digitalRead(BUTTON_PIN);

    if (buttonPState != buttonState) {
      buttonPState = buttonState;

      if (buttonState) {
        if (timerPressStart == 0)
          timerPressStart = millis();
      } else {
        if (timerPressStart != 0) {
          uint32_t holdTime = millis() - timerPressStart;
          if (holdTime >= NIGHT_TOGGLE_HOLD_TIME) {
            night_mode_toggle = !night_mode_toggle;
            Serial.print("[");
            Serial.print(millis());
            Serial.print("мс] Ночной режим: ");
            Serial.println(night_mode_toggle ? "ВКЛ" : "ВЫКЛ");
          } else {
            // короткое нажатие: устанавливаем флаг pedRequest, не переключаем напрямую
            pedRequest = true;
            Serial.print("[");
            Serial.print(millis());
            Serial.println("мс] Кнопка пешехода нажата");
          }
          timerPressStart = 0;
        }
      }
    }
  }

  // Кнопка аварийного режима
  if (millis() - emergencyTmr >= BTN_DEB) {
    emergencyTmr = millis();

    static bool emergencyPState = false;
    bool emergencyState = !digitalRead(EMERGENCY_PIN);

    if (emergencyPState != emergencyState) {
      emergencyPState = emergencyState;

      if (emergencyState) {
        // переключаем флаг аварии
        emergency = !emergency;
        Serial.print("[");
        Serial.print(millis());
        Serial.print("мс] Авария: ");
        Serial.println(emergency ? "ВКЛ" : "ВЫКЛ");
      }
    }
  }
}

// --- Определение события ---
Event pollEvent() {
  bool prevEmergency = emergency; // состояние аварии до опроса входов
  readInputs();                    // может поменять emergency

  if (currentState != S_WARNING && millis() - stateStart >= stateDur) {
    return E_TIMER;
  }

  // Если короткое нажатие пришло во время RED — продлеваем RED один раз (и сбрасываем флаг)
  if (pedRequest && currentState == S_RED) {
    pedRequest = false;
    stateStart = millis();
    stateDur = 15000; // удлиняем RED до 15s с момента нажатия
    Serial.print("[");
    Serial.print(millis());
    Serial.println("мс] Запрос пешехода: увеличиваем КРАСНЫЙ до 15000мс");
    return E_NONE; // обработали, без изменения состояния
  }


  if (pedRequest && currentState == S_GREEN) {
  }

  if (emergency && currentState != S_WARNING) {
    return E_EMERGENCY_ON;
  }

  // Сначала - проверяем ночной режим (вход/выход)
  if (night_mode_toggle && currentState != S_WARNING) {
    return E_NIGHT_ON;
  }

  if (!night_mode_toggle && currentState == S_WARNING && !emergency) {
    return E_NIGHT_OFF;
  }

  // Если авария была включена и только что выключена — это E_EMERGENCY_OFF
  if (!emergency && prevEmergency && currentState == S_WARNING) {
    return E_EMERGENCY_OFF;
  }

  return E_NONE;
}

// --- Setup ---
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(EMERGENCY_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  setupTable();

  currentState = S_GREEN;
  stateDur = 10000;
  stateStart = millis();
  setOutputsForState(currentState);

  Serial.println("=== СВЕТОФОР ВКЛЮЧЕН ===");
  Serial.println("[0ms] Стартовое состояние: ЗЕЛЁНЫЙ");
}

// --- Loop ---
void loop() {
  Event ev = pollEvent();
  if (ev != E_NONE) {
    Handler h = fsmTable[currentState][ev];
    if (h) h();
  }

  // Мигание в ночном/аварийном режиме
  if (currentState == S_WARNING) {
    if (millis() - blinkStart >= BLINK_INTERVAL) {
      digitalWrite(YELLOW_PIN, !digitalRead(YELLOW_PIN));
      blinkStart = millis();
    }
  }

  delay(10);
}
