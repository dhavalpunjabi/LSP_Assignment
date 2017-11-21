/*************************************************************
*                      HEADERS                               *
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*************************************************************
*                       MAIN                                 *
*************************************************************/

int main(int argc, char *argv[]) {
    int option;
    int c_flag = 0;
    int v_flag = 0;
    int num_option = 0;
    char txt_option[200] = "default";

    opterr = 0;

    while ( (option = getopt(argc, argv, "cvb:f:")) != EOF ) {
      switch (option) {
      case 'c' :
          c_flag = 1;
          break;
      case 'v' :
          v_flag = 1;
          break;
      case 'b' :
          num_option = atoi(optarg);
          break;
      case 'f' :
          strcpy(txt_option, optarg);
          break;
      case '?' :
          fprintf(stderr, "invalid option : -%c\n", optopt);
      }
    }
    if (c_flag) printf("Option c is set\n");
    if (v_flag) printf("Option v is set\n");
    printf ("Numeric option value is %d\n",num_option);
    printf ("Text value option is %s\n",txt_option);

    argv += optind;
    argc -= optind;
    while (argc--) {
      printf("arg : %s\n",*argv++);
    }
  return 0;
}
