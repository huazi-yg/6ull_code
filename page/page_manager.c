
#include "page_manager.h"
#include "common.h"
#include "string.h"

static pPageAction g_ptPages = NULL;

void PageRegister(pPageAction ptPageAction)
{
	ptPageAction->ptNext = g_ptPages;
	g_ptPages = ptPageAction;
}

pPageAction Page(char *name)
{
	pPageAction pt_tmp = g_ptPages;

	while (pt_tmp)
		{
			if(strcmp(name,pt_tmp->name) == 0)
				return pt_tmp;
			pt_tmp = pt_tmp->ptNext;
		}
	return NULL;
}

void PagesRegister(void)
{
	extern void MainPageRegister(void);

	MainPageRegister();
}
