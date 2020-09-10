#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertosTask.h>
#include <events/eventID.h>
#include <services/servicesCore.h>
#include <array>

namespace Event
{
    //! Defines the max number of subscribers each event ID can have. 
    #define MAX_EVENT_SUBS      (10)


    /**
     * @brief Structure of a event message.
     * 
     */
    struct eventMessage
    {
        EventID eventID;            //!< ID of the event the message contains.
        uint32_t eventTime;         //!< Unix timestamp of the event.
        uint32_t eventDataLength;   //!< Length of the event data. 0 for no data.
        void *eventData;            //!< Optional data to attach to the event.
    };


    /**
     * @brief Array of queue handles to hold a list of subcribers to a Event ID.
     */
    typedef std::array<QueueHandle_t, MAX_EVENT_SUBS> eventIdSubList;




    /**
     * @brief Main class to handle all events being passed around by services.
     * 
     *  Uses the Mayer Singleton design pattern. Only a single instance will ever exist.
     *  Each service will need to grab a reference to that instance to use it.
     * 
     */
    class EventCore {

    /****************************************************************************************************/
    /**
     * The Following block is to make this class a singleton and ensure only a single copy ever exists.
     */
    private:
        //! Private constructor. Ensures only a single copy is ever made.
        EventCore();
    public:
        static EventCore& getInstance() {
            // Using the Mayer singleton pattern. Only one instance exists.
            static EventCore eventCoreInstance;
            return eventCoreInstance;
        }

        // Ensure we cant copy or assign a new instance.
        EventCore(EventCore const&)       = delete;
        void operator=(EventCore const&)  = delete;
    /****************************************************************************************************/

    private:

        //! Array of Vectors that hold a list of who is subscribed to any Event. Every Event ID gets its own vector. This vector contains all queues subscribed to that ID.
        std::array<eventIdSubList, (size_t)EventID::NUM_EVENTS> eventSubList;


        /**
         * @brief Resets the entire Event list for each Event ID.
         * 
         */
        void resetList();

        /**
         * @brief Resets the event list for only the specified Event ID.
         * 
         * @param id 
         */
        void resetList(EventID id);

        /**
         * @brief Checks whether the queue handle is already registered to the Event ID.
         * 
         * @param handle Queue handle to check.
         * @param event Event ID to check.
         * @return true Handle already registered.
         * @return false Handle not registered.
         */
        bool isQueueRegistered(QueueHandle_t& handle, EventID event); 

        /**
         * @brief Adds the Queue handle to the event ID list.
         * 
         * @param handle Queue handle to add to list.
         * @param event Event ID to add the queue to.
         * @return true Handle was added.
         * @return false Handle was not added. Either no room, or already exists in list.
         */
        bool addQueuetoList(QueueHandle_t& handle, EventID event);

    public:

        /**
         * @brief Registers a list of Event ID's to a queue.
         * 
         * @param handle Handle to the queue to register the list to.
         * @param eventIdList Pointer to a list of Event ID's.
         * @param numEventId  Number of event ID's in the list.
         * @return Whether the registration was successful or not. 
         */
        bool registerList(QueueHandle_t& handle, EventID* eventIdList, uint16_t numEventId);

 

    };

} // namespace event
