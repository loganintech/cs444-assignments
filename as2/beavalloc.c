#include "beavalloc.h"
#include "unistd.h"
#include "stdio.h"  // for stderr
#include "string.h" // for memset

#define bool int
#define TRUE 1
#define FALSE 0
#define REGION_DATA_SIZE sizeof(struct region_info)
#define BLOCK_REQUEST_SIZE 1024

static struct region_info *head;
static bool global_verbose = FALSE;

struct region_info
{
    size_t bytes;
    struct region_info *next;
    bool is_free;
};

struct region_info *get_empty_block(struct region_info **last, size_t size);
struct region_info *get_bytes(struct region_info *last, size_t size);

struct region_info *get_empty_block(struct region_info **last, size_t size)
{
    struct region_info *curr = *last;
    while (curr != NULL && (curr->bytes < size || !curr->is_free))
    {
        *last = curr;
        curr = curr->next;
    }
    return curr;
}

struct region_info *get_bytes(struct region_info *last, size_t size)
{
    // Get base address for this new data
    struct region_info *new_block = sbrk(0);
    struct region_info *second_block = new_block;
    size_t first_total_block_size = size + REGION_DATA_SIZE;
    size_t total = 0;
    void *new_bytes;
    while (total < first_total_block_size + REGION_DATA_SIZE)
    {
        // Load bytes in BLOCK_REQUEST_SIZE increments until we have enough to fit the requested block and the extra space block
        new_bytes = sbrk(BLOCK_REQUEST_SIZE);
        // new_bytes and new_block should point to the same thing now

        if (new_bytes == (void *)-1) // an error occurred
        {
            return NULL;
        }
        total += BLOCK_REQUEST_SIZE;
    }

    if (last != NULL)
    {
        last->next = new_block;
    }

    second_block = (void *)new_block + first_total_block_size;
    second_block->next = NULL;
    second_block->bytes = total - (first_total_block_size + REGION_DATA_SIZE);
    second_block->next = NULL;
    second_block->is_free = TRUE;
    new_block->bytes = size;
    new_block->next = second_block;
    new_block->is_free = FALSE;
    return new_block;
}

void *beavalloc(size_t size)
{
    struct region_info *last;
    struct region_info *new_block;
    if (size == 0)
    {
        return NULL;
    }

    if (head == NULL)
    { // We haven't initialized anything yet
        new_block = get_bytes(NULL, size);
        if (new_block == NULL)
        {
            return NULL;
        }
        head = new_block;
        return new_block + 1;
    }

    //This isn't the first call to malloc
    last = head;
    // get_empty_block traverses the list changing last
    new_block = get_empty_block(&last, size);
    if (new_block != NULL)
    {
        new_block->is_free = FALSE;

        // Check if we have enough space to split the block
        // Make sure it has enough space, giving it at least 64 bytes of overhead
        if (new_block->bytes > size + REGION_DATA_SIZE + 64)
        {
            // We have to divide size by the region size because new_block is already a `region_info` pointer
            struct region_info *second_part = new_block + 1 + (size / REGION_DATA_SIZE);
            second_part->next = new_block->next;                               // Set our second part of this block to the next of w/e the current block is
            second_part->bytes = (new_block->bytes - size) - REGION_DATA_SIZE; // The size of the new block is the size of the old one - the size we're taking - the size of the data region
            second_part->is_free = TRUE;
            new_block->next = second_part; // Set the current block to point to our new second part
            new_block->bytes = size;       // Reset the size on the new block
        }

        return new_block + 1;
    }

    new_block = get_bytes(last, size);
    if (new_block == NULL)
    {
        return NULL;
    }
    return new_block + 1; // Skip past the metadata and return the raw bytes
}

void beavfree(void *ptr)
{
    struct region_info *meta;
    if (ptr == NULL)
    {
        return;
    }

    meta = ptr - REGION_DATA_SIZE;
    meta->is_free = TRUE;
    // printf("\n%p - %p\n", meta + 1 + (meta->bytes / REGION_DATA_SIZE), meta->next);
    while (meta && meta + 1 + (meta->bytes / REGION_DATA_SIZE) == meta->next && meta->next->is_free)
    {
        meta->bytes = meta->bytes + REGION_DATA_SIZE + meta->next->bytes;
        meta->next = meta->next->next;
        meta = meta->next;
    }
}

void beavalloc_reset(void)
{
    brk(head);
    head = NULL;
}

void beavalloc_set_verbose(uint8_t verbose)
{
    global_verbose = verbose > 0;
}

void *beavcalloc(size_t nmemb, size_t size)
{
    size_t calc_size;
    void *ptr;
    calc_size = nmemb * size;
    ptr = beavalloc(calc_size);
    memset(ptr, 0, calc_size);
    return ptr;
}

void *beavrealloc(void *ptr, size_t size)
{
    struct region_info *meta;
    void *new_bytes;

    if (ptr == NULL)
    {
        return beavalloc(size);
    }

    meta = ptr - REGION_DATA_SIZE;
    if (meta->bytes >= size)
    {
        return ptr;
    }

    new_bytes = beavalloc(size);
    if (new_bytes == NULL)
    {
        return NULL;
    }

    memcpy(new_bytes, ptr, meta->bytes);
    beavfree(ptr);
    return new_bytes;
}

void beavalloc_dump(unsigned int leaks_only)
{
    struct region_info *curr = NULL;
    unsigned int i = 0;
    unsigned int leak_count = 0;
    unsigned int user_bytes = 0;
    unsigned int capacity_bytes = 0;
    unsigned int block_bytes = 0;
    unsigned int used_blocks = 0;
    unsigned int free_blocks = 0;

    if (leaks_only)
    {
        fprintf(stderr, "heap lost blocks\n");
    }
    else
    {
        fprintf(stderr, "heap map\n");
    }
    fprintf(stderr, "  %s\t%s\t%s\t%s"
                    "\t%s\t%s\t%s\t%s\t%s\t%s"
                    "\n",
            "blk no  ", "block add ", "next add  ", "data add  ",
            "blk off  ", "dat off  ", "capacity ", "size     ", "blk size ", "status   ");
    for (curr = head, i = 0; curr != NULL; curr = curr->next, i++)
    {
        if (leaks_only == FALSE || (leaks_only == TRUE && curr->is_free == FALSE))
        {
            fprintf(stderr, "  %u\t\t%9p\t%9p\t%9p\t%u\t\t%u\t\t"
                            "%u\t\t%u\t\t%u\t\t%s\t%c\n",
                    i,
                    curr,
                    curr->next,
                    curr + REGION_DATA_SIZE,
                    (unsigned)((void *)curr - (void *)head),
                    (unsigned)((void *)curr + REGION_DATA_SIZE - (void *)head),
                    (curr->is_free > 0) * (unsigned)curr->bytes,
                    (curr->is_free == 0) * (unsigned)curr->bytes,
                    (unsigned)(curr->bytes + REGION_DATA_SIZE),
                    curr->is_free ? "free  " : "in use",
                    curr->is_free ? '*' : ' ');
            if (!curr->is_free)
            {
                user_bytes += curr->bytes;
            }
            else
            {
                capacity_bytes += curr->bytes;
            }
            block_bytes += curr->bytes + REGION_DATA_SIZE;
            if (curr->is_free == FALSE && leaks_only == TRUE)
            {
                leak_count++;
            }
            if (curr->is_free == TRUE)
            {
                free_blocks++;
            }
            else
            {
                used_blocks++;
            }
        }
    }
    if (leaks_only)
    {
        if (leak_count == 0)
        {
            fprintf(stderr, "  *** No leaks found!!! That does NOT mean no leaks are possible. ***\n");
        }
        else
        {
            fprintf(stderr, "  %s\t\t\t\t\t\t\t\t\t\t\t\t"
                            "%u\t\t%u\t\t%u\n",
                    "Total bytes lost", capacity_bytes, user_bytes, block_bytes);
        }
    }
    else
    {
        fprintf(stderr, "  %s\t\t\t\t\t\t\t\t\t\t"
                        "%u\t\t%u\t\t%u\n",
                "Total bytes used", capacity_bytes, user_bytes, block_bytes);
        fprintf(stderr, "  Used blocks: %u  Free blocks: %u  "
                        "Min heap: %p    Max heap: %p\n",
                used_blocks, free_blocks, (void *)head, block_bytes + (void *)head);
    }
}
