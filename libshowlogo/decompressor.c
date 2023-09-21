#include <log/log.h>
#include <showlogo.h>
#include <stddef.h>
#include <zlib.h>

int check_logo_index_valid(int index, void * logo_ptr, struct logo_data * logo_out){
    unsigned int *logouint = (unsigned int*)logo_ptr;
    logo_out->logonum = logouint[0];
    if(index >= logo_out->logonum)
        return -1;

    int logo_offset = logouint[2+index];

    if(index < logo_out->logonum - 1)
        logo_out->logolen = logouint[3+index] + logo_offset;
    else
        logo_out->logolen = logouint[1] + logo_offset;
    
    logo_out->offset = (unsigned int*)((char*)logo_ptr + logo_offset);
    ALOGD("%d, %d, %d", logo_out->logonum, logo_out->logolen, logo_offset);
    return 0;
}

int decompress_logo(void* logo_offset, int* fb_addr, int logolen, int fb_size){
    struct z_stream_s zstr;
    zstr.zalloc = Z_NULL;
    zstr.zfree = Z_NULL;
    zstr.opaque = Z_NULL;
    int data_count = 0;
    int zret = 0;
    if(inflateInit(&zstr) != Z_OK) {
        ALOGE("inflateInit failed!");
        return -1;
    }

    while (zret != Z_STREAM_END){
        zstr.avail_in = logolen;

        if(zstr.avail_in <= 0){
            ALOGD("zstr.avail_in <= 0");
            goto end;
        }
        zstr.next_in = (Bytef*)logo_offset;
        while(zstr.avail_out == 0){
            zstr.next_out = (Bytef*)fb_addr;
            zstr.avail_out = fb_size;
            zret = inflate(&zstr, Z_NO_FLUSH);
            if(zret == Z_MEM_ERROR){
                ALOGE("inflate: Z_MEM_ERROR");
                inflateEnd(&zstr);
                return -1;
            } else if(zret == Z_DATA_ERROR){
                ALOGE("inflate: Z_DATA_ERROR");
                inflateEnd(&zstr);
                return -1;
            } else if(zret == Z_NEED_DICT){
                ALOGE("inflate: Z_NEED_DICT");
                inflateEnd(&zstr);
                return -1;
            }
            data_count += fb_size - zstr.avail_out;
        }
    }
end:
    inflateEnd(&zstr);
    return data_count;
}

