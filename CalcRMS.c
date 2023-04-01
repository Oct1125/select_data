#include "select.h"

// 统计各测站收敛精度的RMS值
/*void CalcRMS(const CFG cfg,pdata* pos,pRMS* rms,pcon_acc* acc,int num)
{
    int i,j,k,m,n1,n2,n3;
    for(i=0;i<num;i++)
    {
        rms[i]=(pRMS)malloc(sizeof(RMS)*cfg.CONACC_TIME/cfg.CONACC_CONTIME);
        memset(rms[i],0,sizeof(RMS)*cfg.CONACC_TIME/cfg.CONACC_CONTIME);
    }
    m=cfg.CONACC_CONTIME*cfg.CONACC_EPOCH;
    for(i=0;i<num;i++)
    {
        for(j=0;j<cfg.CONACC_TIME/cfg.CONACC_CONTIME;j++)
        {
            for(k=j*m;k<(j+1)*m;k++)
            {
                    rms[i][j].E+=pow(pos[i][k].E,2);
                    rms[i][j].N+=pow(pos[i][k].N,2);
                    rms[i][j].U+=pow(pos[i][k].U,2);
            }
            rms[i][j].E=sqrt(rms[i][j].E/(m));
            rms[i][j].N=sqrt(rms[i][j].N/(m));
            rms[i][j].U=sqrt(rms[i][j].U/(m));
        }
    }
}*/

// 统计各测站收敛精度的RMS值
void CalcRMS(const CFG cfg, pdata *pos, pRMS *rms, pcon_acc *acc, int num)
{
    int i, j, k, m, n1, n11, n2, n22, n3, n33;
    for (i = 0; i < num; i++)
    {
        rms[i] = (pRMS)malloc(sizeof(RMS) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
        memset(rms[i], 0, sizeof(RMS) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
    }
    m = cfg.CONACC_CONTIME * cfg.CONACC_EPOCH; // 历元数
    for (i = 0; i < num; i++)
    {
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            n1 = n11 = acc[i][j].time_E / (cfg.TIME_INTERVAL * 60);
            n2 = n22 = acc[i][j].time_N / (cfg.TIME_INTERVAL * 60);
            n3 = n33 = acc[i][j].time_U / (cfg.TIME_INTERVAL * 60);
            for (k = j * m; k < (j + 1) * m; k++)
            {
                if (n1 <= 0 && fabs(pos[i][k].E - 0) > 0.001)
                {
                    rms[i][j].E += pow(pos[i][k].E, 2);
                }
                else if (fabs(pos[i][k].E - 0) < 0.001)
                    n11++;
                if (n2 <= 0 && fabs(pos[i][k].E - 0) > 0.001)
                {
                    rms[i][j].N += pow(pos[i][k].N, 2);
                }
                else if (fabs(pos[i][k].N - 0) < 0.001)
                    n22++;
                if (n3 <= 0 && fabs(pos[i][k].U - 0) > 0.001)
                {
                    rms[i][j].U += pow(pos[i][k].U, 2);
                }
                else if (fabs(pos[i][k].U - 0) < 0.001)
                    n33++;
                n1--;
                n2--;
                n3--;
            }
            rms[i][j].E = sqrt(rms[i][j].E / (m - n11));
            rms[i][j].N = sqrt(rms[i][j].N / (m - n11));
            rms[i][j].U = sqrt(rms[i][j].U / (m - n11));
        }
    }
}
