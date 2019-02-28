#include <windows.h>
#include <conio.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ParkingLot.h"
#include "Queue.h"
#include "CarInfo.h"
#define MAX_VERTEX_NUM 100
#define MAX_LENGTH 200
#define INFINITY 5000
#define MAX_INFO_LENGTH 100

using namespace std;

string carColor[9] = {"红色","黄色","蓝色","绿色","黑色","白色","银灰色","褐色","紫色"};
string carType[20] = {"迈巴赫","宝马","奔驰","大众","桑塔纳","奥迪","红旗","雪佛兰","福特","悦达起亚","兰博基尼","法拉利","保时捷","路虎","凯迪拉克","轩尼诗","别克","布加迪","柯尼塞格","劳斯莱斯"};
string provAbbr[31] = {"京","津","冀","沪","渝","豫","云","辽","黑","湘","皖","鲁","新","苏","浙","赣","鄂","桂","甘","晋","蒙","陕","闽","吉","贵","粤","青","藏","川","宁","琼"};
FILE *file;

int StringToInt( string );
void CreatACar( CarInfo &car );
void CreatPark( ParkingLot &Park );
bool IfAllEmpty( ParkingLot &Park );
void Monitor( ParkingLot &Park );
void FirstUse( ParkingLot &Park );
void Survey( ParkingLot &Park );
void CheckAccount( ParkingLot &Park );


int main()
{
    system("color f0");
    srand(time(NULL));

    int choose1;
    char choose2;
    ParkingLot park;

    cout << "\n\n\t\t欢迎来到停车场管理系统！" << endl << endl;
    Sleep(1000);
    cout << "\t请选择是否建立新的停车场（按y/Y确定，按其他键取消）" << endl;
    choose2 = getch();
    if('y' == choose2 || 'Y' == choose2)
    {
        CreatPark(park);
    }
    if(!(file=fopen("Park.txt", "r")))
    {
        cout << "Park.txt文件打开失败。" << endl;
        Sleep(1000);
        exit(0);
    }
    fread(&park, sizeof(ParkingLot), 1, file);

    while(1)
    {
        system("cls");
        cout << "\n\n\t\t\t停车场管理系统" << endl << endl;
        cout << "\t\t------------------------------" << endl << endl;
        cout << "\t\t\t 1.信息总览" << endl << endl;
        cout << "\t\t\t 2.查看监控" << endl << endl;
        cout << "\t\t\t 3.查看账单" << endl << endl;
        cout << "\t\t\t 4.查找车辆" << endl << endl;
        cout << "\t\t\t 5.退出系统" << endl << endl;
        cout << "\t\t  请输入序号进行选择：";
        while(cin >> choose1, !(1 <= choose1 && 5 >= choose1) || cin.fail())
        {
            cout << "\t\t  输入不合法，请重新输入：";
            cin.clear();
            cin.sync();
        }
        switch(choose1)
        {
        case 1:
            Survey(park);
            break;
        case 2:
            Monitor(park);
            break;
        case 3:
            CheckAccount(park);
            break;
        case 4:

            break;
        case 5:
            exit(0);
            break;
        default:
            break;
        }
    }

    return 0;
}

int StringToInt( string str )
{
    int i = 0;
    int sum = 0;
    int num = str.length();
    for(i = 0; i < num; i++)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            sum = sum * 10 + int(str[i]) - int('0');
        }
    }
    return sum;
}

void CreatACar( CarInfo &car )
{
    car.ifPark = false;
    car.licensePlate.clear();
    car.brandAndType.clear();
    car.color.clear();
    int ascii;
    car.licensePlate += provAbbr[rand()%31]; // 生成车牌省份简称
    Sleep(1);
    ascii = rand()%26 + 65;
    car.licensePlate.append(1, char(ascii)); // 生成城市字母代号
    int i = 0;
    int flag = 0;
    while(i < 5) // 随机加上车牌后五位数字或字母（最多两个字母）
    {
        Sleep(1);
        switch(rand()%2)
        {
        case 1:
            if(flag < 2)
            {
                ascii = rand()%26 + 65;
                car.licensePlate.append(1, char(ascii)); // 接上字母
                flag++;
                i++;
            }
            break;
        default:
            ascii = rand()%10 + 48;
            car.licensePlate.append(1, char(ascii)); // 接上数字
            i++;
            break;
        }
    }
    car.brandAndType += carType[rand()%20];
    car.color += carColor[rand()%9];
    // cout << car.licensePlate<< "    " << car.brandAndType << "    " << car.color << endl;
}

void CreatPark( ParkingLot &Park )
{
    system("cls");
    Park.allSpace = 0;
    cout << "请输入停车场进出口数：";
    cin >> Park.entranceNum;
    for(int i = 0; i < Park.entranceNum; i++)
    {
        InitQueue(Park.entrances[i].Q);
        Park.entrances[i].ifDischarge = false;
    }
    cout << "请输入停车场停车区域数：";
    cin >> Park.areaNum;
    cout << "请分别输入每个停车区的所有停车位数" << endl;
    for(int i = 0; i < Park.areaNum; i++)
    {
        cout << "第" << i+1 << "区：";
        cin >> Park.areas[i].allSpace;
        Park.areas[i].emptySpace = Park.areas[i].allSpace;
        Park.allSpace += Park.areas[i].allSpace;
    }
    Park.emptySpace = Park.allSpace;
    // 初始化整个停车场无向图
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        Park.paths[i][i].distance = 0; // 自己到自己距离为0
        for(int j = i+1; j < Park.entranceNum + Park.areaNum; j++)
        {
            if(i < Park.entranceNum && j < Park.entranceNum)
            {
                cout << "请输入进出口" << i+1 << "与进出口" << j+1 << "的整数距离（非正或超过" << MAX_LENGTH << "视为不可达）：";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
            if(i < Park.entranceNum && j >= Park.entranceNum)
            {
                cout << "请输入进出口" << i+1 << "与停车区" << j-Park.entranceNum+1 << "的整数距离（非正或超过" << MAX_LENGTH << "视为不可达）：";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
            if(i >= Park.entranceNum && j >= Park.entranceNum)
            {
                cout << "请输入停车区" << i-Park.entranceNum+1 << "与停车区" << j-Park.entranceNum+1 << "的整数距离（非正或超过" << MAX_LENGTH << "视为不可达）：";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
        }
    }
    // 初始化最短路径数组
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        for(int j = i; j < Park.entranceNum + Park.areaNum; j++)
        {
            Park.shortestPaths[i][j] = Park.shortestPaths[j][i] = Park.paths[i][j].distance;
        }
    }
    // Floyd算法求最短路径
    for(int k = 0; k < Park.entranceNum + Park.areaNum; k++)
    {
        for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
        {
            for(int j = 0; j < Park.entranceNum + Park.areaNum; j++)
            {
                if(Park.shortestPaths[i][j] > Park.shortestPaths[i][k] + Park.shortestPaths[k][j])
                {
                    Park.shortestPaths[i][j] = Park.shortestPaths[i][k] + Park.shortestPaths[k][j];
                }
            }
        }
    }

    system("cls");
    cout << endl << "\n\n\t\t停车场建立完毕，正保存到文件中。" << endl;
    Sleep(1000);
    if(!(file=fopen("Park.txt", "w")))
    {
        cout << "Park.txt文件打开失败，无法保存停车场信息。" << endl;
        Sleep(1000);
        exit(0);
    }
    fwrite(&Park, sizeof(ParkingLot), 1, file);
    fclose(file);


    /*
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        for(int j = 0; j < Park.entranceNum + Park.areaNum; j++)
        {
            cout << Park.paths[i][j].distance << '\t';
        }
        cout << endl;
    }
    cout << endl;
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        for(int j = 0; j < Park.entranceNum + Park.areaNum; j++)
        {
            cout << Park.shortestPaths[i][j] << '\t';
        }
        cout << endl;
    }*/
}

bool IfAllEmpty( ParkingLot &Park )
{
    int ifAllEmpty = 0;
    for(int i = 0; i < Park.entranceNum; i++)
    {
        if(Park.entrances[i].Q.front == Park.entrances[i].Q.rear)
            ifAllEmpty++;
    }
    if(ifAllEmpty == Park.entranceNum) return true;
    return false;
}

void Monitor( ParkingLot &Park )
{
    CarInfo car;
    int whichEntra;
    bool ifPark;
    while(1)
    {
        if(rand()%3) // 模拟有新来的车（来车概率是不来车的两倍）
        {
            for(int i = 0; i < rand()%5 + 1; i++) // 每次最多来五辆车
            {
                CreatACar(car);
                whichEntra = rand()%Park.entranceNum; // 随机加入某个进出口排队
                car.whereEnter = whichEntra;
                EnQueue(Park.entrances[whichEntra].Q, car);
            }
        }
        cout << "排队等待中的车辆：" << endl;
        for(int i = 0; i < Park.entranceNum; i++)
        {
            cout << "入口" << i+1 << "：" << endl;
            Traverse(Park.entrances[i].Q);
        }

        if(0 < Park.emptySpace)
        {
            Sleep(1000);
            cout << "停车场中有空位。" << endl;

            if(IfAllEmpty(Park)) continue; // 如果所有的入口都为空，那么等待生成新来的车

            for(int i = 0; i < Park.entranceNum; ) // 所有入口都放行一辆车
            {
                bool flag;
                do{ // 随机一个入口，取第一辆车出队
                    flag = true;
                    whichEntra = rand()%Park.entranceNum;
                    if((Park.entrances[whichEntra].Q.front != Park.entrances[whichEntra].Q.rear) &&
                       !(Park.entrances[whichEntra].ifDischarge) == true) // 队列非空且本次未放行过车辆
                    {
                        DeQueue(Park.entrances[whichEntra].Q, car);
                        Park.entrances[whichEntra].ifDischarge = true;
                        ifPark = true;
                        flag = false;
                        i++;
                    }
                    else if((Park.entrances[whichEntra].Q.front == Park.entrances[whichEntra].Q.rear) &&
                            !(Park.entrances[whichEntra].ifDischarge) == true) // 队列为空且本次未放行过车辆
                    {
                        Park.entrances[whichEntra].ifDischarge = true;
                        ifPark = false;
                        flag = false;
                        i++;
                    }
                    else // 队列为空且本次已放行过车辆
                    {
                        ifPark = false;
                    }
                }while(flag);
                if(ifPark)
                {
                    cout << "在入口" << whichEntra+1 << "排队的第一辆车进入停车场。车牌为"<< car.licensePlate << endl;
                    do{ // 随机找一个空车位停下
                        flag = true;
                        int whichArea = rand()%Park.areaNum;
                        int whichSpace = rand()%Park.areas[whichArea].allSpace;
                        if(!(Park.areas[whichArea].parkingCars[whichSpace].ifPark) == true) // 判断此处是否已经有车停放
                        {
                            car.ifPark = true;
                            Park.areas[whichArea].parkingCars[whichSpace] = car;
                            cout << "该车停入停车区" << whichArea+1 << "的第" << whichSpace+1 << "号停车位。" << endl;
                            Park.areas[whichArea].emptySpace--;
                            Park.emptySpace--;
                            flag = false;
                        }
                    }while(flag);
                }
                if(0 >= Park.emptySpace)
                {
                    cout << endl << "停车场已满。" << endl;
                    break;
                }
            }
            for(int i = 0; i < Park.entranceNum; i++)
            {
                Park.entrances[i].ifDischarge = false;
            }
        }
        else
        {
            Sleep(1000);
            cout << endl << "停车场已满。" << endl;
        }
        Sleep(1000);
    }
}

void Survey( ParkingLot &Park )
{

}

void CheckAccount( ParkingLot &Park )
{

}






