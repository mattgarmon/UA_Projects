#include <iostream>
#include <time.h> 
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

/*semaphores and boolean global variables*/
sem_t agentSem, tobacco, paper, match,
      tobaccoSem, paperSem, matchSem, mutex;
bool isTobacco = false, isPaper = false, isMatch = false;

/*agent threads (these wait for a random amount of time (0-100ms) before starting)*/
void* tobacco_agent(void* arg){
    usleep(rand()%100000);
    for(int i = 0; i < 6; i++){
        sem_wait(&agentSem);
        std::cout << "Agent: *Offering paper and a match*\n";
        sem_post(&paper);
        sem_post(&match);
    }
}
void* paper_agent(void* arg){
    usleep(rand()%100000);
    for(int i = 0; i < 6; i++){
        sem_wait(&agentSem);
        std::cout << "Agent: *Offering tobacco and a match*\n";
        sem_post(&tobacco);
        sem_post(&match);
    }
}
void* match_agent(void* arg){
    usleep(rand()%100000);
    for(int i = 0; i < 6; i++){
        sem_wait(&agentSem);
        std::cout << "Agent: *Offering tobacco and paper*\n";
        sem_post(&tobacco);
        sem_post(&paper);
    }
}

/*smoker threads*/
void* tobacco_smoker(void* arg){
    char* id = (char*)arg;
    for(int i = 0; i < 3; ++i){
        sem_wait(&tobaccoSem);
        std::cout << "Tobacco smoker #" << id << ": *Smoking cig #" << i+1 << "*\n";
        sem_post(&agentSem);
    }
    std::cout << "Tobacco smoker #" << id << ": \"I'm hungry, time to go home.\"\n";
}
void* paper_smoker(void* arg){
    char* id = (char*)arg;
    for(int i = 0; i < 3; ++i){
        sem_wait(&paperSem);
        std::cout << "Paper smoker #" << id << ": *Smoking cig #" << i+1 << "*\n";
        sem_post(&agentSem);
    }
    std::cout << "Paper smoker #" << id << ": \"I'm hungry, time to go home.\"\n";
}
void* match_smoker(void* arg){
    char* id = (char*)arg;
    for(int i = 0; i < 3; ++i){
        sem_wait(&matchSem);
        std::cout << "Match smoker #" << id << ": *Smoking cig #" << i+1 << "*\n";
        sem_post(&agentSem);
    }
    std::cout << "Match smoker #" << id << ": \"I'm hungry, time to go home.\"\n";
}

/*pusher threads*/
void* tobacco_pusher(void* arg){
    for(int i = 0; i < 12; i++){
        sem_wait(&tobacco);
        sem_wait(&mutex);
        std::cout << "Tobacco Pusher: \"I have tobacco, ";
        if(isPaper){
            isPaper = false;
            std::cout << "giving ingredients to a match smoker!\"\n";
            sem_post(&matchSem);
        }
        else if(isMatch){
            isMatch = false;
            std::cout << "giving ingredients to a paper smoker!\"\n";
            sem_post(&paperSem);
        }
        else {
            isTobacco = true;
            std::cout << "who needs it?\"\n";
        }
        sem_post(&mutex);
    }
}
void* paper_pusher(void* arg){
    for(int i = 0; i < 12; i++){
        sem_wait(&paper);
        sem_wait(&mutex);
        std::cout << "Paper Pusher: \"I have paper, ";
        if(isTobacco){
            isTobacco = false;
            std::cout << "giving ingredients to a match smoker!\"\n";
            sem_post(&matchSem);
        }
        else if(isMatch){
            isMatch = false;
            std::cout << "giving ingredients to a tobacco smoker!\"\n";
            sem_post(&tobaccoSem);
        }
        else{
            isPaper = true;
            std::cout << "who needs it?\"\n";
        }
        sem_post(&mutex);
    }
}
void* match_pusher(void* arg){
    for(int i = 0; i < 12; i++){
        sem_wait(&match);
        sem_wait(&mutex);
        std::cout << "Match Pusher: \"I have a match, ";
        if(isPaper){
            isPaper = false;
            std::cout << "giving ingredients to a tobacco smoker!\"\n";
            sem_post(&tobaccoSem);
        }
        else if(isTobacco){
            isTobacco = false;
            std::cout << "giving ingredients to a paper smoker!\"\n";
            sem_post(&paperSem);
        }
        else{
            isMatch = true;
            std::cout << "who needs it?\"\n";
        }
        sem_post(&mutex);
    }
}

int main(){
    srand(time(NULL)); //seed rng

    /*initialize semaphores*/
    sem_init(&agentSem, 0, 1);
    sem_init(&tobacco, 0, 0);
    sem_init(&paper, 0, 0);
    sem_init(&match, 0, 0);
    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);
    sem_init(&mutex, 0, 1);

    /*declare threads*/
    pthread_t tobaccoAgent, paperAgent, matchAgent,
              tobaccoPusher, paperPusher, matchPusher,
              tobaccoSmoker1, tobaccoSmoker2, 
              paperSmoker1, paperSmoker2, 
              matchSmoker1, matchSmoker2;
    
    /*create threads*/
    pthread_create(&tobaccoAgent, NULL, tobacco_agent, NULL);
    pthread_create(&paperAgent, NULL, paper_agent, NULL);
    pthread_create(&matchAgent, NULL, match_agent, NULL);
    pthread_create(&tobaccoPusher, NULL, tobacco_pusher, NULL);
    pthread_create(&paperPusher, NULL, paper_pusher, NULL);
    pthread_create(&matchPusher, NULL, match_pusher, NULL);
    pthread_create(&tobaccoSmoker1, NULL, tobacco_smoker, (void*)"1");
    pthread_create(&tobaccoSmoker2, NULL, tobacco_smoker, (void*)"2");
    pthread_create(&paperSmoker1, NULL, paper_smoker, (void*)"1");
    pthread_create(&paperSmoker2, NULL, paper_smoker, (void*)"2");
    pthread_create(&matchSmoker1, NULL, match_smoker, (void*)"1");
    pthread_create(&matchSmoker2, NULL, match_smoker, (void*)"2");

    /*execute threads*/
    pthread_join(tobaccoAgent, NULL);
    pthread_join(paperAgent, NULL);
    pthread_join(matchAgent, NULL);
    pthread_join(tobaccoPusher, NULL);
    pthread_join(paperPusher, NULL);
    pthread_join(matchPusher, NULL);
    pthread_join(tobaccoSmoker1, NULL);
    pthread_join(tobaccoSmoker2, NULL);
    pthread_join(paperSmoker1, NULL);
    pthread_join(paperSmoker2, NULL);
    pthread_join(matchSmoker1, NULL);
    pthread_join(matchSmoker2, NULL);

    /*destroy semaphores*/
    sem_destroy(&agentSem);
    sem_destroy(&tobacco);
    sem_destroy(&paper);
    sem_destroy(&match);
    sem_destroy(&tobaccoSem);
    sem_destroy(&paperSem);
    sem_destroy(&matchSem);
    sem_destroy(&mutex);

    return 0;
}