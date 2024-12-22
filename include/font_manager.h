#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include "common.h"

typedef struct FontBitMap {
	region tregion;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;
	
}FontBitMap,*pFontBitMap;

typedef struct Font_Opr {
	char *name;
	int (*FontInit)(char *aFinename);
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int dwCode,pFontBitMap ptFontBitMap);
	struct Font_Opr *ptNext;
}Font_Opr,*pFont_Opr;


void RegisterFont(pFont_Opr ptFont_Opr);

void FontsRegister(void);
int SelectAndInitFont(char *aFontOprName,char *aFontFileName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode,pFontBitMap ptFontBitMap);


#endif




