/* 
 * Automatic Deallocation of Memory Library v1.0
 * 
 * Uses a stack of pointers to clean memory
 * it clear at the end of execution, not the scope
 * 
 * @author: Gabriel-AB
 * https://github.com/Gabriel-AB/
 * 
 */

#include <stdlib.h>

/* 
 * ### Free your pointer at the end of execution
 * returns `ptr`
 */
void* autofree(void * ptr);

/* 
 * ### Remove the pointer from the cleaning list
 */
void autofreeCancel(void *ptr);