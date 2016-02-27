/* -----------------------
 * main.cpp
 *
 * Created on: 2016/2/27
 * Author: sjean
 * -----------------------
 */
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#include "pattern.h"
#include "config.h"
#include "board.h"
#include "UCT_search.h"
#include "client.h"

using namespace CJCU;
using namespace std;

ofstream cmd_log("commands_log.txt",ios::app);
ofstream winrateOut("record.csv",ios::app);

int setConfig()
{
	string par_name;
	float par_val;

	ifstream fin("setup.txt", ios::in);

	if (!fin.is_open())
	{
		return false;
	}

	while (!fin.eof())
	{
		fin >> par_name;

		if (par_name == "MaxSimulation")
		{
			fin>> par_val;
			MaxSimulation = (int) par_val;
			cmd_log << "MaxSimulation=" << MaxSimulation << endl;
		}
		else if (par_name == "Pattern_Urgency")
		{
			fin>> par_val;
			Pattern_Urgency = (int) par_val;
			cmd_log << "Pattern_Urgency=" << Pattern_Urgency << endl;
		}
		else if (par_name == "Extension_Urgency")
		{
			fin>> par_val;
			Extension_Urgency = (int) par_val;
			cmd_log << "Extension_Urgency=" << Extension_Urgency << endl;
		}
		else if (par_name == "Capture_Urgency")
		{
			fin>> par_val;
			Capture_Urgency = (int) par_val;
			cmd_log << "Capture_Urgency=" << Capture_Urgency << endl;
		}
		else if (par_name == "Distance_Urgency")
		{
			fin>> par_val;
			Distance_Urgency = (int) par_val;
			cmd_log << "Distance_Urgency=" << Distance_Urgency << endl;
		}
		else if (par_name == "ExEValue")
		{
			fin>> par_val;
			ExEValue = (int) par_val;
			cmd_log << "ExEValue=" << ExEValue << endl;
		}
		else if (par_name == "UCT_Komi")
		{
			fin>> par_val;
			UCT_Komi = par_val;
			cmd_log << "UCT_Komi=" << UCT_Komi << endl;
		}
		else if (par_name == "Middle_Urgency_Limit")
		{
			fin>> par_val;
			MiddleGame_Step_Limit = (int)par_val;
			cmd_log << "Middle_Urgency_Limit=" << MiddleGame_Step_Limit << endl;
		}
		else if (par_name == "Final_Urgency_Limit")
		{
			fin>> par_val;
			EndGame_Step_Limit = (int)par_val;
			cmd_log << "Final_Urgency_Limit=" << EndGame_Step_Limit << endl;
		}
		else if (par_name == "Open_Step_Limit")
		{
			fin>> par_val;
			Open_Step_Limit =(int)par_val;
			cmd_log << "Open_Step_Limit=" << Open_Step_Limit << endl;
		}
		else if (par_name == "IPAddress")
		{
			fin>> IPAddress;
			cmd_log << "IPAddress=" << IPAddress << endl;
		}
	}

	fin.close();

	return true;
}

int main()
{
	setConfig(); // Ū��config
	Pattern *pt = new Pattern();

	/* �PWINGO(SERVER)�s�u */
	const char* ip = IPAddress;
	string Msg = "";
	int socket = -1;

	while (socket == -1)
	{
		cout << "Starting initClient!" << endl;
		socket = initClient(ip, tid);

		if(socket == -1)
		{
			cout<<"InitClient Failed!"<<endl;
			socket = -1;
			Sleep(500);

			continue;
		}

		cout << "socket=" << socket << endl;

		while (socket != -1) // �s�u���_or���w���_���O�e���򵥫�
		{
			int wins[BoardArraySize]= { 0 };// times of winning
			int visits[BoardArraySize] = { 0 };// times of visiting
			int MSQ[M_Sequence_Limit] = { 0 };//move sequence
			int hn = 0;// hand number
			int sn = 0;
			int c = 0; // color

			if (recvBoard(socket, &sn, &hn, &c, MSQ) == -1)//�����L��
			{
				cout << "Receive failed!" << endl;
				break;
			}
			else
			{
				/* Ū��Patten */
				if (pt->GetPattern())
				{
					cmd_log << "Pattern Success!" << endl;
				}
				else
				{
					cmd_log << "Pattern Fail!" << endl;
				}

				/* �X�a�dù */
				cout << "UCTSearching...";
				setConfig();
				MaxSimulation = sn;

				if (c == WHITE)
				{
					cout << "Urgency changed...";
					int temp = Extension_Urgency;
					Extension_Urgency = Capture_Urgency;
					Capture_Urgency = temp;
				}

				UCTSearch(MSQ,hn,wins,visits);

				cout << "OK" << endl;

				//�����Ӳv�� record.csv
				winrateOut << "��" << hn << "��" << endl;
				winrateOut << "�ӧQ����" << endl;

				for (int i = 0; i < BoardArraySize; i++)
				{
					if (i % BoardSize == 0 && i != 0)
					{
						winrateOut << endl;
					}

					winrateOut << wins[i];

					if (i != BoardArraySize-1)
					{
						winrateOut << ",";
					}
				}

				winrateOut << "\r\n���X����" << endl;

				for (int i = 0; i < BoardArraySize; i++)
				{
					if (i % BoardSize == 0 && i != 0)
					{
						winrateOut << endl;
					}

					winrateOut << visits[i];

					if (i != BoardArraySize-1)
					{
						winrateOut << ",";
					}
				}

				winrateOut << "\r\n�Ӳv" << endl;

				for (int i = 0; i < BoardArraySize; i++)
				{
					if (i % BoardSize == 0 && i != 0)
					{
						winrateOut << endl;
					}

					if (visits[i] == 0)
					{
						winrateOut << "0,";
						continue;
					}

					float winrate = (float) wins[i] / (float) visits[i];
					winrateOut << winrate;

					if (i != BoardArraySize-1)
					{
						winrateOut << ",";
					}
				}
				winrateOut << "\r\n" << endl; //��������

				sendResult(socket, wins, visits);
			}
		}

		cout << "This is disconnected with Wingo Server!" << endl;
		socket = -1;
		Sleep(500);
	}
}
