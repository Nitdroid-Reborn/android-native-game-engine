#ifndef PROFILER_H
#define PROFILER_H

#include <Utils/EngineTypes.h>
#include <Utils/TextBuffer.h>
#include <Utils/Utils.h>
#include <Utils/RelativeClock.h>

#define PROFILE(name, manager) Profiler p(name, manager)



struct ProfileSample {
    bool valid;
    U16 profileInstances;
    U16 openedProfiles;
    char name[255];
    float startTime;
    F32 accumulator;
    U16 childrenSampleTime;
    U16 numParents;
};

struct ProfileSampleHistory {
    bool valid;
    char name[255];
    F32 partialSumPercent;
    F32 partialSumTime;
    U32 numSamples;
    F32 avgPercent;
    F32 minPercent;
    F32 maxPercent;
    F32 avgTime;
    F32 minTime;
    F32 maxTime;
};


#define MAX_PROFILE_SAMPLES 50



class ProfilerManager {
    bool enabled;
public:
    ProfilerManager();

    static bool profilerEnabled;
    ProfileSample samples[MAX_PROFILE_SAMPLES];
    ProfileSampleHistory samplesHistory[MAX_PROFILE_SAMPLES];
    ProfileSampleHistory total;
    float startTime;
    float stopTime;
    RelativeClock clock;

    TextBuffer outputBuffer;
    void DumpProfileDataToBuffer();
    void StoreProfileInHistory(char* name, float percent, float dt);
    void GerProfileFromHistory(char* name, float*avgPercent, float* minPercent, float* maxPercent,
                                      float*avgTime, float*minTime, float*maxTime);
};

class Profiler
{
    ProfilerManager* manager;
    char* name;
    bool init;
public:
    Profiler(const char* name, ProfilerManager* manager);
    ~Profiler();
};




#endif // PROFILER_H
