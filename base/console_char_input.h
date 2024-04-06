#ifndef CONSOLE_CHAR_INPUT_H__
#define CONSOLE_CHAR_INPUT_H__
//http://safari.oreilly.com/1592991106/app01
	//http://livedocs.adobe.com/flash/8/main/wwhelp/wwhimpl/common/html/wwhelp.htm?context=LiveDocs_Parts&file=00001686.html
	
//need to try this too:
//http://krtkg1.rug.ac.be/~colle/C/get_char_without_enter.html
//http://www.oreilly.com/catalog/secureprgckbk/chapter/ch11.pdf
// The cpp stuff still doesn't work 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> /* For STDIN_FILENO */
#include <sys/select.h> /* For select() */
#include <iostream>

//from oreilly:
//http://www.oreilly.com/catalog/secureprgckbk/chapter/ch11.pdf
void spc_make_fd_nonblocking(int fd) {
int flags;
flags = fcntl(fd, F_GETFL); /* Get flags associated with the descriptor. */
if (flags = = -1) {
perror("spc_make_fd_nonblocking failed on F_GETFL");
exit(-1);
}
flags |= O_NONBLOCK;
/* Now the flags will be the same as before, except with O_NONBLOCK set.
*/
if (fcntl(fd, F_SETFL, flags) = = -1) {
perror("spc_make_fd_nonblocking failed on F_SETFL");
exit(-1);
}
}


//MyClass::count_type MyClass::cin_problem()
//{
	
//const count_type avail= cin.rdbuf()->in_avail();	
 typedef char CinCh;
 FILE *mfp = stdin;
//////////////////////////////////////////////
/*bool try_char=true;
CinCh cc=' ';
while ( try_char)
{
struct timeval tv;
        fd_set readfds;
   int STDIN=0;
        tv.tv_sec = 0; //2;
        tv.tv_usec = 2; //500000;

        FD_ZERO(&readfds);
        FD_SET(STDIN, &readfds);

        // don't care about writefds and exceptfds:
        select(STDIN+1, &readfds, NULL, NULL, &tv);

        if (FD_ISSET(STDIN, &readfds))
  {         // printf("A key was pressed!\n");
             // this blocks too- linebuff
             int ii = read(0, &cc, 1);  
             int ikey=(int)cc;
	if ((ikey<(CmpCh)' ')||(ikey>126)) std::cout<<" unknown cin "<<ikey<<"..."; 
	 KeyboardCallback(SigCh(cc),0,0);
             }
        else try_char=false;
  //          printf("Timed out.\n");

}*/
//////////////////////////////////////////////
// std::cout<<"check ing :"<<avail<<" \n";
 //count_type av=avail; CinCh c=' ';
 //while (av--)
 //{	
 //	cin.get(c);
 //	int ikey=(int)c;
//	if ((ikey<(CmpCh)' ')||(ikey>126)) std::cout<<" unknown cin "<<ikey<<"..."; 
//	 KeyboardCallback(SigCh(c),0,0);
//}
//return avail;	
//}

int main( int argc, char *argv[] ) {
	
typedef char CinCh;
 FILE *mfp = stdin;
//////////////////////////////////////////////
bool try_char=true;
CinCh cc=' ';
while ( try_char)
{

	struct timeval tv;
        fd_set readfds;
   int STDIN=0;
        tv.tv_sec = 0; //2;
        tv.tv_usec = 2; //500000;

        FD_ZERO(&readfds);
        FD_SET(STDIN, &readfds);

        // don't care about writefds and exceptfds:
        select(STDIN+1, &readfds, NULL, NULL, &tv);

        if (FD_ISSET(STDIN, &readfds))
  {         // printf("A key was pressed!\n");
             // this blocks too- linebuff
             int ii = read(0, &cc, 1);  
             int ikey=(int)cc;
	//if ((ikey<(CmpCh)' ')||(ikey>126)) 
	std::cout<<" unknown cin "<<ikey<<"..."; 
	 //KeyboardCallback(SigCh(cc),0,0);
             }
       // else try_char=false;
  else          printf("Timed out.\n");

}
	
return 0;


	}



#endif

