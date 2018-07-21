/*************************************************************************
	> File Name: reader_and_writer.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年06月28日 星期四 13时31分05秒
 ************************************************************************/
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdlib.h>
static int reader_id=0;
static int writer_id=0;
#define INIT(glo,func) int func(void);static int glo=func(); int func()

typedef int semaphore;
int shmid=shmget(IPC_PRIVATE,100,IPC_CREAT|IPC_EXCL|0600);
int sh_db=shmget(IPC_PRIVATE,100,IPC_CREAT|IPC_EXCL|0600);
int sh_rc=shmget(IPC_PRIVATE,100,IPC_CREAT|IPC_EXCL|0600);
semaphore *db=(int *)shmat(sh_db,0,0);
semaphore *mutex=(int *)shmat(shmid,0,0);
semaphore *rc=(int *)shmat(sh_rc,0,0);
INIT(global,function){
    *mutex=1;
    *db=1;
    *rc=0;
}
void read_data(){
    char buff[100]={0};
    memset(buff,0,sizeof(buff));
    FILE *fp=fopen("data.txt","r");
    while((fgets(buff,sizeof(buff)-1,fp))){
        printf("%d read %s",reader_id,buff);
        memset(buff,0,sizeof(buff));
    }
    fclose(fp);
}
void write_data(){
    FILE *fp=fopen("data.txt","a+");

    char in_data[100]="data written by ";
    in_data[15]=writer_id+48;
    fprintf(fp,"%s\n",in_data);
    fclose(fp);
}
void down(int *mu){
    while((*mu)<=0){
        usleep(3000);
    }
    --(*mu);
    return;
}
void up(int *mu){
    ++(*mu);
}
void reader(){
    fflush(stdout);
    down(mutex);
    ++(*rc);
    if(*rc==1) down(db);
    printf("NO.%d reader is entering\n",reader_id);
    up(mutex);
    
    printf("NO.%d reader is reading data\n",reader_id);
    read_data();
    
    down(mutex);
    --(*rc);
    printf("NO.%d reader is exiting\n",reader_id);
    if(*rc==0) up(db);
    up(mutex);
}
void writer(){
    fflush(stdout);
    down(db);
    write_data();
    printf("NO.%d writer is writing data\n",writer_id);
    up(db);
}
int main(){
    int reader_num;
    int writer_num;
    printf("please enter the number of reader:\n");
    scanf("%d",&reader_num);
    printf("please enter the number of writer:\n");
    scanf("%d",&writer_num);
    int pid=fork();
    if(pid==-1) {perror("alocate processing failed");return -1;}
    else if(pid==0){
        int pid_r;
        for(int i=0;i<reader_num;i++){
            ++reader_id;
            pid_r=fork();
            if(pid_r==-1) {perror("alocate reader processing failed");return -1;}
            if(pid_r==0) break;
        } 
    }     
    else if(pid>0){
        int pid_w;
        for(int i=0;i<writer_num;i++){
            ++writer_id;
            pid_w=fork();
            if(pid_w==-1) {perror("alocate writer processing failed");return -1;}
            if(pid_w==0) break;
        }
    }   
    int co;
    int id;
    usleep(3000);
    if(pid==0){
        for(int i=1;i<=reader_num;i++){
            fflush(stdout);
            if(i==reader_id) reader();
        }
    }
    else if(pid>0){
        for(int i=1;i<=writer_id;i++){
            fflush(stdout);
            if(i==writer_id) writer();
        }
    }
    exit(1);
}

