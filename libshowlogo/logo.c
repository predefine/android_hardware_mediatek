#include <errno.h>
#include <fcntl.h>
#include <showlogo.h>
#include <log/log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int anim_logo_init(void){
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

    if(!read(logo_fd, logo_data, LOGO_PARTITION_SIZE)){
        ALOGE("Failed read \"%s\": %s", logo_partition_path, strerror(errno));
        close(logo_fd);
        return 1;
    }
    return 0;
}
