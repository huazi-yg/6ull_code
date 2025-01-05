#include "font_manager.h"
#include "freetype.h"
#include <string.h>


static pFont_Opr g_ptFonts = NULL;
static pFont_Opr g_ptDefaultFontOpr = NULL;

void RegisterFont(pFont_Opr ptFont_Opr)
{
	ptFont_Opr->ptNext = g_ptFonts;
	g_ptFonts = ptFont_Opr;
}

void FontsRegister(void)
{
	FreetypeRegister();
}

int SelectAndInitFont(char *aFontOprName,char *aFontFileName)
{
	pFont_Opr ptTmp = g_ptFonts;
	while (ptTmp)
		{
			if(strcmp(ptTmp->name,aFontOprName) == 0)
			{
				break;
			}
			ptTmp = ptTmp->ptNext;
		}

	if(!ptTmp)
		return -1;
	g_ptDefaultFontOpr = ptTmp;
	return ptTmp->FontInit(aFontFileName);
}

int SetFontSize(int iFontSize)
{
	return g_ptDefaultFontOpr->SetFontSize(iFontSize);
}

int GetFontBitMap(unsigned int dwCode,pFontBitMap ptFontBitMap)
{
	return g_ptDefaultFontOpr->GetFontBitMap(dwCode,ptFontBitMap);
}

