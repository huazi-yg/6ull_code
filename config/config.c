
#include <string.h>
#include "config.h"
#include "stdio.h"


static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];
static int g_iItemCfgCount = 0;
int parse_config_file(void)
{

	//open config file

	FILE *fp;
	char buf[100]; 
	fp = fopen(CFG_FILE,"r");
	char *p = buf;

	if(!fp)
	{
		printf("can not open cfg file %s\n",CFG_FILE);
		return -1;
	}

	while(fgets(buf,100,fp))
	{
		buf[99] = '\0';
		//省略开头的空格和TAB
		while(*p == ' ' || *p == '\t')
			p++;

		//忽略注释
		if(*p == '#')
		{
			continue;
		}
		//处理
		g_tItemCfgs[g_iItemCfgCount].index = g_iItemCfgCount;
		sscanf(p,"%s %d %s",g_tItemCfgs[g_iItemCfgCount].name,\
		&g_tItemCfgs[g_iItemCfgCount].bCanbeTouched,\
		g_tItemCfgs[g_iItemCfgCount].command);

		printf("%s %d %s",g_tItemCfgs[g_iItemCfgCount].name,\
		g_tItemCfgs[g_iItemCfgCount].bCanbeTouched,\
		g_tItemCfgs[g_iItemCfgCount].command);
		
		g_iItemCfgCount++;
	}

	return 0;

}
int get_itemcfg_count(void)
{
	return g_iItemCfgCount;
}
pItemCfg get_itemcfg_by_index(int index)
{
	if(index < g_iItemCfgCount)
	{
		return &g_tItemCfgs[index];
	}
	else
	{
		return NULL;
	}
}
pItemCfg get_itemcfg_by_name(char * name)
{
	int i;

	for(i =0;i<g_iItemCfgCount;i++)
	{
		if(strcmp(name,g_tItemCfgs[i].name) == 0)
			return &g_tItemCfgs[i];
	}

	return NULL; 
}

