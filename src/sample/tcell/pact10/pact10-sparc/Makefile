OS=${shell uname}
OPTS=-O2
ifeq ($(OS),SunOS)
    OPTS += -mcpu=niagara2
endif

all: st-serial st-serial-call st-serial-call-cas st-serial-call-membar st-serial-call-800 st-serial-call-cas-800 st-serial-call-membar-800

# Not use for SPAA10
st-serial: st-ser.c
	gcc $(OPTS) st-ser.c -o $@

st-serial-call: st.c
	gcc $(OPTS) st.c -o $@

st-serial-call-cas: st.c
	gcc $(OPTS) st.c -DPARALLEL_SEARCH -o $@

st-serial-call-membar: st.c
	gcc $(OPTS) st.c -DPARALLEL_SEARCH3 -o $@

st-serial-call-800: st.c
	gcc $(OPTS) st.c -DCALL_BOUND=800 -o $@

st-serial-call-cas-800: st.c
	gcc $(OPTS) st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH -o $@

st-serial-call-membar-800: st.c
	gcc $(OPTS) st.c -DCALL_BOUND=800 -DPARALLEL_SEARCH3 -o $@
