#include "tree.h"
#include <sys/sysinfo.h>
#include <string>
#include <ncurses.h>
#include <sstream>
#include <iostream>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <ctime>

#define MAX_CPU 128
bool verbose = false;
bool coloren = false;
long analoggroups[10][100][3];
long digitalgroups[10][100][3];
long colorgroups[100];
int colorsystems[100];
int currcol = 4;
long csysid[10];
int sys_id = 0;
double load[3];
WINDOW *create_newwin(int height, int width, int starty, int startx);
WINDOW *TGwin;
WINDOW *SYSwin;
WINDOW *OLDwin;
WINDOW *CPUwin;
WINDOW *DATwin;
WINDOW *LOGwinb;
WINDOW *LOGwin;
WINDOW *MTGwin;
WINDOW *ERRwin;
void destroy_win(WINDOW *local_win);
int systemnumber = 0;
int currsys;
int minmsg[10];
int maxmsg[10];
int avgmsg[10];
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
int cpus = 0;
int cpuw = defw;
std::ifstream file;
int curseenable = 0;
std::string Radios[10];
std::string LogMsgs[20];
int digrec[10];
int logpos = 0;
int anarec[10];
long actTG[10][100];
int radiocount = 0;
int spos = 0;
bool looped = false;
int sysmps[10][60];
int sysccc[2][10];
int pastpos = 0;
int history[28][3];
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
int callmax=0;
int audiomax=0;
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



//Start of Window Coordinates
int R1y = 1;
int R2y = 35;
int R1h = 30;
int R2h = 20;
int TGstartx = 1;
int TGblockx = 12;
int TGdefx = 2;
int TGendx;
int SYSstartx;
int SYSdefx = 5;
int SYSblockx = 4;
int SYSendx;
int OLDstartx;
int OLDdefx = 15;
int OLDblockx = 0;
int OLDendx;
int CPUstartx;
int CPUdefx = 6;
int CPUblockx = 1;
int CPUendx;
int DATstartx;
int DATdefx = 20;
int DATblockx = 0;
int DATendx = 0;
int LOGstartx = 1;
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
int MTGendx = 0;
int MTGdefx = 10;
int MTGblockx = 0;
int MTGblocks = 0;
int ERRstartx;
int ERRdefx = 17;
int ERRendx;
int ERRblockx = 0;
int ERRblocks = 0;
long long int CPUusage[100][10];
long long int oldCPUusage[100][10];
double cpuavg[100];
bool CPUsetup=false;
	FILE *cfp;

bool clearall=true;

void Tree::PurgeArrays(){
	for(int i = 0; i < 10; i ++){
		Radios[i]="";
		digrec[i]=0;
		anarec[10]=0;
		sysccc[0][i]=0;
		sysccc[1][i]=0;
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
		ScrRef();
		AudioBuff = AudioBuff+elapsed;
		CallBuff++;
		pastpos++;
		ScrRef();
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
		ScrRef();
	}
}
void Tree::EndCall(long tg, int elapsed, std::string dev){
	if(!curseenable)
		return;
	for(int i = 0; i < TGblocks; i++){
		if(Radios[i]==dev){
		for(int x = 0; x < 100; x++){
			if(analoggroups[i][x][0]==tg){
				//Put logging code here
				Past(tg, analoggroups[i][x][1], analoggroups[i][x][2]);
				analoggroups[i][x][0]=0;
				analoggroups[i][x][1]=0;
				actcall--;
				ScrRef();
				return;
			}
			if(digitalgroups[i][x][0]==tg){
				Past(tg, digitalgroups[i][x][1], digitalgroups[i][x][2]);
				//put logging code here
				digitalgroups[i][x][0]=0;
				digitalgroups[i][x][1]=0;
				actcall--;
				ScrRef();
				return;
			}
		}
	}
	}
}
void Tree::SetCurses(int enable){
	curseenable = enable;
}

void Tree::Rate(int mps){
	/*
	what to do for slow message rate
	*/
}
void Tree::SourceDev(std::string dev, int dig, int ana){
	Radios[TGblocks]=dev.substr(4);
	digrec[TGblocks]=dig;
	anarec[TGblocks]=ana;
	TGblocks++;
}
void Tree::StartCall(long tg, long freq, std::string dev, bool isanalog, int nac){
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
	int syscolor = -1;
		for(int i = 0; i < 10; i++){
		if(sysccc[0][i]==sys_id){
			syscolor = i;
			break;
		}
	}
	if(isanalog){
		for(int i = 0; i < TGblocks; i++){
			if(dev.substr(4) == Radios[i]){
				for(int x = 0; x < anarec[i]; x++){
					if(analoggroups[i][x][0]==0){
						analoggroups[i][x][0]=tg;
						analoggroups[i][x][2]=syscolor;
						actcall++;
						if(actcall > callmax)
							callmax=actcall;
						ScrRef();
						return;
					}
				}
			}
		}
	}
	else{
		for(int i = 0; i < TGblocks; i++){
			if(dev.substr(4) == Radios[i]){
				for(int x = 0; x < digrec[i]; x++){
					if(digitalgroups[i][x][0]==0){
						digitalgroups[i][x][2]=syscolor;
						digitalgroups[i][x][0]=tg;
						actcall++;
						if(actcall>callmax)
							callmax=actcall;
						ScrRef();
						return;
					}
				}
			}
		}
	}
}
void Tree::MissingTG(long tg, int nac){
	if(!curseenable)
		return;
	TGErr++;
	int syscolor = -1;
		for(int i = 0; i < 10; i++){
		if(sysccc[0][i]==nac){
			syscolor = i;
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
		ScrRef();
		mtgcount++;
	}
	else{
		for(int i = 0; i < 27; i++){
			MTGs[i][0]=MTGs[i+1][0];
			MTGs[i][1]=MTGs[i+1][1];
		}
		MTGs[27][0]=tg;
		MTGs[27][1]=syscolor;
		ScrRef();
	}
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
void Tree::SysId(int sysid){
	/*
	Copy our sysid tracking stuff here
	*/
			sysccc[0][SYSblocks]=sysid;
			//sysmps[0][SYSblocks]=sysid;
			sysccc[1][SYSblocks]++;
	SYSblocks++;
}
void Tree::ccId(int sys){
	sys_id = sys;
	if (sys_id==0)
		sys_id=0xf18;
	if (sys_id==0x2d3)
		sys_id=0x2d0;
	if(!curseenable)
		return;
	//Got ID in message
	for(int i = 0; i < 10; i++){
		if(sysccc[0][i]==sys_id){
			sysccc[1][i] = sysccc[1][i]+1;
			break;
		}
	}

}
bool Tree::StartCurses(){
	if(!curseenable)
		return false;
	//Var prep here!
	//PurgeArrays();
	for(int i = 0; i < 20; i++){
		LogMsgs[i]="";
	}
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
	TGendx = TGstartx+TGdefx+(TGblockx*TGblocks); //now we have the end of our TG window!
	SYSstartx = TGendx+1;
	SYSendx = SYSstartx+SYSdefx+(SYSblockx*SYSblocks);
	OLDstartx = SYSendx+1;
	OLDendx = OLDstartx+OLDdefx+(OLDblockx*OLDblocks);
	CPUstartx = OLDendx + 1;
	CPUendx = CPUstartx+CPUdefx+(CPUblockx*CPUblocks);
	DATstartx = CPUendx + 1;
	DATendx = DATstartx+DATdefx+(DATblockx*DATblocks);
	LOGendx = LOGstartx+LOGdefx+(LOGblockx*LOGblocks);
	MTGstartx = DATendx+1;
	MTGendx = MTGstartx+MTGdefx+(MTGblockx*MTGblocks);
	ERRstartx = MTGendx+1;
	ERRendx = ERRstartx+ERRdefx+(ERRblockx*ERRblocks);
	for(int i = 0; i < 10; i++){
		csysid[i] = 0;
		maxmsg[i] = 0;
		minmsg[i] = 0;
		avgmsg[i] = 0;
	}
//newterm(NULL, stdout, stdin);
	initscr();			/* Start curses mode 		*/
	curs_set(0);
	//if (has_colors){
		coloren = true;
		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_CYAN, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(6, COLOR_BLUE, COLOR_BLACK);
		init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	//}
	//cbreak();			/* Line buffering disabled, Pass on
					// * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	TGwin = create_newwin(R1h, TGendx-TGstartx, R1y, TGstartx);
	SYSwin = create_newwin(R1h, SYSendx-SYSstartx, R1y, SYSstartx);
	OLDwin = create_newwin(R1h, OLDendx-OLDstartx, R1y, OLDstartx);
	CPUwin = create_newwin(R1h, CPUendx-CPUstartx, R1y, CPUstartx);
	DATwin = create_newwin(R1h, DATendx-DATstartx, R1y, DATstartx);
	MTGwin = create_newwin(R1h, MTGendx-MTGstartx, R1y, MTGstartx);
	ERRwin = create_newwin(R1h, ERRendx-ERRstartx, R1y, ERRstartx);
	LOGwinb = create_newwin(R2h, LOGendx-LOGstartx, R2y, LOGstartx);//This is the border for the log window
	LOGwin = create_newwin(R2h-2, LOGendx-LOGstartx-2, R2y+1, LOGstartx+1); //This is the actual logging window
	wbkgd(TGwin, COLOR_PAIR(6));
	wbkgd(SYSwin, COLOR_PAIR(6));
	wbkgd(OLDwin, COLOR_PAIR(6));
	wbkgd(CPUwin, COLOR_PAIR(6));
	wbkgd(DATwin, COLOR_PAIR(6));
	wbkgd(LOGwinb, COLOR_PAIR(6));
	wbkgd(MTGwin, COLOR_PAIR(6));
	wbkgd(ERRwin, COLOR_PAIR(6));
	wborder(TGwin, 0,0,0,0,0,0,0,0);
	wborder(SYSwin, 0,0,0,0,0,0,0,0);
	wborder(OLDwin, 0,0,0,0,0,0,0,0);
	wborder(CPUwin, 0,0,0,0,0,0,0,0);
	wborder(DATwin, 0,0,0,0,0,0,0,0);
	wborder(LOGwinb, 0,0,0,0,0,0,0,0);
	wborder(MTGwin, 0,0,0,0,0,0,0,0);
	wborder(ERRwin, 0,0,0,0,0,0,0,0);
	wborder(LOGwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattron(TGwin, COLOR_PAIR(4));
	wmove(TGwin, R1h-1, 1);
	wprintw(TGwin, "Recorders");
	wattroff(TGwin, COLOR_PAIR(4));
	wattron(SYSwin, COLOR_PAIR(4));
	wmove(SYSwin, R1h-1, 1);
	wprintw(SYSwin, "Systems");
	wattroff(SYSwin, COLOR_PAIR(4));
	wattron(OLDwin, COLOR_PAIR(4));
	wmove(OLDwin, R1h-1, 1);
	wprintw(OLDwin, "Past Calls");
	wattroff(OLDwin, COLOR_PAIR(4));
	wattron(CPUwin, COLOR_PAIR(4));
	wmove(CPUwin, R1h-1, 1);
	wprintw(CPUwin, "CPU");
	wattroff(CPUwin, COLOR_PAIR(4));
	wattron(DATwin, COLOR_PAIR(4));
	wmove(DATwin, R1h-1, 1);
	wprintw(DATwin, "Call Data");
	wattroff(DATwin, COLOR_PAIR(4));
	wattron(MTGwin, COLOR_PAIR(4));
	wmove(MTGwin, R1h-1, 1);
	wprintw(MTGwin, "Missing");
	wattroff(MTGwin, COLOR_PAIR(4));
	wattron(ERRwin, COLOR_PAIR(4));
	wmove(ERRwin, R1h-1, 1);
	wprintw(ERRwin, "Errors");
	wattroff(ERRwin, COLOR_PAIR(4));
	wattron(LOGwinb, COLOR_PAIR(4));
	wmove(LOGwinb, 0, 1);
	wprintw(LOGwinb, "Logging");
	wattroff(LOGwinb, COLOR_PAIR(4));
	scrollok(LOGwin, true);
	wrefresh(TGwin);
	wrefresh(SYSwin);
	wrefresh(OLDwin);
	wrefresh(CPUwin);
	wrefresh(DATwin);
	wrefresh(LOGwin);
	wrefresh(LOGwinb);
	wrefresh(MTGwin);
	wrefresh(ERRwin);
	return 1;
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
void Tree::ScrRef(){
	if(!curseenable)
		return;
	//CPU();
	TTime();
	if(clearall){
	//clear();
	werase(TGwin);
	werase(SYSwin);
	werase(OLDwin);
	werase(CPUwin);
	werase(DATwin);
	werase(MTGwin);
	werase(ERRwin);
	//wclear(TGwin);
	//wclear(SYSwin);
	//wclear(OLDwin);
	//wclear(CPUwin);
	//wclear(DATwin);
	//wclear(LOGwinb);
	//wclear(LOGwin);
	//wclear(MTGwin);
	clearall=false;
	LogRef();
	//wrefresh(DATwin);
	}
	else{
	werase(ERRwin);
	werase(TGwin);
	werase(SYSwin);
	werase(OLDwin);
	werase(CPUwin);
	werase(DATwin);
	werase(MTGwin);
	}
	wborder(TGwin, 0,0,0,0,0,0,0,0);
	wborder(SYSwin, 0,0,0,0,0,0,0,0);
	wborder(OLDwin, 0,0,0,0,0,0,0,0);
	wborder(CPUwin, 0,0,0,0,0,0,0,0);
	wborder(DATwin, 0,0,0,0,0,0,0,0);
	wborder(MTGwin, 0,0,0,0,0,0,0,0);
	wborder(ERRwin, 0,0,0,0,0,0,0,0);
	wattron(TGwin, COLOR_PAIR(4));
	wmove(TGwin, R1h-1, 1);
	wprintw(TGwin, "Recorders");
	wattroff(TGwin, COLOR_PAIR(4));
	wattron(SYSwin, COLOR_PAIR(4));
	wmove(SYSwin, R1h-1, 1);
	wprintw(SYSwin, "Systems");
	wattroff(SYSwin, COLOR_PAIR(4));
	wattron(OLDwin, COLOR_PAIR(4));
	wmove(OLDwin, R1h-1, 1);
	wprintw(OLDwin, "Past Calls");
	wattroff(OLDwin, COLOR_PAIR(4));
	wattron(CPUwin, COLOR_PAIR(4));
	wmove(CPUwin, R1h-1, 1);
	wprintw(CPUwin, "CPU");
	wattroff(CPUwin, COLOR_PAIR(4));
	wattron(DATwin, COLOR_PAIR(4));
	wmove(DATwin, R1h-1, 1);
	wprintw(DATwin, "Call Data");
	wattroff(DATwin, COLOR_PAIR(4));
	wattron(MTGwin, COLOR_PAIR(4));
	wmove(MTGwin, R1h-1, 1);
	wprintw(MTGwin, "Missing");
	wattroff(MTGwin, COLOR_PAIR(4));
	wattron(ERRwin, COLOR_PAIR(4));
	wmove(ERRwin, R1h-1, 1);
	wprintw(ERRwin, "Errors");
	wattroff(ERRwin, COLOR_PAIR(4));
	
	//TGwin
	for(int x = 0; x < TGblocks; x++){
	for (int i = 0; i < digrec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(TGwin, 1+i,2+(TGblockx*x));
	}
	else{
	wmove(TGwin, 1+i,1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	wattron(TGwin, COLOR_PAIR(3));
	wprintw(TGwin, d);
	wattroff(TGwin, COLOR_PAIR(3));
	wmove(TGwin, 1+i,4+(TGblockx*x));
	if(getcol(digitalgroups[x][i][2])){
		wattron(TGwin, COLOR_PAIR(currcol));
		if(digitalgroups[x][i][0]!=0){
			ss << digitalgroups[x][i][0] << " " << digitalgroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(TGwin, c);
			wattroff(TGwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
		}
	}
	for (int i = 0; i < anarec[x]; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(TGwin, 1+i+digrec[x],2+(TGblockx*x));
	}
	else{
	wmove(TGwin, 1+i+digrec[x],1+(TGblockx*x));
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	wattron(TGwin, COLOR_PAIR(2));
	wprintw(TGwin, d);
	wattroff(TGwin, COLOR_PAIR(2));
	wmove(TGwin, 1+i+digrec[x],4+(TGblockx*x));
	if(getcol(analoggroups[x][i][2])){
		wattron(TGwin, COLOR_PAIR(currcol));
		if(analoggroups[x][i][0]!=0){
			ss << analoggroups[x][i][0] << " " << analoggroups[x][i][1] << "s";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(TGwin, c);
			wattroff(TGwin, COLOR_PAIR(currcol));
		}
	//		ss.str("");
		//	s = "";
		}
	}
	wmove(TGwin, digrec[x]+anarec[x]+2, 2+(x*TGblockx));
	wattron(TGwin, COLOR_PAIR(4));
	const char * r = Radios[x].c_str();
	wprintw(TGwin, r);
	wattroff(TGwin, COLOR_PAIR(4));
	}
	
	//System and CPU windows
	int datapos = 6;
	wattron(SYSwin, COLOR_PAIR(4));
	wattron(CPUwin, COLOR_PAIR(4));
	wmove(SYSwin, 1, 1);
	wmove(CPUwin, 1, 1);
	wprintw(SYSwin, "100");
	wprintw(CPUwin, "100");
	for(int i = 1; i < 19; i++){
		std::stringstream ss;
		std::string s;
		wmove(SYSwin, i+1, 2);
		wmove(CPUwin, i+1, 2);
		ss << 100-i*5;
		ss >> s;
		const char * c = s.c_str();
		wprintw(SYSwin, c);
		wprintw(CPUwin, c);
	}
	wmove(CPUwin, 20, 3);
	wmove(SYSwin, 20, 3);
	wprintw(SYSwin, "5");
	wprintw(CPUwin, "5");
	wmove(SYSwin, 21, 3);
	wmove(CPUwin, 21, 3);
	wprintw(CPUwin, "0");
	wprintw(SYSwin, "0");
	wmove(SYSwin, 22, 1);
	wmove(CPUwin, 22, 1);
	wprintw(CPUwin, "CPU");
	wprintw(SYSwin, "MPS");
	wmove(CPUwin, 29, 1);
	wprintw(CPUwin, "CPU");
	for(int i = 0; i < CPUblocks; i++){
		std::stringstream ss;
		std::string s;
		wmove(CPUwin, 22, 5+i);
		ss << i;
		ss >> s;
		const char * c = s.c_str();
		wprintw(CPUwin, c);
	}
	/*
	wmove(CPUwin, 22, 6+CPUblocks);
	wprintw(CPUwin, "151");
	wmove(CPUwin, 23, 6+CPUblocks);
	wprintw(CPUwin, "  5");
	wmove(CPUwin, 24, 6+CPUblocks);
	wprintw(CPUwin, "AVG");
	wattroff(CPUwin, COLOR_PAIR(4));
	*/
	wmove(SYSwin, 29, 1);
	wprintw(SYSwin, "Message Rate");
	wattroff(SYSwin, COLOR_PAIR(4));
	wattron(SYSwin, COLOR_PAIR(2));
	wmove(SYSwin, 24, 1);
	wprintw(SYSwin, "Maximum");
	wattroff(SYSwin, COLOR_PAIR(2));
	wattron(SYSwin, COLOR_PAIR(1));
	wmove(SYSwin, 25, 1);
	wprintw(SYSwin, "Average");
	wattroff(SYSwin, COLOR_PAIR(1));
	wattron(SYSwin, COLOR_PAIR(3));
	wmove(SYSwin, 26, 1);
	wprintw(SYSwin, "Minimum");
	wattroff(SYSwin, COLOR_PAIR(3));
	for(int i = 0; i < SYSblocks; i++){
		
		std::stringstream ss;
		std::string s;
		getcol(i);
		wattron(SYSwin, COLOR_PAIR(currcol));
		wmove(SYSwin, 22, datapos-1);
		ss << std::hex << std::uppercase << sysccc[0][i] << std::dec << std::nouppercase;
		ss >> s;
		const char * c = s.c_str();
		wprintw(SYSwin, c);
		wattroff(SYSwin, COLOR_PAIR(currcol));
		wattron(SYSwin, COLOR_PAIR(4));
		wmove(SYSwin, 23, datapos-1);
		std::stringstream ss2;
		ss2 << sysmpsbuff[i];
		//ss2 << "ABC";
		std::string s2 = ss2.str();
		const char * c2 = s2.c_str();
		wprintw(SYSwin, c2);
		wattroff(SYSwin, COLOR_PAIR(4));
			for(int x = 0; x < 21; x++){
				wmove(SYSwin, 21-x, datapos);
				if(maxmsg[i] >= (5*x)){
					wattron(SYSwin, COLOR_PAIR(2));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ACS_CKBOARD);
					wattroff(SYSwin, COLOR_PAIR(2));
				}
			}
			for(int x = 0; x < 21; x++){
				wmove(SYSwin, 21-x, datapos);
				if(avgmsg[i] >= (5*x)){
					wattron(SYSwin, COLOR_PAIR(1));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ACS_CKBOARD);
					wattroff(SYSwin, COLOR_PAIR(1));
				}
			}
			for(int x = 0; x < 21; x++){
				wmove(SYSwin, 21-x, datapos);
				if(minmsg[i] >= (5*x)){
					wattron(SYSwin, COLOR_PAIR(3));
					//wprintw(SYSwin, "X");
					waddch(SYSwin, ACS_CKBOARD);
					wattroff(SYSwin, COLOR_PAIR(3));
				}
			}
		datapos = datapos+4;
	}
	wattron(CPUwin, COLOR_PAIR(3));
	for(int i = 0; i < CPUblocks; i++){
		int cpupos = 5;
		if(cpuavg[i] != 0){
			for(int x = 0; x < 21; x++){
				wmove(CPUwin, 21-x, cpupos+i);
				if(cpuavg[i] >= (5*x)){
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(3));
						wattron(CPUwin, COLOR_PAIR(7));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(3));
						wattron(CPUwin, COLOR_PAIR(2));
					}
					if(i%2==0)
						waddch(CPUwin, ACS_CKBOARD);
					else
						waddch(CPUwin, ' '|A_REVERSE);
					if((5*x)>=50 && (5*x)<75){
						wattroff(CPUwin, COLOR_PAIR(7));
						wattron(CPUwin, COLOR_PAIR(3));
					}
					else if((5*x)>=75){
						wattroff(CPUwin, COLOR_PAIR(2));
						wattron(CPUwin, COLOR_PAIR(3));
					}
				}
			}
		}
	}
	/*
	for(int i = 0; i < 3; i++){
		getloadavg(load, 3);
		for(int x = 0; x < 21; x++){
			wmove(CPUwin, 21-x, 6+CPUblocks+i);
			if(((load[i]/8)*100) >= (5*x)){
				if(i%2!=0)
					waddch(CPUwin, ACS_CKBOARD);
				else
					waddch(CPUwin, ' '|A_REVERSE);
			}
		}
	}*/
	wattroff(CPUwin, COLOR_PAIR(3));
	
	//History window
	wattron(OLDwin, COLOR_PAIR(4));
	if(pastpos<28)
	for(int i = 0; i < pastpos; i ++){
		getcol(history[i][2]);
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
		getcol(history[i][2]);
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
	wattroff(OLDwin, COLOR_PAIR(4));
	
	//Data Window
	wattron(DATwin, COLOR_PAIR(4));
	wmove(DATwin, 1,3);
	wprintw(DATwin, "Call Data");
	for(int i = 0; i < 10; i++){
		std::stringstream dat;
		std::string datstr;
		wmove(DATwin, 2+i, 1);
		switch(i){
			case 0:
			dat << "1m:    " << cparse(Call1m, 10);
			break;
			case 1:
			dat << "5m:    " << cparse(Call5m, 10);
			break;
			case 2:
			dat << "15m:   " << cparse(Call15m, 10);
			break;
			case 3:
			dat << "30m:   " << cparse(Call30m, 10);
			break;
			case 4:
			dat << "1h:    " << cparse(Call1h, 10);
			break;
			case 5:
			dat << "Tday:  " << cparse(CallTDay+CallBuff, 10);
			break;
			case 6:
			dat << "Yday:  " << cparse(CallYDay, 10);
			break;
			case 7:
			dat << "Max:   " << cparse(callmax, 10);
			break;
			case 8:
			dat << "Act:   " << cparse(actcall, 10);
			break;
			case 9:
			dat << "Total: " << cparse(CallTot+CallBuff, 10);
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
	wattroff(DATwin, COLOR_PAIR(4));
	
	//MTGwin
	if(mtgcount<28)
	for(int i = 0; i < mtgcount; i ++){
		std::stringstream hh;
		std::string hstr;
		hh << MTGs[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		getcol(MTGs[i][1]);
		wattron(MTGwin, COLOR_PAIR(currcol));
		wmove(MTGwin, 1+i, 1);
		wprintw(MTGwin, hchar);
		wattroff(MTGwin, COLOR_PAIR(currcol));
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
		getcol(MTGs[i][1]);
		wattron(MTGwin, COLOR_PAIR(currcol));
		wmove(MTGwin, 1+i, 1);
		wprintw(MTGwin, hchar);
		wattroff(MTGwin, COLOR_PAIR(currcol));
	}
	}
	
	//Error Window
	wattron(ERRwin, COLOR_PAIR(4));
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
	wattroff(ERRwin, COLOR_PAIR(4));
	
	wrefresh(TGwin);
	wrefresh(SYSwin);
	wrefresh(OLDwin);
	wrefresh(CPUwin);
	wrefresh(DATwin);
	wrefresh(MTGwin);
	wrefresh(ERRwin);
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

bool Tree::getcol(int loc){
	if(!curseenable)
		return true;
	switch (loc){
		case -1:
		currcol=4;
		break;
		case 0:
		currcol=1;
		break;
		case 1:
		currcol=7;
		break;
		case 2:
		currcol=5;
		break;
		case 3:
		break;
		case 4:
		break;
		case 5:
		break;
		case 6:
		break;
		case 7:
		break;
		case 8:
		break;
		case 9:
		break;
	}
	
}
void Tree::msgdata(){
	if(!curseenable)
		return;
	
	if(looped){
	for(int i = 0; i < SYSblocks; i++){
			sysmps[i][spos] = sysccc[1][i];
			sysmpsbuff[i]=sysccc[1][i];
			sysccc[1][i]=0;
	}
	for(int i = 0; i < 10; i++){
		int minm = 100;
		for(int x = 0; x < 60; x++){
			if(sysmps[i][x] < minm){
				minm = sysmps[i][x];
			}
		}
		minmsg[i]=minm;
	}
	for(int i = 0; i < 10; i++){
		int maxm = 0;
		for(int x = 0; x < 60; x++){
			if(sysmps[i][x] > maxm){
				maxm = sysmps[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 10; i++){
		int avgm = 0;
		for(int x = 0; x < 60; x++){
			avgm = avgm + sysmps[i][x];
		}
		avgmsg[i]=avgm/60;
	}
	}
	else{
	for(int i = 0; i < SYSblocks; i++){
			sysmps[i][spos] = sysccc[1][i];
			sysmpsbuff[i]=sysccc[1][i];
			//sysmps[x][spos] = rand();
			sysccc[1][i]=0;
	}
	for(int i = 0; i < 10; i++){
		int minm = 100;
		for(int x = 0; x < spos; x++){
			if(sysmps[i][x] < minm){
				minm = sysmps[i][x];
			}
		}
		minmsg[i]=minm;
	}
	for(int i = 0; i < 10; i++){
		int maxm = 0;
		for(int x = 0; x < spos; x++){
			if(sysmps[i][x] > maxm){
				maxm = sysmps[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 10; i++){
		int avgm = 0;
		for(int x = 0; x < spos; x++){
			avgm = avgm + sysmps[i][x];
		}
		if(spos != 0)
		avgmsg[i]=avgm/spos;
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
	if(stime >= 86400){
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
		ttp << "   ";
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
		ttp << "      ";
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
			ttp << "          " << stime;
		}
		else{
			ttp << "         " << stime;
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
	if (logpos < 20){
		LogMsgs[logpos] = input;
		logpos++;
	}
	else{
		for(int i = 0; i < 19; i++){
			LogMsgs[i]=LogMsgs[i+1];
		}
		LogMsgs[19]=input;
	}
	LogRef();
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
	if(!curseenable)
		return;
	//LOGwin
	werase(LOGwin);
	werase(LOGwinb);
	wborder(LOGwinb, 0,0,0,0,0,0,0,0);
	wborder(LOGwin, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattron(LOGwinb, COLOR_PAIR(4));
	wmove(LOGwinb, 0, 1);
	wprintw(LOGwinb, "Logging");
	wattroff(LOGwinb, COLOR_PAIR(4));
	wattron(LOGwin, COLOR_PAIR(4));
	wmove(LOGwin, 0,0);
	for(int i = 0; i < 20; i++){
		if(LogMsgs[i]=="")
			break;
		std::stringstream ls;
		ls << LogMsgs[i] << "\n";
		std::string lss = ls.str();
		const char * lchar = lss.c_str();
		wprintw(LOGwin, lchar);
		
	}
	wattroff(LOGwin, COLOR_PAIR(4));
	wrefresh(LOGwinb);
	wrefresh(LOGwin);
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