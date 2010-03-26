#!/bin/sh
gcc st.c -O2 -o st-serial
gcc st.c -O2 -o st-serial-call
gcc st.c -DPARALLEL_SEARCH -O2 -o st-serial-call-cas
gcc st.c -DPARALLEL_SEARCH3 -O2 -o st-serial-call-membar
