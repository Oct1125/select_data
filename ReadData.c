#include "select.h"

// 读取pos
void readdata(FILE *fp, int row, int num, pdata *pos)
{
    int i, j;
    double day, hour, min, sec;
    double DATATIME = 0.0;
    char buff[MAX] = {0};
    fgets(buff, MAX, fp);
    day = str2num(buff, 8, 2);
    hour = str2num(buff, 11, 2);
    min = str2num(buff, 14, 2);
    sec = str2num(buff, 17, 4);
    DATATIME = day * 86400 + hour * 3600 + min * 60 + sec;
    rewind(fp);
    for (i = 0; i < (row / num); i++)
    {
        for (j = 0; j < num; j++)
        {
            fgets(buff, MAX, fp);
            pos[j][i].falg = *(buff + 21);
            setstr(pos[j][i].name, buff + 23, 4);
            pos[j][i].day = str2num(buff, 8, 2);
            pos[j][i].hour = str2num(buff, 11, 2);
            pos[j][i].min = str2num(buff, 14, 2);
            pos[j][i].sec = str2num(buff, 17, 4);
            pos[j][i].X = str2num(buff, 28, 12);
            pos[j][i].Y = str2num(buff, 41, 12);
            pos[j][i].Z = str2num(buff, 54, 12);
            pos[j][i].dx = str2num(buff, 69, 6)*100;
            pos[j][i].dy = str2num(buff, 78, 6)*100;
            pos[j][i].dz = str2num(buff, 87, 6)*100;
            pos[j][i].E = str2num(buff, 98, 9);
            pos[j][i].N = str2num(buff, 110, 9);
            pos[j][i].U = str2num(buff, 123, 9);
            pos[j][i].num = str2num(buff, 136, 2);
            pos[j][i].D = (pos[j][i].dx + pos[j][i].dy + pos[j][i].dz) / 3.0;
            pos[j][i].time = pos[j][i].day * 86400 + pos[j][i].hour * 3600 + pos[j][i].min * 60 + pos[j][i].sec - DATATIME;
        }
    }
}

// 读取dop文件
void readdop(FILE *fp_dop, int row, int num, pdata *pos)
{
    int i = 0;
    int j = 0;
    char buff[MAX] = {0};
    for (i = 0; i < (row / num); i++)
    {
        for (j = 0; j < num; j++)
        {
            if (fabs(pos[j][i].E - 0) < 0.001)
                continue;
            fgets(buff, MAX, fp_dop);
            pos[j][i].PDOP = str2num(buff, 28, 5);
            pos[j][i].HDOP = str2num(buff, 35, 5);
            pos[j][i].VDOP = str2num(buff, 42, 5);
        }
    }
}