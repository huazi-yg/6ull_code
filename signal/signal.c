#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void my_signal(int sign)
{
    printf("get the signal %d\r\n",sign);
}
int main(int argc,char **argv)
{
    int i = 0;
    while(1)
    {
        signal(SIGIO,my_signal);
        {
            printf("hello word %d\r\n",i++);
            sleep(2);
        }
    }
    return 0;


}