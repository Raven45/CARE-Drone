/*******************************************************************************
 * Copyright 2016 Department of Engineering, Harding University

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


 * Created on January 11, 2016, 10:27 AM
*******************************************************************************/


#include "System.h"
#include "Quaternion.h"


int main(int argc, char** argv) {
    
    Math::Quaternion Test(0.75f, 0.01, 0.33f);
    Math::Quaternion A(1, 0, 0, 0);
    Test = Test * A;
    float Roll = Test.GetRoll();
    float Pitch = Test.GetPitch();
    float Yaw = Test.GetYaw();
    
    //Create system object and attempt to initialize system.
    while ( System::GetInstance()->InitializeSystem() == false ) {
        //error condition.
    }
    
    //While system is running.
    while (System::GetInstance()->ClearToProceed()) {
        
        //Call the real main function.
        System::GetInstance()->Main();
    }
    
    return 0;
}

