

#ifndef _diskio_h_
#define _diskio_h_


/* Status of Disk Functions */
typedef u8	DSTATUS;

/* Results of Disk Functions */
typedef enum
{
    RES_OK = 0,		/* 0: Successful */
    RES_ERROR,		/* 1: R/W Error */
    RES_WRPRT,		/* 2: Write Protected */
    RES_NOTRDY,		/* 3: Not Ready */
    RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


#endif
