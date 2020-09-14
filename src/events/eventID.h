#pragma once


namespace Event {


    enum class EventID 
    {
        NO_EVENT,           //!< Default value to indicate there is no message.
        KEEP_ALIVE,         //!< Send to a service periodically to see if it is still functional.
        KEEP_ALIVE_RESP,    //!< Response to a keep alive message. Response payload should be the Tasks ID.





        



        NUM_EVENTS          //!< Always at the end of the enum. Used to indicate the number of event ID's.
    };
}