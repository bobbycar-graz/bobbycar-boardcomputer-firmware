#pragma once

// system includes
#include <cstdint>
#include <string>

class CRC16
{
public:
    static int calcCrc16(const uint8_t *data, uint16_t len);
};

class BmsBluetoothInst
{
public:
    static constexpr const char * const TAG = "BMSBluetoothInst";

    static uint8_t *buildReadBmsInst(uint8_t b, int i, uint8_t b2);

    static uint8_t *buildReadBmsInstWithData(uint8_t b, int i, uint8_t b2, uint8_t *bArr);

    static constexpr uint8_t PROTOCOL_ADD = 0xA1;
    static constexpr uint8_t PROTOCOL_FRAME_HEAD = 0x7E;
};

class BmsInstruction
{
private:
    const uint8_t functionCode;
    const uint8_t length;

    int address{0};

    uint8_t data[32]{};
public:
    static constexpr const char * const TAG = "BMSInstruction";

    BmsInstruction(uint8_t b, uint8_t b2);

    void setData(uint8_t *data, uint8_t length);

    [[nodiscard]] uint8_t getLength() const;

    [[nodiscard]] uint8_t getFunctionCode() const;

    [[nodiscard]] int getAddress() const;

    void setAddress(int address);

    uint8_t* getInstruction();

    [[nodiscard]] std::string toString() const;
};
