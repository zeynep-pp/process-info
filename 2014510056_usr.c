#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>      
#include <asm/uaccess.h>   
#include <linux/syscalls.h>
#include <signal.h>
#include <sys/types.h>



SYSCALL_DEFINE0(processinfo)
{
    printk("Right Usage:\n -all prints some information (process id and itsargument/s) about all processes\n -p takes process id and prints the details of it\n -k takes process id\n");
    return 0;
}


void status(void)  {

	struct file *f;
    char buf[200]; //buffer for first 7 row of process info as buf
    mm_segment_t fs;
    int i;
    for(i=0;i<200;i++)
        buf[i] = 0;//buffering for process inf
 
    f = filp_open("/proc/[pid]/status", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
		printk(KERN_INFO "\nProcess id and the argument list for the processes: \n\n"); 
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf, 121, &f->f_pos); // buffer first 121 ch
        set_fs(fs);
        printk(KERN_INFO "%s\n",buf); // kernel print
    }
    filp_close(f,NULL); //fr close


	}


	void cmdline(void) { 
	struct file *f;
    char *puf = (char *) vmalloc(30); //parsing the cmdline 
	char *temp;
    mm_segment_t fs;
    int k;
   
 
    f = filp_open("/proc/[pid]/cmdline", O_RDONLY, 0); // cmdline
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
		printk(KERN_INFO "\Argument#​[process_id]​-Process Information: \n\n");
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, puf, 128, &f->f_pos);
        set_fs(fs);

		temp = strsep(&puf," ");
        printk("the number of active tasks : %s\n",temp); 
		temp = strsep(&puf," ");    
		temp = strsep(&puf," ");	
		temp = strsep(&puf," ");
        printk("the total number of processes : %s\n",temp); 
    }

    filp_close(f,NULL); 


}



void signal(void) { 

int kill(pid_t pid, int sig);
}

int simple_init(void) { 
	status(); //functioncalls
	cmdline();	
	signal();	

}


void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n"); // remove

}

module_init(simple_init); 
module_exit(simple_exit); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("process info"); 
MODULE_AUTHOR("zeynep");
