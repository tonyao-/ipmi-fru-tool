#ifndef SERIAL_COM_API
#define SERIAL_COM_API

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if CSERIAL_COM_API
class CSerialComApi
{
public:
	CSerialComApi(void);
	~CSerialComApi(void);
};
#endif

void	WINAPI SeiralCom_Enum();
HANDLE	WINAPI SeiralCom_Open();
void	WINAPI SeiralCom_Closed(HANDLE hSerialCom);
void	WINAPI SeiralCom_Config(HANDLE hSerialCom);

#ifdef __cplusplus
}
#endif

#endif
