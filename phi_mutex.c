// Theory p.173
// Problem 4.22
// phi [number of points] [number of threads]
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_OF_POINTS    2
#define NUMBER_OF_THREADS   2

void *runner(void *param);
/* Points in the circle */
int circle_count = 0; //global variables
pthread_mutex_t cost_lock;

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}

int main (int argc, const char *argv[]) {

    pthread_mutex_init(&cost_lock, NULL);

    int points_per_thread;
    int number_of_points= NUMBER_OF_POINTS;
    int number_of_threads=NUMBER_OF_THREADS;
    //printf("arg %i\n", argc);
    if (argc > 1){
      number_of_points=atoi(argv[1]);
      points_per_thread = number_of_points/number_of_threads;
      printf("points_perthread %i\n", points_per_thread);
      //printf("argv %s\n", argv[0]);
      //printf("argv %s\n", argv[1]);
    } else
      points_per_thread = NUMBER_OF_POINTS/ NUMBER_OF_THREADS;

    int i;
    double Pi;
    pthread_t workers[number_of_threads];
    printf("p=3.141592653589793238462643383279502884197169399375105820974944592307816406286\n	");
    float p=3.141592653589793238462643383279502884197169399375105820974944592307816406286;

    /* seed the random number generator */
    srandom((unsigned)time(NULL));
    clock_t begin = clock();
    for (i = 0; i < number_of_threads; i++)
        pthread_create(&workers[i], 0, runner, &points_per_thread);

    for (i = 0; i < number_of_threads; i++)
        pthread_join(workers[i],NULL);

    /* estimating Pi */
    Pi = 4.0 * circle_count / number_of_points;
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("NUMBER OF POINTS = %d\n",number_of_points);
    printf("Pi = %30.27f\n",Pi);
    printf("time = %f\n",time_spent);
    printf("Error=%30.27f\n",Pi-p);
    return 0;
}

//Parameter is the point for each thread
void *runner(void *param)
{
    int POINTS;
    POINTS = *((int *)param);
    int i;
    int hit_count = 0;
    double x,y;

    for (i = 0; i < POINTS; i++) {
        /*generate random numbers between -1.0 and +1.0 (exclusive)*/

        /* to obtain a random (x, y) point*/
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;

        /* is (x, y) point within the circle ? */
        if ( sqrt(x*x + y*y) < 1.0 )
            ++hit_count; /* yes, (x, y) point is in circle */
    }

    pthread_mutex_lock(&cost_lock);
    circle_count += hit_count; //global variables. It may conflict
    pthread_mutex_unlock(&cost_lock);

    pthread_exit(0);
}
