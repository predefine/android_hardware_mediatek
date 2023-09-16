#include <errno.h>
#include <showlogo.h>
#include <log/log.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void anim_fb_init(void){
    // return if framebuffer was initialized
    if(fb_fd > -1) return;

    fb_fd = open(FB_PATH, O_RDWR);

    if(fb_fd < 0){
        ALOGE("Failed open \"%s\": %s", FB_PATH, strerror(errno));
        exit(-1);
    }
}
