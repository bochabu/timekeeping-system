#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SS_PIN 2
#define RST_PIN 15
#define TERMINAL_NAME  "headquarter"
#define ROW_NUM 4
#define COLUMN_NUM 3
#define BUTTON_PIN 5 

Servo myServo;
MFRC522 mfrc522(SS_PIN, RST_PIN);

int servoPin = 4;
int buzzerPin = 35;
int ledPin = 34;

LiquidCrystal_I2C lcd(0x3F, 16, 2);

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {13, 12, 14, 27};
byte pin_column[COLUMN_NUM] = {26, 25, 32};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String currentPassword = "";  
String enteredPassword = "";  
bool passwordMode = false;  

const char *mainLinkForSpr = "https://script.google.com/macros/s/AKfycbwm8hUJ3J0VORKQNHwaBKCS5VuVS0s_QgGCq1Oqf61e10JQQl_v0IseIlcOCPWcyVCm4w/exec";
const char *ssid = "Redmi"; 
const char *password = "bochabu34"; 
const char *root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n" \
"CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
"MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n" \
"MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
"Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n" \
"A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n" \
"27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n" \
"Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n" \
"TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n" \
"qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n" \
"szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n" \
"Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n" \
"MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n" \
"wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n" \
"aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n" \
"VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n" \
"AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n" \
"FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n" \
"C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n" \
"QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n" \
"h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n" \
"7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n" \
"ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n" \
"MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n" \
"Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n" \
"6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n" \
"0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n" \
"2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n" \
"bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n" \
"-----END CERTIFICATE-----\n";

void setup() 
{
    lcd.init(); 
    lcd.backlight();

    SPI.begin(); 
    mfrc522.PCD_Init();

    myServo.setPeriodHertz(50); 
    myServo.attach(servoPin, 500, 2400); 
    myServo.write(55);

    display("Welcome", "");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        lcd.print(".");
    }
    display("WiFi connected", "");
    delay(2000); 
    display("Ready To Scan", "");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() 
{
    if (checkRFIDCard()) {
        delay(1000); 
        mfrc522.PICC_HaltA(); 
        mfrc522.PCD_StopCrypto1(); 
    }

    checkKeypad();
    checkButton(); 
}

void checkKeypad()
{
    char key = keypad.getKey();

    if (key) {
        if (key == '*') {
            passwordMode = !passwordMode;
            if (passwordMode) {
                enteredPassword = "";
                display("Enter password:", "");
            } else {
                display("Ready To Scan", "");
            }
        } else if (key == '#') {
            if (passwordMode) {
                if (enteredPassword == currentPassword) {
                    display("", "Correct password");
                    opendoor();
                } else {
                    display("", "Wrong password");
                    delay(2000);
                }
                enteredPassword = ""; 
                display("Ready To Scan", "");
                passwordMode = false;
            }
        } else {
            if (passwordMode) {
                enteredPassword += key;  
                lcd.setCursor(enteredPassword.length() - 1, 1);
                lcd.print("*");
            }
        }
    }
}

void checkButton()
{
    if (digitalRead(BUTTON_PIN) == LOW) {
        updateCurrentPassword();
        delay(200); 
    }
}


void updateCurrentPassword()
{
    display("Updating", "password...");
    
    String response = fetchPassword();
    int colonIndex = response.lastIndexOf(":");
    currentPassword = response.substring(colonIndex + 1);
    display("Password updated", "");
    delay(2000);
    display("Ready To Scan", "");
}

String fetchPassword() {
    HTTPClient http;
    String data;

    char url[150];
    sprintf(url, "%s?terminal=%s&action=fetchPassword", mainLinkForSpr, TERMINAL_NAME);

    http.begin(url, root_ca);
    int code = http.GET();

    if (code == 302) {
        String newUrl = http.getLocation();
        http.end();
        http.begin(newUrl, root_ca);
        code = http.GET();
    }

    if (code == 200) {
        data = http.getString();
    } else {
        display("Error:", String(code).c_str());
        delay(2000); 
    }

    http.end();
    return data;
}


bool checkRFIDCard() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return false;
    }

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    
    display("Connecting to", "Google...");

    String response = getGoogleData(uid);
    handleDataFromGoogle(response);
    return true;
}

void handleDataFromGoogle(String data) {
    int colonIndex = data.indexOf(":");
    String access = data.substring(0, colonIndex);
    int nextColonIndex = data.indexOf(":", colonIndex + 1);
    String name = data.substring(colonIndex + 1, nextColonIndex);
    colonIndex = nextColonIndex;
    nextColonIndex = data.indexOf(":", colonIndex + 1);
    String text = data.substring(colonIndex + 1, nextColonIndex);
    currentPassword = data.substring(nextColonIndex + 1);

    display("Hi " + name, text);
    delay(2000); 
    display("Ready To Scan", "");

    if (access.equalsIgnoreCase("allow")) {
        opendoor();
    }
}

String getGoogleData(String uid) {
    HTTPClient http;
    String data;

    char url[150];
    sprintf(url, "%s?uid=%s&terminal=%s", mainLinkForSpr, uid.c_str(), TERMINAL_NAME);

    http.begin(url, root_ca);
    int code = http.GET();

    if (code == 302) {
        String newUrl = http.getLocation();
        http.end();
        http.begin(newUrl, root_ca);
        code = http.GET();
    }

    if (code == 200) {
        data = http.getString();
    } else {
        display("Error:", String(code).c_str());
        delay(2000); 
    }

    http.end();
    return data;
}

void opendoor()
{
    myServo.write(150);
    delay(3000);
    myServo.write(55);
}

void display(String line1, String line2)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
