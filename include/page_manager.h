#ifndef _PAGE_MANAGER_H
#define _PAGE_MANAGER_H

typedef struct PageAction
{
	char *name;
	void (*Run)(void *pParam);
	struct PageAction *ptNext;
}PageAction,*pPageAction;

void PagesRegister(void);
void PageRegister(pPageAction ptPageAction);
pPageAction Page(char *name);



#endif

