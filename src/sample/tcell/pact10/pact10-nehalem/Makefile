OPT=-O2 -march=core2

all: st-serial st-serial-call st-serial-call-cas st-serial-call-membar st-serial-call-800 st-serial-call-cas-800 st-serial-call-membar-800

st-serial: st-ser.c
	gcc $(OPT) st-ser.c -o $@

st-serial-call: st.c
	gcc $(OPT) st.c -o $@

st-serial-call-cas: st.c
	gcc $(OPT) st.c -DPARALLEL_SEARCH -o $@

st-serial-call-membar: st.c
	gcc $(OPT) st.c -DPARALLEL_SEARCH3 -o $@

st-serial-call-800: st.c
	gcc $(OPT) st.c -DCALL_BOUND=800 -o $@

st-serial-call-cas-800: st.c
	gcc $(OPT) st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH -o $@

st-serial-call-membar-800: st.c
	gcc $(OPT) st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH3 -o $@
