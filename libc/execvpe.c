#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include<envp.h>
int execvpe(char *bin_name, env_var **head, char *args[], char *envp[]) {
  
    uint64_t result;
    char *path; 
    char *paths;
    char *full_binary_name;

    paths = get_value(head, "PATH");
    path  =strtok(paths,":");

//    puts("\n last attempt\n");
    result = (int)syscall_3(59, (uint64_t)bin_name, (uint64_t) args, (uint64_t) envp); 
    if(result ==0){
        //puts("successfull in first\n");
         return 0;
    } 
    
    full_binary_name = strconcat(strconcat(path,"/"),bin_name);
    while(full_binary_name !=NULL){
    //    puts(full_binary_name);
    //    puts("\n");
        result = syscall_3(59, (uint64_t)full_binary_name, (uint64_t) args, (uint64_t) envp);
        path  = strtok(NULL,":");
        if(result == 0){
      //      puts("result is 0\n");
            return (int)result;
        }else{
          //  putchar(result);
        //    puts("not zero\n");
        }
        
        full_binary_name = strconcat(strconcat(path,"/"),bin_name);
       
    //        if(result == -2)
    }


   return result;//last attempt. user might be oroving fulle path with name -vj like /bin/grep
}
