#include "select.h"

//打印测站收敛精度、时间
void PrintConAcc(const CFG cfg,pdata* pos,pcon_acc* acc,int k,int row,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"con_",4);
    int l=strlen(tmp);
    for(i=0;i<num;i++)
    {
        setstr(tmp+l,pos[i][0].name,4);
        fp=fopen(tmp,"w");
        for(j=0;j<cfg.CONACC_TIME/cfg.CONACC_CONTIME;j++)
        {
            fprintf(fp,"%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf\n",
            fabs(acc[i][j].acc_E),fabs(acc[i][j].acc_N),fabs(acc[i][j].acc_U),
                acc[i][j].time_E/cfg.TIME_UNIT,acc[i][j].time_N/cfg.TIME_UNIT,acc[i][j].time_U/cfg.TIME_UNIT,
                acc[i][j].TTFF/cfg.TIME_UNIT,acc[i][j].D3,acc[i][j].D3_time);
        }
        fclose(fp);
    }
}