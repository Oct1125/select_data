#include "select.h"

int main()
{
    int i = 0, j = 0, m = 0, l = 0, k = 0;
    int row = 0; // 行数
    int num = 0; // 测站数
    /*--------------------------------------read cfg--------------------------------------*/
    char cfgname[] = "ReadData.cfg";
    CFG cfg = readCFG(cfgname);
    l = (int)strlen(cfg.data_address);
    k = (int)strlen(cfg.save_address);
    strcpy(cfg.dop_address, cfg.data_address);
    setstr(cfg.data_address + l, cfg.FILE_NAME, strlen(cfg.FILE_NAME));
    setstr(cfg.dop_address + l, cfg.DOP_NAME, strlen(cfg.DOP_NAME));

    /*------------------------------------------------------------------------------------*/

    FILE *fp = fopen(cfg.data_address, "r");    // pos数据文件指针
    FILE *fp_dop = fopen(cfg.dop_address, "r"); // dop数据文件指针

    /*-------------------------------------获取文件信息-------------------------------------*/

    // 获取文件总行数
    row = getrow(fp);
    rewind(fp);
    // 获取测站数
    num = getnum(fp);
    rewind(fp);

    /*--------------------------------------开辟内存---------------------------------------*/

    // 测站
    pdata *pos = (pdata *)malloc(sizeof(pdata) * num);
    if (pos != NULL)
    {
        for (i = 0; i < num; i++)
        {
            pos[i] = (pdata)malloc(sizeof(data) * row / num);
            memset(pos[i], 0, sizeof(data) * row / num);
        }
    }
    // 时间
    double **time = (double **)malloc(sizeof(double *) * num);
    for (i = 0; i < num; i++)
    {
        time[i] = (double *)malloc(sizeof(double) * row / num);
        memset(time[i], 0, sizeof(double) * row / num);
    }
    // 收敛结果
    pcon_acc *acc = (pcon_acc *)malloc(sizeof(pcon_acc) * num);
    for (i = 0; i < num; i++)
    {
        acc[i] = (pcon_acc)malloc(sizeof(con_acc) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
        memset(acc[i], 0, sizeof(con_acc) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
        // memset(acc[i],0,_msize(acc[i])); //另一种计算内存的方法，MAC中lldb不支持
    }
    // 平均收敛结果
    pcon_acc acc_con_all = (pcon_acc)malloc(sizeof(con_acc) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
    memset(acc_con_all, 0, sizeof(con_acc) * cfg.CONACC_TIME / cfg.CONACC_CONTIME);
    // 测站平均精度结果
    pave_con_acc ave_con = (pave_con_acc)malloc(sizeof(ave_con_acc) * num);
    memset(ave_con, 0, sizeof(ave_con_acc) * num);
    // 最大与最小卫星数
    pSatofNum sat_num = (pSatofNum)malloc(sizeof(SatofNum) * num);
    memset(sat_num, 0, sizeof(SatofNum) * num);
    // 测站经纬度
    pBLH pos_BLH = (pBLH)malloc(sizeof(BLH) * num);
    memset(pos_BLH, 0, sizeof(BLH) * num);
    // RMS值
    pRMS *rms = (pRMS *)malloc(sizeof(pRMS) * num);
    // 收敛百分比
    pConPercent_t *conper_t = (pConPercent_t *)malloc(sizeof(pConPercent_t) * 5);
    for (i = 0; i < 5; i++)
    {
        conper_t[i] = (pConPercent_t)malloc(sizeof(ConPercent_t) * 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL);
        memset(conper_t[i], 0, sizeof(ConPercent_t) * 3600 * cfg.CONACC_CONTIME / cfg.TIME_PER_INTERVAL);
    }

    pConPercent_a *conper_a = (pConPercent_a *)malloc(sizeof(pConPercent_a) * 4);
    for (i = 0; i < 4; i++)
    {
        conper_a[i] = (pConPercent_a)malloc(sizeof(ConPercent_a) * cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL);
        memset(conper_a[i], 0, sizeof(ConPercent_a) * cfg.ACC_PER_ALL / cfg.ACC_PER_INTERVAL);
    }

    pConper conper_time = (pConper)malloc(sizeof(Conper));
    memset(conper_time, 0, sizeof(Conper));

    pConper conper_acc = (pConper)malloc(sizeof(Conper));
    memset(conper_acc, 0, sizeof(Conper));

    /*---------------------------------------计算数据---------------------------------------*/
    // 读取数据
    readdata(fp, row, num, pos);
    fclose(fp);
    readdop(fp_dop, row, num, pos);
    fclose(fp_dop);

    // 从每颗卫星开始，分别判断ENU三个方向的收敛时间、精度(各测站)
    ConAcc(cfg, pos, acc, k, row, num);
    // 收敛时间、精度统计（统计全部测站平均）
    AveConverAcc(cfg, acc_con_all, acc, k, row, num);
    // 计算各测站平均精度(可用于GMT分布图)
    GmtAveCon(cfg, pos, ave_con, acc, sat_num, pos_BLH, row, num);
    // 计算RMS
    CalcRMS(cfg, pos, rms, acc, num);

    /*---------------------------------------输出数据---------------------------------------*/
    printf("-----------------------Begin-----------------------\n");
    printf("原始数据地址:>\n%s\n", cfg.data_address);
    printf("数据保存地址:>\n%s\n", cfg.save_address);
    printf("---------------------------------------------------\n");
    printf("总行数：%d\n", row);
    printf("测站数：%d\n", num);
    // 输出测站原始数据
    PrintData(cfg, pos, k, row, num);
    printf("---------------------------------------------------\n");
    printf("输出各测站原始数据:>\n");
    printf("文件类型:> xxxx (测站名)\n");
    printf("输出结构:> E N U 时间 卫星数 PDOP HDOP VDOP 固定标志\n");
    // 参与计算的测站位置(经纬度)
    printf("---------------------------------------------------\n");
    PrintBLH(cfg, pos, k, row, num);
    printf("输出各测站经纬度数据:>\n");
    printf("文件类型:> pos_sit\n");
    printf("输出结构:> 经度 纬度 锚点 测站名\n");
    // 打印测站收敛精度、时间
    printf("---------------------------------------------------\n");
    PrintConAcc(cfg, pos, acc, k, row, num);
    printf("输出各测站收敛数据:>\n");
    printf("文件类型:> con_XXXX (测站名)\n");
    printf("输出结构:> E N U E_time N_time U_time FFTT\n");
    // 打印测站平均收敛精度、时间（全部测站平均）
    printf("---------------------------------------------------\n");
    PrintConAccALL(cfg, acc_con_all, k, row, num);
    printf("输出全部测站收敛平均数据:>\n");
    printf("文件类型:> con_all\n");
    printf("输出结构:> E N U E_time N_time U_time\n");
    // 打印测站平均收敛精度、时间（全部测站平均，绝对值）
    printf("---------------------------------------------------\n");
    PrintConAccALL_fab(cfg, acc_con_all, k, row, num);
    printf("输出全部测站收敛平均数据(绝对值):>\n");
    printf("文件类型:> con_all_fab\n");
    printf("输出结构:> E N U E_time N_time U_time\n");
    // 计算并打印测站平均精度(可用于GMT分布图)
    printf("---------------------------------------------------\n");
    PrintGmtAveCon(cfg, pos, ave_con, acc, k, num);
    printf("输出各测站平均精度(GMT绘图):>\n");
    printf("文件类型:> gmt_ave_posE/N/U\n");
    printf("输出结构:> 经度 纬度 精度\n");
    // 打印可见卫星数（GMT）
    printf("---------------------------------------------------\n");
    PrintSatNum(cfg, pos, ave_con, sat_num, pos_BLH, k, num);
    printf("输出各测站可见卫星数(GMT绘图):>\n");
    printf("文件类型:> pos_SatNum\n");
    printf("输出结构:> 经度 纬度 可见卫星数\n");
    // 打印收敛时间百分比统计
    printf("---------------------------------------------------\n");
    PrintConPercent(cfg, pos, acc, conper_t, conper_a, k, row, num, conper_time, conper_acc);
    printf("输出所有测站收敛时间统计百分比:>\n");
    printf("文件类型:> con_per_E/N/U\n");
    printf("输出结构:> 时间 百分比 数量\n");
    // RMS值
    printf("---------------------------------------------------\n");
    PrintRMS(cfg, pos, rms, k, num);
    printf("输出所有测站收敛时间统计百分比:>\n");
    printf("文件类型:> RMS_XXXX\n");
    printf("输出结构:> E N U\n");

    /*-------------------------------------------------------------------------------------*/

    FILE *tmp = fopen("/Users/wjj/POS_XYZ", "w");
    for (i = 0; i < num; i++)
    {
        fprintf(tmp, "%20.6lf%20.6lf%20.6lf%10s\n", pos[i][0].X, pos[i][0].Y, pos[i][0].Z, pos[i][0].name);
    }
    fclose(tmp);
    free(pos), free(time), free(acc), free(acc_con_all), free(ave_con), free(sat_num), free(pos_BLH), free(rms), free(conper_t);
    free(conper_time), free(conper_acc);
    return 0;
}