
#include "FruAttr.h"
#include "Common_define.h"
#include "Debug.h"

static FruAttr g_FruAttrTable[] =
{
	 {CM_Dev_ID, 	CM_1_IIC_CHAN, MCH_1_ChanNum, MCH_1_SITENUM, SITE_TYPE_CM, MCH_1_IPMB_ADDR,	0, "CM1"}
	,{PM_1_Dev_ID, 	PM_1_IIC_CHAN, PM_1_ChanNum,  PM_1_SITENUM, SITE_TYPE_PM, PM_1_IPMB_ADDR, 1, "PM1"}
	,{MCH_1_Dev_ID, MCH_1_IIC_CHAN, MCH_1_ChanNum, MCH_1_SITENUM, SITE_TYPE_MCH, MCH_1_IPMB_ADDR, 2, "MCH1"}
	
	,{AMC_1_Dev_ID ,AMC_1_IIC_CHAN , AMC_1_ChanNum , AMC_1_SITENUM,  SITE_TYPE_AMC, AMC_1_IPMB_ADDR, 3, "AMC1"}
	,{AMC_2_Dev_ID ,AMC_2_IIC_CHAN , AMC_2_ChanNum , AMC_2_SITENUM,  SITE_TYPE_AMC, AMC_2_IPMB_ADDR, 4, "AMC2"}
	,{AMC_3_Dev_ID ,AMC_3_IIC_CHAN , AMC_3_ChanNum , AMC_3_SITENUM,  SITE_TYPE_AMC, AMC_3_IPMB_ADDR, 5, "AMC3"}
	,{AMC_4_Dev_ID ,AMC_4_IIC_CHAN , AMC_4_ChanNum , AMC_4_SITENUM,  SITE_TYPE_AMC, AMC_4_IPMB_ADDR, 6, "AMC4"}
	,{AMC_5_Dev_ID ,AMC_5_IIC_CHAN , AMC_5_ChanNum , AMC_5_SITENUM,  SITE_TYPE_AMC, AMC_5_IPMB_ADDR, 7, "AMC5"}
	,{AMC_6_Dev_ID ,AMC_6_IIC_CHAN , AMC_6_ChanNum , AMC_6_SITENUM,  SITE_TYPE_AMC, AMC_6_IPMB_ADDR, 8, "AMC6"}
	,{AMC_7_Dev_ID ,AMC_7_IIC_CHAN , AMC_7_ChanNum , AMC_7_SITENUM,  SITE_TYPE_AMC, AMC_7_IPMB_ADDR, 9, "AMC7"}
	,{AMC_8_Dev_ID ,AMC_8_IIC_CHAN , AMC_8_ChanNum , AMC_8_SITENUM,  SITE_TYPE_AMC, AMC_8_IPMB_ADDR, 10, "AMC8"}
	,{AMC_9_Dev_ID ,AMC_9_IIC_CHAN , AMC_9_ChanNum , AMC_9_SITENUM,  SITE_TYPE_AMC, AMC_9_IPMB_ADDR, 11, "AMC9"}
	,{AMC_10_Dev_ID,AMC_10_IIC_CHAN , AMC_10_ChanNum, AMC_10_SITENUM, SITE_TYPE_AMC, AMC_10_IPMB_ADDR, 12, "AMC10"}
	,{AMC_11_Dev_ID,AMC_11_IIC_CHAN , AMC_11_ChanNum, AMC_11_SITENUM, SITE_TYPE_AMC, AMC_11_IPMB_ADDR, 13, "AMC11"}
	,{AMC_12_Dev_ID,AMC_12_IIC_CHAN , AMC_12_ChanNum, AMC_12_SITENUM, SITE_TYPE_AMC, AMC_12_IPMB_ADDR, 14, "AMC12"}

	,{MCH_2_Dev_ID, MCH_2_IIC_CHAN, MCH_2_ChanNum, MCH_2_SITENUM, SITE_TYPE_MCH, MCH_2_IPMB_ADDR, 15, "MCH2"}
	,{PM_2_Dev_ID, 	PM_2_IIC_CHAN, PM_2_ChanNum,  PM_2_SITENUM, SITE_TYPE_PM, PM_2_IPMB_ADDR, 16, "PM2"}

	,{CU_1_Dev_ID, 	CU_1_IIC_CHAN, CU_1_ChanNum , CU_1_SITENUM, SITE_TYPE_CU, CU_1_IPMB_ADDR, 17, "CU1"}
	,{CU_2_Dev_ID, 	CU_2_IIC_CHAN, CU_2_ChanNum , CU_2_SITENUM, SITE_TYPE_CU, CU_2_IPMB_ADDR, 18, "CU2"}
	,{CM_Dev_ID, 	CM_2_IIC_CHAN, MCH_2_ChanNum, MCH_2_SITENUM, SITE_TYPE_CM, MCH_2_IPMB_ADDR, 19, "CM2"}
	
};

const static uint8 g_TotleFruCount = sizeof(g_FruAttrTable) / sizeof(FruAttr);


FruAttr* FruAttr_get(const uint8 siteNum, const uint8 siteType)
{
	int i = 0;

	for(i = 0; i < g_TotleFruCount; i++)
	{
		if(g_FruAttrTable[i].m_SiteNum == siteNum && g_FruAttrTable[i].m_SiteType == siteType)
		{
			return &g_FruAttrTable[i];
		}
	}

	TRACE_LEVEL(TRACE_WARNING, ("WARNING: No siteNum[%d] and sityType[%d].\n", siteNum, siteType));
	//Assert(False);
	return Null;
}

const FruAttr* FruAttr_getById(const uint8 devId)
{
	int i = 0;

	for(i = 0; i < g_TotleFruCount; i++)
	{
		if(g_FruAttrTable[i].m_DeviceId == devId)
		{
			return &g_FruAttrTable[i];
		}
	}

	//WARNING(("%s() return Null, devId=0x%x\n", _FUNC_, devId));
	//Assert(False);
	return Null;
}

const FruAttr* FruAttr_getByAddress(uint8 ipmbAddress)
{
	int i = 0;

	for(i = 0; i < g_TotleFruCount; i++)
	{
		if(g_FruAttrTable[i].m_IpmbAddress == ipmbAddress)
		{
			return &g_FruAttrTable[i];
		}
	}

	//WARNING(("%s() return Null, ipmbAddress=0x%x\n", _FUNC_, ipmbAddress));
	//Assert(False);
	return Null;
}

