#ifndef START_MUSIC_H
#define START_MUSIC_H


typedef struct
{
    u8 len;   //�������ֳ���(��512Ϊ��λ)
    u8 ofs;
    u8 flag;  //1:�����������ڲ�����
} type_startmusic_ctl;

extern type_startmusic_ctl startmusic_ctl;

void startmusic_play(void);
void startmusic_init(void);
bool startmusic_seek(void);
bool startmusic_read(void *buf);

#endif

