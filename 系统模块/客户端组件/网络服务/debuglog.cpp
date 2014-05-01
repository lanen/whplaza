#include "stdafx.h"

time_t timer;
struct tm *t;
char* __file__;
int __line__;

int DebugLogOut(char *fmt, ...)
              {
                   va_list ap;               	         
	               char	sDbgFile[256];
	               char	sOutBuf[4096];
	               time_t	iCurTim;
	               static time_t	iOldTim=0;
	               static int  DbgFd = -1;
	               struct tm stLocTim;
	               struct stat stStat;
                   iCurTim = time(NULL);
                   va_start(ap, fmt);
				   ofstream fout1;
			       stLocTim = *localtime(&iCurTim);	
                   sprintf(sOutBuf,"<%02d%02d%02d%02d%02d%02d>\"%s\"(%d): ",
			       stLocTim.tm_year % 100, stLocTim.tm_mon + 1,stLocTim.tm_mday,
			       stLocTim.tm_hour,stLocTim.tm_min,stLocTim.tm_sec,__file__,__line__
			       );
            		    
			       sprintf(sDbgFile,"%s.%02d%02d%02d%s","debuglog",stLocTim.tm_year % 100,stLocTim.tm_mon + 1,stLocTim.tm_mday,".txt");								
		   	       if(stat(sDbgFile,&stStat) == -1)
				   {
					   ofstream fout (sDbgFile);					  
		               fout<<sOutBuf;
	                   if(strlen(fmt) > 1024){
	 	                fout<<fmt;
					   }else{
		               va_start(ap,fmt);
		               (void)vsprintf(sOutBuf,fmt,ap);
		               va_end(ap);		
		               fout<<sOutBuf;
					   }		
	                   fout<<"\n";
				   }
				 else{
					  fout1.open(sDbgFile,ios_base::out|ios_base::app);				  
		              fout1<<sOutBuf;
	                  if(strlen(fmt) > 1024){
	 	               fout1<<fmt;
					  }else{
		                va_start(ap,fmt);
		                (void)vsprintf(sOutBuf,fmt,ap);
		                va_end(ap);		
		                fout1<<sOutBuf;
					  }		
	                  fout1<<"\n";
				 }
	              return 1;                                  
                  va_end(ap);
 }
