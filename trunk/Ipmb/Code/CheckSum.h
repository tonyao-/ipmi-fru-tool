
#ifndef  __CHECK_SUM__
#define  __CHECK_SUM__

#ifdef _cplusplus
extern "c"{
#endif

unsigned char CheckSum_Get(const unsigned char * pData, int Length);
unsigned char CheckSum_Check(const unsigned char *pData, int Length);



#ifdef _cplusplus
}
#endif

#endif 

