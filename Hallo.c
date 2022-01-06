/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Buzuluk Mariia");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int t = 1, i;

module_param(t, uint, 0660);
MODULE_PARM_DESC(t, "Define how many times print message");

struct list {
  struct list_head tlist;
  ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
  if (t > 10) {
    printk(KERN_INFO "ERROR! Parameter is incorrect\n");
    return -EINVAL;
  }

  if (t == 0 || (t >= 5 && t <= 10))
    printk(KERN_INFO "WARNING! Parameter may cause errors\n");

  for (i = 0; i < t; i++) {
    struct list *n = kmalloc(sizeof(*n), GFP_KERNEL);

    n->time = ktime_get();
    list_add(&n->tlist, &my_list_head);

    printk(KERN_INFO "Hello, world!\n");
  }

  return 0;
}

static void __exit hello_exit(void)
{
  struct list *md, *tmp;

  list_for_each_entry_safe(md, tmp, &my_list_head, tlist) {
    printk(KERN_INFO "%llx nanoseconds\n", md->time);
    list_del(&md->tlist);
    kfree(md);
  }
  BUG_ON(!list_empty(&my_list_head));
}

module_init(hello_init);
module_exit(hello_exit);