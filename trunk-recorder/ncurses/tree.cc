#include "tree.h"
//#include <sys/sysinfo.h>
#include <string>
#include <ncurses.h>
//#include "../talkgroups.h"
#include <sstream>
#include <iostream>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <signal.h>

#define MAX_CPU 128
bool verbose = false;
bool coloren = false;
long analoggroups[100][100][5];//[radio][recorder][tg, length, color, count, nac, sysnum]
long digitalgroups[100][100][5];
long aconventionalgroups[100][100][6];//[radio][recorder][tg,length,color,idle, nac, sysnum]
long dconventionalgroups[100][100][6];
bool recorderused[100][100][4];
long colorgroups[100];
int colorsystems[100];
int currcol = 4;
int sys_id = 0;
char * LogMsgsc[50][400];
int RecStatus[2][100];
double load[3];
WINDOW *create_newwin(int height, int width, int starty, int startx);
WINDOW *RECwin;
WINDOW *SYSwin;
WINDOW *OLDwin;
WINDOW *CPUwin;
WINDOW *DATwin;
WINDOW *LOGwinb;
WINDOW *LOGwin;
WINDOW *MRECwin;
WINDOW *ERRwin;
WINDOW *CLRwin;
WINDOW *UTwin;
WINDOW *UTwinb;
void destroy_win(WINDOW *local_win);
int systemnumber = 0;
int currsys;
char logconvert[500];
int csyscc[3][100];
int minmsg[100];
int maxmsg[100];
int avgmsg[100];
int syscount = 0;
int strecx = 0;
int strecy = 0;
int stmrx;
int stmry = 0;
int sthstx;
int sthsty = 1;
int stlgx;
int stlgy;
int defh = 30;
int defw = 15;
int mrendx;
int stcpux;
int stcpuy = 0;
long Speed[100];
float CPUPer[100];
long Max = 0;
long Min = 0;
int convcount = 0;
int cpus = 0;
int cpuw = defw;
std::ifstream file;
int curseenable = 0;
int ncurses_cpu = 0;
int ncurses_dbg = 0;
int ncurses_group = 0;
int ncurses_lavg = 0;
std::string Radios[10];
std::string LogMsgs[50];
int LogCol[50];
int digrec[100];
int logpos = 0;
int anarec[100];
int aconvrec[100];
int dconvrec[100];
long actTG[10][100];
int radiocount = 0;
int spos = 0;
bool looped = false;
int sysmps[100][60];
int grpmps[100][60];
int sysccc[9][100];
int grpccc[100][100]; //enable, sysid, min, max, avg, color, current
int pastpos = 0;
int history[28][3];
std::string UThistory[20];
int UTcol[30];
int utpos = 0;
int TreeTime[3];
int CallMin[60];
long CallTDay=0;
long CallYDay=0;
int CallDay[10];
long CallTot=0;
int runtime=0;
int AudioMin[60];
long AudioTDay=0;
long AudioYDay=0;
long AudioTot=0;
volatile int AudioBuff = 0;
volatile int CallBuff = 0;
int Call1m=0;
int Call5m=0;
int Call15m=0;
int Call30m=0;
int Audio1m=0;
int Audio5m=0;
int Audio15m=0;
int Audio30m=0;
int Audio1h=0;
char logbuff[100][400];
int Call1h=0;
long TTraw=0;
int mtgcount = 0;
int MTGs[28][2];
int Calloldtot = 0;
int Audiooldtot = 0;
int Audiooldtday = 0;
int Calloldtday = 0;
int Told=0;
int actcall=0;
int sysmpsbuff[10];
volatile int callmax=0;
volatile int audiomax=0;
double cputotavg=0;
unsigned long long int fields[10], total_tick[MAX_CPU], total_tick_old[MAX_CPU], idle[MAX_CPU], idle_old[MAX_CPU], del_total_tick[MAX_CPU], del_idle[MAX_CPU];
int cpusp = 0, countp;
int DoubleErr = 0;
int KillErr = 0;
int RecErr = 0;
int SrcErr = 0;
int TGErr = 0;
int LgErr = 0;
int Wopen = 0;
int Wfail = 0;
int Wrew = 0;
int Wclose = 0;
int Wdrop = 0;
int Wwrite = 0;
int CPU75 = 0;
int CPU90 = 0;
int CPU100 = 0;
int CPU60 = 0;
long unsigned int delayedref = 0;
int t2syscount = 0;



//Start of Window Coordinates
int R1y = 1;
int R2y = 35;
int R1h = 30;
int R2h = 20;
int TGstartx = 1;
int TGstarty = 1;
int TGblockx = 12;
int TGdefx = 2;
int TGendx;
int SYSstartx;
int SYSstarty;
int SYSdefx = 5;
int SYSblockx = 4;
int SYSendx;
int OLDstartx;
int OLDstarty;
int OLDdefx = 15;
int OLDblockx = 0;
int OLDendx;
int CPUstartx;
int CPUstarty;
int CPUdefx = 6;
int CPUblockx = 1;
int CPUendx;
int DATstartx;
int DATstarty;
int DATdefx = 23;
int DATblockx = 0;
int DATendx = 0;
int LOGstartx = 1;
int LOGstarty;
int LOGdefx = 125;
int LOGblockx = 0;
int LOGendx;
int DATblocks = 0;
int CPUblocks = 0;
int TGblocks = 0;
int OLDblocks = 0;
int LOGblocks = 0;
int SYSblocks = 0;
int MTGstartx = 0;
int MTGstarty;
int MTGendx = 0;
int MTGdefx = 10;
int MTGblockx = 0;
int MTGblocks = 0;
int ERRstartx;
int ERRstarty;
int ERRdefx = 17;
int ERRendx;
int ERRblockx = 0;
int ERRblocks = 0;
int UTstartx;
int UTstarty;
int UTblocks = 0;
int UTblockx = 0;
int UTendx;
int UTdefx = 125;
int UTdesclen = 50;
int rSYSblocks = 0;
volatile bool UtWinEn = false;
volatile bool RecWinEn = false;
volatile bool DatWinEn = false;
volatile bool OldWinEn = false;
volatile bool MrecWinEn = false;
volatile bool LogWinEn = false;
volatile bool CpuWinEn = false;
volatile bool SysWinEn = false;
volatile bool ErrWinEn = false;
int scrW = 0;
int scrH = 0;
long long int CPUusage[100][10];
long long int oldCPUusage[100][10];
double cpuavg[100];
bool CPUsetup=false;
bool resized = false;
	FILE *cfp;

bool clearall=true;

//Talkgroups *talk;


void Tree::do_resize(int null){
	resized = true;
}
void Tree::PurgeArrays(){
	for(int i = 0; i < 10; i ++){
		Radios[i]="";
		digrec[i]=0;
		anarec[i]=0;
		sysccc[0][i]=0;
		sysccc[1][i]=0;
		sysccc[2][i]=0;
		sysccc[3][i]=0;
		for(int x = 0; x < 60; x++){
			sysmps[i][x]=0;
		}
		for(int x = 0; x < 100; x++){
			actTG[i][x]=0;
			CPUusage[x][i]=0;
			oldCPUusage[x][i]=0;
			cpuavg[x]=0;
		}
	}
	for(int i = 0; i < 60; i ++){
		AudioMin[i]=0;
		CallMin[i]=0;
	}
	
}
void Tree::Past(int tg, int elapsed, int color){
	if(!curseenable)
		return;
	if(elapsed>audiomax)
		audiomax=elapsed;
	if(pastpos < 28){
		history[pastpos][0]=tg;
		history[pastpos][1]=elapsed;
		history[pastpos][2]=color;
		OldRef();
		AudioBuff = AudioBuff+elapsed;
		CallBuff++;
		pastpos++;
		DatRef();
	}
	else{
		for(int i = 0; i < 27; i++){
			history[i][0]=history[i+1][0];
		history[i][1]=history[i+1][1];
		history[i][2]=history[i+1][2];
		}
		history[27][0]=tg;
		history[27][1]=elapsed;
		history[27][2]=color;
		CallBuff++;
		AudioBuff =AudioBuff+elapsed;
		OldRef();
		DatRef();
	}
}
void Tree::EndCall(long tg, double elapsed, std::string dev, bool conventional, std::string description, int sysnum){
	if(!curseenable)
		return;
	long endid = 0;
	long endcol = 0;
	int elapsedint = 0;
	int cursite = 0;
	if(conventional){
		elapsedint = elapsed+0.5;
		for(int i = 0; i < 100; i++){
			if(csyscc[1][i]==sysnum){
				cursite = csyscc[2][i];
			break;}
		}
	}
	else{
		elapsedint = elapsed;
		for(int i = 0; i < 100; i++){
			if(sysccc[3][i]==sysnum){
				cursite = sysccc[4][i];
			break;}
		}
	}
	if(!conventional){
		for(int i = 0; i < TGblocks; i++){
			if(Radios[i]==dev){
			for(int x = 0; x < 100; x++){
				if(analoggroups[i][x][0]==tg && analoggroups[i][x][4]==sysnum){
					//Put logging code here
					Past(tg, elapsedint, analoggroups[i][x][2]);
					//endid=analoggroups[i][x][3];
					endcol=analoggroups[i][x][2];
					UTnew(tg, cursite, dev, elapsedint, endcol, description);
					analoggroups[i][x][0]=0;
					analoggroups[i][x][1]=0;
					actcall--;
					RecRef();
					return;
				}
				if(digitalgroups[i][x][0]==tg && digitalgroups[i][x][4]==sysnum){
					Past(tg, elapsedint, digitalgroups[i][x][2]);
					//endid=digitalgroups[i][x][3];
					endcol=digitalgroups[i][x][2];
					UTnew(tg, cursite, dev, elapsedint, endcol, description);
					//put logging code here
					digitalgroups[i][x][0]=0;
					digitalgroups[i][x][1]=0;
					actcall--;
					RecRef();
					return;
				}
			}
		}
		}
	}
	if(conventional){
		//TreeLog() << "End of Conventional Call " << tg << " " << elapsed << " " << dev << std::endl;
		for(int i = 0; i < TGblocks; i++){
			if(Radios[i]==dev){
			for(int x = 0; x < 100; x++){
				if(aconventionalgroups[i][x][0]==tg){
					//Put logging code here
					Past(tg, elapsedint, aconventionalgroups[i][x][2]);
					//endid = aconventionalgroups[i][x][4];
					endcol=aconventionalgroups[i][x][2];
					UTnew(tg, cursite, dev, elapsedint, endcol, description);
					aconventionalgroups[i][x][3]=0;
					aconventionalgroups[i][x][1]=0;
					return;
				}
				else if(dconventionalgroups[i][x][0]==tg){
					//Put logging code here
					Past(tg, elapsedint, dconventionalgroups[i][x][2]);
					//endid = dconventionalgroups[i][x][4];
					endcol=dconventionalgroups[i][x][2];
					UTnew(tg, cursite, dev, elapsedint, endcol, description);
					dconventionalgroups[i][x][3]=0;
					dconventionalgroups[i][x][1]=0;
					return;
				}
			}
		}
		}
	}
}
void Tree::conventionalStatus(int tg, double length, int idle, bool isidle, std::string dev, int sysnum){
	
	//TreeLog() << "Got Status Update" << std::endl;
	//return;
	int intlength = 0;
	if(length > 1)
		intlength = length+0.5;
	else
		return;
	//TreeLog() << "Update: " << tg << " " << length << " " << intlength << " " << idle << " " << isidle << std::endl;
	int csyspos=0;
	for(int i = 0; i < 100; i++){
		if(csyscc[1][i]==sysnum){
			csyspos=i;
			break;
		}
	}
	for(int i = 0; i < TGblocks; i++){
		if(Radios[i]==dev){
			//TreeLog() << "Got Radio" << std::endl;
			for(int x = 0; x < 100; x++){
				//TreeLog() << tg << " " << aconventionalgroups[i][x][0] << " " << aconventionalgroups[i][x][2] << " " << csyspos+SYSblocks << std::endl;
				//if(aconventionalgroups[i][x][0]==tg && aconventionalgroups[i][x][2]==csyspos+SYSblocks-1){
					if(aconventionalgroups[i][x][0]==tg && aconventionalgroups[i][x][2] == (csyspos+rSYSblocks)){
					//TreeLog() << "Updating Group" << std::endl;
						aconventionalgroups[i][x][1]=intlength;
						aconventionalgroups[i][x][3]=1;
					}
					else if(dconventionalgroups[i][x][0]==tg && dconventionalgroups[i][x][2] == (csyspos+rSYSblocks)){
					//TreeLog() << "Updating Group" << std::endl;
						dconventionalgroups[i][x][1]=intlength;
						dconventionalgroups[i][x][3]=1;
					}
			}
		}
	}
	//TreeLog() << "Finished status update";
}
void Tree::SetCurses(int option, int enable){
	switch(option){
		case 0:
			curseenable=enable;
		break;
		case 1:
			ncurses_group = enable;
		break;
		case 2:
			ncurses_cpu = enable;
		break;
		case 3:
			ncurses_dbg = enable;
		break;
		case 4:
			ncurses_lavg = enable;
		break;
	}
	//curseenable = enable;
}

void Tree::Rate(int mps){
	/*
	what to do for slow message rate
	*/
}
void Tree::SourceDev(std::string dev, int dig, int ana){
	int comma=0;
	comma=dev.find(',');
	if(comma!=std::string::npos)
		Radios[TGblocks]=dev.substr(4,comma-4);
	else
		Radios[TGblocks]=dev.substr(4);
	digrec[TGblocks]=dig;
	anarec[TGblocks]=ana;
	TGblocks++;
}
void Tree::StartCall(long tg, long freq, std::string dev, bool isanalog, int nac, bool conventional, int sysnum){
	/*
	Call Start loop, TG needs to be put into analog or digital list for dev based on isanalog
	*/
		/*std::stringstream ll;
		std::string lstr;
		ll << dev.substr(4);
		ll >> lstr;
		const char * lchar = lstr.c_str();
		wprintw(LOGwin, lchar);
		wprintw(LOGwin, ",");
	wrefresh(LOGwin);*/
	if(!conventional){
		int syscolor = -1;
			for(int i = 0; i < 100; i++){
			if(sysccc[0][i]==sys_id){
				syscolor = sysccc[7][i];
				break;
			}
		}
		if(isanalog){
			for(int i = 0; i < TGblocks; i++){
				if(dev == Radios[i]){
					for(int x = 0; x < anarec[i]; x++){
						if(analoggroups[i][x][0]==0){
							analoggroups[i][x][0]=tg;
							analoggroups[i][x][2]=syscolor;
							analoggroups[i][x][3]=nac;
							analoggroups[i][x][4]=sysnum;
							actcall++;
							if(actcall > callmax)
								callmax=actcall;
							RecRef();
							return;
						}
					}
				}
			}
		}
		else{
			for(int i = 0; i < TGblocks; i++){
				if(dev == Radios[i]){
					for(int x = 0; x < digrec[i]; x++){
						if(digitalgroups[i][x][0]==0){
							digitalgroups[i][x][2]=syscolor;
							digitalgroups[i][x][0]=tg;
							digitalgroups[i][x][3]=nac;
							digitalgroups[i][x][4]=sysnum;
							actcall++;
							if(actcall>callmax)
								callmax=actcall;
							RecRef();
							return;
						}
					}
				}
			}
		}
	}
	else if(conventional){
		//TreeLog() << "Start conventional call " << tg << " " << nac << " " << dev << std::endl;
		int syscolor = -1;
		for(int i = 0; i < 100; i++){
			if(csyscc[0][i]==nac && csyscc[1][i]==sysnum){
				syscolor=csyscc[3][i];
				break;
			}
		}
		if(isanalog){
		for(int i = 0; i < TGblocks; i++){
			if(dev == Radios[i]){
				//anarec[i]--;
					if(aconventionalgroups[i][aconvrec[i]][0]==0){
						aconventionalgroups[i][aconvrec[i]][0]=tg;
						aconventionalgroups[i][aconvrec[i]][2]=syscolor;
						aconventionalgroups[i][aconvrec[i]][3]=0;//idle
						aconventionalgroups[i][aconvrec[i]][4]=nac;
						aconventionalgroups[i][aconvrec[i]][5]=sysnum;
						aconvrec[i]++;
						RecRef();
						return;
				}
			}
		}
		}
		else{
			for(int i = 0; i < TGblocks; i++){
			if(dev == Radios[i]){
				//anarec[i]--;
					if(aconventionalgroups[i][dconvrec[i]][0]==0){
						dconventionalgroups[i][dconvrec[i]][0]=tg;
						dconventionalgroups[i][dconvrec[i]][2]=syscolor;
						dconventionalgroups[i][dconvrec[i]][3]=0;//idle
						dconventionalgroups[i][dconvrec[i]][4]=nac;
						dconventionalgroups[i][dconvrec[i]][5]=sysnum;
						dconvrec[i]++;
						RecRef();
						return;
				}
			}
		}
			
		}
	}
}
void Tree::MissingTG(long tg, int nac, int sysnum){
	if(!curseenable)
		return;
	TGErr++;
	int syscolor = -1;
		for(int i = 0; i < 10; i++){
		if(sysccc[0][i]==nac){
			syscolor = sysccc[7][i];
			break;
		}
	}
	/*
	loop for identifying missing TG, should still trigger call start loop
	*/
	for(int i = 0; i < 28; i++){
		if(tg == MTGs[i][0]){
			return;
		}
	}
	if(mtgcount < 28){
		MTGs[mtgcount][0]=tg;
		MTGs[mtgcount][1]=syscolor;
		MtgRef();
		mtgcount++;
	}
	else{
		for(int i = 0; i < 27; i++){
			MTGs[i][0]=MTGs[i+1][0];
			MTGs[i][1]=MTGs[i+1][1];
		}
		MTGs[27][0]=tg;
		MTGs[27][1]=syscolor;
		MtgRef();
	}
	ErrRef();
}
void Tree::Retune(int elasped, int since, long tg, long old_freq, long new_freq, bool isgood){
	/*
	Retune loop, for call tg, inactive for since seconds, call elapsed seconds long
	going from old_freq to new_freq and olddev to newdev, and the retune was successful according
	to isgood
	*/
}
void Tree::DoubleCall(long newtg, long oldtg, long freq, int since, int elapsed){
	/*
	A new TG was found on a currently used frequency, the old tg call is killed, a new one is started
	though startrecorder, we really only have to worry about the old call ending
	*/
	std::stringstream ds;
	ds << "Found new call on active frequency, Freq: " << freq << " Old TG: " << oldtg << " New TG: " << newtg << " Since update: " << since << "s Elapsed: " << elapsed << "s";
	NewLog(ds.str());
	DoubleErr++;
}
void Tree::KillCall(long tg, long freq, int elapsed, int since, int sys){
	/*
	Calls killed on inactivity, this doesn't include radio info, will have to scan for tg at least
	and maybe time
	*/
	std::stringstream ks;
	ks << "Call Killed on Freq: " << freq << " TG: " << tg << " Elapsed: " << elapsed << "s Since: " << since << "s Sys: " << std::hex << std::uppercase << sys << std::nouppercase << std::dec ;
	NewLog(ks.str());
	KillErr++;
}
void Tree::Long(long tg, long freq, int elapsed, int since, int sys){
	/*
	Calls killed on inactivity, this doesn't include radio info, will have to scan for tg at least
	and maybe time
	*/
	std::stringstream ks;
	ks << "Long Call on Freq: " << freq << " TG: " << tg << " Elapsed: " << elapsed << "s Since: " << since << "s Sys: " << std::hex << std::uppercase << sys << std::nouppercase << std::dec ;
	NewLog(ks.str());
	LgErr++;
}
void Tree::SysId(int sysid, bool conventional, int sysnum, int syssite, int hasGroup, int group, int hide){
	/*
	Copy our sysid tracking stuff here
	*/
	if(hide){
		sysccc[8][SYSblocks]=1;
		t2syscount--;
	}
	else{
		sysccc[8][SYSblocks]=0;
	}
	if(hasGroup != 0){
		if(grpccc[0][group]==false){
			t2syscount++;
		}
		grpccc[0][group]=true;
		grpccc[1][group]=sysid;
		grpccc[5][group]=getcol(t2syscount-1);
		sysccc[5][SYSblocks]=true;
		sysccc[6][SYSblocks]=group;
	}
	else{
		t2syscount++;
		sysccc[5][SYSblocks]=false;
	}
	if(!conventional){
			sysccc[0][SYSblocks]=sysid;
			sysccc[3][SYSblocks]=sysnum;
			sysccc[4][SYSblocks]=syssite;
			sysccc[7][SYSblocks]=getcol(t2syscount-1);
			//sysmps[0][SYSblocks]=sysid;
			//sysccc[1][SYSblocks]++;
	SYSblocks++;
	}
	else if(conventional){
		csyscc[0][convcount]=sysid;
		csyscc[1][convcount]=sysnum;
		csyscc[2][convcount]=syssite;
		csyscc[3][convcount]=getcol(t2syscount-1);
		convcount++;
	}
	rSYSblocks = SYSblocks;
	for(int i = 0; i < SYSblocks; i++){
		if(sysccc[5][i]==true || sysccc[8][i]==true)
			rSYSblocks--;
	}
	for(int i = 0; i < 100; i++){
		if(grpccc[0][i]==true)
			rSYSblocks++;
	}
}
void Tree::ccId(int sysnum){
	if(!curseenable)
		return;
	//Got ID in message
	for(int i = 0; i < 100; i++){
		if(sysccc[3][i]==sysnum){
			if(sysccc[5][i]){
				grpccc[6][sysccc[6][i]]++;
			}
			sys_id = sysccc[0][i];
			sysccc[1][i] = sysccc[1][i]+1;
			break;
		}
	}

}
void Tree::Coordinates(){
	int ypos = 0;
	int xpos = 0;
	switch(ncurses_group){
		case 0:
			RecWinEn=true;
			DatWinEn=true;
			CpuWinEn=true;
			LogWinEn=true;
			OldWinEn=true;
			MrecWinEn=true;
			SysWinEn=true;
			ErrWinEn=true;
			R2y=32;
			R2h=20;
			TGstarty = R1y;
			DATstarty = R1y;
			CPUstarty = R1y;
			OLDstarty = R1y;
			SYSstarty = R1y;
			MTGstarty = R1y;
			ERRstarty = R1y;
			LOGstarty = R2y;
			TGendx = TGstartx+TGdefx+(TGblockx*TGblocks); //now we have the end of our TG window!
			SYSstartx = TGendx+1;
			SYSendx = SYSstartx+SYSdefx+(SYSblockx*rSYSblocks);
			DATstartx = SYSendx + 1;
			DATendx = DATstartx+DATdefx+(DATblockx*DATblocks);
			OLDstartx = DATendx+1;
			OLDendx = OLDstartx+OLDdefx+(OLDblockx*OLDblocks);
			CPUstartx = OLDendx + 1;
			if(ncurses_cpu){
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
			}
			else{
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1);
			}
			MTGstartx = CPUendx+1;
			MTGendx = MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
			ERRstartx = MTGendx+1;
			ERRendx = ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
			LOGstartx=1;
			LOGendx = LOGstartx+LOGdefx+(LOGblockx*LOGblocks);
			clearall=true;
		break;
		case 1:
			R1y = 1;
			RecWinEn=true;
			DatWinEn=true;
			SysWinEn=true;
			DATstarty = R1y;
			SYSstarty = R1y;
			TGstarty = R1y;
			TGendx = TGstartx+TGdefx+(TGblockx*TGblocks);
			xpos = TGendx;
			SYSstartx=xpos+1;
			SYSendx=SYSstartx+SYSdefx+(SYSblockx*rSYSblocks);
			xpos = SYSendx;
			DATstartx=xpos+1;
			DATendx=DATstartx+DATdefx+(DATblockx*DATblocks);
			clearall=true;
		break;
		case 2:
			R1y = 1;
			OldWinEn=true;
			CpuWinEn=true;
			MrecWinEn=true;
			CPUstarty = R1y;
			OLDstarty = R1y;
			MTGstarty = R1y;
			OLDstartx=xpos+1;
			OLDendx=OLDstartx+OLDdefx+(OLDblockx*OLDblocks);
			xpos=OLDendx;
			CPUstartx=xpos+1;
			if(ncurses_cpu){
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
			}
			else{
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1);
			}
			xpos=CPUendx;
			MTGstartx=xpos+1;
			MTGendx=MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
			xpos=MTGendx;
		break;
		case 3:
			R1y = 1;
			ErrWinEn=true;
			ERRstarty = R1y;
			LOGstarty = R1y;
			ERRstartx=xpos+1;
			ERRendx=ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
			xpos=ERRendx;
			LogWinEn=true;
			R2h=R1h;
			LOGstartx=xpos+1;
			LOGendx=LOGstartx+50+(LOGblockx*LOGblocks);
			xpos=LOGendx;
		break;
		case 4:
			RecWinEn=true;
			DatWinEn=true;
			CpuWinEn=true;
			OldWinEn=true;
			MrecWinEn=true;
			SysWinEn=true;
			ErrWinEn=true;
			LogWinEn=true;
			R1y = 1;
			R2y=32;
			R2h=30;
			TGstarty = R1y;
			DATstarty = R2y;
			CPUstarty = R2y;
			OLDstarty = R2y;
			SYSstarty = R2y;
			MTGstarty = R2y;
			ERRstarty = R2y;
			TGendx = TGstartx+TGdefx+(TGblockx*TGblocks); //now we have the end of our TG window!
			SYSstartx = TGstartx;
			SYSendx = SYSstartx+SYSdefx+(SYSblockx*rSYSblocks);
			DATstartx = SYSendx + 1;
			DATendx = DATstartx+DATdefx+(DATblockx*DATblocks);
			OLDstartx = DATendx+1;
			OLDendx = OLDstartx+OLDdefx+(OLDblockx*OLDblocks);
			CPUstartx = OLDendx + 1;
			if(ncurses_cpu){
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
			}
			else{
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1);
			}
			MTGstartx = CPUendx+1;
			MTGendx = MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
			ERRstartx = MTGendx+1;
			ERRendx = ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
			LOGstartx=ERRendx+1;
			LOGendx=LOGstartx+40+(LOGblockx*LOGblocks);
			xpos=LOGendx;
		break;
		case 5:
			RecWinEn=true;
			UtWinEn=true;
			R1y=1;
			R2y=32;
			R2h=20;
			R1h=30;
			UTdesclen=100;
			TGstarty=R1y;
			TGstartx=1;
			TGendx=TGstartx+TGdefx+(TGblockx*TGblocks);
			UTstarty=R2y;
			UTstartx=1;
			UTendx=UTstartx+UTdefx+(UTblockx*UTblocks);
		break;
		case 6:
			RecWinEn=true;
			UtWinEn=true;
			LogWinEn=true;
			R1y=1;
			R2y=32;
			R2h=20;
			R1h=30;
			UTdesclen=32;
			TGstarty=R1y;
			TGstartx=1;
			TGendx=TGstartx+TGdefx+(TGblockx*TGblocks);
			UTstarty=R2y;
			UTstartx=1;
			UTendx=UTstartx+75+(UTblockx*UTblocks);
			LOGstartx=UTendx+1;
			LOGendx=LOGstartx+50+(LOGblockx*LOGblocks);
		break;
		case 7:
			RecWinEn=true;
			DatWinEn=true;
			CpuWinEn=true;
			UtWinEn=true;
			MrecWinEn=true;
			SysWinEn=true;
			ErrWinEn=true;
			R2y=32;
			R2h=22;
			UTdesclen=100;
			TGstarty = R1y;
			DATstarty = R1y;
			CPUstarty = R1y;
			SYSstarty = R1y;
			MTGstarty = R1y;
			ERRstarty = R1y;
			LOGstarty = R2y;
			UTstarty=R2y;
			TGendx = TGstartx+TGdefx+(TGblockx*TGblocks); //now we have the end of our TG window!
			SYSstartx = TGendx+1;
			SYSendx = SYSstartx+SYSdefx+(SYSblockx*rSYSblocks);
			DATstartx = SYSendx + 1;
			DATendx = DATstartx+DATdefx+(DATblockx*DATblocks);
			CPUstartx = DATendx + 1;
			if(ncurses_cpu){
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
			}
			else{
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1);
			}
			MTGstartx = CPUendx+1;
			MTGendx = MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
			ERRstartx = MTGendx+1;
			ERRendx = ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
			UTstartx=1;
			UTendx = UTstartx+UTdefx+(UTblockx*UTblocks);
		break;
		case 8:
			RecWinEn=true;
			DatWinEn=true;
			CpuWinEn=true;
			UtWinEn=true;
			MrecWinEn=true;
			SysWinEn=true;
			ErrWinEn=true;
			LogWinEn=true;
			R2y=32;
			R2h=22;
			UTdesclen=32;
			TGstarty = R1y;
			DATstarty = R1y;
			CPUstarty = R1y;
			SYSstarty = R1y;
			MTGstarty = R1y;
			ERRstarty = R1y;
			LOGstarty = R2y;
			UTstarty=R2y;
			TGendx = TGstartx+TGdefx+(TGblockx*TGblocks); //now we have the end of our TG window!
			SYSstartx = TGendx+1;
			SYSendx = SYSstartx+SYSdefx+(SYSblockx*rSYSblocks);
			DATstartx = SYSendx + 1;
			DATendx = DATstartx+DATdefx+(DATblockx*DATblocks);
			CPUstartx = DATendx + 1;
			if(ncurses_cpu){
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
			}
			else{
				if(ncurses_lavg)
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1)+3;
				else
					CPUendx = CPUstartx+CPUdefx+(CPUblockx*1);
			}
			MTGstartx = CPUendx+1;
			MTGendx = MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
			ERRstartx = MTGendx+1;
			ERRendx = ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
			UTstartx=1;
			UTendx = UTstartx+75+(UTblockx*UTblocks);
			LOGstartx=UTendx+1;
			LOGendx = LOGstartx+50+(LOGblockx*LOGblocks);
		break;
		
	}
	
}
void Tree::recreate(){
	if(RecWinEn)
		destroy_win(RECwin);
	if(CpuWinEn)
		destroy_win(CPUwin);
	if(MrecWinEn)
		destroy_win(MRECwin);
	if(LogWinEn){
		destroy_win(LOGwin);
		destroy_win(LOGwinb);
	}
	if(DatWinEn)
		destroy_win(DATwin);
	if(OldWinEn)
		destroy_win(OLDwin);
	if(ErrWinEn)
		destroy_win(ERRwin);
	if(SysWinEn)
		destroy_win(SYSwin);
	if(UtWinEn){
		destroy_win(UTwin);
		destroy_win(UTwinb);
	}
	RecWinEn=false;
	CpuWinEn=false;
	MrecWinEn=false;
	LogWinEn=false;
	DatWinEn=false;
	OldWinEn=false;
	ErrWinEn=false;
	SysWinEn=false;
	UtWinEn=false;
	Coordinates();
	MakeWindows();
}
bool Tree::StartCurses(){
	if(!curseenable)
		return false;
	
	if(CPUblocks == 0){
		const char* match;
		std::string temp;
		file.open("/proc/cpuinfo");
		while(std::getline(file, temp)){
			const char *temp2 = temp.c_str();
			match = strstr (temp2, "cpu MHz");
			if(match != NULL){
				CPUblocks++;
			}
			temp = "";
		}
		file.close();
	}
	//We need to prep our window sizes HERE
	//WINstartx = pWINendx+1
	//WINendx = WINstartx+WINdefx+(WINblockx*blocks)
	
	for(int i = 0; i < 100; i++){
		maxmsg[i] = 0;
		minmsg[i] = 0;
		avgmsg[i] = 0;
		RecStatus[0][i]=0;
		RecStatus[1][i]=0;
		for(int x = 0; x < 60; x++){
			sysmps[i][x]=0;
		}
	}
	for(int i = 0; i<50; i++){
		LogMsgsc[i][0]='\0';
	}
	Coordinates();
//newterm(NULL, stdout, stdin);
//}
//void Tree::StartWindows(){
	initscr();			/* Start curses mode 		*/
	clear();
	nodelay(stdscr, TRUE);
	noecho();
	//timeout(1);
	cbreak();
	//resizeterm(60,130);
	curs_set(0);
	//if (has_colors){
		coloren = true;
		start_color();
		use_default_colors();
		/*init_pair(1,   2, COLOR_BLACK); //Green
		init_pair(2,   1, COLOR_BLACK); //Red
		init_pair(3,   6, COLOR_BLACK); //Cyan
		init_pair(4,   7, COLOR_BLACK); //White
		init_pair(5,   5, COLOR_BLACK); //Magenta
		init_pair(6,   4, COLOR_BLACK); //Blue
		init_pair(7,   3, COLOR_BLACK); //Yellow
		init_pair(8, 214, COLOR_BLACK); //Orange*/
		for(int i = 0; i < 255; i++){
			init_pair(i, i, COLOR_BLACK);
		}
	//}
	//cbreak();			/* Line buffering disabled, Pass on
					// * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	refresh();//This before resize makes sure there's no lingering text, might be a screen issue rather than terminal, given how screen acts with ncurses
	//resizeterm(60,130);
		MakeWindows();
	ScrRef();
	signal(SIGWINCH, do_resize);
	return 1;
}
void Tree::MakeWindows(){
	if(RecWinEn){
		RECwin = create_newwin(R1h, TGendx-TGstartx, TGstarty, TGstartx);
		wbkgd(RECwin, COLOR_PAIR(4));
	}
	if(SysWinEn){
		SYSwin = create_newwin(R1h, SYSendx-SYSstartx, SYSstarty, SYSstartx);
		wbkgd(SYSwin, COLOR_PAIR(4));
	}
	if(OldWinEn){
		OLDwin = create_newwin(R1h, OLDendx-OLDstartx, OLDstarty, OLDstartx);
		wbkgd(OLDwin, COLOR_PAIR(4));
	}
	if(CpuWinEn){
		CPUwin = create_newwin(R1h, CPUendx-CPUstartx, CPUstarty, CPUstartx);
		wbkgd(CPUwin, COLOR_PAIR(4));
	}
	if(DatWinEn){
		DATwin = create_newwin(R1h, DATendx-DATstartx, DATstarty, DATstartx);
		wbkgd(DATwin, COLOR_PAIR(4));
	}
	if(MrecWinEn){
		MRECwin = create_newwin(R1h, MTGendx-MTGstartx, MTGstarty, MTGstartx);
		wbkgd(MRECwin, COLOR_PAIR(4));
	}
	if(ErrWinEn){
		ERRwin = create_newwin(R1h, ERRendx-ERRstartx, ERRstarty, ERRstartx);
		wbkgd(ERRwin, COLOR_PAIR(4));
	}
	if(LogWinEn){
		LOGwinb = create_newwin(R2h, LOGendx-LOGstartx, LOGstarty, LOGstartx);//This is the border for the log window
		LOGwin = create_newwin(R2h-2, LOGendx-LOGstartx-2, LOGstarty+1, LOGstartx+1); //This is the actual logging window
		wbkgd(LOGwin, COLOR_PAIR(4));
		wbkgd(LOGwinb, COLOR_PAIR(4));
		//scrollok(LOGwin, true);
		LogRef();
	}
	if(UtWinEn){
		UTwinb = create_newwin(R2h, UTendx-UTstartx, UTstarty, UTstartx);//This is the border for the log window
		UTwin = create_newwin(R2h-2, UTendx-UTstartx-2, UTstarty+1, UTstartx+1); //This is the actual logging window
		wbkgd(UTwin, COLOR_PAIR(4));
		wbkgd(UTwinb, COLOR_PAIR(4));
		//scrollok(UTwin, true);
		UtRef();
	}
	clearall=true;
	ScrRef();
	delayedref=runtime+2;
}
void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
void Tree::CursesError(){
	if(!curseenable)
		return;
	if(verbose)
	BOOST_LOG_TRIVIAL(error) << "Ncurses Error!";
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

bool Tree::SetupLog(){
	for(int x = 0; x < 10; x++){
		for (int y = 0; y < 100; y++){
			for(int z = 0; z < 2; z++){
				analoggroups[x][y][z] = 0;
			}
		}
	}
	return true;
}

void Tree::TimeUp(){
	if(!curseenable)
		return;
	/*std::string teststr;
	for(int i = 0; i < runtime+50; i ++){
		teststr+="abcdefghijklmnopqrstuvwxyz";
	}
	NewLog(teststr);*/
	//std::cout << "finished test" << std::endl;
		msgdata();

		if(spos < 59){
		spos++;
	}
	else{
		spos = 0;
		looped = true;
	}
	runtime++;
	for(int x = 0; x < TGblocks; x++){
		for(int y = 0; y < 100; y++){
			if(analoggroups[x][y][0]!=0){
				analoggroups[x][y][1]++;
			}
			if(digitalgroups[x][y][0]!=0){
				digitalgroups[x][y][1]++;
			}
		}
	}
		CPUper();

}
void Tree::Get_Key(){
	char c = getch();
	if (c==ERR)
		return;
	/*if('0' <= c <= '9'){
		int group = c-'0';
		ncurses_group=group;
		resized=true;
	}*/
	switch(c){
		case '0':
			if(ncurses_group==0)
				break;
			ncurses_group=0;
			resized=true;
			NewLog("Group 0");
		break;
		case '1':
			if(ncurses_group==1)
				break;
			ncurses_group=1;
			resized=true;
			NewLog("Group 1");
		break;
		case '2':
			if(ncurses_group==2)
				break;
			ncurses_group=2;
			resized=true;
			NewLog("Group 2");
		break;
		case '3':
			if(ncurses_group==3)
				break;
			ncurses_group=3;
			resized=true;
			NewLog("Group 3");
		break;
		case '4':
			if(ncurses_group==4)
				break;
			ncurses_group=4;
			resized=true;
			NewLog("Group 4");
		break;
		case '5':
			if(ncurses_group==5)
				break;
			ncurses_group=5;
			resized=true;
			NewLog("Group 5");
		break;
		case '6':
			if(ncurses_group==6)
				break;
			ncurses_group=6;
			resized=true;
			NewLog("Group 6");
		break;
		case '7':
			if(ncurses_group==7)
				break;
			ncurses_group=7;
			resized=true;
			NewLog("Group 7");
		break;
		case '8':
			if(ncurses_group==8)
				break;
			ncurses_group=8;
			resized=true;
			NewLog("Group 8");
		break;
		case 'r':
			clearall=true;
			NewLog("Cleared Screen");
		break;
		case 'R':
			resized=true;
			NewLog("Resized Screen");
		break;
		case 'c':
			if(CpuWinEn&&ncurses_cpu==1)
				resized=true;
			ncurses_cpu=0;
			NewLog("CPU Average");
		break;
		case 'C':
			if(CpuWinEn&&ncurses_cpu==0)
				resized=true;
			ncurses_cpu=1;
			NewLog("Full CPU");
		break;
		case 'v':
			if(CpuWinEn&&ncurses_lavg==1)
				resized=true;
			ncurses_lavg=0;
			NewLog("Load Average Off");
		break;
		case 'V':
			if(CpuWinEn&&ncurses_lavg==0)
				resized=true;
			ncurses_lavg=1;
			NewLog("Load Average On");
		break;
		default:
			//std::stringstream keyboardin;
			//std::string keyboardinstr
			//keyboardin << c ;
			//NewLog(keyboardin.str());
			TreeLog() << "@4-" << c << std::endl;
		break;
	}
}
void Tree::ScrRef(){
	if(runtime==delayedref){
		clearall=true;
		delayedref=0;
	}
	//NewLog("This is a really really really really long message to test if the scrolling still works and if the stupid border appears! and that wasn't long enough");
		//signal(SIGWINCH, do_resize);
	if(!curseenable)
		return;
	if(resized){
		//scrollok(LOGwin, false);
		resized = false;
		//recreate();
		clear();
		endwin();
		//initscr();
		//resizeterm(scrW, scrH);
		recreate();
		//clear();
		//CLRwin = create_newwin(256, 256, 0, 0);
		//wborder(CLRwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
		//wrefresh(CLRwin);
		//destroy_win(CLRwin);
		//erase();
		//refresh();
		//clearall=true;
		//scrollok(LOGwin, true);
		//ScrRef();
		//wresize(LOGwinb, R2h, 127);
		//wresize(LOGwin, R2h-2, 125);
		//LogRef();
		//clear();
		//erase();
		//flash();
	}
	if(clearall){
		//clear();
		LogRef();
		clearall=false;
	}
	//endwin();
	//clear();
	//refresh();
	TTime();
	RecRef();
	MsgRef();
	DatRef();
	OldRef();
	CpuRef();
	MtgRef();
	ErrRef();
	UtRef();
	//LogRef();
	//wmove(LOGwin, 0, 0);
	refresh();
	//if(delayedref>0)
	//	delayedref=0;
//		ScrRef();
}
void Tree::EndWin(){
	endwin();
			fclose(cfp);
}
void Tree::setColor(long currid, long tg){
	return;
				//for(int i = 0; i < 100; i ++){
				//if(activegroups[i]==tg){	
				//colorsystems[i]=currid;
				//}
				//}
}

int Tree::getcol(int loc){
	//if(!curseenable)
		//return 4;
	switch (loc){
		case -1:
		return 7;
		break;
		case 0:
		return 2;
		break;
		case 1:
		return 3;
		break;
		case 2:
		return 5;
		break;
		case 3:
		return 214;
		break;
		case 4:
		return 40;
		break;
		case 5:
		return 219;
		break;
		case 6:
		return 149;
		break;
		case 7:
		return 83;
		break;
		case 8:
		return 165;
		break;
		case 9:
		return 160;
		break;
		case 10:
		return 49;
		break;
		case 11:
		return 119;
		break;
		case 12:
		return 196;
		break;
		case 13:
		return 193;
		break;
		case 14:
		return 227;
		break;
		case 15:
		return 155;
		break;
		case 16:
		return 85;
		break;
		case 17:
		return 185;
		break;
		default:
		return 4;
		break;
	}
	return 4;
	
}
void Tree::msgdata(){
	if(!curseenable)
		return;
	
	if(looped){
	for(int i = 0; i < SYSblocks; i++){
			if(sysccc[1][i]>80)
				sysccc[1][i]=80;
			sysmps[i][spos] = sysccc[1][i];
			sysmpsbuff[i]=sysccc[1][i];
			sysccc[1][i]=0;
	}
	for(int i = 0; i < 100; i++){
		if(grpccc[0][i]){
			if(grpccc[2][i]>80)
				grpccc[2][i]=80;
			grpmps[i][spos] = grpccc[6][i];
			grpccc[6][i]=0;
		}
	}
	for(int i = 0; i < 100; i++){
		int minm = 80;
		for(int x = 0; x < 60; x++){
			if(sysmps[i][x] < minm){
				minm = sysmps[i][x];
			}
		}
		minmsg[i]=minm;
	}
	for(int i = 0; i < 100; i++){
		int minm = 80;
		if(grpccc[0][i]){
		for(int x = 0; x < 60; x++){
			if(grpmps[i][x] < minm){
				minm = grpmps[i][x];
			}
		}
		grpccc[2][i]=minm;
		}
	}
	for(int i = 0; i < 100; i++){
		int maxm = 0;
		for(int x = 0; x < 60; x++){
			if(sysmps[i][x] > maxm){
				maxm = sysmps[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 100; i++){
		int maxm = 0;
		if(grpccc[0][i]){
		for(int x = 0; x < 60; x++){
			if(grpmps[i][x] > maxm){
				maxm = grpmps[i][x];
			}
		}
		grpccc[3][i]=maxm;
		}
	}
	for(int i = 0; i < 100; i++){
		int avgm = 0;
		for(int x = 0; x < 60; x++){
			avgm = avgm + sysmps[i][x];
		}
		avgmsg[i]=avgm/60;
	}
	for(int i = 0; i < 100; i++){
		int avgm = 0;
		if(grpccc[0][i]){
		for(int x = 0; x < 60; x++){
			avgm = avgm + grpmps[i][x];
		}
		grpccc[4][i]=avgm/60;
		}
	}
	}
	else{
	for(int i = 0; i < SYSblocks; i++){
			sysmps[i][spos] = sysccc[1][i];
			sysmpsbuff[i]=sysccc[1][i];
			//sysmps[x][spos] = rand();
			sysccc[1][i]=0;
	}
	for(int i = 0; i < 100; i++){
		if(grpccc[0][i]){
			grpmps[i][spos] = grpccc[6][i];
			grpccc[6][i]=0;
		}
	}
	for(int i = 0; i < 100; i++){
		int minm = 80;
		for(int x = 0; x < spos; x++){
			if(sysmps[i][x] < minm){
				minm = sysmps[i][x];
			}
		}
		if(spos!=0)
			minmsg[i]=minm;
	}
	for(int i = 0; i < 100; i++){
		int minm = 80;
		if(grpccc[0][i]){
		for(int x = 0; x < spos; x++){
			if(grpmps[i][x] < minm){
				minm = grpmps[i][x];
			}
		}
		if(spos!=0)
			grpccc[2][i]=minm;
		}
	}
	for(int i = 0; i < 100; i++){
		int maxm = 0;
		for(int x = 0; x < spos; x++){
			if(sysmps[i][x] > maxm){
				maxm = sysmps[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 100; i++){
		int maxm = 0;
		if(grpccc[0][i]){
		for(int x = 0; x < spos; x++){
			if(grpmps[i][x] > maxm){
				maxm = grpmps[i][x];
			}
		}
		grpccc[3][i]=maxm;
		}
	}
	for(int i = 0; i < 100; i++){
		int avgm = 0;
		for(int x = 0; x < spos; x++){
			avgm = avgm + sysmps[i][x];
		}
		if(spos != 0)
		avgmsg[i]=avgm/spos;
	}
	for(int i = 0; i < 100; i++){
		int avgm = 0;
		if(grpccc[0][i]){
		for(int x = 0; x < spos; x++){
			avgm = avgm + grpmps[i][x];
		}
		if(spos != 0)
		grpccc[4][i]=avgm/spos;
		}
	}
}
}
void Tree::CallHist(long tg, long elapsed){
	return;
	/*
	std::stringstream ss;
	std::string s;
	ss << tg << " " << elapsed << "s\n";
	s = ss.str();
	const char * c = s.c_str();
	wprintw(hist_win, c);
	wrefresh(hist_win);*/
}

void Tree::CPU(){
	if(!curseenable)
		return;
	for(int i = 0; i < 100; i++){
		Speed[i] = 0;
		CPUPer[i]=0;
	}
	int cpu = 0;
	const char* match;
	std::string temp;
	std::ifstream file;
	file.open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
	//printf("Getting Max\n");
	while(file.good()){
		file >> Max;
		//printf("Max is %ld\n",Max);
		if(Max != 0){
			Max = Max/1000;
			break;
		}
	}
	file.close();
	file.open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq");
	//printf("Getting Min\n");
	while(file.good()){
		file >> Min;
		//printf("Min is %ld\n",Min);
		if(Min != 0){
			Min = Min/1000;
			break;
		}
	}
	file.close();
	file.open("/proc/cpuinfo");
	while(std::getline(file, temp)){
		const char *temp2 = temp.c_str();
		match = strstr (temp2, "cpu MHz");
		if(match != NULL){
			sscanf (match, "cpu MHz : %ld", &Speed[cpu]);
			cpu++;
		}
		temp = "";
	}
	file.close();
	for(int i = 0; i < 100; i++){
		if(Speed[i]!=0){
			float sp = Speed[i]-Min;
			float mx = Max-Min;
			float per = ((sp/mx)*100);
			CPUPer[i]=per;
		}
	}
}

void Tree::TTime(){
	if(!curseenable)
		return;
	//TreeLog() << "This is a " << "StringStream" << " test!" << std::endl;
	time_t buff = time(0);
	tm *ltm = localtime(&buff);
	TreeTime[2]=ltm->tm_sec;
	TreeTime[1]=ltm->tm_min;
	TreeTime[0]=ltm->tm_hour;
	TTraw = (TreeTime[0]*60*60)+(TreeTime[1]*60)+TreeTime[2];
	if(Told==TreeTime[2]){
		return;
	}
	Told=TreeTime[2];
	if(TreeTime[2]==0){
			if(TreeTime[1]%15==0){
				clearall=true;
			}
			Call1m=CallBuff;
			Audio1m=AudioBuff;
			AudioBuff=0;
			CallBuff=0;
			AudioMin[TreeTime[1]]=Audio1m;
			CallMin[TreeTime[1]]=Call1m;
			CallTot+=Call1m;
			AudioTot+=Audio1m;
			CallTDay+=Call1m;
			AudioTDay+=Audio1m;
			/*werase(LOGwin);
			std::stringstream test;
			test << "\n";
			for (int i = 0; i < 60; i ++){
				test << i << " " << AudioMin[i] << ",";
			}
			test << "\nAudio1m= " << Audio1m;
			std::string teststr=test.str();
			const char * testchar = teststr.c_str();
			wmove(LOGwin, 2,2);
			wprintw(LOGwin, testchar);
			wrefresh(LOGwin);*/
			Audio5m = 0;
			Call5m=0;
			for(int i = 0; i < 5; i ++){
				if((TreeTime[1]-i)>=0){
					Audio5m+=AudioMin[TreeTime[1]-i];
					Call5m+=CallMin[TreeTime[1]-i];
				}
				if((TreeTime[1]-i)<0){
					Audio5m+=AudioMin[TreeTime[1]-i+60];
					Call5m+=CallMin[TreeTime[1]-i+60];
				}
			}
			Audio15m = 0;
			Call15m=0;
			for(int i = 0; i < 15; i ++){
				if((TreeTime[1]-i)>=0){
					Audio15m+=AudioMin[TreeTime[1]-i];
					Call15m+=CallMin[TreeTime[1]-i];
				}
				if((TreeTime[1]-i)<0){
					Audio15m+=AudioMin[TreeTime[1]-i+60];
					Call15m+=CallMin[TreeTime[1]-i+60];
				}
			}
		
			Audio30m=0;
			Call30m=0;
			for(int i = 0; i < 30; i ++){
				if((TreeTime[1]-i)>=0){
					Audio30m+=AudioMin[TreeTime[1]-i];
					Call30m+=CallMin[TreeTime[1]-i];
				}
				if((TreeTime[1]-i)<0){
					Audio30m+=AudioMin[TreeTime[1]-i+60];
					Call30m+=CallMin[TreeTime[1]-i+60];
				}
			}
			Audio1h=0;
			Call1h=0;
			for(int i = 0; i < 60; i ++){
				if((TreeTime[1]-i)>=0){
					Audio1h+=AudioMin[TreeTime[1]-i];
					Call1h+=CallMin[TreeTime[1]-i];
				}
				if((TreeTime[1]-i)<0){
					Audio1h+=AudioMin[TreeTime[1]-i+60];
					Call1h+=CallMin[TreeTime[1]-i+60];
				}
			}
			if(TreeTime[0]==0 && TreeTime[1]==0 && TreeTime[2]==0){
				CallYDay=CallTDay;
				AudioYDay=AudioTDay;
				CallTDay=0;
				AudioTDay=0;
		}

	}
	
}

std::string Tree::TTimeParse(int stime){
	if(!curseenable){
		return "";
	}
	std::stringstream ttp;
	if(stime >= 604800){
		if(stime/60/60/24/7 < 100)
			ttp << " ";
		if(stime/60/60/24/7 < 10)
			ttp << " ";
		ttp << stime/60/60/24/7 << ":";
		ttp << stime/60/60/24%7 << ":";
		if(stime/60/60%24<10){
			ttp << "0" << stime/60/60%24 << ":";
		}
		else{
			ttp << stime/60/60%24 << ":";
		}
		if(stime/60%60<10){
			ttp << "0" << stime/60%60 << ":";
		}
		else{
			ttp << stime/60%60 << ":";
		}
		if(stime%60<10){
			ttp << "0" << stime%60;
		}
		else{
			ttp << stime%60;
		}
	}
	else if(stime >= 86400){
		ttp << "   ";
		if(stime/60/60/24<10){
			ttp << " " << stime/60/60/24 << ":";
		}
		else{
			ttp << stime/60/60/24 << ":";
		}
		if(stime/60/60%24<10){
			ttp << "0" << stime/60/60%24 << ":";
		}
		else{
			ttp << stime/60/60%24 << ":";
		}
		if(stime/60%60<10){
			ttp << "0" << stime/60%60 << ":";
		}
		else{
			ttp << stime/60%60 << ":";
		}
		if(stime%60<10){
			ttp << "0" << stime%60;
		}
		else{
			ttp << stime%60;
		}
	}
	else if(stime >= 3600){
		ttp << "      ";
		if(stime/60/60<10){
			ttp << " " << stime/60/60 << ":";
		}
		else{
			ttp << stime/60/60 << ":";
		}
		if(stime/60%60<10){
			ttp << "0" << stime/60%60 << ":";
		}
		else{
			ttp << stime/60%60 << ":";
		}
		if(stime%60<10){
			ttp << "0" << stime%60;
		}
		else{
			ttp << stime%60;
		}
	}
	else if(stime >=60){
		ttp << "         ";
		if(stime/60<10){
			ttp << " " << stime/60 << ":";
		}
		else{
			ttp << stime/60 << ":";
		}
		if(stime%60<10){
			ttp << "0" << stime%60;
		}
		else{
			ttp << stime%60;
		}
	}
	else if(stime < 60){
		if(stime < 10){
			ttp << "             " << stime;
		}
		else{
			ttp << "            " << stime;
		}
	}
	std::string ttpstr = ttp.str();
	return ttpstr;
}

void Tree::CPUper(){
	if(!curseenable)
		return;
  
  double percent_usage;
	int fcp=0;
	if(!CPUsetup)
  cfp = fopen ("/proc/stat", "r");
  if (cfp == NULL)
  {
    return;
  }
 
 if(!CPUsetup){
	if(!curseenable)
		return;
  while (read_fields (cfp, fields) != -1)
  {
    for (fcp=0, total_tick[cpusp] = 0; fcp<10; fcp++)
    { total_tick[cpusp] += fields[fcp]; }
    idle[cpusp] = fields[3]; /* idle ticks index */
    cpusp++;
  }
 CPUsetup=true;
 }
   fseek (cfp, 0, SEEK_SET);
   fflush (cfp);

    for (countp = 0; countp < cpusp; countp++)
    {
      total_tick_old[countp] = total_tick[countp];
      idle_old[countp] = idle[countp];
     
      if (!read_fields (cfp, fields))
      { return; }
 
      for (fcp=0, total_tick[countp] = 0; fcp<10; fcp++)
      { total_tick[countp] += fields[fcp]; }
      idle[countp] = fields[3];
 
      del_total_tick[countp] = total_tick[countp] - total_tick_old[countp];
      del_idle[countp] = idle[countp] - idle_old[countp];
 
      percent_usage = ((del_total_tick[countp] - del_idle[countp]) / (double) del_total_tick[countp]) * 100;
      if (countp == 0)
      { cputotavg=percent_usage;}
      else
      { cpuavg[countp-1]=percent_usage;}
    }
	int totavg=0;
	for(int i = 0; i < CPUblocks; i ++ ){
		totavg += cpuavg[i];
	}
	totavg /= CPUblocks;
	if(totavg >= 100){
		CPU100++;
		return;
	}
	if(totavg >= 90){
		CPU90++;
		return;
	}
	if(totavg >= 75){
		CPU75++;
		return;
	}
	if(totavg >= 60){
		CPU60++;
		return;
	}
}
int Tree::read_fields (FILE *cfp, unsigned long long int *fields)
{
	if(!curseenable)
		return 0;
  int retval;
  char bufferc[4096];
 
  if (!fgets (bufferc, 4096, cfp))
  { return -1; }
  /* line starts with c and a string. This is to handle cpu, cpu[0-9]+ */
  retval = sscanf (bufferc, "c%*s %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
                            &fields[0], 
                            &fields[1], 
                            &fields[2], 
                            &fields[3], 
                            &fields[4], 
                            &fields[5], 
                            &fields[6], 
                            &fields[7], 
                            &fields[8], 
                            &fields[9]); 
  if (retval == 0)
  { return -1; }
  if (retval < 4) /* Atleast 4 fields is to be read */
  {
    return 0;
  }
  return 1;
}
void Tree::NewLog(std::string input){
		if(!curseenable)
		return;
	//return;
	if(input == "")
		return;
	int originallength = strlen(input.c_str());
	std::string colstr = "";
	int col = 7;
	if(input.at(0)=='@'){
		int colend = input.find('-');
		if(colend!=std::string::npos){
		if(colend<4){
			colstr = input.substr(1,colend);
			input = input.substr(colend+1);
			col = atoi(colstr.c_str());
		}}
	}
	//std::cout << "Stringstream " << nlsize << std::endl;
	std::stringstream inputstream;
	//inputstream << TreeTime[0] << ":" << TreeTime[1] << ":" << TreeTime[2] << " - " << input;
	if(TreeTime[0]<10)
		inputstream << "0";
	inputstream << TreeTime[0] << ":";
	if(TreeTime[1]<10)
		inputstream << "0";
	inputstream << TreeTime[1] << ":";
	if(TreeTime[2]<10)
		inputstream << "0";
	inputstream << TreeTime[2] << " - " << input;
	input = inputstream.str();
	if(strlen(input.c_str())>375){
		input=input.substr(0,375);
	}
	if (logpos < 50){
		//LogMsgsc[logpos][0]='\0';
		memset(LogMsgsc[logpos],'\0',sizeof(LogMsgsc[logpos]));
		//strcpy(LogMsgsc[logpos][0],input.c_str());
		memcpy(LogMsgsc[logpos], input.c_str(), strlen(input.c_str())+1);
		//LogMsgs[logpos] = input;
		//LogMsgsc[logpos]=input.c_str();
		LogCol[logpos] = col;
		logpos++;
	}
	else{
		for(int i = 0; i < 49; i++){
			size_t msgsize = sizeof(LogMsgsc[i+1]);
			
			memset(LogMsgsc[i],'\0',sizeof(LogMsgsc[logpos]));
			memcpy(LogMsgsc[i], LogMsgsc[i+1], msgsize+1);
			//LogMsgsc[i][msgsize-1]='\0';
			//LogMsgsc[i]=LogMsgsc[i+1];
			//memcpy(LogMsgsc[i][0],LogMsgsc[i+1][0],strlen(LogMsgsc[i+1][0]));
			LogCol[i]=LogCol[i+1];
		}
		//LogMsgsc[49][0]='\0';
		memset(LogMsgsc[49],'\0',sizeof(LogMsgsc[logpos]));
		memcpy(LogMsgsc[49], input.c_str(), strlen(input.c_str())+1);
		//LogMsgs[49]=input;
		LogCol[49]=col;
	}
	//std::cout << "Input size " << originallength << std::endl;
	LogRef();
	//std::cout << "newlog done" << std::endl;
}

void Tree::NoSource(long freq, long tg, int sys){
	if(!curseenable)
		return;
	std::stringstream ns;
	ns << "No Source for Freq: " << freq << " Talkgroup: " << tg << " Sys: " << std::hex << std::uppercase << sys << std::nouppercase << std::dec;
	std::string nss = ns.str();
	NewLog(nss);
	SrcErr++;
}

void Tree::NoRecorder(long freq, long tg, int sys, std::string rad){
	if(!curseenable)
		return;
	std::stringstream nr;
	nr << "No Recorder for Freq: " << freq << " Talkgroup: " << tg << " Sys: " << std::hex << std::uppercase << sys << std::nouppercase << std::dec << " Radio: " << rad.substr(4);
	std::string nrs = nr.str();
	NewLog(nrs);
	RecErr++;
}
void Tree::LogRef(){
	//return;
	if(!curseenable)
		return;
	if(!LogWinEn)
		return;
	//LOGwin
	werase(LOGwin);
	werase(LOGwinb);
	wborder(LOGwinb, 0,0,0,0,0,0,0,0);
	wborder(LOGwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattron(LOGwinb, COLOR_PAIR(7));
	wmove(LOGwinb, 0, 1);
	wprintw(LOGwinb, "Logging");
	wattroff(LOGwinb, COLOR_PAIR(7));
	wmove(LOGwin, 0,0);
	
	
	
	
	int logarea = (LOGendx-LOGstartx-2)/(R2h);
	int logwidth = LOGendx-LOGstartx-2;
	int logheight = R2h-2;
	int logline = logheight - 1;
	int logcolor[logheight];
	//std::string logbuff[logheight];
	
	for(int i = 0; i < 100; i++){
		logbuff[i][0]='\0';
	}
	//eturn;
	for(int i = 0; i < 50; i++){
		if(logline<0)
			break;
		memset(logconvert, '\0',sizeof(logconvert));
		size_t msglength = 0;
		if(LogMsgsc[49-i][0]!='\0'){
			msglength=400;
			memcpy(logconvert, LogMsgsc[49-i], msglength);
			//std::cout << "Found Message" << std::endl;
			msglength=strlen(logconvert);
			//msglength=strlen(LogMsgsc[49-1][0]);
		}
		if(msglength!=0)
		//std::cout << " msg size " << msglength << std::endl;
		//size_t msglength = sizeof(LogMsgsc[49-i]);
		if(msglength!=0){
			if(msglength>logwidth){
				//std::cout << "Long Message" << std::endl;
				char * longbuff[6][400];
				for(int x = 0; x<6; x++){
					memset(longbuff[x],'\0',400);
				}
				int extralines=0;
				int offset = 0;
				int lengthbuff = msglength;
				char buffspace = ' ';
				bool msgtrim = false;
				while(lengthbuff>logwidth-2){
					if(extralines>=6){
						lengthbuff=0;
						extralines=5;
						msgtrim=true;
						break;
					}
					if(extralines==0){
						//std::cout << "First Line" << msglength << std::endl;
						memcpy(longbuff[extralines],logconvert,logwidth);
						
						longbuff[extralines][logwidth+1]='\0';
						extralines++;
						lengthbuff-=logwidth;
						//lengthbuff=0;
						offset+=logwidth;
					}
					else{
						//std::cout << "Line " << extralines << " length " << msglength << std::endl;
						memcpy(longbuff[extralines],&logconvert[offset-2],logwidth);
						//strncpy(longbuff[extralines],logconvert+offset-2,logwidth);
//						for(int x = 0; x<1; x++){
//							char testchar[100];
//							memset(testchar, '\0', 100);
//							memcpy(testchar, &logconvert[offset], logwidth-2);
//							printf("%s\n", testchar);
//							memcpy(longbuff[extralines],&logconvert[offset-2],logwidth);
							//longbuff[extralines][2+x]=logconvert[offset+x];
//							std::cout << "Copying char " << x << std::endl;
//							memset(longbuff[extralines][2+x],'a',1);
//						}
//						memset(longbuff[extralines][0],' ',2);
//						longbuff[extralines][logwidth+1]='\0';
						offset+=logwidth-2;
						extralines++;
						lengthbuff-=logwidth-2;
					}
				}
				if(!msgtrim){
					//extralines--;
					memcpy(longbuff[extralines],&logconvert[offset-2],lengthbuff);
					longbuff[extralines][lengthbuff+1]='\0';
				}
//				else
//					NewLog("Trimmed");
//				memcpy(longbuff[extralines]+2,logconvert+offset,lengthbuff);
//				longbuff[extralines][lengthbuff+1]='\0';
//				memset(longbuff[extralines],' ',2);
				for(int x = extralines; x >= 0; x--){
					if(logline<0)
						break;
					memcpy(logbuff[logline], longbuff[extralines], logwidth);
					logbuff[logline][logwidth+1]='\0';
					logcolor[logline]=LogCol[49-i];
					if(extralines!=0){
						memset(logbuff[logline], ' ', 2);
					}
					logline--;
					extralines--;
				}
			}
			else{
				memcpy(logbuff[logline],logconvert,msglength+1);
				logcolor[logline]=LogCol[49-i];
				logline--;
			}
		}
	}
	int offsetpos = 0;
	for(int i = logheight-1; i >=0; i--){
		if(logbuff[logheight-1-i][0]=='\0'){
			offsetpos--;
		}
	}
	for(int i = logheight-1; i >=0; i--){
		wmove(LOGwin, logheight-1-i+offsetpos, 0);
		wattron(LOGwin, COLOR_PAIR(logcolor[logheight-1-i]));
		wprintw(LOGwin, "%s",logbuff[logheight-1-i]);
		wattroff(LOGwin, COLOR_PAIR(logcolor[logheight-1-i]));
	}
	//return;
	wrefresh(LOGwinb);
	wrefresh(LOGwin);
	//std::cout << "logref done" << std::endl;
}

std::string Tree::cparse(long input, int spaces){
	long inputbuff = input;
	int magnitude = 0;
	std::stringstream inputst;
	while(inputbuff/10>0){
		magnitude++;
		inputbuff /= 10;
	}
	spaces = spaces-magnitude;
	for (int i = 0; i < spaces; i++){
		inputst << " ";
	}
	inputst << input;
	return inputst.str();
}
void Tree::Wav(int msg){
	switch (msg){
		case 0:
		NewLog("Error opening WAV file!");
		Wopen++;
		break;
		case 1:
		NewLog("Failed to open WAV!");
		Wfail++;
		break;
		case 2:
		NewLog("Error Rewinding!");
		Wrew++;
		break;
		case 3:
		NewLog("Could not write to WAV file!");
		Wwrite++;
		break;
		case 4:
		NewLog("Error closing WAV file!");
		Wclose++;
		break;
		case 5:
		NewLog("Dropping WAV items!");
		Wdrop++;
		break;
	}
}
void Tree::RecRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!RecWinEn)
		return;
	werase(RECwin);
	wborder(RECwin, 0,0,0,0,0,0,0,0);
	wattron(RECwin, COLOR_PAIR(7));
	wmove(RECwin, R1h-1, 1);
	wprintw(RECwin, "Recorders");
	wattroff(RECwin, COLOR_PAIR(7));
	//RECwin
	for(int x = 0; x < TGblocks; x++){
		bool highlight = false;
		bool flash = false;
	for (int i = 0; i < digrec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(RECwin, 1+i,2+(TGblockx*x));
	}
	else{
	wmove(RECwin, 1+i,1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	if(i < digrec[x]-2){
	wattron(RECwin, COLOR_PAIR(6));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(6));
	}
	else {
	if(digitalgroups[x][i][0]!=0){
		recorderused[x][i][0]=true;
		RecStatus[0][x]=1;
		if(digrec[x]==i)
			RecStatus[1][x]=1;
	wattron(RECwin, COLOR_PAIR(6));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(6));
	}
	else{
		if(recorderused[x][i][0]){
	wattron(RECwin, COLOR_PAIR(7));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(7));
	}
	else{
	wattron(RECwin, COLOR_PAIR(6));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(6));
	}
	}
	}
	wmove(RECwin, 1+i,4+(TGblockx*x));
	currcol = digitalgroups[x][i][2];
		wattron(RECwin, COLOR_PAIR(currcol));
		if(digitalgroups[x][i][0]!=0){
			ss << digitalgroups[x][i][0] << " " << digitalgroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
	}
	for (int i = 0; i < anarec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(RECwin, 1+i+digrec[x],2+(TGblockx*x));
	}
	else{
	wmove(RECwin, 1+i+digrec[x],1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	if(i<anarec[x]-2){
		wattron(RECwin, COLOR_PAIR(1));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(1));
	}
	else{
	if(analoggroups[x][i][0]!=0){
		recorderused[x][i][1]=true;
		RecStatus[0][x]=1;
		if(anarec[x]==i)
			RecStatus[1][x]=1;
	wattron(RECwin, COLOR_PAIR(1));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(1));
	}
	else{
		if(recorderused[x][i][1]){
	wattron(RECwin, COLOR_PAIR(7));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(7));
	}
	else{
	wattron(RECwin, COLOR_PAIR(1));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(1));
	}
	}
	}
	wmove(RECwin, 1+i+digrec[x],4+(TGblockx*x));
	currcol = analoggroups[x][i][2];
		wattron(RECwin, COLOR_PAIR(currcol));
		if(analoggroups[x][i][0]!=0){
			ss << analoggroups[x][i][0] << " " << analoggroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
	}
	for (int i = 0; i < dconvrec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(RECwin, 1+i+digrec[x]+anarec[x],2+(TGblockx*x));
	}
	else{
	wmove(RECwin, 1+i+digrec[x]+anarec[x],1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	wattron(RECwin, COLOR_PAIR(214));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(214));
	wmove(RECwin, 1+i+digrec[x]+anarec[x],4+(TGblockx*x));
	currcol = dconventionalgroups[x][i][2];
		wattron(RECwin, COLOR_PAIR(currcol));
		if(dconventionalgroups[x][i][1]!=0){
			ss << dconventionalgroups[x][i][0] << " " << dconventionalgroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
		else {
			ss << dconventionalgroups[x][i][0];
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
	}
	for (int i = 0; i < aconvrec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(RECwin, 1+i+digrec[x]+anarec[x],2+(TGblockx*x));
	}
	else{
	wmove(RECwin, 1+i+digrec[x]+anarec[x],1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	wattron(RECwin, COLOR_PAIR(5));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(5));
	wmove(RECwin, 1+i+digrec[x]+anarec[x]+dconvrec[x],4+(TGblockx*x));
	currcol = aconventionalgroups[x][i][2];
		wattron(RECwin, COLOR_PAIR(currcol));
		if(aconventionalgroups[x][i][1]!=0){
			ss << aconventionalgroups[x][i][0] << " " << aconventionalgroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
		else {
			ss << aconventionalgroups[x][i][0];
			s = ss.str();
			const char * c = s.c_str();
			wprintw(RECwin, c);
			wattroff(RECwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
	}
	wmove(RECwin, digrec[x]+anarec[x]+aconvrec[x]+dconvrec[x]+2, 2+(x*TGblockx));
	wattron(RECwin, COLOR_PAIR(7));
	if(RecStatus[0][x]==1)
		wattron(RECwin, A_REVERSE);
	if(RecStatus[1][x]==1)
		wattron(RECwin, A_BLINK);
	const char * r = Radios[x].c_str();
	wprintw(RECwin, r);
	if(RecStatus[1][x]==1)
		wattroff(RECwin, A_BLINK);
	if(RecStatus[0][x]==1)
		wattroff(RECwin, A_REVERSE);
	wattroff(RECwin, COLOR_PAIR(7));
	}
	wrefresh(RECwin);
}
void Tree::MsgRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!SysWinEn)
		return;
	werase(SYSwin);
	wborder(SYSwin, 0,0,0,0,0,0,0,0);
	wattron(SYSwin, COLOR_PAIR(7));
	wmove(SYSwin, R1h-1, 1);
	wprintw(SYSwin, "Systems");
	wattroff(SYSwin, COLOR_PAIR(7));
	int datapos = 6;
	wattron(SYSwin, COLOR_PAIR(7));
	for(int i = 0; i < 15; i++){
		std::stringstream ss;
		std::string s;
		wmove(SYSwin, i+1, 2);
		ss << 80-i*5;
		ss >> s;
		const char * c = s.c_str();
		wprintw(SYSwin, c);
	}
	wmove(SYSwin, 16, 3);
	wprintw(SYSwin, "5");
	wmove(SYSwin, 17, 3);
	wprintw(SYSwin, "0");
	wmove(SYSwin, 18, 1);
	wprintw(SYSwin, "MPS");
	wmove(SYSwin, 29, 1);
	if(SYSblocks < 3){
		wprintw(SYSwin, "Msg Rate");
	}
	else{
		wprintw(SYSwin, "Message Rate");
	}
	wattroff(SYSwin, COLOR_PAIR(7));
	wattron(SYSwin, COLOR_PAIR(1));
	wmove(SYSwin, 26, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Max");
	else
		wprintw(SYSwin, "Maximum");
	wattroff(SYSwin, COLOR_PAIR(1));
	wattron(SYSwin, COLOR_PAIR(2));
	wmove(SYSwin, 27, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Avg");
	else
		wprintw(SYSwin, "Average");
	wattroff(SYSwin, COLOR_PAIR(2));
	wattron(SYSwin, COLOR_PAIR(6));
	wmove(SYSwin, 28, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Min");
	else
		wprintw(SYSwin, "Minimum");
	wattroff(SYSwin, COLOR_PAIR(6));
	for(int i = 0; i < 100; i++){
		if(grpccc[0][i]){
		std::stringstream ss;
		std::string s;
		currcol = grpccc[5][i];
		wattron(SYSwin, COLOR_PAIR(currcol));
		wmove(SYSwin, 18, datapos-1);
		ss << std::hex << std::uppercase << grpccc[1][i] << std::dec << std::nouppercase;
		ss >> s;
		if(strlen(s.c_str()) > 3){
			s = s.substr(0,3);
		}
		const char * c = s.c_str();
		wprintw(SYSwin, c);
		wattroff(SYSwin, COLOR_PAIR(currcol));
		int buffint = 0;
		int buffcol = 0;
		for(int x = 0; x < 4; x++){
			switch(x){
				case 0:
					buffint = grpccc[3][i];
					buffcol = 1;
				break;
				case 1:
					buffint = grpccc[4][i];
					buffcol = 2;
				break;
				case 2:
					buffint = grpccc[2][i];
					buffcol = 6;
				break;
				case 3:
					if(spos>0)
						buffint = grpmps[i][spos-1];
					else
						buffint = grpmps[i][59];
					buffcol = 7;
				break;
			}
			wattron(SYSwin, COLOR_PAIR(buffcol));
			wmove(SYSwin, 19+x, datapos-1);
			std::stringstream ss2;
			ss2 << buffint;
			std::string s2 = ss2.str();
			const char * c2 = s2.c_str();
			wprintw(SYSwin, c2);
			wattroff(SYSwin, COLOR_PAIR(buffcol));
		}
			for(int x = 0; x < 17; x++){
				wmove(SYSwin, 17-x, datapos);
				if(maxmsg[i] >= (5*x)&&maxmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(1));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(1));
				}
				wmove(SYSwin, 17-x, datapos);
				if(avgmsg[i] >= (5*x)&& avgmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(2));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(2));
				}
				wmove(SYSwin, 17-x, datapos);
				if(minmsg[i] >= (5*x) && minmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(6));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(6));
				}
			}
		datapos = datapos+4;
		}
	}
	for(int i = 0; i < SYSblocks; i++){
		if(!sysccc[5][i] && !sysccc[8][i]){
		std::stringstream ss;
		std::string s;
		currcol = sysccc[7][i];
		wattron(SYSwin, COLOR_PAIR(currcol));
		wmove(SYSwin, 18, datapos-1);
		ss << std::hex << std::uppercase << sysccc[4][i] << std::dec << std::nouppercase;
		ss >> s;
		if(strlen(s.c_str()) > 3){
			s = s.substr(0,3);
		}
		const char * c = s.c_str();
		wprintw(SYSwin, c);
		wattroff(SYSwin, COLOR_PAIR(currcol));
		int buffint = 0;
		int buffcol = 0;
		for(int x = 0; x < 4; x++){
			switch(x){
				case 0:
					buffint = maxmsg[i];
					buffcol = 1;
				break;
				case 1:
					buffint = avgmsg[i];
					buffcol = 2;
				break;
				case 2:
					buffint = minmsg[i];
					buffcol = 6;
				break;
				case 3:
					if(spos>0)
						buffint = sysmps[i][spos-1];
					else
						buffint = sysmps[i][59];
					buffcol = 7;
				break;
			}
			wattron(SYSwin, COLOR_PAIR(buffcol));
			wmove(SYSwin, 19+x, datapos-1);
			std::stringstream ss2;
			ss2 << buffint;
			std::string s2 = ss2.str();
			const char * c2 = s2.c_str();
			wprintw(SYSwin, c2);
			wattroff(SYSwin, COLOR_PAIR(buffcol));
		}
			for(int x = 0; x < 17; x++){
				wmove(SYSwin, 17-x, datapos);
				if(maxmsg[i] >= (5*x)&&maxmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(1));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(1));
				}
				wmove(SYSwin, 17-x, datapos);
				if(avgmsg[i] >= (5*x)&& avgmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(2));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(2));
				}
				wmove(SYSwin, 17-x, datapos);
				if(minmsg[i] >= (5*x) && minmsg[i]!=0){
					wattron(SYSwin, COLOR_PAIR(6));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ' '|A_REVERSE);
					wattroff(SYSwin, COLOR_PAIR(6));
				}
			}
		datapos = datapos+4;
		}
	}
	wrefresh(SYSwin);
}
void Tree::DatRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!DatWinEn)
		return;
	werase(DATwin);
	wborder(DATwin, 0,0,0,0,0,0,0,0);
	wattron(DATwin, COLOR_PAIR(7));
	wmove(DATwin, R1h-1, 1);
	wprintw(DATwin, "Call Data");
	wmove(DATwin, 1,3);
	wprintw(DATwin, "Call Data");
	for(int i = 0; i < 10; i++){
		std::stringstream dat;
		std::string datstr;
		wmove(DATwin, 2+i, 1);
		switch(i){
			case 0:
			dat << "1m:    " << cparse(Call1m, 13);
			break;
			case 1:
			dat << "5m:    " << cparse(Call5m, 13);
			break;
			case 2:
			dat << "15m:   " << cparse(Call15m, 13);
			break;
			case 3:
			dat << "30m:   " << cparse(Call30m, 13);
			break;
			case 4:
			dat << "1h:    " << cparse(Call1h, 13);
			break;
			case 5:
			dat << "Tday:  " << cparse(CallTDay+CallBuff, 13);
			break;
			case 6:
			dat << "Yday:  " << cparse(CallYDay, 13);
			break;
			case 7:
			dat << "Max:   " << cparse(callmax, 13);
			break;
			case 8:
			dat << "Act:   " << cparse(actcall, 13);
			break;
			case 9:
			dat << "Total: " << cparse(CallTot+CallBuff, 13);
			break;
		}
		datstr=dat.str();
		const char * datchar = datstr.c_str();
		wprintw(DATwin, datchar);
	}
	wmove(DATwin, 13, 3);
	wprintw(DATwin, "Audio Data");
	for(int i = 0; i < 9; i++){
		int holdbuff[60];
		holdbuff[TreeTime[1]] = AudioBuff;
		std::stringstream dat;
		std::string datstr;
		wmove(DATwin, 14+i, 1);
		switch(i){
			case 0:
			dat << "1m:    " << TTimeParse(Audio1m);
			break;
			case 1:
			dat << "5m:    " << TTimeParse(Audio5m);
			break;
			case 2:
			dat << "15m:   " << TTimeParse(Audio15m);
			break;
			case 3:
			dat << "30m:   " << TTimeParse(Audio30m);
			break;
			case 4:
			dat << "1h:    " << TTimeParse(Audio1h);
			break;
			case 5:
			dat << "Tday:  " << TTimeParse(AudioTDay+AudioBuff);
			break;
			case 6:
			dat << "Yday:  " << TTimeParse(AudioYDay);
			break;
			case 7:
			dat << "Max:   " << TTimeParse(audiomax);
			break;
			case 8:
			dat << "Total: " << TTimeParse(AudioTot+AudioBuff);
			break;
		}
		datstr=dat.str();
		const char * datchar = datstr.c_str();
		wprintw(DATwin, datchar);
	}
	wmove(DATwin, R1h-3, 1);
	std::stringstream dat2;
	dat2 << "Time:  " << TTimeParse(TTraw);
	std::string datstr2 = dat2.str();
	const char * datchar2=datstr2.c_str();
	wprintw(DATwin, datchar2);
	wmove(DATwin, R1h-5, 1);
	dat2.str("");
	dat2 << "Up:    " << TTimeParse(runtime);
	datstr2 = dat2.str();
	const char * datchar3 = datstr2.c_str();
	wprintw(DATwin, datchar3);
	wattroff(DATwin, COLOR_PAIR(7));
	wrefresh(DATwin);
}
void Tree::OldRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!OldWinEn)
		return;
	werase(OLDwin);
	wborder(OLDwin, 0,0,0,0,0,0,0,0);
	wattron(OLDwin, COLOR_PAIR(7));
	wmove(OLDwin, R1h-1, 1);
	wprintw(OLDwin, "Past Calls");
	wattroff(OLDwin, COLOR_PAIR(7));
	wattron(OLDwin, COLOR_PAIR(7));
	if(pastpos<28)
	for(int i = 0; i < pastpos; i ++){
		currcol = history[i][2];
		std::stringstream hh;
		std::string hstr;
		std::stringstream h2;
		std::string hstr2;
		hh << history[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		wmove(OLDwin, 1+i, 1);
		wattron(OLDwin, COLOR_PAIR(currcol));
		wprintw(OLDwin, hchar);
		wprintw(OLDwin, " ");
		h2 << history[i][1];
		h2 >> hstr2;
		const char * hchar2 = hstr2.c_str();
		wprintw(OLDwin, hchar2);
		wprintw(OLDwin, "s");
		wattroff(OLDwin, COLOR_PAIR(currcol));
	}
	else{
		for(int i = 0; i < 28; i ++){
		currcol = history[i][2];
		std::stringstream hh;
		std::stringstream h2;
		std::string hstr;
		std::string hstr2;
		hh << history[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		wmove(OLDwin, 1+i, 1);
		wattron(OLDwin, COLOR_PAIR(currcol));
		wprintw(OLDwin, hchar);
		wprintw(OLDwin, " ");
		h2 << history[i][1];
		h2 >> hstr2;
		const char * hchar2 = hstr2.c_str();
		wprintw(OLDwin, hchar2);
		wprintw(OLDwin, "s");
		wattroff(OLDwin, COLOR_PAIR(currcol));
	}
	}
	wattroff(OLDwin, COLOR_PAIR(7));
	wrefresh(OLDwin);
}
void Tree::CpuRef(){
	int cpupos=5;
	if(delayedref!=0){
		LogRef();
	}
	if(!CpuWinEn)
		return;
	werase(CPUwin);
	wborder(CPUwin, 0,0,0,0,0,0,0,0);
	wattron(CPUwin, COLOR_PAIR(7));
	wmove(CPUwin, R1h-1, 1);
	wprintw(CPUwin, "CPU");
	wattroff(CPUwin, COLOR_PAIR(7));
		int datapos = 6;
	wattron(CPUwin, COLOR_PAIR(7));
	wmove(CPUwin, 1, 1);
	wprintw(CPUwin, "100");
	for(int i = 1; i < 19; i++){
		std::stringstream ss;
		std::string s;
		wmove(CPUwin, i+1, 2);
		ss << 100-i*5;
		ss >> s;
		const char * c = s.c_str();
		wprintw(CPUwin, c);
	}
	wmove(CPUwin, 20, 3);
	wprintw(CPUwin, "5");
	wmove(CPUwin, 21, 3);
	wprintw(CPUwin, "0");
	wmove(CPUwin, 22, 1);
	wprintw(CPUwin, "CPU");
	wmove(CPUwin, 29, 1);
	wprintw(CPUwin, "CPU");
	if(ncurses_cpu){
	for(int i = 0; i < CPUblocks; i++){
		if(CPUblocks > 99){
			if(i < 10){
				wmove(CPUwin, 22, 5+i);
				wprintw(CPUwin, "0");
				wmove(CPUwin, 23, 5+i);
				wprintw(CPUwin, "0");
				std::stringstream ss;
				std::string s;
				wmove(CPUwin, 24, 5+i);
				ss << i;
				ss >> s;
				const char * c = s.c_str();
				wprintw(CPUwin, c);
			}
			else if(i < 100){
				wmove(CPUwin, 22, 5+i);
				wprintw(CPUwin, "0");
				wmove(CPUwin, 23, 5+i);
				std::stringstream ss2;
				std::string s2;
				ss2 << i/10%10;
				ss2 >> s2;
				const char * c2 = s2.c_str();
				wprintw(CPUwin, c2);
				std::stringstream ss;
				std::string s;
				wmove(CPUwin, 24, 5+i);
				ss << i%10;
				ss >> s;
				const char * c = s.c_str();
				wprintw(CPUwin, c);
			}
			else{
				wmove(CPUwin, 22, 5+i);
				std::stringstream ss3;
				std::string s3;
				ss3 << i/10/10%10;
				ss3 >> s3;
				const char * c3 = s3.c_str();
				wprintw(CPUwin, c3);
				wmove(CPUwin, 23, 5+i);
				std::stringstream ss2;
				std::string s2;
				ss2 << i/10%10;
				ss2 >> s2;
				const char * c2 = s2.c_str();
				wprintw(CPUwin, c2);
				std::stringstream ss;
				std::string s;
				wmove(CPUwin, 24, 5+i);
				ss << i%10;
				ss >> s;
				const char * c = s.c_str();
				wprintw(CPUwin, c);
			}
		}
		else if(CPUblocks > 9){
			if(i < 10){
				wmove(CPUwin, 22, 5+i);
				wprintw(CPUwin, "0");
				std::stringstream ss;
				std::string s;
				wmove(CPUwin, 23, 5+i);
				ss << i;
				ss >> s;
				const char * c = s.c_str();
				wprintw(CPUwin, c);
			}
			else if(i < 100){
				wmove(CPUwin, 22, 5+i);
				std::stringstream ss2;
				std::string s2;
				ss2 << i/10%10;
				ss2 >> s2;
				const char * c2 = s2.c_str();
				wprintw(CPUwin, c2);
				std::stringstream ss;
				std::string s;
				wmove(CPUwin, 23, 5+i);
				ss << i%10;
				ss >> s;
				const char * c = s.c_str();
				wprintw(CPUwin, c);
			}
		}
		else{
			std::stringstream ss;
			std::string s;
			wmove(CPUwin, 22, 5+i);
			ss << i;
			ss >> s;
			const char * c = s.c_str();
			wprintw(CPUwin, c);
		}
	}
	wattron(CPUwin, COLOR_PAIR(6));
	for(int i = 0; i < CPUblocks; i++){
		if(cpuavg[i] != 0){
			for(int x = 0; x < 21; x++){
				wmove(CPUwin, 21-x, cpupos);
				if(cpuavg[i] >= (5*x)){
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(3));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(1));
					}
					if(cpupos%2==0)
						waddch(CPUwin, ACS_CKBOARD);
					else
						waddch(CPUwin, ' '|A_REVERSE);
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(3));
						wattron(CPUwin, COLOR_PAIR(6));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(1));
						wattron(CPUwin, COLOR_PAIR(6));
					}
				}
			}
		}
		cpupos++;
	}
	}
	else{
		wmove(CPUwin, 22, cpupos);
		wprintw(CPUwin, "A");
		wmove(CPUwin, 23, cpupos);
		wprintw(CPUwin, "V");
		wmove(CPUwin, 24, cpupos);
		wprintw(CPUwin, "G");
		wattron(CPUwin, COLOR_PAIR(6));
		int realavg = 0;
		for(int i = 0; i < CPUblocks; i++){
			realavg+=cpuavg[i];
		}
		realavg = realavg/CPUblocks;
		if(realavg != 0){
			for(int x = 0; x < 21; x++){
				wmove(CPUwin, 21-x, cpupos);
				if(realavg >= (5*x)){
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(3));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(1));
					}
					if(cpupos%2!=0)
						waddch(CPUwin, ACS_CKBOARD);
					else
						waddch(CPUwin, ' '|A_REVERSE);
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(3));
						wattron(CPUwin, COLOR_PAIR(6));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(1));
						wattron(CPUwin, COLOR_PAIR(6));
					}
				}
			}
		}
		cpupos++;
	}
	if(ncurses_lavg){
		wattroff(CPUwin, COLOR_PAIR(6));
		wattron(CPUwin, COLOR_PAIR(3));
		wmove(CPUwin, 23, cpupos);
		wprintw(CPUwin, "151");
		wmove(CPUwin, 24, cpupos+2);
		wprintw(CPUwin, "5");
		wmove(CPUwin, 22, cpupos);
		wprintw(CPUwin, "AVG");
		wattroff(CPUwin, COLOR_PAIR(3));
		wattron(CPUwin, COLOR_PAIR(6));
		getloadavg(load, 3);
	for(int i = 0; i < 3; i++){
		if(load[i] != 0){
			for(int x = 0; x < 21; x++){
				wmove(CPUwin, 21-x, cpupos);
				if((load[i]/CPUblocks)*100 >= (5*x)){
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(3));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(6));
						wattron(CPUwin, COLOR_PAIR(1));
					}
					if(cpupos%2!=0)
						waddch(CPUwin, ACS_CKBOARD);
					else
						waddch(CPUwin, ' '|A_REVERSE);
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(3));
						wattron(CPUwin, COLOR_PAIR(6));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(1));
						wattron(CPUwin, COLOR_PAIR(6));
					}
				}
			}
		}
		cpupos++;
	}
	}
	wattroff(CPUwin, COLOR_PAIR(6));
	wrefresh(CPUwin);
}
void Tree::MtgRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!MrecWinEn)
		return;
	werase(MRECwin);
	wborder(MRECwin, 0,0,0,0,0,0,0,0);
	wattron(MRECwin, COLOR_PAIR(7));
	wmove(MRECwin, R1h-1, 1);
	wprintw(MRECwin, "Missing");
	wattroff(MRECwin, COLOR_PAIR(7));
	if(mtgcount<28)
	for(int i = 0; i < mtgcount; i ++){
		std::stringstream hh;
		std::string hstr;
		hh << MTGs[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		currcol = MTGs[i][1];
		wattron(MRECwin, COLOR_PAIR(currcol));
		wmove(MRECwin, 1+i, 1);
		wprintw(MRECwin, hchar);
		wattroff(MRECwin, COLOR_PAIR(currcol));
	}
	else{
		for(int i = 0; i < 28; i ++){
		std::stringstream hh;
		std::stringstream h2;
		std::string hstr;
		std::string hstr2;
		hh << MTGs[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		currcol = MTGs[i][1];
		wattron(MRECwin, COLOR_PAIR(currcol));
		wmove(MRECwin, 1+i, 1);
		wprintw(MRECwin, hchar);
		wattroff(MRECwin, COLOR_PAIR(currcol));
	}
	}
	wrefresh(MRECwin);
}
void Tree::ErrRef(){
	if(delayedref!=0){
		LogRef();
	}
	if(!ErrWinEn)
		return;
	werase(ERRwin);
	wborder(ERRwin, 0,0,0,0,0,0,0,0);
	wattron(ERRwin, COLOR_PAIR(7));
	wmove(ERRwin, R1h-1, 1);
	wprintw(ERRwin, "Errors");
	wattroff(ERRwin, COLOR_PAIR(7));
	wattron(ERRwin, COLOR_PAIR(7));
	for(int i = 0; i < 12; i++){
		std::stringstream dat;
		std::string datstr;
		wmove(ERRwin, 1+i, 1);
		switch(i){
			case 0:
			dat << "Killed:   " << cparse(KillErr, 4);
			break;
			case 1:
			dat << "Double:   " << cparse(DoubleErr, 4);
			break;
			case 2:
			dat << "Recorder: " << cparse(RecErr, 4);
			break;
			case 3:
			dat << "Source:   " << cparse(SrcErr, 4);
			break;
			case 4:
			dat << "TG:       " << cparse(TGErr, 4);
			break;
			case 5:
			dat << "Long:     " << cparse(LgErr, 4);
			break;
			case 6:
			dat << "CPU60:    " << cparse(CPU60, 4);
			break;
			case 7:
			dat << "CPU75:    " << cparse(CPU75, 4);
			break;
			case 8:
			dat << "CPU90:    " << cparse(CPU90, 4);
			break;
			case 9:
			dat << "CPU100:   " << cparse(CPU100, 4);
			break;
			case 10:
			dat << "Wclose:   " << cparse(Wclose, 4);
			break;
			case 11:
			dat << "Wdrop:    " << cparse(Wdrop, 4);
			break;
		}
		datstr=dat.str();
		const char * datchar = datstr.c_str();
		wprintw(ERRwin, datchar);
	}
	wattroff(ERRwin, COLOR_PAIR(7));
	wrefresh(ERRwin);
}
void Tree::UtRef(){
	if(!curseenable)
		return;
	if(!UtWinEn)
		return;
	//LOGwin
	werase(UTwin);
	werase(UTwinb);
	wborder(UTwinb, 0,0,0,0,0,0,0,0);
	wborder(UTwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattron(UTwinb, COLOR_PAIR(7));
	wmove(UTwinb, 0, 1);
	wprintw(UTwinb, "Call History");
	wattroff(UTwinb, COLOR_PAIR(7));
	//wattron(UTwin, COLOR_PAIR(7));
	wmove(UTwin, 0,0);
	for(int i = 0; i < 20 && i<utpos; i++){
		if(UThistory[i]=="")
			break;
		std::stringstream ls;
		std::string UTbuff = UThistory[i];
		int UTmax = UTendx-UTstartx-3;
		if(strlen(UTbuff.c_str())>UTmax)
			UTbuff=UTbuff.substr(0,UTmax);
		if (i < 19)
			ls << UTbuff << "\n";
		else
			ls << UTbuff;
		std::string lss = ls.str();
		const char * lchar = lss.c_str();
		wattron(UTwin, COLOR_PAIR(UTcol[i]));
		wprintw(UTwin, lchar);
		wattroff(UTwin, COLOR_PAIR(UTcol[i]));
		
	}
	//wattroff(LOGwin, COLOR_PAIR(7));
	wrefresh(UTwinb);
	wrefresh(UTwin);
}

void Tree::UTnew(int tg, long nac, std::string radio, int length, int color, std::string description){
		std::string short_desc = description;
		std::string buffstr;
		std::stringstream buffstream;
		/*
		if(strlen(short_desc.c_str())>UTdesclen){
			short_desc=short_desc.substr(0,UTdesclen);
		}
		*/
		std::stringstream UTstream;
		if(TreeTime[0]<10)
			UTstream << "0";
		UTstream << TreeTime[0] << ":";
		if(TreeTime[1]<10)
			UTstream << "0";
		UTstream << TreeTime[1] << ":";
		if(TreeTime[2]<10)
			UTstream << "0";
		UTstream << TreeTime[2];
		UTstream << " - ";
		buffstream << tg;
		buffstr = buffstream.str();
		if(strlen(buffstr.c_str())<7)
			UTstream << " ";
		if(strlen(buffstr.c_str())<6)
			UTstream << " ";
		if(strlen(buffstr.c_str())<5)
			UTstream << " ";
		if(strlen(buffstr.c_str())<4)
			UTstream << " ";
		if(strlen(buffstr.c_str())<3)
			UTstream << " ";
		if(strlen(buffstr.c_str())<2)
			UTstream << " ";
		UTstream << tg << " - ";
		buffstream.str("");
		buffstr = "";
		buffstream << std::hex << std::uppercase << nac << std::dec << std::nouppercase;
		buffstr = buffstream.str();
		if(strlen(buffstr.c_str())<5)
			UTstream << " ";
		if(strlen(buffstr.c_str())<4)
			UTstream << " ";
		UTstream << buffstr << " - ";
		if(strlen(radio.c_str())<5)
			UTstream << " ";
		if(strlen(radio.c_str())<4)
			UTstream << " ";
		if(strlen(radio.c_str())<3)
			UTstream << " ";
		if(strlen(radio.c_str())<2)
			UTstream << " ";
		UTstream << radio << " - ";
		buffstream.str("");
		buffstream << length;
		buffstr = "";
		buffstr = buffstream.str();
		if(strlen(buffstr.c_str())<3)
			UTstream << " ";
		if(strlen(buffstr.c_str())<2)
			UTstream << " ";
		UTstream << buffstr << " - " << short_desc;
		//NewLog(UTstream.str());
		if (utpos < 20){
		UThistory[utpos] = UTstream.str();
		//TreeLog() << "@" << color << "-" << UTstream.str() << std::endl;
		UTcol[utpos]=color;
		utpos++;
		}
		else{
			//TreeLog() << "@" << color << "-" << UTstream.str() << std::endl;
			for(int i = 0; i < 19; i++){
				UThistory[i]=UThistory[i+1];
				UTcol[i]=UTcol[i+1];
			}
			UThistory[19]=UTstream.str();
			UTcol[19]=color;
		}
}
