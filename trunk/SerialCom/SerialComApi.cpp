#include "StdAfx.h"
#include "SerialComApi.h"
#include "DlgComConfig.h"
#include "SerialComApp.h"

#if CSERIAL_COM_API

CSerialComApi::CSerialComApi(void)
{
}


CSerialComApi::~CSerialComApi(void)
{
}
#endif

static CSerialComApp* g_pTheApp = NULL;

HANDLE	WINAPI SeiralCom_Open()
{
	if(g_pTheApp)
	{
		TRACE("Open Failed");
		return NULL;
	}

	g_pTheApp = new CSerialComApp();
	g_pTheApp->CreateThread();

	return g_pTheApp;
}

void WINAPI SeiralCom_Closed(HANDLE hSerialCom)
{
}

void WINAPI SeiralCom_Enum()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDlgComConfig dlg;
	dlg.DoModal();
}
