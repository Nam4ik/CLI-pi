#include <stdio.h>
#include <stdlib.h>
#include <mpfr.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h> 

// -lmpfr -lgmp -lm -Wall -Wextra -Werror

// Gauss-Legendre method
int calculate_pi(int len, bool infinite_mode, int delay_ms) {
    mp_bitcnt_t precision = (mp_bitcnt_t)(len * 3.321928094 + 100); // Увеличил запас точности

    mpfr_t a, b, t, p, a_next, t_tmp, p_tmp, pi;
    mpfr_inits2(precision, a, b, t, p, a_next, t_tmp, p_tmp, pi, (mpfr_ptr)0);


    mpfr_set_d(a, 1.0, MPFR_RNDN);
    mpfr_sqrt_ui(b, 2, MPFR_RNDN);  // b = sqrt(2)
    mpfr_ui_div(b, 1, b, MPFR_RNDN); // b = 1/sqrt(2)
    mpfr_set_d(t, 0.25, MPFR_RNDN);
    mpfr_set_d(p, 1.0, MPFR_RNDN);

    int iterations = (int)(log2(len)) + 3;

    for (int i = 0; i < iterations; i++) {
        // a_next = (a + b)/2
        mpfr_add(a_next, a, b, MPFR_RNDN);
        mpfr_div_2ui(a_next, a_next, 1, MPFR_RNDN);

        // b = sqrt(a * b)
        mpfr_mul(b, a, b, MPFR_RNDN);
        mpfr_sqrt(b, b, MPFR_RNDN);

        // t_tmp = t - p * (a - a_next)^2
        mpfr_sub(t_tmp, a, a_next, MPFR_RNDN);
        mpfr_sqr(t_tmp, t_tmp, MPFR_RNDN);
        mpfr_mul(t_tmp, p, t_tmp, MPFR_RNDN);
        mpfr_sub(t, t, t_tmp, MPFR_RNDN);

        // p = 2 * p
        mpfr_mul_2ui(p, p, 1, MPFR_RNDN);

        // a = a_next
        mpfr_set(a, a_next, MPFR_RNDN);
    }

    // π = (a + b)^2 / (4 * t)
    mpfr_add(pi, a, b, MPFR_RNDN);
    mpfr_sqr(pi, pi, MPFR_RNDN);
    mpfr_mul_ui(t, t, 4, MPFR_RNDN);
    mpfr_div(pi, pi, t, MPFR_RNDN);

    if (!infinite_mode) {
    
        printf("π = ");
        mpfr_out_str(stdout, 10, len, pi, MPFR_RNDN);
        printf("\n");
    } else {
    
        mpfr_exp_t exp;
        char *pi_str;

        pi_str = mpfr_get_str(NULL, &exp, 10, 0, pi, MPFR_RNDN);

        if (pi_str == NULL) {
            fprintf(stderr, "Error occurred while translating pi to char\n");
            mpfr_clears(a, b, t, p, a_next, t_tmp, p_tmp, pi, (mpfr_ptr)0);
            return 1;
        }

    
        if (exp == 1) {
    
            printf("π = %c", pi_str[0]);
            fflush(stdout); 
            if (delay_ms > 0) {
                 struct timespec ts;
                 ts.tv_sec = delay_ms / 1000;
                 ts.tv_nsec = (delay_ms % 1000) * 1000000L;
                 nanosleep(&ts, NULL);
            }

            printf(".");
            fflush(stdout);
            if (delay_ms > 0) {
                struct timespec ts;
                ts.tv_sec = delay_ms / 1000;
                ts.tv_nsec = (delay_ms % 1000) * 1000000L;
                nanosleep(&ts, NULL);
            }


            for (int i = 1; pi_str[i] != '\0'; i++) {
                printf("%c", pi_str[i]);
                fflush(stdout);
                if (delay_ms > 0) {
                    struct timespec ts;
                    ts.tv_sec = delay_ms / 1000;
                    ts.tv_nsec = (delay_ms % 1000) * 1000000L;
                    nanosleep(&ts, NULL);
                }
            }
            printf("\n");
        } else {
             
            printf("π = 0.%se%ld\n", pi_str, (long)exp); 
        }
        
        mpfr_free_str(pi_str);
    }

    mpfr_clears(a, b, t, p, a_next, t_tmp, p_tmp, pi, (mpfr_ptr)0);
    return 0;
}

int main(int argc, char *argv[]) {
    int delay_ms = 500; 
    int count = 0;      
    bool infinite_mode = false;
    bool help = false;          
    int opt;


    while ((opt = getopt(argc, argv, "ihc:t:")) != -1) {
        switch (opt) {
            case 'i':
                infinite_mode = true;
                break;
            case 'h':
                help = true;
                break;
            case 'c':
                count = atoi(optarg);
                if(count <= 0) {
                    fprintf(stderr, "Invalid value for -c. Must be a positive integer.\n");
                    return 1;
                }
                break;
            case 't':
                delay_ms = atoi(optarg);
                if(delay_ms < 0) {
                    fprintf(stderr, "Invalid value for -t. Must be a non-negative integer.\n");
                    return 1;
                }
                break;
            case '?':
              
                if (optopt == 'c' || optopt == 't') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option: -%c\n", optopt);
                }
                return 1;
            default:
                abort();
        }
    }

    if (help) {
        printf("tpi - C-written, MPFR-based CLI program for calculating π\n");
        printf("ARGS:\n");
        printf("  -h        - Showing this message.\n");
        printf("  -i        - Infinitely outputs the digits of pi one at a time.\n");
        printf("  -c=int    - Outputs the number of digits of pi passed to the argument.\n");
        printf("  -t=int    - Setting the output speed in -i mode (milliseconds). The default value is 500.\n");
        return 0;
    }

    
    if (infinite_mode) {
      
        int internal_precision = 1000000; 
        if(count > 0 && count < internal_precision) {
             internal_precision = count; 
        }
      
        calculate_pi(internal_precision, true, delay_ms);
    } else if (count > 0) {
      
        calculate_pi(count, false, 0); 
    } else {
      
        calculate_pi(500, false, 0);
    }

    return 0;
}
