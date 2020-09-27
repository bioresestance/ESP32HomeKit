#include <events/eventCore.h>


namespace Event
{

    EventCore::EventCore() {
        resetList();
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
    

} // namespace event