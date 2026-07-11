#ifndef STATION_H
#define STATION_H

class Station {
    private:
        static inline Globals_status** globals_status = 0;
        SingleStation *_ptr = nullptr;
        struct StationEditQueue {
            int id;
            int techlevel;
            std::string name;
            bool edittechlevel;
            bool editname;
        };
    public:
        Station();
        Station(SingleStation *ptr);
        ~Station();
        
        static inline std::vector<SingleStation> created_stations;
        static inline std::vector<StationEditQueue> editqueue;
        static void init(void);
        int getid(void);
        void setid(int value);
        std::string getname(void);
        void setname(const std::string value);
        int gettechlevel(void);
        void settechlevel(int value);
        static int gethangaritemscount(void);
        static void sethangaritemscount(int value);
        static int gethangarshipscount(void);
        static void sethangarshipscount(int value);
        static int getagentscount();
        static void setagentscount(int value);
        static bool isvoid(void);
        static void sethangarshipinfo(int id, sol::table shipinfo);
        static int create(const std::string& str, int techlevel, int textureid, int systemid);
        static std::string getagentname(int id);
        static int getagentfaction(int id);
        static void createagent(const std::string& str, int factiontype, int terranwoman, sol::table imageinfo, sol::table agentinfo);
        static void removehangaritem(int id);
        static bool hasiteminhangar(int id);
        static void addhangarship(int id);
        SingleStation *getstruct() const;
        static SingleStation *getstationbyid(int id);
};
#endif