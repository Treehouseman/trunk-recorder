#ifndef TREE_H
#define TREE_H
#include <boost/log/trivial.hpp>
#include <iostream>
#include <cassert>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <iomanip>
#include <map>
#include <vector>
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <ncurses.h>




class Tree {
	#include<ncurses.h>
public:
void SetCurses(int enable);
void SourceDev(std::string dev, int dig, int ana);
void PurgeArrays();
void StartCall(long tg, long freq, std::string dev, bool isanalog, int nac);
bool StartCurses();
void MissingTG(long tg, int nac);
void KillCall(long tg, long freq, int elapsed, int since, int sys);
void Long(long tg, long freq, int elapsed, int since, int sys);
void Retune(int elasped, int since, long tg, long old_freq, long new_freq, bool isgood);
void DoubleCall(long newtg, long oldtg, long freq, int since, int elapsed);
void SysId(int sysid);
void Rate(int mps);
void CursesError();
bool SetupLog();
void ScrRef();
void EndWin();
void setColor(long currid, long tg);
bool getcol(int loc);
void msgdata();
void EndCall(long tg, int elapsed, std::string dev);
void CallHist(long tg, long elapsed);
void ccId(int sys);
void TimeUp();
void Past(int tg, int elapsed, int color);
void CPU();
void TTime();
void CPUper();
int read_fields (FILE *cfp, unsigned long long int *fields);
std::string TTimeParse(int stime);
void NewLog(std::string input);
void NoSource(long freq, long tg, int sys);
void NoRecorder(long freq, long tg, int sys, std::string rad);
void LogRef();
std::string cparse(long input, int space);
void Wav(int msg);
private:

};
#endif
