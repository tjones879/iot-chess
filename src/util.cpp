#include "util.hpp"

bool pdToBoolean(BaseType_t val)
{
    if (val == pdTRUE)
        return true;
    else
        return false;
}

SemaphoreHandle::SemaphoreHandle(SemaphoreHandle_t *handle,
                                 TickType_t ticksToWait)
  : semaphore(handle)
{
    xSemaphoreTake(*semaphore, ticksToWait);
}

SemaphoreHandle::SemaphoreHandle(SemaphoreHandle &&other)
{
    semaphore = other.semaphore;
    other.semaphore = nullptr;
}

SemaphoreHandle &SemaphoreHandle::operator=(SemaphoreHandle &&other)
{
    semaphore = other.semaphore;
    other.semaphore = nullptr;
}

SemaphoreHandle::~SemaphoreHandle()
{
    if (semaphore) {
        xSemaphoreGive(*semaphore);
        semaphore = nullptr;
    }
}

BinarySemaphore::BinarySemaphore()
{
    semaphore = xSemaphoreCreateBinary();
}

BinarySemaphore::~BinarySemaphore()
{
    vSemaphoreDelete(semaphore);
}

bool BinarySemaphore::take(TickType_t blockTime)
{
    auto result = xSemaphoreTake(semaphore, blockTime);
    return pdToBoolean(result);
}

bool BinarySemaphore::give()
{
    auto result = xSemaphoreGive(semaphore);
    return pdToBoolean(result);
}

bool BinarySemaphore::giveFromISR(BaseType_t *taskWoken)
{
    auto result = xSemaphoreGiveFromISR(semaphore, taskWoken);
}