/*
	Автоматическое управление питанием тв приставки (приёмник цифрового тв).

	Трёхпозиционный тумблер задаёт режимы работы:
		1. Авто выключение - неактивно, авто включение - неактивно.
		   То есть ничего не делает.
		2. Авто выключение - активно, авто включение - неактивно.
		   Если приставка включена, а тв выключен, то запускается таймер, 
		   по достижении которого приставка выключается.
		3. Авто выключение - активно, авто включение - активно.
		   Выключение аналогично режиму 2.
		   Если приставка выключена, а тв включен, то
		   приставка включается.

	Author: Alexey Zaytsev (marfikus)
	Email: alex.rv9rh@gmail.com
*/


// Пины:
// сигнал состояния тв
const byte TV_POWER = 0;
// сигнал состояния приставки
const byte RECEIVER_POWER = 1;
// тумблер настройки авто выключения
const byte AUTO_OFF = 2;
// тумблер настройки авто включения
const byte AUTO_ON = 3;
// кнопка управления питанием приставки
const byte RECEIVER_POWER_BT = 4;

// интервал главного цикла в мс
const int MAIN_CYCLE_DELAY = 50;

// задержка при авто выключении в мс
const int AUTO_OFF_DELAY = 15000;

// задержка при авто включении в мс
const int AUTO_ON_DELAY = 2000;

long autoOffCounter = 0;
long autoOffMaxCounter = 0;

long autoOnCounter = 0;
long autoOnMaxCounter = 0;

void pushReceiverPowerButton() {
	digitalWrite(RECEIVER_POWER_BT, HIGH);
	delay(200);
	digitalWrite(RECEIVER_POWER_BT, LOW);
}


void setup() {
	pinMode(TV_POWER, INPUT);
	pinMode(RECEIVER_POWER, INPUT);
	pinMode(AUTO_OFF, INPUT);
	pinMode(AUTO_ON, INPUT);
	pinMode(RECEIVER_POWER_BT, OUTPUT);

	// перевод задержки авто выключения в количество итераций главного цикла
	autoOffMaxCounter = round(AUTO_OFF_DELAY / MAIN_CYCLE_DELAY);
	// коррекция погрешности таймера
	autoOffMaxCounter = autoOffMaxCounter - round(autoOffMaxCounter * 0.2);
	autoOffCounter = 0;

	// перевод задержки авто включения в количество итераций главного цикла
	autoOnMaxCounter = round(AUTO_ON_DELAY / MAIN_CYCLE_DELAY);
	// коррекция погрешности таймера
	autoOnMaxCounter = autoOnMaxCounter - round(autoOnMaxCounter * 0.2);
	autoOnCounter = 0;
}


void loop() {
	if (digitalRead(AUTO_OFF) == HIGH || digitalRead(AUTO_ON) == HIGH) {
		if (digitalRead(RECEIVER_POWER) == HIGH && digitalRead(TV_POWER) == LOW) {
			autoOffCounter++;

			if (autoOffCounter >= autoOffMaxCounter) {
				pushReceiverPowerButton();
				autoOffCounter = 0;
			}
		} else {
			autoOffCounter = 0;
		}
	}

	if (digitalRead(AUTO_ON) == HIGH) {
		if (digitalRead(RECEIVER_POWER) == LOW && digitalRead(TV_POWER) == HIGH) {
			autoOnCounter++;

			if (autoOnCounter >= autoOnMaxCounter) {
				pushReceiverPowerButton();
				autoOnCounter = 0;
			}
		} else {
			autoOnCounter = 0;
		}
	}

	delay(MAIN_CYCLE_DELAY);
}

