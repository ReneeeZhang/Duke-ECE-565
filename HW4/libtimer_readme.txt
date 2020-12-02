===================================================================
Description:

This library contains a few timer functions that you can use to
better understand how much execution time is spent different
sections of a program. You may find this helpful for HW4.

To use the library there are a few simple steps:
1) Add C extern declarations for 3 timer functions (shown below).
2) Instrument your code with calls to Timer_Start(...)
   and Timer_Stop(...) around code sections of interest.
3) Add a call to Timer_Print() at the end of the program.
4) When compiling the program, link to the libtimer.a
   library (example below).

After running the program, results will be written to a file
named timer_results.out in the current directory.


===================================================================
Example use in a simple test program:

#include <stdio.h>
#include <stdlib.h>

extern void Timer_Start(char *);
extern void Timer_Stop(char *);
extern void Timer_Print();

int main(int argc, char *argv[])
{
  Timer_Start("whole_program");

  for (int i=0; i < 100; i++) {
    Timer_Start("block1");
    printf("Hello\n");
    Timer_Stop("block1");
    
    Timer_Start("block2");
    printf("World!\n");
    Timer_Stop("block2");
  }
  
  Timer_Stop("whole_program");

  Timer_Print();
  return 0;
}


===================================================================
Example compilation where test.c and libtimer.a are in the same
directory:

gcc -O3 -o test test.c -L. -ltimer


===================================================================
Example output for the code above:

------------------------------------------------------------------------------------------------
Timer Results:                    #calls       avg(sec)     total(sec)
------------------------------------------------------------------------------------------------
whole_program                           1      0.000440     0.000440
block1                                100      0.000002     0.000250
block2                                100      0.000002     0.000172

