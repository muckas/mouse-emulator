#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

void mouse_down(Display* display, unsigned int button){
	XTestFakeButtonEvent(display, button, True, 0);
	XFlush(display);
}

void mouse_up(Display* display, unsigned int button){
	XTestFakeButtonEvent(display, button, False, 0);
	XFlush(display);
}

void handle_key(Display* display, unsigned int button, unsigned int value, bool modkey_down) {
	switch (value) {
		case 1:
			/* if (modkey_down) { */
				mouse_down(display, button);
			/* } */
			break;
		case 0:
			mouse_up(display, button);
			break;
	}
}

int main() {
	bool modkey_down = false;
	char device_path[] = "/srv/input/kbd0";
	int device = open(device_path, O_NONBLOCK);
	struct input_event event;
	Display *display = XOpenDisplay(NULL);
	while(1) {
		read(device, &event, sizeof(event));
		if (event.type == EV_KEY) {
			switch (event.code) {
				case KEY_CAPSLOCK:
					if (event.value == 1) {
						modkey_down = true;
					} 
					if (event.value == 0) {
						modkey_down = false;
					}
					break;
				case KEY_7:
					handle_key(display, 1, event.value, modkey_down);
					break;
				case KEY_Y:
					handle_key(display, 2, event.value, modkey_down);
					break;
				case KEY_8:
					handle_key(display, 3, event.value, modkey_down);
					break;
			}
		}
	}
}
