#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

int main() {
    char executable_path[1024];
    char dir_path[1024];
    char file_path[1024];
    struct stat st;
    DIR *dir;
    struct dirent *entry;

    // Ottieni il percorso dell'eseguibile
    if (readlink("/proc/self/exe", executable_path, sizeof(executable_path)) == -1) {
        perror("Errore nella lettura del percorso dell'eseguibile");
        exit(EXIT_FAILURE);
    }

    // Ottieni il percorso della directory dell'eseguibile
    strcpy(dir_path, dirname(executable_path));

    while (1) {
        // Apri la directory e scorri i file
        dir = opendir(dir_path);
        if (dir == NULL) {
            perror("Errore nell'apertura della directory");
            exit(EXIT_FAILURE);
        }

        while ((entry = readdir(dir)) != NULL) {
            // Costruisci il percorso completo del file
            sprintf(file_path, "%s/%s", dir_path, entry->d_name);

            // Verifica se il file è un file regolare
            if (stat(file_path, &st) == -1) {
                perror("Errore nella lettura delle informazioni sul file");
                exit(EXIT_FAILURE);
            }

            if (S_ISREG(st.st_mode)) {
                // Verifica se il file è un file FLAC e se è stato modificato negli ultimi 5 secondi
                printf("%s",entry->d_name);
                if (strstr(entry->d_name, ".flac") != NULL && difftime(time(NULL), st.st_mtime) < 5.0) {
                    // Esegue il comando
                    char command[1024];
                    sprintf(command, "whisper %s --language English --model base", entry->d_name);
                    printf("Eseguo il comando: %s\n", command);
                    system(command);
                }
            }
        }

        // Chiudi la directory
        closedir(dir);

        // Attendi 5 secondi prima di effettuare una nuova scansione
        sleep(5);
    }

    return 0;
}
