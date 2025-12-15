#include <stdlib.h>
#include "random.h"


namespace random {
    int xdy(int x,int y)//生成一个xdy的随机数
    {
        int sum=0;
        while(x>0){
            sum+=a_to_b(1,y);
            --x;
        }
        return sum;
    }
    int a_to_b(int a,int b)//生成一个>=a,<=b的随机数
    {
        return rand()%(b-a+1)+a;
    }
}
