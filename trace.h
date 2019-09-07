#ifndef TRACE_H
#define TRACE_H

extern int get_turn_amount(int line_pos, int target_pos);
extern void general_control();
extern int search_run(int vel);
extern int shortcut_run(int vel_mode, int sc_mode);

//extern void accelation();

#endif