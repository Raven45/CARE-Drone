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
    
    //Create device objects
    Devices.push_back(new HAL::Gyroscope        (ADDRESS_GYRO, &DeviceManager));
    Devices.push_back(new HAL::Accelerometer    (ADDRESS_GYRO, &DeviceManager));
    Devices.push_back(new HAL::Magnetometer     (ADDRESS_GYRO, &DeviceManager));
    Devices.push_back(new HAL::Altimeter        (ADDRESS_BAROMETER, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_1, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_2, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_3, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_4, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_5, &DeviceManager));
    Devices.push_back(new HAL::PWMC             (ADDRESS_MOTOR_6, &DeviceManager));
    
    //Initialize the usb bus.
    usb_init();
    
    //Initialize devices
    for (unsigned int i = 0; i < Devices.size(); i++) {
        Devices[i]->Initialize();
    }
    
    return true;
}

bool System::UpdateSystem() {
    
    //Update devices
    for (unsigned int i = 0; i < Devices.size(); i++) {
        Devices[i]->Update();
    }
}

void System::Main() {
    
    if (State == States::Debug) {
        DebugMain();
    }
    
    else if (State == States::Run) {
        RunMain();
    }
    
    else if (State == States::Standby) {
        StandbyMain();
    }
}

bool System::ClearToProceed() {
    return true;
}

bool System::IsUSBAttached() {
    return false;
}

#ifdef SYSTEM_IS_SINGLETON
System* System::GetInstance() {
    
    if (!Instance) {
        
        try{
            Instance = new System();
        }
        catch (std::bad_alloc) {
            return 0;
        }
    }
    return Instance;
}
#endif

void System::StandbyMain() {
    
    UpdateSystem();
    
    if (IsUSBAttached()) {
        //GoToState(States::Debug);
    }
    else {
        
        if (Safety /*&& Throttle == 0*/) {
            Safety = false;
        }
        else if (!Safety /*&& Throttle > 0*/) {
            //GoToState(States::Standby);
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
    
    UpdateSystem();
    
    if (HAL::Timer::GetInstance()->TimerList[0].GetTime_US() > 1100) {
        Input_Roll = (((HAL::Timer::GetInstance()->TimerList[0].GetTime_US() - 1000)/1000)*30.0f)-15.0f;
    }
    
    if (HAL::Timer::GetInstance()->TimerList[1].GetTime_US() > 1100) {
        Input_Pitch = (((HAL::Timer::GetInstance()->TimerList[1].GetTime_US() - 1000)/1000)*30.0f)-15.0f;
    }
    
    if (HAL::Timer::GetInstance()->TimerList[2].GetTime_US() > 1100) {
        Input_Yaw = (((HAL::Timer::GetInstance()->TimerList[2].GetTime_US() - 1000)/1000)*360.0f);
    }
    
    if (HAL::Timer::GetInstance()->TimerList[3].GetTime_US() > 1100) {
        Input_Throttle = (((HAL::Timer::GetInstance()->TimerList[3].GetTime_US() - 1000)/1000)*100.0f);
    }
    
    //This is probably where we want to set the output compare channel for the 
    //radio output. Still need to figure out how we're going to achieve this.
    
    //Get the set point for our PID controller.
    Math::Quaternion SetPoint(Input_Roll, Input_Pitch, Input_Yaw);
    
    //Get the amount of time since last frame.
    DeltaTime = HAL::Timer::GetInstance()->TimerList[4].GetTime_US();
    HAL::Timer::GetInstance()->TimerList[4].SetClock(0,0,0);
    
    //Get the current orientation.
    //CurrentOrientation = AHRS_Update(  );
    
    //Calculate the error
    Math::Quaternion Error = SetPoint*CurrentOrientation;
    
    //Calculate our output
    Math::Quaternion Output = CalculatePID(Error);
    //Output_Roll = Output.GetRoll();
    //Output_Pitch = Output.GetPitch();
    //Output_Yaw = Output.GetYaw();
    
    //Calculate what throttles we need. This is where Matt's control
    //algorithm is utilized. 
    //Motor1.SetThrottle();
    //Motor2.SetThrottle();
    //Motor3.SetThrottle();
    //Motor4.SetThrottle();
    //Motor5.SetThrottle();
    //Motor6.SetThrottle();
}

void System::DebugMain() {
    
    UpdateSystem();
    const unsigned char * Command = ReceiveCommand();
    ExecuteCommand(Command);
}


void System::GoToState(UnsignedInteger16 State) {
    this->State = State;
}
    
bool System::CreateDevice(ADDRESS, short int Type) {
    
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
    else if (strCommand.rfind("GetGravity") != std::string::npos) { 
        Command_GetGravity();
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
    
    if (Command.rfind("/X") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Y") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Z") != std::string::npos) {
            //Send X axis gyro reading
    }
}

bool System::Command_ReadAccelerometer(std::string Command) {
    
    if (Command.rfind("/X") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Y") != std::string::npos) {
            //Send X axis gyro reading
    }
    if (Command.rfind("/Z") != std::string::npos) {
            //Send X axis gyro reading
    }
}

bool System::Command_GetGravity() {
    
}

bool System::Command_GetPressure() {
    
}

bool System::Command_GetTemperature() {
    
}

bool System::Command_GetAltitude() {
    
}

bool System::Command_SetThrottle(std::string Command) {
    
    if (Command.rfind("/C:") != std::string::npos) {
            //Get channel.
    }
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_GetThrottle(std::string Command) {
    
    if (Command.rfind("/C:") != std::string::npos) {
            //Get channel.
    }
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_StopAllMotors() {
    
}

bool System::Command_SetAllMotors(std::string Command) {
    
    if (Command.rfind("/D:") != std::string::npos) {
            //Get throttle and then set the throttle.
    }
}

bool System::Command_ReleaseCargo() {
    
    PORTSetBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
    
    //delay()
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
}

bool System::Command_HoldCargo() {
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTSetBits(IOPORT_C, BIT_5);
    
    //delay()
    
    PORTClearBits(IOPORT_C, BIT_4);
    PORTClearBits(IOPORT_C, BIT_5);
}

bool System::Command_ReturnToStandby() {
    GoToState(States::Standby);
}