#include "select.h"

//打印可见卫星数，PDOP，测站经纬度（GMT）

//可见卫星数和PDOP
void PrintSatNum(const CFG cfg,pdata* pos,pave_con_acc ave_con,pSatofNum sat_num,pBLH pos_BLH,int k,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    //平均卫星数
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_aveNum",10);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(ave_con[i].ave_num==0)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,ave_con[i].ave_num);
    }
    fclose(fp);
    //最大卫星数
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_maxNum",10);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(sat_num[i].max_num==0)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,sat_num[i].max_num);
    }
    fclose(fp);
    //最小卫星数
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_minNum",10);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(sat_num[i].min_num==0)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,sat_num[i].min_num);
    }
    fclose(fp);
    //平均pdop
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_avepdop",11);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(ave_con[i].pdop==0)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,ave_con[i].pdop);
    }
    fclose(fp);
    //max pdop
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_maxpdop",11);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(sat_num[i].max_pdop==0||sat_num[i].max_pdop>15)continue;
        //if(sat_num[i].max_pdop==0||sat_num[i].max_pdop>15)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,sat_num[i].max_pdop);
    }
    fclose(fp);
    //min pdop
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_minpdop",11);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        if(sat_num[i].min_pdop==0||sat_num[i].min_pdop<0.5)continue;
        fprintf(fp,"%10.3lf%10.3lf%10.3lf\n",pos_BLH[i].L*180/pi,pos_BLH[i].B*180/pi,sat_num[i].min_pdop);
    }
    fclose(fp);
}


//打印测站经纬度及名称，TC表示GMT中的锚点
void PrintBLH(const CFG cfg,pdata* pos,int k,int row,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"pos_sit",7);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        BLH sit=XYZtoLB(pos[i][0].X,pos[i][0].Y,pos[i][0].Z);
        fprintf(fp,"%10.3lf%10.3lf   TC   %4s\n",sit.L*180/pi,sit.B*180/pi,pos[i][0].name);
    }
    fclose(fp);
}

//打印测站平均精度(可用于GMT分布图)
void PrintGmtAveCon(const CFG cfg,pdata* pos,pave_con_acc ave_con,pcon_acc* acc,int k,int num)
{
    int i=0,j=0;
    char tmp[MAX]={0};
    FILE* fp=NULL;
    setstr(tmp,cfg.save_address,k);
    //输出ENU平均精度
    //E
    setstr(tmp+k,"gmt_ave_posE",12);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        BLH sit=XYZtoLB(pos[i][0].X,pos[i][0].Y,pos[i][0].Z);
        fprintf(fp,"%15.6lf%15.6lf%15.6lf\n",sit.L*180/pi,sit.B*180/pi,ave_con[i].ave_E);
    }
    fclose(fp);
    fp=NULL;
    //N
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"gmt_ave_posN",12);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        BLH sit=XYZtoLB(pos[i][0].X,pos[i][0].Y,pos[i][0].Z);
        fprintf(fp,"%15.6lf%15.6lf%15.6lf\n",sit.L*180/pi,sit.B*180/pi,ave_con[i].ave_N);
    }
    fclose(fp);
    fp=NULL;
    //U
    memset(tmp,0,sizeof(tmp));
    setstr(tmp,cfg.save_address,k);
    setstr(tmp+k,"gmt_ave_posU",12);
    fp=fopen(tmp,"w");
    for(i=0;i<num;i++)
    {
        BLH sit=XYZtoLB(pos[i][0].X,pos[i][0].Y,pos[i][0].Z);
        fprintf(fp,"%15.6lf%15.6lf%15.6lf\n",sit.L*180/pi,sit.B*180/pi,ave_con[i].ave_U);
    }
    fclose(fp);
}