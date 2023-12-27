#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    char* username;
    username = getlogin();

    uid_t uid = getuid();
    gid_t gid = getgid();
    setgid(0);
    setuid(0);

    FILE *file;
    file = fopen("mac.policy", "r");
    char line[1024];
    char* name;
    char* level;
    char* list_names[1024];
    int list_levels[1024];
    int count = 0;

    while (fgets (line, 1024, file) != NULL) {
        name = strtok(line, ":");
        list_names[count] = strdup(name);
        level = strtok(NULL, " ");
        level[strcspn(level, "\n")] = 0;

        if (strcmp(level, "TOP_SECRET") == 0) {
            list_levels[count] = 4;
        } else if (strcmp(level, "SECRET") == 0) {
            list_levels[count] = 3;
        } else if (strcmp(level, "CONFIDENTIAL") == 0) {
            list_levels[count] = 2;
        } else if (strcmp(level, "UNCLASSIFIED") == 0) {
            list_levels[count] = 1;
        }

        count++;
    }

    setegid(gid);
    seteuid(uid);

    char filename[1024];
    sprintf(filename, "%s.log", username);
    FILE *log;
    mode_t permissions = 0640;
    chmod((const char *) log, permissions);
    log = fopen(filename, "a");
    for (int i = 1; i < argc; i++) {
        if (i == argc-1){
            fprintf(log, "%s\n", argv[i]);
        } else {
            fprintf(log, "%s ", argv[i]);
        }
    }

    int i, index = -1; // initialize the index to -1 (not found)
    for (i = 0; i < count; i++) {
        if (strcmp(list_names[i], username) ==0) {
            index = i;
            break; // exit the loop once element is found
        }
    }

    int user_level = 0;
    if (index != -1) {
        user_level = list_levels[index];
    }

    if (strcmp(argv[1], "read") == 0){
        if (strcmp(argv[2], "secret.data") == 0 && argc == 3 && user_level >= 3){
            FILE *secret;
            char new_line[1024];
            secret = fopen("secret.data", "r");
            while (fgets (new_line, 1024, secret) != NULL) {
                printf("%s", new_line);
            }
            printf("\n");
            fclose(secret);
        } else if (strcmp(argv[2], "confidential.data") == 0 && argc == 3 && user_level >= 2){
            FILE *confidential;
            char new_line[1024];
            confidential = fopen("confidential.data", "r");
            while (fgets (new_line, 1024, confidential) != NULL) {
                printf("%s", new_line);
            }
            printf("\n");
            fclose(confidential);
        } else if (strcmp(argv[2], "top_secret.data") == 0 && argc == 3 && user_level >= 4){
            FILE *top_secret;
            char new_line[1024];
            top_secret = fopen("top_secret.data", "r");
            while (fgets (new_line, 1024, top_secret) != NULL) {
                printf("%s", new_line);
            }
            printf("\n");
            fclose(top_secret);
        } else if (strcmp(argv[2], "unclassified.data") == 0 && argc == 3 && user_level >= 1){
            FILE *unclassified;
            char new_line[1024];
            unclassified = fopen("unclassified.data", "r");
            while (fgets (new_line, 1024, unclassified) != NULL) {
                printf("%s", new_line);
            }
            printf("\n");
            fclose(unclassified);
        } else {
            printf("ACCESS DENIED\n"); }
    } else if (strcmp(argv[1], "write") == 0){
        if (strcmp(argv[2], "secret.data") == 0 && user_level >= 3){
            FILE *secret;
            secret = fopen("secret.data", "a");
            for (int j = 3; j < argc; j++) {
                if (j == argc-1){
                    fprintf(secret, "%s\n", argv[j]);
                } else {
                    fprintf(secret, "%s ", argv[j]);
                }
            }
            fclose(secret);
        } else if (strcmp(argv[2], "confidential.data") == 0 && user_level >= 2){
            FILE *confidential;
            confidential = fopen("confidential.data", "a");
            for (int j = 3; j < argc; j++) {
                if (j == argc-1){
                    fprintf(confidential, "%s\n", argv[j]);
                } else {
                    fprintf(confidential, "%s ", argv[j]);
                }
            }
        } else if (strcmp(argv[2], "top_secret.data") == 0 && user_level >= 4){
            FILE *top_secret;
            top_secret = fopen("top_secret.data", "a");
            for (int j = 3; j < argc; j++) {
                if (j == argc-1){
                    fprintf(top_secret, "%s\n", argv[j]);
                } else {
                    fprintf(top_secret, "%s ", argv[j]);
                }
            }
            fclose(top_secret);
        } else if (strcmp(argv[2], "unclassified.data") == 0 && user_level >= 1){
            FILE *unclassified;
            unclassified = fopen("unclassified.data", "a");
            for (int j = 3; j < argc; j++) {
                if (j == argc-1){
                    fprintf(unclassified, "%s\n", argv[j]);
                } else {
                    fprintf(unclassified, "%s ", argv[j]);
                }
            }
            fclose(unclassified);
        } else {
            printf("ACCESS DENIED\n"); }
    }
    fclose(file);
    fclose(log);
    return 0;
}
