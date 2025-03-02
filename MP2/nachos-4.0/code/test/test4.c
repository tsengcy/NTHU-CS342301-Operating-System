#include "syscall.h"

main(){
	int i;
	int v;
	for(i=0; i<5; i++){
		v = DoubleInt(i);
		PrintInt(v);
	}
}
