
#include <NewPing.h>

#define TRIGGER_PIN  5
#define ECHO_PIN     6
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);
  }

void loop() {
  Serial.print("Ping: ");
  unsigned int us=sonar.ping_median();     //傳回 5 次偵測的回應時間中數 (微秒)
  Serial.print(sonar.convert_cm(us));         //將回應時間 (微秒) 轉成距離 (公分)
  Serial.println("cm");
  Serial.println(us);
  delay(500);
  }
