#include <stdio.h>
#include <string.h>
#include "login.h"

void login_screen() {
    char user[64];

    printf("=====================================\n");
    printf("        C - SYSTEMSUITE LOGIN        \n");
    printf("=====================================\n");

    printf("Usuário: ");
    fgets(user, sizeof(user), stdin);

    user[strcspn(user, "\n")] = 0;

    printf("\nBem-vindo, %s!\n", user);
}
