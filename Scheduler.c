#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>	
struct Process
{
	int ProcessNo;
	int ServiceTime,ArrivalTime,TurnAroundTime,WaitTime,SetFlag;
	struct Process *next,*prev;
};
typedef struct Process ReadyList;
FILE *fp;
ReadyList *front=NULL,*rear=NULL,*temp=NULL,*new=NULL;
int timequantum,processno;
/*Implementing RounRobin Scheduling*/
void RoundRobin(FILE *fp,ReadyList *temp,int at,int overhead,int pno)
{	
	int st,sum=0,counter=0;
	float average;	
	temp=NULL;
	if(!at)
	{
		while(temp!=front)
		{
			if(temp==NULL)
				temp=front;
			temp->ArrivalTime=0;
			temp=temp->next;
		}
	}	
	temp=front;	
	while(pno>0)
	{				
		if((temp->ServiceTime!=0)&&((temp->ArrivalTime<=counter-overhead)||(temp->ArrivalTime==0)))
	       	{				
			st=temp->ServiceTime;
			st=st-timequantum;				
			if(st<=0)
			{					
			fprintf(fp,"%d:%d:%d:FINISHED\n",counter,temp->ProcessNo,counter+temp->ServiceTime);
			if((overhead)&&(pno!=1))
				fprintf(fp,"%d:Scheduler:%d\n",counter+temp->ServiceTime,counter+temp->ServiceTime+overhead);
			counter=counter+temp->ServiceTime+overhead;
			temp->TurnAroundTime=counter-temp->ArrivalTime-overhead;	
			temp->ServiceTime=0;				
			pno--;
			}
			else			
			{	
				if(temp->SetFlag==0)
				{
					temp->SetFlag=1;
					temp->WaitTime=counter-temp->ArrivalTime;
				}					
				fprintf(fp,"%d:%d:%d:NOT FINISHED\n",counter,temp->ProcessNo,counter+timequantum);
				if(overhead)
					fprintf(fp,"%d:Scheduler:%d\n",counter+timequantum,counter+timequantum+overhead);
				counter=counter+timequantum+overhead;	
				temp->ServiceTime=st;					
				}
		  	}
		  	temp=temp->next;
		}
		/*TurnAround Time*/
		temp=NULL;
		while(temp!=front)
		{
			if(temp==NULL)
				temp=front;
			fprintf(fp,"TRnd(P%d)=%d\n",temp->ProcessNo,temp->TurnAroundTime);
			sum=sum+temp->TurnAroundTime;				
			temp=temp->next;
		}	
		average=sum/processno;
		fprintf(fp,"AvgTRnd time=%f\n",average);		
		/*Wait Time*/			
		temp=NULL;	
		sum=0;		
		while(temp!=front)
		{
			if(temp==NULL)
				temp=front;
			fprintf(fp,"W(P%d)=%d\n",temp->ProcessNo,temp->WaitTime);
			sum=sum+temp->WaitTime;				
			temp=temp->next;
		}
		average=sum/processno;
		fprintf(fp,"AvgWait time=%f\n",average);
		fclose(fp);
}	
ReadyList * sorting(ReadyList *front)
{
	/*Sorting the list*/
	int pid,at,st;	
	temp=NULL;
	new=NULL;
	while(temp!=front)
	{		
		if(temp==NULL)
			temp=front;		
		while(new!=front)
		{
			if(new==NULL)
				new=front;
			if(new->ArrivalTime>temp->ArrivalTime)
			{
				pid=new->ProcessNo;
				st=new->ServiceTime;
				at=new->ArrivalTime;
				new->ProcessNo=temp->ProcessNo;
				new->ArrivalTime=temp->ArrivalTime;
				new->ServiceTime=temp->ServiceTime;
				temp->ProcessNo=pid;
				temp->ServiceTime=st;
				temp->ArrivalTime=at;
			}
			new=new->next;
		}
		new=NULL;	
		temp=temp->next;
	}
	temp=front;
 	return temp;
}		
main(int argc,char *argv[])
{
	ReadyList *new;
	FILE *fp;
	int i,j,k,pid,st,at,overhead;
	int pno,choice;
	char str1[200];
	fp=fopen(argv[1],"r");
	fscanf(fp,"%s %s %d",str1,str1,&processno);
	fscanf(fp,"%s %s %d",str1,str1,&timequantum);
	fscanf(fp,"%s %s %d",str1,str1,&overhead);	
	
	/*Create the list*/		
	new=(ReadyList *)malloc(sizeof(ReadyList));	
	front=new;	
	for(i=0;i<processno;i++)
	{
		temp=new;		
		fscanf(fp,"%s %s %d %s %d %s %d",str1,str1,&pid,str1,&st,str1,&at);
		new->ProcessNo=pid;
		new->ServiceTime=st;
		new->ArrivalTime=at;
		new->SetFlag=0;
		new->next=(ReadyList *)malloc(sizeof(ReadyList));
		new=new->next;
		new->prev=temp;	
	}
	rear=new->prev;
	rear->next=front;
	front->prev=rear;
	new=NULL;
	fclose(fp);
	/*Enter the choices*/	
	printf("\n\tEnter your Choice:\n\t1)No arival and No context switch overhead\n\t2)No overhead but considering arrival time\n\t3)Considering overhead but no arival time\n\t4)Considering both arrival time and overhead");
	printf("\n\tChoice:");
	scanf("%d",&choice);
	temp=front;
	pno=processno;
	switch(choice)
	{
			case 1:
			/*no arrival,no overhead*/			
			fp=fopen("output1.txt","w");
			fprintf(fp,"Without considering arrival time and overhead\n");			
			RoundRobin(fp,front,0,0,processno);
			printf("\n\tYour output has been generated into the file 'output1.txt'");			
			break;
			case 2:
			/*no overhead*/
			fp=fopen("output2.txt","w");
			fprintf(fp,"Considering arrival time but not overhead\n");
			front=sorting(front);			
			RoundRobin(fp,front,1,0,processno);
			printf("\n\tYour output has been generated into the file 'output2.txt'");
			break;
			case 3:
			/*No arrival time but considering overhead*/
			fp=fopen("output3.txt","w");
			fprintf(fp,"Without considering arrival time but considering overhead\n");
			RoundRobin(fp,front,0,overhead,processno);
			printf("\n\tYour output has been generated into the file 'output3.txt'");
			break;
			case 4:
			/*with arrival time and ovrhead*/
			fp=fopen("output4.txt","w");
			fprintf(fp,"Considering arrival time and overhead\n");
			front=sorting(front);
			RoundRobin(fp,front,1,overhead,processno);
			printf("\n\tYour output has been generated into the file 'output4.txt'");	
			break;
	}	
}
