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
static void enviarMensaje(int fd, char *mensaje)
{

    // Enviar cada byte del mensaje
    for (int i = 0; i < strlen(mensaje); i++)
    {

        write(fd, &mensaje[i], 1);
        usleep(1000); // Retardo en microsegundos entre cada byte
    }
}

static void agregarComas(char *cadena)
{
    size_t longitud = strlen(cadena);
    char *resultado = (char *)malloc(2 * longitud); // Asignar memoria para el resultado
    int indice = 0;

    for (size_t i = 0; i < longitud; i++)
    {
        resultado[indice++] = cadena[i];
        if (i < longitud - 1)
        {
            resultado[indice++] = ',';
        }
    }

    resultado[indice] = '\0'; // Agregar el carácter nulo al final de la cadena

    strcpy(cadena, resultado); // Copiar el resultado de vuelta a la variable original

    free(resultado); // Liberar la memoria asignada
}

void unlocker_conn(int modo, char *decripted_number)
{

    const char *port = "/dev/ttyUSB0"; // Puerto serie

    speed_t baudRate = B9600; // Baud rate
    char message[strlen(decripted_number) * 2 + 8];
    agregarComas(decripted_number);

    if (modo == 1)
    {
        strcpy(message, "500,");
    }
    else if (modo == 2)
    {
        strcpy(message, "501,");
    }
    else
    {
        strcpy(message, "502,");
    }

    strcat(message, decripted_number);
    strcat(message, ",%");
    printf("%s\n", message);

    // Configurar el puerto serie
    int fd = setupSerialPort(port, baudRate);

    // Enviar el mensaje según el modo seleccionado
    enviarMensaje(fd, message);

    // Cerrar el puerto serie
    close(fd);
}