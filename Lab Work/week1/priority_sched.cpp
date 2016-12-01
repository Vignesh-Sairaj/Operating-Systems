#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <queue>


class Process {
    public:
        /*

            aTime --- Arrival Time
            bTime --- Burst Time
            sTime --- Start Time
            wTime --- Wait Time = fTime - aTime - bTime
            fTime --- Finish Time
            rTime --- Remaining Time

        */
        int id;
        int aTime;
        int bTime;
        float priority;
        int wTime;
        int sTime;
        int fTime;
        int rTime;
};


bool cmpByPriority_gt (Process p1, Process p2);

bool cmpByArrTime_lt (Process p1, Process p2);

bool cmpByArrTime_lt (Process p1, Process p2) {
    return p1.aTime < p2.aTime;
}

bool cmpByPriority_gt (Process p1, Process p2) {

    if (p1.priority == p2.priority) {
        return p1.aTime > p2.aTime;
    }

    return p1.priority > p2.priority;
}




int main(int argc, char* argv[]) {
    
    if(argc != 3) {
        throw std::runtime_error("Incorrect number of arguments; expected 3.");
    }
    
    // First arg -- numProcesses
    int numProc;
    
    { //To scope temp tightly
        std::string temp (argv[1]);
        numProc = std::stoi(temp);
    }
    
    std::cout << "Here\n";
    
    std::ifstream inFile;
    inFile.open(argv[2]);
    
    /*
        The input csv file fields:
        Process_id, arrival_time, burst_time
    */
    
    std::vector<Process> list;
    
    for (int i = 0; i < numProc; i++) {
        char dlmtr;
        Process temp;
        
        inFile >> temp.id >> dlmtr >> temp.aTime >> dlmtr >> temp.bTime >> dlmtr >> temp.priority;
        //inFile >> temp.id >> temp.aTime >> temp.bTime;
        list.push_back(temp);
    }
    
    inFile.close();


    for (int i = 0; i < numProc; ++i) {

        list[i].wTime = -1; list[i].sTime = -1; list[i].fTime = -1;
        list[i].rTime = list[i].bTime;
    }



    std::sort(list.begin(), list.end(), cmpByArrTime_lt);



    //---------------------------------------------------------------------------------//


    //==================================   <Static>   ====================================//
    

    std::vector<Process> statList;


    { //Tight Scope

        std::vector<Process> heap;

        int currTime = 0;
        int i = 0;


        while ( (i < numProc) or not(heap.empty()) ) {

            if(heap.empty() and currTime < list[i].aTime) { //Wait and fast forward to when it arrives
                currTime = list[i].aTime;
            }

            while( (i < numProc) and (list[i].aTime <= currTime) ) {
                heap.push_back(list[i]);
                push_heap(heap.begin(), heap.end(), cmpByPriority_gt);
                ++i;
            }

            pop_heap(heap.begin(), heap.end(), cmpByPriority_gt);
            Process current = heap.back();
            heap.pop_back();

            current.sTime = currTime;
            current.wTime = current.sTime - current.aTime;
            current.fTime = current.sTime + current.bTime;

            currTime = current.fTime;

            statList.push_back(current);

        }
    }




    //==================================   </Static>   ====================================//

    //---------------------------------------------------------------------------------//

    //==================================   <Dynamic>   ====================================//

    std::vector<Process> srtList;


    { //Tight Scope

        std::sort(list.begin(), list.end(), cmpByArrTime_lt);

        std::vector<Process> heap;

        int currTime = 0;
        int i = 0;


        while ( (i < numProc) or not(heap.empty()) ) {

            if(heap.empty() and currTime < list[i].aTime) { //Wait and fast forward to when it arrives
                currTime = list[i].aTime;
            }

            while( (i < numProc) and (list[i].aTime <= currTime) ) {
                heap.push_back(list[i]);
                push_heap(heap.begin(), heap.end(), cmpByRemTime_gt);
                ++i;
            }

            pop_heap(heap.begin(), heap.end(), cmpByRemTime_gt);
            Process current = heap.back();
            heap.pop_back();

            //First time
            if(current.sTime < 0) current.sTime = currTime;

            //If another process is available before current ends
            if (i < numProc and list[i].aTime < currTime + current.rTime) {

                //subtract by time untill arrival; process run till arrival of next
                current.rTime -= list[i].aTime - currTime;
                currTime = list[i].aTime;

                //Reinsert
                heap.push_back(current);
                push_heap(heap.begin(), heap.end(), cmpByRemTime_gt);
            }


            else {
                currTime += current.rTime;
                current.rTime = 0;

                current.fTime = currTime;
                current.wTime = current.fTime - current.aTime - current.bTime;

                srtList.push_back(current);
            }
        }
    }


    //==================================   </Dynamic>   ====================================//

    //---------------------------------------------------------------------------------//






    std::ofstream statOut;
    statOut.open("stat.csv");
    
    for (int i = 0; i < numProc; i++) {
        statOut << statList[i].id << ',' << statList[i].aTime << ',' << statList[i].bTime << ',' << statList[i].priority
         << ',' << statList[i].wTime << ',' << statList[i].sTime << ',' << statList[i].fTime << std::endl;
    }
    statOut.close();

    return 0;
}