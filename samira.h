 void *myMalloc(size_t size)
{
    void *ptr;
    
    if ((ptr = (void *) malloc(size)) == NULL)
    {
        printf("Malloc error");
        exit(1);
    }
    return ptr;
}


void *myRealloc(void *ptr, size_t numMembers)
{
    void *newptr;
    
    if ((newptr = (void *) realloc(ptr, numMembers)) == NULL)
    {
        printf("Realloc error");
        exit(1);
    }
    return newptr;
}


void *myCalloc(size_t numMembers, size_t size)
{
    void *ptr;
    
    if ((ptr = (void *) calloc(numMembers, size)) == NULL)
    {
        printf("Calloc error");
        exit(1);
    }
    return ptr;
}


FILE *myFopen(char *file, char *mode)
{
    FILE *fp;
    
    if ((fp = fopen(file, mode)) == NULL)
    {
        printf("Fopen error on %s!\n", file);
        exit(1);
    }
    return fp;
}


void myFclose(FILE *fp)
{
    if (fclose(fp) != 0)
    {
        printf("Fclose failed!\n");
        exit(1);
    }
}

