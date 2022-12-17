#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <sys/time.h>
#include <argp.h>

const char *argp_program_version = "mouse-emulator v0.1.0";
static char doc[] = "Emulates mouse actions with keyboard keys\n Keycodes defined in linux/input-event-codes.h";
static char args_doc[] = "--input [FILE] --output [FILE]";
static struct argp_option options[] = {
	{"input", 'i', "Input", 0, "Input device (keyboard) path"}, 
	{"output", 'o', "Output", 0, "Output device (mouse) path"}, 
	{"mod", 'M', "ModKey", OPTION_ARG_OPTIONAL, "Keyboard key code for modifier key"}, 
	{"mouseleft", 'l', "MouseLeft", OPTION_ARG_OPTIONAL, "Keyboard key code for LMB"}, 
	{"mouselefthold", 'L', "MouseLeftHold", OPTION_ARG_OPTIONAL, "Keyboard key code for holding LMB"}, 
	{"mouseright", 'r', "MouseRight", OPTION_ARG_OPTIONAL, "Keyboard key code for RMB"}, 
	{"mousemiddle", 'm', "MouseMiddle", OPTION_ARG_OPTIONAL, "Keyboard key code for MMB"}, 
};

struct arguments{
	char *Input;
	char *Output;
	int Mod;
	int MouseLeft;
	int MouseLeftHold;
	int MouseRight;
	int MouseMiddle;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	switch(key){
		case 'i':
			arguments->Input = arg;
			break;
		case 'o':
			arguments->Output = arg;
			break;
		case 'M':
			arguments->Mod = atoi(arg);
			break;
		case 'l':
			arguments->MouseLeft = atoi(arg);
			break;
		case 'L':
			arguments->MouseLeftHold = atoi(arg);
			break;
		case 'r':
			arguments->MouseRight = atoi(arg);
			break;
		case 'm':
			arguments->MouseMiddle = atoi(arg);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

struct argp argp = {options, parse_opt, args_doc, doc};

void inject_event(int device, int code, int value) {
	struct input_event event_key, event_end;

	gettimeofday(&event_key.time, NULL);
	event_key.type = EV_KEY;
	event_key.code = code;
	event_key.value = value;
	write(device, &event_key, sizeof(event_key));

	gettimeofday(&event_end.time, NULL);
	event_end.type = EV_SYN;
	event_end.code = SYN_REPORT;
	event_end.value = 0;
	write(device, &event_end, sizeof(event_end));
}

void mouse_click(int device, int code) {
	inject_event(device, code, 1);
	inject_event(device, code, 0);
}

void mouse_down(int device, int code) {
	inject_event(device, code, 1);
}

int main(int argc, char *argv[]) {
	struct arguments arguments;

	arguments.Input = "";
	arguments.Output = "";
	arguments.Mod = 0;
	arguments.MouseLeft = 0;
	arguments.MouseLeftHold = 0;
	arguments.MouseRight = 0;
	arguments.MouseMiddle = 0;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	bool modkey_down = false;
	int kbd = open(arguments.Input, O_RDONLY | O_NONBLOCK);
	if (kbd < 0) {
		printf("Error opening input device %s\n", arguments.Input);
		exit(EXIT_FAILURE);
	}
	int mouse = open(arguments.Output, O_RDWR);
	if (mouse < 0) {
		printf("Error opening output device %s\n", arguments.Output);
		exit(EXIT_FAILURE);
	}
	struct input_event event;
	while(1) {
		read(kbd, &event, sizeof(event));
		if (event.type == EV_KEY) {
			if (event.code == arguments.Mod) {
				if (event.value == 1) {
					modkey_down = true;
				} 
				if (event.value == 0) {
					modkey_down = false;
				}
			} else if (event.code == arguments.MouseLeft) {
				if (event.value == 0 && (modkey_down || arguments.Mod == 0)) { 
					mouse_click(mouse, 272);
				}
			} else if (event.code == arguments.MouseLeftHold) {
				if (event.value == 0 && (modkey_down || arguments.Mod == 0)) { 
					mouse_down(mouse, 272);
				}
			} else if (event.code == arguments.MouseRight) {
				if (event.value == 0 && (modkey_down || arguments.Mod == 0)) { 
					mouse_click(mouse, 273);
				}
			} else if (event.code == arguments.MouseMiddle) {
				if (event.value == 0 && (modkey_down || arguments.Mod == 0)) { 
					mouse_click(mouse, 274);
				}
			}
		}
		usleep(8);
	}
}
