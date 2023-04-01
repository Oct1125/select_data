#include "select.h"

// 从每颗卫星开始，分别判断ENU三个方向的收敛时间、精度
void ConAcc(const CFG cfg, pdata *pos, pcon_acc *acc, int k, int row, int num)
{
    int i = 0, j = 0, n = 0, m = 0, p, flag;
    double E_num, N_num, U_num, D_num;
    double count, count2, sum = 0;
    for (i = 0; i < num; i++)
    {
        // E
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH - cfg.CONACC_Num; n++)
            {
                flag = 0;
                for (p = n; p < n + cfg.CONACC_Num; p++)
                {
                    if (pos[i][p].E < cfg.CONACC_E && fabs(pos[i][p].E - 0) > 0.001)
                        flag++;
                }
                if (flag == cfg.CONACC_Num)
                {
                    acc[i][j].time_E = (pos[i][n].hour - cfg.CONACC_CONTIME * j - cfg.CONACC_START) * 3600 +
                                       pos[i][n + cfg.CONACC_timeN].min * 60 + pos[i][n + cfg.CONACC_timeN].sec;
                    break;
                }
            }
        }
        // N
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH - cfg.CONACC_Num; n++)
            {
                flag = 0;
                for (p = n; p < n + cfg.CONACC_Num; p++)
                {
                    if (pos[i][p].N < cfg.CONACC_N && fabs(pos[i][p].N - 0) > 0.001)
                        flag++;
                }
                if (flag == cfg.CONACC_Num)
                {
                    acc[i][j].time_N = (pos[i][n].hour - cfg.CONACC_CONTIME * j - cfg.CONACC_START) * 3600 +
                                       pos[i][n + cfg.CONACC_timeN].min * 60 + pos[i][n + cfg.CONACC_timeN].sec;
                    break;
                }
            }
        }
        // U
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH - cfg.CONACC_Num; n++)
            {
                flag = 0;
                for (p = n; p < n + cfg.CONACC_Num; p++)
                {
                    if (pos[i][p].U < cfg.CONACC_U && fabs(pos[i][p].U - 0) > 0.001)
                        flag++;
                }
                if (flag == cfg.CONACC_Num)
                {
                    acc[i][j].time_U = (pos[i][n + cfg.CONACC_timeN].hour - cfg.CONACC_CONTIME * j - cfg.CONACC_START) * 3600 +
                                       pos[i][n + cfg.CONACC_timeN].min * 60 + pos[i][n + cfg.CONACC_timeN].sec;
                    break;
                }
            }
        }
        // D3
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH - cfg.CONACC_Num; n++)
            {
                flag = 0;
                for (p = n; p < n + cfg.CONACC_Num; p++)
                {
                    if (pos[i][p].dx < cfg.CONACC_D && pos[i][p].dy < cfg.CONACC_D &&
                        pos[i][p].dz < cfg.CONACC_D && fabs(pos[i][p].dx * pos[i][p].dy * pos[i][p].dz) > 1.0e-10)
                        flag++;
                }
                if (flag == cfg.CONACC_Num)
                {
                    acc[i][j].D3_time = (pos[i][n + cfg.CONACC_timeN].hour - cfg.CONACC_CONTIME * j - cfg.CONACC_START) * 3600 +
                                        pos[i][n + cfg.CONACC_timeN].min * 60 + pos[i][n + cfg.CONACC_timeN].sec;
                    break;
                }
            }
        }
    }
    // ACC
    for (i = 0; i < num; i++)
    {
        // E
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            E_num = N_num = U_num = D_num = 0;
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH - cfg.CONACC_Num; n++)
            {
                if (fabs(pos[i][n].E) < 0.0001)
                {
                    E_num++;
                }
                else
                {
                    acc[i][j].acc_E += pow((pos[i][n].E), 2);
                }
                if (fabs(pos[i][n].N) < 0.0001)
                {
                    N_num++;
                }
                else
                {
                    acc[i][j].acc_N += pow((pos[i][n].N), 2);
                }
                if (fabs(pos[i][n].U) < 0.0001)
                {
                    U_num++;
                }
                else
                {
                    acc[i][j].acc_U += pow((pos[i][n].U), 2);
                }
                if (fabs(pos[i][n].D) < 0.0001)
                {
                    D_num++;
                }
                else
                {
                    acc[i][j].D3 += pow((pos[i][n].D), 2);
                }
            }
            count2 = num * cfg.CONACC_TIME / cfg.CONACC_CONTIME;
            acc[i][j].acc_E = sqrt(acc[i][j].acc_E / (count2 - E_num));
            acc[i][j].acc_N = sqrt(acc[i][j].acc_N / (count2 - N_num));
            acc[i][j].acc_U = sqrt(acc[i][j].acc_U / (count2 - U_num));
            acc[i][j].D3 = sqrt(acc[i][j].D3 / (count2 - D_num));
        }
    }
    for (i = 0; i < num; i++)
    {
        // FFTT
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH; n++)
            {
                if (pos[i][n].falg == 'x')
                {
                    acc[i][j].TTFF = (pos[i][n].hour - cfg.CONACC_CONTIME * j - cfg.CONACC_START) * 3600 +
                                     pos[i][n + cfg.CONACC_timeN].min * 60 + pos[i][n + cfg.CONACC_timeN].sec;
                    break;
                }
            }
        }
    }
    for (i = 0; i < num; i++)
    {
        // FFTT
        for (j = 0; j < cfg.CONACC_TIME / cfg.CONACC_CONTIME; j++)
        {
            count = 0.0;
            count2 = 0;
            flag = 0;
            for (n = j * cfg.CONACC_EPOCH; n < j * cfg.CONACC_EPOCH + cfg.CONACC_EPOCH; n++)
            {
                if (pos[i][n].falg == 'x')
                {
                    count = count + 1;
                    if (flag == 0)
                    {
                        flag = 1;
                    }
                }
                else
                {
                    if (flag == 0)
                        count2 += 1;
                }
            }
            if (count == 0)
                acc[i][j].Fix_Rate = 0;
            else
                acc[i][j].Fix_Rate = count / (cfg.CONACC_EPOCH - count2);
            sum += acc[i][j].Fix_Rate;
        }
    }
    double a = sum / (num * cfg.CONACC_TIME / cfg.CONACC_CONTIME) * 100;
    printf("平均固定率：%.2lf\n", a);
}
