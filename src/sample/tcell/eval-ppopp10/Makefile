all: st-serial st-serial-call st-serial-call-cas st-serial-call-membar st-serial-call-800 st-serial-call-cas-800 st-serial-call-membar-800

st-serial: st-ser.c
	gcc -O2 st-ser.c -o $@

st-serial-call: st.c
	gcc -O2 st.c -o $@

st-serial-call-cas: st.c
	gcc -O2 st.c -DPARALLEL_SEARCH -o $@

st-serial-call-membar: st.c
	gcc -O2 st.c -DPARALLEL_SEARCH3 -o $@

st-serial-call-800: st.c
	gcc -O2 st.c -DCALL_BOUND=800 -o $@

st-serial-call-cas-800: st.c
	gcc -O2 st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH -o $@

st-serial-call-membar-800: st.c
	gcc -O2 st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH3 -o $@
