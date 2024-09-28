#define RDM6300 Serial

int statusRFID = 0; 
unsigned int readData[10];
int counter = -1; 
char tagId[11];
void setup() {                
  RDM6300.begin(9600);
  delay(2000);
  Serial.println("Dekatkan RFID Tag 125Khz ke RFID Reader");
  Serial.println();
}
void parseTag() {
  int i;
  for (i = 0; i < 10; ++i) {
    tagId[i] = readData[i];
  }
  tagId[10] = 0;
}
void clearSerial() {
  while (RDM6300.read() >= 0) {
    ; 
  }
}
void loop() {
  if (RDM6300.available() > 0) {
    statusRFID = RDM6300.read();
    if (statusRFID == 2) {
      counter = 0;
    } 
    else if (statusRFID == 3) {
      parseTag();
      
      RDM6300.print("Tag RFID: ");
      RDM6300.println(tagId);      
      delay(1000);
      
      clearSerial();
      counter = -1;
    }
    else if (counter >= 0) {
      readData[counter] = statusRFID;
      ++counter;
    } 
  }
}
