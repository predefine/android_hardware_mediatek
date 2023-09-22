#include <errno.h>
#include <showlogo.h>
#include <log/log.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <cutils/properties.h>

void anim_fb_addr_switch(void){
    //TODO
}

int anim_fb_init(void){
    // return if framebuffer was initialized
    if(fb_buf != NULL) return 0;

    char hwrotation_str[92];

    fb_fd = open(FB_PATH, O_RDWR);

    if(fb_fd < 0){
        ALOGE("Failed open \"%s\": %s", FB_PATH, strerror(errno));
        return 1;
    }
    
    ioctl(fb_fd, FBIOGET_FSCREENINFO, &fbfscreeninfo);
    ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvscreeninfo);

    fb_bpp      = fbvscreeninfo.bits_per_pixel;
    fb_width    = fbfscreeninfo.line_length / (fb_bpp>>3);
    fb_height   = fbvscreeninfo.yres;
    fb_size     = fb_width * fb_height;
    fb_buf_size = fb_size * (fb_bpp>>3);
    fb_buf      = mmap(0, fb_buf_size*2,
                PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    
    fb_red_offset = fbvscreeninfo.red.offset;
    fb_blue_offset = fbvscreeninfo.blue.offset;

    if(fb_buf == NULL) {
        munmap(fb_buf, fb_buf_size);
        close(fb_fd);
        ALOGE("fb_buf == NULL");
        return 1;
    }
    if(property_get("ro.logo.hwrotation", hwrotation_str, 0) < 0)
        //set to zero lol
        hwrotation_str[0] = '0';

    hwrotation = atoi(hwrotation_str);

    if(!(hwrotation > -1 && hwrotation < 361 && hwrotation % 90 == 0))
        hwrotation = 0;

    return 0;
}


void anim_fb_deinit(void){
  munmap(fb_buf, fb_buf_size);
  close(fb_fd);
  return;
}

void anim_fb_disp_update(void){
    if(anim_fb_init()){
        ALOGE("anim_fb_init failed!");
        return;
    }

    //lets rotate screen before update!
    anim_fb_rotate();

    fbvscreeninfo.yoffset  = 0;
    fbvscreeninfo.activate = fbvscreeninfo.activate | FB_ACTIVATE_FORCE;
    ioctl(fb_fd, FBIOPUT_VSCREENINFO, &fbvscreeninfo);
}

void anim_fb_rotate(void){
    if(hwrotation == 0) return;
    int* fb_local = (int*)malloc(fb_buf_size);
    memcpy((void*)fb_local, fb_buf, fb_buf_size);
    if(hwrotation == 180)
        for(int x = 0; x < fb_height; x++)
            for(int y = 0; y < fb_width; y++){
                int new_pos = (fb_height - x - 1) * fb_width + (fb_width - y - 1);
                ((int*)fb_buf)[x * fb_width + y] = ((int*)fb_local)[new_pos];
            }
    if(hwrotation == 270)
        for(int x = 0; x < fb_height; x++)
            for(int y = 0; y < fb_width; y++){
                int new_x = y;
                int new_y = -x;
                int new_pos = new_x * fb_height + (fb_height + new_y - 1);
                ((int*)fb_buf)[x * fb_width + y] = fb_local[new_pos];
            }
    if(hwrotation == 90)
        for(int x = 0; x < fb_height; x++)
            for(int y = 0; y < fb_width; y++){
                int new_x = -y;
                int new_y = x;
                int new_pos = (fb_width + new_x - 1) * fb_height + new_y;
                ((int*)fb_buf)[x * fb_width + y] = fb_local[new_pos];
            }
    free(fb_local);
}
