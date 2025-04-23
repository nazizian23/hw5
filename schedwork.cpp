#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

bool assignWorker(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<int>& shiftsWorked,
    size_t day,
    size_t slot
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t numDays= avail.size();
    size_t numWorkers=avail[0].size();
    //Initialize schedule matrix
    for(size_t i=0; i<numDays;++i){
        sched.push_back(vector<Worker_T>());
    }
    //Initialize shift count for each worker
    vector<int>shiftsWorked(numWorkers,0);
    //Start recursion
    return assignWorker(avail,dailyNeed,maxShifts,sched,shiftsWorked,0,0);

}

bool assignWorker(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    vector<int>& shiftsWorked,
    size_t day,
    size_t slot
)
{
    size_t numDays= avail.size();
    size_t numWorkers=avail[0].size();
    //Base case for scenario where all days are scheduled
    if(day==numDays){
        return true;
    }
    //If the current day is full move on to next day 
    if(slot==dailyNeed){
        return assignWorker(avail,dailyNeed,maxShifts,sched,shiftsWorked,day+1,0);
    }
    //Try all the workers 
    for(size_t worker=0; worker<numWorkers;++worker){
        if(avail[day][worker]&&shiftsWorked[worker]<static_cast<int>(maxShifts)&& find(sched[day].begin(),sched[day].end(),worker)==sched[day].end()){
            //If all constraints pass assign the worker to the current slot
            sched[day].push_back(worker);
            shiftsWorked[worker]++;//Update their shift count
            //Recurse to fill the next slot 
            if(assignWorker(avail,dailyNeed,maxShifts,sched,shiftsWorked,day,slot+1)){
                return true;
            }
            //Backtrack if recursion fails 
            sched[day].pop_back();
            shiftsWorked[worker]--;
        }
    }
    return false;
}

