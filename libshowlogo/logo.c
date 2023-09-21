#include <errno.h>
#include <fcntl.h>
#include <showlogo.h>
#include <log/log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int anim_logo_init(void){
    if(logo_data != NULL) return 0;
    logo_data = malloc(LOGO_PARTITION_SIZE);

    char* logo_partition_path = get_partition_path("/logo");
    if(logo_partition_path == NULL){
        ALOGE("anim_logo_init: failed get logo partition block");
        return 1;
    }
    int logo_fd = open(logo_partition_path, 0);
    if(logo_fd < 1){
        ALOGE("Failed open \"%s\": %s", logo_partition_path, strerror(errno));
        return 1;
    }

    read(logo_fd, logo_data, 512);
    if(!read(logo_fd, logo_data, LOGO_PARTITION_SIZE - 512)){
        ALOGE("Failed read \"%s\": %s", logo_partition_path, strerror(errno));
        close(logo_fd);
        return 1;
    }
    return 0;
}

// "variadic" for stock binaries
void fill_animation_logo(int logo_index,...){
    struct logo_data ldata;
    if(check_logo_index_valid(logo_index, 
        (int*)logo_data, &ldata)!=0){
        ALOGE("Failed to get logo with index %d!", logo_index);
        return;
    }
    if(decompress_logo(ldata.offset, (int*)fb_buf,
        ldata.logolen, fb_buf_size) < 1){
        ALOGE("Failed decompress logo with index %d!", logo_index);
        return;
    }
    anim_fb_disp_update();
    return;
}

// All variables is global for libshowlogo, anim_show_logo is just wrapper
void anim_show_logo(int logo_index){
    fill_animation_logo(logo_index);
}
