#include <iostream>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <unistd.h>
using namespace std;

int time_delay_const = 50000;

// int percent; // user selected value between 1 and 100
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

int calc_effective_sleep_time(double time_diff, double percentage);//e takes the desired CPU percentage and the measured time of the inner while loop, and calculates how long the process should sleep:


int main()
{
    double percent = 0;
    cout << "enter a percentage" <<endl;
    cin >> percent;
    double time_diff = 0;
    if(percent > 100 || percent < 1){
        cout <<"enter a value between 1-100" <<endl;
        cin >> percent;
    }
    else {
        while(1){
            std::clock_t c_start = std::clock();
            //std::clock_t c_end = std::clock();//ends clock time
            while(1){
                std::clock_t c_start2 = std::clock();
                time_diff = c_start2 - c_start;
                //cout << time_diff << endl;
                if(time_diff > time_delay_const){
                    break;
                }
                int sleep_time = calc_effective_sleep_time(time_diff, percent);
                cout << sleep_time << endl;
                usleep(sleep_time);
            }
            

        }
    }
    return 0;
}

int calc_effective_sleep_time(double time_diff, double percent){
    
    double p = percent/100;
    double sleep = time_diff*(1/p-1);
    return sleep;

}
