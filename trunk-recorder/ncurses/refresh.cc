void RecRef(){
	werase(RECwin);
	wborder(RECwin, 0,0,0,0,0,0,0,0);
	wattron(RECwin, COLOR_PAIR(4));
	wmove(RECwin, R1h-1, 1);
	wprintw(RECwin, "Recorders");
	wattroff(RECwin, COLOR_PAIR(4));
	//RECwin
	for(int x = 0; x < TGblocks; x++){
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
	wattron(RECwin, COLOR_PAIR(3));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(3));
	wmove(RECwin, 1+i,4+(TGblockx*x));
	if(getcol(digitalgroups[x][i][2])){
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
	wattron(RECwin, COLOR_PAIR(2));
	wprintw(RECwin, d);
	wattroff(RECwin, COLOR_PAIR(2));
	wmove(RECwin, 1+i+digrec[x],4+(TGblockx*x));
	if(getcol(analoggroups[x][i][2])){
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
	}
	wmove(RECwin, digrec[x]+anarec[x]+2, 2+(x*TGblockx));
	wattron(RECwin, COLOR_PAIR(4));
	const char * r = Radios[x].c_str();
	wprintw(RECwin, r);
	wattroff(RECwin, COLOR_PAIR(4));
	}
}
void MsgRef(){
	werase(SYSwin);
	wborder(SYSwin, 0,0,0,0,0,0,0,0);
	wattron(SYSwin, COLOR_PAIR(4));
	wmove(SYSwin, R1h-1, 1);
	wprintw(SYSwin, "Systems");
	wattroff(SYSwin, COLOR_PAIR(4));
	int datapos = 6;
	wattron(SYSwin, COLOR_PAIR(4));
	wmove(SYSwin, 1, 1);
	wprintw(SYSwin, "100");
	for(int i = 1; i < 19; i++){
		std::stringstream ss;
		std::string s;
		wmove(SYSwin, i+1, 2);
		ss << 100-i*5;
		ss >> s;
		const char * c = s.c_str();
		wprintw(SYSwin, c);
	}
	wmove(SYSwin, 20, 3);
	wprintw(SYSwin, "5");
	wmove(SYSwin, 21, 3);
	wprintw(SYSwin, "0");
	wmove(SYSwin, 22, 1);
	wprintw(SYSwin, "MPS");
	
	wmove(SYSwin, 29, 1);
	if(SYSblocks < 3){
		wprintw(SYSwin, "Msg Rate");
	}
	else{
		wprintw(SYSwin, "Message Rate");
	}
	wattroff(SYSwin, COLOR_PAIR(4));
	wattron(SYSwin, COLOR_PAIR(2));
	wmove(SYSwin, 24, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Max");
	else
		wprintw(SYSwin, "Maximum");
	wattroff(SYSwin, COLOR_PAIR(2));
	wattron(SYSwin, COLOR_PAIR(1));
	wmove(SYSwin, 25, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Avg");
	else
		wprintw(SYSwin, "Average");
	wattroff(SYSwin, COLOR_PAIR(1));
	wattron(SYSwin, COLOR_PAIR(3));
	wmove(SYSwin, 26, 1);
	if(SYSblocks<2)
		wprintw(SYSwin, "Min");
	else
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
}
void DatRef(){
	werase(DATwin);
	wborder(DATwin, 0,0,0,0,0,0,0,0);
	wattron(DATwin, COLOR_PAIR(4));
	wmove(DATwin, R1h-1, 1);
	wprintw(DATwin, "Call Data");
	wattroff(DATwin, COLOR_PAIR(4));
	wattron(DATwin, COLOR_PAIR(4));
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
	wattroff(DATwin, COLOR_PAIR(4));
}
void OldRef(){
	werase(OLDwin);
	wborder(OLDwin, 0,0,0,0,0,0,0,0);
	wattron(OLDwin, COLOR_PAIR(4));
	wmove(OLDwin, R1h-1, 1);
	wprintw(OLDwin, "Past Calls");
	wattroff(OLDwin, COLOR_PAIR(4));
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
}
void CpuRef(){
	werase(CPUwin);
	wborder(CPUwin, 0,0,0,0,0,0,0,0);
	wattron(CPUwin, COLOR_PAIR(4));
	wmove(CPUwin, R1h-1, 1);
	wprintw(CPUwin, "CPU");
	wattroff(CPUwin, COLOR_PAIR(4));
		int datapos = 6;
	wattron(CPUwin, COLOR_PAIR(4));
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
	/*
	wmove(CPUwin, 22, 6+CPUblocks);
	wprintw(CPUwin, "151");
	wmove(CPUwin, 23, 6+CPUblocks);
	wprintw(CPUwin, "  5");
	wmove(CPUwin, 24, 6+CPUblocks);
	wprintw(CPUwin, "AVG");
	wattroff(CPUwin, COLOR_PAIR(4));
	*/
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
}
void MtgRef(){
	werase(MRECwin);
	wborder(MRECwin, 0,0,0,0,0,0,0,0);
	wattron(MRECwin, COLOR_PAIR(4));
	wmove(MRECwin, R1h-1, 1);
	wprintw(MRECwin, "Missing");
	wattroff(MRECwin, COLOR_PAIR(4));
	if(mtgcount<28)
	for(int i = 0; i < mtgcount; i ++){
		std::stringstream hh;
		std::string hstr;
		hh << MTGs[i][0];
		hh >> hstr;
		const char * hchar = hstr.c_str();
		getcol(MTGs[i][1]);
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
		getcol(MTGs[i][1]);
		wattron(MRECwin, COLOR_PAIR(currcol));
		wmove(MRECwin, 1+i, 1);
		wprintw(MRECwin, hchar);
		wattroff(MRECwin, COLOR_PAIR(currcol));
	}
	}
}
void ErrRef(){
	werase(ERRwin);
	wborder(ERRwin, 0,0,0,0,0,0,0,0);
	wattron(ERRwin, COLOR_PAIR(4));
	wmove(ERRwin, R1h-1, 1);
	wprintw(ERRwin, "Errors");
	wattroff(ERRwin, COLOR_PAIR(4));
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
}