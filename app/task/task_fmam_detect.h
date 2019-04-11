#ifndef _USER_FMAM_H_
#define _USER_FMAM_H_

typedef struct
{
	u32 freq;
} type_fmam_ctl;
extern IAR_XDATA_A type_fmam_ctl fmam_ctl;


void task_fmam_detect(void);

#endif
