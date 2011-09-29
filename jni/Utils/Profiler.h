#ifndef PROFILER_H
#define PROFILER_H

#include <Utils/EngineTypes.h>
#include <Utils/TextBuffer.h>
#include <Utils/Utils.h>
#include <Utils/RelativeClock.h>

#define PROFILE(name, manager) Profiler p(name, manager)


//! Hold informations about profile of code chunk
struct ProfileSample {
    bool valid;
    //! Count how many times given chunk of code was called during profile
    U16 profileInstances;
    //! Checks if code chunk was not called recursively.
    /*!
      There can be only one opened instance of chunk code during profiling.
      */
    U16 openedProfiles;
    //! Name of code chunk, displayed layter in profiler
    char name[255];

    //! Start time of chunk executing
    float startTime;

    //! Accumulates execution time of all instances of code chunk
    F32 accumulator;

    //! Accumulates execution time of all profiled child code chunks of this chunk
    U16 childrenSampleTime;

    //! Store number of parent chunks, used to correcty format profiler output
    U16 numParents;
};


//! Stores time information about profiler sample
struct ProfileSampleHistory {
    bool valid;
    //! Name of sample
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


//! Manages all profiles
/*!
  Allows to easy enable/disable all profiles.
  Formats output of profilers.
  */
class ProfilerManager {
    bool enabled;

    //! Information about totat times of all profiles
    ProfileSampleHistory total;

    float startTime;

    float stopTime;

public:
    ProfilerManager();

    //! Indicate if profilers are enabled
    static bool profilerEnabled;

    //! Information about profile samples
    ProfileSample samples[MAX_PROFILE_SAMPLES];

    //! Information about profile samples times
    ProfileSampleHistory samplesHistory[MAX_PROFILE_SAMPLES];

    RelativeClock clock;

    TextBuffer outputBuffer;

    //! Dumps profile data to output text buffer
    void DumpProfileDataToBuffer();
    //! Stores information of profile in history, updating it's time informations
    void StoreProfileInHistory(char* name, float percent, float dt);

    //! Gets informations of profile from history
    void GerProfileFromHistory(char* name, float*avgPercent, float* minPercent, float* maxPercent,
                                      float*avgTime, float*minTime, float*maxTime);
};

//! Profiles chunk of code
class Profiler
{
    ProfilerManager* manager;
    char* name;
public:
    //! Starts profiler
    /*!
      Profiled chunk of code should surronded by additional { } eg.
      {
        Profiler("Test code");
        ...
      }

      */
    Profiler(const char* name, ProfilerManager* manager);
    //! Ends profiler and calculates statistics
    ~Profiler();
};




#endif // PROFILER_H
