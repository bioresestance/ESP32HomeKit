
#include "service.h"


void Service::taskMain() 
{
    while(true) {
        ESP_LOGI( this->_taskName, "Testing");
        this->delay(1000);
    }
}

void Service::startService() 
{
    
}
