#include "select.h"

// 打印测站收敛时间百分比
void PrintConPercent(const CFG cfg, pdata *pos, pcon_acc *acc, pConPercent_t *conper, pConPercent_a *accper, int n, int row, int num, pConper conper_time, pConper conper_acc)
{
    int i = 0, j = 0, k = 0, m = 0;
    double mean = 0;
    double tmp = 0;
    double countFix_all;
    double count;
    conper_time->countE_all = conper_time->countN_all = conper_time->countU_all = conper_time->countD_all = conper_time->countTTFF_all = countFix_all = cfg.CONACC_TIME / cfg.CONACC_CONTIME * num;
    conper_acc->countE_all = conper_acc->countN_all = conper_acc->countU_all = conper_acc->countD_all = row;
    FILE *fp = NULL;
    pConPercent_f *conper_f = (pConPercent_f *)malloc(sizeof(pConPercent_f) * 1);
    conper_f[0] = (pConPercent_f)malloc(sizeof(ConPercent_f) * (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL);
    memset(conper_f[0], 0, sizeof(ConPercent_f) * (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL);

    for (i = 0; i < 5; i++)
    {
        for (j = 0, tmp = 0; j < 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL; j++)
        {
            conper[i][j].time = tmp;
            tmp += cfg.TIME_PER_INTERVAL;
        }
    }
    for (i = 0; i < 4; i++)
    {
        for (j = 0, tmp = 0; j < cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL; j++)
        {
            accper[i][j].acc = tmp;
            tmp += cfg.ACC_PER_INTERVAL;
        }
    }
    for (j = 0, tmp = cfg.FIX_PER_ST; j <= (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL; j++)
    {
        conper_f[0][j].rate = tmp;
        tmp += cfg.FIX_PER_INTERVAL;
    }
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < num; j++)
        {
            for (k = 0; k < cfg.CONACC_TIME / cfg.CONACC_CONTIME; k++)
            {
                for (m = 0; m < 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL; m++)
                {
                    // E
                    if (i == 0)
                    {
                        if (fabs(acc[j][k].time_E - 0) < 0.001)
                        {
                            // conper_time->countE_all--;
                            break;
                        }
                        else if (acc[j][k].time_E <= conper[i][m].time)
                        {
                            conper[i][m].num_time += 1;
                            conper_time->countE += 1;
                            break;
                        }
                    }
                    // N
                    else if (i == 1)
                    {
                        if (fabs(acc[j][k].time_N - 0) < 0.001)
                        {
                            // conper_time->countN_all--;
                            break;
                        }
                        else if (acc[j][k].time_N <= conper[i][m].time)
                        {
                            conper[i][m].num_time += 1;
                            conper_time->countN += 1;
                            break;
                        }
                    }
                    // U
                    else if (i == 2)
                    {
                        if (fabs(acc[j][k].time_U - 0) < 0.001)
                        {
                            // conper_time->countU_all--;
                            break;
                        }
                        else if (acc[j][k].time_U <= conper[i][m].time)
                        {
                            conper[i][m].num_time += 1;
                            conper_time->countU += 1;
                            break;
                        }
                    }
                    // D
                    else if (i == 3)
                    {
                        if (fabs(acc[j][k].D3_time - 0) < 0.001)
                        {
                            break;
                        }
                        else if (acc[j][k].D3_time <= conper[i][m].time)
                        {
                            conper[i][m].num_time += 1;
                            conper_time->countD += 1;
                            break;
                        }
                    }
                    // TTFF
                    else if (i == 4)
                    {
                        if (fabs(acc[j][k].TTFF - 0) < 0.001)
                        {
                            break;
                        }
                        else if (acc[j][k].TTFF <= conper[i][m].time)
                        {
                            conper[i][m].num_time += 1;
                            conper_time->countTTFF += 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < num; j++)
        {
            for (k = 0; k < row / num; k++)
            {
                for (m = 0; m < cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL; m++)
                {
                    if (i == 0)
                    {
                        if (fabs(pos[j][k].E - 0) < 0.001)
                        {
                            // conper_acc->countE_all--;
                            break;
                        }
                        else if (fabs(pos[j][k].E) < accper[i][m].acc)
                        {
                            accper[i][m].num_acc += 1;
                            conper_acc->countE += 1;
                            break;
                        }
                    }
                    else if (i == 1)
                    {
                        if (fabs(pos[j][k].N - 0) < 0.001)
                        {
                            // conper_acc->countN_all--;
                            break;
                        }
                        else if (fabs(pos[j][k].N) < accper[i][m].acc)
                        {
                            accper[i][m].num_acc += 1;
                            conper_acc->countN += 1;
                            break;
                        }
                    }
                    else if (i == 2)
                    {
                        if (fabs(pos[j][k].U - 0) < 0.001)
                        {
                            // conper_acc->countU_all--;
                            break;
                        }
                        else if (fabs(pos[j][k].U) < accper[i][m].acc)
                        {
                            accper[i][m].num_acc += 1;
                            conper_acc->countU += 1;
                            break;
                        }
                    }
                    else if (i == 3)
                    {
                        mean = (pos[i][k].dx + pos[i][k].dy + pos[i][k].dz) / 3.0;
                        if (fabs(pos[j][k].dx * pos[j][k].dy * pos[j][k].dz) < 0.001)
                        {
                            // conper_acc->countD_all--;
                            break;
                        }
                        else if (fabs(mean) < accper[i][m].acc)
                        {
                            accper[i][m].num_acc += 1;
                            conper_acc->countD += 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 5; i++)
    {
        // time
        for (j = 0; j < 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL; j++)
        {
            if (i == 0)
            {
                conper[i][j].percent_time = conper[i][j].num_time / conper_time->countE_all * 100;
            }
            else if (i == 1)
            {
                conper[i][j].percent_time = conper[i][j].num_time / conper_time->countN_all * 100;
            }
            else if (i == 2)
            {
                conper[i][j].percent_time = conper[i][j].num_time / conper_time->countU_all * 100;
            }
            else if (i == 3)
            {
                conper[i][j].percent_time = conper[i][j].num_time / conper_time->countD_all * 100;
            }
            else if (i == 4)
            {
                conper[i][j].percent_time = conper[i][j].num_time / conper_time->countTTFF_all * 100;
            }
        }
    }
    for (i = 0; i < 4; i++)
    {
        // acc
        for (j = 0; j < cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL; j++)
        {
            if (i == 0)
            {
                accper[i][j].percent_acc = accper[i][j].num_acc / conper_acc->countE_all * 100;
            }
            else if (i == 1)
            {
                accper[i][j].percent_acc = accper[i][j].num_acc / conper_acc->countN_all * 100;
            }
            else if (i == 2)
            {
                accper[i][j].percent_acc = accper[i][j].num_acc / conper_acc->countU_all * 100;
            }
            else if (i == 3)
            {
                accper[i][j].percent_acc = accper[i][j].num_acc / conper_acc->countD_all * 100;
            }
        }
    }
    // 输出
    // TIME
    for (i = 0; i < 5; i++)
    {
        char tmp[MAX] = {0};
        setstr(tmp, cfg.save_address, n);
        if (i == 0)
        {
            setstr(tmp + n, "conper_timeE", 12);
            fp = fopen(tmp, "w");
        }
        else if (i == 1)
        {
            setstr(tmp + n, "conper_timeN", 12);
            fp = fopen(tmp, "w");
        }
        else if (i == 2)
        {
            setstr(tmp + n, "conper_timeU", 12);
            fp = fopen(tmp, "w");
        }
        else if (i == 3)
        {
            setstr(tmp + n, "conper_timeD", 12);
            fp = fopen(tmp, "w");
        }
        else if (i == 4)
        {
            setstr(tmp + n, "conper_timeTTFF", 15);
            fp = fopen(tmp, "w");
        }
        for (j = 0; j < 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL; j++)
        {
            fprintf(fp, "%15.6lf%15.6lf%15.6lf\n", conper[i][j].time / cfg.TIME_UNIT_ACC_PER, conper[i][j].percent_time, conper[i][j].num_time);
        }
        fclose(fp);
    }
    // ACC
    for (i = 0; i < 4; i++)
    {
        char tmp[MAX] = {0};
        setstr(tmp, cfg.save_address, n);
        if (i == 0)
        {
            setstr(tmp + n, "conper_accE", 11);
            fp = fopen(tmp, "w");
        }
        else if (i == 1)
        {
            setstr(tmp + n, "conper_accN", 11);
            fp = fopen(tmp, "w");
        }
        else if (i == 2)
        {
            setstr(tmp + n, "conper_accU", 11);
            fp = fopen(tmp, "w");
        }
        else if (i == 3)
        {
            setstr(tmp + n, "conper_accD", 11);
            fp = fopen(tmp, "w");
        }
        for (j = 0; j < cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL; j++)
        {
            fprintf(fp, "%15.6lf%15.6lf%15.6lf\n", accper[i][j].acc, accper[i][j].percent_acc, accper[i][j].num_acc);
        }
        fclose(fp);
    }

    for (j = 0; j < num; j++)
    {
        for (k = 0; k < cfg.CONACC_TIME / cfg.CONACC_CONTIME; k++)
        {
            for (m = 0; m <= (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL; m++)
            {
                if (fabs(acc[j][k].Fix_Rate - 0) < 0.0001)
                    break;
                else if (acc[j][k].Fix_Rate * 100 <= conper_f[0][m].rate)
                {
                    count += 1;
                    conper_f[0][m].num_rete += 1;
                    break;
                }
            }
        }
    }
    for (j = 0; j <= (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL; j++)
    {
        conper_f[0][j].percent_rate = conper_f[0][j].num_rete / countFix_all * 100;
    }

    char arr[MAX] = {0};
    setstr(arr, cfg.save_address, n);
    setstr(arr + n, "conper_FixRate", 14);
    fp = fopen(arr, "w");
    for (j = 0; j <= (cfg.FIX_PER_EN - cfg.FIX_PER_ST) / cfg.FIX_PER_INTERVAL; j++)
    {
        fprintf(fp, "%15.6lf%15.6lf%15.6lf\n", conper_f[0][j].rate, conper_f[0][j].percent_rate, conper_f[0][j].num_rete);
    }
    fclose(fp);

    printf("--------------------TIME--------------------\n");
    printf("E方向收敛:>%10.3lf\n", conper_time->countE / conper_time->countE_all);
    printf("N方向收敛:>%10.3lf\n", conper_time->countN / conper_time->countN_all);
    printf("U方向收敛:>%10.3lf\n", conper_time->countU / conper_time->countU_all);
    printf("D方向收敛:>%10.3lf\n", conper_time->countD / conper_time->countD_all);
    printf("TTFF固定率:>%10.3lf\n", conper_time->countTTFF / conper_time->countTTFF_all);
    printf("---------------------ACC--------------------\n");
    printf("E方向收敛:>%10.3lf\n", conper_acc->countE / conper_acc->countE_all);
    printf("N方向收敛:>%10.3lf\n", conper_acc->countN / conper_acc->countN_all);
    printf("U方向收敛:>%10.3lf\n", conper_acc->countU / conper_acc->countU_all);
    printf("D方向收敛:>%10.3lf\n", conper_acc->countD / conper_acc->countD_all);
}
