#ifndef TIMER_H
#define TIMER_H

void InitTimer();
void ShutdownTimer();

extern int systemtime, timer_count;
extern int (*callback) (void);

#endif
