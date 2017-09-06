#include<sys/defs.h>
int env_list_length(env_var  *head);
void insert_env(env_var **head,int position, const char *key, const char *value);
void print_env_var(env_var *head);

env_var* update_env(env_var *head,char *var_name,  char *var_value);    
char *get_value(env_var *head ,const char *key);

