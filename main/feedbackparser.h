#pragma once

#include <cstdint>
#include <functional>

#include <espchrono.h>

#include "bobbycar-serial.h"

namespace {
class FeedbackParser
{
public:
    FeedbackParser(const std::reference_wrapper<HardwareSerial> &serial, bool &feedbackValid, bobbycar::protocol::serial::Feedback &feedback) :
        m_serial{serial}, m_feedbackValid{feedbackValid}, m_feedback{feedback}
    {
    }

    void update()
    {
        using namespace bobbycar::protocol::serial;

        // Check for new data availability in the Serial buffer
        while (m_serial.get().available())
        {
            m_incomingByte    = m_serial.get().read();                                // Read the incoming byte
            m_bufStartFrame = ((uint16_t)(m_incomingBytePrev) << 8) + m_incomingByte; // Construct the start frame

            //Serial.printf("received: %x\r\n", m_incomingByte);

            // Copy received data
            if (m_bufStartFrame == Feedback::VALID_HEADER) {                     // Initialize if new data is detected
                m_p   = (uint8_t*)&m_newFeedback;
                *m_p++  = m_incomingBytePrev;
                *m_p++  = m_incomingByte;
                m_idx  = 2;
            } else if (m_idx >= 2 && m_idx < sizeof(m_newFeedback)) { // Save the new received data
                *m_p++  = m_incomingByte;
                m_idx++;
            }

            // Check if we reached the end of the package
            if (m_idx == sizeof(m_newFeedback)) {
                uint16_t checksum = calculateChecksum(m_newFeedback);

                // Check validity of the new data
                if (m_newFeedback.start == Feedback::VALID_HEADER && checksum == m_newFeedback.checksum)
                {
                    m_feedbackValid = true;
                    m_feedback = m_newFeedback;
                    m_lastFeedback = espchrono::millis_clock::now();
                }
//                else
//                {
//                    if (m_newFeedback.start == Feedback::VALID_HEADER)
//                        //Serial.println("header matched");
//                    else
//                        //Serial.println("header did not match");

//                    if (checksum == m_newFeedback.checksum)
//                        //Serial.println("checksum matched");
//                    else
//                        //Serial.println("checksum did not match");
//                }
                m_idx = 0; // Reset the index (it prevents to enter in this if condition in the next cycle)
            }

            // Update previous states
            m_incomingBytePrev  = m_incomingByte;
        }

        if (espchrono::millis_clock::now() - m_lastFeedback > 500)
            m_feedbackValid = false;
    }

private:
    uint8_t m_idx = 0;                        // Index for new data pointer
    uint16_t m_bufStartFrame{};               // Buffer Start Frame
    uint8_t *m_p{};                           // Pointer declaration for the new received data
    uint8_t m_incomingByte{};
    uint8_t m_incomingBytePrev{};

    espchrono::millis_clock::time_point m_lastFeedback{espchrono::millis_clock::now()};
    const std::reference_wrapper<HardwareSerial> &m_serial;
    bool &m_feedbackValid;
    bobbycar::protocol::serial::Feedback &m_feedback, m_newFeedback;
};
}
