//
//    프로세스 간 통신을 하기 전에 전송할 데이터 종류에 대한 프로토콜 정의 ..
//
struct Protocol
{
    enum { PROTOCOL_ACK=0, PROTOCOL_TFILE_INFO };
};

//
// 해당 프로토콜에 대해 전달 될 데이터 형
//
struct TeachingInfo
{
    TCHAR m_sEquipCode[50];
    TCHAR m_sProductCode[50];
    TCHAR m_sProductType[2];
		unsigned int m_unProductRev;
		unsigned int m_unTeachingVer;

		TeachingInfo()
		{
			Init();
		}

		void Init()
		{
			for (int i = 0; i < 50; i++)
			{
				m_sEquipCode[i] = 0;
				m_sProductCode[i]=0;
			}
			for (int i = 0; i < 2; i++)
			{
				m_sProductType[i] = 0;
			}
			m_unProductRev=0;
			m_unTeachingVer=0;
		}
};

struct AckInfo
{
   BOOL m_blACK;
};

//
// 해당 프로토콜에 대해 전달 될 데이터 형
//
struct SSAInfo
{
   int m_nResult;
   int m_nErrorCode;
};


/******************************************************************************************************************************
Color
******************************************************************************************************************************/
#define RGB_BLACK				RGB(0x01,0x01,0x01)
#define RGB_WHITE				RGB(0xFF,0xFF,0xFF)
#define RGB_GRAY				RGB(0x90,0x90,0x90)
#define RGB_GRAY_WHITE			RGB(0xFC,0xFC,0xFC)
#define RGB_RED					RGB(0xFF,0x00,0x00)
#define RGB_RED_DARK			RGB(0x88,0x00,0x00)
#define RGB_RED_SOFT			RGB(0xFF,0xB2,0xAF)
#define RGB_GREEN				RGB(0x00,0xFF,0x00)
#define RGB_GREEN_DARK			RGB(0x00,0x99,0x00)
#define RGB_BLUE				RGB(0x00,0x00,0xFF)
#define RGB_BLUE_LIGHT			RGB(0x00,0xFF,0xFF)
#define RGB_YELLOW				RGB(0xFF,0xFF,0x00)
#define RGB_ORANGE				RGB(0xFF,0xAA,0x00)
#define RGB_VIEW_BTN			RGB(0x40,0x40,0x40)
#define	RGB_VIEW_BK				RGB(0x50,0x50,0x50)
#define	RGB_DLG_MAIN			RGB(0x68,0x68,0x68)
#define	RGB_DLG_BK				RGB(0xD7,0xD7,0xD7)
#define RGB_BLUE_DARK			RGB(0x3C,0x50,0x87)
#define	RGB_GRID_BK				RGB_GRAY_WHITE//RGB(0xFF,0xFF,0xFF)
