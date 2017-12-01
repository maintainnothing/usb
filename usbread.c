#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/mod_devicetable.h>
#include <linux/trace_events.h>
#include <linux/slab.h>
#include <linux/tboot.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("Dual BSD/GPL");

void *usbva = 0;

static int hello_init(void)
{
  void *usbva = NULL;
  int capbase = 0;
  short capid = 0;
  int capnext = 0;
  int __capnext = 0;
  int capp = 0;
  int i = 0;

  usbva = (void *)ioremap(0xf3320000, 16*PAGE_SIZE);

  capbase = *(int *)(usbva + 0x10);
  printk(KERN_ALERT "usbread: capbase %x\n", capbase);
  capbase = (capbase & 0xffff0000) >> 14;
  printk(KERN_ALERT "usbread: capbase %x\n", capbase);

  do {
    capp = *(int *)(usbva + capbase + capnext);

    printk(KERN_ALERT "usbread: capp %x\n", capp);
    capid = capp & 0xff;
    __capnext = (capp & 0xff00) >> 6;
    capnext += __capnext;
    printk(KERN_ALERT "usbread: capid %d\n", capid);
    printk(KERN_ALERT "usbread: capnext %x\n", capnext);
    i++;
  } while (__capnext != 0 && i < 20);

  return 0;
}
static void hello_exit(void)
{
  iounmap(usbva);
  printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(hello_init);
module_exit(hello_exit);
