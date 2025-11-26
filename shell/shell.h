#ifndef SHELL_H
#define SHELL_H

void shell_main();
int shell_parse(char* linha, char** argv, int max_args);
void shell_execute(int argc, char** argv);

#endif
