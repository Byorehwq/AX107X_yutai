;Masrom.a51的生成
set(out, 8, le);               设置输出位宽为8BIT，小端
load(hex, app.a51);     载入
;map(4000, 4000, 1EC0);
;map(100, 2000, A00);   
;map(1600, 3000, A00);
setoutput(4000, 1EC0);
;save(hex, app_isd.hex);     输出
clear(out);
map(4000, 0, 4000);   
;setoutput(0, 4000);
save(hex, app.hex);
save(bin, app.bin);
