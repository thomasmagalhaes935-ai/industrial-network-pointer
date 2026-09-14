#ifndef COUNTER_H
#define COUNTER_H

void counter_init(void);

void counter_bom_process_detection(void);
void counter_ruim_process_detection(void);

int counter_get_bom_count(void);
int counter_get_ruim_count(void);

#endif