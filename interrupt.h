#ifndef __INTERRUPT_HEADER__
#define __INTERRUPT_HEADER__

extern void mtu3_tgra(void);
extern void mtu3_tgrb(void);
extern void mtu4_tgrb(void);
extern void increment_timer_ms(void);
extern void increment_timer_sec(void);

enum type_time{
	TYPE_MS,
	TYPE_SEC
};
	

#endif

