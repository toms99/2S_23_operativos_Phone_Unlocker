#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arduino_lib.h>

#define BUFFER_SIZE 1024
#define PORT 8080

void decryptNumber(char *number, const char *key)
{
    size_t numberLen = strlen(number);
    size_t keyLen = strlen(key);

    for (size_t i = 0; i < numberLen; i++)
    {
        number[i] = ((number[i] - '0') ^ key[i % keyLen]) + '0';
    }
}

char *getEncryptionKey(const char *key_path)
{
    FILE *keyFilePtr = fopen(key_path, "r");
    if (keyFilePtr == NULL)
    {
        printf("Error: No se pudo abrir el archivo de clave.\n");
        return NULL;
    }

    char key[100];
    if (fgets(key, sizeof(key), keyFilePtr) == NULL)
    {
        printf("Error: No se pudo leer la clave del archivo.\n");
        fclose(keyFilePtr);
        return NULL;
    }

    fclose(keyFilePtr);

    size_t keyLen = strlen(key);
    if (key[keyLen - 1] == '\n')
    {
        key[keyLen - 1] = '\0'; // Eliminar el salto de línea si se leyó
    }

    char *encryptedKey = malloc((keyLen + 1) * sizeof(char));
    if (encryptedKey == NULL)
    {
        printf("Error: No se pudo asignar memoria para la clave encriptada.\n");
        return NULL;
    }

    strcpy(encryptedKey, key);

    return encryptedKey;
}

char *key_path = "util/key.txt";

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];

    // Crear socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // Configurar dirección del servidor
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Vincular el socket a la dirección del servidor
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("Error al vincular el socket");
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d\n", PORT);

    while (1)
    {
        socklen_t len = sizeof(cliaddr);

        // Esperar la recepción de datos del cliente
        ssize_t numBytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (numBytes == -1)
        {
            perror("Error al recibir datos");
            exit(1);
        }

        // Deserializar el objeto JSON recibido
        json_error_t error;
        json_t *root = json_loads(buffer, 0, &error);
        if (!root)
        {
            fprintf(stderr, "Error al deserializar JSON: %s\n", error.text);
            exit(1);
        }

        // Obtener los valores del objeto JSON
        const char *number = json_string_value(json_object_get(root, "number"));
        int mode = json_integer_value(json_object_get(root, "mode"));

        // Copiar el número en un nuevo arreglo de caracteres
        char decripted_number[strlen(number) + 1];
        strcpy(decripted_number, number);

        // Crear tubería para comunicación entre procesos
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("Error al crear la tubería");
            exit(1);
        }

        // Crear proceso hijo
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("Error al crear proceso hijo");
            exit(1);
        }
        else if (pid == 0)
        {
            // Proceso hijo

            // Cerrar el extremo de escritura de la tubería
            close(pipefd[0]);

            // Obtener la clave de encriptación
            char *key = getEncryptionKey(key_path);

            // Enviar la clave al proceso padre a través de la tubería
            write(pipefd[1], key, strlen(key) + 1);

            // Cerrar el extremo de lectura de la tubería
            close(pipefd[1]);

            // Salir del proceso hijo
            exit(0);
        }
        else
        {
            // Proceso padre

            // Cerrar el extremo de escritura de la tubería
            close(pipefd[1]);

            // Leer la clave enviada por el proceso hijo desde la tubería
            char key[100];
            read(pipefd[0], key, sizeof(key));

            // Cerrar el extremo de lectura de la tubería
            close(pipefd[0]);

            // Esperar a que el proceso hijo termine
            int status;
            waitpid(pid, &status, 0);

            // Descifrar el número
            decryptNumber(decripted_number, key);

            printf("Número descifrado: %s\n", decripted_number);
            printf("Modo: %d\n", mode);

            arduino_hello();

            // Liberar memoria
            json_decref(root);
        }
    }
    // Cerrar el socket
    close(sockfd);

    return 0;
}
