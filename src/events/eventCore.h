#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertosTask.h>
#include <events/eventID.h>
#include <list>
#include <vector>
#include <array>

namespace Event
{

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

        eventMessage(EventID eventId, void* message, uint16_t messageLength) 
        :eventID(eventId),
        eventTime(freeRTOS::Task::getCurrentTimeMs()),
        eventDataLength(messageLength)
        {
            if(eventDataLength > 0) {
                // Allocate the block of memory to use with the message.
                eventData = malloc(eventDataLength);
                memcpy(eventData, message, messageLength);
            } else {
                // No payload, ensure its set to null.
                eventData = nullptr;
            } 
        }

        // Copy Constructor.
        eventMessage(const eventMessage& msg) 
        : eventID(msg.eventID), eventTime(msg.eventTime), eventDataLength(msg.eventDataLength), eventData(nullptr) {
            if( eventDataLength > 0 ) {
                // create a new block of memory and make a copy of the event data.
                eventData = malloc(eventDataLength);
                memcpy(eventData, msg.eventData, eventDataLength);
            }            
        }

        ~eventMessage() {
            if(eventData != nullptr) {
                free(eventData);
            }  
        }
    };

    /**
     * @brief List of queue handles to hold a list of subcribers to a Event ID.
     */
    typedef std::vector<QueueHandle_t> eventIdSubList;


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

        // Ensure we can't copy or assign a new instance.
        EventCore(EventCore const&)       = delete;
        void operator=(EventCore const&)  = delete;
    /****************************************************************************************************/

    private:

        /**
         * @brief Used to store an event item and all the subscribers to it.
         */
        struct eventItem {
            uint16_t numSubscribers;//!< Number of subscribers to the event.
            uint16_t numReleased;   //!< Number of subscriber who have released the event.
            eventMessage *msg;      //!< Pointer to allocated memory to hold the message.

            eventItem(EventID eventId, void* message, uint16_t msgLength, uint16_t numSub) 
            :   numSubscribers(numSub), numReleased(0) 
            {
                // Create the message from provided parameters.
                msg = new eventMessage(eventId, message, msgLength);
            }
            eventItem() = default;

            ~eventItem() 
            {
                // Free the memory for the message.
                delete msg;
            }

            /**
             * @brief Releases a subscriber from the list.
             */
            void subscriberRelease() {
                numReleased++;
            }

            /**
             * @brief Determines if the item can be released.
             * 
             * @return true  If all subscribers have released the item.
             * @return false If there is at least one subscriber not released.
             */
            bool isReleased() {
                return (numReleased >= numSubscribers);
            }

        };

        //! Array of Vectors that hold a list of who is subscribed to any Event. Every Event ID gets its own vector. This vector contains all queues subscribed to that ID.
        std::array<eventIdSubList, (size_t)EventID::NUM_EVENTS> eventSubList;

        //! List of event items currently in use.
        std::list<eventItem*> eventItemList;


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
        bool isQueueRegistered(QueueHandle_t handle, EventID event); 

        /**
         * @brief Get the Next Free Index in the provided list.
         * 
         * @param list List to search
         * @param idx  Holds the result of the search.
         * @return true If a spot was found. Result in idx.
         * @return false If no spot was found.
         */
        bool getNextFreeIndex(eventIdSubList& list, uint16_t* idx);

        /**
         * @brief Adds the Queue handle to the event ID list.
         * 
         * @param handle Queue handle to add to list.
         * @param event Event ID to add the queue to.
         * @return true Handle was added.
         * @return false Handle was not added. Either no room, or already exists in list.
         */
        bool addQueuetoList(QueueHandle_t handle, EventID event);

        /**
         * @brief Removes a subscriber of a posted Event Item.
         * 
         *  Removes a subscriber from a event item. If all subs are done
         *  the item will be removed from the master list and deleted.
         * 
         * @param item Event Item to remove subscriber from.
         * @return true If the item is still valid.
         * @return false If the item has been deleted
         */
        bool removeEventItemSubscriber(eventItem* item);

    public:

                /**
         * @brief Creates an event queue for a service to use.
         * 
         * @return QueueHandle_t Queue handle for the service to use.
         */
        QueueHandle_t createEventQueue();
        
        /**
         * @brief Registers a list of Event ID's to a queue.
         * 
         * @param handle Handle to the queue to register the list to.
         * @param eventIdList Pointer to a list of Event ID's.
         * @param numEventId  Number of event ID's in the list.
         * @return Whether the registration was successful or not. 
         */
        bool registerList(QueueHandle_t handle, const EventID* eventIdList, uint8_t numEventId);

        /**
         * @brief Registers a list of Event ID's to a queue.
         * 
         * @param handle Handle to the queue to register the list to.
         * @param eventIdList list of Event ID's.
         * @return Whether the registration was successful or not. 
         */
        bool registerList(QueueHandle_t handle, EventIDList& eventIdList);

        /**
         * @brief Post an Event with a payload.
         * 
         *  Posts a payload to an Event ID. The payload will be copied, so no need to 
         *  keep the memory in scope. 
         * 
         * @param event ID of the event to post.
         * @param payload Pointer to a payload to post with the event.
         * @param payloadLength Length of the payload.
         * @return true If the event was posted successfully.
         * @return false If there was a problem posting the message, such as no memory available.
         */
        bool postEvent(EventID event, void* payload, uint16_t payloadLength);

        /**
         * @brief Post an Event without a payload.
         * 
         * @param event ID of the event to post.
         * @return true If the post was successful.
         * @return false If the post failed.
         */
        bool postEvent(EventID event);


        /**
         * @brief Gets any available events for the service.
         * 
         *  Waits up to the provided amount of time for a event to arrive in the queue.
         *  When/if an event is available, a copy of the message is returned.
         * 
         * @param queue The queue to check for messages.
         * @param eventMsg Pointer to a event msg pointer. New message stored at pointer location.  
         * @param msToWait Max number of ms to wait for a new event.
         * @return true If a event was received in the wait period.
         * @return false If there was no event before the wait period was over.
         */
        bool getEvent(QueueHandle_t queue, eventMessage** eventMsg, uint32_t msToWait);

    };

} // namespace event
