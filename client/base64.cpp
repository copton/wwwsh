/* this code is from debian's mime-codecs package
 * the original version reads from stdin and writes to stdout
 * I modified the code to be a library
 *
 * alexander.bernauer@ulm.ccc.de
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* b64decode (const char* from) 
{
    static char inalphabet[256], decoder[256];
    int i, bits, c, char_count, errors = 0;

    const int max = strlen(from);
    char* to = (char*) malloc(max);
    int offsetfrom = 0;
    int offsetto = 0;

    for (i = (sizeof alphabet) - 1; i >= 0 ; i--) {
        inalphabet[alphabet[i]] = 1;
        decoder[alphabet[i]] = i;
    }

    char_count = 0;
    bits = 0;
    while (1) {
        if (offsetfrom == max) {
            if (char_count) {
                fprintf(stderr, "base64-decode: base64 encoding incomplete: at least %d bits truncated", ((4 - char_count) * 6));
            }
            break;
        }

        c = from[offsetfrom++];

        if (c == '=') {
            switch (char_count) {
              case 1:
                fprintf(stderr, "base64-decode: base64 encoding incomplete: at least 2 bits missing");
                break;
              case 2:
                to[offsetto++] = (bits >> 10);
                break;
              case 3:
                to[offsetto++] = (bits >> 16);
                to[offsetto++] = ((bits >> 8) & 0xff);
                break;
            }

            break;
        }
        
        if (c > 255 || ! inalphabet[c]) continue;

        bits += decoder[c];
        char_count++;
        if (char_count == 4) {
            to[offsetto++] = (bits >> 16);
            to[offsetto++] = ((bits >> 8) & 0xff);
            to[offsetto++] = (bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }

    to[offsetto] = 0;
    return to;
}

char* b64encode(const char* from)
{
    int bits, c, char_count;
    const int max = strlen(from);
    char* to = (char*) malloc(2 * max);
    int offsetfrom = 0;
    int offsetto = 0;

    char_count = 0;
    bits = 0;

    while (1) {
        if (offsetfrom == max) {
            if (char_count != 0) {
                bits <<= 16 - (8 * char_count);
                to[offsetto++] = alphabet[bits >> 18];
                to[offsetto++] = alphabet[(bits >> 12) & 0x3f];
                if (char_count == 1) {
                    to[offsetto++] = '=';
                    to[offsetto++] = '=';
                } else {
                    to[offsetto++] = alphabet[(bits >> 6) & 0x3f];
                    to[offsetto++] = '=';
                }
            }
            break;
        }

        c = from[offsetfrom++];

        if (c > 255) {
            fprintf(stderr, "encountered char > 255 (decimal %d)", c);
            exit(1);
        }

        bits += c;
        char_count++;
        if (char_count == 3) {
            to[offsetto++] = (alphabet[bits >> 18]);
            to[offsetto++] = (alphabet[(bits >> 12) & 0x3f]);
            to[offsetto++] = (alphabet[(bits >> 6) & 0x3f]);
            to[offsetto++] = (alphabet[bits & 0x3f]);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 8;
        }
    }

    to[offsetto] = 0;
    return to;
}
