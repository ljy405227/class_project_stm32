#ifndef __MY_MAIN_H
#define __MY_MAIN_H

void my_main(void);
void app_task_key(void *argument);
void app_task_control(void *argument);
void app_task_shell(void *argument);
void app_task_timer(void *argument);
void app_timer_irq_handler(void);

#endif /* __MY_MAIN_H */
