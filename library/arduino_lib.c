#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ARDUINO_DEVICE "/dev/arduino_device"
#define ARDUINO_CUSTOM_CMD 0

void arduino_hello()
{
    int fd = open(ARDUINO_DEVICE, O_RDWR);
    if (fd < 0)
    {
        perror("Error al abrir el archivo de dispositivo");
        return;
    }

    if (ioctl(fd, ARDUINO_CUSTOM_CMD) < 0)
    {
        perror("Error al ejecutar el comando personalizado");
        close(fd);
        return;
    }

    close(fd);
}