#include <kernel/sh.h>
#include <kernel/tty.h>
#include <libc/stdio.h>
#include <libc/string.h>

#include <kernel/sh/hwinfo.h>

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
    printf("available commands: help, clear, uname, hwinfo\n");
  } else if (strcmp(cmd, "clear") == 0) {
    terminal_init();
  } else if (strcmp(cmd, "uname") == 0) {
    printf("patienceOS/i386 (alpha) (kernel)\n");
  } else if (strlen(cmd) == 0) {
  } else if (strcmp(cmd, "hwinfo") == 0) {
    struct sys_info info;
    get_sys_info(&info);
    
    printf("Total memory: %u MB\n", info.total_memory_kb / 1000);
    printf("Used: %u MB\n", info.used_memory_kb);
    printf("Free: %u MB\n", info.free_memory_kb / 1000);
    printf("Physical pages allocated: %u\n", info.total_allocated_pages);
    printf("Virtual pages allocated: %u\n", info.total_virtual_pages);
    printf("CPU: %s\n\n", info.cpu_vendor);

  } else {
    printf("unknown command: %s\n", cmd);
  }
}
