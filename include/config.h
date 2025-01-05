#ifndef _CONFIG_H
#define _CONFIG_H

#define ITEMCFG_MAX_NUM 30
#define CFG_FILE  "/etc/test_gui/gui.conf"

typedef struct ItemCfg {
	int  index;
	char name[100];
	int bCanbeTouched;
	char command[100];
}ItemCfg,*pItemCfg;

int parse_config_file(void);
int get_itemcfg_count(void);
pItemCfg get_itemcfg_by_index(int index);
pItemCfg get_itemcfg_by_name(char * name);

#endif