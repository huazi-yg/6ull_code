#include <linux/module.h>

#include <asm/io.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/major.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/tty.h>

#include "button_drv.h"

static int major = 0;                          // 设备号
static struct class *button_class;             // 类
static struct button_operations *p_button_opr; // 按键资源

// 注册led操作结构体
void register_button_operations(struct button_operations *opr) {
  p_button_opr = opr;
  device_create(button_class, NULL, MKDEV(major, p_button_opr->count), NULL,
                "button%d", p_button_opr->count); // dev/hello
}
void unregister_button_operations(struct button_operations *opr) {
  device_destroy(button_class, MKDEV(major, opr->count));
}

EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);

static ssize_t button_read(struct file *file, char __user *buf, size_t size,
                           loff_t *offset) {
  unsigned int minor = iminor(file_inode(file));
  int level;
  int err;

  level = p_button_opr->read(minor);
  err = copy_to_user(buf, &level, 1);
  return 1;
}

static int button_open(struct inode *node, struct file *file) {
  int minor = iminor(node);
  p_button_opr->init(minor);
  return 0; // 必须要返回值，不然会报错
}

// 2.定义自己的file_operations结构体
static struct file_operations button_ops = {
    .owner = THIS_MODULE,
    .open = button_open,
    .read = button_read,
};

static int __init button_init(void) {
  int err;
  int i;
  // 1. 确定设备号
  major = register_chrdev(0, "button",
                          &button_ops); // 注册字符设备,可在/proc/devices下看见

  button_class = class_create(THIS_MODULE, "button_class");
  err = PTR_ERR(button_class);
  if (IS_ERR(button_class)) {
    // printk("%s %s  line %d\n"__FILE__,__FUNCTION__,__LINE__);
    unregister_chrdev(major, "button");
    return -1;
  }
  // device_create(led_class,NULL,MKDEV(major,i),NULL,"led");//dev/hello
  return 0;
}

static void __exit button_exit(void) {
  int i;
  // device_destroy(led_class,MKDEV(major, i));
  class_destroy(button_class);
  unregister_chrdev(major, "button");
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");
