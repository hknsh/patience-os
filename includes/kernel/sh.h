#ifndef SHELL_H
#define SHELL_H

#define SHELL_INPUT_MAX 128

void shell_spawn(void);
void shell_handle_char(char c);
void shell_process_command(const char* input);

#endif // !SHELL_H
