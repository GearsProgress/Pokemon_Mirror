#include <tonc.h>
#include "mystery_gift_injector.h"
#include "flash_mem.h"
#include "gba_flash.h"
#include "mystery_gift_builder.h"
#include "rom_data.h"

// This will need to be modified for the JP releases
static u8 bill_wonder_card[0x14E] = {
    0x39, 0x2C, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0xD9, 0xD5, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xCC, 0xE3, 0xE9, 0xE8, 0xD9, 0x00, 0xA3, 0xA6, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xD7, 0xDC, 0xD5, 0xE8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xBC, 0xC3, 0xC6, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8, 0xE3, 0x00, 0xDB, 0xD9, 0xE8, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static u8 lanette_wonder_card[0x14E] = {
    0x3F, 0x7C, 0x00, 0x00, 0xBA, 0xB4, 0xBE, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0xCA, 0xCC, 0xC9, 0xC0, 0xBF, 0xCD, 0xCD, 0xC9, 0xCC, 0x00, 0xC0, 0xBF, 0xC8, 0xC8, 0xBF, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0x00, 0xBD, 0xD9, 0xE6, 0xE8, 0xDD, 0xDA, 0xDD, 0xD7, 0xD5, 0xE8, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0xD9, 0xD5, 0xD8, 0x00, 0xE8, 0xE3, 0x00, 0xCC, 0xE3, 0xE9, 0xE8, 0xD9, 0x00, 0xA2, 0xA2, 0xA5, 0x00, 0xD5, 0xE2, 0xD8, 0x00, 0xD7, 0xDC, 0xD5, 0xE8, 0x00, 0xEB, 0xDD, 0xE8, 0xDC, 0x00, 0xC6, 0xBB, 0xC8, 0xBF, 0xCE, 0xCE, 0xBF, 0x00, 0xE8, 0xE3, 0x00, 0xDB, 0xD9, 0xE8, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBE, 0xE3, 0x00, 0xE2, 0xE3, 0xE8, 0x00, 0xE8, 0xE3, 0xE7, 0xE7, 0x00, 0xE8, 0xDC, 0xDD, 0xE7, 0x00, 0xBF, 0xEC, 0xD7, 0xDC, 0xD5, 0xE2, 0xDB, 0xD9, 0x00, 0xBD, 0xD5, 0xE6, 0xD8, 0x00, 0xD6, 0xD9, 0xDA, 0xE3, 0xE6, 0xD9, 0x00, 0x00, 0x00, 0xE6, 0xD9, 0xD7, 0xD9, 0xDD, 0xEA, 0xDD, 0xE2, 0xDB, 0x00, 0xED, 0xE3, 0xE9, 0xE6, 0x00, 0xE8, 0xE6, 0xD5, 0xE2, 0xE7, 0xDA, 0xD9, 0xE6, 0xD9, 0xD8, 0x00, 0xCA, 0xC9, 0xC5, 0x1B, 0xC7, 0xC9, 0xC8, 0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // checksum

bool inject_mystery(Pokemon incoming_party_array[])
{
    bool contains_valid = false;
    for (int i = 0; i < 6; i++)
    {
        contains_valid = contains_valid || incoming_party_array[i].get_validity();
    }
    if (!contains_valid)
    {
        return false;
    }

    mystery_gift_script script;
    script.build_script(incoming_party_array);
    u32 checksum = 0;
    if (curr_rom.is_ruby_sapphire())
    {
        checksum = script.calc_checksum32();
    }
    else
    {
        checksum = script.calc_crc16();
    }

    flash_read(memory_section_array[4], &global_memory_buffer[0], 0x1000);
    if (curr_rom.is_hoenn())
    {
        for (int i = 0; i < 0x14E; i++)
        {
            global_memory_buffer[curr_rom.offset_wondercard + i] = lanette_wonder_card[i];
        }
    }
    else
    {
        for (int i = 0; i < 0x14E; i++)
        {
            global_memory_buffer[curr_rom.offset_wondercard + i] = bill_wonder_card[i];
        }
    }

    // Set checksum and padding
    global_memory_buffer[curr_rom.offset_script] = checksum >> 0;
    global_memory_buffer[curr_rom.offset_script + 1] = checksum >> 8;
    global_memory_buffer[curr_rom.offset_script + 2] = checksum >> 16;
    global_memory_buffer[curr_rom.offset_script + 3] = checksum >> 24;

    // Add in Mystery Script data
    for (int i = 0; i < MG_SCRIPT_SIZE; i++)
    {
        global_memory_buffer[curr_rom.offset_script + 4 + i] = script.get_script_value_at(i);
    }
    update_memory_buffer_checksum();
    flash_write(memory_section_array[4], &global_memory_buffer[0], 0x1000);

    // Set flags
    flash_read(memory_section_array[1 + ((curr_rom.offset_flags + (FLAG_ID_START / 8)) / 0xF80)], &global_memory_buffer[0], 0x1000);
    global_memory_buffer[(curr_rom.offset_flags + (FLAG_ID_START / 8)) % 0xF80] &= (~0b01111111 << (FLAG_ID_START % 8)); // Set "collected all" flag to 0 and reset the "to obtain" flags
    for (int i = 0; i < 6; i++)
    {
        if (incoming_party_array[i].get_validity())
        {
            global_memory_buffer[(curr_rom.offset_flags + (FLAG_ID_START / 8)) % 0xF80] |= ((1 << i) << (FLAG_ID_START % 8)); // Set "to obtain" flags accordingly
        }
    }

    update_memory_buffer_checksum();
    flash_write(memory_section_array[1 + ((curr_rom.offset_flags + (FLAG_ID_START / 8)) / 0xF80)], &global_memory_buffer[0], 0x1000);

    // Update and save custom save data
    for (int i = 0; i < 6; i++)
    {
        if (incoming_party_array[i].get_validity())
        {
            set_caught(incoming_party_array[i].get_dex_number()); // Add the Pokemon to the dream dex
        }
    }
    write_custom_save_data();
    return true;
}