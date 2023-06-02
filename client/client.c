#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    encryptNumber(number, key);

    printf("Número descifrado: %s\n", number);

    return 0;
}