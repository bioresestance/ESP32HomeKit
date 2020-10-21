#include <services/servicesCore.h>
#include <esp_log.h>
#include <services/testService.h>
#include <events/eventCore.h>


// static Service testServices[] = {
//     Service("Test Task 1"),
//     Service("Test Task 2")
// };
static Service::TestService service;

extern "C" void app_main() {

    while(1) {
        ESP_LOGI("MAIN", "Main!");
        vTaskDelay(100);
    }

}