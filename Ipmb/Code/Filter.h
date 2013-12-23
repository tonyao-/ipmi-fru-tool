

#ifndef  __FILTER_H_
#define  __FILTER_H_

#ifdef _cplusplus
extern "c"{
#endif

#define BUFFER_LEN 100
typedef int (*FilterTransferFun)(void* pObj, const unsigned char * buffer, const unsigned char  len);
typedef struct _tagFilter
{
	unsigned char 	m_isFindHead:1;
	unsigned char 	m_isValid:1;
	unsigned char 	m_errCount:6;
	
	unsigned char 	m_Len;	//Len of Rx data, Not include Packet head/tail
	unsigned char 	m_buffer[BUFFER_LEN];

	FilterTransferFun FilterOk;	//Call back fun
	void*			m_pObj;		//Param for Call back
}Filter;

void Filter_Init(Filter* filter
		, FilterTransferFun 	filterOk
		, void*			pObj
		);

int Filter_UnPacking(Filter* filter, const unsigned char * pByte, unsigned int len)  ;
int Filter_Packing(Filter* filter
	, unsigned char * pDst
	, unsigned char* dstLen
	, const unsigned char * pSrc
	, const unsigned char  srcLen);


#ifdef _cplusplus
}
#endif

#endif

