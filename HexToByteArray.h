#include <Arduino.h>

// convert a hex string such as "A489B1" into an array like [0xA4, 0x89, 0xB1].
class HexToByteArray {
  public:
    void hexCharacterStringToBytes(byte *byteArray, const char *hexString);
    void dumpByteArray(const byte * byteArray, const byte arraySize);
    byte nibble(char c);
};
