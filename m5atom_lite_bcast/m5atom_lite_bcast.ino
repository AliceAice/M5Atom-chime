#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEUtils.h>

/* LED */
#define LED_COLOR_INACTIVE  0xF00000
#define LED_COLOR_ACTIVE    0x00F000

/* BLE */
BLEAdvertising *pAdv;
#define DEVICE_NAME "M5Atom-chime"
#define ADVERTISING_TIME 3000 //In miliseconds

void setAdvertisementData(BLEAdvertising *pAdvertising) {
  std::string strData = "";
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();

  //Device Name
  oAdvertisementData.setName(DEVICE_NAME);
 
  //Flags
  oAdvertisementData.setFlags(0x01|0x04); // LE Limited Discoverable Mode | BR_EDR_NOT_SUPPORTED

  //Data
  strData += (char)0xff;                      //Type :Manufacture Specific
  strData += (char)0xff;                      //Company ID(L) 
  strData += (char)0xff;                      //Company ID(H) :for Test(0xffff)
  strData += (char)0xff;                      //dmy Data
  strData = (char)strData.length() + strData; //Length
  oAdvertisementData.addData(strData);
  pAdvertising->setAdvertisementData(oAdvertisementData);

  //AdvertisementType
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND);
}

void setup() {
  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0,LED_COLOR_INACTIVE);
  
   //Create BLE Device
  BLEDevice::init("");
    
  //Set AdvertisementData
  pAdv = BLEDevice::getAdvertising();
  setAdvertisementData(pAdv); 
  
  delay(100);
}

void loop() {
  if (M5.Btn.isPressed())
  {
    M5.dis.drawpix(0,LED_COLOR_ACTIVE);
    pAdv->start();  //Start advertising
    delay(ADVERTISING_TIME);
    pAdv->stop();   //Stop advertising
    M5.dis.drawpix(0,LED_COLOR_INACTIVE);
  }
  
  delay(100);
  M5.update();
}
