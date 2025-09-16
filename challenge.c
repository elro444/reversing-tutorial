#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define XOR_KEY 0x42

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static char decoding_table[256] = {0};

void build_decoding_table() {
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


unsigned char *b64decode(const char *data, size_t *output_length)
{
    int pad = 0;
    size_t input_length = strlen(data);
    if (input_length < 2)
        return NULL;
    if (data[input_length - 1] == '=')
        pad++;
    if (data[input_length - 2] == '=')
        pad++;

    *output_length = (input_length * 3) / 4 - pad;

    unsigned char *decoded_data = malloc(*output_length + 1);
    if (!decoded_data)
        return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned int)data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned int)data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned int)data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[(unsigned int)data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
                        + (sextet_b << 2 * 6)
                        + (sextet_c << 1 * 6)
                        + (sextet_d << 0 * 6);
        
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    decoded_data[*decoded_data] = 0;
    return decoded_data;
}

// --- Very naive JSON key extraction ---
char *extract_json_value(const char *body, const char *key)
{
    static char buf[1024];
    char pattern[128];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);
    char *pos = strstr(body, pattern);
    if (!pos)
        return NULL;
    pos += strlen(pattern);
    char *end = strchr(pos, '"');
    if (!end)
        return NULL;
    size_t len = end - pos;
    if (len >= sizeof(buf))
        return NULL;
    strncpy(buf, pos, len);
    buf[len] = '\0';
    return buf;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[4096];

    build_decoding_table();

    // Secret after XOR decoding should equal "supersecret"
    const char *expected = "supersecret";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind failed");
        exit(1);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(1);
    }

    printf("HTTP Server listening on http://localhost:%d/\n", PORT);

    while (1)
    {
        socklen_t addrlen = sizeof(addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen)) < 0)
        {
            perror("accept failed");
            continue;
        }
        printf("Accepted client\n");

        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0)
        {
            goto error;
        }
        buffer[n] = '\0';

        // Extract body (very naive: look for "\r\n\r\n")
        char *body = strstr(buffer, "\r\n\r\n");
        if (NULL == body || '\0' == *(body + 4))
        {
            printf("Missing HTTP data!\n");
            goto error;
        }
        body += 4;

        char *b64val = extract_json_value(body, "key");
        if (!b64val)
        {
            printf("Invalid json contents!\n");
            goto error;
        }

        size_t outlen;
        unsigned char *decoded = b64decode(b64val, &outlen);
        if (!decoded)
        {
            printf("Invalid base64 value!\n");
            goto error;
        }

        // XOR decode
        for (size_t i = 0; i < outlen; i++)
        {
            decoded[i] ^= XOR_KEY;
        }

        char response[256];
        if (strncmp((char *)decoded, expected, strlen(expected)) == 0)
        {
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nAccess Granted\n");
            printf("Sent 200! Congrats!\n\n");
        }
        else
        {
            snprintf(response, sizeof(response),
                     "HTTP/1.1 403 Forbidden\r\nContent-Type: text/plain\r\n\r\nAccess Denied\n");
            printf("Sent 403!\n\n");
        }
        goto close;
    error:
        snprintf(response, sizeof(response),
                 "HTTP/1.1 401 Bad Request\r\nContent-Type: text/plain\r\n\r\nBad request\n");
        printf("Sent 401!\n\n");
    close:
        write(client_fd, response, strlen(response));
        close(client_fd);
        free(decoded); // free(NULL) is fine
    }
    return 0;
}
