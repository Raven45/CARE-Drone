/*******************************************************************************
  * Copyright 2016  Aaron Burns,
 *                  Joshua Donaway,
 *                  Matthew Love,
 *                  Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*******************************************************************************/

#include "System.h"
#include "Altimeter.h"
#include "Accelerometer.h"

#ifdef SYSTEM_IS_SINGLETON
System* System::Instance = 0;
#endif

System::System() {
    
}

System::~System() {
    
}

bool System::InitializeSystem() {
    
    //InitializeSystemClock();
    
    try {
        //Create the devices
        _Gyroscope = new HAL::Gyroscope(ADDRESS_GYRO, &DeviceManager);
        _Accelerometer = new HAL::Accelerometer(ADDRESS_ACCEL, &DeviceManager);
        _Magnetometer = new HAL::Magnetometer(ADDRESS_MAG, &DeviceManager);
        _Altimeter = new HAL::Altimeter(ADDRESS_BAROMETER, &DeviceManager);
        Motor_1 = new HAL::PWMC(ADDRESS_MOTOR_1, &DeviceManager);
        Motor_2 = new HAL::PWMC(ADDRESS_MOTOR_2, &DeviceManager);
        Motor_3 = new HAL::PWMC(ADDRESS_MOTOR_3, &DeviceManager);
        Motor_4 = new HAL::PWMC(ADDRESS_MOTOR_4, &DeviceManager);
        Motor_5 = new HAL::PWMC(ADDRESS_MOTOR_5, &DeviceManager);
        Motor_6 = new HAL::PWMC(ADDRESS_MOTOR_6, &DeviceManager);
    
        //Build the device list.
        Devices.push_back(_Gyroscope);
        Devices.push_back(_Accelerometer);
        Devices.push_back(_Magnetometer);
        Devices.push_back(_Altimeter);
        Devices.push_back(Motor_1);
        Devices.push_back(Motor_2);
        Devices.push_back(Motor_3);
        Devices.push_back(Motor_4);
        Devices.push_back(Motor_5);
        Devices.push_back(Motor_6);

        //Initialize the usb bus.
        usb_init();

        //Initialize devices
        for (unsigned int i = 0; i < Devices.size(); i++) {
            Devices[i]->Initialize();
        }

        return true;
    }
    catch (...) {
        //return false if we fail to initialize, more than likely due to
        //failure to allocate memory. 
        return false;
    }
}

bool System::UpdateSystem() {
    
    //Update devices
    for (unsigned int i = 0; i < Devices.size(); i++) {
        Devices[i]->Update();
    }
}

void System::Main() {
    
    //Run the debug main function if we're in debug mode.
    if (State == States::Debug) {
        DebugMain();
    }
    
    //Run the main function for when the motors are running.
    else if (State == States::Run) {
        RunMain();
    }
    
    //Go to standby.
    else if (State == States::Standby) {
        StandbyMain();
    }
}

bool System::ClearToProceed() {
    return true;
}

bool System::IsUSBAttached() {
    
    return (usb_is_configured() && !usb_out_endpoint_halted(2));
}

#ifdef SYSTEM_IS_SINGLETON
System* System::GetInstance() {
    
    //If singleton object is not initialized.
    if (!Instance) {
        
        //Attempt to create System object.
        try{
            Instance = new System();
        }
        
        //Return zero if the system object failed to initialize.
        catch (std::bad_alloc) {
            return 0;
        }
    }
    
    //Return the pointer to the System object.
    return Instance;
}
#endif

void System::StandbyMain() {
    
    float Input_Throttle;
    
    //Update all SPI devices. Mostly, this will read sensors in
    //Standby mode.
    UpdateSystem();
    
    //Get the throttle input from user. Incoming signal is a RC Servo signal ranging from
    //1 ms pulses (0%) to 2ms pulses (100%).
    if (HAL::Timer::GetInstance()->TimerList[3].GetTime_US() > INPUT_FLOOR &&
        HAL::Timer::GetInstance()->TimerList[3].GetTime_US() <= INPUT_CEILING) {
        
        Input_Throttle = (((HAL::Timer::GetInstance()->TimerList[3].GetTime_US() - 1000)/1000)*100.0f);
    }
    
    //Go to debug mode if USB cable is attached.
    if (IsUSBAttached()) {
        GoToState(States::Debug);
    }
    else {
        
        if (Safety && Input_Throttle == 0) {
            Safety = false;
        }
        else if (!Safety && Input_Throttle > 15) {
            GoToState(States::Standby);
        }
    }
}

void System::RunMain() {
    
    float Input_Roll;
    float Input_Pitch;
    float Input_Yaw;
    float Output_Roll;
    float Output_Pitch;
    float Output_Yaw;
    float Input_Throttle;
    float Input_Cargo;
    
    //Perform system wide update
    UpdateSystem();
    
    //Get the roll input from user. Incoming signal is a RC Servo signal ranging from
    //1 ms pulses (-15 degrees) to 2ms pulses (15 degrees).
    if (HAL::Timer::GetInstance()->TimerList[0].GetTime_US() > INPUT_FLOOR &&
        HAL::Timer::GetInstance()->TimerList[0].GetTime_US() <= INPUT_CEILING) {
        
        Input_Roll = (((HAL::Timer::GetInstance()->TimerList[0].GetTime_US() - 1000)/1000)*30.0f)-15.0f;
    }
    
    //Get the pitch input from user. Incoming signal is a RC Servo signal ranging from
    //1 ms pulses (-15 degrees) to 2ms pulses (15 degrees).
    if (HAL::Timer::GetInstance()->TimerList[1].GetTime_US() > INPUT_FLOOR &&
        HAL::Timer::GetInstance()->TimerList[1].GetTime_US() <= INPUT_CEILING) {
        
        Input_Pitch = (((HAL::Timer::GetInstance()->TimerList[1].GetTime_US() - 1000)/1000)*30.0f)-15.0f;
    }
    
    //Get the yaw input from user. Incoming signal is a RC Servo signal ranging from
    //1 ms pulses to 2ms pulses. Input controls the rate of yaw.
    if (HAL::Timer::GetInstance()->TimerList[2].GetTime_US() > INPUT_FLOOR &&
        HAL::Timer::GetInstance()->TimerList[2].GetTime_US() <= INPUT_CEILING) {
        
        Input_Yaw += (((HAL::Timer::GetInstance()->TimerList[2].GetTime_US() - 1000)/1000)*10.0f) * DeltaTime;
    }
    
    //Get the throttle input from user. Incoming signal is a RC Servo signal ranging from
    //1 ms pulses (0%) to 2ms pulses (100%).
    if (HAL::Timer::GetInstance()->TimerList[3].GetTime_US() > INPUT_FLOOR &&
        HAL::Timer::GetInstance()->TimerList[3].GetTime_US() <= INPUT_CEILING) {
        
        Input_Throttle = (((HAL::Timer::GetInstance()->TimerList[3].GetTime_US() - 1000)/1000)*100.0f);
    }

    //Handle RC output
    if (HAL::Timer::GetInstance()->TimerList[5].GetTime_MS()  > RC_Output) {
        
        //Turn output off.
        LATCbits.LATC7 = 0;
    }
    if (HAL::Timer::GetInstance()->TimerList[5].GetTime_MS()  > 20) {
        
        //Turn output on.
        LATCbits.LATC7 = 1;
        
        //Reset timer.
        HAL::Timer::GetInstance()->TimerList[5].SetClock(0, 0, 0);
    }
    
    
    //Get the set point for our PID controller.
    Math::Quaternion SetPoint(Input_Roll, Input_Pitch, Input_Yaw);
    
    //Get the amount of time since last frame.
    DeltaTime = HAL::Timer::GetInstance()->TimerList[4].GetTime_US();
    HAL::Timer::GetInstance()->TimerList[4].SetClock(0,0,0);
    
    //Get the current orientation using Madgwick's sensor fusion algorithm.
    AHRS_Update();
    
    //Calculate the error
    Math::Quaternion Error = SetPoint*CurrentOrientation;
    
    //Calculate our output
    Math::Quaternion Output = CalculatePID(Error);
    
    //Convert to euler angles.
    //Output_Roll = Output.GetRoll();
    //Output_Pitch = Output.GetPitch();
    //Output_Yaw = Output.GetYaw();
    
    //Calculate what throttles we need. This is where Matt's control
    //algorithm is utilized. 
    //Motor_1.SetThrottle();
    //Motor_2.SetThrottle();
    //Motor_3.SetThrottle();
    //Motor_4.SetThrottle();
    //Motor_5.SetThrottle();
    //Motor_6.SetThrottle();
}

void System::DebugMain() {
    
    //Run system wide update.
    UpdateSystem();
    
    //Get the amount of time since last frame.
    DeltaTime = HAL::Timer::GetInstance()->TimerList[4].GetTime_US();
    HAL::Timer::GetInstance()->TimerList[4].SetClock(0,0,0);
    
    //Retrieve commands received through USB bus.
    const unsigned char * Command = ReceiveCommand();
    
    //Execute received commands.
    ExecuteCommand(Command);
}


void System::GoToState(UnsignedInteger16 State) {
    this->State = State;
}
    
    
const unsigned char * System::ReceiveCommand() {
    
    //Receive data from host
    if (usb_is_configured() &&
        !usb_out_endpoint_halted(2) &&
        usb_out_endpoint_has_data(2)) {
        
        const unsigned char * Incoming;
        
        if (usb_get_out_buffer(2, &Incoming) <= 0) {
            //Empty response condition
            usb_arm_out_endpoint(2);
            return 0;
        }
        else {
            return Incoming;
        }
    }
    
    else {
        return 0;
    }
}

bool System::ExecuteCommand(const unsigned char * Command) {
    
    
    std::string strCommand((const char*)Command);
    
    if (strCommand.rfind("ReadGyroscope") != std::string::npos) { 
        Command_ReadGyroscope(strCommand);
    }
    else if (strCommand.rfind("ReadAccelerometer") != std::string::npos) { 
        Command_ReadAccelerometer(strCommand);
    }
    else if (strCommand.rfind("GetOrientation") != std::string::npos) { 
        Command_GetOrientation();
    }
    else if (strCommand.rfind("GetPressure") != std::string::npos) { 
        Command_GetPressure();
    }
    else if (strCommand.rfind("GetTemperature") != std::string::npos) { 
        Command_GetTemperature();
    }
    else if (strCommand.rfind("GetAltitude") != std::string::npos) { 
        Command_GetAltitude();
    }
    else if (strCommand.rfind("SetThrottle") != std::string::npos) { 
        Command_SetThrottle(strCommand);
    }
    else if (strCommand.rfind("GetThrottle") != std::string::npos) { 
        Command_GetThrottle(strCommand);
    }
    else if (strCommand.rfind("StopAllMotors") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("SetAllMotors") != std::string::npos) { 
        Command_SetAllMotors(strCommand);
    }
    else if (strCommand.rfind("ReleaseCargo") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("HoldCargo") != std::string::npos) { 
        Command_StopAllMotors();
    }
    else if (strCommand.rfind("ReturnToStandby") != std::string::npos) { 
        Command_ReturnToStandby();
    }
}

bool System::SendUSBData(std::string Message) {
    
    if (usb_is_configured() &&
        !usb_in_endpoint_halted(2) &&
        !usb_in_endpoint_busy(2)) {
        
        unsigned char * cMessage = (unsigned char *)Message.c_str();
        cMessage = usb_get_in_buffer(2);
        
        usb_send_in_buffer(2, Message.length());
        return true;
    }
    else {
        return false;
    }
}

Math::Quaternion System::AHRS_Update() {
    
    float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalization)
	if (    (_Magnetometer->GetMagX() == 0.0f) && 
            (_Magnetometer->GetMagY() == 0.0f) && 
            (_Magnetometer->GetMagZ() == 0.0f)) {
        
		return IMU_Update();
	}

	// Rate of change of quaternion from gyroscope
    Math::Quaternion Gyro(0, _Gyroscope->GetRateX(), _Gyroscope->GetRateY(), _Gyroscope->GetRateZ());
    Math::Quaternion RateOfChange = 0.5f * CurrentOrientation * Gyro;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalization)
	if(!(   (_Accelerometer->GetAccelX() == 0.0f) && 
            (_Accelerometer->GetAccelY() == 0.0f) && 
            (_Accelerometer->GetAccelZ() == 0.0f))) {

        //Normalize accelerometer measurement
        Math::Quaternion Accel(0, _Accelerometer->GetAccelX(), _Accelerometer->GetAccelY(), _Accelerometer->GetAccelZ());
        Accel = Accel.Normalize();
		 
        //Normalize magnetometer measurement
        Math::Quaternion Mag(0, _Magnetometer->GetMagX(), _Magnetometer->GetMagY(), _Magnetometer->GetMagZ());
        Mag = Mag.Normalize();

		// Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * CurrentOrientation[0] * _Magnetometer->GetMagX();
        _2q0my = 2.0f * CurrentOrientation[0] * _Magnetometer->GetMagY();
        _2q0mz = 2.0f * CurrentOrientation[0] * _Magnetometer->GetMagZ();
        _2q1mx = 2.0f * CurrentOrientation[1] * _Magnetometer->GetMagX();
		_2q0 = 2.0f * CurrentOrientation[0];
		_2q1 = 2.0f * CurrentOrientation[1];
		_2q2 = 2.0f * CurrentOrientation[2];
		_2q3 = 2.0f * CurrentOrientation[3];
		_2q0q2 = 2.0f * CurrentOrientation[0] * CurrentOrientation[2];
		_2q2q3 = 2.0f * CurrentOrientation[2] * CurrentOrientation[3];
		q0q0 = CurrentOrientation[0] * CurrentOrientation[0];
		q0q1 = CurrentOrientation[0] * CurrentOrientation[1];
		q0q2 = CurrentOrientation[0] * CurrentOrientation[2];
		q0q3 = CurrentOrientation[0] * CurrentOrientation[3];
		q1q1 = CurrentOrientation[1] * CurrentOrientation[1];
		q1q2 = CurrentOrientation[1] * CurrentOrientation[2];
		q1q3 = CurrentOrientation[1] * CurrentOrientation[3];
		q2q2 = CurrentOrientation[2] * CurrentOrientation[2];
		q2q3 = CurrentOrientation[2] * CurrentOrientation[3];
		q3q3 = CurrentOrientation[3] * CurrentOrientation[3];

		// Reference direction of Earth's magnetic field
		hx = _Magnetometer->GetMagX() * q0q0 - _2q0my * CurrentOrientation[3] + _2q0mz * CurrentOrientation[2] + _Magnetometer->GetMagX() * q1q1 + _2q1 * _Magnetometer->GetMagY() * CurrentOrientation[2] + _2q1 * _Magnetometer->GetMagZ() * CurrentOrientation[3] - _Magnetometer->GetMagX() * q2q2 - _Magnetometer->GetMagX() * q3q3;
		hy = _2q0mx * CurrentOrientation[3] + _Magnetometer->GetMagY() * q0q0 - _2q0mz * CurrentOrientation[1] + _2q1mx * CurrentOrientation[2] - _Magnetometer->GetMagY() * q1q1 + _Magnetometer->GetMagY() * q2q2 + _2q2 * _Magnetometer->GetMagZ() * CurrentOrientation[3] - _Magnetometer->GetMagY() * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * CurrentOrientation[2] + _2q0my * CurrentOrientation[1] + _Magnetometer->GetMagZ() * q0q0 + _2q1mx * CurrentOrientation[3] - _Magnetometer->GetMagZ() * q1q1 + _2q2 * _Magnetometer->GetMagY() * CurrentOrientation[3] - _Magnetometer->GetMagZ() * q2q2 + _Magnetometer->GetMagZ() * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
        Math::Quaternion StepMagnitude;
		StepMagnitude[0] = -_2q2 * (2.0f * q1q3 - _2q0q2 - _Accelerometer->GetAccelX()) + _2q1 * (2.0f * q0q1 + _2q2q3 - _Accelerometer->GetAccelY()) - _2bz * CurrentOrientation[2] * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - _Magnetometer->GetMagX()) + (-_2bx * CurrentOrientation[3] + _2bz * CurrentOrientation[1]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - _Magnetometer->GetMagY()) + _2bx * CurrentOrientation[2] * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - _Magnetometer->GetMagZ());
		StepMagnitude[1] = _2q3 * (2.0f * q1q3 - _2q0q2 - _Accelerometer->GetAccelX()) + _2q0 * (2.0f * q0q1 + _2q2q3 - _Accelerometer->GetAccelY()) - 4.0f * CurrentOrientation[1] * (1 - 2.0f * q1q1 - 2.0f * q2q2 - _Accelerometer->GetAccelZ()) + _2bz * CurrentOrientation[3] * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - _Magnetometer->GetMagX()) + (_2bx * CurrentOrientation[2] + _2bz * CurrentOrientation[0]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - _Magnetometer->GetMagY()) + (_2bx * CurrentOrientation[3] - _4bz * CurrentOrientation[1]) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - _Magnetometer->GetMagZ());
		StepMagnitude[2] = -_2q0 * (2.0f * q1q3 - _2q0q2 - _Accelerometer->GetAccelX()) + _2q3 * (2.0f * q0q1 + _2q2q3 - _Accelerometer->GetAccelY()) - 4.0f * CurrentOrientation[2] * (1 - 2.0f * q1q1 - 2.0f * q2q2 - _Accelerometer->GetAccelZ()) + (-_4bx * CurrentOrientation[2] - _2bz * CurrentOrientation[0]) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - _Magnetometer->GetMagX()) + (_2bx * CurrentOrientation[1] + _2bz * CurrentOrientation[3]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - _Magnetometer->GetMagY()) + (_2bx * CurrentOrientation[0] - _4bz * CurrentOrientation[2]) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - _Magnetometer->GetMagZ());
		StepMagnitude[3] = _2q1 * (2.0f * q1q3 - _2q0q2 - _Accelerometer->GetAccelX()) + _2q2 * (2.0f * q0q1 + _2q2q3 - _Accelerometer->GetAccelY()) + (-_4bx * CurrentOrientation[3] + _2bz * CurrentOrientation[1]) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - _Magnetometer->GetMagX()) + (-_2bx * CurrentOrientation[0] + _2bz * CurrentOrientation[2]) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - _Magnetometer->GetMagY()) + _2bx * CurrentOrientation[1] * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - _Magnetometer->GetMagZ());
        StepMagnitude = StepMagnitude.Normalize();

		// Apply feedback step
        RateOfChange -= Beta*StepMagnitude;
	}

	// Integrate rate of change of quaternion to yield quaternion
    CurrentOrientation += RateOfChange * DeltaTime;

	// Normalize quaternion
    CurrentOrientation = CurrentOrientation.Normalize();
    return CurrentOrientation;
}

Math::Quaternion System::IMU_Update() {
    
    float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
    Math::Quaternion Gyro(0, _Gyroscope->GetRateX(), _Gyroscope->GetRateY(), _Gyroscope->GetRateZ());
    Math::Quaternion RateOfChange = 0.5f * CurrentOrientation * Gyro;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalization)
	if(!(   (_Accelerometer->GetAccelX() == 0.0f) && 
            (_Accelerometer->GetAccelY() == 0.0f) && 
            (_Accelerometer->GetAccelZ() == 0.0f))) {

		//Normalize accelerometer measurement
        Math::Quaternion Accel(
                0, 
                _Accelerometer->GetAccelX(), 
                _Accelerometer->GetAccelY(), 
                _Accelerometer->GetAccelZ());
        Accel = Accel.Normalize();  

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * CurrentOrientation[0];
		_2q1 = 2.0f * CurrentOrientation[1];
		_2q2 = 2.0f * CurrentOrientation[2];
		_2q3 = 2.0f * CurrentOrientation[3];
		_4q0 = 4.0f * CurrentOrientation[0];
		_4q1 = 4.0f * CurrentOrientation[1];
		_4q2 = 4.0f * CurrentOrientation[2];
		_8q1 = 8.0f * CurrentOrientation[1];
		_8q2 = 8.0f * CurrentOrientation[2];
		q0q0 = CurrentOrientation[0] * CurrentOrientation[0];
		q1q1 = CurrentOrientation[1] * CurrentOrientation[1];
		q2q2 = CurrentOrientation[2] * CurrentOrientation[2];
		q3q3 = CurrentOrientation[3] * CurrentOrientation[3];

		// Gradient decent algorithm corrective step
        Math::Quaternion StepMagnitude;
		StepMagnitude[0] = _4q0 * q2q2 + _2q2 * _Accelerometer->GetAccelX() + _4q0 * q1q1 - _2q1 * _Accelerometer->GetAccelY();
		StepMagnitude[1] = _4q1 * q3q3 - _2q3 * _Accelerometer->GetAccelX() + 4.0f * q0q0 * CurrentOrientation[1] - _2q0 * _Accelerometer->GetAccelY() - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * _Accelerometer->GetAccelZ();
		StepMagnitude[2] = 4.0f * q0q0 * CurrentOrientation[2] + _2q0 * _Accelerometer->GetAccelX() + _4q2 * q3q3 - _2q3 * _Accelerometer->GetAccelY() - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * _Accelerometer->GetAccelZ();
		StepMagnitude[3] = 4.0f * q1q1 * CurrentOrientation[3] - _2q1 * _Accelerometer->GetAccelX() + 4.0f * q2q2 * CurrentOrientation[3] - _2q2 * _Accelerometer->GetAccelY();
		StepMagnitude = StepMagnitude.Normalize();

		// Apply feedback step
        RateOfChange -= Beta*StepMagnitude;
	}

	// Integrate rate of change of quaternion to yield quaternion
    CurrentOrientation += RateOfChange * DeltaTime;

	// Normalize quaternion
    CurrentOrientation = CurrentOrientation.Normalize();
}

Math::Quaternion System::CalculatePID(Math::Quaternion Error) {
    
    Math::Quaternion IntegralComponent = Error * DeltaTime;
    Math::Quaternion DerivativeComponent = Error / DeltaTime;
    return Kp*Error + Ki*IntegralComponent + Kd*DerivativeComponent;
}

bool System::Command_ReadGyroscope(std::string Command) {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    //Parse string for commands
    int X_pos = Command.rfind("/X");
    int Y_pos = Command.rfind("/Y");
    int Z_pos = Command.rfind("/Z");
    
    //Get pointer to the USB send buffer.
    unsigned char *buf = usb_get_in_buffer(2);
    
    if (X_pos == std::string::npos &&
        Y_pos == std::string::npos &&
        Z_pos == std::string::npos ) {
        
        return false;
    }
    
    //If get_x command was found
    if (X_pos != std::string::npos) {
        
        //Retrieve X axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Gyroscope->GetRateX();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send X axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    //If get_y command was found
    if (Y_pos != std::string::npos) {
        
        //Retrieve Y axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Gyroscope->GetRateY();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send Y axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    //If get_z command was found
    if (Z_pos != std::string::npos) {
        
        //Retrieve Z axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Gyroscope->GetRateZ();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send Z axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    return true;
}

bool System::Command_ReadAccelerometer(std::string Command) {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    //Parse string for commands
    int X_pos = Command.rfind("/X");
    int Y_pos = Command.rfind("/Y");
    int Z_pos = Command.rfind("/Z");
    
    //Get pointer to the USB send buffer.
    unsigned char *buf = usb_get_in_buffer(2);
    
    if (X_pos == std::string::npos &&
        Y_pos == std::string::npos &&
        Z_pos == std::string::npos ) {
        
        return false;
    }
    
    //If get_x command was found
    if (X_pos != std::string::npos) {
        
        //Retrieve X axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Accelerometer->GetAccelX();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send X axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    //If get_y command was found
    if (Y_pos != std::string::npos) {
        
        //Retrieve Y axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Accelerometer->GetAccelY();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send Y axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    //If get_z command was found
    if (Z_pos != std::string::npos) {
        
        //Retrieve Z axis gyro reading
        FloatBuffer fbuf;
        fbuf.a = _Accelerometer->GetAccelZ();
        buf = fbuf.b;
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));
        
        //Send Z axis gyro reading.
        usb_send_in_buffer(2, 4);
    }
    
    return true;
}

bool System::Command_GetOrientation() {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    FloatBuffer fbuf;
    unsigned char *buf = usb_get_in_buffer(2);
    
    AHRS_Update();
    
    for (int i = 0; i < 4; i++) {
       
        fbuf.a = CurrentOrientation[i];
        buf = fbuf.b;

        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));

        //Send the orientation data.
        usb_send_in_buffer(2, 4);
    }
    
    return true;
}

bool System::Command_GetPressure() {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    FloatBuffer fbuf;
    unsigned char *buf = usb_get_in_buffer(2);
    fbuf.a = _Altimeter->GetPressure();
    buf = fbuf.b;

    //Wait until the PC can accept data.
    while (usb_in_endpoint_busy(2));

    //Send pressure reading.
    usb_send_in_buffer(2, 4);
    
    return true;
}

bool System::Command_GetTemperature() {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    FloatBuffer fbuf;
    unsigned char *buf = usb_get_in_buffer(2);
    fbuf.a = _Altimeter->GetTemperature();
    buf = fbuf.b;

    //Wait until the PC can accept data.
    while (usb_in_endpoint_busy(2));

    //Send pressure reading.
    usb_send_in_buffer(2, 4);
    
    return true;
}

bool System::Command_GetAltitude() {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    FloatBuffer fbuf;
    unsigned char *buf = usb_get_in_buffer(2);
    fbuf.a = _Altimeter->GetAltitude();
    buf = fbuf.b;

    //Wait until the PC can accept data.
    while (usb_in_endpoint_busy(2));

    //Send pressure reading.
    usb_send_in_buffer(2, 4);
    
    return true;
}

bool System::Command_SetThrottle(std::string Command) {
    
    int Channel = Command.rfind("/C:");
    int Throttle = Command.rfind("/D:");
    
    if (Channel != std::string::npos) {
        
        //Get channel from string using saved location of argument.
        Channel = Command[Channel+3] - 30;
    }
    else {
        return false;
    }
  
    if (Throttle != std::string::npos) {
        
        //Get throttle and then set the throttle.
        Throttle = atoi(Command.substr(Throttle+3).c_str());
        
        //Bounds check the Throttle input.
        if (Throttle < 0 || Throttle > 100) {
            return false;
        }
        else {
            
            switch (Channel) {
                case 1: Motor_1->SetThrottle(Throttle); break;
                case 2: Motor_2->SetThrottle(Throttle); break;
                case 3: Motor_3->SetThrottle(Throttle); break;
                case 4: Motor_4->SetThrottle(Throttle); break;
                case 5: Motor_5->SetThrottle(Throttle); break;
                case 6: Motor_6->SetThrottle(Throttle); break;
                default: return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}

bool System::Command_GetThrottle(std::string Command) {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    int Channel = Command.rfind("/C:");
    unsigned char *buf = usb_get_in_buffer(2);
    
    if (Channel != std::string::npos) {
        
        //Get channel from string using saved location of argument.
        Channel = Command[Channel+3] - 30;
        
        switch (Channel) {
            case 1: buf[0] = Motor_1->GetThrottle(); break;
            case 2: buf[0] = Motor_2->GetThrottle(); break;
            case 3: buf[0] = Motor_3->GetThrottle(); break;
            case 4: buf[0] = Motor_4->GetThrottle(); break;
            case 5: buf[0] = Motor_5->GetThrottle(); break;
            case 6: buf[0] = Motor_6->GetThrottle(); break;
            default: return false;
        }
        
        //Wait until the PC can accept data.
        while (usb_in_endpoint_busy(2));

        //Send pressure reading.
        usb_send_in_buffer(2, 4);
    }
    else {
        return false;
    }
}

bool System::Command_StopAllMotors() {
    
    Motor_1->StopMotor();
    Motor_2->StopMotor();
    Motor_3->StopMotor();
    Motor_4->StopMotor();
    Motor_5->StopMotor();
    Motor_6->StopMotor();
    
    return true;
}

bool System::Command_SetAllMotors(std::string Command) {
    
    int Throttle = Command.rfind("/D:");
    
    if (Command.rfind("/D:") != std::string::npos) {
        
        //Get throttle and then set the throttle.
        Throttle = atoi(Command.substr(Throttle+3).c_str());
        
        //Bounds check the Throttle input.
        if (Throttle < 0 || Throttle > 100) {
            return false;
        }
        else {
            Motor_1->SetThrottle(Throttle); 
            Motor_2->SetThrottle(Throttle); 
            Motor_3->SetThrottle(Throttle); 
            Motor_4->SetThrottle(Throttle); 
            Motor_5->SetThrottle(Throttle); 
            Motor_6->SetThrottle(Throttle);
        }
    }
}

bool System::Command_ReleaseCargo() {
    
    PORTSetBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
    
}

bool System::Command_HoldCargo() {
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTSetBits(IOPORT_C, BIT_5);
    
    Time Delaytimer;
    Delaytimer.SetClock(2, 0, 0);
    HAL::Timer::GetInstance()->Delay(Delaytimer);
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
}

bool System::Command_ReturnToStandby() {
    GoToState(States::Standby);
    return true;
}

bool System::Command_USBTest() {
    
    if (!IsUSBAttached()) {
        return false;
    }
    
    unsigned char *buf = usb_get_in_buffer(2);
    
    buf = "There shall be wings!";
    
    //Wait until the PC can accept data.
    while (usb_in_endpoint_busy(2));

    //Send pressure reading.
    usb_send_in_buffer(2, sizeof(buf));
}