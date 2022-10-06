#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main() {
	char device_path[] = "/srv/input/kbd0";
	int device = open(device_path, O_RDONLY);
	struct input_event event;
	while(1) {
		read(device, &event, sizeof(event));
		if (event.type == 1 && event.value == 1) {
			printf("Key: %i State: %i\n", event.code, event.value);
		}
	}
}
