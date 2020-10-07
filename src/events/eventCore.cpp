#include <events/eventCore.h>


namespace Event
{

    EventCore::EventCore() {
        resetList();
        eventItemList.clear();
    }
    
    QueueHandle_t EventCore::createEventQueue() 
    {
        return xQueueCreate(20, sizeof(eventItem*));
    }

    void EventCore::resetList() {
        // Reset the event list for each Event ID.
        for(uint32_t eventID = (uint32_t) EventID::NO_EVENT; eventID < (uint32_t) EventID::NUM_EVENTS; eventID++) {
            resetList( static_cast<EventID>(eventID) );
        }
    }

    void EventCore::resetList(EventID id) {
        // Get the list for the event id and set all handles to null.
        eventSubList[(uint32_t)id].clear();
    }
    
    bool EventCore::isQueueRegistered(QueueHandle_t& handle, EventID event) {
        // Gets the list for the given event.
        eventIdSubList& list = eventSubList[static_cast<uint16_t>(event)];

        // search through the list to see if the queue is registered with the event.
        for(auto& listHandle : list) {
            if(listHandle == handle) return true;
        }

        return false;
    }
    
    bool EventCore::getNextFreeIndex(eventIdSubList& list, uint16_t* idx) {

        uint16_t i = 0;
        for(auto& handle : list) {
            // Search the array for the first available spot.
            if(handle == nullptr) {
                *idx = i;
                return true;
            }
            i++;
        }
        return false;        
    }
    
    bool EventCore::addQueuetoList(QueueHandle_t& handle, EventID event) {

        bool retVal = false;
        eventIdSubList& list = eventSubList[static_cast<uint16_t>(event)];

        // First ensure the queue is not already in the list.
        if(!isQueueRegistered(handle, event)) {
               list.push_back(&list);
               retVal = true;
        } 

        return retVal;
    }
    
    bool EventCore::removeEventItemSubscriber(eventItem* item) {
        item->subscriberRelease();
        if(item->isReleased()) {
            eventItemList.remove(item);
            delete item;
            return false;
        }    
        return true;
    }
    

    bool EventCore::registerList(QueueHandle_t& handle, const EventID* eventIdList, uint8_t numEventId) {
        assert(handle);
        assert(eventIdList);
        assert(!(eventIdList == nullptr and numEventId > 0)); // If list is null, but number indicates not 0.


        for(uint16_t idx = 0; idx < numEventId; idx++) {
            EventID id = eventIdList[idx];
            addQueuetoList(handle, id);
        }
        return true;
    }
    
    bool EventCore::registerList(QueueHandle_t& handle, EventIDList& eventIdList) 
    {
        assert(handle);
        return registerList(handle, eventIdList.getList(), eventIdList.getListSize());
    }
    
    bool EventCore::postEvent(EventID event, void* payload, uint16_t payloadLength) {
        
        assert(event < EventID::NUM_EVENTS);
        // See if null pointer when there should be something.
        assert(!(payload == nullptr && payloadLength > 0));

        // Get the list of subscribers to the given event.
        eventIdSubList& list = eventSubList[static_cast<uint16_t>(event)];

        // See if there are any subscribers to post to.
        if(list.size() > 0) {
            //! Create our new item that contains the event.
            eventItem *item = new eventItem(event, payload, payloadLength, list.size());
            
            // Add the item to the list to keep track of all created items.
            eventItemList.push_back(item);

            // Send the item to all queues subscribed to the event.
            for(auto& serviceQ : list) {
                if(xQueueSend(serviceQ, &item, 0) == pdFALSE) {
                    // Failed to send to queue, remove subscriber.
                    removeEventItemSubscriber(item);
                }
            }
        } else {
            return false;
        }
        return true;
    }
    
    bool EventCore::postEvent(EventID event) 
    {
        return postEvent(event, nullptr, 0);
    }
    
    // bool EventCore::getEvent(QueueHandle_t& queue, const eventMessage *const eventMsg, uint32_t msToWait) 
    // {
    //     eventItem *item;
    //     if(xQueueReceive(queue, &item, msToWait) == pdTRUE) {

    //     }
    // }
    

} // namespace event