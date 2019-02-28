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

string carColor[9] = {"��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","��ɫ","����ɫ","��ɫ","��ɫ"};
string carType[20] = {"���ͺ�","����","����","����","ɣ����","�µ�","����","ѩ����","����","�ô�����","��������","������","��ʱ��","·��","��������","����ʫ","���","���ӵ�","��������","��˹��˹"};
string provAbbr[31] = {"��","��","��","��","��","ԥ","��","��","��","��","��","³","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��"};
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

    cout << "\n\n\t\t��ӭ����ͣ��������ϵͳ��" << endl << endl;
    Sleep(1000);
    cout << "\t��ѡ���Ƿ����µ�ͣ��������y/Yȷ������������ȡ����" << endl;
    choose2 = getch();
    if('y' == choose2 || 'Y' == choose2)
    {
        CreatPark(park);
    }
    if(!(file=fopen("Park.txt", "r")))
    {
        cout << "Park.txt�ļ���ʧ�ܡ�" << endl;
        Sleep(1000);
        exit(0);
    }
    fread(&park, sizeof(ParkingLot), 1, file);

    while(1)
    {
        system("cls");
        cout << "\n\n\t\t\tͣ��������ϵͳ" << endl << endl;
        cout << "\t\t------------------------------" << endl << endl;
        cout << "\t\t\t 1.��Ϣ����" << endl << endl;
        cout << "\t\t\t 2.�鿴���" << endl << endl;
        cout << "\t\t\t 3.�鿴�˵�" << endl << endl;
        cout << "\t\t\t 4.���ҳ���" << endl << endl;
        cout << "\t\t\t 5.�˳�ϵͳ" << endl << endl;
        cout << "\t\t  ��������Ž���ѡ��";
        while(cin >> choose1, !(1 <= choose1 && 5 >= choose1) || cin.fail())
        {
            cout << "\t\t  ���벻�Ϸ������������룺";
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
    car.licensePlate += provAbbr[rand()%31]; // ���ɳ���ʡ�ݼ��
    Sleep(1);
    ascii = rand()%26 + 65;
    car.licensePlate.append(1, char(ascii)); // ���ɳ�����ĸ����
    int i = 0;
    int flag = 0;
    while(i < 5) // ������ϳ��ƺ���λ���ֻ���ĸ�����������ĸ��
    {
        Sleep(1);
        switch(rand()%2)
        {
        case 1:
            if(flag < 2)
            {
                ascii = rand()%26 + 65;
                car.licensePlate.append(1, char(ascii)); // ������ĸ
                flag++;
                i++;
            }
            break;
        default:
            ascii = rand()%10 + 48;
            car.licensePlate.append(1, char(ascii)); // ��������
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
    cout << "������ͣ��������������";
    cin >> Park.entranceNum;
    for(int i = 0; i < Park.entranceNum; i++)
    {
        InitQueue(Park.entrances[i].Q);
        Park.entrances[i].ifDischarge = false;
    }
    cout << "������ͣ����ͣ����������";
    cin >> Park.areaNum;
    cout << "��ֱ�����ÿ��ͣ����������ͣ��λ��" << endl;
    for(int i = 0; i < Park.areaNum; i++)
    {
        cout << "��" << i+1 << "����";
        cin >> Park.areas[i].allSpace;
        Park.areas[i].emptySpace = Park.areas[i].allSpace;
        Park.allSpace += Park.areas[i].allSpace;
    }
    Park.emptySpace = Park.allSpace;
    // ��ʼ������ͣ��������ͼ
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        Park.paths[i][i].distance = 0; // �Լ����Լ�����Ϊ0
        for(int j = i+1; j < Park.entranceNum + Park.areaNum; j++)
        {
            if(i < Park.entranceNum && j < Park.entranceNum)
            {
                cout << "�����������" << i+1 << "�������" << j+1 << "���������루�����򳬹�" << MAX_LENGTH << "��Ϊ���ɴ��";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
            if(i < Park.entranceNum && j >= Park.entranceNum)
            {
                cout << "�����������" << i+1 << "��ͣ����" << j-Park.entranceNum+1 << "���������루�����򳬹�" << MAX_LENGTH << "��Ϊ���ɴ��";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
            if(i >= Park.entranceNum && j >= Park.entranceNum)
            {
                cout << "������ͣ����" << i-Park.entranceNum+1 << "��ͣ����" << j-Park.entranceNum+1 << "���������루�����򳬹�" << MAX_LENGTH << "��Ϊ���ɴ��";
                cin >> Park.paths[i][j].distance;
                if(0 >= Park.paths[i][j].distance || 200 < Park.paths[i][j].distance)
                    Park.paths[i][j].distance = INFINITY;
                Park.paths[j][i].distance = Park.paths[i][j].distance;
            }
        }
    }
    // ��ʼ�����·������
    for(int i = 0; i < Park.entranceNum + Park.areaNum; i++)
    {
        for(int j = i; j < Park.entranceNum + Park.areaNum; j++)
        {
            Park.shortestPaths[i][j] = Park.shortestPaths[j][i] = Park.paths[i][j].distance;
        }
    }
    // Floyd�㷨�����·��
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
    cout << endl << "\n\n\t\tͣ����������ϣ������浽�ļ��С�" << endl;
    Sleep(1000);
    if(!(file=fopen("Park.txt", "w")))
    {
        cout << "Park.txt�ļ���ʧ�ܣ��޷�����ͣ������Ϣ��" << endl;
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
        if(rand()%3) // ģ���������ĳ������������ǲ�������������
        {
            for(int i = 0; i < rand()%5 + 1; i++) // ÿ�������������
            {
                CreatACar(car);
                whichEntra = rand()%Park.entranceNum; // �������ĳ���������Ŷ�
                car.whereEnter = whichEntra;
                EnQueue(Park.entrances[whichEntra].Q, car);
            }
        }
        cout << "�Ŷӵȴ��еĳ�����" << endl;
        for(int i = 0; i < Park.entranceNum; i++)
        {
            cout << "���" << i+1 << "��" << endl;
            Traverse(Park.entrances[i].Q);
        }

        if(0 < Park.emptySpace)
        {
            Sleep(1000);
            cout << "ͣ�������п�λ��" << endl;

            if(IfAllEmpty(Park)) continue; // ������е���ڶ�Ϊ�գ���ô�ȴ����������ĳ�

            for(int i = 0; i < Park.entranceNum; ) // ������ڶ�����һ����
            {
                bool flag;
                do{ // ���һ����ڣ�ȡ��һ��������
                    flag = true;
                    whichEntra = rand()%Park.entranceNum;
                    if((Park.entrances[whichEntra].Q.front != Park.entrances[whichEntra].Q.rear) &&
                       !(Park.entrances[whichEntra].ifDischarge) == true) // ���зǿ��ұ���δ���й�����
                    {
                        DeQueue(Park.entrances[whichEntra].Q, car);
                        Park.entrances[whichEntra].ifDischarge = true;
                        ifPark = true;
                        flag = false;
                        i++;
                    }
                    else if((Park.entrances[whichEntra].Q.front == Park.entrances[whichEntra].Q.rear) &&
                            !(Park.entrances[whichEntra].ifDischarge) == true) // ����Ϊ���ұ���δ���й�����
                    {
                        Park.entrances[whichEntra].ifDischarge = true;
                        ifPark = false;
                        flag = false;
                        i++;
                    }
                    else // ����Ϊ���ұ����ѷ��й�����
                    {
                        ifPark = false;
                    }
                }while(flag);
                if(ifPark)
                {
                    cout << "�����" << whichEntra+1 << "�Ŷӵĵ�һ��������ͣ����������Ϊ"<< car.licensePlate << endl;
                    do{ // �����һ���ճ�λͣ��
                        flag = true;
                        int whichArea = rand()%Park.areaNum;
                        int whichSpace = rand()%Park.areas[whichArea].allSpace;
                        if(!(Park.areas[whichArea].parkingCars[whichSpace].ifPark) == true) // �жϴ˴��Ƿ��Ѿ��г�ͣ��
                        {
                            car.ifPark = true;
                            Park.areas[whichArea].parkingCars[whichSpace] = car;
                            cout << "�ó�ͣ��ͣ����" << whichArea+1 << "�ĵ�" << whichSpace+1 << "��ͣ��λ��" << endl;
                            Park.areas[whichArea].emptySpace--;
                            Park.emptySpace--;
                            flag = false;
                        }
                    }while(flag);
                }
                if(0 >= Park.emptySpace)
                {
                    cout << endl << "ͣ����������" << endl;
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
            cout << endl << "ͣ����������" << endl;
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






