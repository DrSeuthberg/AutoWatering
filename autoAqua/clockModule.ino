
void initClock() 
{
  Wire.begin();
}

void showTimeAndTemp() {
  DateTime now = RTC.now();

  Serial.print(now.day(), DEC);
  Serial.print('.');
  Serial.print(now.month(), DEC);
  Serial.print('.');
  Serial.print(now.year(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" - ");
  Serial.print(tempDS3231());
  Serial.print("°C");
  Serial.println();
}



float tempDS3231() {
  // Die Temperatur des DS3231 auslesen  
  float temp;
  int msb, lsb;
  Wire.beginTransmission(DS3231_ADDRESSE);
  Wire.write(0x11); // DS3231 Register zu 11h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_ADDRESSE, 2); // 2 Byte Daten vom DS3231 holen
  msb = Wire.read();
  lsb = Wire.read();
  temp=((msb << 2) + (lsb >> 6) ) /4.0;
  return temp;
}
