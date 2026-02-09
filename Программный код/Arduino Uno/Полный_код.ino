#include <Servo.h>

Servo servo;   //название серво

#define IN1 10   //пины моторов
#define IN2 9
#define IN3 8
#define IN4 7
#define ENA 11
#define ENB 6

#define PIN_ECHO 5   // Пин Echo ультразвукового датчика
#define PIN_TRIG 4   // Пин Trig ультразвукового датчика

long duration, cm; //переменные
int n = 1;   //шаг выполнения цикла
int t = 0;   //кол-во поворотов
int t1 = 0;
int q = 1;
String color;   //цвет
String data;
String mesto;   //положение на поле

int ex[] = {0, 1, 4, 5, 7, 8, 9, 12, 13, 16, 17, 18, 22, 23, 24, 28, 29, 30, -1, -4, -5, -7, -8, -9, -11, -12, -13, -16};
bool found = false;   // Флаг: нашли или нет

void vpered(int k){
  for (int i = 0; i < k; i++) {
    analogWrite(ENA, 50);
    analogWrite(ENB, 50);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(115);   //подпрограмма вперед
    stopm(); delay(300);
  }
}
void nazad(int k){
  for (int i = 0; i < k; i++) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 50);
    analogWrite(ENB, 50);
    delay(115);   //подпрограмма назад
    stopm(); delay(300);
  }
}
void left(int k){
  for (int i = 0; i < k; i++) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 50);
    analogWrite(ENB, 50);
    delay(35);   //подпрограмма влево
    stopm(); delay(300);
  }
}
void right(int k){
  for (int i = 0; i < k; i++) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 50);
    analogWrite(ENB, 50);
    delay(35);   //подпрограмма вправо
    stopm(); delay(300);
  }
}
void stopm(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);   //подпрограмма стоп
}

void setup() 
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);   //подключение моторов
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  servo.attach(A1);   //подключение серво
  pinMode(PIN_ECHO, INPUT); //подключение сонара
  pinMode(PIN_TRIG, OUTPUT);
  servo.write(90); delay(1000);   //захват закрыт
  stopm(); delay(40000);   //инициализация
  mesto = "в центре";
}


void loop() {
  //ищем объект
  if (n == 1 and t1 < 17){
    servo.write(0); delay(1000);   //захват открыт
    left(3); t1 = t1 + 1;   //сделал 1 поворот
    String color = readColor();
    if (color != "No"){   //если объект найден, то перемещаемся к нему
      n = 2;
    }
  }
  if (n == 2){
    t = t + t1;
    if (mesto == "в центре"){   //если робот стоит в центре поля
      for (int i = 0; i < 28; i++) {
        if (ex[i] == t) {
          found = true;
          break;   //прерываем цикл, если нашли совпадение
        }
      }

      if (found) {   //если перед роботом люк
        left(6);
        String color = readColor();
        if (color == "No"){   //нет препятствия
          vpered(18);
          right(12); t = t - 4;
          vpered(12);
          right(6); t = t - 2;
          vpered(2);
          q = 1;
        }
        else{   //есть препятствие
          right(12); t = t - 4;
          vpered(18);
          left(12); t = t + 4;
          vpered(12);
          left(6); t = t + 2;
          vpered(2);
          q = 2;
        }
        mesto = "за люком";   //новое положение робота
      }
      else {   //если перед роботом нет люка
        vpered(3);
      }
      n = 3;
    }
    if (mesto == "за люком" and n == 2){   //робот стоит за люком
      left(9); t = t + 3;
      vpered(6);
      right(12); t = t - 4;
      vpered(8);
      mesto = "между люками";   //новое положение робота
      n = 3;
    }
    if (mesto == "между люками" and n == 2){   //если робот стоит между люками
      left(6); t = t + 2;
      vpered(12);
      right(12); t = t - 4;
      vpered(2);
      mesto = "за люком";   //новое положение робота
      n = 3;
    }
  }//робот подъехал к объекту
  if (n == 3){ 
    String color = readColor();   //ещё раз считали цвет
    servo.write(90); delay(1000);   //захват закрыт
    if (mesto == "в центре"){   //если робот стоит в центре
      if (color == "white"){   //если объект белого цвета
        left(6); t = t + 2;
        vpered(3);
      }
      else{   //если объект не белого цвета
        right(6); t = t - 2;
        vpered(3);
      }
      n = 4;
    }
    if (mesto == "за люком" and n == 3){   //робот стоит за люком
      if (color == "white"){   //объект белого цвета
        vpered(2);
        right(12); t = t - 4;
        vpered(1);
      }
      else{   //объект не белого цвета
        if (q == 1){   //если к объекту подъезжали слева
          nazad(4);
          right(12); t = t - 4;
          vpered(9);
          right(6); t = t - 2;
          vpered(6);
        }
        else{    //если к объекту подъезжали справа
          nazad(4);
          left(12); t = t + 4;
          vpered(9);
          left(6); t = t + 2;
          vpered(6);
        }
      }
      n = 4;
      mesto = "в центре";   //новое положение робота
    }
    if (mesto == "между люками" and n == 3){
      left(6); t = t + 2;
      vpered(3);
      n = 4;
    }
  }
  if (n == 4){   //если нужно, то подъезжаем поближе к люку
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    duration = pulseIn(PIN_ECHO, HIGH);
    cm = (duration / 2) / 29.1;
    if(cm < 6){
      vpered(1);
    }
    else{
      n = 5;
    }
  }
  if (n == 5){
    servo.write(0); delay(1000);   //захват открыт
    if (mesto == "за люком"){   //если робот стоит за люком, то отезжаем назад чуть-чуть
      nazad(2);
    }
    else{   //если робот стоит не за люком, то отъезжает назад побольше
      nazad(4);
    }
    n = 1;   //обнуляем нужные переменные
    t1 = 0;
    q = 1;
    found = false;
  }
}


// Считывание цвета перед роботом
String readColor() {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('\n');
  }
  return data;
}
