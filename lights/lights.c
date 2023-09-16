#include <hardware/hardware.h>
#include <hardware/lights.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BRIGHTNESS_FILE "/sys/class/leds/lcd-backlight/brightness"

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;

void init_globals(void)
{
    pthread_mutex_init(&g_lock, NULL);
}

static int write_int(const char* path, int value){
    int fd = open(path, O_WRONLY);
    if (fd < 0) goto error;
    char buffer[sizeof(value)+1];
    sprintf(buffer, "%d\n", value);
    if(write(fd, buffer, sizeof(buffer)) < 1) goto error;
    errno = 0;

error:
    if(fd > 0) close(fd);
    return -errno;
}

static int rgb_to_whiteblack(int color){
    return (
        (77*((color>>16)&0xff)) +
        (150*((color>>8)&0xff)) + 
        (29*(color&0xff))
    ) >> 8;
}

static int set_light_backlight(struct light_device_t* device, const struct light_state_t* state){
    (void)device;
    pthread_mutex_lock(&g_lock);
    int err = write_int(BRIGHTNESS_FILE,
        rgb_to_whiteblack(state->color)
    );
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int close_lights(struct light_device_t *device){
    if (device)
        free(device);
    return 0;
}

static int open_lights(const struct hw_module_t* module, const char* type,
                       struct hw_device_t** device){
    if(strcmp(LIGHT_ID_BACKLIGHT, type))
        //Not implemented
        return -EINVAL;

    pthread_once(&g_init, init_globals);

    struct light_device_t* dev = malloc(sizeof(struct light_device_t));
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = (int(*)(struct hw_device_t*))close_lights;
    dev->set_light = set_light_backlight;
    
    *device = (struct hw_device_t*)dev;
    return 0;
}

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .hal_api_version = HARDWARE_HAL_API_VERSION,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "MT6580 OpenSource Lights hardware",
    .author = "predefine",
    .methods = &(struct hw_module_methods_t){
        .open = open_lights
    },
};
