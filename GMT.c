#include "select.h"


//计算各测站平均精度(可用于GMT分布图)
void GmtAveCon(const CFG cfg,pdata* pos,pave_con_acc ave_con,pcon_acc* acc,pSatofNum sat_num,pBLH pos_BLH,int row,int num)
{
    int i=0,j=0;
    double max=0,min=100;
    char tmp[MAX]={0};
    BLH tmp2={0};
    for(i=0;i<num;i++)
    {
        for(j=0;j<cfg.CONACC_TIME/cfg.CONACC_CONTIME;j++)
        {
            ave_con[i].ave_E+=acc[i][j].acc_E;
            ave_con[i].ave_N+=acc[i][j].acc_N;
            ave_con[i].ave_U+=acc[i][j].acc_U;
        }
        ave_con[i].ave_E/=cfg.CONACC_TIME;
        ave_con[i].ave_N/=cfg.CONACC_TIME;
        ave_con[i].ave_U/=cfg.CONACC_TIME;
    }
     for(i=0;i<num;i++)
    {
        max=0;
        min=100;
        for(j=0;j<row/num;j++)
        {
            ave_con[i].ave_num+=pos[i][j].num;
            if(pos[i][j].num>max)
            {
                max=pos[i][j].num;
            }
            if(pos[i][j].num<min)
            {
                min=pos[i][j].num;
            }
        }
        ave_con[i].ave_num/=(row/num);
        sat_num[i].max_num=max;
        sat_num[i].min_num=min;
    }
    for(i=0;i<num;i++)
    {
        max=0;
        min=100;
        for(j=0;j<row/num;j++)
        {
            ave_con[i].pdop+=pos[i][j].PDOP;
            if(pos[i][j].PDOP>max)
            {
                max=pos[i][j].PDOP;
            }
            if(pos[i][j].PDOP<min)
            {
                min=pos[i][j].PDOP;
            }
        }
        ave_con[i].pdop/=(row/num);
        sat_num[i].max_pdop=max;
        sat_num[i].min_pdop=min;
    }
    for(i=0;i<num;i++)
    {
        tmp2=XYZtoLB(pos[i][0].X,pos[i][0].Y,pos[i][0].Z);
        pos_BLH[i].B=tmp2.B;
        pos_BLH[i].L=tmp2.L;
        pos_BLH[i].H=tmp2.H;
    }
}





    
    