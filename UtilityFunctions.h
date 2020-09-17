void prntln(String text) {
  unsigned long runTime = millis();
  Serial.print((runTime/3600000)/1000);
  Serial.print("h");
  Serial.print(((runTime%3600000)/60)/1000);
  Serial.print("m"); 
  Serial.print((runTime%60000));
  Serial.print("ms: "); 
  Serial.println(text);
}
void prnt(String text) {
  Serial.print(text);
}

