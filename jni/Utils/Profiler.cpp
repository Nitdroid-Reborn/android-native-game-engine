#include "Profiler.h"
#include <string.h>
#include <Utils/Utils.h>

bool ProfilerManager::profilerEnabled = false;

Profiler::Profiler(const char* name, ProfilerManager*manager) {
    U16 i=0;
    this->manager = manager;

    if(manager->profilerEnabled) {
        init = true;
        while(i<MAX_PROFILE_SAMPLES && manager->samples[i].valid) {
            if(strcmp(name, manager->samples[i].name) == 0) {
                manager->samples[i].openedProfiles++;
                manager->samples[i].profileInstances++;
                manager->samples[i].startTime = manager->clock.GetTime();
                this->name = manager->samples[i].name;
                ASSERT(manager->samples[i].openedProfiles == 1, "Recursive call of profiled block");
                return;
            }
            i++;
        }

        if(i>=MAX_PROFILE_SAMPLES) {
            ASSERT(0, "Exceeded max profile samples");
            return;
        }

        strcpy(manager->samples[i].name, name);
        manager->samples[i].valid=true;
        manager->samples[i].openedProfiles = 1;
        manager->samples[i].profileInstances = 1;
        manager->samples[i].accumulator = 0.0f;
        manager->samples[i].startTime = manager->clock.GetTime();
        manager->samples[i].childrenSampleTime = 0.0f;
        this->name = manager->samples[i].name;
    }
    init = false;
}

Profiler::~Profiler() {
    U16 i=0;
    U16 numParents=0;

    if(manager->profilerEnabled) {
        while(i<MAX_PROFILE_SAMPLES && manager->samples[i].valid) {
            if(strcmp(manager->samples[i].name, name) == 0) {
                U16 inner = 0;
                I16 parent = -1;
                float endTime = manager->clock.GetTime();

                manager->samples[i].openedProfiles--;

                while(manager->samples[inner].valid) {
                    if(manager->samples[inner].openedProfiles > 0) {
                        numParents++;
                        if(parent<0) {
                            parent = inner;
                        }
                        else if(manager->samples[inner].startTime >= manager->samples[parent].startTime) {
                            parent = inner;
                        }
                    }
                    inner++;
                }

                manager->samples[i].numParents = numParents;
                if(parent>=0) {
                    manager->samples[parent].childrenSampleTime += (endTime - manager->samples[i].startTime);
                }

                manager->samples[i].accumulator += (endTime - manager->samples[i].startTime);
                return;
            }
            i++;
        }
    }
}

ProfilerManager::ProfilerManager() {
    clock.Init();
    total.numSamples=0;
    total.maxTime=0;
    total.minTime=50000;
    total.avgTime=0;
    total.maxPercent=0;
    total.minPercent=50000;
    total.avgPercent=0;
    total.partialSumPercent=0.0f;
    total.partialSumTime=0.0f;

    for(int i=0;i<MAX_PROFILE_SAMPLES;i++) {
        samples[i].valid = false;
        samplesHistory[i].valid = false;
    }

    startTime = clock.GetTime();
    enabled = profilerEnabled;
}

void ProfilerManager::DumpProfileDataToBuffer() {
    if(!profilerEnabled)
        enabled=false;

    if(enabled) {
        U16 i=0;

        stopTime = clock.GetTime();
        outputBuffer.Clear();
        outputBuffer.Print("       Avg :        Min :        Max :   # :  Profile\n");
        outputBuffer.Print("-----------------------------------------------------\n");


        float totalTime = stopTime-startTime;

        total.numSamples++;
        total.partialSumTime+=totalTime;
        total.avgTime = total.partialSumTime/total.numSamples;

        if(totalTime<total.minTime)
            total.minTime = totalTime;

        if(total.minTime<0.0f)
            total.minTime=0.0f;

        if(totalTime>total.maxTime)
            total.maxTime = totalTime;


        F32 sampleTime, percentTime, aveTime, minTime, maxTime;
        F32 avgPercent, minPercent, maxPercent;
        char line[256], name[256], indentedName[256];
        char ave[16], min[16], max[16], num[16];

        while( i < MAX_PROFILE_SAMPLES && samples[i].valid) {
            U16 indent = 0;


            if( samples[i].openedProfiles > 0 ) {
                ASSERT(0, "There are still opened profiles");
            }

            sampleTime = samples[i].accumulator - samples[i].childrenSampleTime;

            if(stopTime-startTime!=0.0f)
                percentTime = ( sampleTime / (stopTime - startTime) ) * 100.0f;
            else percentTime = 0;


            aveTime = minTime = maxTime = sampleTime;
            avgPercent = minPercent = maxPercent = percentTime;


            //Add new measurement into the history and get the ave, min, and max
            StoreProfileInHistory( samples[i].name, percentTime, sampleTime );
            GerProfileFromHistory( samples[i].name, &avgPercent, &minPercent, &maxPercent,
                                   &aveTime, &minTime, &maxTime);

            //Format the data
            sprintf( ave, "%2.1f (%2.0f%)", aveTime, avgPercent );
            sprintf( min, "%2.1f (%2.0f%)", minTime, minPercent );
            sprintf( max, "%2.1f (%2.0f%)", maxTime, maxPercent );
            sprintf( num, "%3d", samples[i].profileInstances );

            strcpy( indentedName, samples[i].name );
            for( indent=0; indent<samples[i].numParents; indent++ ) {
                sprintf( name, "   %s", indentedName );
                strcpy( indentedName, name );
            }

            sprintf(line,"%11s: %11s: %11s: %3s : %s\n", ave, min, max, num, indentedName);
            outputBuffer.Print(line);


            sprintf( ave, "%2.1f", total.avgTime);
            sprintf( min, "%2.1f", total.minTime);
            sprintf( max, "%2.1f", total.maxTime);


            i++;
        }
        outputBuffer.Print("-----------------------------------------------------\n");
        sprintf(line,"%11s: %11s: %11s:", ave, min, max);
        outputBuffer.Print(line);


        for( i=0; i<MAX_PROFILE_SAMPLES; i++ ) {
            samples[i].valid = false;
        }
        startTime = clock.GetTime();
    }
    else {
        if(profilerEnabled) {
            clock.Init();
            total.numSamples=0;
            total.maxTime=0;
            total.minTime=50000;
            total.avgTime=0;
            total.maxPercent=0;
            total.minPercent=50000;
            total.avgPercent=0;
            total.partialSumPercent=0.0f;
            total.partialSumTime=0.0f;

            for(int i=0;i<MAX_PROFILE_SAMPLES;i++) {
                samples[i].valid = false;
                samplesHistory[i].valid = false;
            }

            startTime = clock.GetTime();
            enabled = true;
        }
    }
}


void ProfilerManager::StoreProfileInHistory( char* name, float percent, float dt )
{
    U16 i = 0;

    while(i<MAX_PROFILE_SAMPLES && samplesHistory[i].valid) {
        if(strcmp(name, samplesHistory[i].name) == 0) {

            samplesHistory[i].numSamples++;
            samplesHistory[i].partialSumPercent+=percent;
            samplesHistory[i].avgPercent = samplesHistory[i].partialSumPercent/samplesHistory[i].numSamples;

            if(percent<samplesHistory[i].minPercent)
                samplesHistory[i].minPercent = percent;

            if(samplesHistory[i].minPercent<0.0f)
                samplesHistory[i].minPercent=0.0f;

            if(percent>samplesHistory[i].maxPercent)
                samplesHistory[i].maxPercent = percent;


            samplesHistory[i].partialSumTime+=dt;
            samplesHistory[i].avgTime = samplesHistory[i].partialSumTime/samplesHistory[i].numSamples;

            if(dt<samplesHistory[i].minTime)
                samplesHistory[i].minTime = dt;

            if(samplesHistory[i].minTime<0.0f)samplesHistory[i].minTime=0.0f;

            if(dt>samplesHistory[i].maxTime)
                samplesHistory[i].maxTime = dt;

            return;
        }
        i++;
    }


    if(i<MAX_PROFILE_SAMPLES) {
        strcpy(samplesHistory[i].name, name);
        samplesHistory[i].valid=true;
        samplesHistory[i].minPercent=samplesHistory[i].maxPercent=samplesHistory[i].avgPercent = percent;
        samplesHistory[i].minTime=samplesHistory[i].maxTime=samplesHistory[i].avgTime = dt;
        samplesHistory[i].numSamples=1;
        samplesHistory[i].partialSumPercent=percent;
        samplesHistory[i].partialSumTime=dt;
    }
    else {
        ASSERT(0, "Exceeded max profile samples");
    }
}

void ProfilerManager::GerProfileFromHistory( char* name, float* avgPercent, float* minPercent, float* maxPercent,
                                      float* avgTime, float* minTime, float* maxTime)
{
    U16 i=0;
    while(i<MAX_PROFILE_SAMPLES && samplesHistory[i].valid) {
        if(strcmp(samplesHistory[i].name, name) == 0) {
            *avgPercent = samplesHistory[i].avgPercent;
            *minPercent = samplesHistory[i].minPercent;
            *maxPercent = samplesHistory[i].maxPercent;
            *avgTime = samplesHistory[i].avgTime;
            *minTime = samplesHistory[i].minTime;
            *maxTime = samplesHistory[i].maxTime;
            return;
        }
        i++;
    }
    *avgPercent=*avgTime=*minPercent=*minTime=*maxPercent=*maxTime=0.0f;
}
