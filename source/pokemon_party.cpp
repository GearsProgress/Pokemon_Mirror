#include "gameboy_colour.h"
#include "pokemon_party.h"
#include "pokemon.h"
#include "flash_mem.h"
#include "debug_mode.h"
#include "mystery_gift_injector.h"
#include "gb_rom_values/base_gb_rom_struct.h"
#include "sprite_data.h"
#include "box_menu.h"
#include "payload_builder.h"

const GB_ROM *list_of_gb_roms[NUM_PAYLOADS] = {
        &ENG_RED_BLUE,
		&ENG_YELLOW,
		&ENG_GOLD,
		&ENG_SILVER,
		&ENG_CRYSTAL,
    };

byte gen1_debug_box_data[0x462] = {
	// Num of Pokemon
	0x14,
	// Pokemon 1-20
	//0x02, 0x70, 0x70, 0x70, 0x71, 0x71, 0x70, 0x71, 0x7B, 0x71, 0x71, 0x7C, 0x71, 0x71, 0x71, 0x71, 0x24, 0x24, 0xA5, 0x15, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0xFF,
	// Pokemon list
	0x02, 0x00, 0x11, 100, 0x00, 0x07, 0x03, 0xFF, 0x28, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0xDE, 0x23, 0x28, 0x00, 0x00, 
	0x70, 0x00, 0x12, 0x04, 0x00, 0x07, 0x03, 0xFF, 0x28, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5D, 0xD8, 0x23, 0x28, 0x00, 0x00, 
	0x70, 0x00, 0x12, 0x04, 0x00, 0x07, 0x03, 0xFF, 0x28, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x13, 0x23, 0x28, 0x00, 0x00, 
	0x70, 0x00, 0x0F, 0x03, 0x00, 0x07, 0x03, 0xFF, 0x28, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDA, 0x81, 0x23, 0x28, 0x00, 0x00, 
	0x71, 0x00, 0x14, 0x05, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD6, 0x80, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x15, 0x05, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7, 0x95, 0x1E, 0x00, 0x00, 0x00, 
	0x70, 0x00, 0x13, 0x05, 0x00, 0x07, 0x03, 0xFF, 0x28, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC9, 0xCC, 0x23, 0x28, 0x00, 0x00, 
	0x71, 0x00, 0x11, 0x04, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0x7A, 0x1E, 0x00, 0x00, 0x00, 
	0x7B, 0x00, 0x10, 0x03, 0x00, 0x07, 0x07, 0xFF, 0x21, 0x51, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5C, 0x84, 0x23, 0x28, 0x00, 0x00, 
	0x71, 0x00, 0x15, 0x06, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAC, 0x64, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x13, 0x05, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4, 0x1D, 0x1E, 0x00, 0x00, 0x00, 
	0x7C, 0x00, 0x12, 0x04, 0x00, 0x07, 0x07, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x31, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x12, 0x04, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0x40, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x16, 0x06, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0xAA, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x12, 0x04, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0xE9, 0x1E, 0x00, 0x00, 0x00, 
	0x71, 0x00, 0x14, 0x05, 0x00, 0x07, 0x03, 0x78, 0x6A, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB, 0xEC, 0x1E, 0x00, 0x00, 0x00, 
	0x24, 0x00, 0x12, 0x04, 0x00, 0x00, 0x02, 0xFF, 0x10, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0xC2, 0x23, 0x00, 0x00, 0x00, 
	0x24, 0x00, 0x11, 0x04, 0x00, 0x00, 0x02, 0xFF, 0x10, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D, 0x2A, 0x23, 0x00, 0x00, 0x00, 
	0xA5, 0x00, 0x0E, 0x03, 0x00, 0x00, 0x00, 0xFF, 0x21, 0x27, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0xBE, 0x23, 0x1E, 0x00, 0x00, 
	0x15, 0x00, 0x0F, 0x03, 0x00, 0x00, 0x02, 0xFF, 0x10, 0x00, 0x00, 0x00, 0x6F, 0xC6, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0xE6, 0x23, 0x00, 0x00, 0x00,
	// Trainer names 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x80, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	// Nicknames
	0x96, 0x84, 0x84, 0x83, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x96, 0x84, 0x84, 0x83, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x96, 0x84, 0x84, 0x83, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x96, 0x84, 0x84, 0x83, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x96, 0x84, 0x84, 0x83, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x82, 0x80, 0x93, 0x84, 0x91, 0x8F, 0x88, 0x84, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8C, 0x84, 0x93, 0x80, 0x8F, 0x8E, 0x83, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8A, 0x80, 0x8A, 0x94, 0x8D, 0x80, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8F, 0x88, 0x83, 0x86, 0x84, 0x98, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x8F, 0x88, 0x83, 0x86, 0x84, 0x98, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x91, 0x80, 0x93, 0x93, 0x80, 0x93, 0x80, 0x50, 0x50, 0x50, 0x50, 
	0x8F, 0x88, 0x83, 0x86, 0x84, 0x98, 0x50, 0x50, 0x50, 0x50, 0x50,
	};
byte gen2_debug_box_data[0x44E] = {
	// Num of Pokemon
	0x14, 
	// Pokemon 1-20
	0xD5, 0x7B, 0xE3, 0x9D, 0xC3, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xD5, 0xFF, 
	// Pokemon list
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0x7B, 0x00, 0x62, 0x2B, 0x74, 0xE4, 0xBB, 0x4C, 0x00, 0x08, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBA, 0xF9, 0x1E, 0x1E, 0x1E, 0x14, 0x46, 0x00, 0x00, 0x00, 0x0D, 
	0xE3, 0x00, 0x13, 0xA8, 0x81, 0x61, 0xBB, 0x4C, 0x01, 0xC5, 0x71, 0x17, 0x99, 0x1D, 0x33, 0x1B, 0x77, 0x19, 0x7F, 0x18, 0xB9, 0xDF, 0x87, 0x0F, 0x0A, 0x14, 0x1E, 0xFF, 0x00, 0x00, 0x00, 0x2D, 
	0x9D, 0x00, 0xAC, 0x09, 0x59, 0x46, 0xBB, 0x4C, 0x01, 0xAA, 0xDE, 0x23, 0xE8, 0x2C, 0xCF, 0x29, 0x00, 0x2C, 0xD4, 0x28, 0x55, 0xF6, 0xD7, 0x19, 0x0F, 0x0A, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x30, 
	0xC3, 0x1B, 0xD6, 0x27, 0x15, 0x37, 0xBB, 0x4C, 0x00, 0x1F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x10, 0x0A, 0x1E, 0x14, 0x19, 0x4C, 0x00, 0x00, 0x00, 0x14, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	0xD5, 0x00, 0x84, 0x6E, 0x23, 0xE3, 0xBB, 0x4C, 0x00, 0x0A, 0x4E, 0x00, 0x20, 0x00, 0x32, 0x00, 0x23, 0x00, 0x34, 0x00, 0x28, 0x9C, 0x90, 0x23, 0x28, 0x14, 0x05, 0x58, 0x00, 0x00, 0x00, 0x10, 
	// Trainer names
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	0x80, 0x94, 0x92, 0x93, 0x88, 0x8D, 0x50, 0x50, 0x00, 0x00, 0x00, 
	// Nicknames
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x82, 0x98, 0x93, 0x87, 0x84, 0x91, 0x50, 0x50, 0x50, 0x50, 
	0x82, 0x80, 0x8C, 0x84, 0x8B, 0x8E, 0x93, 0x50, 0x50, 0x50, 0x50, 
	0x93, 0x88, 0x8C, 0x8D, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 
	0x90, 0xF3, 0x80, 0x9B, 0xE6, 0x9C, 0x91, 0x9E, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50, 
	0x92, 0x87, 0x94, 0x82, 0x8A, 0x8B, 0x84, 0x50, 0x50, 0x50, 0x50
	};

Pokemon_Party::Pokemon_Party(){};

void Pokemon_Party::start_link()
{
	if (IGNORE_LINK_CABLE)
	{
		for (int i = 0; i < 0x44E; i++)
		{
			box_data_array[i] = gen2_debug_box_data[i];
		}
	}
	else
	{
		setup();
		last_error = loop(&box_data_array[0], generate_payload(curr_gb_rom, false), &curr_gb_rom, simple_pkmn_array);
	}
}

void Pokemon_Party::continue_link(){
	if (!IGNORE_LINK_CABLE){
		last_error = loop(&box_data_array[0], generate_payload(curr_gb_rom, false), &curr_gb_rom, simple_pkmn_array);
	}
}

int Pokemon_Party::get_last_error()
{
	return last_error;
}

Pokemon Pokemon_Party::get_converted_pkmn(int index)
{
	Pokemon converted_mon;
	converted_mon.load_data(index, box_data_array, game, lang);
	converted_mon.convert_to_gen_three(false, stabilize_mythic);
	has_new_pkmn = has_new_pkmn || converted_mon.get_is_new();
	simple_pkmn_array[index] = converted_mon.get_simple_pkmn();
	return converted_mon;
}

bool Pokemon_Party::get_has_new_pkmn() // If Pokemon is not in the dex
{
	return has_new_pkmn;
}

bool Pokemon_Party::get_contains_mythical(){
	return contains_mythical;
}

void Pokemon_Party::set_mythic_stabilization(bool stabilize){
	stabilize_mythic = stabilize;
}


void Pokemon_Party::set_game(int nGame)
{
	game = nGame;
}

void Pokemon_Party::set_lang(int nLang)
{
	lang = nLang;
}

int Pokemon_Party::get_lang(){
	return lang;
}

bool Pokemon_Party::load_payload(){
	for (int i = 0; i < NUM_PAYLOADS; i++)
	{
		if (lang == list_of_gb_roms[i]->language &&
			game == list_of_gb_roms[i]->version)
		{
			curr_gb_rom = *list_of_gb_roms[i];
			return true;
		}
	}  
	return false;
}

Simplified_Pokemon Pokemon_Party::get_simple_pkmn(int index){
	return simple_pkmn_array[index];
}

void Pokemon_Party::fill_simple_pkmn_array(){
	for (int index = 0; index < 30; index++){
		Pokemon converted_mon;
		converted_mon.load_data(index, box_data_array, game, lang);
		converted_mon.convert_to_gen_three(true, stabilize_mythic);
		has_new_pkmn = has_new_pkmn || converted_mon.get_is_new();
		contains_mythical = contains_mythical || 
			converted_mon.get_dex_number() == 151 || converted_mon.get_dex_number() == 251;
		simple_pkmn_array[index] = converted_mon.get_simple_pkmn();
	}
}