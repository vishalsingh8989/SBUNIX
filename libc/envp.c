#include<sys/defs.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>

int compare_string(char *str_one, char *str_two){

    int i = 0;
    while(str_one[i] == str_two[i] ){
        if(str_one[i] == '\0'){
            return 1;
            }
        i++;
    }
    return 0;
}
void update_env(env_var *head,char *var_name, char *var_value){
    env_var *temp;
    temp = head;
    while(temp !=NULL){
        //puts("Found  :");
        //puts(temp->key);
            

        //puts(temp->value);
        if(compare_string(temp->key, var_name)){
            temp->value = var_value;
            return;  
        }
       temp = temp->next; 
    }
    return;

} 

char *get_value(env_var *head, const char *key){
    while(head !=NULL){
        if(!strcmp(head->key , key)){
           return head->value;
        }
        head=head->next;
     }
    return NULL;


}

int env_list_length(env_var  *head){
    int count = 0;
    while(head!=NULL){
        count++;
        head=head->next;
    }
    return count;
}


void insert_env(env_var **head, char *key, char *value,int position){
    
    if(position){};

    env_var *p, *new_env;
    new_env = (env_var *)malloc(sizeof(env_var));
    if(!new_env){
        //printf("Not able to get free memory");
        return ;
    }
    new_env->key    = key;
    new_env->value  = value;
    new_env->next   = NULL;
    
    if(*head ==NULL){
        *head = new_env;
     }else{
        p = *head;
        //puts("Head\n");
        
        while(p->next!=NULL){
           // puts("one more\n");
            p=p->next;
        }
        p->next = new_env;
        new_env->prev = p;
    }
    
    
}


//only for debugging
void print_env_var(env_var *head){
    puts("*****  env variables test *****\n");
    while(head!=NULL){
        puts("Key   :");
        puts(head->key);
        puts(",");
        puts("\n");
        puts("Value :");
        puts(head->value);
        puts(",");
        puts("\n");
        puts("***********************\n");
        head=head->next;
    }
}
