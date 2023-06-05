#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    const char *fqbn = "arduino:avr:uno";

    // Comando de compilación con arduino-cli para obtener el archivo .hex
    char compileCommand[256];
    snprintf(compileCommand, sizeof(compileCommand), "arduino-cli compile --fqbn %s --output-dir . .", fqbn);

    // Ejecutar el comando de compilación
    int compileResult = system(compileCommand);
    if (compileResult != 0)
    {
        printf("Error al compilar el archivo .ino\n");
        return 1;
    }

    // Ruta al archivo .hex generado por arduino-cli
    const char *hexFile = "tests.ino.hex";

    // Comando avrdude para cargar el archivo .hex en el Arduino Uno
    char uploadCommand[256];
    snprintf(uploadCommand, sizeof(uploadCommand), "avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:%s:i", hexFile);

    // Ejecutar el comando avrdude
    int uploadResult = system(uploadCommand);
    if (uploadResult != 0)
    {
        printf("Error al cargar el archivo .hex en el Arduino Uno\n");
        return 1;
    }

    return 0;
}
