#pragma once

#include <FreeRTOS/queue.h>

/**
 * @brief TaskQueue serves as a type-safe wrapper around the
 *        queue provided by FreeRTOS.
 *
 */
template<typename T, size_t size>
class TaskQueue
{
    QueueHandle_t queue;

public:
    template<typename T, size_t size>
    TaskQueue()
    {
        queue = xQueueCreate(size, sizeof(T));
    }

    /**
     * @brief Push an item to the back of the queue.
     *
     * @param item         The item that should be copied into the queue.
     * @param ticksToWait  How long the task should be blocked if the queue
     *                     is currently full.
     * @return             True if successful, False if the queue was full.
     */
    template<typename T>
    bool push_back(T item, TickType_t ticksToWait = portMAX_DELAY)
    {
        auto res = xQueueSend(queue, item, ticksToWait);
        if (res == errQUEUE_FULL)
            return false;
        else
            return true;
    }

    /**
     * @brief Read from the queue.
     *
     * @param ticksToWait  How long the task should be blocked if the
     *                     queue is currently empty.
     */
    template<typename T>
    T pop(TickType_t ticksToWait = portMAX_DELAY)
    {
        T ret;
        xQueueReceive(queue, &ret, ticksToWait);
        return ret;
    }
};