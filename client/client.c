#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void encryptNumber(char *number, const char *key)
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

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        printf("Error: Se requieren dos parámetros.\n");
        printf("Uso: %s <número> <clave>\n", argv[0]);
        return 1;
    }

    char *number = argv[1];
    const char *modo = argv[2];

    if (strlen(number) > 9)
    {
        printf("Error: El número debe tener hasta 9 dígitos.\n");
        return 1;
    }

    if (strlen(modo) == 0)
    {
        printf("Error: La clave no puede estar vacía.\n");
        return 1;
    }

    char *key = getEncryptionKey(key_path);

    encryptNumber(number, key);

    printf("Número cifrado: %s\n", number);

    // Crear el objeto JSON
    json_t *root = json_object();
    json_object_set_new(root, "number", json_string(number));
    json_object_set_new(root, "mode", json_string(modo));

    // Convertir el objeto JSON a una cadena
    char *jsonStr = json_dumps(root, JSON_COMPACT);
    printf("JSON enviado: %s\n", jsonStr);

    // Crear socket UDP
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // Configurar dirección del servidor
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(SERVER_PORT);

    // Enviar el JSON al servidor
    ssize_t numBytes = sendto(sockfd, jsonStr, strlen(jsonStr), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (numBytes == -1)
    {
        perror("Error al enviar datos");
        exit(1);
    }

    // Liberar recursos
    json_decref(root);
    close(sockfd);

    return 0;
}