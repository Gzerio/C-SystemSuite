#ifndef UI_H
#define UI_H

void ui_clear();
void ui_wait_enter();
void ui_header(const char* title);

int ui_line_is_back(const char* s);
void ui_set_go_root();
int ui_should_go_root();
void ui_reset_go_root();

#endif
