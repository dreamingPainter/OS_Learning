#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
//-p, --show-pids: 打印每个进程的进程号。
//-n --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。sort output by PID
//-V --version: 打印版本信息。
//考虑子进程pid比父进程大
#define TRUE 1
#define FALSE 0
//排序按照ppid的大小去排
//存储目录文件的结构
struct myprocess{
  pid_t Mypid;
  pid_t Myppid;
  char * process_name;
  myprocess *brother;
  myprocess *son;
};

void new_read_dir(char path[128],myprocess *now,myprocess *before);
void read_status_file(char path[128],myprocess *info);
//本来想用于简化命令的，感觉更麻烦了
struct pstree_para{
  char state_p; //show pid
  char state_n; //sort output by pid    
};
typedef struct pstree_para pstree_para;
typedef struct myprocess myprocess;

//若读到的第一个不是pid=1,存在可能性的bug
void new_read_dir(char path[128],myprocess *now,myprocess *before)//
{
  char order[128],num_of_dir[4],line[64];
  //目录个数统计命令拼接
  strcat(order,"ls ");
  strcat(order,path);
  strcat(order, " -lR | grep \"^d\" | wc -l");
  FILE *fp_d_num = popen(order,"r");
  strcpy(order,"");

  if (fp_d_num == NULL) exit(1);
  else if (fgets(num_of_dir,sizeof(num_of_dir),fp_d_num)!=NULL)
  {
    printf("This directory has %s sub-dir,loading directory name\n",num_of_dir);
        
    //存储目录名称的列表命令拼接
    strcat(order,"ls ");
    strcat(order,path);
    strcat(order, " -F|grep \"/$\"");
    FILE *fp_d_name = popen(order,"r");
    strcpy(order,"");

    while (fgets(line,sizeof(line),fp_d_name))
    {
      char temp[32];
      int value;
      myprocess *temp_process;

      if(sscanf(line,"%s/",temp))
      {
        //读当前行内容
        value = atoi(temp);
        temp_process->Mypid = value;
        char new_path[128]; 
        strcat(new_path,path);
        strcat(new_path,line);
        read_status_file(new_path,temp_process);//读一帧
        
        //对第一帧的处理
        if(now->Mypid==-1)
        {
          now->Mypid = temp_process->Mypid;
          now->Myppid = temp_process->Myppid;
          strcpy(now->process_name,temp_process->process_name);
          before = now;
          now = (myprocess*)malloc(sizeof(myprocess));
        }
        //有没有儿子
        if(strcmp(line,"task/")==0)//有儿子
        {
          strcat(path,"task/");
          new_read_dir(path,now,before);
        }
        //没儿子
        else{
          //是上一个的儿子还是兄弟
          if(before == NULL)
            continue;
          else if(now->Myppid == before->Myppid)//是兄弟
            {
              before->brother = now;
              before = now;
              now = (myprocess*)malloc(sizeof(myprocess));
              continue;
            }
          else if(now->Myppid == before->Mypid)//是儿子,此处逻辑有bug,注意!!!!即task和ppid值在pstree上打印不一致的情况
          {
            before->son = now;
            now = (myprocess*)malloc(sizeof(myprocess));
            continue;
          }
        }
      }
    }
  }
}

// 参数:所读文件路径,进程编号,对应的pid信息结构体
void read_status_file(char path[128],myprocess *info)
{
  char file_path[128],temp[32];
  int i = 0;
  strcat(file_path,path);
  strcat(file_path,"status");
  FILE *fp = fopen(file_path,"r");

  for(i=0;i<=6;i++)
  {
    char name[128];
    fgets(temp,sizeof(temp),fp);
    if(i == 0) {
      sscanf("Name:%s",name);
      info->process_name = (char*)malloc((strlen(name)+1)*sizeof(char));
      strcpy(info->process_name,name);
      }
    else if( i==6 ) sscanf(temp,"Ppid:%d",&(info->Myppid));
  }
}

/*目标:实现pstree结构|实现排序功能|显示进程号功能*/
int main(int argc, char *argv[]) {
  //test zone///////////
  
  //////////////////////
  
  //变量初始化//
  myprocess *head,*prv;
  myprocess *process_list = (myprocess *)malloc(sizeof(myprocess));
  process_list->Mypid = -1;
  head = process_list;//head指向第一个进程
  prv = NULL;

  //读命令//
  /*参数:
    argc表示参数数目;
    argv[0]为程序启动的路径;
  */
  pstree_para para;
  for (int i = 1; i < argc; i++) {
    assert(argv[i]); // C 标准保证
    printf("argv[%d] = %s\n", i, argv[i]);
    if(strcmp(argv[i],"-n")==0) para.state_n = TRUE;
    else if(strcmp(argv[i],"-p")==0) para.state_p = TRUE;
    else{
      printf("Error, order not found!\n");
      exit(1);
    }
  }
  assert(!argv[argc]); //C标准保证
  
  //读取指定目录下所有进程//
  new_read_dir("/proc/",process_list,prv);

  //释放空间
  return 0;
  }
