/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>

/**
 * @brief Preemptive OS projet
 */

extern info_t *info;

/**
 * @brief Task 1 processus
 * Must run in ring 3
 * Task 1 : write a uint32_t counter in a shared memory emplacement
 */
void user1()
{
    /* TODO */
}

void user2()
{
    /* TODO */
}

void main()
{
    debug("Hello from the kernel !\n");
}
