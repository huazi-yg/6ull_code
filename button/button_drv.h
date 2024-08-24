#ifndef __LED_DRV_H
#define __LED_DRV_H

struct button_operations {
  int count;
  int (*init)(int which);
  int (*read)(int which);
};

void register_button_operations(struct button_operations *opr);
void unregister_button_operations(struct button_operations *opr);

#endif
