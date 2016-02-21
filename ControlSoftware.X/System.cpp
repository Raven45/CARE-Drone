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
//        _Gyroscope = new HAL::Gyroscope(ADDRESS_GYRO, &DeviceManager);
//        _Accelerometer = new HAL::Accelerometer(ADDRESS_ACCEL, &DeviceManager);
//        _Magnetometer = new HAL::Magnetometer(ADDRESS_MAG, &DeviceManager);
//        _Altimeter = new HAL::Altimeter(ADDRESS_BAROMETER, &DeviceManager);
//        Motor_1 = new HAL::PWMC(ADDRESS_MOTOR_1, &DeviceManager);
//        Motor_2 = new HAL::PWMC(ADDRESS_MOTOR_2, &DeviceManager);
//        Motor_3 = new HAL::PWMC(ADDRESS_MOTOR_3, &DeviceManager);
//        Motor_4 = new HAL::PWMC(ADDRESS_MOTOR_4, &DeviceManager);
//        Motor_5 = new HAL::PWMC(ADDRESS_MOTOR_5, &DeviceManager);
//        Motor_6 = new HAL::PWMC(ADDRESS_MOTOR_6, &DeviceManager);
//    
//        //Build the device list.
//        Devices.push_back(_Gyroscope);
//        Devices.push_back(_Accelerometer);
//        Devices.push_back(_Magnetometer);
//        Devices.push_back(_Altimeter);
//        Devices.push_back(Motor_1);
//        Devices.push_back(Motor_2);
//        Devices.push_back(Motor_3);
//        Devices.push_back(Motor_4);
//        Devices.push_back(Motor_5);
//        Devices.push_back(Motor_6);

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
//    for (unsigned int i = 0; i < Devices.size(); i++) {
//        Devices[i]->Update();
//    }
}

void System::Main() {
    
    if (ValidationTestMode == TEST_MODE_OFF) {
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
    
    else if (ValidationTestMode == TEST_PROPULSION) {
        
    }
    
    else if (ValidationTestMode == TEST_CARGO) {
        
    }
    
    else if (ValidationTestMode == TEST_FLIGHT_DATA) {
        
    }
}

bool System::ClearToProceed() {
    return true;
}

bool System::IsUSBAttached() {
    
    return (usb_is_configured() && !usb_out_endpoint_halted(2));
}

//<editor-fold defaultstate="collapsed" desc="input handlers">

void System::SetRollInput(unsigned int input) {
    this->Input_Roll = input;
}

void System::SetPitchInput(unsigned int input) {
    this->Input_Pitch = input;
}

void System::SetYawInput(unsigned int input) {
    this->Input_Yaw = input;
}

void System::SetThrottleInput(unsigned int input) {
    this->Input_Throttle = input;
}

void System::SetCargoInput(unsigned int input) {
    this->Input_Cargo = input;
}

//</editor-fold>

#ifdef SYSTEM_IS_SINGLETON
System* System::GetInstance() {
    
    //If singleton object is not initialized.
    if (!Instance) {
        
        //Attempt to create System object.
        try {
            Instance = new System();
        }
        
        //Return zero if the system object failed to initialize.
        catch (...) {
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
    
    float Output_Roll;
    float Output_Pitch;
    float Output_Yaw;
    
    //Perform system wide update
    UpdateSystem();

    //If we are at zero throttle and we're under ten feet.
    if (Input_Throttle == 0 && _Altimeter->GetAltitude() < 10) {
        Command_StopAllMotors();
        GoToState(States::Standby);
        return;
    }
    
    //Else we're in the air.
    else {
        
        //Get the amount of time since last frame.
        DeltaTime = CoreTimer.DeltaTimer.GetTime_US();
        CoreTimer.DeltaTimer.SetClock(0,0,0);
        
        //Get the current orientation using Madgwick's sensor fusion algorithm.
        AHRS_Update();

        //Calculate the error
        Math::Quaternion Error = SetPoint*CurrentOrientation;
        
        //Get the set point for our PID controller.
        Math::Quaternion SetPoint(Input_Roll, Input_Pitch, Input_Yaw);

        //Calculate our output
        Math::Quaternion Output = CalculatePID(Error);

        //Convert to euler angles.
        Output_Roll = Output.GetRoll();
        Output_Pitch = Output.GetPitch();
        Output_Yaw = Output.GetYaw();
    
        //Calculate what throttles we need. This is where Matt's control
        //algorithm is utilized. 
        if (CargoIsReleased) {
            Motor_1->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Pitch * PITCH_COEF_NO_CARGO);
            Motor_2->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Pitch * PITCH_COEF_NO_CARGO);
            Motor_3->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Pitch * PITCH_COEF_NO_CARGO);
            Motor_4->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Pitch * PITCH_COEF_NO_CARGO);
            Motor_5->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Roll * ROLL_COEF_NO_CARGO);
            Motor_6->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_NO_CARGO + Output_Roll * ROLL_COEF_NO_CARGO);
        }
        else {
            Motor_1->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO + Output_Pitch * PITCH_COEF_CARGO);
            Motor_2->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO + Output_Pitch * PITCH_COEF_CARGO);
            Motor_3->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO + Output_Pitch * PITCH_COEF_CARGO);
            Motor_4->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO + Output_Pitch * PITCH_COEF_CARGO);
            Motor_5->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO);
            Motor_6->SetThrottle(Input_Throttle + Output_Yaw * YAW_COEF_CARGO + Output_Roll * ROLL_COEF_CARGO);
        }
    }
}

void System::DebugMain() {
    
    //Run system wide update.
    UpdateSystem();

    //Get the amount of time since last frame.
    DeltaTime = CoreTimer.DeltaTimer.GetTime_US();
    CoreTimer.DeltaTimer.SetClock(0,0,0);
    
    //Retrieve commands received through USB bus.
    const unsigned char * Command = ReceiveCommand();
    
    if (Command != 0) {
        //Execute received commands.
        ExecuteCommand(Command);
        
    }
    usb_arm_out_endpoint(2);
}

void System::GoToState(UnsignedInteger16 State) {
    this->State = State;
}

//<editor-fold defaultstate="collapsed" desc="USB command handlers">
    
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
    
    if (strcmp((const char*)Command, "GetOrientation\n") == 0) { 
        Command_GetOrientation();
    }
    else if (strcmp((const char*)Command, "GetPressure\n") == 0) { 
        Command_GetPressure();
    }
    else if (strcmp((const char*)Command, "GetStartingPressure\n") == 0) { 
        Command_GetStartingPressure();
    }
    else if (strcmp((const char*)Command, "GetTemperature\n") == 0) { 
        Command_GetTemperature();
    }
    else if (strcmp((const char*)Command, "GetAltitude\n") == 0) { 
        Command_GetAltitude();
    }
    else if (strcmp((const char*)Command, "SetThrottle\n") == 0) { 
        Command_GetThrottle((char *)Command);
    }
    else if (strcmp((const char*)Command, "GetThrottle\n") == 0) { 
        Command_GetThrottle((char *)Command);
    }
    else if (strcmp((const char*)Command, "StopAllMotors\n") == 0) { 
        Command_StopAllMotors();
    }
    else if (strcmp((const char*)Command, "SetAllMotors\n") == 0) { 
        Command_SetAllMotors((char *)Command);
    }
    else if (strstr((const char*)Command, "ReleaseCargo\n") != 0) { 
        Command_ReleaseCargo(); 
    }
    else if (strstr((const char*)Command, "HoldCargo\n") != 0) { 
        Command_HoldCargo();
    }
    else if (strcmp((const char*)Command, "ReturnToStandby\n") == 0) { 
        Command_ReturnToStandby();
    }
    else if (strstr((const char*)Command, "SelfTest\n") != 0) { 
        Command_USBTest();
    }
    
    return true;
}

bool System::SendUSBData(std::string Message) {
    
    if (usb_is_configured() &&
        !usb_in_endpoint_halted(2) &&
        !usb_in_endpoint_busy(2)) {
        
        char * cMessage = (char*)usb_get_in_buffer(2);
        strcpy(cMessage, Message.c_str());
        
        usb_send_in_buffer(2, Message.length());
        return true;
    }
    else {
        return false;
    }
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Control Algorithm">

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

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Console Commands">

bool System::Command_GetOrientation() {
    
    AHRS_Update();
    FloatBuffer fbuf;
    
    for (int i = 0; i < 4; i++) {
       
        fbuf.a = CurrentOrientation[i];
        char * buf = (char*)fbuf.b;
        SendUSBData(buf);
    }
    
    return true;
}

bool System::Command_GetPressure() {
    
    char * pressure = 0;
    sprintf(pressure, "%f", _Altimeter->GetPressure());
    SendUSBData(pressure);
    return true;
}

bool System::Command_GetStartingPressure() {
    
    char * sp = 0;
    sprintf(sp, "%f", _Altimeter->GetStartingPressure());
    SendUSBData(sp);
    return true;
}

bool System::Command_GetTemperature() {
    
    char * temp = 0;
    sprintf(temp, "%f", _Altimeter->GetTemperature());
    SendUSBData(temp);
    return true;
}

bool System::Command_GetAltitude() {
    
    char * altitude = 0;
    sprintf(altitude, "%f", _Altimeter->GetAltitude());
    SendUSBData(altitude);
    return true;
}

bool System::Command_SetThrottle(char * Command) {
    
}

bool System::Command_GetThrottle(char * Command) {
    
    return true;
}

bool System::Command_StopAllMotors() {
    
    SendUSBData("Stopping all motors.\n");
    Motor_1->StopMotor();
    Motor_2->StopMotor();
    Motor_3->StopMotor();
    Motor_4->StopMotor();
    Motor_5->StopMotor();
    Motor_6->StopMotor();
    
    return true;
}

bool System::Command_SetAllMotors(char * Command) {
    

}

bool System::Command_ReleaseCargo() {
    
    CargoIsReleased = true;
    
    #if defined(__32MX270F256D__)
        PORTSetBits(IOPORT_C, BIT_4);
        PORTClearBits(IOPORT_C, BIT_5);
    #elif defined(__32MX270F256B__)
        PORTSetBits(IOPORT_A, BIT_0);
        PORTClearBits(IOPORT_A, BIT_1);
    #endif

    Time Delaytimer;
    Delaytimer.SetClock(2, 0, 0);
    CoreTimer.Delay(Delaytimer);
    
    #if defined(__32MX270F256D__)
        PORTClearBits(IOPORT_C, BIT_4);
        PORTClearBits(IOPORT_C, BIT_5);
    #elif defined(__32MX270F256B__)
        PORTClearBits(IOPORT_A, BIT_0);
        PORTClearBits(IOPORT_A, BIT_1);
    #endif
    

    return true;
}

bool System::Command_HoldCargo() {  
    
    #if defined(__32MX270F256D__)
        PORTClearBits(IOPORT_C, BIT_4);
        PORTSetBits(IOPORT_C, BIT_5);
    #elif defined(__32MX270F256B__)
        PORTClearBits(IOPORT_A, BIT_0);
        PORTSetBits(IOPORT_A, BIT_1);
    #endif
    
    Time Delaytimer;
    Delaytimer.SetClock(2, 0, 0);
    CoreTimer.Delay(Delaytimer);
    
    #if defined(__32MX270F256D__)
        PORTClearBits(IOPORT_C, BIT_4);
        PORTClearBits(IOPORT_C, BIT_5);
    #elif defined(__32MX270F256B__)
        PORTClearBits(IOPORT_A, BIT_0);
        PORTClearBits(IOPORT_A, BIT_1);
    #endif
    
    CargoIsReleased = false;
    
    return true;
}

bool System::Command_ReturnToStandby() {
    GoToState(States::Standby);
    SendUSBData("Going to standby.\n");
    return true;
}

bool System::Command_USBTest() {
    
    SendUSBData("There shall be wings!\n");
    return true;
}

bool System::Command_GetRCInput(char * Command) {
    
    char * strInput_Roll = 0;
    char * strInput_Pitch = 0;
    char * strInput_Yaw = 0;
    char * strInput_Throttle = 0;
    char * strInput_Cargo = 0;
    
    sprintf(strInput_Roll, "%f", Input_Roll);
    sprintf(strInput_Pitch, "%f", Input_Pitch);
    sprintf(strInput_Yaw, "%f", Input_Yaw);
    sprintf(strInput_Throttle, "%f", Input_Throttle);
    sprintf(strInput_Cargo, "%f", Input_Cargo);
    
    char * channel = strstr(Command, "C:");
    if (channel != 0 ) {
        if      (channel[2] == '1') { SendUSBData(strInput_Roll); }
        else if (channel[2] == '2') { SendUSBData(strInput_Pitch); }
        else if (channel[2] == '3') { SendUSBData(strInput_Yaw); }
        else if (channel[2] == '4') { SendUSBData(strInput_Throttle); }
        else if (channel[2] == '5') { SendUSBData(strInput_Throttle); }
        else                        { SendUSBData(strInput_Cargo); }
    }
    
    return true;
}

bool System::Command_GetRoll() {
    
    char * roll = 0;
    sprintf(roll, "%f", CurrentOrientation.GetRoll());
    SendUSBData(roll);
    
    return true;
}

bool System::Command_GetPitch() {
    
    char * pitch = 0;
    sprintf(pitch, "%f", CurrentOrientation.GetPitch());
    SendUSBData(pitch);
    
    return true;
}

bool System::Command_GetYaw() {
    
    char * yaw = 0;
    sprintf(yaw, "%f", CurrentOrientation.GetYaw());
    SendUSBData(yaw);
    
    return true;
}

//</editor-fold>