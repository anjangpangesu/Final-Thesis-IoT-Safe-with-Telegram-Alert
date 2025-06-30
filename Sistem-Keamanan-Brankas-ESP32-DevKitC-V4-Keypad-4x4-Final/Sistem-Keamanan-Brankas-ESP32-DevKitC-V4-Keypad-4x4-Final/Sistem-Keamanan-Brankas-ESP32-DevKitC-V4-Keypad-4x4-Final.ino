/*
  Program     : Sistem Keamanan Brankas ESP32-DevKitC-V4 Keypad 4x4
  Dibuat Oleh : Anjang Pangestu S
*/

#include <LiquidCrystal_I2C.h> //Library Untuk LCD & I2C
#include <Keypad.h> //Library Untuk Keypad 4x4
#include "CTBot.h" //Library Untuk Bot Telegram
CTBot myBot; //Inisialisasi Bot Telegram

// ===========================
// Penginputan Wifi & Password
// ===========================
const char* ssid = "My home"; //Nama Wifi
const char* pass = "Pangestu23"; //Password Wifi

// ==============================
// Penginputan Token Bot Telegram
// ==============================
String token = "6866213741:AAHb9480xjoVWuC12ZJhktKj3Fzs6spFa44"; //Token Bot Telegram

//Deklarasi PIN
#define Buzzer 15 //Koneksi ke Buzzer Melalui GPIO15
#define Relay 12  //Koneksi ke Relay Melalui GPIO12
#define ROWS  4   //Baris Keypad 4x4
#define COLS  4   //Kolom Keypad 4x4

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {19, 18, 5, 17}; //Pinout Baris Pada Keypad
byte colPins[COLS] = {16, 4, 0, 2}; //Pinout Kolom Pada Keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //Inisialisasi Keypad
String reply; //Inisialisasi Reply Pesan Bot Telegram

const String password_1 = "123690"; //Password Pertama
const String password_2 = "234567"; //Password Kedua
const String password_3 = "098765"; //Password Ketiga
String input_password;
int posisi=0;

LiquidCrystal_I2C lcd(0x27, 16, 2); //Inisialisasi LCD & I2C

void setup() {
  Serial.begin(115200);
	myBot.wifiConnect(ssid, pass);
	myBot.setTelegramToken(token);

  //Cek Koneksi Internet
	if (myBot.testConnection())
		Serial.println("\ntestConnection OK");
	else
		Serial.println("\ntestConnection NOK");

  input_password.reserve(32); //Maksimal Penginputan Karakter

  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);

  //Tampilan Teks Yang Display DI LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Brankas IOT   ");
  lcd.setCursor(0,1);
  lcd.print(" Pass || Kamera ");
  delay(2000);
  lcd.clear();
}

void loop() {
  TBMessage msg;

  lcd.setCursor(0,0);
  lcd.print("Input Password :");

  char key = customKeypad.getKey();
    if (key) {
      Serial.println(key);
      if (key == '*') {
        input_password = ""; //Reset Password
        lcd.clear();
      } else if (key == '#') {
        lcd.clear();
          if (input_password == password_1 || input_password == password_2 || input_password == password_3) {
            Serial.println("Password Benar => Brankas Terbuka");
            reply="Password Benar, Brankas Terbuka";
            myBot.sendMessage(1100606131,reply); //
            lcd.setCursor(0, 0);
            lcd.print(" Password Benar ");
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            delay(100);
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            delay(100);
            digitalWrite(Buzzer, HIGH);
            delay(100);
            digitalWrite(Buzzer, LOW);
            delay(3000);
            lcd.setCursor(0, 1);
            lcd.print("Brankas Terbuka");
            digitalWrite(Relay, HIGH);  // unlock the door for 20 seconds
            delay(12000);
            lcd.setCursor(0,1);
            lcd.print("Brankas Ditutup!");
            Serial.println("Waktu Habis!, Brankas Ditutup");
            reply="Waktu Habis!, Brankas Ditutup!";
            myBot.sendMessage(1100606131,reply); //
            delay(2000);
            digitalWrite(Relay, LOW); // lock the door
            delay(1000);
            lcd.clear();
          } else {
            digitalWrite(Buzzer, HIGH);
            reply="Password Salah, Silahkan Masukan Pin Ulang";
            myBot.sendMessage(1100606131,reply); //1100606131
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Password Salah!");
            delay(3000);
            delay(10);
            digitalWrite(Buzzer, LOW);
            lcd.setCursor(0, 1);
            lcd.print("Ulangi Kembali!");
            Serial.println("Password Salah => Silahkan Ulangi!");
            delay(3000);
            lcd.clear();
            posisi=0;
          }
          input_password = ""; //Reset Password
          posisi=0;
          } else {
            digitalWrite(Buzzer, HIGH);
            delay(20);
            digitalWrite(Buzzer, LOW);
            lcd.setCursor(posisi, 1); //Pindah ke posisi baru
            lcd.print('*'); //Pin Password Dicetak Menjadi (*) Untuk Menyembunyikan Password
            input_password += key; //Menambahkan Karakter Baru ke String input_password
          }
    posisi+=1;
	}
}