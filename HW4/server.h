#ifndef FILE__H
#define FILE__H
struct ext_struct {
    int  *in;
    int client_calls;
    int sum;
};

extern pthread_mutex_t runsum;
extern struct ext_struct sum_struct;
#endif
