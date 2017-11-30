#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]) {


	if (argc > 3 && !strcmp(argv[1], "-9")) {

			printf("execute kill here.\n");
			//kill(-9, atoi(argv[2]));
	        exit(0);
	    }

	if(argc  == 3){
		printf("-sbush: kill -9  <%s>:  arguments must be process or job IDs\n", argv[2]);
	}else{
		printf("-sbush: kill -9  <pid>:  arguments must be process or job IDs\n");
	}

  return 0;
}
