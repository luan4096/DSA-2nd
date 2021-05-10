#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <type_traits>

using namespace std;

class trip {
private:
    string licensePlate; // 10 chars max
    int departureTime, arrivalTime;
    bool caseCheck; // the bus goes from start to end if false, and go from end to start if true, 

public:
    trip(string lPlate, int dTime, int aTime, bool caseCheck)
    {
        this->licensePlate = lPlate;
        this->departureTime = dTime;
        this->arrivalTime = aTime;
        this->caseCheck = caseCheck;
    }

    string& getLicensePlate()
    {
        return this->licensePlate;
    }

    int getDepartureTime()
    {
        return this->departureTime;
    }

    int getArrivalTime()
    {
        return this->arrivalTime;
    }

    bool getCase()
    {
        return this->caseCheck;
    }
};

class route {
private:
    int maxTrip;
    string code; // 5 char max
    trip** trips;
    int currentTripIndex; // index of the last trip in the route, if there is no trip this value is set to -1

public:
    route(string code, int maxTrip)
    {
        this->maxTrip = maxTrip;
        this->trips = new trip*[maxTrip];
        this->code = code;
        this->currentTripIndex = -1;
    }

    int getNumberOfTrips()
    {
        return this->currentTripIndex + 1;
    }

    string& getCode()
    {
        return code;
    }

    string routeInformation()
    {
        stringstream inf;
        
        inf << "------------------------------" << endl;
        if (this->currentTripIndex == -1)
        {
            inf << "There is no trip in here!!!" << endl;
            inf << "------------------------------" << endl;
            return inf.str();
        }
        int numberOfTrips = this->currentTripIndex + 1;
        for (int i = 0; i < numberOfTrips; i++)
        {
            inf << "License Plate: " << trips[i]->getLicensePlate() << endl;
            inf << "Departure Time: " << trips[i]->getDepartureTime() << endl;
            inf << "Arrival Time: " << trips[i]->getArrivalTime() << endl;
            inf << "Case: " << trips[i]->getCase() << endl;
            inf << "------------------------------" << endl;
        }
        return inf.str();
    }

    int getMaxTrip()
    {
        return this->maxTrip;
    }

    int addTrip(string lPlate, int dTime, int aTime, bool caseCheck = false)
    {
        if (dTime >= aTime)
            return -1;
            
        if (this->currentTripIndex == this->maxTrip - 1 || dTime >= aTime)
            return -1;

        for (int i = 0; i <= this->currentTripIndex; i++)
        {
            if (trips[i]->getLicensePlate() == lPlate)
            {
                if (trips[i]->getArrivalTime() >= dTime)
                    return -1;
            }

            if (trips[i]->getDepartureTime() == dTime)
            {
                if (trips[i]->getCase() == caseCheck || trips[i]->getLicensePlate() == lPlate)
                    return -1;
            }
        }

        int index = ++this->currentTripIndex;
        this->trips[index] = new trip(lPlate, dTime, aTime, caseCheck);
        return this->currentTripIndex + 1;
    }

    int deleteTrip(int timeA = -1, int timeB = -1)
    {
        
        if (timeA != -1)
        {
            if (timeB != -1 && timeA != timeB) // timeA and timeB
            {
                if (timeA < timeB)
                    return 0;
                int count = 0;
                for (int i = 0; i <= this->currentTripIndex; i++)
                {
                    int time = this->trips[i]->getDepartureTime();
                    if (time >= timeA && time <= timeB)
                    {
                        delete this->trips[i];
                        count++;
                        if (i < this->currentTripIndex)
                        {
                            for (int j = i; j < this->currentTripIndex; j++)
                                this->trips[j] = this->trips[j + 1];
                            i--;
                        }
                        this->currentTripIndex--;
                    }
                }
                if (this->currentTripIndex == -1)
                {
                    delete this->trips;
                    return -1 * count;
                }
                else return count;
            }
            else // timeA but no timeB
            {
                int count = 0;
                for (int i = 0; i <= this->currentTripIndex; i++)
                {
                    if (this->trips[i]->getDepartureTime() == timeA)
                    {
                        delete this->trips[i];
                        count++;
                        if (i < this->currentTripIndex)
                        {
                            for (int j = i; j < this->currentTripIndex; j++)
                                this->trips[j] = this->trips[j + 1];
                            i--;
                        }
                        this->currentTripIndex--;
                    }
                }
                if (this->currentTripIndex == -1)
                {
                    delete this->trips;
                    return -1 * count;
                }
                else return count;
            }
        }
        else // no timeA, no timeB
        {
            for (int i = 0; i <= currentTripIndex; i++)
            {
                delete this->trips[i];
            }
            delete this->trips;
            return this->currentTripIndex + 1;
        }
    }

    int numOfUnArrivedTrips(int time, int caseCheck = -1)
    {
        int tripsCount = 0;
        if (caseCheck == -1)
        {
            for (int i = 0; i <= this->currentTripIndex; i++)
            {
                if (this->trips[i]->getDepartureTime() <= time 
                    && this->trips[i]->getArrivalTime() >= time)
                    tripsCount++;
            }
            return tripsCount;
        }

        for (int i = 0; i <= this->currentTripIndex; i++)
        {
            if (this->trips[i]->getDepartureTime() <= time 
                && this->trips[i]->getArrivalTime() >= time 
                && this->trips[i]->getCase() == caseCheck)
                tripsCount++;
        }
        return tripsCount;
    }

    int numOfArrivedTrips(int time, int caseCheck = -1)
    {
        int tripsCount = 0;
        if (caseCheck == -1)
        {
            for (int i = 0; i <= this->currentTripIndex; i++)
            {
                if (this->trips[i]->getArrivalTime() < time)
                    tripsCount++;
            }
            return tripsCount;
        }

        for (int i = 0; i <= this->currentTripIndex; i++)
        {
            if (this->trips[i]->getArrivalTime() < time
                && this->trips[i]->getCase() == caseCheck)
                tripsCount++;
        }
        return tripsCount;
    }

    string lpOfDeparturedTrip(int time, int caseCheck = -1)
    {
        string lp = "-1";
        int min = INT32_MAX, difference;
        if (caseCheck == -1)
        {
            for (int i = 0; i <= this->currentTripIndex; i++)
            {
                difference = time - this->trips[i]->getDepartureTime();
                if (difference >= 0)
                {
                    if (difference == min)
                        if (this->trips[i]->getCase() == false)
                            lp = this->trips[i]->getLicensePlate();
                    if (difference < min)
                    {
                        lp = this->trips[i]->getLicensePlate();
                        min = difference;
                    }
                }
            }
            return lp;
        }
        for (int i = 0; i <= this->currentTripIndex; i++)
        {
            difference = time - this->trips[i]->getDepartureTime();
            if (this->trips[i]->getCase() == caseCheck && difference >= 0 && difference < min)
            {
                lp = this->trips[i]->getLicensePlate();
                min = difference;
            }
        }
        return lp;
    }

    string lpOfArrivedTrip(int time, int caseCheck = -1)
    {
        string lp = "-1";
        int min = INT32_MAX, minDTime = INT32_MAX, difference;
        bool caseOfMin;
        if (caseCheck == -1)
        {
            for (int i = 0; i <= this->currentTripIndex; i++)
            {
                difference = time - this->trips[i]->getArrivalTime();
                if (difference > 0)
                {
                    if (difference == min)
                    {
                        if (this->trips[i]->getCase() == caseOfMin)
                        {
                            if (this->trips[i]->getDepartureTime() < minDTime)
                            {
                                lp = this->trips[i]->getLicensePlate();
                                minDTime = this->trips[i]->getDepartureTime();
                            }
                        }
                        else if (caseOfMin == true)
                        {
                            lp = this->trips[i]->getLicensePlate();
                            minDTime = this->trips[i]->getDepartureTime();
                            caseOfMin = this->trips[i]->getCase();
                        }
                    }
                    else if (difference < min)
                    {
                        lp = this->trips[i]->getLicensePlate();
                        minDTime = this->trips[i]->getDepartureTime();
                        caseOfMin = this->trips[i]->getCase();
                        min = difference;
                    }
                }
            }
            return lp;
        }
        for (int i = 0; i <= this->currentTripIndex; i++)
        {
            difference = time - this->trips[i]->getArrivalTime();
            if (this->trips[i]->getCase() == caseCheck 
                && difference > 0)
            {
                if (difference == min && this->trips[i]->getDepartureTime() < minDTime)
                {
                    lp = this->trips[i]->getLicensePlate();
                    minDTime = this->trips[i]->getDepartureTime();
                }
                if (difference < min)
                {
                    lp = this->trips[i]->getLicensePlate();
                    min = difference;
                }
            }
        }
        return lp;
    }
};

class BusSystem
{
private:
    int maxTrip;
    list<route> routes;

public:
    BusSystem()
    {
        this->maxTrip = 0;
    }

    string query(string instruction) {
        // TODO: Your implementation
        if (instruction == "" || instruction[0] == ' ' || instruction[instruction.length() - 1] == ' ')
            return "-1";

        vector<string> ins;
        string temp;
        int last = 0;
        int insLength = instruction.length();
        bool spaceCheck = false; // this is used to check for extra space in instruction
        for (int i = 0; i < insLength + 1; i++)
        {
            if (instruction[i] == ' ' || instruction[i] == '\0')
            {
                if (spaceCheck == true)
                    return "-1";
                else
                {
                    spaceCheck = true;
                    ins.push_back(instruction.substr(last, i - last));
                    last = i + 1;
                }
            }
            else spaceCheck = false;
        }

        if (ins[0] == "SQ")
        {
            if (ins.size() < 2)
                return "-1";

            int n;
            try {
                n = stoi(ins[1]);
            }
            catch (std::invalid_argument)
            {
                return "-1";
            }
            
            if (n < 0)
                return "-1";
            this->maxTrip = stoi(ins[1]);
            return "1";
        }
        else if (ins[0] == "INS")
        {
            if (this->maxTrip == 0 || ins.size() < 5 || ins[1].length() > 5 || ins[2].length() > 10)
                return "-1";

            int timeA, timeB;
            int caseCheck;
            if (ins.size() == 5)
            {
                try {
                    timeA = stoi(ins[3]);
                    timeB = stoi(ins[4]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
            }
            else if (ins.size() == 6)
            {
                try {
                    caseCheck = stoi(ins[3]);
                    timeA = stoi(ins[4]);
                    timeB = stoi(ins[5]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                
                if (caseCheck != 0 && caseCheck != 1)
                    return "-1";
            }
            else return "-1";

            if (routes.empty())
            {
                if (this->maxTrip > 0)
                    this->routes.push_back(route(ins[1], this->maxTrip));
                list<route>::iterator first = this->routes.begin();
                int returnValue;
                if (ins.size() == 5)
                    returnValue = (*first).addTrip(ins[2], stoi(ins[3]), stoi(ins[4]));
                else
                    returnValue = (*first).addTrip(ins[2], stoi(ins[4]), stoi(ins[5]), stoi(ins[3]));

                return to_string(returnValue);
            }

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++)
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast)
            {
                this->routes.push_back(route(ins[1], this->maxTrip));
                i = this->routes.end();
                i--;
            }

            int returnValue;
            if (ins.size() == 5)
                returnValue = (*i).addTrip(ins[2], timeA, timeB);
            else
                returnValue = (*i).addTrip(ins[2], timeA, timeB, caseCheck);

            return to_string(returnValue);
        }
        else if (ins[0] == "DEL") // return 0 if the syntax of the query is corrected but no trip satisfies the request
        {
            if (ins.size() < 2 || this->routes.empty()) // check if instruction is incorrect or list is empty
                return "0";

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++) // loop through the list
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast) // case: no code in the list match
                return "0";

            int numOfDelTrips, timeA, timeB;
            if (ins.size() == 2)
            {
                numOfDelTrips = (*i).deleteTrip();
                this->routes.erase(i);
            }
            else if (ins.size() == 3)
            {
                try {
                    timeA = stoi(ins[2]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                numOfDelTrips = (*i).deleteTrip(timeA);
                if (numOfDelTrips < 0)
                {
                    this->routes.erase(i);
                    numOfDelTrips *= -1;
                }
            } 
            else if (ins.size() == 4)
            {
                try {
                    timeA = stoi(ins[2]);
                    timeB = stoi(ins[3]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                numOfDelTrips = (*i).deleteTrip(timeA, timeB);
                if (numOfDelTrips < 0)
                {
                    this->routes.erase(i);
                    numOfDelTrips *= -1;
                }
            }
            else numOfDelTrips = 0;

            return to_string(numOfDelTrips);
        }
        else if (ins[0] == "CS")
        {
            if (ins.size() < 3 || this->routes.empty()) // check if instruction is incorrect or list is empty
                return "0";

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++) // loop through the list
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast) // case: no code in the list match
                return "0";

            int numOfTrips, time, caseCheck;
            if (ins.size() == 3)
            {
                try {
                    time = stoi(ins[2]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                numOfTrips = (*i).numOfUnArrivedTrips(time);
            }
            else if (ins.size() == 4)
            {
                try {
                    time = stoi(ins[2]);
                    caseCheck = stoi(ins[3]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                
                if (caseCheck != 0 && caseCheck != 1)
                    return "-1";
                
                numOfTrips = (*i).numOfUnArrivedTrips(time, caseCheck);
            }
            else numOfTrips = 0;

            return to_string(numOfTrips);
        }
        else if (ins[0] == "CE")
        {
            if (ins.size() < 3 || this->routes.empty()) // check if instruction is incorrect or list is empty
                return "0";

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++) // loop through the list
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast) // case: no code in the list match
                return "0";

            int numOfTrips, time, caseCheck;
            if (ins.size() == 3)
            {
                try {
                    time = stoi(ins[2]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                numOfTrips = (*i).numOfArrivedTrips(time);
            }
            else if (ins.size() == 4)
            {
                try {
                    time = stoi(ins[2]);
                    caseCheck = stoi(ins[3]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                
                if (caseCheck != 0 && caseCheck != 1)
                    return "-1";
                
                numOfTrips = (*i).numOfArrivedTrips(time, caseCheck);
            }
            else numOfTrips = 0;

            return to_string(numOfTrips);
        }
        else if (ins[0] == "GS")
        {
            if (ins.size() < 3 || this->routes.empty()) // check if instruction is incorrect or list is empty
                return "-1";

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++) // loop through the list
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast) // case: no code in the list match
                return "-1";

            int time, caseCheck;
            if (ins.size() == 3)
            {
                try {
                    time = stoi(ins[2]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                return (*i).lpOfDeparturedTrip(time);
            }
            else if (ins.size() == 4)
            {
                try {
                    time = stoi(ins[2]);
                    caseCheck = stoi(ins[3]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                
                if (caseCheck != 0 && caseCheck != 1)
                    return "-1";
                
                return (*i).lpOfDeparturedTrip(time, caseCheck);
            }
            return "-1";
        }
        else if (ins[0] == "GE")
        {
            if (ins.size() < 3 || this->routes.empty()) // check if instruction is incorrect or list is empty
                return "-1";

            list<route>::iterator afterLast = this->routes.end(), i;
            for (i = this->routes.begin(); i != afterLast; i++) // loop through the list
            {
                if ((*i).getCode() == ins[1])
                    break;
            }

            if (i == afterLast) // case: no code in the list match
                return "-1";

            int time, caseCheck;
            if (ins.size() == 3)
            {
                try {
                    time = stoi(ins[2]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                return (*i).lpOfArrivedTrip(time);
            }
            else if (ins.size() == 4)
            {
                try {
                    time = stoi(ins[2]);
                    caseCheck = stoi(ins[3]);
                }
                catch (std::invalid_argument)
                {
                    return "-1";
                }
                
                if (caseCheck != 0 && caseCheck != 1)
                    return "-1";
                
                return (*i).lpOfArrivedTrip(time, caseCheck);
            }
            return "-1";
        }

        return "-1";
    }

    void printRoutes()
    {
        if (routes.empty())
        {
            cout << "**************************************************" << endl;
            cout << "There is no routes in here!!!" << endl;
            cout << "**************************************************" << endl;
            return;
        }
        list<route>::iterator afterLast = this->routes.end();
        for (list<route>::iterator i = this->routes.begin(); i != afterLast; i++)
        {
            cout << "**************************************************" << endl;
            cout << "Route's code: " << (*i).getCode() << endl;
            cout << "Number of trip: " << (*i).getNumberOfTrips() << endl;
            cout << (*i).routeInformation() << endl;
            cout << "**************************************************" << endl << endl;
        }
    }

    void printMaxTrip()
    {
        cout << "Max Trip: " << this->maxTrip << endl;
    }
};