#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <driver/i2s.h>
#include "chime.h"  //wav file

/* LED */
#define LED_COLOR_INACTIVE  0xF00000
#define LED_COLOR_ACTIVE    0x00F000

/* BLE */
BLEScan* pBLEScan;
#define TARGET_DEVNAME  "M5Atom-chime"
#define SCAN_TIME 1 //In seconds
#define DELAY_AFTERCHIME 2000 //In miliseconds

/* I2S */
#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define CONFIG_I2S_DATA_IN_PIN  23
#define SPEAKER_I2S_NUMBER      I2S_NUM_0
#define MODE_MIC                0
#define MODE_SPK                1

bool scanChimeDevice()
{
  bool found = false;
  int devcount;

  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
  devcount = foundDevices.getCount();
  for(int i=0; i<devcount; i++)
  {
      BLEAdvertisedDevice d = foundDevices.getDevice(i);
      if(d.haveName())
      {
        if(d.getName() == TARGET_DEVNAME)
        {
          found = true;
          break;
        }
      }
  }

  // delete results fromBLEScan buffer to release memory
  pBLEScan->clearResults();

  return found;
}

void InitI2SSpeakerOrMic(int mode)
{
  esp_err_t err = ESP_OK;
 
  i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER),
    .sample_rate          = 16000,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format       = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 6,
    .dma_buf_len          = 60,
    .use_apll             = false,
    .tx_desc_auto_clear   = true,
    .fixed_mclk           = 0
  };
  if (mode == MODE_MIC)
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
  }
  else
  {
    i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
  }
 
  err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);
 
  i2s_pin_config_t tx_pin_config = {
    .bck_io_num           = CONFIG_I2S_BCK_PIN,
    .ws_io_num            = CONFIG_I2S_LRCK_PIN,
    .data_out_num         = CONFIG_I2S_DATA_PIN,
    .data_in_num          = CONFIG_I2S_DATA_IN_PIN,
  };
  err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);
 
  if (mode != MODE_MIC) {
    err += i2s_set_clk(SPEAKER_I2S_NUMBER, 16000, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
  }
 
  i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);
}

void setup() {
  M5.begin(true, false, true);
  delay(50);
  M5.dis.drawpix(0,LED_COLOR_INACTIVE);

  //Speaker,Mic Initialize(Mic Mode)
  InitI2SSpeakerOrMic(MODE_MIC);

  //Create BLE Device
  BLEDevice::init("");

  //BLE Scan Initialize
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(false); //Passive Scan
  
  delay(100);
}

void loop() {
  if(scanChimeDevice())
  {
    size_t bytes_written;
 
    M5.dis.drawpix(0,LED_COLOR_ACTIVE);
    
    //Set Speaker Mode
    InitI2SSpeakerOrMic(MODE_SPK);  
    //Write Speaker
    i2s_write(SPEAKER_I2S_NUMBER, chime_wav, sizeof(chime_wav), &bytes_written, portMAX_DELAY);
    i2s_zero_dma_buffer(SPEAKER_I2S_NUMBER);
    //Set Mic Mode
    InitI2SSpeakerOrMic(MODE_MIC);
    
    delay(DELAY_AFTERCHIME);
    M5.dis.drawpix(0,LED_COLOR_INACTIVE);
  }

  delay(100);
  M5.update();
}
