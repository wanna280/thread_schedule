#include <stdio.h>

extern void mysleep(int seconds);

void fun1()
{
    int i = 10;
    while (i--)
    {
        printf("Wanna--fun1 is running\n");
        mysleep(2);
    }
}

void fun2()
{
    int i = 10;
    while (i--)
    {
        printf("Wanna--fun2 is running\n");
        mysleep(1);
    }
}

void fun3()
{
    int i = 2;
    while (i--)
    {
        printf("Wanna--fun3 is running\n");
        mysleep(5);
    }
}

void fun4() {
  int i = 15;
  int m;
  int n;
  while(i--) {
    printf("Wanna--fun4 is running\n");

    for (m = 0; m < 1000; ++m)
      for (n = 0; n < 1000; ++n);
  }
}

