#include "select.h"

//收敛时间、精度统计（全部测站平均）
void AveConverAcc(const CFG cfg,pcon_acc acc_con_all,pcon_acc* acc,int k,int row,int num)
{
    int i=0,j=0;   
    for(i=0;i<cfg.CONACC_TIME/cfg.CONACC_CONTIME;i++)
    {
        for(j=0;j<num;j++)
        {
            acc_con_all[i].acc_E+=acc[j][i].acc_E;
            acc_con_all[i].acc_N+=acc[j][i].acc_N;
            acc_con_all[i].acc_U+=acc[j][i].acc_U;
            acc_con_all[i].time_E+=acc[j][i].time_E;
            acc_con_all[i].time_N+=acc[j][i].time_N;
            acc_con_all[i].time_U+=acc[j][i].time_U;
        }
        acc_con_all[i].acc_E/=num;
        acc_con_all[i].acc_N/=num;
        acc_con_all[i].acc_U/=num;
        acc_con_all[i].time_E/=num;
        acc_con_all[i].time_N/=num;
        acc_con_all[i].time_U/=num;
    }
}