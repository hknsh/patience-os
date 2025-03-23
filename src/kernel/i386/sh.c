#include <kernel/sh.h>
#include <kernel/tty.h>
#include <libc/stdio.h>
#include <libc/string.h>

static char input_buffer[SHELL_INPUT_MAX];
static size_t input_index = 0;

void shell_spawn(void) {
  printf("[root@patience]$ ");
  terminal_lock_position();
}

void shell_handle_char(char c) {
  if (c == '\n') {
    terminal_putchar('\n');
    input_buffer[input_index] = '\0';
    shell_process_command(input_buffer);
    input_index = 0;
    shell_spawn();
    return;
  }

  if (c == '\b') {
    if (input_index > 0) {
      input_index--;
      terminal_putchar('\b');
    }
    return;
  }

  if (input_index < SHELL_INPUT_MAX - 1 && c >= ' ' && c <= '~') {
    input_buffer[input_index++] = c;
    terminal_putchar(c);
  }
}

void shell_process_command(const char* cmd) {
  if (strcmp(cmd, "help") == 0) {
    printf("\navailable commands: help, clear, uname\n\n");
  } else if (strcmp(cmd, "clear") == 0) {
    terminal_init();
  } else if (strcmp(cmd, "uname") == 0) {
    printf("\npatienceOS/i386 (alpha) (kernel)\n\n");
  } else if (strlen(cmd) == 0) {} else {
    printf("\nunknown command: %s\n\n", cmd);
  }
}
