#include <iostream>

int main (int argc, char *argv[]) {
    
    volatile int i;
    while(true)
        i = 42 ;
    return 0;
}
