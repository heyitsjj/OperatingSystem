#include <os-scheduling.h>

/*******************************
 * QUEUE MANAGEMENT PRIMITIVES *
 *******************************/

void printQueues(task tasks[], sched_data* schedData) {
    int i, j, taskIndex = 0;
    printf("Nb of queues %d\n", schedData->nbOfQueues);
    for (i = 0; i < schedData->nbOfQueues; i++) {
        j = 0;
        printf("Q%d => ", i);
        while (j < MAX_NB_OF_TASKS) {
            taskIndex = schedData->queues[i][j];
            if (taskIndex == -1) {
                j = MAX_NB_OF_TASKS;
            } else {
                printf("%s ", tasks[taskIndex].name);
                j++;
            }
        }
        printf("\n");
    }
}

void initQueues(int nbQ, sched_data* schedData) {
    int i, j;
    printf("Initializing %d job queue(s)\n", nbQ);
    schedData->nbOfQueues = nbQ;
    for (j = 0; j < nbQ; j++) {
        for (i = 0; i < MAX_NB_OF_TASKS; i++) {
            schedData->queues[j][i] = -1;
        }
    }
}

int enqueue(sched_data* schedData, int queueIndex, int taskIndex) {
    int end = 0;
    while ((end < MAX_NB_OF_TASKS) && (schedData->queues[queueIndex][end] != -1))
        end++;
    if (end < MAX_NB_OF_TASKS) {
        schedData->queues[queueIndex][end] = taskIndex;
        return 0;
    } else {
        // memory exhausted
        return -1;
    }
}


int dequeue(sched_data* schedData, int queueIndex) {
    int j;
    int taskIndex = schedData->queues[queueIndex][0];
    if (taskIndex != -1) {
        for (j = 0; j < MAX_NB_OF_TASKS - 1; j++) {
            schedData->queues[queueIndex][j] = schedData->queues[queueIndex][j+1];
        }
        schedData->queues[queueIndex][MAX_NB_OF_TASKS - 1] = -1;
    }
    return taskIndex;
}

int head(sched_data* schedData, int queueIndex) {
    return schedData->queues[queueIndex][0];
}


/*******************************
 * TASKS MANAGEMENT PRIMITIVES *
 *******************************/

int admitNewTasks(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j;
    j = 0;
    while (schedData->queues[0][j] != -1)
        j++;
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
            schedData->queues[0][j] = i;
            j++;
        }
    }
    return 1;
}


/***********************
 * SCHEDULING POLICIES *
 ***********************/

int FCFS(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    
    int i;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        initQueues(1, schedData);
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);
    
    // Is the first task in the queue running? Has that task finished its computations?
    //   If so, put it in terminated state and remove it from the queue
    //   If not, continue this task
    i = head(schedData, 0);
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                dequeue(schedData, 0);
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    // Otherwise, elect the first task in the queue
    i = head(schedData, 0);
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}


int SJF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, minCompTime;
    
    // N.B: SJF does not require any queue!
    
    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running? Has that task finished its computations?
    //   If so, put it in terminated state
    //   If not, continue this task
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            if (tasks[i].executionTime == tasks[i].computationTime) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                break;
            } else {
                /* Reelect this task */
                tasks[i].executionTime ++;
                return i;
            }
        }
    }
    
    //Otherwise, find the task in READY state that has the shortest computation time
    j = -1;
    minCompTime = 0;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            if ((j == -1) || (minCompTime > tasks[i].computationTime)) {
                j = i;
                minCompTime = tasks[i].computationTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int SRTF(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i, j, remainingCompTime, minRemainingCompTime;
    
    // N.B: SRTF does not require any queue!
    
    // Admit new tasks (currentTime >= arrivalTime)
    for(i = 0; i < nbOfTasks; i++) {
        if ((tasks[i].state == UPCOMING) && (tasks[i].arrivalDate == currentTime)) {
            tasks[i].state = READY;
        }
    }
    
    // Is there a task running?
    //      => determine its remaining computation time
    //   Has that task finished its computations?
    //      => put it in terminated state
    //   else
    //      => put it back to READY
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == RUNNING) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if (remainingCompTime == 0) {
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
            } else {
                tasks[i].state = READY;
            }
            break;
        }
    }
    
    //Now elect the task in READY state that has the shortest remaining time
    j = -1;
    minRemainingCompTime = -1;
    for (i = 0; i < nbOfTasks; i++) {
        if (tasks[i].state == READY) {
            remainingCompTime = tasks[i].computationTime - tasks[i].executionTime;
            if ((j == -1) ||
                (minRemainingCompTime > remainingCompTime)) {
                j = i;
                minRemainingCompTime = remainingCompTime;
            }
        }
    }
    if (j != -1){
        tasks[j].executionTime ++;
        tasks[j].state = RUNNING;
        return j;
    }
    
    return -1;
}


int RR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i;

    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        initQueues(1, schedData);
    }

    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData); 

    /* If there is a task running: 
        Case1 Task completed  
            - then terminated and dequeue 
        Case2 Task not completed and reached quantum 
            - then put it at the end of the queue 
            - dequeue it from the head 
        Case3 Task not completed and not reached quantum 
            - reelected 
     */ 
    i = head(schedData, 0);
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            unsigned int remainingTime = tasks[i].computationTime - tasks[i].executionTime; 
            if (remainingTime == 0){      
                // Completed 
                // Task being terminated 
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                dequeue(schedData, 0);
            } else if (remainingTime > 0){ 
                if (tasks[i].cyclesInQuantum == schedData->quantum){ 
                    // Not completed && reached quantum 
                    // task being put at the end of the queue, dequeued from the head, waiting to be reelected 
                    tasks[i].cyclesInQuantum = 0; 
                    tasks[i].state = READY; 
                    enqueue(schedData, 0, i); 
                    dequeue(schedData, 0);
                } else if (tasks[i].cyclesInQuantum < schedData->quantum){ 
                    // Not completed && not reach quantum 
                    // task reelected 
                    tasks[i].executionTime ++; 
                    tasks[i].cyclesInQuantum ++; 
                    return i; 
                }
            }
        }  
    } 

    // If there is no task running, elect a new task 
    i = head(schedData, 0);
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].cyclesInQuantum ++; 
        tasks[i].state = RUNNING;
        return i;
    }
    
    return -1;
}

int MFQ(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    int i;
    
    // Initialize single queue
    if (currentTime == 0) {
        printf("Initializing job queue\n");
        initQueues(3, schedData);
    }
    
    admitNewTasks(tasks, nbOfTasks, schedData, currentTime);
    printQueues(tasks, schedData);
    

    // Check if jobs in queues are finished 
    // Check which queue is running tasks 
    i = head(schedData, 0);
    if (i == -1){ 
        i = head(schedData,1); 
        if (i == -1){ 
            i = head(schedData,2); 
        }
    }
    if (i != -1) {
        if (tasks[i].state == RUNNING) {
            unsigned int remainingTime = tasks[i].computationTime - tasks[i].executionTime; 
            if (remainingTime == 0){      
                // Completed 
                // Task being terminated 
                tasks[i].state = TERMINATED;
                tasks[i].completionDate = currentTime;
                dequeue(schedData, tasks[i].priorityLevel - 1);
            } else if (remainingTime > 0){ 
                if (tasks[i].cyclesInQuantum == (tasks[i].priorityLevel * schedData->quantum)){ 
                    /* Not completed && reached quantum in the current queue 
                       Case 1 priorityLevel = 3 
                          task being enqueued into the first level queue 
                          task being dequeued from the original level queue 
                       Case 2 priorityLevel != 3 (which means it equals to 1 or 2)
                          task being enqueued into the next level queue 
                          task being dequeued from the original level queue 
                    */ 

                    // Update cyclesInQuantum value 
                    tasks[i].cyclesInQuantum = 0; 

                    // Update state value 
                    tasks[i].state = READY; 

                    // Update priorityLevel of the task 
                    if (tasks[i].priorityLevel == 3) {
                        tasks[i].priorityLevel = 1; 
                        enqueue(schedData, 0, i); 
                        dequeue(schedData, 2);
                    } else {
                        enqueue(schedData, tasks[i].priorityLevel, i); 
                        dequeue(schedData, tasks[i].priorityLevel - 1);
                        tasks[i].priorityLevel += 1; 
                    } 
                } else if (tasks[i].cyclesInQuantum < (tasks[i].priorityLevel * schedData->quantum)){ 
                    // Not completed && not reach quantum of the current queue 
                    // task reelected 
                    tasks[i].executionTime ++; 
                    tasks[i].cyclesInQuantum ++; 
                    return i; 
                }
            }
        }  
    } 

    
    // If there is no task running, elect a new task 
    // Locate the next task to run 
    i = head(schedData, 0);
    if (i == -1){ 
        i = head(schedData,1); 
        if (i == -1){ 
            i = head(schedData,2); 
        }
    }
    if (i != -1){
        tasks[i].executionTime ++;
        tasks[i].cyclesInQuantum ++; 
        tasks[i].state = RUNNING;
        return i;
    }
    
    // No task could be elected
    return -1;
}

int IORR(task tasks[], int nbOfTasks, sched_data* schedData, int currentTime) {
    //TODO
    return -1;
}
