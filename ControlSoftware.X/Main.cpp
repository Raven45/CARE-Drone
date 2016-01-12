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

/*
 * 
 */
int main(int argc, char** argv) {

    
    //Create system object and attempt to initialize system.
    System CAREDrone;
    while ( CAREDrone.InitializeSystem() == false ) {
        //error condition.
    }
    
    //While system is running.
    while (CAREDrone.ClearToProceed()) {
        
        //Call the real main function.
        CAREDrone.Main();
    }
    
    return 0;
}
