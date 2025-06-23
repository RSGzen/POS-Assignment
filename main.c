#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Mutex represents a single dorm
// Only a student or a repair worker can have rights to here at a same time
pthread_mutex_t single_dorm; 

// Semaphore represents a double dorm
// Two students can have rights to live here at the same time
sem_t double_dorm; 

// Declare and initialize total years people spent in the room from 2025
int total_years = 0;

// Declare and initialize total number of active threads running at the same time as 0
// Each student count as a thread and the repair worker also count as a single thread
// Active thread can be referred as total number of person in the room
int active = 0;

// Iitialize and set current year to 2025
int current_year = 2025;

// Initialize and set the years of studies left (YOSL) for each student
int studentA_yosl = 3;
int studentB_yosl = 4;
int studentC_yosl = 4;

// Intialize and set the number of years required for repair worker renovation
int renovation_years_required = 1;

// Function for threads that utilizes mutex

void* single_dorm_operation(void* arg)
{
    pthread_mutex_lock(&single_dorm); // Lock mutex
    
    int *repair_check = (int *)arg; // Cast to int data type

    int number1 = 1; // Just for the line below

    active++; // Current active threads plus 1

    // Print current active threads
    printf("\n| Current number of person in dorm room: %d |\n", active);

    // Provides boolean check for whether it is the repair worker renovating or student staying in
    if (*repair_check == number1)
    {
        printf("\nRepair worker is renovating the single dorm at year %d", current_year);

        // Adjust the year to after renovation
        current_year += renovation_years_required;

        // Increment total number of years repair worker spent in this room
        total_years += renovation_years_required;
        
        sleep(1); // Simulate the time required for renovation
        
        printf("\nRepair worker completed renovation at year %d.\n", current_year);
    }
    else
    {
        printf("\nStudent A is studying in the single dorm at year %d\n", current_year);

        // Decrease years of studies left for student A year by year
        for (int i = 0; i < studentA_yosl; i++)
        {
            // Increment current year counter
            current_year++;

            // Increment total number of years student A spent in this room per year
            total_years++;

            // Simulate time passed per year while studying
            sleep(1);

            printf("\nAfter %d year of study, it is now year %d", i+1, current_year);
        }

        printf("\n\nStudent A completed degreee at year %d\n", current_year);
    }

    printf("\nTotal years spent in this room: %d\n", total_years);

    pthread_mutex_unlock(&single_dorm); // Unlock mutex

    active--; // Number of active threads minus 1

    // Print current active threads
    printf("\n| Current number of person in dorm room: %d |\n", active);

    return NULL;
}

void* double_dorm_operation(void* arg)
{
    sem_wait(&double_dorm); // Wait (decrement of semaphore)

    int *student_flag = (int *)arg; // Cast to character data type for student names

    char student_name;

    int yosl; // Declare integer of year of studies left for current student

    int number0 = 0; // Just for the line below

    active++; // Current active threads plus 1

    // Print current active threads
    printf("\n| Current number of person in dorm room: %d |\n", active);

    // Determine whether the year of studies left is student A or student B
    if (*student_flag == number0)
    {
        yosl = studentB_yosl;
        student_name = 'B';
    }
    else
    {
        yosl = studentC_yosl;
        student_name = 'C';
    }

    printf("\nStudent %c is studying in double dorm at year %d\n", student_name, current_year);
    
    // Decrease years of studies left for student year by year
    for (int i = 0; i < yosl; i++)
        {
            int remaining_years; // Temp integer just for printing purposes

            // Decrement years left of study for student B / C
            if (student_name == 'B')
                {
                    studentB_yosl--;
                    remaining_years = studentB_yosl;

                    // Increase total number of years student B spent in this room
                    total_years++;
                }
            else
            {
                studentC_yosl--;
                remaining_years = studentC_yosl;

                // Increase total number of years student C spent in this room
                total_years++;
            }
            
            // Simulate time passed per year while studying
            sleep(1);

            printf("\nStudent %c studied for %d years, still %d years left", student_name, i+1, remaining_years);
            printf("\n\nTotal years spent in this room: %d\n", total_years);
        }
    
    sem_post(&double_dorm); // Signal (increment of semaphore)

    active--; // Number of active threads minus 1

    // Print current active threads
    printf("\n| Current number of person in dorm room: %d |\n", active);

    return NULL;
}

int main()
{
    pthread_t repair_worker, student_A;

    printf("\n===================================================\n");
    
    // Initialize mutex (single dorm)
    pthread_mutex_init(&single_dorm, NULL);
    printf("\nCurrently still at single dorm:");

    // Create flags for both threads
    int flag_studentA = 0;
    int flag_repairworker = 1;

    // Create two threads
    pthread_create(&student_A, NULL, single_dorm_operation, &flag_studentA);
    pthread_create(&repair_worker, NULL, single_dorm_operation, &flag_repairworker);

    // Wait for threads to finish
    pthread_join(student_A, NULL); // Student A gets to study until degree finishes
    pthread_join(repair_worker, NULL); // Repair worker can renovate after student A graduates

    // Destroy mutex (single dorm)
    pthread_mutex_destroy(&single_dorm);
    
    printf("\n\nNow single person dorm has renovated to double dorm");
    printf("\n===================================================\n");

    pthread_t student_B, student_C;

    // Initialize the semaphore
    
    sem_init(&double_dorm, 0, 2);
        // 2nd parameter, 0 => Allow semaphores to be shared between threads
        // 3rd parameter, 2 => Allow maximum of 2 threads in semaphore
    
    // Create flag for two threads
    int flag_studentB = 0; // 0 represents student B
    int flag_studentC = 1; // 1 represents student C

    // Create two threads
    pthread_create(&student_B, NULL, double_dorm_operation, &flag_studentB);
    pthread_create(&student_C, NULL, double_dorm_operation, &flag_studentC);

    // Wait for threads to finish
    pthread_join(student_B, NULL);
    pthread_join(student_C, NULL);

    // Increment the current year counter for time taken for student B and C to graduate    
    current_year += 4; // Years taken is 4 years

    printf("\nStudent B completed degreee at year %d\n", current_year);
    printf("\nStudent C completed degreee at year %d\n", current_year);

    printf("\nTotal years spent in this room: %d\n", total_years);

    // Destroy semaphore
    sem_destroy(&double_dorm);
    
    return 0;
}