#ifndef MISSION_H
#define MISSION_H
#include <Game/structs.h>

class Mission {
    private:
        static inline Globals_status** globals_status = 0;
        struct CustomMission {
            int stationid;
            std::wstring description;
            int type;
            int enabled;
            int entered_mission;
        };
    public:
        static inline std::vector<CustomMission> created_missions;
        static inline int savedrealmissionid;
        static void init(void);
        static int getid(void);
        static void setid(int value);
        static int getcompletedsidemissions(void);
        static void setcompletedsidemissions(int value);
        static int create(int stationid, std::string description, int type);
        static void enable(int custom_missionid);
        static void disable(int custom_missionid);
        
        static void enablevalkyrie(void);
        static void nextcampaignmission();
};
#endif