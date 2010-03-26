#ifndef xcc_atomic_read_char_to_finish_access
#define xcc_atomic_read_char_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_char(loc); })
#endif
#ifndef xcc_atomic_read_short_to_finish_access
#define xcc_atomic_read_short_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_short(loc); })
#endif
#ifndef xcc_atomic_read_int_to_finish_access
#define xcc_atomic_read_int_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_int(loc); })
#endif
#ifndef xcc_atomic_read_long_to_finish_access
#define xcc_atomic_read_long_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_long(loc); })
#endif
#ifndef xcc_atomic_read_long_long_to_finish_access
#define xcc_atomic_read_long_long_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_long_long(loc); })
#endif
#ifndef xcc_atomic_read_ptr_to_finish_access
#define xcc_atomic_read_ptr_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_ptr(loc); })
#endif
#ifndef xcc_atomic_read_float_to_finish_access
#define xcc_atomic_read_float_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_float(loc); })
#endif
#ifndef xcc_atomic_read_double_to_finish_access
#define xcc_atomic_read_double_to_finish_access(loc) \
 ({ xcc_albar(); xcc_atomic_read_double(loc); })
#endif

#ifndef xcc_atomic_read_char_to_finish_read
#define xcc_atomic_read_char_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_char(loc); })
#endif
#ifndef xcc_atomic_read_short_to_finish_read
#define xcc_atomic_read_short_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_short(loc); })
#endif
#ifndef xcc_atomic_read_int_to_finish_read
#define xcc_atomic_read_int_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_int(loc); })
#endif
#ifndef xcc_atomic_read_long_to_finish_read
#define xcc_atomic_read_long_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_long(loc); })
#endif
#ifndef xcc_atomic_read_long_long_to_finish_read
#define xcc_atomic_read_long_long_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_long_long(loc); })
#endif
#ifndef xcc_atomic_read_ptr_to_finish_read
#define xcc_atomic_read_ptr_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_ptr(loc); })
#endif
#ifndef xcc_atomic_read_float_to_finish_read
#define xcc_atomic_read_float_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_float(loc); })
#endif
#ifndef xcc_atomic_read_double_to_finish_read
#define xcc_atomic_read_double_to_finish_read(loc) \
 ({ xcc_llbar(); xcc_atomic_read_double(loc); })
#endif

#ifndef xcc_atomic_read_char_to_finish_write
#define xcc_atomic_read_char_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_char(loc); })
#endif
#ifndef xcc_atomic_read_short_to_finish_write
#define xcc_atomic_read_short_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_short(loc); })
#endif
#ifndef xcc_atomic_read_int_to_finish_write
#define xcc_atomic_read_int_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_int(loc); })
#endif
#ifndef xcc_atomic_read_long_to_finish_write
#define xcc_atomic_read_long_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_long(loc); })
#endif
#ifndef xcc_atomic_read_long_long_to_finish_write
#define xcc_atomic_read_long_long_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_long_long(loc); })
#endif
#ifndef xcc_atomic_read_ptr_to_finish_write
#define xcc_atomic_read_ptr_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_ptr(loc); })
#endif
#ifndef xcc_atomic_read_float_to_finish_write
#define xcc_atomic_read_float_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_float(loc); })
#endif
#ifndef xcc_atomic_read_double_to_finish_write
#define xcc_atomic_read_double_to_finish_write(loc) \
 ({ xcc_slbar(); xcc_atomic_read_double(loc); })
#endif

*****

#ifndef xcc_atomic_swap_char_to_finish_access
#define xcc_atomic_swap_char_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_char((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_short_to_finish_access
#define xcc_atomic_swap_short_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_short((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_int_to_finish_access
#define xcc_atomic_swap_int_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_int((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_to_finish_access
#define xcc_atomic_swap_long_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_access
#define xcc_atomic_swap_long_long_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_long_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_access
#define xcc_atomic_swap_ptr_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_ptr((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_float_to_finish_access
#define xcc_atomic_swap_float_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_float((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_double_to_finish_access
#define xcc_atomic_swap_double_to_finish_access(loc,ov) \
 ({ xcc_asbar(); xcc_atomic_swap_double((loc),(ov)); })
#endif

#ifndef xcc_atomic_swap_char_to_finish_read
#define xcc_atomic_swap_char_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_char((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_short_to_finish_read
#define xcc_atomic_swap_short_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_short((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_int_to_finish_read
#define xcc_atomic_swap_int_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_int((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_to_finish_read
#define xcc_atomic_swap_long_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_read
#define xcc_atomic_swap_long_long_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_long_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_read
#define xcc_atomic_swap_ptr_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_ptr((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_float_to_finish_read
#define xcc_atomic_swap_float_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_float((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_double_to_finish_read
#define xcc_atomic_swap_double_to_finish_read(loc,ov) \
 ({ xcc_lsbar(); xcc_atomic_swap_double((loc),(ov)); })
#endif

#ifndef xcc_atomic_swap_char_to_finish_write
#define xcc_atomic_swap_char_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_char((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_short_to_finish_write
#define xcc_atomic_swap_short_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_short((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_int_to_finish_write
#define xcc_atomic_swap_int_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_int((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_to_finish_write
#define xcc_atomic_swap_long_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_long_long_to_finish_write
#define xcc_atomic_swap_long_long_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_long_long((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_ptr_to_finish_write
#define xcc_atomic_swap_ptr_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_ptr((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_float_to_finish_write
#define xcc_atomic_swap_float_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_float((loc),(ov)); })
#endif
#ifndef xcc_atomic_swap_double_to_finish_write
#define xcc_atomic_swap_double_to_finish_write(loc,ov) \
 ({ xcc_ssbar(); xcc_atomic_swap_double((loc),(ov)); })
#endif

*******

#ifndef xcc_atomic_write_char_to_finish_access
#define xcc_atomic_write_char_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_char((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_finish_access
#define xcc_atomic_write_short_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_short((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_finish_access
#define xcc_atomic_write_int_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_int((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_finish_access
#define xcc_atomic_write_long_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_finish_access
#define xcc_atomic_write_long_long_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_long_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_finish_access
#define xcc_atomic_write_ptr_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_ptr((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_finish_access
#define xcc_atomic_write_float_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_float((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_finish_access
#define xcc_atomic_write_double_to_finish_access(loc,val) \
 do{ xcc_asbar(); xcc_atomic_write_double((loc),(val)); }while(0)
#endif

#ifndef xcc_atomic_write_char_to_finish_read
#define xcc_atomic_write_char_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_char((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_finish_read
#define xcc_atomic_write_short_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_short((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_finish_read
#define xcc_atomic_write_int_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_int((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_finish_read
#define xcc_atomic_write_long_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_finish_read
#define xcc_atomic_write_long_long_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_long_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_finish_read
#define xcc_atomic_write_ptr_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_ptr((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_finish_read
#define xcc_atomic_write_float_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_float((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_finish_read
#define xcc_atomic_write_double_to_finish_read(loc,val) \
 do{ xcc_lsbar(); xcc_atomic_write_double((loc),(val)); }while(0)
#endif

#ifndef xcc_atomic_write_char_to_finish_write
#define xcc_atomic_write_char_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_char((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_short_to_finish_write
#define xcc_atomic_write_short_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_short((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_int_to_finish_write
#define xcc_atomic_write_int_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_int((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_to_finish_write
#define xcc_atomic_write_long_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_long_long_to_finish_write
#define xcc_atomic_write_long_long_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_long_long((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_ptr_to_finish_write
#define xcc_atomic_write_ptr_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_ptr((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_float_to_finish_write
#define xcc_atomic_write_float_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_float((loc),(val)); }while(0)
#endif
#ifndef xcc_atomic_write_double_to_finish_write
#define xcc_atomic_write_double_to_finish_write(loc,val) \
 do{ xcc_ssbar(); xcc_atomic_write_double((loc),(val)); }while(0)
#endif

******

#ifndef xcc_cas_char_to_finish_access
#define xcc_cas_char_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_char((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_short_to_finish_access
#define xcc_cas_short_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_short((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_int_to_finish_access
#define xcc_cas_int_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_int((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_to_finish_access
#define xcc_cas_long_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_long_to_finish_access
#define xcc_cas_long_long_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_long_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_ptr_to_finish_access
#define xcc_cas_ptr_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_ptr((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_float_to_finish_access
#define xcc_cas_float_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_float((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_double_to_finish_access
#define xcc_cas_double_to_finish_access(loc,ov,nv) \
 ({ xcc_asbar(); xcc_cas_double((loc),(ov),(nv)); })
#endif

#ifndef xcc_cas_char_to_finish_read
#define xcc_cas_char_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_char((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_short_to_finish_read
#define xcc_cas_short_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_short((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_int_to_finish_read
#define xcc_cas_int_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_int((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_to_finish_read
#define xcc_cas_long_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_long_to_finish_read
#define xcc_cas_long_long_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_long_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_ptr_to_finish_read
#define xcc_cas_ptr_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_ptr((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_float_to_finish_read
#define xcc_cas_float_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_float((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_double_to_finish_read
#define xcc_cas_double_to_finish_read(loc,ov,nv) \
 ({ xcc_lsbar(); xcc_cas_double((loc),(ov),(nv)); })
#endif

#ifndef xcc_cas_char_to_finish_write
#define xcc_cas_char_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_char((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_short_to_finish_write
#define xcc_cas_short_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_short((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_int_to_finish_write
#define xcc_cas_int_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_int((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_to_finish_write
#define xcc_cas_long_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_long_long_to_finish_write
#define xcc_cas_long_long_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_long_long((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_ptr_to_finish_write
#define xcc_cas_ptr_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_ptr((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_float_to_finish_write
#define xcc_cas_float_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_float((loc),(ov),(nv)); })
#endif
#ifndef xcc_cas_double_to_finish_write
#define xcc_cas_double_to_finish_write(loc,ov,nv) \
 ({ xcc_ssbar(); xcc_cas_double((loc),(ov),(nv)); })
#endif
