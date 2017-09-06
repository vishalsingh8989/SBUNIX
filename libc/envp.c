#include<sys/defs.h>
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>

int compare_string(const char *str_one, const char *str_two){

    int i = 0;
    while(str_one[i] == str_two[i] ){
        if(str_one[i] == '\0'){
            return 1;
            }
        i++;
    }
    return 0;
}

int env_list_length(env_var *head){
    int count;
    env_var *current=head;
    while(current!=NULL){
        count++;
        current=current->next;
    }
    return count;
}

char *get_value(env_var *head, const char *key){
    while(head !=NULL){
        puts(head->key);
        puts("\n");
        if(compare_string(head->key , key)){
        
           return head->value;
        }
        head=head->next;
     }
    puts("return NULL from get_value\n");
    return NULL;


}
char * strcopy (char * dest, char * src) {

    if (!dest || !src) return NULL;

    char * tmp = dest;

    while (*dest != '\0' || *src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *(++dest)='\0';
    return tmp;
}


int get_length(env_var *head){
    int count;
    env_var *curr=head;
    while(curr!=NULL){
        count++;
        curr=curr->next;
    }
    return count;
}
void insert_env(env_var **head, int pos, char *key, char *value){
    int k=1;
    env_var *q,*p;
    env_var *new_env=(env_var* ) malloc(sizeof(env_var));
    if(!new_env){
        puts("Memory Error\n");
        return;
    }

    new_env->key=(char *)malloc(sizeof(char)*50);
    new_env->value=(char *)malloc(sizeof(char)*50);
    strcopy(new_env->key ,key);
    strcopy(new_env->value ,value);

    p=*head;
    if(pos==1 || p==NULL){
        new_env->next=*head;
        *head=new_env;
    }
    else{
        while(p!=NULL && (k<pos)){
            k++;
            q=p;
            p=p->next;
        }
        new_env->next=q->next;
        q->next=new_env;
    }
}

void print_env_var(env_var *head){
    while(head!=NULL){
        puts(head->key);
        puts(head->value);
        head=head->next;
    }
    puts("\n");
}
