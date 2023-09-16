#include <showlogo.h>
#include <cutils/properties.h>
#include <log/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <fs_mgr.h>
#include <unistd.h>

#define FSTAB_BASE_TEMPLATE "/fstab."
#define FSTAB_TEMPLATE FSTAB_BASE_TEMPLATE"%s"
#define FSTAB_BASE_TEMPLATE_SIZE sizeof(FSTAB_BASE_TEMPLATE)

char* get_fstab_path(void){
    if(fstab_path != NULL)
        return fstab_path;
    char hardware[64];
    fstab_path = malloc(sizeof(hardware) + FSTAB_BASE_TEMPLATE_SIZE);
    if(!property_get("ro.hardware", hardware, "")){
        ALOGW("Can't get ro.hardware, maybe we in recovery");
        if(access("/etc/recovery.fstab", F_OK)){
            ALOGE("Can't find fstab!");
            exit(-1);
        }
        return "/etc/recovery.fstab";
    }
    sprintf(fstab_path, FSTAB_TEMPLATE, hardware);
    return fstab_path;
}

struct fstab* get_fstab(void){
    if(g_fstab != NULL) return g_fstab;
    g_fstab = fs_mgr_read_fstab(get_fstab_path());
    if(g_fstab == NULL){
        ALOGE("Error reading fstab!");
        exit(-1);
    }
    return g_fstab;
}

char* get_partition_path(char* partition_mount){
    struct fstab* fstab = get_fstab();
    struct fstab_rec * entry = \
        fs_mgr_get_entry_for_mount_point(fstab, partition_mount);
    if(entry == NULL){
        ALOGW("Failed get \"%s\" partition device", partition_mount);
        return NULL;
    }
    return entry->blk_device;
    
}

int get_int_value(char *path){
    (void)path;
    return 0;
}
