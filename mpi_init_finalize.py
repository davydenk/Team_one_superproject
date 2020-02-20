import ctypes as ct
import sys

my_mpi_lib = ct.CDLL('../lib/mpi_init_finalize.so',ct.RTLD_GLOBAL)

LP_c_char = ct.POINTER(ct.c_char)
LP_LP_c_char = ct.POINTER(LP_c_char)

my_mpi_lib.initialize_mpi.argtypes = (ct.c_int, LP_LP_c_char)

argv = sys.argv 
argc = len(argv)
p = (LP_c_char*len(argv))()

def mpi_start(): 
  for i, arg in enumerate(argv):  
    enc_arg = arg.encode('utf-8')
    p[i] = ct.create_string_buffer(enc_arg)
  na = ct.cast(p, LP_LP_c_char)
  my_mpi_lib.initialize_mpi(argc, na) 

def mpi_end():
  my_mpi_lib.finalize_mpi()
 
