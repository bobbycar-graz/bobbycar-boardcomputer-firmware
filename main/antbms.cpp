#include "antbms.h"

// esp-idf includes
#include <esp_log.h>

BmsInstruction::BmsInstruction(uint8_t b, uint8_t b2) :
        functionCode{b},
        length{b2}
{}

void BmsInstruction::setData(uint8_t *_data, uint8_t _length)
{
    std::copy(_data, _data + _length, this->data);
}

int BmsInstruction::getAddress() const
{
    return address;
}

void BmsInstruction::setAddress(int _address)
{
    address = _address;
}

uint8_t *BmsInstruction::getInstruction()
{
    if (length == 0)
    {
        return BmsBluetoothInst::buildReadBmsInst(functionCode, address, 0);
    }
    return BmsBluetoothInst::buildReadBmsInstWithData(this->functionCode, this->address, this->length, this->data);
}

std::string BmsInstruction::toString() const
{
    return "BmsInstruction{functionCode=" + std::to_string(functionCode) + ", address=" + std::to_string(address) + ", inst = " + ", data = " + "}";
}

int CRC16::calcCrc16(const uint8_t *data, uint16_t len)
{
    // calculate crc16
    uint16_t crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (uint16_t) data[pos];          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--)
        {    // Loop over each bit
            if ((crc & 0x0001) != 0)
            {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                            // Else LSB is not set
                crc >>= 1;                    // Just shift right
        }
    }

    return crc;
}

uint8_t *BmsBluetoothInst::buildReadBmsInst(uint8_t b, int i, uint8_t b2)
{
    auto *bArr = new uint8_t[64];
    bArr[0] = PROTOCOL_FRAME_HEAD;
    bArr[1] = PROTOCOL_ADD;
    bArr[2] = b;
    bArr[3] = (uint8_t) (i & 255);
    bArr[4] = (uint8_t) ((i >> 8) & 255);
    bArr[5] = b2;
    int crc16 = CRC16::calcCrc16(bArr + 1, 5);
    ESP_LOGI(TAG, "crc: %d", crc16);
    bArr[6] = (uint8_t) (crc16 >> 8);
    bArr[7] = (uint8_t) (crc16 & 255);
    bArr[8] = -86;
    bArr[9] = 85;
    return bArr;
}

uint8_t *BmsBluetoothInst::buildReadBmsInstWithData(uint8_t b, int i, uint8_t b2, uint8_t *bArr)
{
    auto *bArr2 = new uint8_t[64 + sizeof(bArr)];
    bArr2[0] = PROTOCOL_FRAME_HEAD;
    bArr2[1] = PROTOCOL_ADD;
    bArr2[2] = b;
    bArr2[3] = (uint8_t) (i & 255);
    bArr2[4] = (uint8_t) ((i >> 8) & 255);
    bArr2[5] = b2;
    int i2 = 5;
    for (int _i = 0; _i < sizeof(bArr); _i++)
    {
        i2++;
        bArr2[i2] = bArr[_i];
    }
    int i3 = i2 + 1;
    int crc16 = CRC16::calcCrc16(bArr2 + 1, (uint16_t) (sizeof(bArr) + 5));
    ESP_LOGI(TAG, "crc: %d", crc16);
    bArr2[i3] = (uint8_t) (crc16 >> 8);
    int i4 = i3 + 1;
    bArr2[i4] = (uint8_t) (crc16 & 255);
    int i5 = i4 + 1;
    bArr2[i5] = -86;
    int i6 = i5 + 1;
    bArr2[i6] = 85;
    return bArr2;
}
