int fib(proc_env pr, int (*bkf)(req_buf_t *), int k){
  int s = 0;			/* the sum of solutions */
  void **saved_req_port_p;	/* (req_buf_t **) */
  int result_buf;
  int result_port;
  int restp;
  if(k <= 2) return 1;
  restp = 1;
  {
    int ret = 1;
    int (*bkf)(req_buf_t *) = bk;
    int bk(req_buf_t *req_buf_p){
      if(ret) ret = bkf(req_buf_p);
      if(ret) return 1;
      if(restp){
	task_buf_t *tbp = req_buf_p->task_buf_p;
	int *tpp        = req_buf_p->task_port_p;
	restp = 0;
	/* save info for "request back" */
	saved_req_port_p = req_buf_p->req_port_p;
	/* result not available yet */
	result_port = 0;
	/* put a task */
	tbp->f = tf_fib;
	tbp->sender = pr->myid;
	tbp->a.t1.result_buf_p = &result_buf;
	tbp->a.t1.result_port_p = &result_port;
	tbp->a.t1.k = k-2;
	/* transfer the task */
	finish_write_before_write();
	atomic_write_int(*tpp, 1);
	return 1;
      }
      return 0;
    }
    POLL(pr->req_port, bk);
    s += fib(pr, bk, k-1);
  }
  if(restp){
    restp = 0;
    s += fib(pr, bk, k-2);
  }else{
    while(atomic_read_int(result_port) == 0)
      /* request back a task and run*/
      steal_run_task(pr, saved_req_port_p, 1);
    start_read_after_read();
    s += result_buf;
  }
  return s;
}
