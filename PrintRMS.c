#include "select.h"

void PrintRMS(const CFG cfg,pdata* pos,pRMS* rms,int k,int num)
{
    int i=0,j=0,m=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    //平均卫星数
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"RMS_",4);
    m=strlen(tmp);
     for(i=0;i<num;i++)
    {   
        setstr(tmp+m,pos[i][0].name,4);
        fp=fopen(tmp,"w");
        for(j=0;j<cfg.CONACC_TIME/cfg.CONACC_CONTIME;j++)
        {
            //输出单个测站数据
            fprintf(fp,"%15.6lf%15.6lf%15.6lf\n",rms[i][j].E,rms[i][j].N,rms[i][j].U);
        }
        fclose(fp);
    }
}
