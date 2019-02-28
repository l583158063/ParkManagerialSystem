#include <string>
#ifndef CARINFO_H_INCLUDED
#define CARINFO_H_INCLUDED

using namespace std;

typedef struct CarInfo{
    string licensePlate;
    string brandAndType;
    string color;
    bool ifPark;
    int whereEnter;
    float parkTime;
    float parkFee;
}CarInfo;

#endif // CARINFO_H_INCLUDED
