#include "RuuviParse.h"

uint16_t RuuviParse::getManufacturer(uint8_t* data) {
  return (data[0] << 8) | (data[1]);
}

int RuuviParse::getFormat(uint8_t* data) {
  return data[2];
}

float RuuviParse::getHumidity(uint8_t* data) {
  float humidity;

  switch(getFormat(data)) {
    case 3:
      humidity = data[3] * 0.5;
      break;
    case 5:
      humidity = ((data[3] & 0xFF) << 8 | data[4] & 0xFF) / 400;
      break;
    default:
      humidity = 0;
  }
  return humidity;
}

float RuuviParse::getTemperature(uint8_t* data) {
  float temp;
  uint8_t sign;
  
  //Return temperature in celsius
  switch(getFormat(data)) {
    case 3:
      temp = (data[4] & ~(1 << 7)) + (data[5]*0.01f);
      sign = (data[4] >> 7) & 1;
      break;
    case 5:
      temp = twosComplement((data[1] << 8) + data[2]) / 200;
      sign = 0;
      break;
    default:
      temp = 0;
      sign = 0;
  }
  
  if (sign == 0) {
    return temp;
  } else {
    return -1 * temp;
  }
}

int RuuviParse::getPressure(uint8_t* data) {
  int pres;

  switch(getFormat(data)) {
    case 3:
      pres = (data[6] << 8) + data[7] + 50000;
      break;
    case 5:
      pres = ((data[5] & 0xFF) << 8 | data[6] & 0xFF) + 50000;
      break;
    default:
      pres = 0;
  }
  
  return pres;
}

int16_t RuuviParse::getAccelerationX(uint8_t* data) {
  int16_t accX;
  uint8_t sign;

  switch(getFormat(data)) {
    case 3:
      accX = twosComplement((data[8] << 8) + data[9]);
      sign = (data[8] >> 7) & 1;
      break;
    case 5:
      accX = twosComplement((data[7] << 8) + data[8]);
      sign = 0;
      break;
    default:
      accX = 0;
      sign = 0;
  }
  
  if (sign == 0) {
    return accX;
  } else {
    return -1 * accX;
  }
}

int16_t RuuviParse::getAccelerationY(uint8_t* data) {
  int16_t accY;
  uint8_t sign;

  switch(getFormat(data)) {
    case 3:
      accY = twosComplement((data[10] << 8) + data[11]);
      sign = (data[10] >> 7) & 1;
      break;
    case 5:
      accY = twosComplement((data[9] << 8) + data[10]);
      sign = 0;
      break;
    default:
      accY = 0;
      sign = 0;
  }
  
  if (sign == 0) {
    return accY;
  } else {
    return -1 * accY;
  }
}

int16_t RuuviParse::getAccelerationZ(uint8_t* data) {
  int16_t accZ = twosComplement((data[12] << 8) + data[13]);
  uint8_t sign = (data[12] >> 7) & 1;

  switch(getFormat(data)) {
    case 3:
      accZ = twosComplement((data[12] << 8) + data[13]);
      sign = (data[12] >> 7) & 1;
      break;
    case 5:
      accZ = twosComplement((data[11] << 8) + data[12]);
      sign = 0;
      break;
    default:
      accZ = 0;
      sign = 0;
  }
  
  if (sign == 0) {
    return accZ;
  } else {
    return -1 * accZ;
  }
}

double RuuviParse::getBattery(uint8_t* data) {
  return (data[14] << 8) + data[15];
}

int16_t RuuviParse::twosComplement(int16_t value) {
  if ((value & (1 << (16 - 1))) != 0) {
    value = -value;
  }
  return value;
}
