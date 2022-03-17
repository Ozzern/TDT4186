#include <pthread.h>

// from https://eng.libretexts.org/Bookshelves/Computer_Science/Operating_Systems/Book%3A_Think_OS_-_A_Brief_Introduction_to_Operating_Systems_(Downey)/11%3A_Semaphores_in_C/11.03%3A_Make_your_own_semaphores
struct SEM
{
    int val, wakeups;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} SEM;

struct SEM *sem_init(int initVal)
{
    struct SEM *semaphore = check_malloc(sizeof(SEM));
    semaphore->val = initVal;
    semaphore->wakeups = 0;
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    semaphore->mutex = &mut;
    pthread_cond_init(semaphore->cond, NULL);
    return semaphore;
}

void P(struct SEM *sem)
{
    pthread_mutex_lock(sem->mutex);
    sem->val--;

    if (sem->val < 0)
    {
        do
        {
            pthread_cond_wait(sem->cond, sem->mutex);
        } while (sem->wakeups < 1);
        sem->wakeups--;
    }
    pthread_mutex_unlock(sem->mutex);
}

void V(struct SEM *sem)
{
    pthread_mutex_lock(sem->mutex);
    sem->val++;

    if (sem->val <= 0)
    {
        sem->wakeups++;
        pthread_cond_signal(sem->cond);
    }

    pthread_mutex_unlock(sem->mutex);
}