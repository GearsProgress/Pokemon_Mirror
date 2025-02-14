#include <tonc.h>
#include <string>
#include <cstring>
// #include <maxmod.h> //Music

#include "flash_mem.h"
#include "interrupt.h"
#include "gb_link.h"
#include "gameboy_colour.h"
#include "pokemon.h"
#include "random.h"
#include "text_engine.h"
#include "background_engine.h"
#include "pokemon_party.h"
#include "script_array.h"
#include "sprite_data.h"
#include "button_handler.h"
#include "button_menu.h"
#include "debug_mode.h"
// #include "soundbank.h"
// #include "soundbank_bin.h"
#include "dex_handler.h"
#include "pokedex.h"
#include "global_frame_controller.h"
#include "pkmn_font.h"
#include "save_data_manager.h"
#include "mystery_gift_injector.h"
#include "mystery_gift_builder.h"
#include "rom_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"

/*

TODO:
- Add in putting item from Pkmn to bag in gen 2
- Determine if transfered Shiny Pokemon are square/star sparkles
- Music and sound effects
- Wii Channel
- Events
- MissingNo/Enigma Berry
- Text translations
- Add support for other languages
- Add in dolls for gen 2/3
- Doxygen generation
--------
*/

int delay_counter = 0;
int curr_selection = 0;
bool skip = true;
rom_data curr_rom;
Button_Menu yes_no_menu(1, 2, 40, 24, false);

/*
int test_main(void) Music
{

	irq_init(NULL);
	// Initialize maxmod with default settings
	// pass soundbank address, and allocate 8 channels.

	irq_set(II_VBLANK, mmVBlank, 0);
	irq_enable(II_VBLANK);

	mmInitDefault((mm_addr)soundbank_bin, 8);

	mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP);
	// Song is playing now (well... almost)
	while (1)
	{
		// ..process game logic..

		// Update Maxmod
		mmFrame();

		// Wait for new frame (SWI 5)
		VBlankIntrWait();

		// ..update graphical data..
	}
}
*/

// (R + G*32 + B*1024)
#define RGB(r, g, b) (r + (g * 32) + (b * 1024))

template <typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
	static const char *digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}

void load_graphics()
{
	tte_erase_rect(0, 0, H_MAX, V_MAX);
	// Load opening background first so it hides everything else
	load_flex_background(BG_OPENING, 1);
	load_background();
	load_textbox_background();
	load_eternal_sprites();

	// Set up global yes no button
	yes_no_menu.clear_vector();
	yes_no_menu.add_button(Button(button_yes), true);
	yes_no_menu.add_button(Button(button_no), false);
}

void initalization_script(void)
{
	// Initalizations
	REG_DISPCNT = DCNT_BLANK | DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_BG2 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_1D;
	irq_init(NULL);
	irq_enable(II_VBLANK);

	// Disable for save data read/write
	REG_IME = 0;
	REG_IE = 0;

	// Sound bank init
	irq_init(NULL);
	irq_enable(II_VBLANK);
	// irq_set(II_VBLANK, mmVBlank, 0); //Music
	// mmInitDefault((mm_addr)soundbank_bin, 8); //Music
	// mmStart(MOD_FLATOUTLIES, MM_PLAY_LOOP); //Music

	// Graphics init
	oam_init(obj_buffer, 128);
	load_graphics();

	// Prepare dialouge
	populate_script();
	init_text_engine();

	// Set the random seed
	rand_set_seed(0x1216);

	VBlankIntrWait();
	REG_DISPCNT &= ~DCNT_BLANK;
};

void game_load_error(void)
{
	// EG_BG0CNT = (REG_BG0CNT & ~BG_PRIO_MASK) | BG_PRIO(3);
	// REG_BG1CNT = (REG_BG1CNT & ~BG_PRIO_MASK) | BG_PRIO(2);
	REG_BG2CNT = (REG_BG2CNT & ~BG_PRIO_MASK) | BG_PRIO(1);
	REG_BG2VOFS = 0;
	tte_set_pos(40, 24);
	tte_set_margins(40, 24, 206, 104);
	set_textbox_large();
	tte_write("#{cx:0xF000}The Pok@mon save\nfile was not loaded successfully.\n\nPlease remove and\nreinsert the Game\nPak, and then press the A button.");
	key_poll();
	while (!key_hit(KEY_A))
	{
		global_next_frame();
	}
	tte_erase_screen();
	delay_counter = 0;

	while (delay_counter < 60)
	{
		delay_counter++;
		global_next_frame();
	}
}

void first_load_message(void)
{
	tte_set_pos(8, 0);
	tte_set_ink(10);
	tte_write("#{cx:0xD000}\n\nHello! Thank you for using\nPok@ Transporter GB!\n\nJust as a word of caution- \nPok@ Transporter GB WILL\nmodify both the GameBoy and GameBoy Advance save files.\n\nPlease note that Pok@\nTransporter GB is still in\nbeta, so save file backups\nare HIGHLY recommended\nbefore using. With that all\nbeing said, please enjoy!\n\n      -The Gears of Progress");
	while (!key_hit(KEY_A))
	{
		global_next_frame();
	}
	tte_erase_rect(0, 0, H_MAX, V_MAX);
}

int credits()
{
#define CREDITS_ARRAY_SIZE 19
	int curr_credits_num = 0;
	std::string credits_array[CREDITS_ARRAY_SIZE] = {
		//"testing: う",
		"Lead developer:\n\nThe Gears of\nProgress",
		"Logo and co-ideator:\n\n-Jome\n\nSpritework:\n\n-LJ Birdman\n\n",
		"Icon Sprites: \n\n-LuigiTKO\n-GuiAbel\n-SourApple\n & the artists from\nPok@mon Showdown and\nCrystal Clear",
		"Remote and Arbitrary\nCode Execution\nassistance:\n\n\n-TimoVM",
		"Development\nassistance:\n\n-im a blisy\n-rileyk64\n-Shao",
		"Space Optimization:\n\n-easyaspi314",
		"Built using:\n\n\n-DevkitPro\n-LibTonc\n-LibGBA",
		"Inspired by the\nworks of:\n\n-Goppier\n-Lorenzooone\n-im a blisy\n-RETIRE",
		"Programs used:\n\n\n-HexManiacAdvance\n-PKHeX\n-WC3Tool\n-Usenti\n-SappyMidToAGB",
		"Open Source Code and\nLibraries:\n\n-libtonc-examples\n-PokemonGen3toGenX\n-gba-link-connection\n-awesome-gbadev\n-arduino-poke-gen2",
		"Research resources:\n\n-arm-docs\n-PokemonGen3toGenX\n\nFull links can be\nfound on this\nprogram's GitHub",
		"ROM data obtained\nfrom decompilations created by the PRET team",
		"Pok@mon data\nobtained from:\n\n-Bulbapedia\n-Serebii\n-PokeAPI.com",
		"Discord community\nassistance:\n\n-Hex Maniac Advance\n Development\n-gbadev\n-pret",
		"Writing assistance:\n\n\n-Mad",
		"An immense thanks to\nLorenzooone for\ntheir assistance in\nreading/writing save\ndata. Without them,\nthis project would\nnot have been\npossible.",
		"Special thanks to\nEnn, roku, Sleepy,\nEza, sarahtonin,\nBasabi, Mad, and\neveryone who has\nlistened to me talk\nabout this for\nmonths!",
		"All Pok@mon names,\nsprites, and names\nof related resources\nare copyright\nNintendo,\nCreatures Inc.,\nand GAME FREAK Inc.",
		"This project is not endorsed or\nsupported by\nGameFreak/Nintendo.\n\nPlease support the\noriginal developers.",
	};

	while (true)
	{
		set_textbox_large();
		tte_write(credits_array[curr_credits_num].c_str());

		if (key_hit(KEY_B))
		{
			hide_text_box();
			set_textbox_small();
			return 0;
		}
		if (key_hit(KEY_LEFT) && curr_credits_num > 0)
		{
			curr_credits_num--;
		}
		if (key_hit(KEY_RIGHT) && curr_credits_num < (CREDITS_ARRAY_SIZE - 1))
		{
			curr_credits_num++;
		}
		if (ENABLE_DEBUG_SCREEN && key_hit(KEY_SELECT))
		{
			if (key_held(KEY_UP) && key_held(KEY_L) && key_held(KEY_R))
			{
				set_treecko(true);
			}
			u32 pkmn_flags = 0;
			bool e4_flag = read_flag(curr_rom.e4_flag);
			bool mg_flag = read_flag(curr_rom.mg_flag);
			bool all_collected_flag = read_flag(curr_rom.all_collected_flag);
			for (int i = 0; i < 30; i++)
			{
				pkmn_flags |= (read_flag(curr_rom.pkmn_collected_flag_start + i) << i);
			}

			bool tutorial = get_tutorial_flag();
			int def_lang = get_def_lang_num();

			set_textbox_large();
			tte_write("Debug info:\n\nG: ");
			std::string lang;
			lang += curr_rom.language;
			tte_write(lang.c_str());
			switch (curr_rom.gamecode)
			{
			case RUBY_ID:
				tte_write("-R-");
				break;
			case SAPPHIRE_ID:
				tte_write("-S-");
				break;
			case FIRERED_ID:
				tte_write("-F-");
				break;
			case LEAFGREEN_ID:
				tte_write("-L-");
				break;
			case EMERALD_ID:
				tte_write("-E-");
				break;
			}
			tte_write(std::to_string(curr_rom.version).c_str());

			tte_write("\nF: ");
			tte_write(std::to_string(e4_flag).c_str());
			tte_write(std::to_string(mg_flag).c_str());
			tte_write(std::to_string(all_collected_flag).c_str());
			tte_write("-");
			tte_write((n2hexstr(pkmn_flags)).c_str());
			tte_write("\nS:   ");
			tte_write(std::to_string(tutorial).c_str());
			tte_write("-");
			tte_write((n2hexstr(def_lang)).c_str());

			tte_write("\n\n");
			tte_write(VERSION);
			if (get_treecko_enabled())
			{
				tte_write(".T");
			}
			while (true)
			{
				if (key_hit(KEY_B))
				{
					hide_text_box();
					set_textbox_small();
					return 0;
				}
				global_next_frame();
			}
		}
		global_next_frame();
	}
};

#define NUM_MENU_OPTIONS 3

int main_menu_loop()
{
	bool update = true;
	std::string_view menu_options[NUM_MENU_OPTIONS] = {"Transfer Pok@mon", "Dream Dex", "Credits"};
	int return_values[NUM_MENU_OPTIONS] = {BTN_TRANSFER, BTN_POKEDEX, BTN_CREDITS};
	while (true)
	{
		if (update)
		{
			for (int i = 0; i < NUM_MENU_OPTIONS; i++)
			{
				int x = (6 + ((18 - menu_options[i].length()) / 2)) * 8;
				tte_set_pos(x, ((i * 2) + 10) * 8);
				if (i == curr_selection)
				{
					tte_write("#{cx:0xD000}");
				}
				else
				{
					tte_write("#{cx:0xE000}");
				}
				tte_write(menu_options[i].data());
			}
		}

		update = true;
		if (key_hit(KEY_DOWN))
		{
			curr_selection = ((curr_selection + 1) % NUM_MENU_OPTIONS);
		}
		else if (key_hit(KEY_UP))
		{
			curr_selection = ((curr_selection + (NUM_MENU_OPTIONS - 1)) % NUM_MENU_OPTIONS);
		}
		else if (key_hit(KEY_A))
		{
			tte_erase_rect(0, 0, H_MAX, V_MAX);
			tte_write("#{cx:0xF000}");
			return return_values[curr_selection];
		}
		else
		{
			update = false;
		}

		global_next_frame();
	}
}

int main(void)
{
	initalization_script();

	// Set colors based on current ROM
	set_background_pal(0, false, false);

	// Legal mumbo jumbo
	tte_set_pos(8, 0);
	tte_write("#{cx:0xE000}\n\nPok@ Transporter GB was made\nout of love and appreciation\nfor the Pokemon franchise\nwith no profit in mind.\nIt will ALWAYS be free.\n\nPlease support the original developers-\nNintendo and GAME FREAK.\n\nAll Pokemon names, sprites, and music are owned by \nNintendo, Creatures Inc, and\nGAME FREAK Inc.\n\n\n         Press A to continue");
	tte_write("#{cx:0xF000}"); // Set the color to grey
	bool wait = true;
	while (wait)
	{
		global_next_frame();
		if (key_hit(KEY_A))
		{
			wait = false;
		}
	}

	// Gears of Progress
	tte_erase_rect(0, 0, 240, 160);
	REG_BG1VOFS = 0;
	delay_counter = 0;
	while (delay_counter < (15 * 60))
	{
		global_next_frame();
		delay_counter++;
		if (key_hit(KEY_A))
		{
			delay_counter = (15 * 60);
		}
	}
	REG_BG1CNT = REG_BG1CNT | BG_PRIO(3);

	key_poll(); // Reset the keys
	curr_rom.load_rom();

	obj_set_pos(ptgb_logo_l, 56, 12);
	obj_set_pos(ptgb_logo_r, 56 + 64, 12);
	obj_unhide_multi(ptgb_logo_l, 1, 2);
	bool start_pressed = false;
	REG_BLDCNT = BLD_BUILD(BLD_BG3, BLD_BG0, 1);
	tte_set_pos(6 * 8, 12 * 8);
	tte_write("#{cx:0xF000}Push Start Button!");
	int fade = 0;
	while (!start_pressed)
	{
		fade = abs(((get_frame_count() / 6) % 24) - 12);
		global_next_frame();
		start_pressed = key_hit(KEY_START) | key_hit(KEY_A);
		REG_BLDALPHA = BLDA_BUILD(0b10000, fade);
	};
	key_poll();
	tte_erase_rect(0, 0, H_MAX, V_MAX);

	//  Check if the game has been loaded correctly.
	while (!curr_rom.load_rom())
	{
		obj_hide_multi(ptgb_logo_l, 2);
		game_load_error();
		// initalization_script();
	}

	// Initalize memory and save data after loading the game
	set_textbox_small();
	REG_BG2CNT = REG_BG2CNT | BG_PRIO(3);
	init_bank();
	initalize_memory_locations();
	load_custom_save_data();

	set_background_pal(curr_rom.gamecode, false, true);

	if (!IGNORE_MG_E4_FLAGS && (!get_tutorial_flag() || FORCE_TUTORIAL))
	{
		obj_hide_multi(ptgb_logo_l, 2);
		text_loop(BTN_TRANSFER);
		initalize_save_data();
	}

	obj_unhide_multi(ptgb_logo_l, 1, 2);

	// MAIN LOOP
	while (true)
	{
		if (DEBUG_MODE)
		{
			print_mem_section();
			curr_rom.print_rom_info();
		}
		load_flex_background(BG_MAIN_MENU, 2);

		obj_unhide_multi(ptgb_logo_l, 1, 2);
		obj_set_pos(ptgb_logo_l, 56, 12);
		obj_set_pos(ptgb_logo_r, 56 + 64, 12);

		switch (main_menu_loop())
		{
		case (BTN_TRANSFER):
			obj_hide_multi(ptgb_logo_l, 2);
			load_flex_background(BG_FENNEL, 2);
			text_loop(BTN_TRANSFER);
			break;
		case (BTN_POKEDEX):
			load_flex_background(BG_DEX, 2);
			set_background_pal(curr_rom.gamecode, true, false);
			obj_hide_multi(ptgb_logo_l, 2);
			pokedex_loop();
			load_flex_background(BG_DEX, 3);
			set_background_pal(curr_rom.gamecode, false, false);
			break;
		case (BTN_CREDITS):
			set_textbox_large();
			show_text_box();
			REG_BG1CNT = (REG_BG1CNT & ~BG_PRIO_MASK) | BG_PRIO(3);
			obj_set_pos(ptgb_logo_l, 56, 108);
			obj_set_pos(ptgb_logo_r, 56 + 64, 108);
			credits();
			break;
		case (BTN_EVENTS):
			obj_hide_multi(ptgb_logo_l, 2);
			text_loop(BTN_EVENTS);
			break;
		default:
			global_next_frame();
		}
	}
}