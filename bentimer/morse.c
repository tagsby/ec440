//Morse code LED module for Raspberry Pi
static char *morse_table[] = {".-","-...","-.-.","-..", //A-D
    ".","..-.","--.","....","..",".---","-.-",".-..", //E-L
    "--","-.","---",".--.","--.-",".-.","...","-","..-", //M-U
    "...-",".--","-..-","-.--","--..", //V-Z
    ".----","..---","...--","....-",".....", //1-5
    "-....","--...","---..","----.","-----", //6-0
};

#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>
MODULE_DESCRIPTION("Example module illustrating the use of Keyboard LEDs.");
MODULE_LICENSE("GPL");
struct timer_list my_timer;
int timer_on = 0;
struct tty_driver *my_driver;
char morse_in = 'a';
char morse_status = 0;
#define DIT HZ/5 //dot
#define DAH HZ*4/5 //dash
#define SPACE HZ*2/5 //space

//Note. Each time you set up a new timer, you need to consider how long
//you want *that* timer to last. As in, you want to always set the LED
//status for dots/dashes in the space preceeding them
static void Dit(char morse_in) {
	printk(KERN_ERR "Morse in: %c\n",morse_in);
	timer_on=1; //Lock the function.
	if (morse_table[97-morse_in][0]==".") {
		morse_status = 6;
		printk(KERN_ERR "We're here!\n");
	} else {
	        if (morse_status == 0) {
			morse_status = 1;
			printk(KERN_ERR "Dot\n");
			my_timer.expires = jiffies + SPACE; //Was blink delay
			add_timer(&my_timer);
		} else if (morse_status == 1) {
			morse_status = 2;
			printk(KERN_ERR "Off\n");
			my_timer.expires = jiffies + DAH; //Was blink delay
			add_timer(&my_timer);
		} else  if (morse_status == 2) {
			printk(KERN_ERR "Done\n");
			morse_status = 0;
			timer_on=2;
		}
	}
}

static int __init kbleds_init(void) {
        printk(KERN_ERR "morse: loading\n");
        init_timer(&my_timer);
        my_timer.function = Dit;
        my_timer.data = morse_in;
        my_timer.expires = jiffies + DAH;
        add_timer(&my_timer);
	printk(KERN_ERR "Got there!\n");
        return 0;
}
static void __exit kbleds_cleanup(void) {
        printk(KERN_ERR "morse: unloading...\n");
        del_timer(&my_timer);
}
module_init(kbleds_init);
module_exit(kbleds_cleanup);
