all: st-serial st-serial-call st-serial-call-cas st-serial-call-membar st-serial-call-800 st-serial-call-cas-800 st-serial-call-membar-800

st-serial:
	gcc -O2 st-ser.c -o $@

st-serial-call:
	gcc -O2 st.c -o $@

st-serial-call-cas:
	gcc -O2 st.c -DPARALLEL_SEARCH -o $@

st-serial-call-membar:
	gcc -O2 st.c -DPARALLEL_SEARCH3 -o $@

st-serial-call-800:
	gcc -O2 st.c -DCALL_DEPTH=800 -o $@

st-serial-call-cas-800:
	gcc -O2 st.c -DCALL_DEPTH=800 -DPARALLEL_SEARCH -o $@

st-serial-call-membar-800:
	gcc -O2 st.c -DCALL_DEPTH=800 -DPARALLEL_SEARCH3 -o $@
