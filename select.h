#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.141592653589793238                          
#define MAX 150             //字符串最大值

//CFG文件结构体
typedef struct CFG
{
    char data_address[MAX]; //原始pos数据地址
    char dop_address[MAX];  //原始dop数据地址
    char save_address[MAX]; //保存数据地址
    char FILE_NAME[MAX];    //数据文件名
    char DOP_NAME[MAX];     //DOP文件名
    char AMB_NAME[MAX];     //AMB文件名
    double TIME_INTERVAL;      //历元间隔(min)
    int CONACC_CONTIME;     //收敛周期（h）
    int CONACC_EPOCH;       //间隔历元数                    
    int CONACC_START;       //起始时刻（h）                      
    int CONACC_TIME;        //一天中有多少小时参与计算（h）          
    int CONACC_E;           //E方向收敛精度标准（cm） 
    int CONACC_N;           //N方向收敛精度标准（cm）        
    int CONACC_U;           //U方向收敛精度标准（cm）
    int CONACC_D;           //三维方向收敛（cm）
    int CONACC_Num;
    int CONACC_timeN;       //0表示记录n的时间，3表示记录0～n+2的时间总和       
    int CONACC_dataN;       //2表示记录0～n+2的平均精度
    int ACC_UNIT;           //原始数据单位为cm，100表示各测站数据输出以m为单位
    int TIME_UNIT;          //原始数据单位为s，60表示收敛时间以min为单位
    int ACC_UNIT_ALL;       //总测站
    int TIME_UNIT_ALL;
    int TIME_PER_INTERVAL;  //收敛间隔统计
    int TIME_UNIT_ACC_PER; 
    double ACC_PER_INTERVAL;   //收敛精度统计
    double ACC_PER_ALL;
    int ACC_UNIT_ACC_PER;
    double FIX_PER_INTERVAL;
    int FIX_PER_ST;
    int FIX_PER_EN;

} CFG, *pCFG;

//存储数据结构体
typedef struct data
{
    char falg;
    char name[5];
    double day;
    double hour;
    double min;
    double sec;
    double time;
    double X;
    double Y;
    double Z;
    double dx;
    double dz;
    double dy;
    double D;
    double E;
    double N;
    double U;
    double PDOP;
    double HDOP;
    double VDOP;
    double num; //卫星数
    int amb;

}data,*pdata;

//用于经纬度函数传参
typedef struct BLH
{
	double B;//维度
	double L;//经度
	double H;//高
}BLH,*pBLH;

//精度收敛结果结构体
typedef struct con_acc
{
    double B;
    double L;
    double H;
    double acc_E;//E方向收敛精度
    double acc_N;
    double acc_U;
    double D3;
    double time_E;//E方向收敛时间
    double time_N;
    double time_U;
    double D3_time;
    double TTFF;
    double Fix_Rate;
}con_acc,*pcon_acc;

typedef struct ave_con_acc
{
    double ave_E;//平均精度
    double ave_N;
    double ave_U;
    double ave_num;
    double pdop;
} ave_con_acc, *pave_con_acc;

//卫星数统计
typedef struct SatofNum
{
    double min_num;
    double max_num;
    double min_pdop;
    double max_pdop;
}SatofNum,*pSatofNum;

//收敛时间百分比计算
typedef struct ConPercent_t
{
    double time;
    double percent_time;
    double num_time;
}ConPercent_t,*pConPercent_t;

//收敛精度统计
typedef struct ConPercent_a
{
    double acc;
    double percent_acc;
    double num_acc;
}ConPercent_a,*pConPercent_a;

//固定率统计
typedef struct ConPercent_f
{
    double rate;
    double percent_rate;
    double num_rete;
}ConPercent_f,*pConPercent_f;

//记录百分比
typedef struct Conper
{
    double countE;
    double countN;
    double countU;
    double countD;
    double countTTFF;
    double countFixRate;
    double countE_all;
    double countN_all;
    double countU_all;
    double countD_all;
    double countTTFF_all;
    double countFixRate_all;
}Conper,*pConper;



//RMS值
typedef struct RMS
{
    double E;
    double N;
    double U;
}RMS,*pRMS;



/*------------------------------------------------函数部分----------------------------------------------------*/

int getrow(FILE* fp);//获取行数
extern int getnum(FILE* fp);//获取卫星数
extern double str2num(const char* buff, int i, int n);//字符串转浮点数
extern void setstr(char* des, const char* src, int n);//实现strncpy补全‘\0’
extern BLH XYZtoLB(double X, double Y, double Z);//xyz转经纬度

/*------------------------------------------------读取部分----------------------------------------------------*/

CFG readCFG(char* cfgname);//read CFG
extern void readdata(FILE*fp,int row,int num,pdata* pos);//读取数据
extern void readdop(FILE*fp_dop,int row,int num,pdata* pos);//读取dop数据

/*------------------------------------------------计算部分----------------------------------------------------*/

//从每颗卫星开始，分别判断ENU三个方向的收敛时间、精度
extern void ConAcc(const CFG cfg,pdata* pos,pcon_acc* acc,int k,int row,int num);
//统计各测站收敛精度的RMS值
extern void CalcRMS(const CFG cfg,pdata* pos,pRMS* rms,pcon_acc* acc,int num);
//收敛时间、精度统计（全部测站平均）
extern void AveConverAcc(const CFG cfg,pcon_acc acc_con_all,pcon_acc* acc,int k,int row,int num);
//计算各测站平均精度(可用于GMT分布图)
extern void GmtAveCon(const CFG cfg,pdata* pos,pave_con_acc ave_con,pcon_acc* acc,pSatofNum sat_num,pBLH pos_BLH,int row,int num);


/*------------------------------------------------打印部分----------------------------------------------------*/
//输出函数
extern void PrintData(const CFG cfg,pdata* pos,int k,int row,int num);
//打印测站经纬度及名称，TC表示GMT中的锚点
extern void PrintBLH(const CFG cfg,pdata* pos,int k,int row,int num);
//打印测站收敛精度、时间
extern void PrintConAcc(const CFG cfg,pdata* pos,pcon_acc* acc,int k,int row,int num);
//打印测站平均收敛精度、时间（全部测站平均）
extern void PrintConAccALL(const CFG cfg,pcon_acc acc_con_all,int k,int row,int num);
//打印测站平均收敛精度、时间（全部测站平均，绝对值）
extern void PrintConAccALL_fab(const CFG cfg,pcon_acc acc_con_all,int k,int row,int num);
//打印测站平均精度(可用于GMT分布图)
extern void PrintGmtAveCon(const CFG cfg,pdata* pos,pave_con_acc ave_con,pcon_acc* acc,int k,int num);
//打印可见卫星数（GMT）
extern void PrintSatNum(const CFG cfg,pdata* pos,pave_con_acc ave_con,pSatofNum sat_num,pBLH pos_BLH,int k,int num);
//打印测站收敛时间百分比
extern void PrintConPercent(const CFG cfg,pdata* pos, pcon_acc* acc,pConPercent_t* conper,pConPercent_a* accper,int n,int row,int num,pConper conper_time,pConper conper_acc);
//打印RMS
extern void PrintRMS(const CFG cfg,pdata* pos,pRMS* rms,int k,int num);
