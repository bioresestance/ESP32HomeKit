#pragma once


namespace Event {

    enum class EventID 
    {
        NO_EVENT,           //!< Default value to indicate there is no message.
        KEEP_ALIVE,         //!< Send to a service periodically to see if it is still functional.
        KEEP_ALIVE_RESP,    //!< Response to a keep alive message. Response payload should be the Tasks ID.





        



        NUM_EVENTS          //!< Always at the end of the enum. Used to indicate the number of event ID's.
    };

    struct EventIDList
    {

    private:
        EventID *_list;
        uint8_t _num_in_list;

    public:
        /**
         * @brief Construct a new Event I D List object
         * 
         * @param list List of event IDs to add to object.
         * @param num_in_list Number of event IDs in list. 
         */
        EventIDList(EventID list[], uint8_t num_in_list)
        {

            _num_in_list = num_in_list;

            // Create new list and copy to local buffer.
            _list = new EventID[_num_in_list];
            memcpy(_list, list, sizeof(EventID[_num_in_list]));
        }

        EventIDList() {
            _num_in_list = 0;
            _list = nullptr;
        }

        /**
         * @brief Destroy the EventID List object
         * 
         */
        ~EventIDList()
        {
            delete _list;
        }

        /**
         * @brief Get the List of Event IDs
         * 
         * @return const EventID* Pointer to internal list.
         */
        const EventID *getList()
        {
            return _list;
        }

        /**
         * @brief Get the number of items in the list.
         * 
         * @return uint8_t Number of items in the list.
         */
        uint8_t getListSize()
        {
            return _num_in_list;
        }
    };
}