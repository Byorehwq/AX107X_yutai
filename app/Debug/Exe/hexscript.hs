;Masrom.a51������
set(out, 8, le);               �������λ��Ϊ8BIT��С��
load(hex, app.a51);     ����
;map(4000, 4000, 1EC0);
;map(100, 2000, A00);   
;map(1600, 3000, A00);
setoutput(4000, 1EC0);
;save(hex, app_isd.hex);     ���
clear(out);
map(4000, 0, 4000);   
;setoutput(0, 4000);
save(hex, app.hex);
save(bin, app.bin);
