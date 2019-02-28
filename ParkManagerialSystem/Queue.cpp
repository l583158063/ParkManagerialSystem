#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ParkingLot.h"
#include "Queue.h"
#include "stdafx.h"
#define MAX_VERTEX_NUM 100
#define MAX_LENGTH 200
#define INFINITY 5000
#define MAX_INFO_LENGTH 100
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define SHOWSPACE 10

using namespace std;

typedef int Status;

Status InitQueue( LinkQueue &Q )
// 初始化带头结点的队列
{
    Q.front = Q.rear = new QNode;
    if(!Q.front) exit(OVERFLOW);
    Q.front->next = NULL;
    return OK;
}

Status EnQueue( LinkQueue &Q, CarInfo e )
// 插入元素e为Q的新的队尾元素
{
    QueuePtr p = new QNode;
    if(!p) exit(OVERFLOW);
    p->carData = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = Q.rear->next;
    return OK;
}

Status DeQueue( LinkQueue &Q, CarInfo &e )
// 若队列不空，则删除Q的队头元素，
// 用 e 返回其值，并返回OK；否则返回ERROR
{
    if(Q.front == Q.rear) return ERROR;
    QueuePtr p = Q.front->next;
    e = p->carData;
    Q.front->next = p->next;
    if(Q.rear == p) Q.rear = Q.front;
    free(p);
    return OK;
}

void DisplayQElem( QNode Q )
{// 显示队列中的车牌、型号、颜色
    cout << setw(SHOWSPACE) << Q.carData.licensePlate;
    cout << setw(SHOWSPACE) << Q.carData.brandAndType;
    cout << setw(SHOWSPACE) << Q.carData.color << endl;
}

Status Traverse( LinkQueue Q )
{
    QueuePtr p = Q.front->next;
    if(Q.front == Q.rear)
    {
        cout << "目前无车排队" << endl;
        return ERROR;
    }
    int i = 1;
    while(p != NULL)
    {
        cout << i++ << setw(SHOWSPACE);
        DisplayQElem(*p);
        p = p->next;
    }
    return OK;
}


