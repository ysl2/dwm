#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Magically finds the current's executable path
 *
 * I'm doing the do{}while(); trick because Linux (what I'm running) is not
 * POSIX compilant and so lstat() cannot be trusted on /proc entries
 *
 * @return char* the path of the current executable
 */
char *get_dwm_path(){
    struct stat s;
    int r, length, rate = 42;
    char *path = NULL;

    if(lstat("/proc/self/exe", &s) == -1){
        perror("lstat:");
        return NULL;
    }

    length = s.st_size + 1 - rate;

    do{
        length+=rate;

        free(path);
        path = malloc(sizeof(char) * length);

        if(path == NULL){
            perror("malloc:");
            return NULL;
        }

        r = readlink("/proc/self/exe", path, length);

        if(r == -1){
            perror("readlink:");
            return NULL;
        }
    }while(r >= length);

    path[r] = '\0';

    return path;
}

/**
 * self-restart
 *
 * Initially inspired by: Yu-Jie Lin
 * https://sites.google.com/site/yjlnotes/notes/dwm
 */
#include "restoreafterrestart.c"
void self_restart(const Arg *arg) {
    char *const argv[] = {get_dwm_path(), NULL};

    if(argv[0] == NULL){
        return;
    }

    saveSession();
    execv(argv[0], argv);
}


# include <string.h>

void exitdwm ()
{
# if							   \
	defined S_LOCK				|| \
	defined S_RESTART_DWM		|| \
	defined S_OFFSCREEN			|| \
	defined S_EXIT				|| \
	defined S_REBOOT			|| \
	defined S_SHUTDOWN			|| \
	defined S_LOCK_ICON			|| \
	defined S_RESTART_DWM_ICON	|| \
	defined S_OFFSCREEN_ICON	|| \
	defined S_EXIT_ICON			|| \
	defined S_REBOOT_ICON		|| \
	defined S_SHUTDOWN_ICON		|| \
	defined S_FORMAT			|| \
	defined S_FORMAT_CLEAR
# error (conflicting macro names)
# endif

# define S_LOCK "Lock"
# define S_RESTART_DWM "restart Dwm"
# define S_OFFSCREEN "Off-screen"
# define S_EXIT "Exit"
# define S_REBOOT "Reboot"
# define S_SHUTDOWN "Shutdown"
# define S_LOCK_ICON "\uf023"			// <= FontAwesome icons
# define S_RESTART_DWM_ICON "\uf01e"
# define S_OFFSCREEN_ICON "\uf108"
# define S_EXIT_ICON "\uf08b"
# define S_REBOOT_ICON "\uf021"
# define S_SHUTDOWN_ICON "\uf011"

# define S_FORMAT(ACTION) S_##ACTION##_ICON " " S_##ACTION
# define S_FORMAT_CLEAR "sed 's/^..//'"

	FILE * exit_menu = popen (
		"echo \""
			S_FORMAT (LOCK) "\n"
			S_FORMAT (RESTART_DWM) "\n"
			S_FORMAT (OFFSCREEN) "\n"
			S_FORMAT (EXIT) "\n"
			S_FORMAT (REBOOT) "\n"
			S_FORMAT (SHUTDOWN)
			"\" | dmenu -p exit: | " S_FORMAT_CLEAR
		,
		"r"
	);

	char exit_action [16];

	if (
		exit_menu == NULL ||
		fscanf (exit_menu, "%15[a-zA-Z -]", exit_action) == EOF
	) {
		fputs ("Error. Failure in exit_dwm.", stderr);
		goto close_streams;
	}

	if (strcmp (exit_action, S_LOCK) == 0) system ("slock & sleep .5; xset dpms force off");
	else if (strcmp (exit_action, S_RESTART_DWM) == 0) self_restart (& (const Arg) {0});
	else if (strcmp (exit_action, S_OFFSCREEN) == 0) system ("sleep .5; xset dpms force off");
	else if (strcmp (exit_action, S_EXIT) == 0) quit (& (const Arg) {0});
	else if (strcmp (exit_action, S_REBOOT) == 0) system ("systemctl reboot");
	else if (strcmp (exit_action, S_SHUTDOWN) == 0) system ("systemctl poweroff -i");

close_streams:
	pclose (exit_menu);

# undef S_LOCK
# undef S_RESTART_DWM
# undef S_OFFSCREEN
# undef S_EXIT
# undef S_REBOOT
# undef S_SHUTDOWN
# undef S_LOCK_ICON
# undef S_RESTART_DWM_ICON
# undef S_OFFSCREEN_ICON
# undef S_EXIT_ICON
# undef S_REBOOT_ICON
# undef S_SHUTDOWN_ICON
# undef S_FORMAT
# undef S_FORMAT_CLEAR
}
