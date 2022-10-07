#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <sys/time.h>
#include <errno.h>

void inject_event(int device, __u16 code, __s32 value) {
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

void mouse_click(int device, __u16 code) {
	inject_event(device, code, 1);
	inject_event(device, code, 0);
}

void mouse_down(int device, __u16 code) {
	inject_event(device, code, 1);
}

int main() {
	bool modkey_down = false;
	char kbd_path[] = "/srv/input/kbd0";
	char mouse_path[] = "/srv/input/mouse0";
	int kbd = open(kbd_path, O_RDONLY | O_NONBLOCK);
	int mouse = open(mouse_path, O_RDWR);
	if (kbd < 0 || mouse < 0) {
		printf("Error opening device: %i\n", errno);
		return -1;
	}
	struct input_event event;
	while(1) {
		read(kbd, &event, sizeof(event));
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
				case KEY_7: // mouse left
					if (event.value == 0) {
						mouse_click(mouse, 272);
					}
					break;
				case KEY_8: // mouse right
					if (event.value == 0) {
						mouse_click(mouse, 273);
					}
					break;
				case KEY_Y: // mouse middle
					if (event.value == 0) {
						mouse_click(mouse, 274);
					}
					break;
				case KEY_U: // hold left mouse
					if (event.value == 0) {
						mouse_down(mouse, 272);
					}
					break;
			}
		}
	}
}
