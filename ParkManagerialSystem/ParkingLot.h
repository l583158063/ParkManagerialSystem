#include <string>
#ifndef PARKINGLOT_H_INCLUDED
#define PARKINGLOT_H_INCLUDED
#include "Queue.h"
#include "CarInfo.h"
#define MAX_VERTEX_NUM 100
#define MAX_PARKSPACE_NUM 100
#define MAX_LENGTH 200
#define INFINITY 5000
#define MAX_INFO_LENGTH 100

using namespace std;

typedef struct ParkPath{
    int distance;
    string info;
}ParkPath, ParkPathMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct ParkingArea{
    int allSpace;
    int emptySpace;
    CarInfo parkingCars[MAX_PARKSPACE_NUM];
}ParkingArea;

typedef struct AccountNode{
    string licensePlate;
    int whereEnter;
    int whereExit;
    float parkTime;
    float parkFee;
    struct EntranceAccount *next;
}AccountNode;

typedef struct Entrance{
    bool ifDischarge; // 放行一辆车就置为true
    LinkQueue Q;
    AccountNode accounts;
}Entrance;

typedef struct ParkingLot{
    ParkPathMatrix paths;
    int shortestPaths[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    ParkingArea areas[MAX_VERTEX_NUM];
    int entranceNum;
    Entrance entrances[MAX_VERTEX_NUM];
    int pathNum;
    int areaNum;
    int allSpace;
    int emptySpace;
}ParkingLot;

#endif // PARKINGLOT_H_INCLUDED
