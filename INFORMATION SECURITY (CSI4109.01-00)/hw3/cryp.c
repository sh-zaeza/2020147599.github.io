#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#define MAX_BUFFER_SIZE 1024
void print_hex(const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02x", data[i]);
    }
    printf("\n");
}
char* readFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(fileSize + 1); // Allocate memory for the buffer (+1 for null-terminator)
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0'; // Null-terminate the buffer
    fclose(file);

    return buffer;
}

int tagHMAC(const char* fileOut, const char* fileTag, const char* fileKey) {
    // Open the encrypted file
    FILE* encrypted_file = fopen(fileOut, "rb");
    if (encrypted_file == NULL) {
        return 2;
    }

    // Determine the file size
    fseek(encrypted_file, 0, SEEK_END);
    long file_size = ftell(encrypted_file);
    fseek(encrypted_file, 0, SEEK_SET);

    // Read the ciphertext from the encrypted file
    unsigned char* ciphertext = (unsigned char*)malloc(file_size);
    fread(ciphertext, sizeof(unsigned char), file_size, encrypted_file);
    fclose(encrypted_file);

    // Generate a secret key
    const char* key = readFile(fileKey);
    size_t key_len = strlen(key);

    // Compute the HMAC of the ciphertext
    unsigned char hmac_tag[EVP_MAX_MD_SIZE];
    unsigned int hmac_tag_len;
    HMAC(EVP_sha256(), key, key_len, ciphertext, file_size, hmac_tag, &hmac_tag_len);

    // Write the HMAC tag to a separate file
    FILE* hmac_file = fopen(fileTag, "wb");
    if (hmac_file == NULL) {
        return 2;
    }

    fwrite(hmac_tag, sizeof(unsigned char), hmac_tag_len, hmac_file);
    fclose(hmac_file);

    // Cleanup memory and resources
    free(ciphertext);

    return 0;
}

int checkHMAC(const char* fileOut, const char* fileTag, const char* fileKey) {
    // Read the encrypted file and HMAC tag
    FILE* file = fopen(fileOut, "rb");
    if (file == NULL) {
        return 2;
    }

    FILE* tagFile = fopen(fileTag, "rb");
    if (tagFile == NULL) {
        return 2;
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the ciphertext
    unsigned char* ciphertext = (unsigned char*)malloc(file_size);
    fread(ciphertext, sizeof(unsigned char), file_size, file);

    // Read the HMAC tag
    unsigned char provided_tag[EVP_MAX_MD_SIZE];
    size_t provided_tag_len = fread(provided_tag, sizeof(unsigned char), EVP_MD_size(EVP_sha256()), tagFile);

    fclose(file);
    fclose(tagFile);
    // Generate a secret key
    const char* key = readFile(fileKey);
    size_t key_len = strlen(key);

    // Compute the HMAC of the ciphertext
    unsigned char computed_tag[EVP_MAX_MD_SIZE];
    unsigned int computed_tag_len;
    HMAC(EVP_sha256(), key, key_len, ciphertext, file_size, computed_tag, &computed_tag_len);

    // Compare the computed HMAC with the provided HMAC tag
    if (provided_tag_len != computed_tag_len ||
        memcmp(provided_tag, computed_tag, provided_tag_len) != 0) {
        return 1;
    }
    return 0;
}

int encrypt_file(const char *inputFile, const char *outputFile, const char *keyFile, const char *tagFile) {
    // after encryption, you need to produce a HMAC-SHA256 tag (
    FILE *inFile = fopen(inputFile, "rb");
    if (inFile == NULL)
    {
        printf("ERROR\n");
        return 2;
    }

    FILE *outFile = fopen(outputFile, "wb");
    if (outFile == NULL)
    {
        printf("ERROR\n");
        fclose(inFile);
        return 2;
    }

    // Generate a secret key
    char *key = readFile(keyFile);
    size_t key_len = strlen(key);

    // Initialize the cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        return 2;
    }

    unsigned char iv[EVP_MAX_IV_LENGTH];
    unsigned char keyHash[EVP_MAX_MD_SIZE];
    unsigned int keyHashLen;

    // Generate a random IV
    if (RAND_bytes(iv, EVP_MAX_IV_LENGTH) != 1)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        EVP_CIPHER_CTX_free(ctx);
        return 2;
    }

    FILE *tag = fopen(tagFile, "wb");
    if (tagFile == NULL)
    {
        printf("ERROR\n");
        fclose(tag);
        return 2;
    }

    // Generate the HMAC-SHA256 key
    HMAC(EVP_sha256(), key, key_len, iv, EVP_MAX_IV_LENGTH, keyHash, &keyHashLen);

    // Write the IV to the output file
    fwrite(iv, sizeof(unsigned char), EVP_MAX_IV_LENGTH, outFile);

    // Initialize encryption operation
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        EVP_CIPHER_CTX_free(ctx);
        return 2;
    }

    unsigned char inBuffer[MAX_BUFFER_SIZE];
    unsigned char outBuffer[MAX_BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, bytesWritten, totalBytesWritten = 0;

    // Encrypt the input file and write the ciphertext to the output file
    while ((bytesRead = fread(inBuffer, sizeof(unsigned char), MAX_BUFFER_SIZE, inFile)) > 0)
    {
        if (EVP_EncryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead) != 1)
        {
            printf("ERROR\n");
            fclose(inFile);
            fclose(outFile);
            EVP_CIPHER_CTX_free(ctx);
            return 2;
        }

        fwrite(outBuffer, sizeof(unsigned char), bytesWritten, outFile);
        totalBytesWritten += bytesWritten;
    }

    // Finalize the encryption operation
    if (EVP_EncryptFinal_ex(ctx, outBuffer, &bytesWritten) != 1)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        EVP_CIPHER_CTX_free(ctx);
        return 2;
    }

    fwrite(outBuffer, sizeof(unsigned char), bytesWritten, outFile);
    totalBytesWritten += bytesWritten;

    EVP_CIPHER_CTX_free(ctx);
    fclose(inFile);
    fclose(outFile);

    if (tagHMAC(outputFile, tagFile, keyFile)!= 0) {
        printf("ERROR\n");
        return 2;
    }
    return 0;
}

int decrypt_file(const char *inputFile, const char *outputFile, const char *keyFile, const char *tagFile) {
    // before decryption, you would need to first verify the authenticity of the given ciphertext with the given HMAC tag.
    // Read the encrypted file and HMAC tag
    if (checkHMAC(inputFile, tagFile, keyFile) != 0) {
        printf("VERIFICATION FAILURE\n");
        return 1;
    }


    FILE* inFile = fopen(inputFile, "rb");
    if (inFile == NULL) {
        printf("ERROR\n");
        return 2;
    }

    // Read the IV from the input file
    unsigned char iv[EVP_MAX_IV_LENGTH];
    fread(iv, sizeof(unsigned char), EVP_MAX_IV_LENGTH, inFile);

    // Generate a secret key
    char *key = readFile(keyFile);
    size_t key_len = strlen(key);

    // Generate the HMAC-SHA256 key
    unsigned char keyHash[EVP_MAX_MD_SIZE];
    unsigned int keyHashLen;
    HMAC(EVP_sha256(), key, key_len, iv, EVP_MAX_IV_LENGTH, keyHash, &keyHashLen);

    FILE *outFile = fopen(outputFile, "wb");
    if (outFile == NULL)
    {
        printf("ERROR\n");
        fclose(inFile);
        return 2;
    }

    // Initialize the cipher context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        return 2;
    }

    // Initialize decryption operation
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        EVP_CIPHER_CTX_free(ctx);
        return 2;
    }

    unsigned char inBuffer[MAX_BUFFER_SIZE];
    unsigned char outBuffer[MAX_BUFFER_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead, bytesWritten, totalBytesWritten = 0;

    // Decrypt the input file and write the plaintext to the output file
    while ((bytesRead = fread(inBuffer, sizeof(unsigned char), MAX_BUFFER_SIZE, inFile)) > 0)
    {
        if (EVP_DecryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead) != 1)
        {
            printf("ERROR\n");
            fclose(inFile);
            fclose(outFile);
            EVP_CIPHER_CTX_free(ctx);
            return 2;
        }

        fwrite(outBuffer, sizeof(unsigned char), bytesWritten, outFile);
        totalBytesWritten += bytesWritten;
    }

    // Finalize the decryption operation
    if (EVP_DecryptFinal_ex(ctx, outBuffer, &bytesWritten) != 1)
    {
        printf("ERROR\n");
        fclose(inFile);
        fclose(outFile);
        EVP_CIPHER_CTX_free(ctx);
        return 2;
    }

    fwrite(outBuffer, sizeof(unsigned char), bytesWritten, outFile);
    totalBytesWritten += bytesWritten;

    EVP_CIPHER_CTX_free(ctx);
    fclose(inFile);
    fclose(outFile);

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc!=10) {
            printf("ERROR\n");
            return 2;
    }

    const char *mode = argv[1];
    const char *key_file = argv[3];
    const char *input_file = argv[5];

    if (strcmp(mode, "enc") == 0) {

        const char *output_file = argv[7];
        const char *tag_file = argv[9];

        if (strcmp(argv[2], "-key") == 0 && strcmp(argv[4], "-in") == 0
        && strcmp(argv[6], "-out") == 0 && strcmp(argv[8], "-tag") == 0) {
            if (encrypt_file(input_file, output_file, key_file, tag_file) == 0) {
                return 0;
            }
        } else {
            printf("ERROR\n");
            return 2;
        }

    } else if (strcmp(mode, "dec") == 0) {

        const char *tag_file = argv[7];
        const char *output_file = argv[9];

        if (strcmp(argv[2], "-key") == 0 && strcmp(argv[4], "-in") == 0
            && strcmp(argv[6], "-tag") == 0 && strcmp(argv[8], "-out") == 0) {
            if (decrypt_file(input_file, output_file, key_file, tag_file) == 0) {
                return 0;
            }
        } else {
            printf("ERROR\n");
            return 2;
        }
    } else {
        printf("ERROR\n");
        return 2;
        }
    }