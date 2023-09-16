#include <errno.h>
#include <hardware/hardware.h>
#include <hardware/memtrack.h>

int memtrack_init(const struct memtrack_module *module){
    (void)module;
    return 0;
}

int memtrack_getMemory(const struct memtrack_module *module, pid_t pid,
                        int type,struct memtrack_record *records,
                        size_t *num_records){
    
    //TODO
    (void)module;
    (void)pid;
    (void)type;
    (void)records;
    (void)num_records;
    return 0;
}

struct memtrack_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = MEMTRACK_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = MEMTRACK_HARDWARE_MODULE_ID,
        .name = "MT6580 OpenSource Memtrack hardware",
        .author = "predefine"
    },
    .init = memtrack_init,
    .getMemory = memtrack_getMemory
};
