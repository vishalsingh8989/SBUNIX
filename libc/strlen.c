
int strlen(char *string){
    char *address;
    address = string;
    while(*address){
        address++;    
    }
    return (address - string); //any error introduced ..I am not sure - vj
}


