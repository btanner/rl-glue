#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gnu.h"
//code for calling gnuplot from C. required import.
#include "callGnu.h" 
/*********************************************************************************
*	This routine takes the files saved using the stats routines in RL_util.
*	Call this after experiment has run. 
*
*	Feel free to tune to your needs but mind file name conventions. Need to match
*	those found in RL_util.
*********************************************************************************/
int gnuplot(const char *gnucommand);
int graph(int num, bool Cont);
int graphBenchmark(int num, bool Cont);


//------------------------------------------------------------------
//This routine basically creates the string that contains your
//plotting commands, which is passed to gnuplot.
//
//num is number of episodes
//cont is wether task was continuing or not
//
//If you change data file naming conventions here be sure to change 
//them in RL_util.h as well
//------------------------------------------------------------------
int graph(int num, bool Cont)
{     
	char temp[10000];
	char gnuString[100000];
	char file1[10000];
	char file2[10000];
	char file3[10000];
	int ret;
	

		if(!Cont)
		{
			strcpy(file1,"Data/averageRewardChallenge");
			strcpy(file2,"Data/numberstepsChallenge");
			strcpy(file3,"Data/cummulativeRewardChallenge");
  
			sprintf(temp,"_%d.dat", num);
			strcat(file1,temp);
			strcat(file2,temp);  
			strcat(file3,temp);

			strcpy(gnuString,"set multiplot; set size .5,.5; set xlabel 'Number of episodes'; set ylabel 'Average reward'; set origin 0.5,.5;plot'");
			strcat(gnuString,file1);
			strcat(gnuString,"' with lines;set ylabel 'Number of Steps'; set origin 0,0; plot '");
			strcat(gnuString,file2);
			strcat(gnuString,"' with lines;set ylabel 'Cummulative Reward';set origin 0,.5; plot '");   //.33
			strcat(gnuString,file3);
			strcat(gnuString,"' with lines;");
				
			ret = gnuplot(gnuString);            
		}
		else
		{
			strcpy(file1,"Data/averageRewardChallenge");
			strcpy(file3,"Data/cummulativeRewardChallenge");
			
			strcpy(temp,"CONT.dat");
			strcat(file1,temp);
			strcat(file3,temp);
	
			strcpy(gnuString,"set multiplot; set size 1,.5; set xlabel 'Number of steps'; set ylabel 'Average reward'; set origin 0,.5;plot '");   
			strcat(gnuString,file1);
			strcat(gnuString,"' with lines;set ylabel 'Cummulative Reward';set origin 0,0; plot '");   //.33
			strcat(gnuString,file3);
			strcat(gnuString,"' with lines;");
			
			ret = gnuplot(gnuString); //pass in plotting routine           
		}
	
	if(ret == -1)
	{
		printf("\nYou dont have gnuplot OR I can't find it (either download it or put path in your .bassh file OR edit the execution path in the exec call of 'callGnu.h')\n");
		printf("\nYour output is stored in data files: \n\n%s,\n%s,\n%s\n\n",file1,file2,file3);
	} 
	exit(1);
}

//GRAPH ROUTINE USED FOR NIPS
int graphBenchmark(int num, int n, bool Cont)
{     
	char temp[10000];
	char tmp[10000];
	char gnuString[100000];
	char file1[10000];
	char file2[10000];
	char file3[10000];
	int ret;
	

		if(!Cont)
		{

  
  			sprintf(tmp,"set multiplot; set size .5,.5; set xlabel '%d episode blocks'; set ylabel 'Average reward'; set origin 0.5,.5;plot '",n);
			strcpy(gnuString,tmp);
			
			strcpy(file1,"Data/averageRewardChallenge");
			strcpy(file2,".Data/numberstepsChallenge");
			strcpy(file3,"Data/TimeChallenge");
			
			sprintf(temp,"_%d.dat", num);
			strcat(file1,temp);
			strcat(file2,temp);  
			strcat(file3,temp);



			strcat(gnuString,file1);
			strcat(gnuString,"' with lines;set ylabel 'Average Number of Steps'; set origin 0,0; plot '");
			strcat(gnuString,file2);
			strcat(gnuString,"' with lines;set ylabel 'Average Time (micro sec)';set origin 0,.5; plot '");   //.33
			strcat(gnuString,file3);
			strcat(gnuString,"' with lines;");
				
			ret = gnuplot(gnuString);            
		}
		else
		{
  			sprintf(tmp,"set multiplot; set size .5,.5; set xlabel '%d episode blocks'; set ylabel 'Average reward'; set origin 0.5,.5;plot '",n);
			strcpy(gnuString,tmp);
			
			strcpy(file1,"Data/averageRewardChallenge");
			strcpy(file3,"Data/TimeChallenge");
			
			strcpy(temp,"CONT.dat");
			strcat(file1,temp);
			strcat(file3,temp);
	
			strcat(gnuString,file1);
			strcat(gnuString,"' with lines;set ylabel 'Average Time (micro sec)';set origin 0,0; plot '");   //.33
			strcat(gnuString,file3);
			strcat(gnuString,"' with lines;");
			
			ret = gnuplot(gnuString); //pass in plotting routine           
		}
	
	if(ret == -1)
	{
		printf("\nYou dont have gnuplot OR I can't find it (either download it or put path in your .bassh file OR edit the execution path in the exec call of 'callGnu.h')\n");
		printf("\nYour output is stored in data files: \n\n%s,\n%s,\n%s\n\n",file1,file2,file3);
	} 
	exit(1);
}

