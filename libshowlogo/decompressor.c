#include <log/log.h>
#include <showlogo.h>
#include <stddef.h>
#include <zlib.h>

#define Z_ERR_HANDLER(stream, ret, err, end_label, data_count_var) if(ret==err) {\
    ALOGE("inflate: %s", #err);\
    data_count_var = err;\
    goto end_label; \
}

int check_logo_index_valid(int index, void * logo_ptr, struct logo_data * logo_out){
    unsigned int *logouint = (unsigned int*)logo_ptr;
    logo_out->logonum = logouint[0];
    if(index >= logo_out->logonum)
        return -1;

    int logo_offset = logouint[2+index];

    if(index < logo_out->logonum - 1)
        logo_out->logolen = logouint[3+index] - logo_offset;
    else
        logo_out->logolen = logouint[1] - logo_offset;
    
    logo_out->offset = (unsigned int*)((char*)logo_ptr + logo_offset);
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
            Z_ERR_HANDLER(&zstr, zret, Z_MEM_ERROR, end, data_count);
            Z_ERR_HANDLER(&zstr, zret, Z_DATA_ERROR, end, data_count);
            Z_ERR_HANDLER(&zstr, zret, Z_NEED_DICT, end, data_count);
            data_count += fb_size - zstr.avail_out;
        }
    }
end:
    inflateEnd(&zstr);
    return data_count;
}

