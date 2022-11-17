#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 256

char **readtxtfile (char *fn, size_t *idx);
char **realloc_char (char **p, size_t *n);
void prn_chararray (char **ca);
void free_chararray (char **ca);
void vigenere(char *ciper, char *plaintext);

int main (int argc, char **argv) {

    if (argc < 3 ) {
        fprintf (stderr, "error: insufficient input, usage: %s <filename1> <filename2>\n", argv[0]);
        return 1;
    }

    size_t file1_size = 0;  /* placeholders to be filled by readtxtfile */
    size_t file2_size = 0;  /* for general use, not needed to iterate   */

    /* read each file into an array of strings,
    number of lines read, returned in file_size */
    char **file1 = readtxtfile (argv[1], &file1_size);
    char **file2 = readtxtfile (argv[2], &file2_size);

    /* simple print function */
    printf("\n");
    if (file1) prn_chararray(file1);
    printf("\n");
    if (file2) prn_chararray(file2);

    printf("\n\n");
    // cipher text
    char cipher[512];
    int c = 0;
    for(int i = 0; i < strlen(*file1); i++){
      if((file1[0][i] >= 65 && file1[0][i] <= 90) || (file1[0][i] >= 97 && file1[0][i] <= 122)){
        if(file1[0][i] <= 90)
          file1[0][i] += 32;
        cipher[c] = file1[0][i];
        c++;
      }
    }
    cipher[c] = '\0';
   // printf("%s\n\n", cipher);
    
    // plaintext text
    char plaintext[513];
    c = 0;
    for(int i = 0; i < strlen(*file2); i++){
      if((file2[0][i] >= 65 && file2[0][i] <= 90) || (file2[0][i] >= 97 && file2[0][i] <= 122)){
        if(file2[0][i] <= 90)
          file2[0][i] += 32;
        plaintext[c] = file2[0][i];
        c++;
      }
    }
    
    int x = 512 - c;
    for(int i = 0; i < x; i++){
      plaintext[c] = 'x';
      c++;
    }
    plaintext[512] = '\0';
    //printf("%s\n\n", plaintext);
    
    vigenere(cipher, plaintext);
    printf("\n\n");
    

    /* simple free memory function */
    if (file1) free_chararray(file1);
    if (file2) free_chararray(file2);

    return 0;
}

void vigenere(char *cipher, char *plaintext){
  int cipherCounter = 0;
  int plainCounter = 0;
  int encrypt;
  for(int i = 0; i < 512; i++){
    encrypt = abs(cipher[cipherCounter]+plaintext[plainCounter] - 194) + 97;
    if(encrypt > 122){
      encrypt -= 26;
    }
    printf("%c", encrypt);
    plainCounter++;
    cipherCounter++;
    if(plainCounter%80 == 0)
      printf("\n");
    if(cipherCounter >= strlen(cipher)){
      cipherCounter = 0;
    }
  }
}


char** readtxtfile (char *fn, size_t *idx)
{
    if (!fn) return NULL;           /* validate filename provided       */

    char *ln = NULL;                /* NULL forces getline to allocate  */
    size_t n = 0;                   /* max chars to read (0 - no limit) */
    ssize_t nchr = 0;               /* number of chars actually read    */
    size_t nmax = NMAX;             /* check for reallocation           */
    char **array = NULL;            /* array to hold lines read         */
    FILE *fp = NULL;                /* file pointer to open file fn     */

    /* open / validate file */
    if (!(fp = fopen (fn, "r"))) {
        fprintf (stderr, "%s() error: file open failed '%s'.", __func__, fn);
        return NULL;
    }

    /* allocate NMAX pointers to char* */
    if (!(array = calloc (NMAX, sizeof *array))) {
        fprintf (stderr, "%s() error: memory allocation failed.", __func__);
        return NULL;
    }

    /* read each line from fp - dynamicallly allocated   */
    while ((nchr = getline (&ln, &n, fp)) != -1)
    {
        /* strip newline or carriage rtn    */
        while (nchr > 0 && (ln[nchr-1] == '\n' || ln[nchr-1] == '\r'))
            ln[--nchr] = 0;

        array[*idx] = strdup (ln);  /* allocate/copy ln to array        */

        (*idx)++;                   /* increment value at index         */

        if (*idx == nmax)           /* if lines exceed nmax, reallocate */
            array = realloc_char (array, &nmax);
    }

    if (ln) free (ln);              /* free memory allocated by getline */
    if (fp) fclose (fp);            /* close open file descriptor       */

    return array;
}

/* print an array of character pointers. */
void prn_chararray (char **ca)
{
    register size_t n = 0;
    while (ca[n])
    {
        printf ("%s\n", ca[n]);
        n++;
    }
}

/* free array of char* */
void free_chararray (char **ca)
{
    if (!ca) return;
    register size_t n = 0;
    while (ca[n])
        free (ca[n++]);
    free (ca);
}

/*  realloc an array of pointers to strings setting memory to 0.
*  reallocate an array of character arrays setting
*  newly allocated memory to 0 to allow iteration
*/
char **realloc_char (char **p, size_t *n)
{
    char **tmp = realloc (p, 2 * *n * sizeof *p);
    if (!tmp) {
        fprintf (stderr, "%s() error: reallocation failure.\n", __func__);
        // return NULL;
        exit (EXIT_FAILURE);
    }
    p = tmp;
    memset (p + *n, 0, *n * sizeof *p); /* memset new ptrs 0 */
    *n *= 2;

    return p;
}
