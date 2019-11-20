#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "benny_thread.h"

#ifdef KTHREADS

#ifndef TRUE
# define TRUE 1
#endif // TRUE
#ifndef FALSE
# define FALSE 0
#endif // FALSE

// I have run with 10 threads, but not more
#define MAX_THREADS 10

// if you go much higher than this, it causes problems. memory exhaustion
#define MAX_MATRIX_SIZE 30

#define MYRAND_MAX  (1 << 31)

#ifndef MATRIX_TYPE
# define MATRIX_TYPE int
#endif // MATRIX_TYPE

typedef struct matrix_s {
    //char *file_name; // not used in xv6
    uint rows;
    uint cols;
    MATRIX_TYPE **data;
} matrix_t;

static long numThreads = 1;
static matrix_t *omatrix = NULL;    // the output matrix - goes to default file
static matrix_t *lmatrix = NULL;    // the lefthand matrix
static matrix_t *rmatrix = NULL;    // the righthand matrix

static void gen_data(matrix_t *matrix);
static void free_matrix(matrix_t *matrix);
static void calc_row(matrix_t *lmatrix, matrix_t *rmatrix, matrix_t *omatrix, uint row);
static void output_matrix(matrix_t *matrix, char *fname);

static void cal_rowThreadStep(void *arg);

static const unsigned long DIVISOR = (1 << 16);
static unsigned long next = 1;

// taken from the rand(3) man page.
static unsigned long
myrand(void)
{
    next = next * 1103515245 + 12345;
    
    return((unsigned) (next / DIVISOR) % MYRAND_MAX);
}

static void
mysrand(unsigned seed)
{
    next = seed;
}
#endif // KTHREADS

int
main(int argc, char **argv)
{
#ifdef KTHREADS
    int isVerbose = FALSE;

    // 3 really is a good number
    mysrand(3);

    lmatrix = malloc(sizeof(matrix_t));
    rmatrix = malloc(sizeof(matrix_t));
    omatrix = malloc(sizeof(matrix_t));

    if (argc < 2) {
        printf(2, "You must specify the number of threads to use\n");
        exit();
    }

    numThreads = atoi(argv[1]);
    printf(1, "num threads %d\n", numThreads);
    if (numThreads < 1 || numThreads > MAX_THREADS) {
        printf(2, "Bad thread count\n");
        exit();
    }

    {
        long tcount = 0;
        // I'm trying to keep some presure off of malloc()
        benny_thread_t wthreads[MAX_THREADS];

        gen_data(lmatrix);
        gen_data(rmatrix);

        printf(1, "%s %d: %d\n", __FILE__, __LINE__, numThreads);
        omatrix->rows = lmatrix->rows;
        omatrix->cols = rmatrix->cols;
        omatrix->data = malloc(omatrix->rows * sizeof(MATRIX_TYPE *));

        if (lmatrix->cols == rmatrix->rows) {
            printf(1, "%s %d: %d\n", __FILE__, __LINE__, numThreads);

            for (tcount = 0; tcount < numThreads; tcount++) {
                benny_thread_create(&(wthreads[tcount]), cal_rowThreadStep
                               , (void *) tcount);
                printf(1, "  created thread %d %d\n", benny_thread_tid(wthreads[tcount]), tcount);
                if (benny_thread_tid(wthreads[tcount]) < 1) {
                    exit();
                }
                //sleep(2);
            }
            for (tcount = 0; tcount < numThreads; tcount++) {
                printf(1, "  join thread %d %d\n", benny_thread_tid(wthreads[tcount]), tcount);
                benny_thread_join(wthreads[tcount]);
            }
        }
        else {
            printf(2, "*** the left matrix must have the "
                    "same number of colums as the right matrix has rows ***\n");
        }
    }

    // its the output that takes most of the time
    output_matrix(omatrix, "op.txt");
    if (isVerbose) {
        output_matrix(lmatrix, "left.txt");
        output_matrix(rmatrix, "right.txt");
    }

    free_matrix(lmatrix);
    free_matrix(rmatrix);
    free_matrix(omatrix);

#endif // KTHREADS

    exit();
}

#ifdef KTHREADS
void 
cal_rowThreadStep(void *arg)
{
    long tid = (long) arg;
    int row = tid;

    //printf(1, "thread: %s %d: %d\n", __FILE__, __LINE__, tid);

    // We are using static work allocation for this simple matrix
    // multiplication example.
    for (row = tid; row < omatrix->rows; row += numThreads) {
        calc_row(lmatrix, rmatrix, omatrix, row);
    }

    benny_thread_exit(7);
}

void
gen_data(matrix_t *matrix)
{
    matrix->rows = matrix->cols = MAX_MATRIX_SIZE;
    {
        uint row = 0;
        uint col = 0;
        int neg = 0;
        
        // instead of reading input from a file, which is really messy in xv6
        // we will just generate the input values

        matrix->data = malloc(matrix->rows * sizeof(MATRIX_TYPE *));
        for (row = 0; row < matrix->rows; row++) {
            matrix->data[row] = malloc(matrix->cols * sizeof(MATRIX_TYPE));
            for (col = 0; col < matrix->cols; col++) {
                // I want to have a mix of positive and negative values.
                // if the generated value is even, leave the number positive
                // the the number is odd, negate value
                neg = (myrand() % 2) == 0 ? 1 : -1;
                matrix->data[row][col] = (myrand() % 100) * neg;
            }
        }
    }
}

void
free_matrix(matrix_t *matrix)
{
    uint row = 0;

    if (matrix != NULL) {
        for (row = 0; row < matrix->rows; row++) {
            free(matrix->data[row]);
        }
        free(matrix->data);
        free(matrix);
    }
}

void
calc_row(matrix_t *lmat, matrix_t *rmat, matrix_t *omat, uint calc_row)
{
    uint lcol = 0;
    uint rcol = 0;
    MATRIX_TYPE sum = 0;

    omat->data[calc_row] = malloc(omat->cols * sizeof(MATRIX_TYPE));

    for (rcol = 0; rcol < rmat->cols; rcol++) {
        sum = 0;
        for (lcol = 0; lcol < lmat->cols; lcol++) {
            sum += lmat->data[calc_row][lcol] * rmat->data[lcol][rcol];
        }
        omat->data[calc_row][rcol] = sum;
    }
}

void
output_matrix(matrix_t *matrix, char *fname)
{
    int file = 1;
    uint row = 0;
    uint col = 0;

    if (NULL != matrix) {
        // always goes into the same file name
        file = open(fname, O_CREATE | O_RDWR);

        printf(file,  "%d %d\n", matrix->rows, matrix->cols);
        for (row = 0; row < matrix->rows; row++) {
            for (col = 0; col < matrix->cols; col++) {
                printf(file, "%d ", matrix->data[row][col]);
            }
            printf(file, "\n");
        }
        printf(file, "\n");

        close(file);
    }
}

#endif // KTHREADS
