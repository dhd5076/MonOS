/*
*  init.c
*/
extern void write_port(unsigned short port, short data);
extern short read_port(unsigned short port);

#include "kb.h"
#include "io.h"
#include "cli.h"
#include "hd.h"
#include "fs.h"
				
//TODO: add timer/sleep function/syscall
 
//Kernel Entry
void main(void)
{
	kprintf("INIT: Entering runlevel 3\n");
	kb_init();//Init Keyboard
	fs_init();//Init filesystem
	cli_init();//Init command line
	while(1==1);//Prevent overflow			
}
