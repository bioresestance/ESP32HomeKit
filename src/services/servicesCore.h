#pragma once

#include <freertosTask.h>
#include <freertos/queue.h>
#include <events/eventCore.h>
#include <esp_log.h>
#include <array>
#include <utility>

namespace Service
{

    /**
 * @brief Base Service Class. All service are based on this class.
 * 
 */
    class ServiceCore : freeRTOS::Task
    {

    private:
        //! The queue handle for the service. Used to access all events the service subscribes to.
        QueueHandle_t serviceQueueHandle;
        //! Reference to the Event core. Used to post and get events.
        Event::EventCore &eventCore = Event::EventCore::getInstance();



        /**
         * @brief Internal setup for the service.
         */
        void serviceCoreSetup();

    public:


        /**
         * @brief Construct a new ServiceCore object with a name, stack size and priority level.
         * 
         * @param serviceName Name of the service.
         * @param stackSize  Size of the stack to assign the service, value in words (4 bytes).    
         * @param priority Priority level to give the service. Higher value is higher priority
         */
        ServiceCore(const char *serviceName, uint32_t stackSize, uint16_t priority)
            : freeRTOS::Task(serviceName, stackSize, priority)
        {
            

            // Start the task.
            this->start();
        }



        /**
         * @brief Construct a new ServiceCore object with only a name. Used default memory and priority.
         * 
         * @param serviceName 
         */
        ServiceCore(const char *serviceName) : ServiceCore(serviceName, defaultStackSize, defaultPriority)
        {
        }



        /**
         * @brief Main Loop for the service.
         * 
         */
        virtual void taskMain() override;



        /**
         * @brief Gets the list of events the service is subscribed to.
         * 
         * @return A pair type containing a list of event id's and the number of those id's
         */
        virtual Event::EventIDList& eventSubListGet() = 0;



        /**
         * @brief Runs any setup that the service may need to perform.
         */
        virtual void initialize() = 0;



        /**
         * @brief Gets the next timeout tick.
         * 
         * @return TickType_t The value of when the service should wait until.
         */
        virtual TickType_t getNextTimeout() = 0;



        /**
         * @brief Gets called whenever there is a new event to handle for the servie.
         * 
         * @param event The newly received event message.
         */
        virtual void handleEvent(Event::eventMessage& event) = 0;

        /**
         * @brief Allows the service to do something in the event of a timeout.
         * 
         *  Base function is empty. If a service wants to use it, it can override it.
         */
        virtual void handleTimeout() {}


        /**
         * @brief Allows the service to do something after each loop.
         * 
         * 
         * This function gets called after an event is received or a timeout.
         *  Currently does nothing, but can be overriden in service if needed.
         * 
         */
        virtual void handleLoop() {}

    };
} // namespace Service