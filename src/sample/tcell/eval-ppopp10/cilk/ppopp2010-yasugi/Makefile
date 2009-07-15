PROG_C = st st-par st-par2 st-par3 st-ser
PROG_CILK = st-cilk st-par-cilk st-par2-cilk st-par3-cilk \
	st-par-cilk-slow st-par2-cilk-slow st-par3-cilk-slow 

GCC_OPTION = -O2 -g

all: $(PROG_CILK)

st: st.c
	gcc $(GCC_OPTION) st.c  -o st

st-par: st.c xccmem.h
	gcc $(GCC_OPTION) -DPARALLEL_SEARCH st.c  -o st-par

st-par2: st.c xccmem.h
	gcc $(GCC_OPTION) -DPARALLEL_SEARCH2 st.c  -o st-par2

st-par3: st.c xccmem.h
	gcc $(GCC_OPTION) -DPARALLEL_SEARCH3 st.c  -o st-par3

st-ser: st-ser.c
	gcc $(GCC_OPTION) st-ser.c  -o st-ser


st-cilk: st.cilk
	cilkc $(GCC_OPTION) st.cilk  -o st-cilk

st-par-cilk: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH -DUSE_SYNCHED st.cilk  -o st-par-cilk

st-par2-cilk: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH2 -DUSE_SYNCHED st.cilk  -o st-par2-cilk

st-par3-cilk: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH3 -DUSE_SYNCHED st.cilk  -o st-par3-cilk

st-par-cilk-slow: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH st.cilk  -o st-par-cilk-slow

st-par2-cilk-slow: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH2 st.cilk  -o st-par2-cilk-slow

st-par3-cilk-slow: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH3 st.cilk  -o st-par3-cilk-slow


st-ser-cilk: st-ser.cilk
	cilkc -O2 st-ser.cilk  -o st-ser-cilk


clean:
	rm -f $(PROG_C) $(PROG_CILK) *~
