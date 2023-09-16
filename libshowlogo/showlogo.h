#pragma once

#include <linux/fb.h>

// 8 Megabytes
#define LOGO_PARTITION_SIZE 1<<23
#define FB_PATH "/dev/graphics/fb0"

//framebuffer
int anim_fb_init(void);
void anim_fb_disp_update(void);
void anim_fb_deinit(void);
void anim_fb_addr_switch(void);

//logo
int anim_logo_init(void);
void anim_show_logo(int logo_index);

//other
void set_draw_mode(int draw_mode);
void anim_init(void);

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
// #framebuffer
static int fb_fd = 0;
struct fb_var_screeninfo fbvscreeninfo;
struct fb_fix_screeninfo fbfscreeninfo;
int fb_width;
int fb_height;
int fb_size;
int fb_buf_size;
char* fb_buf;
int fb_bpp;
int fb_red_offset;
int fb_blue_offset;
// #logo
static char logo_data[LOGO_PARTITION_SIZE];
// #fstab
static char* fstab_path;
static struct fstab* g_fstab;
// #properties
static int hwrotation;
