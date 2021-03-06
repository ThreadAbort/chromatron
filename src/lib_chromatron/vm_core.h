// <license>
// 
//     This file is part of the Sapphire Operating System.
// 
//     Copyright (C) 2013-2018  Jeremy Billheimer
// 
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// </license>

#ifndef _VM_CORE_H
#define _VM_CORE_H

#include <stdint.h>


#define VM_ISA_VERSION              8

#define RETURN_VAL_ADDR             0

#define FILE_MAGIC      0x20205846  // 'FX  '
#define PROGRAM_MAGIC   0x474f5250  // 'PROG'
#define CODE_MAGIC      0x45444f43  // 'CODE'
#define DATA_MAGIC      0x41544144  // 'DATA'
#define META_MAGIC      0x4154454d  // 'META'

#define VM_MAX_CYCLES               32768

#define DATA_LEN                    4


#define VM_STATUS_OK                    0
#define VM_STATUS_ERR_BAD_CRC           -1
#define VM_STATUS_ERR_BAD_FILE_MAGIC    -2
#define VM_STATUS_ERR_BAD_PROG_MAGIC    -3
#define VM_STATUS_ERR_INVALID_ISA       -4
#define VM_STATUS_ERR_BAD_CODE_MAGIC    -5
#define VM_STATUS_ERR_BAD_DATA_MAGIC    -6
#define VM_STATUS_ERR_BAD_FILE_HASH     -7

#define VM_STATUS_ERR_MAX_CYCLES        -30

#define VM_STATUS_CODE_MISALIGN         -40
#define VM_STATUS_DATA_MISALIGN         -41
#define VM_STATUS_IMAGE_TOO_LARGE       -42
#define VM_STATUS_HEADER_MISALIGN       -43
#define VM_STATUS_READ_KEYS_MISALIGN    -44
#define VM_STATUS_WRITE_KEYS_MISALIGN   -45
#define VM_STATUS_PUBLISH_VARS_MISALIGN -46
#define VM_STATUS_PIXEL_MISALIGN        -47

#define VM_STATUS_RESTRICTED_KEY        -70

#define VM_STATUS_ASSERT                -99
#define VM_STATUS_TRAP                  -100
#define VM_STATUS_HALT                  1


#define VM_LOAD_FLAGS_CHECK_HEADER      1


typedef struct __attribute__((packed)){
    int8_t status;
    int8_t return_code;
    uint32_t loop_time;
    uint32_t fader_time;
} vm_info_t;

// note this needs to pad to 32 bit alignment!
typedef struct __attribute__((packed)){
    uint32_t hash;
    uint8_t addr;
    uint8_t padding[3];
} vm_publish_t;

typedef struct __attribute__((packed)){
    uint32_t file_magic;
    uint32_t prog_magic;
    uint16_t isa_version;
    uint16_t code_len;
    uint16_t data_len;
    uint16_t read_keys_len; // length in BYTES, not number of objects!
    uint16_t write_keys_len; // length in BYTES, not number of objects!
    uint16_t publish_len; // length in BYTES, not number of objects!
    uint16_t pix_obj_len; // length in BYTES, not number of objects!
    uint16_t padding;
    uint16_t init_start;
    uint16_t loop_start;
    // variable length data:
    // read keys
    // write keys
    // publish vars
    // pixel objects
} vm_program_header_t;

// do not set packed on this struct, will crash the Xtensa
typedef struct{
    uint16_t code_start;
    uint16_t data_start;
    uint16_t prog_size;
    uint16_t data_len;
    
    uint16_t data_count;
    uint16_t init_start;
    uint16_t loop_start;
    uint16_t frame_number;

    // MUST BE 32 bit aligned on ESP8266!
    uint64_t rng_seed;

    uint8_t read_keys_count;
    uint16_t read_keys_start;

    uint8_t write_keys_count;
    uint16_t write_keys_start;

    uint8_t publish_count;
    uint16_t publish_start;

    uint8_t pix_obj_count;
    uint16_t pix_obj_start;

    uint8_t byte0;
} vm_state_t;

int8_t vm_i8_run(
    uint8_t *stream,
    uint16_t offset,
    vm_state_t *state,
    int32_t *data );

int8_t vm_i8_run_init(
    uint8_t *stream,
    vm_state_t *state );

int8_t vm_i8_run_loop(
    uint8_t *stream,
    vm_state_t *state );

int32_t vm_i32_get_data( 
    uint8_t *stream,
    vm_state_t *state,
    uint8_t addr );

void vm_v_get_data_multi( 
    uint8_t *stream,
    vm_state_t *state,
    uint8_t addr, 
    uint16_t len,
    int32_t *dest );

void vm_v_set_data( 
    uint8_t *stream,
    vm_state_t *state,
    uint8_t addr, 
    int32_t data );

int8_t vm_i8_load_program(
    uint8_t flags,
    uint8_t *stream,
    uint16_t len,
    vm_state_t *state );

int8_t vm_i8_eval( uint8_t *stream, int32_t *data, int32_t *result );

#endif
