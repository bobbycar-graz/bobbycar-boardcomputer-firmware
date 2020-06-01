/*
Copyright 2015 - 2017 Andreas Chaitidis Andreas.Chaitidis@gmail.com

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "VescUart.h"
#include "buffer.h"
#include "crc.h"
#include "datatypes.h"
//#include <SoftwareSerial.h>


static SerialType* serialPort1;
static SerialType* serialPort2;
static SerialType* serialPort3;
static SerialType* serialPort4;
static SerialType* debugSerialPort = NULL;

bool UnpackPayload(uint8_t* message, int lenMes, uint8_t* payload, int lenPa);
bool ProcessReadPacket(uint8_t* message, struct bldcMeasure& values, int len);

void SetSerialPort(SerialType*  _serialPort1, SerialType*  _serialPort2, SerialType*  _serialPort3, SerialType*  _serialPort4)
{
	serialPort1 = _serialPort1;
	serialPort2 = _serialPort2;
	serialPort3 = _serialPort3;
	serialPort4 = _serialPort4;
}

void SetSerialPort(SerialType* _serialPort)
{
	SetSerialPort(_serialPort, _serialPort, _serialPort, _serialPort);
}

void SetDebugSerialPort(SerialType * _debugSerialPort)
{
	debugSerialPort = _debugSerialPort;
}


//SerialType *serial; ///@param num as integer with the serial port in use (0=Serial; 1=Serial1; 2=Serial2; 3=Serial3;)
int ReceiveUartMessage(uint8_t* payloadReceived, int num) {

	//Messages <= 255 start with 2. 2nd byte is length
	//Messages >255 start with 3. 2nd and 3rd byte is length combined with 1st >>8 and then &0xFF

	int counter = 0;
	int endMessage = 256;
	bool messageRead = false;
	uint8_t messageReceived[256];
	int lenPayload = 0;
    SerialType* serial = nullptr;


	switch (num) {
		case 0:
			serial = serialPort1;
			break;
		case 1:
			serial = serialPort2;
			break;
		case 2:
			serial = serialPort3;
			break;
		case 3:
			serial = serialPort4;
			break;
		default:
			break;

	}

	int mycnt=0;
	if(!serial->available())delay(10);
	while (serial->available()) {
		mycnt++;

		messageReceived[counter++] = serial->read();

		if (counter == 2) {//case if state of 'counter' with last read 1

			switch (messageReceived[0])
			{
			case 2:
				endMessage = messageReceived[1] + 5; //Payload size + 2 for sice + 3 for SRC and End.
				lenPayload = messageReceived[1];
				break;
			case 3:
				//ToDo: Add Message Handling > 255 (starting with 3)
				break;
			default:
				break;
			}

		}
		if (counter >= sizeof(messageReceived))
		{
			break;
		}

		if (counter == endMessage && messageReceived[endMessage - 1] == 3) {//+1: Because of counter++ state of 'counter' with last read = "endMessage"
			messageReceived[endMessage] = 0;
			if (debugSerialPort != NULL) {
				debugSerialPort->println("End of message reached!");
			}
			messageRead = true;
			break; //Exit if end of message is reached, even if there is still more data in buffer.
		}
		if(!serial->available())delay(50);
	}

	bool unpacked = false;
	if (messageRead) {
		unpacked = UnpackPayload(messageReceived, endMessage, payloadReceived, messageReceived[1]);
	}
	if (unpacked)
	{
		//Serial.println(String("")+"payload :"+lenPayload);
		return lenPayload; //Message was read

	}
	else {
		//Serial.println("not unpacked :(");
		return 0; //No Message Read
	}
}

bool UnpackPayload(uint8_t* message, int lenMes, uint8_t* payload, int lenPay) {
	uint16_t crcMessage = 0;
	uint16_t crcPayload = 0;
	//Rebuild src:
	crcMessage = message[lenMes - 3] << 8;
	crcMessage &= 0xFF00;
	crcMessage += message[lenMes - 2];
if(debugSerialPort!=NULL){
	debugSerialPort->print("SRC received: "); debugSerialPort->println(crcMessage);
} // DEBUG

	//Extract payload:
	memcpy(payload, &message[2], message[1]);

	crcPayload = crc16(payload, message[1]);
if(debugSerialPort!=NULL){
	debugSerialPort->print("SRC calc: "); debugSerialPort->println(crcPayload);
}
	if (crcPayload == crcMessage)
	{
if(debugSerialPort!=NULL){
		debugSerialPort->print("Received: "); SerialPrint(message, lenMes); debugSerialPort->println();
		debugSerialPort->print("Payload :      "); SerialPrint(payload, message[1] - 1); debugSerialPort->println();
} // DEBUG

		return true;
	}
	else
	{
		return false;
	}
}




int PackSendPayload(uint8_t* payload, int lenPay, int num) {
	uint16_t crcPayload = crc16(payload, lenPay);
	int count = 0;
	uint8_t messageSend[256];

	if (lenPay <= 256)
	{
		messageSend[count++] = 2;
		messageSend[count++] = lenPay;
	}
	else
	{
		messageSend[count++] = 3;
		messageSend[count++] = (uint8_t)(lenPay >> 8);
		messageSend[count++] = (uint8_t)(lenPay & 0xFF);
	}
	memcpy(&messageSend[count], payload, lenPay);

	count += lenPay;
	messageSend[count++] = (uint8_t)(crcPayload >> 8);
	messageSend[count++] = (uint8_t)(crcPayload & 0xFF);
	messageSend[count++] = 3;
    messageSend[count] = 0;

if(debugSerialPort!=NULL){
	debugSerialPort->print("UART package send: "); SerialPrint(messageSend, count);

} // DEBUG


    SerialType *serial = nullptr;

	switch (num) {
		case 0:
			serial=serialPort1;
			break;
		case 1:
			serial=serialPort2;
			break;
		case 2:
			serial=serialPort3;
			break;
		case 3:
			serial=serialPort4;
			break;
		default:
			break;
	}

	//Sending package
	serial->write(messageSend, count);


	//Returns number of send bytes
	return count;
}


bool ProcessReadPacket(uint8_t* message, struct bldcMeasure& values, int len) {
	COMM_PACKET_ID packetId;
	int32_t ind = 0;

	packetId = (COMM_PACKET_ID)message[0];
	message++;//Eliminates the message id
	len--;

	switch (packetId)
	{

	case COMM_GET_VALUES:
		values.tempFetFiltered		= buffer_get_float16(message, 1e1, &ind);
		values.tempMotorFiltered	= buffer_get_float16(message, 1e1, &ind);
		values.avgMotorCurrent		= buffer_get_float32(message, 100.0, &ind);
		values.avgInputCurrent		= buffer_get_float32(message, 100.0, &ind);
		values.avgId				= buffer_get_float32(message, 1e2, &ind);
		values.avgIq				= buffer_get_float32(message, 1e2, &ind);
		values.dutyNow				= buffer_get_float16(message, 1000.0, &ind);
		values.rpm					= buffer_get_float32(message, 1.0, &ind);
		values.inpVoltage			= buffer_get_float16(message, 10.0, &ind);
		values.ampHours				= buffer_get_float32(message, 10000.0, &ind);
		values.ampHoursCharged		= buffer_get_float32(message, 10000.0, &ind);
		values.wattHours			= buffer_get_float32(message, 1e4, &ind);
		values.watthoursCharged		= buffer_get_float32(message, 1e4, &ind);
		values.tachometer			= buffer_get_int32(message, &ind);
		values.tachometerAbs		= buffer_get_int32(message, &ind);
		values.faultCode			= message[ind];
		return true;
		break;
	default:
		return false;
		break;
	}

}
/*
bool ProcessReadPacketMC(uint8_t* message, struct mc_configuration& values, int len) {
	COMM_PACKET_ID packetId;
	int32_t ind = 0;

	packetId = (COMM_PACKET_ID)message[0];
	message++;//Eliminates the message id
	len--;

	switch (packetId)
	{

	case COMM_GET_MCCONF:


		mc_pwm_mode pwm_mode;
		mc_comm_mode comm_mode;
		mc_motor_type motor_type;
		mc_sensor_mode sensor_mode;
		float l_current_max;
		float l_current_min;
		float l_in_current_max;
		float l_in_current_min;
		float l_abs_current_max;
		float l_min_erpm;
		float l_max_erpm;
		float l_max_erpm_fbrake;
		float l_max_erpm_fbrake_cc;
		float l_min_vin;
		float l_max_vin;
		float l_battery_cut_start;
		float l_battery_cut_end;
		bool l_slow_abs_current;
		bool l_rpm_lim_neg_torque;
		float l_temp_fet_start;
		float l_temp_fet_end;
		float l_temp_motor_start;
		float l_temp_motor_end;
		float l_min_duty;
		float l_max_duty;
		float lo_current_max;
		float lo_current_min;
		float lo_in_current_max;
		float lo_in_current_min;
		float sl_min_erpm;
		float sl_min_erpm_cycle_int_limit;
		float sl_max_fullbreak_current_dir_change;
		float sl_cycle_int_limit;
		float sl_phase_advance_at_br;
		float sl_cycle_int_rpm_br;
		float sl_bemf_coupling_k;
		int8_t hall_table[8];
		float hall_sl_erpm;
		float foc_current_kp;
		float foc_current_ki;
		float foc_f_sw;
		float foc_dt_us;
		float foc_encoder_offset;
		bool foc_encoder_inverted;
		float foc_encoder_ratio;
		float foc_motor_l;
		float foc_motor_r;
		float foc_motor_flux_linkage;
		float foc_observer_gain;
		float foc_pll_kp;
		float foc_pll_ki;
		float foc_duty_dowmramp_kp;
		float foc_duty_dowmramp_ki;
		float foc_openloop_rpm;
		float foc_sl_openloop_hyst;
		float foc_sl_openloop_time;
		float foc_sl_d_current_duty;
		float foc_sl_d_current_factor;
		mc_foc_sensor_mode foc_sensor_mode;
		uint8_t foc_hall_table[8];
		float foc_sl_erpm;
		float s_pid_kp;
		float s_pid_ki;
		float s_pid_kd;
		float s_pid_min_erpm;
		float p_pid_kp;
		float p_pid_ki;
		float p_pid_kd;
		float p_pid_ang_div;
		float cc_startup_boost_duty;
		float cc_min_current;
		float cc_gain;
		float cc_ramp_step_max;
		int32_t m_fault_stop_time_ms;
		float m_duty_ramp_step;
		float m_duty_ramp_step_rpm_lim;
		float m_current_backoff_gain;
		uint32_t m_encoder_counts;
		sensor_port_mode m_sensor_port_mode;

		values.tempFetFiltered		= buffer_get_float16(message, 1e1, &ind);
		values.tempMotorFiltered	= buffer_get_float16(message, 1e1, &ind);
		values.avgMotorCurrent		= buffer_get_float32(message, 100.0, &ind);
		values.avgInputCurrent		= buffer_get_float32(message, 100.0, &ind);
		values.avgId				= buffer_get_float32(message, 1e2, &ind);
		values.avgIq				= buffer_get_float32(message, 1e2, &ind);
		values.dutyNow				= buffer_get_float16(message, 1000.0, &ind);
		values.rpm					= buffer_get_float32(message, 1.0, &ind);
		values.inpVoltage			= buffer_get_float16(message, 10.0, &ind);
		values.ampHours				= buffer_get_float32(message, 10000.0, &ind);
		values.ampHoursCharged		= buffer_get_float32(message, 10000.0, &ind);
		values.wattHours			= buffer_get_float32(message, 1e4, &ind);
		values.watthoursCharged		= buffer_get_float32(message, 1e4, &ind);
		values.tachometer			= buffer_get_int32(message, &ind);
		values.tachometerAbs		= buffer_get_int32(message, &ind);
		values.faultCode			= message[ind];
		return true;
		break;
	default:
		return false;
		break;
	}

}*/
int VescUartGetMC(uint8_t* values) {
	uint8_t command[1] = { COMM_GET_MCCONF_DEFAULT };
	uint8_t payload[512];
	PackSendPayload(command, 1, 0);
	//delay(0); //needed, otherwise data is not read
	int lenPayload = ReceiveUartMessage(payload, 0);
	if (lenPayload > 1) {
		//bool read = ProcessReadPacketMC(payload, values, lenPayload); //returns true if sucessful
		memcpy(values,payload+1,lenPayload-1);
		return lenPayload;
	}
	else
	{
		return 0;
	}
}







bool VescUartGetValue(bldcMeasure& values, int num) {
	uint8_t command[1] = { COMM_GET_VALUES };
	uint8_t payload[256];
	PackSendPayload(command, 1, num);
	delay(10); //needed, otherwise data is not read
	int lenPayload = ReceiveUartMessage(payload, num);
	if (lenPayload > 1) {
		bool read = ProcessReadPacket(payload, values, lenPayload); //returns true if sucessful
		return read;
	}
	else
	{
		return false;
	}
}


bool VescUartGetValue(bldcMeasure& values) {
	return VescUartGetValue(values, 0);
}

void VescUartSetCurrent(float current, int num) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_CURRENT ;
	buffer_append_int32(payload, (int32_t)(current * 1000), &index);
	PackSendPayload(payload, 5, num);
}
void VescUartSetCurrent(float current){
	VescUartSetCurrent(current, 0);
}

void VescUartSetPosition(float position, int num) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_POS ;
	buffer_append_int32(payload, (int32_t)(position * 1000000.0), &index);
	PackSendPayload(payload, 5, num);
}
void VescUartSetPosition(float position) {
	VescUartSetPosition(position, 0);
}

void VescUartSetDuty(float duty, int num) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_DUTY ;
	buffer_append_int32(payload, (int32_t)(duty * 100000), &index);
	PackSendPayload(payload, 5, num);
}
void VescUartSetDuty(float duty) {
	VescUartSetDuty(duty, 0);
}


void VescUartSetRPM(float rpm, int num) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_RPM ;
	buffer_append_int32(payload, (int32_t)(rpm), &index);
	PackSendPayload(payload, 5, num);
}
void VescUartSetRPM(float rpm) {
	VescUartSetRPM(rpm, 0);
}

void VescUartSetCurrentBrake(float brakeCurrent, int num) {
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_CURRENT_BRAKE;
	buffer_append_int32(payload, (int32_t)(brakeCurrent * 1000), &index);
	PackSendPayload(payload, 5, num);
}
void VescUartSetCurrentBrake(float brakeCurrent) {
	VescUartSetCurrentBrake(brakeCurrent, 0);
}


void VescUartSetNunchukValues(remotePackage& data, int num) {
	int32_t ind = 0;
	uint8_t payload[11];
	payload[ind++] = COMM_SET_CHUCK_DATA;
	payload[ind++] = data.valXJoy;
	payload[ind++] = data.valYJoy;
	buffer_append_bool(payload, data.valLowerButton, &ind);
	buffer_append_bool(payload, data.valUpperButton, &ind);
	//Acceleration Data. Not used, Int16 (2 byte)
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;

if(debugSerialPort!=NULL){
	debugSerialPort->println("Data reached at VescUartSetNunchuckValues:");
	debugSerialPort->print("valXJoy = "); debugSerialPort->print(data.valXJoy); debugSerialPort->print(" valYJoy = "); debugSerialPort->println(data.valYJoy);
	debugSerialPort->print("LowerButton = "); debugSerialPort->print(data.valLowerButton); debugSerialPort->print(" UpperButton = "); debugSerialPort->println(data.valUpperButton);
}

	PackSendPayload(payload, 11, num);
}
void VescUartSetNunchukValues(remotePackage& data) {
	VescUartSetNunchukValues(data, 0);
}


void SerialPrint(uint8_t* data, int len) {

	//	debugSerialPort->print("Data to display: "); debugSerialPort->println(sizeof(data));

	for (int i = 0; i <= len; i++)
	{
		debugSerialPort->print(data[i]);
		debugSerialPort->print(" ");
	}
	debugSerialPort->println("");
}


void SerialPrint(const struct bldcMeasure& values) {
	debugSerialPort->print("tempFetFiltered:	"); debugSerialPort->println(values.tempFetFiltered);
	debugSerialPort->print("tempMotorFiltered:"); debugSerialPort->println(values.tempMotorFiltered);
	debugSerialPort->print("avgMotorCurrent:	"); debugSerialPort->println(values.avgMotorCurrent);
	debugSerialPort->print("avgInputCurrent:	"); debugSerialPort->println(values.avgInputCurrent);
	debugSerialPort->print("avgId:			"); debugSerialPort->println(values.avgId);
	debugSerialPort->print("avgIq:			"); debugSerialPort->println(values.avgIq);
	debugSerialPort->print("dutyNow:			"); debugSerialPort->println(values.dutyNow);
	debugSerialPort->print("rpm:				"); debugSerialPort->println(values.rpm);
	debugSerialPort->print("inpVoltage:		"); debugSerialPort->println(values.inpVoltage);
	debugSerialPort->print("ampHours:		"); debugSerialPort->println(values.ampHours);
	debugSerialPort->print("ampHoursCharged:	"); debugSerialPort->println(values.ampHoursCharged);
	debugSerialPort->print("wattHours:		"); debugSerialPort->println(values.wattHours);
	debugSerialPort->print("wattHoursCharged:	"); debugSerialPort->println(values.watthoursCharged);
	debugSerialPort->print("tachometer:		"); debugSerialPort->println(values.tachometer);
	debugSerialPort->print("tachometerAbs:	"); debugSerialPort->println(values.tachometerAbs);
	debugSerialPort->print("faultCode:		"); debugSerialPort->println(values.faultCode);


}
