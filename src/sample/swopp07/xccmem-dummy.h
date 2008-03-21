#define atomic_read_ptr(x) (x)
#define atomic_read_int(x) (x)

#define atomic_write_ptr(x,y) ((x)=(y))
#define atomic_write_int(x,y) ((x)=(y))

#define start_read_after_read() 0
#define finish_write_before_write() 0

#define cas_ptr(x,y,z) (((x)==(y))?(((x)=(z)),0):1)
#define cas_int(x,y,z) (((x)==(y))?(((x)=(z)),0):1)

#define systhr_create(work,arg) 0
#define systhr_bind_to_proc(pid) 0
#define systhr_init(n) 0
