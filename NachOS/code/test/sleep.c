#include "syscall.h"

//using namespace std;
main() {
    int i;
    for(i = 0; i < 5; i++) {
	//cout << "testing Waituntil: " <<endl;
        Sleep(1000000*i);
	PrintInt(222);
        //cout <<"fininsh NO." << i << endl<<endl;
    }
    return 0;
}
