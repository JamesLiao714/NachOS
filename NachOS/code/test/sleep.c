#include "syscall.h"

//using namespace std;
main() {
	int i;
	for(i = 0; i < 5; ++i)
	{
        	Sleep(5000*i);
		PrintInt(222);
	}	            
    return 0;
}
