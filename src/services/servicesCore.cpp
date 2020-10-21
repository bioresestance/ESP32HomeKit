#include "servicesCore.h"

namespace Service
{
    void ServiceCore::serviceCoreSetup() 
    {

        serviceQueueHandle = eventCore.createEventQueue();
        // Register the service with the event core.  
        eventCore.registerList(serviceQueueHandle, eventSubListGet());
    }


    void ServiceCore::taskMain() {

        // Run the base setup for this service.
        serviceCoreSetup();

        // Run the services setup routine.
        initialize();

        Event::eventMessage *message = nullptr;
        while (true) {

            // Check if there are any messages for this service.
            if(eventCore.getEvent( serviceQueueHandle,  &message, getNextTimeout())) {
                // If there is, let the service handle it.
                handleEvent(*message);
                // Clean up memory after its used.
                delete message;
            } else {
                // Timeout event handler.
                handleTimeout();
            }
            // Generic loop handler.
            handleLoop();
        }
    }

} // namespace Service