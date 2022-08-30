#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/sysctl.h> // since <sys/sysinfo.h> not available on mac os
// #include <sys/sysinfo.h>
#include <sys/types.h>
#include <grp.h>
#include <math.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <ncurses.h>


#define clean() printf("\033[H\033[J") //cleaning up the Terminal's/Command prompt's info


/*
  Function declarations(return types & parameters)
 */

int shell_ls(char **args);
int shell_mkdir(char **args);
int shell_uptime(char **args);
int shell_sl(char **args);

int shell_help(char **args);
int shell_exit(char **args);

void init_shell(void);


//  We were supposed to implement ls, mkdir, uptime, sl

char *implmntd[] = {
  "help",
  "exit",
  "ls",
  "mkdir",
  "uptime",
  "sl"
};

// The function that "maps" the methods above to their implementations
int (*implmntd_func[]) (char **) = {
  &shell_help,
  &shell_exit,
  &shell_ls,
  &shell_mkdir,
  &shell_uptime,
  &shell_sl

};

// Size of the array of implemented methods

int implmntd_size() {
  return sizeof(implmntd) / sizeof(char *);
}

/*
  The implementations of the functions above
*/

int shell_ls(char **args)
{

// if we have left out the directory_name

  if(args[1] == NULL)
 {
    fprintf(stderr, "lsh: expected argument to \"ls\"\n");
     init_shell(); }

// if we just want to print contents of a particular directory

  else if(args[1] != NULL && (strcmp(args[1], "--help") != 0 ) && args[2] == NULL)
  {
    DIR *d;
    struct dirent *dir;
    d = opendir(args[1]);
    if (d == NULL) {
       perror("Cannot find directory");
       init_shell();
       exit(-1);
       }

    while ((dir = readdir(d)) != NULL) {
       printf("%s\n", dir->d_name); }
    closedir(d);
  }

//option --color=always to print files in color

   else if(strcmp(args[1], "--color=always") == 0)
   {
     DIR *d;
     struct dirent *dir;
     d = opendir(args[2]);
     if (d == NULL) {
        perror("Cannot find directory");
        init_shell();
        exit(-1);
        }

     while ((dir = readdir(d)) != NULL) {
        printf("\x1b[35;46m%s\x1b[0m\n", dir->d_name); }
     closedir(d);
  }

// manual option for ls

   else if(strcmp(args[1], "--help") == 0)
  {
    printf("Usage:\n");
    printf("The ls command is used simply for listing files.\n");
    printf("Format used and implemented in this shell = > ls directory_name\n");
    printf("Options = > ls --color=always for colorful output\n");
    printf("            ls --help for instructions to use ls\n");
  }

/* it was piping or at least redirecting output, so we chose to redirect
   the output of ls to a file Ex. ls Tony > os.txt */

   else if(strcmp(args[2], ">") == 0){
      FILE * fPtr;
      char file_name[strlen(args[3])];
      strcpy(file_name, args[3]);
      fPtr = fopen(file_name, "w");
      if(fPtr == NULL)
    {
// File not created hence exit
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
    DIR *d;
    struct dirent *dir;
    d = opendir(args[1]);
    if (d == NULL) {
       perror("Cannot find directory");
       init_shell();
       exit(-1);
       }

    while ((dir = readdir(d)) != NULL) {
      fputs(dir->d_name, fPtr);
      fputs("\n", fPtr);
      }
    closedir(d);
    /* Close file to save file data */
    fclose(fPtr);
   }
// in case we have a bad syntax
   else {
     printf("If you need help use the ls --help command for additional information.\n");

  }

init_shell();
  return 1;
}


int shell_mkdir(char **args)
{
  if(args[1]==NULL)
 {
    fprintf(stderr, "lsh: expected argument to \"mkdir\"\n");
     init_shell(); }

  else{
    int i;

// this way we implement the option of passing multiple arguments and creating multiple directories

  for(i=1;i<=sizeof(args);i++){

    errno = 0;
    int ret = mkdir(args[i], S_IRWXU);
    if (ret == -1) {
      switch (errno) {
          case EACCES :
              printf("The parent directory does not allow to write\n");
              init_shell(); //this way we can try again
          case EEXIST:
              printf("Pathname already exists\n");
              init_shell();
          default:
              init_shell();
       } }

    printf("\x1b[35;46mDirectory is created\x1b[0m\n");
   }
 }
  return 0;
}

/*
uptime provides in terminal information in this form
16:44  up 3 days,  6:32, 2 users, load averages: 1.40 1.33 1.35
but despite all the struggle we were not able to find any other way to display all of the listed info
This is due to <sys/sysinfo.h> not being available on mac os and the "replacement" library #include <sys/sysctl.h> not offering the valid info
*/

int shell_uptime(char **args)
{
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    time_t t;
    if( sysctl(mib, 2, &boottime, &len, NULL, 0) < 0 )
    {
        return -1.0;
    }
    time(&t);
    time_t bsec = boottime.tv_sec;
    time_t csec = time(NULL);
    char *c_time_string = ctime(&t);
    int len_of_new_line = strlen(c_time_string) - 1;
    c_time_string[len_of_new_line] = '\0';
    if(args[1] == NULL){
    printf("\x1b[35;46m%s up %d days\x1b[0m\n", c_time_string, (int)difftime(csec, bsec)/(3600*24));
  }
// here we print how many days has the device been up for by entering uptime -p option
    else if(strcmp(args[1],"-p") == 0)
    {
      printf("\x1b[35;46mup %d days\x1b[0m\n", (int)difftime(csec, bsec)/(3600*24));
    }
// here we print the date and time by entering the uptime -s command
    else if(strcmp(args[1], "-s") == 0){

      printf("\x1b[35;46m%s\x1b[0m\n", c_time_string);
    }
// in case we enter uptime some_invalid_parameter/option
    else {
    printf("Check the manual for the usage of uptime and its syntax\n");
  }
    init_shell();
    return 0;
}

emoji(f,a,s)char*s;
{
char c;
return f&1?a!=*s++?emoji(f,a,s):s[11]:f&2?a!=*s++?1+emoji(f,a,s):1:f&4?a--?
 putchar(*s),emoji(f,a,s):a:f&8?*s?emoji(8,32,(c=emoji(1,*s++,"Arjan Kenter. \no$../.\""),
 emoji(4,emoji(2,*s++,"POCnWAUvBVxRsoqatKJurgXYyDQbzhLwkNjdMTGeIScHFmpliZEf"),&c),s)):
 65:(emoji(8,34,"rgeQjPruaOnDaPeWrAaPnPrCnOrPaPnPjPrCaPrPnPrPaOrvaPndeOrAnOrPnOrP\
nOaPnPjPaOrPnPrPnPrPtPnPrAaPnBrnnsrnnBaPeOrCnPrOnCaPnOaPnPjPtPnAaPnPrPnPrCaPn\
BrAnxrAnVePrCnBjPrOnvrCnxrAnxrAnsrOnvjPrOnUrOnornnsrnnorOtCnCjPrCtPnCrnnirWtP\
nCjPrCaPnOtPrCnErAnOjPrOnvtPnnrCnNrnnRePjPrPtnrUnnrntPnbtPrAaPnCrnnOrPjPrRtPn\
CaPrWtCnKtPnOtPrBnCjPronCaPrVtPnOtOnAtnrxaPnCjPrqnnaPrtaOrsaPnCtPjPratPnnaPrA\
aPnAaPtPnnaPrvaPnnjPrKtPnWaOrWtOnnaPnWaPrCaPnntOjPrrtOnWanrOtPnCaPnBtCjPrYtOn\
UaOrPnVjPrwtnnxjPrMnBjPrTnUjP"),0);
}
/*
   Steam locomotive, sl command also known as joke command.
   We implemented this smiley face instead, since on our devices the source code of sl did not want to run,
   because x84_64 architecture does not provide support

*/

int shell_sl(char **args)
{
    if (fork() == 0){  // implementation of fork() in C-program of choice

      return emoji(0,75,"mIWltouQJGsBniKYvTxODAfbUcFzSpMwNCHEgrdLaPkyVRjXeqZh");
    }

    else
    {
        wait(NULL);  // implementation of wait() in C-program of choice
        init_shell();
    }
}

// THIS IS A DIFFERENT VERSION FOR SL, USING EXCV() WHICH WORKS ONLY WHEN THE EXE VERSION OF THIS FILE IS EXECUTED IN TERMINAL ./shell

/*
int shell_sl(char **args)
{
      char *arg[]={"./emoji", NULL};
      execv(arg[0],arg);  // implementation of one of the exec() family members
      return 0;
}
*/

// here we have managed somehow to implement something similar to ls, which still does not run on our devices but you can try it

/*
void indexToEnd(const char* str, char* dst, size_t start) {
    size_t j = 0;
    for (size_t i = start; i <= strlen(str); ++i) {
        dst[j++] = str[i];
    }
    dst[j] = 0;
} */

/*
int shell_ls(char **args)
{
    initscr();
    clear();
    noecho();
    curs_set(0);
    char top[] = "                *''`'`'`'`''''`'`'`          ";
    char md1[] = "               ||__  _______ _______         ";
    char md2[] = "             c/____|-|_____|-|_____|         ";
    char btm[] = "                                             ";
    char dst[] = "                                             ";
    for (int i = 0; i < strlen(top); i++) {
        indexToEnd(top, dst, i);
        mvprintw(0, 0, dst);
        indexToEnd(md1, dst, i);
        mvprintw(1, 0, dst);
        indexToEnd(md2, dst, i);
        mvprintw(2, 0, dst);
        indexToEnd(btm, dst, i);
        mvprintw(3, 0, dst);
        usleep(15000);  // wait for these seconds
        refresh();
}
endwin();
return 0;
}  */



int shell_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}


int shell_help(char **args)
{
  int i;
  printf("Manual for use:\n");
  printf("Syntax = > command + argument\n");
  printf("The following commands are built in:\n");

  for (i = 0; i < implmntd_size(); i++) {
    printf("  %s\n", implmntd[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}


int shell_exit(char **args)
{
  //this.Close();
  return 0;
}


int launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();  // implementation of fork()
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED); // waitpid() implementation
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < implmntd_size(); i++) {
    if (strcmp(args[0], implmntd[i]) == 0) {
      return (*implmntd_func[i])(args);
    }
  }

  return launch(args);
}


char *shell_line_reader(void)
{
  char *line = NULL;
  size_t bufsize = 0; // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } else  {
      perror("Error occured while line reading");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}



#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char **shell_line_splitter(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// allowing next input after completion of one process

void init_shell(void)
{
  char *line;
  char **args;
  int status;

  struct utsname unameData;
  uname(&unameData); // We tried so many ways to retrieve the computer name, but this is as best as we could go for the machine name
  char* username = getenv("USER");

  do {
    printf("%c]0;%s%c", '\033', "Ajlin i Farisov Shell", '\007');
    printf("\x1b[35;46m%s@%s: ~ $ \x1b[0m",unameData.machine,username); //Format to be satisfied=> machinename@username:~$
    line = shell_line_reader();
    args = shell_line_splitter(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}



int main(int argc, char **argv)
{

  // Clear data from compiler window
  clean();
  // Initialize shell
  init_shell();

  return EXIT_SUCCESS;
}
