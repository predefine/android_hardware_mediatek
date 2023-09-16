#pragma once

// 8 Megabytes
#define LOGO_PARTITION_SIZE 1<<23
#define FB_PATH "/dev/graphics/fb0"

void set_draw_mode(int draw_mode);
void anim_init(void);
int anim_fb_init(void);
void anim_logo_init(void);
void anim_deinit(void);
void set_anim_version(int ver);

void show_kernel_logo(void);
void show_low_battery(void);
void show_charger_ov_logo(void);
void show_battery_capacity(int procent);
void show_black_logo(void);

// Utils
int get_int_value(char *path);
char* get_fstab_path(void);
struct fstab* get_fstab(void);
char* get_partition_path(char* partition_mount);

// Vars
static int fb_fd = 0;
static char* fb_ptr;
static char logo_data[LOGO_PARTITION_SIZE];
static char* fstab_path;
static struct fstab* g_fstab;
