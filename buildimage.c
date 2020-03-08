/* Author(s): <Your name(s) here>
 * Creates operating system image suitable for placement on a boot disk
*/
/* TODO: Comment on the status of your submission. Largely unimplemented */
#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512       /* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
// more defines...

/* Reads in an executable file in ELF format*/
Elf32_Phdr * read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr){
  Elf32_Phdr* phdr = malloc(sizeof(Elf32_Phdr));
  *execfile = fopen((const char*)filename, "rb");
  if (*execfile){

    size_t step = fread(*ehdr, 1, sizeof(**ehdr), *execfile);
    // fseek(*execfile, step, SEEK_SET);
    printf("AI MEU PIRUUUUUUUUUUUUUUUUUU\n");
    step = fread(phdr, 1, sizeof(*phdr), *execfile);
    
    fclose(*execfile);
    printf("novinha senta de vagaf\n");
    return phdr;

  }
  else
  {
    return NULL;
  }
  
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile,FILE *bootfile,Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){
 

}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile,FILE *kernelfile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){

 
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
   
    return 0;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec){
    
}


/* Prints segment information for --extended option */
void extended_opt(Elf32_Phdr *bph, int k_phnum, Elf32_Phdr *kph, int num_sec){

  /* print number of disk sectors used by the image */

  
  /*bootblock segment info */
 

  /* print kernel segment info */
  

  /* print kernel size in sectors */
}
// more helper functions...

/* MAIN */
// ignore the --vm argument when implementing (project 1)
int main(int argc, char **argv){
  FILE *kernelfile, *bootfile,*imagefile;  //file pointers for bootblock,kernel and image
  Elf32_Ehdr *boot_header = malloc(sizeof(Elf32_Ehdr));//bootblock ELF header
  Elf32_Ehdr *kernel_header = malloc(sizeof(Elf32_Ehdr));//kernel ELF header
  Elf32_Phdr *boot_program_header; //bootblock ELF program header
  Elf32_Phdr *kernel_program_header; //kernel ELF program header

  /* build image file */
  imagefile =  fopen(IMAGE_FILE, "w");  

  /* read executable bootblock file */  
  boot_program_header = read_exec_file(&bootfile, argv[1], &boot_header);
  printf("a sua mae foi trabalhar na zona\n");
  /* write bootblock */    
  fwrite(boot_header, 1, sizeof(*boot_header), imagefile);
  fwrite(boot_program_header, 1, sizeof(*boot_program_header), imagefile);

  /* read executable kernel file */

  /* write kernel segments to image */

  /* tell the bootloader how many sectors to read to load the kernel */

  /* check for  --extended option */
  if(!strncmp(argv[1],"--extended",11)){
	/* print info */
  }  
  fclose(imagefile);
  return 0;
} // ends main()



