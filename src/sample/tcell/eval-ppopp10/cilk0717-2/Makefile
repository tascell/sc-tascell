PROG_C = st st-par st-par2 st-par3 st-ser
PROG_CILK = st-cilk st-par-cilk st-par2-cilk st-par3-cilk \
	st-cilk-s st-par-cilk-s st-par2-cilk-s st-par3-cilk-s \
	st-cilk-800 st-par-cilk-800 st-par2-cilk-800 st-par3-cilk-800 \
	st-cilk-s-800 st-par-cilk-s-800 st-par2-cilk-s-800 st-par3-cilk-s-800

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
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH st.cilk  -o st-par-cilk

st-par2-cilk: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH2 st.cilk  -o st-par2-cilk

st-par3-cilk: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH3 st.cilk  -o st-par3-cilk

st-cilk-s: st_s.cilk
	cilkc $(GCC_OPTION) st_s.cilk  -o st-cilk-s

st-par-cilk-s: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH st_s.cilk  -o st-par-cilk-s

st-par2-cilk-s: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH2 st_s.cilk  -o st-par2-cilk-s

st-par3-cilk-s: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DPARALLEL_SEARCH3 st_s.cilk  -o st-par3-cilk-s

st-cilk-800: st.cilk
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 st.cilk  -o st-cilk-800

st-par-cilk-800: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH st.cilk  -o st-par-cilk-800

st-par2-cilk-800: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH2 st.cilk  -o st-par2-cilk-800

st-par3-cilk-800: st.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH3 st.cilk  -o st-par3-cilk-800

st-cilk-s-800: st_s.cilk
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 st_s.cilk  -o st-cilk-s-800

st-par-cilk-s-800: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH st_s.cilk  -o st-par-cilk-s-800

st-par2-cilk-s-800: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH2 st_s.cilk  -o st-par2-cilk-s-800

st-par3-cilk-s-800: st_s.cilk xccmem.h
	cilkc $(GCC_OPTION) -DCALL_BOUND=800 -DPARALLEL_SEARCH3 st_s.cilk  -o st-par3-cilk-s-800

st-ser-cilk: st-ser.cilk
	cilkc -O2 st-ser.cilk  -o st-ser-cilk


clean:
	rm -f $(PROG_C) $(PROG_CILK) *~
