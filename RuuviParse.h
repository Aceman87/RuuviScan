#include <Arduino.h>

class RuuviParse {
  public:
    uint16_t getManufacturer(uint8_t* data);
    int getFormat(uint8_t* data);
    float getHumidity(uint8_t* data);
    float getTemperature(uint8_t* data);
    int getPressure(uint8_t* data);
    int16_t getAccelerationX(uint8_t* data);
    int16_t getAccelerationY(uint8_t* data);
    int16_t getAccelerationZ(uint8_t* data);
    double getBattery(uint8_t* data);
  private:
    int16_t twosComplement(int16_t value);
};
