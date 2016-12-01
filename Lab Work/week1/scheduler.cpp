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
        int wTime;
        int sTime;
        int fTime;
        int rTime;
};

bool cmpByArrTime_lt (Process p1, Process p2);

bool cmpByJobTime_gt (Process p1, Process p2);

bool cmpByRemTime_gt (Process p1, Process p2);


bool cmpByArrTime_lt (Process p1, Process p2) {
    return p1.aTime < p2.aTime;
}

bool cmpByJobTime_gt (Process p1, Process p2) {

    if (p1.bTime == p2.bTime) {
        return p1.aTime > p2.aTime;
    }

    return p1.bTime > p2.bTime;
}

bool cmpByRemTime_gt (Process p1, Process p2) {

    if (p1.rTime == p2.rTime) {
        return p1.aTime > p2.aTime;
    }

    return p1.rTime > p2.rTime;
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
    std::vector<Process> fcfsList;
    std::vector<Process> sjfList;
    
    for (int i = 0; i < numProc; i++) {
        char dlmtr;
        Process temp;
        
        inFile >> temp.id >> dlmtr >> temp.aTime >> dlmtr >> temp.bTime;
        //inFile >> temp.id >> temp.aTime >> temp.bTime;
        list.push_back(temp);
        fcfsList.push_back(temp);
        sjfList.push_back(temp);
    }
    
    inFile.close();


    for (int i = 0; i < numProc; ++i) {
        list[i].wTime = -1; list[i].sTime = -1; list[i].fTime = -1;
        fcfsList[i].wTime = -1; fcfsList[i].sTime = -1; fcfsList[i].fTime = -1;
        sjfList[i].wTime = -1; sjfList[i].sTime = -1; sjfList[i].fTime = -1;

        list[i].rTime = list[i].bTime;
        fcfsList[i].rTime = fcfsList[i].bTime;
        sjfList[i].rTime = sjfList[i].bTime;
    }



    std::sort(list.begin(), list.end(), cmpByArrTime_lt);


    //=================================   <FCFS>   ==================================//  
    { // Tight scoping
        std::sort(fcfsList.begin(), fcfsList.end(), cmpByArrTime_lt);

        int currTime = 0;

        for (int i = 0; i < numProc; ++i) {

            if(currTime < fcfsList[i].aTime) { //More timeto wait for arrival

                currTime = fcfsList[i].aTime;
                fcfsList[i].sTime = fcfsList[i].aTime;
                fcfsList[i].wTime = 0; //Started as immediately on arrival
            }

            else {

                fcfsList[i].sTime = currTime;
                fcfsList[i].wTime = currTime - fcfsList[i].aTime; //Waited after arrival to start
            }

            currTime += fcfsList[i].bTime;
            fcfsList[i].fTime = currTime;
        }
    }
    
    //=================================   </FCFS>   ===================================//



    //---------------------------------------------------------------------------------//


    //==================================   <SJF>   ====================================//
    
    std::vector<Process> sjfDone;

    { //Tight Scope

        std::sort(sjfList.begin(), sjfList.end(), cmpByArrTime_lt);

        std::vector<Process> heap;

        int currTime = 0;
        int i = 0;


        while ( (i < numProc) or not(heap.empty()) ) {

            if(heap.empty() and currTime < sjfList[i].aTime) { //Wait and fast forward to when it arrives
                currTime = sjfList[i].aTime;
            }

            while( (i < numProc) and (sjfList[i].aTime <= currTime) ) {
                heap.push_back(sjfList[i]);
                push_heap(heap.begin(), heap.end(), cmpByJobTime_gt);
                ++i;
            }

            pop_heap(heap.begin(), heap.end(), cmpByJobTime_gt);
            Process current = heap.back();
            heap.pop_back();

            current.sTime = currTime;
            current.wTime = current.sTime - current.aTime;
            current.fTime = current.sTime + current.bTime;

            currTime = current.fTime;

            sjfDone.push_back(current);

        }
    }
    


    //==================================  </SJF>   ====================================//

    //---------------------------------------------------------------------------------//

    //==================================   <RRB>   ====================================//

    std::vector<Process> rrbList;

    { //Tignt scope

        int timeQuant = 2;

        std::queue<Process> queue;

        int currTime = 0;
        int i = 0;

        std::cout << "\n\nExecuting Round Robin, timeQuant = " << timeQuant << std::endl;

        while ( (i < numProc) or not(queue.empty()) ) {

            if(queue.empty() and currTime < list[i].aTime) { //Wait and fast forward to when it arrives
                currTime = list[i].aTime;
            }


            //Add all available proc
            while( (i < numProc) and (list[i].aTime <= currTime) ) {
                queue.push(list[i]);
                ++i;
            }


            Process current = queue.front();
            queue.pop();


            std::cout << "Starting/Resuming Process " << current.id << " currTime: " << currTime 
                << " Remaining Time: " << current.rTime << std::endl;

            //First time
            if(current.sTime < 0) current.sTime = currTime;

            if (current.rTime > timeQuant) {

                current.rTime -= timeQuant;
                currTime += timeQuant;

                //Add all available
                while( (i < numProc) and (list[i].aTime <= currTime) ) {
                    queue.push(list[i]);
                    ++i;
                }

                queue.push(current);
            }

            else { //Finish process

                currTime += current.rTime;
                current.rTime = 0;

                current.fTime = currTime;
                current.wTime = current.fTime - current.aTime - current.bTime;

                rrbList.push_back(current);
            }
        }
    }

    //==================================  </RRB>   ====================================//

    //---------------------------------------------------------------------------------//

    //==================================   <SRT>   ====================================//

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








    //==================================  </SRT>   ====================================//


    std::ofstream fcfsOut;
    fcfsOut.open("fcfs.csv");
    
    for (int i = 0; i < numProc; i++) {
        fcfsOut << fcfsList[i].id << ',' << fcfsList[i].aTime << ',' << fcfsList[i].bTime
         << ',' << fcfsList[i].wTime << ',' << fcfsList[i].sTime << ',' << fcfsList[i].fTime << std::endl;
    }
    fcfsOut.close();


    std::ofstream sjfOut;
    sjfOut.open("sjf.csv");
    
    for (int i = 0; i < numProc; i++) {
        sjfOut << sjfDone[i].id << ',' << sjfDone[i].aTime << ',' << sjfDone[i].bTime
         << ',' << sjfDone[i].wTime << ',' << sjfDone[i].sTime << ',' << sjfDone[i].fTime << std::endl;
    }
    sjfOut.close();
    
    

    std::ofstream rrbOut;
    rrbOut.open("rrb.csv");
    
    for (int i = 0; i < numProc; i++) {
        rrbOut << rrbList[i].id << ',' << rrbList[i].aTime << ',' << rrbList[i].bTime
         << ',' << rrbList[i].wTime << ',' << rrbList[i].sTime << ',' << rrbList[i].fTime << std::endl;
    }
    rrbOut.close();


    std::ofstream srtOut;
    srtOut.open("srt.csv");
    
    for (int i = 0; i < numProc; i++) {
        srtOut << srtList[i].id << ',' << srtList[i].aTime << ',' << srtList[i].bTime
         << ',' << srtList[i].wTime << ',' << srtList[i].sTime << ',' << srtList[i].fTime << std::endl;
    }
    srtOut.close();


    return 0;
}