
#include <freertosTask.h>
#include <esp_log.h>
#include <array>


class Service : freeRTOS::Task {

public:
   
    Service( const char* serviceName, uint32_t stackSize, uint16_t priority ) : freeRTOS::Task(serviceName, stackSize, priority )
    {
        // Start the task.
        this->start();
    }

    Service( const char* serviceName ) : Service(serviceName, defaultStackSize, defaultPriority)
    {
    }

    virtual void taskMain() override;


    void startService();
};


/**
 * @brief Class used to store a number of service objects. Size must be know at compile time.
 * 
 * @tparam numServices The number of services to store in the list. 
 */
template<size_t numServices>
class ServiceList {

private:

    std::array<Service* , numServices> serviceArray;

public:
    ServiceList(std::array<Service* , numServices> services )
    : serviceArray(services)
    {
    }
};