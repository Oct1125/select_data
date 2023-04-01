#include "select.h"

/*---------------------------------------------*/
//                  空行跳过
//                  首字母‘#’跳过
/*---------------------------------------------*/
//字符串去掉尾巴
int pstr(char*src)
{
    while(*src)
    {
        if(*src==' ')
        {
            *src='\0';
            return 0;
        }
        src++;
    }
    return -1;
}
//字符串拷贝，考虑前后有空格的情况
int strcopy(const char*src,char*dest,char* lable)
{
    while(*src)
    {
        if(*src=='#')return 0;
        else if(*src==' ')
        {
            src++;
            continue;
        }
        else if(*src!=':')
        {
            *lable++ = *src++;
            continue;
        }
        else if(*src==':')
        {
            src++;
            while(*src)
            {
                if(*src==' ')
                {
                    src++;
                    continue;
                }
                if(*src=='#')return 0;
                if(*src=='\n')return 0;
                *dest++ = *src++;
            }
            return 0;
        }
        src++;
    }
    return -1;
}

void my_strcpy(const char*src,char*dest,char*lable)
{
    memset(lable,0,MAX);
    memset(dest,0,MAX);
    strcopy(src,dest,lable);
    pstr(dest);
}

//读取
CFG readCFG(char* cfgname)
{
    CFG cfg={0};
    FILE*fp=NULL;
    fp=fopen(cfgname,"r");
    char buff[MAX]={0};
    char tmp[MAX]={0};
    char lable[MAX]={0};
    while(fgets(buff,MAX,fp))
    {
        my_strcpy(buff,tmp,lable);
        if(strcmp(lable,"data_address")==0)
        {
            strcpy(cfg.data_address,tmp);
        }
        else if(strcmp(lable,"save_address")==0)
        {
            strcpy(cfg.save_address,tmp);
        }
         else if(strcmp(lable,"FILE_NAME")==0)
        {
            strcpy(cfg.FILE_NAME,tmp);
        }
         else if(strcmp(lable,"DOP_NAME")==0)
        {
            strcpy(cfg.DOP_NAME,tmp);
        }
         else if(strcmp(lable,"AMB_NAME")==0)
        {
            strcpy(cfg.AMB_NAME,tmp);
        }
        else if(strcmp(lable,"TIME_INTERVAL")==0)
        {
            cfg.TIME_INTERVAL=str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_CONTIME")==0)
        {
            cfg.CONACC_CONTIME=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_START")==0)
        {
            cfg.CONACC_START=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_TIME")==0)
        {
            cfg.CONACC_TIME=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_E")==0)
        {
            cfg.CONACC_E=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_N")==0)
        {
            cfg.CONACC_N=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_U")==0)
        {
            cfg.CONACC_U=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_D")==0)
        {
            cfg.CONACC_D=(int)str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"CONACC_Num")==0)
        {
            cfg.CONACC_Num=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_timeN")==0)
        {
            cfg.CONACC_timeN=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"CONACC_dataN")==0)
        {
            cfg.CONACC_dataN=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"ACC_UNIT")==0)
        {
            cfg.ACC_UNIT=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"TIME_UNIT")==0)
        {
            cfg.TIME_UNIT=(int)str2num(tmp,0,strlen(tmp));
        }
          else if(strcmp(lable,"ACC_UNIT_ALL")==0)
        {
            cfg.ACC_UNIT_ALL=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"TIME_UNIT_ALL")==0)
        {
            cfg.TIME_UNIT_ALL=(int)str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"TIME_PER_INTERVAL")==0)
        {
            cfg.TIME_PER_INTERVAL=(int)str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"TIME_UNIT_ACC_PER")==0)
        {
            cfg.TIME_UNIT_ACC_PER=(int)str2num(tmp,0,strlen(tmp));
        }
        else if(strcmp(lable,"ACC_PER_INTERVAL")==0)
        {
            cfg.ACC_PER_INTERVAL=str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"ACC_PER_ALL")==0)
        {
            cfg.ACC_PER_ALL=str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"TIME_UNIT_ACC_PER")==0)
        {
            cfg.TIME_UNIT_ACC_PER=(int)str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"FIX_PER_INTERVAL")==0)
        {
            cfg.FIX_PER_INTERVAL=str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"FIX_PER_ST")==0)
        {
            cfg.FIX_PER_ST=(int)str2num(tmp,0,strlen(tmp));
        }
         else if(strcmp(lable,"FIX_PER_EN")==0)
        {
            cfg.FIX_PER_EN=(int)str2num(tmp,0,strlen(tmp));
        }
    }
    cfg.CONACC_EPOCH=cfg.CONACC_CONTIME*60/cfg.TIME_INTERVAL;
    fclose(fp);
    return cfg;
}