#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define REPEATS 50000

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */
double dclock()
{
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (gtod_ref_time_sec == 0.0)
    gtod_ref_time_sec = (double)tv.tv_sec;
  norm_sec = (double)tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}

/* C equivalent of the remove_ctrl function */
char* remove_ctrl(const char* s, char* result)
{
    size_t result_len = 0;
    size_t s_len = strlen(s);
    
    for (size_t begin = 0, i = begin, end = s_len; begin < end; begin = i + 1) {
        for (i = begin; i < end; i++) {
            if (s[i] < 0x20) break;
        }
        memcpy(result + result_len, s + begin, i - begin);
        result_len += (i - begin);
    }
    
    result[result_len] = '\0';
    return result;
}

int main(int argc, const char* argv[])
{
    int i, j, k, iret = 0;
    double dtime;
    
    printf("call to remove\n");
    
    /* Read input */
    char* s = NULL;
    size_t s_size = 0;
    size_t s_capacity = 0;
    
    char line[1024];
    while (fgets(line, sizeof(line), stdin)) {
        size_t line_len = strlen(line);
        
        /* Ensure buffer is large enough */
        if (s_size + line_len + 1 > s_capacity) {
            s_capacity = s_capacity == 0 ? 4096 : s_capacity * 2;
            char* new_s = (char*)realloc(s, s_capacity);
            if (!new_s) {
                perror("Failed to allocate memory");
                free(s);
                return 1;
            }
            s = new_s;
        }
        
        /* Append line to buffer */
        memcpy(s + s_size, line, line_len);
        s_size += line_len;
    }
    
    /* Ensure null termination */
    if (s) {
        s[s_size] = '\0';
    } else {
        /* Handle empty input */
        s = (char*)malloc(1);
        if (!s) {
            perror("Failed to allocate memory");
            return 1;
        }
        s[0] = '\0';
        s_capacity = 1;
    }
    
    /* Allocate result buffer */
    char* result = (char*)malloc(s_capacity);
    if (!result) {
        perror("Failed to allocate memory for result");
        free(s);
        return 1;
    }
    
    /* Benchmark the removal operation */
    dtime = dclock();
    for (i = 0; i < REPEATS; i++) {
        remove_ctrl(s, result);
    }
    dtime = dclock() - dtime;
    
    /* Output results */
    printf("%s\n", result);
    printf("Time: %f\n", dtime);
    fflush(stdout);
    
    /* Clean up */
    free(s);
    free(result);
    
    return iret;
}