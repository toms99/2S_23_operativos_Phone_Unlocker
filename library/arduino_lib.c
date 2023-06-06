#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

// Función para configurar la comunicación serie
static int setupSerialPort(const char *port, speed_t baudRate)
{
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("Error al abrir el puerto serie");
        exit(1);
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

// Función para enviar un byte por el puerto serie
static void serialWrite(int fd, char byte)
{
    write(fd, &byte, 1);
}

// Función para enviar un mensaje según el modo seleccionado
static void enviarMensaje(int fd, int modo)
{
    char mensaje[10];
    switch (modo)
    {
    case 1:
        sprintf(mensaje, "100");
        break;
    case 2:
        sprintf(mensaje, "500");
        break;
    case 3:
        sprintf(mensaje, "1000");
        break;
    default:
        sprintf(mensaje, "2000");
    }
    printf("escribiendo el mensaje modo: %d\n", modo);

    // Enviar cada byte del mensaje
    for (int i = 0; i < strlen(mensaje); i++)
    {

        write(fd, &mensaje[i], 1);
        usleep(1000); // Retardo en microsegundos entre cada byte
    }
}

void arduino_hello(int modo)
{

    const char *port = "/dev/ttyACM0"; // Puerto serie
    speed_t baudRate = B9600;          // Baud rate

    // Configurar el puerto serie
    int fd = setupSerialPort(port, baudRate);

    // Enviar el mensaje según el modo seleccionado
    enviarMensaje(fd, modo);

    // Cerrar el puerto serie
    close(fd);
}