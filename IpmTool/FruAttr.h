
#ifndef  __FRU_ATTR_H__
#define  __FRU_ATTR_H__

#ifdef _cplusplus
extern "c"{
#endif

#include "typedef.h"

//Fru Attribute
typedef struct _FruAttr
{
	uint8 m_DeviceId;
	uint8 m_IicChannel;
	uint8 m_PowerChannel;
	uint8 m_SiteNum;
	uint8 m_SiteType;
	uint8 m_IpmbAddress;
	uint8 m_SlotNum;
	uint8 m_Name[8];
}FruAttr;

FruAttr* FruAttr_get(const uint8 siteNum, const uint8 siteType);
const FruAttr* FruAttr_getById(const uint8 devId);
const FruAttr* FruAttr_getByAddress(uint8 ipmbAddress);

#ifdef _cplusplus
}
#endif

#endif 

