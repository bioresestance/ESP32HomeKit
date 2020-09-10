
#include <iostream>
#include <services/servicesCore.h>
#include <esp_log.h>
#include "string.h"

#include <events/eventCore.h>

using namespace std;



// static Service testServices[] = {
//     Service("Test Task 1"),
//     Service("Test Task 2")
// };


extern "C" void app_main() {
  
    
    cout << "Starting Test!" << endl;


    while(1) {
        vTaskDelay(1000);
    }

}