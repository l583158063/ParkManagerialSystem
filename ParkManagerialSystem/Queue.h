#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED
#include "ParkingLot.h"
#include "CarInfo.h"

typedef struct QNode{
    CarInfo carData;
    struct QNode *next;
}QNode, *QueuePtr;

typedef struct LinkQueue{
    QueuePtr front;
    QueuePtr rear;
}LinkQueue;

typedef int Status;

Status InitQueue( LinkQueue &Q );
Status EnQueue( LinkQueue &Q, CarInfo e );
Status DeQueue( LinkQueue &Q, CarInfo &e );
void DisplayQElem( QNode Q);
Status Traverse( LinkQueue Q );

#endif // QUEUE_H_INCLUDED
