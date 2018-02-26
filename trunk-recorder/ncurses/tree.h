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
void SetCurses(int option, int enable);
void SourceDev(std::string dev, int dig, int ana);
void PurgeArrays();
void StartCall(long tg, long freq, std::string dev, bool isanalog, int nac, bool conventional, int sysnum);
bool StartCurses();
void MissingTG(long tg, int nac, int sysnum);
void KillCall(long tg, long freq, int elapsed, int since, int sys);
void Long(long tg, long freq, int elapsed, int since, int sys);
void Retune(int elasped, int since, long tg, long old_freq, long new_freq, bool isgood);
void DoubleCall(long newtg, long oldtg, long freq, int since, int elapsed);
void SysId(int sysid, bool isconventional, int sysnum, int newsite, int hasGroup, int group);
void UtRef();
void Rate(int mps);
void CursesError();
bool SetupLog();
void ScrRef();
void EndWin();
void setColor(long currid, long tg);
int getcol(int loc);
void msgdata();
void EndCall(long tg, double elapsed, std::string dev, bool conventional, std::string description, int sysnum);
void CallHist(long tg, long elapsed);
void ccId(int sys);
void TimeUp();
void Past(int tg, int elapsed, int color);
void CPU();
void TTime();
void CPUper();
void RecRef();
void MsgRef();
void DatRef();
void OldRef();
void CpuRef();
void MtgRef();
void ErrRef();
void recreate();
void Get_Key();
void MakeWindows();
void Coordinates();
void conventionalStatus(int tg, double length, int idle, bool isidle, std::string dev, int sysnum);
//void NewLogs(std::ostringstream &nls);
//void StartWindows();
int read_fields (FILE *cfp, unsigned long long int *fields);
std::string TTimeParse(int stime);
void NewLog(std::string input);
void NoSource(long freq, long tg, int sys);
void NoRecorder(long freq, long tg, int sys, std::string rad);
void LogRef();
std::string cparse(long input, int space);
void Wav(int msg);
private:
//#include "../talkgroups.h"
void UTnew(int tg, long nac, std::string radio, int length, int color, std::string description);

static void do_resize(int null);
};
typedef std::ostream& (*STRFUNC)(std::ostream&);

class TreeLog
{
public:

    TreeLog()
    {}

    template<typename T>
    TreeLog& operator<<( const T &output )
    {
            //std::stringstream nls;
			nls << output;
			//tout.NewLog(nls.str());
        return *this;
    }
    // for std::endl and other manipulators
    typedef std::ostream& (*STRFUNC)(std::ostream&);
    TreeLog& operator<<( STRFUNC func )
    {
		tout.NewLog(nls.str());
		nls.flush();
       // func(std::cerr);
        return *this;
    }
private:
Tree tout;
std::stringstream nls;
};
#endif
