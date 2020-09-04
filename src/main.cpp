
#include <iostream>
#include <service.h>
#include <esp_log.h>
#include "string.h"

using namespace std;



static Service testServices[] = {
    Service("Test Task 1"),
    Service("Test Task 2")
};




ServiceList<sizeof(testServices)/sizeof(Service)> services(testServices);


extern "C" void app_main() {
  
    
    cout << "Starting Test!" << endl;


    while(1) {
        vTaskDelay(1000);
    }

}