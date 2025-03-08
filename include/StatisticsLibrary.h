#ifndef STATISTICSLIBRARY_H
#define STATISTICSLIBRARY_H
#include <string>
#include <vector>

typedef int statid_t;

class StatisticsLibrary {
    public:
        StatisticsLibrary();
        ~StatisticsLibrary();

        statid_t GenerateNewIdFromName(std::string name);
        void Publish(statid_t id, int* target);
        void Publish(statid_t id, double* target);
        void Publish(statid_t id, int (*getter)());
        void Publish(statid_t id, double (*getter)());

        statid_t FindIdFromName(std::string name); // if statid_t == 0, name not generated yet
        int ReadIntFromId(statid_t id);
        double ReadDoubleFromId(statid_t id);

        void Unpublish(statid_t id);
    private:
        enum AccessMethod {
            INVALID = 0, // the invalid AccessorData element is at index 0
            UNPOPULATED,
            INT_POINTER,
            DOUBLE_POINTER,
            INT_GETTER,
            DOUBLE_GETTER
        };
        struct AccessorData {
            void* pointer;
            AccessMethod method;
            std::string name;
        };
        std::vector<AccessorData> stats;
};

#endif // STATISTICSLIBRARY_H
