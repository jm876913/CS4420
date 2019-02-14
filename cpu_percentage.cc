#include <iostream>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
using namespace std;



// int user_defined_percentage; // user selected value between 1 and 100
// while(1){
//  measure_cpu_time(t1);
//  while(1){
//  mesure_cpu_time(t2);
// time_diff = time(t2)-time(t1);
//  if( time_diff > TIME_DELAY_CONSTANT)
//  break;
//  }
//  sleep_time = calc_effective_sleep_time(time_diff, percentage);
//  sleep(sleep_time);
// }

int calc_effective_sleep_time(time_diff, percentage);//e takes the desired CPU percentage and the measured time of the inner while loop, and calculates how long the process should sleep:


int main()
{
    int user_defined_percentage = 0;
    cin >> user_defined_percentage;
    if(user_defined_percentage > 100 || user_defined_percentage < 1){
        cout <<"enter a value between 1-100" <<endl;
        cin >> user_defined_percentage;
    }
    else {
        while(1){
            std::clock_t c_start = std::clock();
            //std::clock_t c_end = std::clock();//ends clock time
            while(1){

            }

        }
    }
    return 0;
}

int calc_effective_sleep_time(float time_diff, int percentage){

}
