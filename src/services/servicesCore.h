#pragma once

#include <freertosTask.h>
#include <freertos/queue.h>
//#include <events/eventCore.h>
#include <esp_log.h>
#include <array>
#include <utility>




/**
 * @brief Base Service Class. All service are based on this class.
 * 
 */
class Service : freeRTOS::Task {

private:

    QueueHandle_t serviceQueueHandle;

    //Event::EventCore& eventCore = Event::EventCore::getInstance();

    


public:
   
    /**
     * @brief Construct a new Service object with a name, stack size and priority level.
     * 
     * @param serviceName Name of the service.
     * @param stackSize  Size of the stack to assign the service, value in words (4 bytes).    
     * @param priority Priority level to give the service. Higher value is higher priority
     */
    Service( const char* serviceName, uint32_t stackSize, uint16_t priority ) 
    : freeRTOS::Task(serviceName, stackSize, priority )
    {
        // Start the task.
        this->start();
    }

    Service( const char* serviceName ) : Service(serviceName, defaultStackSize, defaultPriority)
    {
    }

    virtual void taskMain() override;

    /**
     * @brief Gets the list of events the service is subscribed to.
     * 
     * @return eventSubList List of Event ID's that the service is subscribed to.
     */
    //virtual std::pair<Event::EventID*, uint16_t> eventSubListGet() = 0;  

};




// /**
//  * @brief Class used to store a number of service objects. Size must be know at compile time.
//  * 
//  * @tparam numServices The number of services to store in the list. 
//  */
// template<size_t numServices>
// class ServiceList {

// private:

//     std::array<Service* , numServices> serviceArray;

// public:
//     ServiceList(std::array<Service* , numServices> services )
//     : serviceArray(services)
//     {
//     }
// };