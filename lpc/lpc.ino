#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <ArduinoJson.h>
IPAddress server_addr(127,0,0,1);
char user[] = "888";
char password[] = "8888";
char ssid[] = "xxxx"; 
char pass[] = "bugaosuni";
int switch_state = 1;
int water_state = 0;
WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;
char* itostr(char *str, int i)
{
    sprintf(str, "%d", i);
    return str;
}
char* dtostr(char *str, double d)
{
    sprintf(str, "%f", d);
    return str;
}
void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status()!= WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (conn.connect(server_addr, 3306, user, password))
    Serial.println("OK.");
  cursor = new MySQL_Cursor(&conn);
}

void loop()
{
  char json[100];
  int i = 0;
  while (!Serial.available()); 
  while (Serial.available()) 
  {
    json[i] = Serial.read();
    i++;
    delay(3);
  }
  Serial.println(json);
  char INSERT_SQL[200];
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  double water = doc["water"];
  int switch_state = doc["switch"];
  int state = doc["state"];
  String INSERT_SQL_Str = "INSERT INTO sensor.attrs(water,state,switch) VALUES (";
  INSERT_SQL_Str.concat(water);
  INSERT_SQL_Str.concat(',');
  INSERT_SQL_Str.concat(state);
  INSERT_SQL_Str.concat(',');
  INSERT_SQL_Str.concat(switch_state);
  INSERT_SQL_Str.concat(')');
  strcpy(INSERT_SQL,INSERT_SQL_Str.c_str());
  if (conn.connected())
    cursor->execute(INSERT_SQL);
}
