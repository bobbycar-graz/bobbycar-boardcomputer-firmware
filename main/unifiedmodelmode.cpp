#include "unifiedmodelmode.h"

std::pair<bobbycar::protocol::ControlType, bobbycar::protocol::ControlMode> split(UnifiedModelMode mode)
{
  using bobbycar::protocol::ControlMode;
  using bobbycar::protocol::ControlType;

  switch (mode)
  {
  case UnifiedModelMode::Commutation: return std::make_pair(ControlType::Commutation, ControlMode::Voltage);
  case UnifiedModelMode::Sinusoidal: return std::make_pair(ControlType::Sinusoidal, ControlMode::Voltage);
  case UnifiedModelMode::FocVoltage: return std::make_pair(ControlType::FieldOrientedControl, ControlMode::Voltage);
  case UnifiedModelMode::FocSpeed: return std::make_pair(ControlType::FieldOrientedControl, ControlMode::Speed);
  case UnifiedModelMode::FocTorque: return std::make_pair(ControlType::FieldOrientedControl, ControlMode::Torque);
  }

  // Serial.printf("Unknown UnifiedModelMode: %i\r\n", int(mode));

  return std::make_pair(ControlType::FieldOrientedControl, ControlMode::OpenMode);
}
