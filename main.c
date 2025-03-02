#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


void print_permissions(mode_t mode) {
  if (S_ISDIR(mode)) {
    printf("d");
  } else if (S_ISLNK(mode)) {
    printf("l");
  } else if (S_ISREG(mode)) {
    printf("-");
  } else if (S_ISSOCK(mode)) {
    printf("s");
  } else if (S_ISFIFO(mode)) {
    printf("p");
  } else if (S_ISCHR(mode)) {
    printf("c");
  } else if (S_ISBLK(mode)) {
    printf("b");
  }
  
  printf("%c", (mode & S_IRUSR) ? 'r' : '-');  
  printf("%c", (mode & S_IWUSR) ? 'w' : '-');  
  printf("%c", (mode & S_IXUSR) ? 'x' : '-');  

  printf("%c", (mode & S_IRGRP) ? 'r' : '-');  
  printf("%c", (mode & S_IWGRP) ? 'w' : '-');  
  printf("%c", (mode & S_IXGRP) ? 'x' : '-');  

  printf("%c", (mode & S_IROTH) ? 'r' : '-');  
  printf("%c", (mode & S_IWOTH) ? 'w' : '-');
  printf("%c", (mode & S_IXOTH) ? 'x' : '-');
  
}

int main() {
  DIR *dir;
  struct dirent *entry;
  struct stat file_stat;
  
  dir = opendir(".");
  if (dir == NULL) {
    printf("erreur lors de l'ouverture du répertoire");
    return 1;
  }
  
  printf("Liste des fichiers : \n");
  
  while ((entry = readdir(dir)) != NULL) {
    if (stat(entry->d_name, &file_stat) == 0) {
      print_permissions(file_stat.st_mode);
      printf(" - %li - %i - %i - %li - %s", file_stat.st_nlink, file_stat.st_uid, file_stat.st_gid, file_stat.st_size, entry->d_name);
      
      if (S_ISLNK(file_stat.st_mode)) {
        char target[1024];
        ssize_t len = readlink(entry->d_name, target, sizeof(target) - 1);
        if (len != -1) {
          target[len] = '\0';
          printf(" -> %s", target);
        }
      }
      printf("\n");
    }
    else {
      printf("erreur stat pour le fichier : %s\n", entry->d_name);
    }
  }
  
  closedir(dir);
  return 0;
}
