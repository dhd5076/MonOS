/*
*  kernel.c
*/
extern void write_port(unsigned short port, short data);
extern short read_port(unsigned short port);
extern char read_key();

const int STDOUT_FILENO = 1;//File descriptor for std out
const int STDIN_FILENO = 0;//File descriptor for std in
char* MEM_FILELOC = 0x100000;//Location of file in memory
//Write

unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
char *vidptr = (char*)0xb8000; //Video memory location
int curpos = 0; //Location of cursor
void write(int fd,const char *buf,int nbytes) //Write nbytes of *buf to fd
{
	if(fd == STDOUT_FILENO)//print to video memory
	{
		int i = 0;//Declare counter 
		while(i < nbytes)//Write number of bytes specified
		{
			if(curpos > (80 * 25 * 2))//Check to see if we need to wrap around
			{
				curpos = 0;//Reset cursor
			}
			vidptr[curpos] = buf[i];//Set location in video memory to buffer
			vidptr[curpos + 1] = 0x07;//Set style to white over black
			curpos = curpos + 2;//Increment cursor by two;
			i++;//Increment counter 	
		}
	}
	else//!stdio so we write to file in memory
	{
		int i = 0;//Declare counter
		while(i < nbytes)//Write number of bytes specified
		{
			MEM_FILELOC[i] = buf[i];//Copy current buffer bytes to file 
			i++;//Increment memory location
		}
	}	

}
//Read 
void read(int fd,char *buf,int nbytes)
{
	if(fd == STDIN_FILENO)//Are we reading from standard input
	{
		int i = 0;//Declare counter
		while(i < nbytes)//Read number of bytes specified
		{
			write_port(0x21, 0xFD); //Request keyboard
			char status = read_port(0x64); //Get status of keyboard
			if(status & 0x01)//Make sure kb is ready to be read
			{
				char keycode = read_port(0x60); //Get keycode	
				buf[i] = keyboard_map[keycode]; //Map keycode to ASCII and put it into buffer
				i++;//Increment memory location
			}
		}					
	}
	else//!stdio so we read from file in memory
	{
		int i = 0;//Declare counter
		while(i < nbytes)//Read number of bytes specified
		{
			buf[i] = MEM_FILELOC[i]; //Copy current file byte to buffer
			i++;//Increment memory location
		}	
	}
}	
//Open system call
void open(const char *filename, int flags)
{
//TODO: Read disk and put file in MEM_FILELOC memory location 	
}
//Close system call
void close(const char *fd)
{
//TODO: Clear MEM_FILELOC
}
//Execute system call
void exec(const char *filename)
{

}
//Exit system call
void eexit(const char *filename)
{

}

void sleep(int ticks) 
{
	while (ticks > 0)
	{
		ticks--;
	}
}

void debug_disk_stuff(unsigned int LBA, unsigned int sectors_to_access)
{
	unsigned int tmpword;
	char *buffer;
	while((read_port(0x1F7)&0xC0) != 0x40);	
	write_port(0x1F1, 0x00);//Null byte
	write_port(0x1F2, (unsigned char)sectors_to_access);//Sectors to read/write
	write_port(0x1F3, (unsigned char)LBA);//Sector to access
	write_port(0x1F4, (unsigned char)(LBA >> 8));//Low bits of cylinder
	write_port(0x1F5, (unsigned char)(LBA >> 16));//High bits of cylinder
	write_port(0x1F6, (unsigned char)(0xE0 | (0) << 4) | ((LBA >> 24) & 0x0F));//Combine drive number with head
	write_port(0x1F7, 0x20);//Tell drive we want to read from it NOTE: 0x30 for write
	int i = 0;
	while (!(read_port(0x1F7) & 0x08)) {}
	while(i < (512 * sectors_to_access))
    {
		tmpword = read_port(0x1F0);//Read data
		buffer[i * 2]= (unsigned char)tmpword;//Write to buffer
		buffer[i * 2 + 1] = (unsigned char)(tmpword >> 8);//Write to buffer
		i++;//Increment counter
	}
	write(STDOUT_FILENO, buffer,512 * sectors_to_access);//Write contents of disk to stdout	
} 
//Kernel Entry
void main(void)
{
	int I = 64; //current sector;
	int key;
	write_port(0x21, 0xFD); //Init Keyboard TODO: Put in seperate function
	while(1)
	{
		debug_disk_stuff
	}
	debug_disk_stuff(I, 1);
}