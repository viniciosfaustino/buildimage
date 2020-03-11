/* Author(s): <Your name(s) here>
 * Creates operating system image suitable for placement on a boot disk
*/
/* TODO: Comment on the status of your submission. Largely unimplemented */
#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512       /* floppy sector size in bytes */
#define BOOTLOADER_SIG_OFFSET 0x1fe /* offset for boot loader signature */
// more defines...

void print_ehdr(Elf32_Ehdr ehdr)
{
  printf("ehdr\n");
  printf("e_ehsize: %u\n", ehdr.e_ehsize);  
  printf("e_entry: %u\n", ehdr.e_entry);
  printf("e_flags: %u\n", ehdr.e_flags);
  printf("e_ident: %u\n", ehdr.e_ident);
  printf("e_machine: %u\n", ehdr.e_machine);
  printf("e_phentsize: %u\n", ehdr.e_phentsize);
  printf("e_phnum: %u\n", ehdr.e_phnum);
  printf("e_phoff: %u\n", ehdr.e_phoff);
  printf("e_shstrndx: %u\n", ehdr.e_shstrndx);
  printf("e_shentsize: %u\n", ehdr.e_shentsize);
}

void print_phdr(Elf32_Phdr phdr)
{
  printf("p_phdr\n");
  printf("p_align: %u\n", phdr.p_align);
  printf("p_filesz: %u\n", phdr.p_filesz);
  printf("p_flags: %u\n", phdr.p_flags);
  printf("p_memsz: %u\n", phdr.p_memsz);
  printf("p_offset: %u\n", phdr.p_offset);
  printf("p_vaddr: %u\n", phdr.p_vaddr);
  printf("p_type: %u\n", phdr.p_type);
}

/* Reads in an executable file in ELF format*/
Elf32_Phdr * read_exec_file(FILE **execfile, char *filename, Elf32_Ehdr **ehdr){  
  if (*execfile){
    size_t step = fread(*ehdr, 1, sizeof(**ehdr), *execfile);
    print_ehdr(**ehdr);
    if ((*ehdr)->e_phoff != 0){
      Elf32_Phdr* phdr = malloc(sizeof(Elf32_Phdr));
      step = fread(phdr, 1, sizeof(*phdr), *execfile);
      print_phdr(*phdr);      
      return phdr;
    }    
  }
  return NULL;
}

/* Writes the bootblock to the image file */
void write_bootblock(FILE **imagefile,FILE *bootfile,Elf32_Ehdr *boot_header, Elf32_Phdr *boot_phdr){

  void * buffer =  malloc(boot_phdr->p_filesz);
  size_t step = fread(buffer, 1, boot_phdr->p_filesz, bootfile);
  fwrite(buffer, 1, boot_phdr->p_filesz, *imagefile);

  int mod = boot_phdr->p_filesz % 512;
  if(mod > 0)
  {
    void* padding = calloc(512 - mod, 1);
    fwrite(padding, 1, 512 - mod,  *imagefile);
  }
}

/* Writes the kernel to the image file */
void write_kernel(FILE **imagefile,FILE *kernelfile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
  void * buffer =  malloc(kernel_phdr->p_filesz);
  size_t step = fread(buffer, 1, kernel_phdr->p_filesz, kernelfile);
  fwrite(buffer, 1, kernel_phdr->p_filesz, *imagefile);

  int mod = kernel_phdr->p_filesz % 512;
  if(mod > 0)
  {
    void* padding = calloc(512 - mod, 1);
    fwrite(padding, 1, 512 - mod,  *imagefile);
  }
}

/* Counts the number of sectors in the kernel */
int count_kernel_sectors(Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr){
    int sectors = ceil(kernel_phdr->p_filesz/512.0);
    return sectors;
}

/* Records the number of sectors in the kernel */
void record_kernel_sectors(FILE **imagefile,Elf32_Ehdr *kernel_header, Elf32_Phdr *kernel_phdr, int num_sec){
  fseek(*imagefile, 2, SEEK_SET);
  fwrite(&num_sec, 1, sizeof(int), *imagefile);  
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
  Elf32_Phdr *boot_program_header = malloc(sizeof(Elf32_Phdr));; //bootblock ELF program header
  Elf32_Phdr *kernel_program_header = malloc(sizeof(Elf32_Phdr));; //kernel ELF program header

  /* build image file */
  imagefile =  fopen(IMAGE_FILE, "w");
  bootfile =  fopen(argv[1], "rb");
  kernelfile =  fopen(argv[2], "rb");
  
  /* read executable bootblock file */
  boot_program_header = read_exec_file(&bootfile, argv[1], &boot_header);  
    
  /* write bootblock */
  // write_bootblock(&imagefile, bootfile, boot_header, boot_program_header);

  /* read executable kernel file */
  kernel_program_header = read_exec_file(&kernelfile, argv[2], &kernel_header);

  /* write kernel segments to image */
  write_kernel(&imagefile, kernelfile, kernel_header, kernel_program_header);

  /* tell the bootloader how many sectors to read to load the kernel */
  int sectors = count_kernel_sectors(kernel_header, kernel_program_header);

  record_kernel_sectors(&imagefile, kernel_header, kernel_program_header, sectors);
  
  /* check for  --extended option */
  if(!strncmp(argv[1],"--extended",11)){
	/* print info */
  }
  fclose(bootfile);
  fclose(imagefile);
  fclose(kernelfile);
  return 0;
} // ends main()
