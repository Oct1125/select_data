#include "select.h"

//打印全部测站平均收敛精度、时间
void PrintConAccALL(const CFG cfg,pcon_acc acc_con_all,int k,int row,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"con_all",7);
    fp=fopen(tmp,"w");
    for(i=0;i<cfg.CONACC_TIME/cfg.CONACC_CONTIME;i++)
    {
        fprintf(fp,"%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",acc_con_all[i].acc_E/cfg.ACC_UNIT_ALL,acc_con_all[i].acc_N/cfg.ACC_UNIT_ALL,acc_con_all[i].acc_U/cfg.ACC_UNIT_ALL,
        acc_con_all[i].time_E/cfg.TIME_UNIT_ALL,acc_con_all[i].time_N/cfg.TIME_UNIT_ALL,acc_con_all[i].time_U/cfg.TIME_UNIT_ALL);
    }
    fclose(fp);
}

//打印全部测站平均收敛精度、时间（绝对值）
void PrintConAccALL_fab(const CFG cfg,pcon_acc acc_con_all,int k,int row,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"con_all_fab",11);
    fp=fopen(tmp,"w");
    for(i=0;i<cfg.CONACC_TIME/cfg.CONACC_CONTIME;i++)
    {
        fprintf(fp,"%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",fabs(acc_con_all[i].acc_E)/cfg.ACC_UNIT_ALL,fabs(acc_con_all[i].acc_N)/cfg.ACC_UNIT_ALL,fabs(acc_con_all[i].acc_U)/cfg.ACC_UNIT_ALL,
        acc_con_all[i].time_E/cfg.TIME_UNIT_ALL,acc_con_all[i].time_N/cfg.TIME_UNIT_ALL,acc_con_all[i].time_U/cfg.TIME_UNIT_ALL);
    }
    fclose(fp);
}