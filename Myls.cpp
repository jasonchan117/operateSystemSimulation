/*************************************************************************
	> File Name: Myls.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年05月18日 星期五 20时42分42秒
 ************************************************************************/
#include<dirent.h>
#include<pwd.h>
#include<unistd.h>
#include<iostream>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>
#include<ctime>
#include<grp.h>
using namespace std;
char *judge_mod(mode_t m,char *mod){
    if(S_ISDIR(m)) mod[0]='d';
    else mod[0]='-';

    if((m&S_IRWXU)&S_IRUSR) mod[1]='r';
    else mod[1]='-';
    if((m&S_IRWXU)&S_IWUSR) mod[2]='w';
    else mod[2]='-';
    if((m&S_IRWXU)&S_IXUSR) mod[3]='x';
    else mod[3]='-';

    if((m&S_IRWXG)&S_IRGRP) mod[4]='r';
    else mod[4]='-';
    if((m&S_IRWXG)&S_IWGRP) mod[5]='w';
    else mod[5]='-';
    if((m&S_IRWXG)&S_IXGRP) mod[6]='x';
    else mod[6]='-';

    if((m&S_IRWXO)&S_IROTH) mod[7]='r';
    else mod[7]='-';
    if((m&S_IRWXO)&S_IWOTH) mod[8]='w';
    else mod[8]='-';
    if((m&S_IRWXO)&S_IXOTH) mod[9]='x';
    else mod[9]='-';
    return mod;
}
int main(int argc,char* argv[],char** env){
    DIR *dir;
    struct dirent *dp;
    int oc;
    int flaga=0;
    int flagl=0;
    for(int i=0;i<argc;i++){
        if(strcmp(argv[i],"-a")==0) flaga=1;
        if(strcmp(argv[i],"-l")==0) flagl=1;
    }
    int f=0;
    if(argc==1){

        dir=opendir(".");
        while((dp=readdir(dir))!=NULL){
            if(dp->d_name[0]!='.'){
                                    
                                        struct passwd *u;
                                        struct passwd *g;
                                        struct stat st;
                                      
                                        char *filename=dp->d_name;        
                                        struct group *ggg;
                                        ggg=getgrgid(getgid());
                                        char file_path[]="./";
                                        strcat(file_path,dp->d_name);
                                        stat(file_path,&st);
                                        char moo[11]={0};
                                        judge_mod(st.st_mode,moo);
                                        if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
                                        else if(!access(file_path,X_OK)) printf("\033[;32m%10s\033[0m ",filename);
                                        else printf("%10s",filename);
                                        printf("\n");
                                        memset(file_path,0,sizeof(file_path));
                                    }
                            }
            }
    else if(argc==2&&strcmp(argv[1],"-a")!=0&&strcmp(argv[1],"-l")!=0&&strcmp(argv[1],"-al")!=0&&strcmp(argv[1],"-la")!=0){
        dir=opendir(argv[1]);
        while((dp=readdir(dir))!=NULL){
            if(dp->d_name[0]!='.'){

                                        char tempp[50];
                                        strcpy(tempp,argv[1]);
                                        struct passwd *u;
                                        struct passwd *g;
                                        struct stat st;
                                      
                                        char *filename=dp->d_name;        
                                        struct group *ggg;
                                        ggg=getgrgid(getgid());
                                        
                
                                                strcat(tempp,dp->d_name);
                                                stat(tempp,&st);
                                        char moo[11]={0};
                                        judge_mod(st.st_mode,moo);
                                        u=getpwuid(st.st_uid);
                                    
                                        char *temp=ctime(&st.st_ctime);
                
                                        
                                        if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
                                        else if(!access(tempp,X_OK)) printf("\033[;32m%10s\033[0m ",filename);
                                        else printf("%10s",filename);
                                        printf("\n");
                                        memset(tempp,0,sizeof(tempp));
                                    }
            }

    }
    else if(flaga==1&&flagl==1){
        if(argc>3) dir=opendir(argv[3]);
        else dir=opendir(".");
        if(argc>3) f=1;
        char file_path[]="./";
        while((dp=readdir(dir))!=NULL){
            char te[50]={0};
            if(f==1) strcpy(te,argv[3]);
            struct passwd *u;
            struct passwd *g;
            struct stat st;
            char *filename=dp->d_name;
            struct group *ggg;
            ggg=getgrgid(getgid());
            if(f==1){
                strcat(te,dp->d_name);
                stat(te,&st);
            }
            else{
            strcat(file_path,dp->d_name);
            stat(file_path,&st);
            }
            char moo[11]={0};
            judge_mod(st.st_mode,moo);
            u=getpwuid(st.st_uid);
            printf("%10s",moo);
            printf("%10lu",st.st_nlink);
            printf("%10s",u->pw_name);
            printf("%10s",ggg->gr_name);
            printf("%10ld",st.st_size);
            printf(" ");
            char *temp=ctime(&st.st_ctime);
            for(int i=0;i<strlen(temp);i++) if(temp[i]!='\n') printf("%c",temp[i]);
            printf("  ");
            if(moo[0]=='d')printf("\033[;34m%10s\033[0m",filename);
            else if(!access(file_path,X_OK)&&f==0) printf("\033[;32m%10s\033[0m ",filename);
            else if(!access(te,X_OK)&&f==1) printf("\033[;32m%10s\033[0m ", filename);
            else printf("%10s",filename);
            printf("\n");
            memset(te,0,sizeof(te));
            memset(file_path,0,sizeof(file_path));
        }
    }else{
    while((oc=getopt(argc,argv,"a::l::"))!=-1){
        switch(oc){
            case 'a': 
                if(optarg!=NULL){
                    int flag=0;
                    if(argc>=3) {
                        flag=1;
                        dir=opendir(argv[2]);
                    }
                    else dir=opendir(".");
                    while((dp=readdir(dir))!=NULL){
                        char tempp[50]={0};
                        
                        if(flag==1) strcpy(tempp,argv[2]);
                        struct passwd *u;
                        struct passwd *g;
                        struct stat st;
                                                  
                        char *filename=dp->d_name;        
                        struct group *ggg;
                        ggg=getgrgid(getgid());
                        char file_path[]="./";
                        
                        if(flag==1){
                        strcat(tempp,dp->d_name);
                        stat(tempp,&st);
                        }
                        else{
                            strcat(file_path,dp->d_name);
                            stat(file_path,&st);
                        }
                        
                        char moo[11]={0};
                        judge_mod(st.st_mode,moo);
                        u=getpwuid(st.st_uid);
                        printf("%10s",moo);
                        printf("%10lu",st.st_nlink);
                        printf("%10s",u->pw_name);
                        printf("%10s" ,ggg->gr_name);
                        printf("%10ld",st.st_size);
                        printf(" ");
                        char *temp=ctime(&st.st_ctime);
                        for(int i=0;i<strlen(temp);i++){
                            if(temp[i]!='\n') printf("%c",temp[i]);                                    
                        }
                        printf("  ");
                        if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
                        else if(!access(file_path,X_OK)&&flag==0) printf("\033[;32m%10s\033[0m ",filename);
                        else if(!access(tempp,X_OK)&&flag==1) printf("\003[;32m%10s\033[0m ",filename);
                        else printf("%10s",filename);
                        printf("\n");
                        memset(file_path,0,sizeof(file_path));
                        memset(tempp,0,sizeof(tempp));
                    }
                }
                else{
                    int flag=0;
                    if(argc>=3&&strcmp(argv[argc-1],"-l")!=0){
                        flag=1;              
                        dir=opendir(argv[argc-1]);
                    }
                    else dir = opendir(".");
                    while((dp=readdir(dir))!=NULL){
                        char tempp[50]={0};
                        if(flag==1) strcpy(tempp,argv[argc-1]);
                        struct passwd *u;
                        struct passwd *g;
                        struct stat st;
                        char *filename=dp->d_name;        
                        struct group *ggg;
                        ggg=getgrgid(getgid());
                        char file_path[]="./";
                        if(flag==1){
                            strcat(tempp,dp->d_name);
                            stat(tempp,&st);
                        }
                        else{
                        strcat(file_path,dp->d_name);
                        stat(file_path,&st);
                        }
                        char moo[11]={0};
                        judge_mod(st.st_mode,moo);
                        u=getpwuid(st.st_uid);
                        if(moo[0]=='d')printf("\033[;34m%s\033[0m ",filename);
                        else if(!access(file_path,X_OK)&&flag==0) printf("\033[;32m%s\033[0m ",filename);
                        else if(!access(tempp,X_OK)&&flag==1) printf("\033[;32m%s\033[0m ",filename);
                        else printf("%s",filename);
                        memset(tempp,0,sizeof(tempp));
                        memset(file_path,0,sizeof(file_path));
                    }
                    printf("\n");
                }
                break;
            case 'l':

                //dir=opendir(".");
                if(optarg!=NULL){
                    int flag=0;
                    if(argc>=3){
                        flag=1;
                        dir=opendir(argv[2]);
                    }
                    else dir=opendir(".");
                    while((dp=readdir(dir))!=NULL){
                        char tempp[50]={0};
                        if(flag==1) strcpy(tempp,argv[2]);
                        //printf("****%s****%s      ",tempp,argv[2]);
                        struct passwd *u;
                        struct passwd *g;
                        struct stat st;   
                        char *filename=dp->d_name;        
                        struct group *ggg;
                        ggg=getgrgid(getgid());
                        
                        char file_path[]="./";
                        if(flag==1){
                            strcat(tempp,dp->d_name);
                            stat(tempp,&st);
                            
                        }
                        else{
                        strcat(file_path,dp->d_name);
                        stat(file_path,&st);
                        }
                        char moo[11]={0};
                        judge_mod(st.st_mode,moo);
                        u=getpwuid(st.st_uid);
                        printf("%10s",moo);
                        printf("%10lu",st.st_nlink);
                        printf("%10s",u->pw_name);
                        printf("%10s" ,ggg->gr_name);
                        printf("%10ld",st.st_size);
                        printf(" ");
                        
                        char *temp=ctime(&st.st_ctime);
                        for(int i=0;i<strlen(temp);i++){
                            if(temp[i]!='\n') printf("%c",temp[i]);                            
                        }
                        printf("  ");
                        if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
                        else if(!access(file_path,X_OK)&&flag==0) printf("\033[;32m%10s\033[0m ",filename);
                        else if(!access(tempp,X_OK)&&flag==1) printf("\033[;32m%10s\033[0m ",filename);
                        else printf("%10s",filename);
                        printf("\n");          
                        memset(tempp,0,sizeof(tempp));
                        memset(file_path,0,sizeof(file_path));
                    }
                }
                else{
                int flag=0;
                if(argc>=3&&strcmp(argv[argc-1],"-a")!=0) {
                    flag=1;
                   dir=opendir(argv[argc-1]);
                }
                else dir = opendir(".");
                while((dp=readdir(dir))!=NULL){
                    if(dp->d_name[0]!='.'){
                        char tempp[50];
                        if(flag==1) strcpy(tempp,argv[argc-1]);
                        struct passwd *u;
                        struct passwd *g;
                        struct stat st;
                      
                        char *filename=dp->d_name;        
                        struct group *ggg;
                        ggg=getgrgid(getgid());
                        char file_path[]="./";
                        if(flag==1){
                            strcat(tempp,dp->d_name);
                            stat(tempp,&st);
                        }
                        else{
                        strcat(file_path,dp->d_name);
                        stat(file_path,&st);
                        }
                        char moo[11]={0};
                        judge_mod(st.st_mode,moo);
                        u=getpwuid(st.st_uid);
                        printf("%10s",moo);
                        printf("%10lu",st.st_nlink);
                        printf("%10s",u->pw_name);
                        printf("%10s" ,ggg->gr_name);
                        printf("%10ld",st.st_size);
                        printf(" ");
                        char *temp=ctime(&st.st_ctime);
                        for(int i=0;i<strlen(temp);i++){
                            if(temp[i]!='\n') printf("%c",temp[i]);
                                                    
                        }
                        printf("  ");
                        if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
                        else if(!access(file_path,X_OK)&&flag==0) printf("\033[;32m%10s\033[0m ",filename);
                        else if(!access(tempp,X_OK)&&flag==1) printf("\033[;32m%10s\033[0m ",filename);
                        else printf("%10s",filename);
                        printf("\n");
                        memset(tempp,0,sizeof(tempp));
                        memset(file_path,0,sizeof(file_path));
                    }
                }
                }
                break;
            
        }
        /*while((dp=readdir(dir))!=NULL){
            struct passwd *u;
            struct passwd *g;
            struct stat st;
        
            char *filename=dp->d_name;        
            struct group *ggg;
            ggg=getgrgid(getgid());
            char file_path[]="./";
            strcat(file_path,dp->d_name);
            stat(file_path,&st);
            char moo[11]={0};
            judge_mod(st.st_mode,moo);
            u=getpwuid(st.st_uid);
            printf("%10s",moo);
            printf("%10lu",st.st_nlink);
            printf("%10s",u->pw_name);
            printf("%10s" ,ggg->gr_name);
            printf("%10ld",st.st_size);
            if(moo[0]=='d')printf("\033[;34m%10s\033[0m ",filename);
            else if(!access(file_path,X_OK)) printf("\033[;32m%10s\033[0m ",filename);
            else printf("%10s",filename);
            printf("%10s\n",ctime(&st.st_ctime));
        
        }*/
        }
    }
    closedir(dir);
    return 0;
}
