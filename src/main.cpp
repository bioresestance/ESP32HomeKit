#include <services/servicesCore.h>
#include <esp_log.h>

#include <events/eventCore.h>


// static Service testServices[] = {
//     Service("Test Task 1"),
//     Service("Test Task 2")
// };


extern "C" void app_main() {
  
    while(1) {
        vTaskDelay(1000);
    }

}