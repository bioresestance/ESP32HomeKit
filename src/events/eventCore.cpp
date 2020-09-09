#include <events/eventCore.h>

namespace event
{

    EventCore::EventCore()
    {
        resetList();
    }

    void EventCore::resetList()
    {
        // Reset the event list for each Event ID.
        for(uint32_t eventID = (uint32_t) EventID::NO_EVENT; eventID < (uint32_t) EventID::NUM_EVENTS; eventID++) {
            resetList( static_cast<EventID>(eventID) );
        }
    }

    void EventCore::resetList(EventID id) {
        // Get the list for the event id and set all handles to null.
        eventIdSubList &list = eventSubList[(uint32_t)id];
        for (QueueHandle_t &handle : list) {
            handle = nullptr;
        }
    }

} // namespace event