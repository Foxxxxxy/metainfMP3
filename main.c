#include <stdio.h>
#include <malloc.h>
#include <string.h>

char* get_file_name (char* filepath) {
    size_t file_name_len = strlen(filepath) - 11;

    char *file_name = (char *) malloc(file_name_len);
    strcpy(file_name, filepath + 11);

    return file_name;
}

char* get_command (char* raw_command) {
    char* command = (char *) malloc(4);
    strncpy(command, raw_command + 2, 4);
    command[4] = '\0';

    char* res;
    if (strcmp(command, "show") == 0)
        res = "show";
    else if (strcmp(command, "get=") == 0)
        res = "get";
    else if (strcmp(command, "set=") == 0)
        res = "set";
    else
        res = "";

    return res;
}

char* get_command_frame (char* raw_command) {
    char* res = (char *) malloc(strlen(raw_command) - 6);
    strcpy(res, raw_command + 6);
    return res;
}

char* get_command_val (char* raw_command_val) {
    char* res = (char *) malloc(strlen(raw_command_val) - 8);
    strcpy(res, raw_command_val + 8);
    return res;
}

int get_size (char c1, char c2, char c3, char c4) {
    int d1 = (int)c1 << 21;
    int d2 = (int)c2 << 14;
    int d3 = (int)c3 << 7;
    int d4 = (int)c4;
    return d4 + d3 + d2 + d1;
}

int is_letter_or_num (char c) {
    if (c >= 'A' && c <= 'Z' || c >= '1' && c <='9')
        return 1;
    return 0;
}

int is_frame (char* s) {
    //if (s[0] != 'T' && s[0] != 'W')
        //return 0;

    for (int i = 0; i < 4; ++i) {
        if (!is_letter_or_num(s[i]))
            return 0;
    }

    for (int i = 4; i < 8; ++i) {
        if (s[i] > 127)
            return 0;
    }

    return 1;
}

int main(int argc, char * argv[]) {
    //char* file_name = get_file_name("C:\\Users\\Nikita\\Documents\\sample-15s.mp3");
    //char* command = get_command(argv[2]);

    FILE* file = fopen("C:\\Users\\Nikita\\Documents\\Rick Astley - Never Gonna Give You Up (radio edit).mp3", "r+");

    char header[10];
    fread(header, sizeof(char), 10, file);

    int metainf_size = get_size(header[6], header[7], header[8], header[9]);

    char buf [10];
    while (fread(buf, sizeof(char), 10, file)) {
        if (is_frame(buf)) {
            int frame_size = get_size(buf[4], buf[5], buf[6], buf[7]);
            char frame_inf [frame_size];
            fread(frame_inf, sizeof(char), frame_size, file);
            printf("%4s: ", buf);
            printf("%s\n", frame_inf);
        }
    }

    return 0;
}
