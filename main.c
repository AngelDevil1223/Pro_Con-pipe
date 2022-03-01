#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

unsigned char char_with_parity(unsigned char);
void bit_writter(unsigned char* arr, FILE* fp);
void framing(int fd[2], char fname[]);
void error_maker(int frame_number,unsigned char arr[], int len);
void deframing(int fd[2] ,FILE* outfptr);
int error_checker(int buffer_number,unsigned char arr[], int len);
int producer(int fd[2] ,char *filename);
int consumer(int fd[2], char *filename);


int producer(int fd[2] ,char *filename) {
    framing(fd,filename);
    return 0;
}

unsigned char char_with_parity(unsigned char chr) {
    // routines to handle tasks such as converting a character into a binary bit pattern,
    // and add parity bit
    int ones=0;
    char temp = chr;
    for (int i = 1; temp; temp=temp/2, i++) {
        if (temp % 2)
            ones++;
    }
    return chr + (128 * (!(ones % 2)));
}

void framing(int fd[2],char fname[]) {
    unsigned char buffer[64];
    int buffer_len;
    FILE *infile, *outfile;
    infile = fopen(fname, "rb");


    if(infile == NULL)
        return;
     
    int frame_number = 1;
    while((buffer_len = fread(buffer, 1, 64, infile))){
        for(int i = 0 ; i < buffer_len; i++){
            buffer[i] = char_with_parity(buffer[i]);
        }
        char control_character[4];
        char len_char = buffer_len;
        error_maker(frame_number,buffer, buffer_len);
        snprintf(control_character, 4, "\22\22%c", len_char);
        /*printf("buffer len is : %d and len_char is %c\n", buffer_len, len_char);*/
        /*printf("And control_character is : %s\n", control_character);*/
        close(fd[0]);
        write(fd[1], control_character, 3);
        close(fd[0]);
        write(fd[1], buffer, buffer_len);
    }

    fclose(infile);
}

void error_maker(int frame_number,unsigned char arr[], int len) {
    int no_of_errors;
    srand(time(0));

    no_of_errors = rand() % 3; // limiting maximum number of errors to two in every 64 bytes
    for (int i=0; i < no_of_errors; i++){
        int byte_number = rand() % len;
        int bit_number = rand() % 8;
        // bit flips
        arr[byte_number] ^= 1 << bit_number; // making sure that error terms don't mess with parity bit
        
        fprintf(stderr,"Error Creation information : %dth frame %d byte and %dth bit\n", frame_number, byte_number / 8, byte_number % 8);
    }

}

int consumer(int fd[2], char *filename){
    FILE *outfptr;
    outfptr = fopen(filename, "wt");

    deframing(fd,outfptr);

    fclose(outfptr);

    return 0;
    
}

int error_checker(int buffer_number,unsigned char arr[], int len){
    int ones;
    int temp;
    int no_of_errors = 0;
    for (int i=0; i < len; i++) {
        int parity = 0;
        unsigned char n = arr[i];
        while (n)
        {
            parity = !parity;
            n      = n & (n - 1);
        }
        if(parity == 0){
            no_of_errors++;
            printf("Error bit exists at %dth bit\n", buffer_number * 64 + i);
        }
    }
    return no_of_errors;
}

void deframing(int fd[2] ,FILE* outfptr){
    int total_errors = 0;
    unsigned char buffer[67];
    int buffer_len;
    char ch;
    int i = 1;
    while (true) {
        close(fd[1]);
        read(fd[0], buffer, 3);
        buffer_len = buffer[2];
        close(fd[1]);
        read(fd[0], buffer, buffer_len);
        total_errors += error_checker(i,buffer, buffer_len);

        for (int i=0; i<buffer_len; i++) {
            if (buffer[i] < 128)
                ch = buffer[i];
            else
                ch = buffer[i] - 128;
            fwrite(&ch, 1, 1, outfptr);
        }
        i++;
        if(buffer_len < 64){
            break;
        }
    }

    printf("total number of errors are: %d\n", total_errors);
}


int main(void){
    int fd[2];
    char *input_filename = "filename.inpf";
    char *output_filename = "filename.outf";
    pid_t producer_process, consumer_process;
    if(pipe(fd) < 0 ){
        fprintf(stderr, "Error in creating pipes!\n");
    }
    producer_process = fork();
    if(producer_process == 0){
        // we are in child process
        producer(fd, input_filename);
        exit(0);
    }
    // in parent process
    consumer_process = fork();
    if(consumer_process == 0){
        consumer(fd, output_filename);
        exit(0);
    }
    // in parent process
    wait(0);
    wait(0);
    return 0;
}
