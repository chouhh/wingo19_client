/*
 * config.h
 *
 * Created on: 2013/8/2
 * Author: chouhh
 */

#ifndef UCTCLIENT_0_CONFIG_H_
#define UCTCLIENT_0_CONFIG_H_

#include <string>
#include <map>
#include <set>

using namespace std;

namespace CJCU
{
#define M_Sequence_Limit 400          					//�ۤ�Ӽƭ���
#define Same_Pattern_num 16

#define EMPTY  0
#define BLACK  1
#define WHITE  -1
#define EDGE   8

#define RESIGN -1
#define PASS 0

#define RealBoardSize 19                                //�ѽL��l�j�p
#define BoardSize  (RealBoardSize+2)                   //�[�W�䪺�ѽL�j�p
#define BoardArraySize (BoardSize*BoardSize)           //�@�����ѽL�j�p(�t���)

#define PATTERN_LIMIT 65536                            // �̦h�ѧμ�
#define MAXSTRNUM 172                                  //�̤j�Ѧ�Ӽ�

#define MAX_BUF 1024								   //socket ���q�̤j�ǿ�q

extern int adj_dir[4];
extern int all_dir[8];
extern int extend_dir[16];

extern int MaxSimulation;
extern int Simulation_Limit;

extern int Pattern_Urgency;
extern int Extension_Urgency;
extern int Capture_Urgency;
extern int Distance_Urgency;

extern float UCT_Komi ;
extern int ExEValue ;       //UCB�Ѽ�

extern bool Patterns_3[PATTERN_LIMIT];
extern  set<string> Pattern5b;
extern  set<string> Pattern5w;

extern int Main_Time;
extern int BY_Time;
extern int BY_Stones;

extern int LeftTime;
extern int TotalTime;
extern int PastTime;

extern void setup();

extern int Open_Step_Limit;
extern int MiddleGame_Step_Limit;
extern int EndGame_Step_Limit;

extern int tid;
extern char IPAddress[16];

} //namespace CJCU

#endif /* UCTCLIENT_0_CONFIG_H_ */
