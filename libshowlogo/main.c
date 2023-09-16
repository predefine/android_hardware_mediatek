#include <log/log.h>
#include <showlogo.h>
#include <stdlib.h>
#include <string.h>

void anim_init(void){
    if(anim_logo_init()){
        ALOGE("anim_logo_init failed!");
        exit(-1);
    }
    if(anim_fb_init()){
        ALOGE("anim_fb_init failed!");
        exit(-1);
    }
}


void set_draw_mode(int draw_mode){
    if(draw_mode!=1){
        ALOGE("draw_mode != 1 not supported currently!");
        exit(-1);
    }
}

void set_anim_version(int ver){
    (void)ver;
}

void show_black_logo(void){
    memset(fb_buf, 0, fb_buf_size);
    anim_fb_disp_update();
}

void show_battery_capacity(int procent){
    (void)procent;
}

void show_kernel_logo(void){
    anim_show_logo(4);
}

void anim_deinit(void){
    anim_fb_deinit();
}
