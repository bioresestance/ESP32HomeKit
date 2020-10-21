#include <services/servicesCore.h>
#include <esp_log.h>

namespace Service {


    class TestService : ServiceCore {

        Event::EventID idList[1] = {Event::EventID::KEEP_ALIVE};
        Event::EventIDList* list;

        const char TAG[5] = "TEST";

        TickType_t nextTimeout;

    public:
        TestService() 
        : ServiceCore("TestService")
        {
            nextTimeout = freeRTOS::Task::getTickCount() + 100;
            ESP_LOGI(TAG, "Created Test Service");
        }

    private:
        Event::EventIDList& eventSubListGet() override {
            list = new Event::EventIDList(idList, 1);
            return *list;
        }


        void initialize() override {

        }


        TickType_t getNextTimeout() override {

            int32_t delta = nextTimeout - freeRTOS::Task::getTickCount();
            return (delta < 0) ? 0 : delta;
        }

        void handleEvent(Event::eventMessage& event) override {

            switch(event.eventID) {

                case Event::EventID::KEEP_ALIVE:
                    ESP_LOGI(TAG, "Got Keep Alive!!!!");
                    break;
                default:
                    break;
            }
        }

        void handleTimeout() override {
            nextTimeout = freeRTOS::Task::getTickCount() + 100;
            eventCore.postEvent(Event::EventID::KEEP_ALIVE);
            ESP_LOGI(TAG, "Timeout");
        }

    };
    
}