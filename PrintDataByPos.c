#include "select.h"

// 打印各测站原始数据
void PrintData(const CFG cfg, pdata *pos, int k, int row, int num)
{
    int i = 0, j = 0;
    char tmp[MAX] = {0};
    FILE *fp = NULL;
    setstr(tmp, cfg.save_address, k);
    for (i = 0; i < num; i++)
    {
        setstr(tmp + k, pos[i][0].name, 4);
        fp = fopen(tmp, "w");
        for (j = 0; j < row / num; j++)
        {
            int falg = 0;
            // 输出单个测站数据
            if (pos[i][j].falg == 'x')
                falg = 1;
            fprintf(fp, "%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%15.6lf%5d\n",
                    pos[i][j].time / cfg.TIME_UNIT,
                    pos[i][j].E / cfg.ACC_UNIT, pos[i][j].N / cfg.ACC_UNIT, pos[i][j].U / cfg.ACC_UNIT,
                    pos[i][j].dx, pos[i][j].dy, pos[i][j].dz,
                    pos[i][j].num,
                    pos[i][j].PDOP, pos[i][j].HDOP, pos[i][j].VDOP,
                    falg);
        }
        fclose(fp);
    }
}