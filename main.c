#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Semaphore which refers to the bridge
sem_t bridge;

// Mutex which refers to the diving spot
pthread_mutex_t diving_spot;

// Counter for current number of tourists on the bridge
int num_tourist_on_bridge = 0;

// Counter for current number of tourists scuba diving
int num_tourist_diving = 0;

// Mutex function 
void* diving_function(int id)
{
    // Lock the mutex
    pthread_mutex_lock(&diving_spot);

    // Increment the number of tourist scuba diving
    num_tourist_diving++;

    // Show which tourist in scuba diving right now
    printf("\nTourist %d is scuba diving.\nNumber of tourists scuba diving: %d\n", id, num_tourist_diving);

    // Simulate time taken to scuba dive
    sleep(1);

    // Decrement the number of tourists scuba diving
    num_tourist_diving--;

    // Show which tourist finished scuba diving
    printf("\nTourist %d finished scuba diving.\nNumber of tourists scuba diving: %d\n", id, num_tourist_diving);

    // Unlock the mutex
    pthread_mutex_unlock(&diving_spot);

    return NULL;
}

// Semaphore Function
void* bridge_function(void* arg)
{
    // (Wait) Decrease the semaphore value
    sem_wait(&bridge);

    // Increment the number of tourist on the bridge
    num_tourist_on_bridge++;

    // Show which tourist in on the bridge right now
    printf("\nTourist %d is on the bridge.\nNumber of tourists on bridge: %d\n", *(int*)arg, num_tourist_on_bridge);

    // Simulating waiting time on the bridge
    sleep(1);

    // Tourist can dive if the diving spot is empty
    diving_function(*(int*)arg);

    // Decrease the number of tourist on the bridge after finishing diving
    num_tourist_on_bridge--;

    // Show which tourist in on the bridge right now
    printf("\nTourist %d left the bridge.\nnNumber of tourists on bridge: %d\n", *(int*)arg, num_tourist_on_bridge);

    // Signal (Increase the semaphore value)
    sem_post(&bridge);

    return NULL;
}

int main()
{
    // Declare 5 threads respective to 5 tourists
    pthread_t t1, t2, t3, t4, t5;

    // Declare the ID to differentiate each tourist
    int id1 = 1;
    int id2 = 2;
    int id3 = 3;
    int id4 = 4;
    int id5 = 5;

    /*
    Initialize the sempahore:
        -> 2nd argument: 0 = Sempahore is shared between threads
        -> 3rd argument: 3 = Maximum value of semaphore
    */
    sem_init(&bridge, 0, 3);

    // Create 5 threads respective to 5 tourists waiting at beach
    pthread_create(&t1, NULL, bridge_function, &id1);
    pthread_create(&t2, NULL, bridge_function, &id2);
    pthread_create(&t3, NULL, bridge_function, &id3);
    pthread_create(&t4, NULL, bridge_function, &id4);
    pthread_create(&t5, NULL, bridge_function, &id5);

    // Wait for all 5 tourists to queue and finish scuba diving
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&diving_spot);

    // Destroy the semaphore
    sem_destroy(&bridge);

    return 0;
}