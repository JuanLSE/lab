/*
 * mutex_blk.h
 *
 *  Created on: 16/12/2016
 *      Author: alvaro
 */
///////////////////////////////////////////////////////////////////////////////
/*
 * Este fichero implementa dos funciones en ensamblador que aprovecha la infra-
 * estructura del ARM para implementar un sem�foro (mutex). Una funci�n sirve
 * para el cierre del sem�foro (lock_mutex) y la otra para la apertura
 * (unlock_mutex).
 */
///////////////////////////////////////////////////////////////////////////////

#ifndef MUTEX_BLK_H_
#define MUTEX_BLK_H_

///////////////////////////////////////////////////////////////////////////////
// Mutex control
///////////////////////////////////////////////////////////////////////////////

#define locked   1
#define unlocked 0

#ifdef __cplusplus
extern "C" {
#endif
extern void lock_mutex(void* mutex);
extern void unlock_mutex(void* mutex);
#ifdef __cplusplus
}
#endif


#endif /* MUTEX_BLK_H_ */
