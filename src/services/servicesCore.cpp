#include "servicesCore.h"

namespace Service
{
    void ServiceCore::serviceCoreSetup() 
    {


       // auto[eventList, numEvents] = eventSubListGet();
        
        //eventCore.registerList(serviceQueueHandle, get)


    }


    void ServiceCore::taskMain() {

        // Run the base setup for this service.
        serviceCoreSetup();

        // Run the services setup routine.
        initialize();

        Event::eventMessage message;
        while (true) {

            // Check if there are any messages for this service.
            if(eventCore.getEvent( serviceQueueHandle,  &message, 1000)) {
                // If there is, let the service handle it. Make sure to clean up memory too.
                handleEvent(message);
                eventCore.disposeEvent(&message);
            } else {
                handleTimeout();
            }

            handleLoop();
        }
    }

} // namespace Service