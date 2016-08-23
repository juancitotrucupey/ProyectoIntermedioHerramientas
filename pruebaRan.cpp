#include <iostream>
#include <cmath>
#include "Random64.h"

int main(){
Crandom ran2(1);
for(int t=0;t<10000000000;t++){
    cout<<ran2.r()<<endl;
    } 
return 0;
}
