#include "freetype.h"
#include "font_manager.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>

#include "ft2build.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H


int FreetypeFontInit(char *aFinename);
int FreetypeSetFontSize(int iFontSize);
int FreetypeGetFontBitMap(unsigned int dwCode,pFontBitMap ptFontBitMap);

static Font_Opr g_tFreeTypeOpr = {
	.name = "freetype",
	.FontInit = FreetypeFontInit,
	.SetFontSize = FreetypeSetFontSize,
	.GetFontBitMap = FreetypeGetFontBitMap,
};

static FT_Face g_tFace;
static int g_iDefaultfont_size = 12;
int FreetypeFontInit(char *aFinename)
{

	FT_Library	  library;

	int error;
	
    error = FT_Init_FreeType( &library );              /* initialize library */
    if(error)
    {
    	printf("FT_Init_FreeType err\n");
		return -1;
    }
    error = FT_New_Face( library, aFinename, 0, &g_tFace ); /* create face object */
    if(error)
    {
    	printf("FT_New_Face err\n");
		return -1;
    }

	FT_Set_Pixel_Sizes(g_tFace, g_iDefaultfont_size, 0);

	return 0;

}
int FreetypeSetFontSize(int iFontSize)
{
	FT_Set_Pixel_Sizes(g_tFace, iFontSize, 0);
	return 0;
}

int FreetypeGetFontBitMap(unsigned int dwCode,pFontBitMap ptFontBitMap)
{
	int error;
    FT_Vector pen;
    FT_GlyphSlot slot = g_tFace->glyph;

	pen.x = ptFontBitMap->iCurOriginX * 64;//单位 1/64个像素点
	pen.y = ptFontBitMap->iCurOriginY * 64;pen.x = ptFontBitMap->iCurOriginX * 64;

	/* 转换：transformation */
	FT_Set_Transform(g_tFace, 0, &pen);

	/* 加载位图: load glyph image into the slot (erase previous one) */
	error = FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER);
	if (error)
	{
		printf("FT_Load_Char error\n");
		return -1;
	}

	ptFontBitMap->pucBuffer = slot->bitmap.buffer;

	ptFontBitMap->tregion.iLeftupx = slot->bitmap_left;
	ptFontBitMap->tregion.ileftupy = ptFontBitMap->iCurOriginY*2 - slot->bitmap_top;
	ptFontBitMap->tregion.iwidth = slot->bitmap.width;
	ptFontBitMap->tregion.iheigh = slot->bitmap.rows;//行数
	ptFontBitMap->iCurOriginX = ptFontBitMap->iCurOriginX + slot->advance.x/64;
	ptFontBitMap->iCurOriginY = ptFontBitMap->iCurOriginY;
	
	return 0;

}

void FreetypeRegister(void)
{
	RegisterFont(&g_tFreeTypeOpr);
}

