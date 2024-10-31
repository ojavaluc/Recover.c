#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Verifica o uso correto
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover IMAGE\n");
        return 1;
    }

    // Abre o arquivo de entrada
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    FILE *output = NULL;
    char filename[8];
    int file_count = 0;

    // Percorre os blocos de 512 bytes
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // Verifica a assinatura do JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Se já estiver escrevendo uma imagem, fecha o arquivo atual
            if (output != NULL)
            {
                fclose(output);
            }

            // Cria um novo arquivo para a nova imagem
            sprintf(filename, "%03i.jpg", file_count++);
            output = fopen(filename, "w");
        }

        // Escreve no arquivo se estiver aberto
        if (output != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }

    // Fecha os arquivos restantes
    if (output != NULL)
    {
        fclose(output);
    }

    fclose(input);

    return 0;
}
