#pragma once

#include <string>
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/semphr.h>

bool pdToBoolean(BaseType_t val);

/**
 * @brief Promise should be inherited to provide a callback mechanism
 *        between tasks.
 * 
 * All promises must provide a mechanism for reporting a failure or success.
 * Promises will be consumed on a separate task from the one that created
 * them, so code should be restricted to the necessary FreeRTOS inter-task
 * communication to avoid race conditions.
 */ 
class Promise {
public:
    virtual void onSuccess(std::string resp);
    virtual void onFailure(std::string fail);
};

/**
 * @brief SemaphoreHandle provides a weak RAII interface for taking
 *        and giving semaphores in FreeRTOS.
 * 
 * Note that SemaphoreHandle does not obtain ownership of the semaphore
 * and will not handle allocation or deallocation of the object.
 */
class SemaphoreHandle {
    SemaphoreHandle_t *semaphore;
public:
    SemaphoreHandle(SemaphoreHandle_t *handle, TickType_t ticksToWait);
    SemaphoreHandle(SemaphoreHandle &&other);
    SemaphoreHandle &operator=(SemaphoreHandle &&other);
    ~SemaphoreHandle();
private:
    SemaphoreHandle(const SemaphoreHandle &other) = delete;
    SemaphoreHandle &operator=(const SemaphoreHandle &other) = delete;
};

/**
 * @brief A C++ wrapper around the BinarySemaphore provided by FreeRTOS.
 * 
 * Note that BinarySemaphore expresses strict ownership of semaphore object
 * itself rather than the value it represents. A new semaphore object will
 * be allocated with each construction of a BinarySemaphore object.
 */
class BinarySemaphore {
    SemaphoreHandle_t semaphore;
public:
    BinarySemaphore();
    ~BinarySemaphore();
    bool take(TickType_t blockTime);
    bool give();
    bool giveFromISR(BaseType_t *taskWoken);
    SemaphoreHandle getRAII();
};