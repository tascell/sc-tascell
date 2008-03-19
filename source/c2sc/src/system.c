/*-
 * Copyright (c) 1998, 2002-2004 Kiyoshi Matsui <kmatsui@t3.rim.or.jp>
 * All rights reserved.
 *
 * Some parts of this code are derived from the public domain software
 * DECUS cpp (1984,1985) written by Martin Minow.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 *                          S Y S T E M . C
 *          S y s t e m   D e p e n d e n t   R o u t i n e s
 *
 * Routines dependent on character set, O.S., compiler or compiler-driver.
 * To implement cpp for the systems not yet implemented, you must
 *      1. specify the constants in "configed.H" or "noconfig.H",
 *      2. append the system-dependent routines in this file.
 */

/*
 * Edit history of DECUS CPP / cpp1.c, cpp2.c, cpp3.c
 * 08-Nov-85            Latest revision
 */

/*
 * CPP Version 2.0 / system.c
 * 1998/08      kmatsui
 *      Created setoptlist(), setcplus(), bsl2sl(), put_depend(),
 *          dopragma(), doonce(), included(), dumppath(), is_junk(),
 *          alloc_mem(), print_heap();
 *      Split reopen_stdout(), setstdin() from main(),
 *          put_start_file(), putfname() from cppmain(),
 *          setfiles(), usage(), mem_model(), is_id() from dooptions(),
 *          doold(), dodebug(), doasm(), put_asm() from control().
 *      Split setincdirs() to setsysdirs(), setenvdirs(), parse_env(),
 *          set_a_dir().
 *      Moved sharp(), sharpsub(), getredirection()
 *              from cpp1.c to system.c,
 *          doinclude(), openinclude(), hasdirectory(), vmsparse()
 *              from cpp2.c to system.c,
 *          setincdirs(), dooptions(), zap_uc() from cpp3.c to system.c,
 *      Moved type[] from cpp6.c to system.c.
 *      Revised most of the functions.
 */

/*
 * CPP Version 2.2 / system.c
 * 1998/11      kmatsui
 *      Revised several functions.
 *      Removed alloc_mem().
 */

/*
 * CPP Version 2.3 pre-release 1 / system.c
 * 2002/08      kmatsui
 *      Changed specification of -S option and added -V, -h option.
 *      Extended -D option to enable function-like macro definition.
 *      Added -a (-lang-asm, -x assembler-with-cpp) option.
 *      Added handling of #pragma __include_next and #pragma __warning_cpp.
 *      Added handling of #include_next and #warning for GNU C.
 *      Added -dM, -dD, -include and -isystem option for GNU C.
 *      Fixed the bugs of parse_env() and bsl2sl().
 *      Created conv_case(), chk_env() and at_end().
 *      Split set_limit(), set_pragma_op(), def_a_macro() from dooptions().
 *      Split search_dir() from openinclude().
 *      Removed is_id().
 *      Renamed the several functions using underscore.
 *
 * CPP Version 2.3 pre-release 2 / system.c
 * 2002/12      kmatsui
 *      Added norm_path() to normalize include directories.
 *      Added -I- option and -std=xx option for GNU C.
 *      Fixed the bug of #pragma __include_next (#include_next).
 *
 * CPP Version 2.3 release / system.c
 * 2003/02      kmatsui
 *      Enabled options interspersed between the filename arguments.
 *      Added -j option (for the GNU C compatible diagnostic format).
 *      Changed #pragma __debug and #pragma __warning to #pragma __debug_cpp
 *          and #pragma __warning_cpp.
 *
 * CPP Version 2.3 patch 1 / system.c
 * 2003/03      kmatsui
 *      Revised the MODEs other than STANDARD.
 */

/*
 * MCPP Version 2.4 prerelease
 * 2003/11      kmatsui
 *      Changed some macro names accoding to config.h.
 *      Moved open_file() and add_file() from main.c to system.c.
 *      Added fnamelist[] and fname_end variables.
 *      Changed FILEINFO and DEFBUF, created set_fname() accordingly,
 *          revised open_include(), search_dir(), open_file(), add_file(),
 *          norm_path(), def_a_macro() and at_start().
 *      Implemented #pragma __push_macro (push_macro), #pragma __pop_macro
 *          (pop_macro), #pragma __preprocess and #pragma __preprocessed,
 *          created push_or_pop() and do_preprocess(), revised do_pragma()
 *          accordingly.
 *      Implemented -MF, -MT, -MQ, -MP options, created md_init() and
 *          md_quote().
 *      Moved sharp() from system.c to main.c.
 *      Added Visual C++ implementation and revised do_options(), usage(),
 *          set_opt_list() and do_pragma() accordingly.
 *      Removed VMS, DEC C and OS-9/09 settings and removed accordingly
 *          vmsparse(), reopen_stdout(), set_stdin(), get_redirection(),
 *          put_start_file(), put_fname() and put_source().
 *
 * MCPP Version 2.4 release
 * 2004/02      kmatsui
 *      Created mbchar.c and moved type[] to mbchar.c.
 *      Implemented -m option, #pragma __setlocale and enabled environment
 *          variable LC_ALL, LC_CTYPE and LANG.
 *      Added Plan 9 / pcc implementation.
 */

/*
 * The system/compiler dependent routines are placed here.
 */

#if PREPROCESSED
#include    "cpp.H"
#else
#include    "system.H"
#include    "internal.H"
#endif

/*
 * PATH_DELIM is defined for the O.S. which has single byte path-delimiter.
 * Note: '\\' or any other character identical to second byte of MBCHAR should
 * not be used for PATH_DELIM for convenience of path-list parsing.
 */
#if SYS_FAMILY == SYS_UNIX || SYS_FAMILY == SYS_OS9 || SYS_FAMILY == SYS_MSDOS
#define PATH_DELIM      '/'
#define SPECIAL_PATH_DELIM  FALSE
#else
#if SYSTEM == SYS_MAC
#define PATH_DELIM      ':'
#else   /* Any other path-delimiter, define by yourself */
#define PATH_DELIM      '/'
#endif
#define SPECIAL_PATH_DELIM  TRUE    /* Any path-delimiter other than '/'    */
#endif

/*
 * OBJEXT is the suffix to denote "object" file.
 */
#ifndef OBJEXT
#if SYS_FAMILY == SYS_UNIX || SYSTEM == SYS_DJGPP
#define OBJEXT     "o"
#else
#if SYS_FAMILY == SYS_MSDOS
#define OBJEXT     "obj"
#else
#if SYS_FAMILY == SYS_OS9
#define OBJEXT     "r"
#else
/* Add here appropriate definitions for other systems.  */
#define OBJEXT     "o"
#endif
#endif
#endif
#endif

#if PROTO

static void     version( void);
static void     usage( int opt);
static void     set_opt_list( char * optlist);
static void     def_a_macro( int opt);
#if MODE >= STANDARD
static void     set_cplus( char * val, const char * stdc_name
        , const char * stdc_v_name, int nflag);
static void     set_limit( void);
#if OK_PRAGMA_OP
static void     set_pragma_op( void);
#endif
#endif
static char *   set_files( int argc, char ** argv, char ** in_pp
        , char ** out_pp);
static void     set_sys_dirs( int set_cplus_dir);
#if HOST_HAVE_GETENV
static void     set_env_dirs( void);
static void     parse_env( const char * env);
#endif
static void     set_a_dir( const char * dirname);
static char *   norm_path( const char * dirname);
#if OK_MAKE
#if COMPILER == GNUC
static void     chk_env( void);
#endif
static char *   md_init( const char * filename, char * output);
static char *   md_quote( char * output);
#endif
static int      open_include( char * filename, int searchlocal, int next);
static int      has_directory( const char * source, char * directory);
static int      search_dir( char * filename, int searchlocal, int next);
static int      open_file( const char ** dirp, const char * filename
        , int local);
static const char *     set_fname( const char * filename);
static void     do_preprocessed( void);
#if ! OK_IF_JUNK
static int      is_junk( void);
#endif
#if MODE >= STANDARD
static void     do_once( const char * filename);
static int      included( const char * filename);
static void     push_or_pop( int direction);
#endif
#if DEBUG || DEBUG_EVAL
static int      do_debug( int set);
#endif
#if DEBUG
static void     dump_path( void);
#endif
#if MODE == PRE_STANDARD
static void     do_asm( int asm_start);
#endif

#else   /* ! PROTO  */

static void     version();      /* Print version message            */
static void     usage();        /* Putout usage of MCPP             */
static void     set_opt_list(); /* Set list of legal option chars   */
static void     def_a_macro();  /* Do a -D option                   */

#if MODE >= STANDARD
static void     set_cplus();    /* Set the macro __cplusplus        */
static void     set_limit();    /* Set minimum translation limits   */
#if OK_PRAGMA_OP
static void     set_pragma_op();    /* Set the _Pragma() operator   */
#endif
#endif
static char *   set_files();    /* Set input, output, diagnostic    */
static void     set_sys_dirs(); /* Set system-specific include dirs */
#if HOST_HAVE_GETENV
static void     set_env_dirs(); /* Set user-defined include dirs    */
static void     parse_env();    /* Parse environment variables      */
#endif
static void     set_a_dir();    /* Append an include directory      */
static char *   norm_path();    /* Normalize pathname to compare    */
#if OK_MAKE
#if COMPILER == GNUC
static void     chk_env();      /* Check the environment variables  */
#endif
static char *   md_init();      /* Initialize makefile dependency   */
static char *   md_quote();     /* 'Quote' special characters       */
#endif
static int      open_include(); /* Open the included file           */
static int      has_directory();    /* Get directory part of fname  */
static int      search_dir();   /* Search the include directories   */
static int      open_file();    /* Open a source file               */
static char *   set_fname();    /* Remember the source filename     */
static void     do_preprocessed();  /* Process preprocessed file    */
#if ! OK_IF_JUNK
static int      is_junk();      /* The directive has trailing junk? */
#endif
#if MODE >= STANDARD
static void     do_once();      /* Process #pragma __once           */
static int      included();     /* The file has been included ?     */
static void     push_or_pop();  /* Push or pop a macro definition   */
#endif
#if DEBUG || DEBUG_EVAL
static int      do_debug();     /* #pragma __debug_cpp, #debug      */
#endif
#if DEBUG
static void     dump_path();    /* Print include search path        */
#endif
#if MODE < STANDARD
static void     do_asm();       /* Process #asm, #endasm            */
#endif

#endif  /* ! PROTO  */

#if FOLD_CASE
static void     zap_uc();               /* Toupper option arguments */
#endif

#if SYSTEM == SYS_MSDOS
static int      mem_model( int model);  /* Specify memory-model     */
#else
static int      mb_changed = FALSE;     /* Flag of -m option        */
#endif

#if SYS_FAMILY == SYS_MSDOS
static char *   bsl2sl( char * filename, int in_source);
                                        /* Convert \ to /           */
#endif

/*
 * incdir[] stores the -I directories (and the system-specific #include <...>
 * directories).  This is set by set_a_dir().  A trailing PATH_DELIM is
 * appended if absent.
 */
static const char *     incdir[ NINCLUDE];      /* Include directories      */
static const char **    incend = incdir;        /* -> active end of incdir  */

/*
 * fnamelist[] stores the souce file names opened by #include directive for
 * debugging information.
 */
#define FNAMELIST   (NINCLUDE * 8)
static const char *     fnamelist[ FNAMELIST];  /* Source file names        */
static const char **    fname_end = fnamelist;
                                            /* -> active end of fnamelist   */

/*
 * 'search_rule' holds searching rule of #include "header.h" to search first
 * before searching user specified or system-specific include directories.
 * 'search_rule' is initialized to SEARCH_INIT.  It can be changed by -I1, -I2
 * or -I3 option.  -I1 specifies CURRENT, -I2 SOURCE and -I3 both.
 */

static int      search_rule = SEARCH_INIT;  /* Rule to search include file  */

static int      dDflag = FALSE;         /* Flag of -dD option (for GNU C)   */

#if OK_MAKE
static FILE *   mkdep_fp;                       /* For -Mx option   */
static char *   mkdep_target;
    /* For -MT TARGET option and for GNU C's queer environment variables.   */
static char *   mkdep_mf;               /* Argument of -MF option   */
static char *   mkdep_md;               /* Argument of -MD option   */
static char *   mkdep_mq;               /* Argument of -MQ option   */
static char *   mkdep_mt;               /* Argument of -MT option   */
#endif

#if COMPILER == GNUC
/* sys_dirp indicates the first directory to search for system headers  */
static const char **    sys_dirp = incdir;      /* For -I- option   */
#endif

#if COMPILER == GNUC || COMPILER == MSC
/*
 * preinclude points to the file specified by -include (-Fl for MSC) option,
 * which is included prior to the main input file.
 */
#define         NPREINCLUDE 8
char *          preinclude[ NPREINCLUDE];       /* File to pre-include      */
static char **  preinc_end = preinclude;    /* -> active end of preinclude  */
static int      dMflag = FALSE;                 /* Flag of -dM option       */
#endif

#if HOST_COMPILER == BORLANDC
extern unsigned     _stklen = NMACWORK + (NEXP * 30) + (sizeof (int) * 1280)
#if MODE >= STANDARD
                        + (sizeof (char *) * 12 * RESCAN_LIMIT)
#endif
                        + 0x800;
#endif

#if COMPILER == LCC
static const char *     optim_name = "__LCCOPTIMLEVEL";
#endif

#define LINE90LIMIT         32767
#define LINE_CPLUS_LIMIT    32767

#define OPTLISTLEN  128         // C2SC, original -> 64

void
#if PROTO
do_options( int argc, char ** argv, char ** in_pp, char ** out_pp)
#else
do_options( argc, argv, in_pp, out_pp)
    int         argc;
    char **     argv;
    char **     in_pp;                      /* Input file name      */
    char **     out_pp;                     /* Output file name     */
#endif
/*
 * Process command line arguments (-D, etc.), called only at cpp startup.
 */
{
    extern int      optind;                 /* Defined in getopt()  */
    extern char *   optarg;                 /*      ditto           */
    char        optlist[ OPTLISTLEN];       /* List of option letter*/
    const char *    warning = "warning: -%c%s option is ignored\n";
    int         vflag, nflag;               /* -v, -N options       */
    int         unset_sys_dirs;
    /* Unset system-specific and site-specific include directories ?    */
    int         set_cplus_dir;  /* Set C++ include directory ? (for GNU C)  */
    int         show_path;          /* Show include directory list  */
    DEFBUF *    defp;
    int         i;
    register int    opt;

#if MODE >= STANDARD
    const char *    stdc_name = "__STDC__";
    const char *    stdc_v_name = "__STDC_VERSION__";
    char        std_version[ 16];
    long        std_val;    /* Value of __STDC_VERSION__ or __cplusplus__   */
    VAL_SIGN    *valp;
    int         sflag, Vflag;               /* -S, -V, -H option    */
#endif
#if OK_MAKE || COMPILER == GNUC
    char *      cp;
#endif
#if SYSTEM == SYS_MSDOS
    int         memmodel = FALSE;
#endif
#if COMPILER == GNUC
#define NSYSDIR   8
    /* System include directory specified by -isystem   */
    char *      sysdir[ NSYSDIR] = { NULL, };
    char **     sysdir_end = sysdir;
#endif
#if COMPILER == LCC
    const char *    debug_name = "__LCCDEBUGLEVEL";
#endif

    set_opt_list( optlist);

    vflag = nflag = unset_sys_dirs = show_path = FALSE;
    set_cplus_dir = TRUE;
#if MODE >= STANDARD
    sflag = Vflag = FALSE;
#endif

opt_search: ;
    while (optind < argc
            && (opt = getopt( argc, argv, optlist)) != EOF) {

#if FOLD_CASE
        if (islower( opt))
            opt = toupper( opt);            /* Normalize the case   */
        if (optarg)
            zap_uc( optarg);                /* Touppers argument    */
#endif

        switch (opt) {          /* Command line option character    */
          // <- C2SC
        case 'e':
          c2sc_process_mode = *optarg - '0';
          if (optarg+1) 
            strcpy (sebase, optarg+1);
          // check the value of process mode and open other output files
          switch (c2sc_process_mode)
            {
            case 1:
              if ( sebase[0] != EOS ) {
                char sefile[BUFFER_SIZE];
                strcpy (sefile, sebase);
                strcat (sefile, ".se2");
                if ( (fp_se2 = fopen( sefile , "w")) == NULL) {
                  fprintf( fp_err, "Can't open output file \"%s\".\n", sefile);
                  exit( IO_ERROR);
                }
                strcpy (sefile, sebase);
                strcat (sefile, ".se3");
                if ( (fp_se3 = fopen( sefile , "w")) == NULL) {
                  fprintf( fp_err, "Can't open output file \"%s\".\n", sefile);
                  exit( IO_ERROR);
                }          
              }
              
            case 2:
            case 3:
              break;
              
            default:
              fprintf (fp_err, "Unexpected c2sc_process_mode '%d'.\n",
                       c2sc_process_mode);
              break;
            }    
          break;
          // C2SC ->

#if COMPILER == GNUC
        case 'A':       /* Ignore -A system(gnu), -A cpu(vax) or so */
            break;
#endif

#if MODE != POST_STANDARD && TOP_SPACE
#if FOLD_CASE
        case 'A':
#else
        case 'a':
            lang_asm = TRUE;                /* "assembler" source   */
            break;
#endif
#endif  /* MODE != POST_STANDARD && TOP_SPACE   */

#if ! STD_LINE_PREFIX
#if FOLD_CASE
        case 'B':
#else
        case 'b':
#endif
            std_line_prefix = TRUE; /* Putout line and file infor-  */
            break;                  /*   mation in C source style.  */
#endif

        case 'C':                           /* Keep comments        */
            cflag = TRUE;
            break;

#if MODE == STANDARD
#if FOLD_CASE
        case 'f':
#else
        case 'c':
#endif
            compat_mode = TRUE;
            break;
#endif

#if COMPILER == GNUC
        case 'd':
            if (str_eq( optarg, "M"))       {       /* -dM          */
                dMflag = TRUE;
                no_output++;
            } else if (str_eq( optarg, "D"))  {     /* -dD          */
                dDflag = TRUE;
#if MODE >= STANDARD
            } else if (str_eq( optarg, "igraphs")) {        /* -digraphs    */
                digraphs = TRUE;
#endif
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC   */

#if COMPILER == LSIC
        case 'd':
#endif
        case 'D':                           /* Define symbol        */
            def_a_macro( opt);
            break;

        case 'E':                       /* Ignore non-fatal errors  */
            eflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'f':
            if (str_eq( optarg, "no-show-column"))
                break;                      /* Ignore the option    */
            else
                usage( opt);
            break;

        case 'g':
            if (!isdigit( *optarg) && str_eq( argv[ optind - 2], "-g"))
                optind--;   /* Neither '-g 0' nor '-ggdb' -- No argument    */
            break;                          /* Ignore the option    */
#else
#if COMPILER == LCC
        case 'g':               /* Define __LCCDEBUGLEVEL as <n>    */
            if (*(optarg + 1) == EOS && isdigit( *optarg)) {
                defp = look_id( debug_name);
                strcpy( defp->repl, optarg);
            } else {
                usage( opt);
            }
            break;
#endif
#endif

#if MODE >= STANDARD
        case 'h':
            if (*(optarg + 1) == EOS && isdigit( *optarg)) {
                defp = look_id( "__STDC_HOSTED__");
                strcpy( defp->repl, optarg);
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == MSC
        case 'X':
                unset_sys_dirs = TRUE;
                break;
#endif
        case 'I':                           /* Include directory    */
            if (str_eq( optarg, "-")) {     /* -I-                  */
#if COMPILER == GNUC
                sys_dirp = incend;  /* Split include directories    */
#else
                unset_sys_dirs = TRUE;      /* Unset pre-specified  */
                break;                      /*   include directories*/
#endif
            }
            if (*(optarg + 1) == EOS && isdigit( *optarg)
                    && (i = *optarg - '0') != 0
                    && (i & ~(CURRENT | SOURCE)) == 0) {
                search_rule = i;            /* -I1, -I2 or -I3      */
                break;
            }
            set_a_dir( optarg);             /* User-defined direct. */
            break;

#if COMPILER == MSC
        case 'F':
            if (str_eq( optarg, "l")) {             /* -Fl          */
                if (preinc_end >= &preinclude[ NPREINCLUDE]) {
                    fputs( "Too many -Fl options.\n", fp_err);
                    exit( 1);
                }
                *preinc_end++ = argv[ optind++];
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == GNUC
        case 'i':
            if (str_eq( optarg, "nclude")) {        /* -include     */
                if (preinc_end >= &preinclude[ NPREINCLUDE]) {
                    fputs( "Too many -include options.\n", fp_err);
                    exit( 1);
                }
                *preinc_end++ = argv[ optind++];
            } else if (str_eq( optarg, "system")) { /* -isystem     */
                if (sysdir_end >= &sysdir[ NSYSDIR]) {
                    fputs( "Too many -isystem options.\n", fp_err);
                    exit( 1);
                }
                *sysdir_end++ = argv[ optind++];
                /* Add the directory before system include directory*/
            } else if (str_eq( optarg, "prefix")        /* -iprefix */
                    || str_eq( optarg, "withprefix")    /* -iwithprefix     */
                    || str_eq( optarg, "withprefixbefore")
                                            /* -iwithprefixbefore   */
                    || str_eq( optarg, "dirafter")) {   /* -idirafter       */
                optind++;                   /* Skip the argument    */
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == PLAN9_PCC && DEBUG
        case 'i':
            show_path = TRUE;
            break;
#endif

#if FOLD_CASE
        case 'J':
#endif
        case 'j':
            no_source_line = TRUE;
            break;  /* Do not output the source line in diagnostics */

#if COMPILER == GNUC
        case 'l':
            if (memcmp( optarg, "ang-", 4) != 0) {
                usage( opt);
            } else if (str_eq( optarg + 4, "c")) {      /* -lang-c          */
                break;                      /* Ignore this option   */
#if MODE >= STANDARD
            } else if (str_eq( optarg + 4, "c99")       /* -lang-c99*/
                        || str_eq( optarg + 4, "c9x")) {    /* -lang-c9x    */
                if (! sflag) {
                    i = 1;                  /* Define __STDC__ to 1 */
                    std_val = 199901L;
                    strcpy( std_version, "199901L");
                    Vflag = TRUE;
                    goto  stdc;
                }
            } else if (str_eq( optarg + 4, "c89")) {    /* -lang-c89*/
                if (! sflag) {
                    i = 1;                  /* Define __STDC__ to 1 */
                    goto  stdc;
                }
            } else if (str_eq( optarg + 4, "c++")) {    /* -lang-c++*/
                goto  plus;
#endif
#if MODE != POST_STANDARD && TOP_SPACE
            } else if (str_eq( optarg + 4, "asm")) {    /* -lang-asm*/
                lang_asm = TRUE;
                break;
#endif
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC   */

#if OK_MAKE
        case 'M':           /* Output source file dependency line   */
            if (str_eq( optarg, "M")) {                     /* -MM  */
                ;
            } else if (str_eq( optarg, "D")) {              /* -MD  */
                mkdep |= (MD_SYSHEADER | MD_FILE);
            } else if (str_eq( optarg, "MD")) {             /* -MMD */
                mkdep |= MD_FILE;
            } else if (str_eq( optarg, "P")) {              /* -MP  */
                mkdep |= MD_PHONY;
            } else if (str_eq( optarg, "Q")) {      /* -MQ target   */
                mkdep |= MD_QUOTE;
                mkdep_mq = argv[ optind++];
            } else if (str_eq( optarg, "T")) {      /* -MT target   */
                mkdep_mt = argv[ optind++];
            } else if (str_eq( optarg, "F")) {      /* -MF file     */
                mkdep_mf = argv[ optind++];
            } else if (argv[ optind - 1] == optarg) {       /* -M   */
                mkdep |= MD_SYSHEADER;
                optind--;
            } else {
                usage( opt);
            }
            if (str_eq( optarg, "D") || str_eq( optarg, "MD")) {
                cp = argv[ optind];
                if (*cp != '-' && (cp += strlen( cp) - 1, *cp != 'c')
                        && *cp != 'C' && !mkdep_fp)
                    /* -MD (-MMD) file, and not specified -MF file  */
                    mkdep_md = argv[ optind++];
            }
            mkdep |= MD_MKDEP;
            break;
#endif

#if FOLD_CASE
        case 'K':
        case 'k':
#else
        case 'm':
#if SYSTEM == SYS_MSDOS
            /* Specify a memory model   */
            if (*(optarg + 1) != EOS)
                usage( opt);
            else if (mem_model( toupper( *optarg & UCHARMAX)) == FALSE) {
                fprintf( fp_err, "Invalid memory model '%s'\n", optarg);
                usage( opt);
            }
            memmodel = TRUE;
#else
            /* Change the default MBCHAR encoding   */
            if (set_encoding( optarg, FALSE, 0) == NULL)
                usage( opt);
            mb_changed = TRUE;
#endif  /* SYSTEM != SYS_MSDOS  */
#endif  /* ! FOLD_CASE  */
            break;

#if COMPILER == GNUC
        case 'u':
            if (! str_eq( optarg, "ndef"))  /* -undef               */
                usage( opt);
            un_predefine( FALSE);   /* Remove "unix", "i386", etc.  */
            break;
#endif

#if COMPILER == MSC
        case 'u':
#endif
#if COMPILER == PLAN9_PCC
        case 'n':
#else
        case 'N':
#endif
            /* No predefines:   remove "vax", "__VAX" and friends.  */
            nflag = TRUE;
            break;

#if COMPILER == PLAN9_PCC
        case 'N':
            unset_sys_dirs = TRUE;  /* Unset default include directories    */
            break;
#endif

#if COMPILER == GNUC
        case 'n':
            if (str_eq( optarg, "ostdinc")) {               /* -nostdinc    */
                unset_sys_dirs = TRUE;  /* Unset pre-specified directories  */
#if MODE >= STANDARD
            } else if (str_eq( optarg, "ostdinc++")) {      /* -nostdinc++  */
                set_cplus_dir = FALSE;  /* Unset C++-specific directories   */
#endif
            } else if (str_eq( optarg, "oprecomp")) {       /* -noprecomp   */
                fprintf( fp_err, warning, opt, optarg);
                break;
            } else {
                usage( opt);
            }
            break;
#endif

#if COMPILER == LCC
        case 'O':                   /* Define __LCCOPTIMLEVEL as 1  */
            defp = look_id( optim_name);
            strcpy( defp->repl, "1");
            break;
#endif

#if FOLD_CASE
        case 'O':
#endif
        case 'o':
            *out_pp = optarg;               /* Output file name     */
            break;

        case 'P':                           /* No #line output      */
            pflag = TRUE;
            break;

#if COMPILER == GNUC
        case 'p':
            if (str_eq( optarg, "edantic")          /* -pedantic    */
                    || str_eq( optarg, "edantic-errors")) {
                                            /* -pedantic-errors     */
#if MODE >= STANDARD
                if (warn_level == -1)
                    warn_level = 0;
                warn_level |= (1 | 2 | 4);
                if (! sflag && ! cplus) {
                    i = 1;
                    goto  stdc;             /* Set -S1 option       */
                }
#else
                fputs( "Use STANDARD MODE cpp for -pedantic option\n"
                        , fp_err);
                usage( opt);
#endif
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC   */

        case 'Q':
            qflag = TRUE;
            break;

#if MODE >= STANDARD
        case 'S':
            if (cplus || sflag) {       /* C++ or the second time   */
                fprintf( fp_err, warning, opt, optarg);
                break;
            }
            i = *optarg;
            if (! isdigit( i) || *(optarg + 1) != EOS)
                usage( opt);
            i = i - '0';
#if COMPILER == GNUC
stdc:
#endif
            defp = look_id( stdc_name);
            sprintf( defp->repl, "%d", i);          /* Redefine     */
            stdc_val = i;
            sflag = TRUE;
            break;
#endif  /* MODE >= STANDARD */

#if COMPILER == GNUC
        case 'r':
            if (str_eq( optarg, "emap"))
                fprintf( fp_err, warning, opt, optarg);
                                            /* Ignore -remap option */
            else
                usage( opt);
            break;

#if MODE >= STANDARD
        case 's':
            if (memcmp( optarg, "td=", 3) == 0 && strlen( optarg) > 3) {
                /* -std=STANDARD    */
                cp = optarg + 3;
                if (str_eq( cp, "c89")              /* std=c89      */
                        || str_eq( cp, "c90")       /* std=c90      */
                        || str_eq( cp, "gnu89")     /* std=gnu89    */
                        || str_eq( cp, "iso9899:1990")) {
                    std_val = 0L;               /* C90 + extensions */
                    strcpy( std_version, "0L"); /* Will be modified later   */
                } else if (str_eq( cp, "c99")       /* std=c99      */
                        || str_eq( cp, "c9x")       /* std=c9x      */
                        || str_eq( cp, "gnu99")     /* std=gnu99    */
                        || str_eq( cp, "gnu9x")     /* std=gnu9x    */
                        || str_eq( cp, "iso9899:1999")
                        || str_eq( cp, "iso9899:199x")) {
                    std_val = 199901L;
                    strcpy( std_version, "199901L");
                } else if (str_eq( cp, "c++98")) {  /* std=c++98    */
                    cplus = std_val = 199711L;
                    strcpy( std_version, "199711L");
                } else if (memcmp( cp, "iso9899:", 8) == 0
                        && strlen( cp) >= 14) { /* std=iso9899:199409, etc. */
                    optarg = cp + 8;
                    goto Version;
                } else if (memcmp( cp, "iso14882", 8) == 0) {
                    cp += 8;
                    if (cp && *cp == ':' && strlen( cp) >= 7) {
                                    /* std=iso14882:199711, etc.    */
                        cplus = CPLUS;
                        optarg = cp + 1;
                        goto Version;
                    } else {
                        goto plus;
                    }
                } else {
                    usage( opt);
                }
                Vflag = TRUE;
                i = 1;
                goto stdc;
            } else {
                usage( opt);
            }
            break;
#endif  /* MODE >= STANDARD     */
#endif  /* COMPILER == GNUC   */

#if COMPILER == LSIC
        case 's':
            pflag = TRUE;
            break;
#endif

#if COMPILER == GNUC
        case 't':
            if (str_eq( optarg, "raditional")) {    /* -traditional */
#if ! COMMENT_INVISIBLE || ! STRING_FORMAL
                fputs( "Use OLD_PREPROCESSOR for -traditional\n", fp_err);
                usage( opt);
#else
                break;
#endif
#if MODE == STANDARD && OK_TRIGRAPHS
            } else if (str_eq( optarg, "rigraphs")) {
                tflag = TRUE;                       /* -trigraphs   */
#endif
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC   */

#if COMPILER == MSC
        case 'T':
            if (strlen( optarg) > 1)
                usage( opt);
            i = tolower( *optarg);                  /* Fold case    */
            if (i == 'c') {
                break;                      /* Ignore this option   */
#if MODE >= STANDARD
            } else if (i == 'p') {
                cplus = CPLUS;
                break;
#endif
            } else {
                usage( opt);
            }
#endif

        case 'U':                           /* Undefine symbol      */
            /*
             * We don't need to map trigraphs as they can't be part of a
             * symbol name. (_ isn't trigraphable).
             */
            if ((defp = look_id( optarg)) != NULL) {
                if (defp->nargs == DEF_NOARGS - 1) {
                    undef_a_predef( optarg);
                }
#if MODE >= STANDARD
                else if (defp->nargs < DEF_NOARGS - 1) {
                    fprintf( fp_err, "\"%s\" shouldn't be undefined\n"
                            , optarg);
                    usage( opt);
                }
#endif
                undefine( optarg);
            } else {
                fprintf( fp_err, "\"%s\" wasn't defined\n", optarg);
            }
            break;

#if MODE >= STANDARD
#if FOLD_CASE
        case 'T':
#else
#if COMPILER == PLAN9_PCC
        case 's':
#else
        case 'V':
#endif
#endif
#if COMPILER == GNUC
Version:
#endif
            valp = eval_num( optarg);
            if (valp->sign == VAL_ERROR)
                usage( opt);
            std_val = (long) valp->val;
            sprintf( std_version, "%ldL", std_val);
            Vflag = TRUE;
            break;
#endif  /* MODE >= STANDARD */

#if COMPILER == PLAN9_PCC
        case 'V':                               /* Ignore this option   */
            break;
#endif

#if FOLD_CASE
        case 'V':
#else
        case 'v':
#endif
            vflag = TRUE;
            show_path = TRUE;
            break;

        case 'W':                           /* warning level        */
            if (warn_level == -1)           /* Have to initialize   */
                warn_level = 0;
#if COMPILER == GNUC
            if (argv[ optind - 1] == optarg) {      /* No argument  */
                /*
                 * Note: -W without argument is not officially supported.
                 *  It may cause an error.
                 */
                warn_level |= (1 | 2 | 4 | 16);
                optind--;
                break;
            } else if (str_eq( optarg, "comment")
                        || str_eq( optarg, "comments")
                        || str_eq( optarg, "sign-compare")) {
                warn_level |= 1;
                break;
            } else if (str_eq( optarg, "undef")) {
                warn_level |= 4;
                break;
            } else if (str_eq( optarg, "all")) {
                warn_level |= (1 | 16);     /* Convert -Wall to -W17*/
                break;
            }
#if MODE == STANDARD && OK_TRIGRAPHS
            else if (str_eq( optarg, "trigraphs")) {
                warn_level |= 16;
                break;
            }
#endif
#endif  /* COMPILER == GNUC   */
#if COMPILER == MSC
            if (str_eq( optarg, "all")) {
                warn_level |= (1 | 16);     /* Convert -Wall to -W17*/
                break;
            } else if (str_eq( optarg, "L")) {
                no_source_line = TRUE;  /* Single-line diagnostic   */
                break;
            }
#endif
            if ((warn_level |= atoi( optarg)) > 31 || warn_level < 0)
                usage( opt);
            if (warn_level == 0)
                warn_level = 0xFF;          /* Remember this option */
            /* Else ignore the option   */
            break;

#if COMPILER == GNUC || COMPILER == MSC || COMPILER == LSIC
        case 'w':
            warn_level = 0xFF;              /* Remenber this option */
            break;
#endif

#if COMPILER == GNUC
        case 'x':
            if (str_eq( optarg, "c")) {
                break;                      /* -x c -- ignore this  */
#if MODE >= STANDARD
            } else if (str_eq( optarg, "c++")) {
                goto plus;
#endif
#if MODE != POST_STANDARD && TOP_SPACE
            } else if (str_eq( optarg, "assembler-with-cpp")) {
                lang_asm = TRUE;
                break;
#endif
            } else {
                usage( opt);
            }
            break;
#endif  /* COMPILER == GNUC   */

#if COMPILER == MSC
        case 'Z':
            if (*optarg != 'a')
                usage( opt);
            /* Else ignore this option (-Za)    */
            break;
#endif

        case 'z':
            zflag = TRUE;           /* No output of included file   */
            break;

#if MODE >= STANDARD && OK_DIGRAPHS
        case '2':                   /* Revert digraphs recognition  */
            digraphs = ! digraphs;
            break;
#endif

#if MODE == STANDARD && OK_TRIGRAPHS
        case '3':                   /* Revert trigraph recogniion   */
            tflag = ! tflag;
            break;
#endif

#if MODE >= STANDARD
        case '+':
#if COMPILER == GNUC
plus:
#endif
            if (cplus || sflag) {
                fprintf( fp_err, "warning: -+ option is ignored\n");
                break;
            }
            cplus = CPLUS;
            break;
#endif  /* MODE >= STANDARD     */

#if COMPILER == GNUC && ! DOLLAR_IN_NAME
        case '$':                       /* Forbid '$' in identifier */
            break;                          /* Ignore this option   */
#endif

        default:                            /* What is this one?    */
            usage( opt);
            break;
        }                               /* Switch on all options    */

    }                                   /* For all arguments        */

    if (optind < argc && set_files( argc, argv, in_pp, out_pp) != NULL)
        goto  opt_search;       /* More options after the filename  */
    if (warn_level == -1)               /* No -W option             */
        warn_level = 1;                 /* Default warning level    */
    else if (warn_level == 0xFF)
        warn_level = 0;                 /* -W0 has high precedence  */
    if (nflag)
        un_predefine( TRUE);
#if MODE >= STANDARD
    else if (stdc_val || cplus)
        un_predefine( FALSE);           /* Undefine "unix" or so    */
    if (Vflag) {                        /* Version is specified     */
        if (cplus)
            cplus = std_val;            /* Value of __cplusplus     */
        else
            stdc_ver = std_val;     /* Value of __STDC_VERSION__    */
    } else {
        if (!cplus)
            stdc_ver = stdc_val ? STDC_VERSION : 0L;
        sprintf( std_version, "%ldL", cplus ? cplus : stdc_ver);
    }
    if (!cplus && stdc_ver)         /* Define __STDC_VERSION__      */
        look_and_install( stdc_v_name, DEF_NOARGS - 2, "", std_version);
    set_cplus( std_version, stdc_name, stdc_v_name, nflag);
    set_limit();
    stdc2 = cplus || stdc_ver >= 199901L;
    stdc3 = (cplus >= 199901L) || (stdc_ver >= 199901L);
                /* (cplus >= 199901L) makes C++ C99-compatible mode */
#if OK_PRAGMA_OP
    if (stdc3)
        set_pragma_op();
#endif
#endif  /* MODE >= STANDARD */

#if SYSTEM == SYS_MSDOS
    if (memmodel == FALSE)
        mem_model( 'S');            /* Default memory model : SMALL */
#endif
#if COMPILER == GNUC
    if (sysdir < sysdir_end) {
        char **     dp = sysdir;
        while (dp < sysdir_end)
            set_a_dir( *dp++);
    }
#endif
#if HOST_HAVE_GETENV
    if (! unset_sys_dirs)
        set_env_dirs();
#endif
    if (! unset_sys_dirs)
        set_sys_dirs( set_cplus_dir);
#if OK_MAKE
    if (mkdep_mf)                           /* -MF overrides -MD    */
        mkdep_fp = fopen( mkdep_mf, "w");
    else if (mkdep_md)
        mkdep_fp = fopen( mkdep_md, "w");
    if (mkdep_mq)                           /* -MQ overrides -MT    */
        mkdep_target = mkdep_mq;
    else if (mkdep_mt)
        mkdep_target = mkdep_mt;
#endif
#if COMPILER == GNUC
    chk_env();  /* Check the env-vars to specify version and dependency line*/
#endif
    if (vflag)
        version();
    if (show_path) {
        fp_debug = stderr;
        dump_path();
        fp_debug = stdout;
    }
}

static void
#if PROTO
version( void)
#else
version()
#endif
/*
 * Print version message.
 */
{
    char *      mes[] = {

#ifdef  VERSION_MSG
        "MCPP V.2.4.1 (2004/03) "
#else
        "MCPP V.", VERSION, " (", DATE, ") "
#endif
#if     MODE == STANDARD
            , "STANDARD"
#else
#if     MODE == POST_STANDARD
            , "POST_STANDARD"
#else
#if     OLD_PREPROCESSOR
            , "OLD_PREPROCESSOR"
#else
            , "PRE_STANDARD"
#endif
#endif
#endif
#ifdef  VERSION_MSG
            , " mode for "
            , VERSION_MSG
#else
            , " mode"
#ifdef  CMP_NAME
            , " for ", CMP_NAME
#ifdef  HOST_CMP_NAME
            , " compiled by ", HOST_CMP_NAME
#if     HOST_COMPILER == GNUC
            , " V.", GCC_MAJOR_VERSION, ".", GCC_MINOR_VERSION
#endif
#endif
#endif
#endif
            , "\n", NULL
        };

    char **     mpp = mes;
    while (*mpp != NULL)
        fputs( *mpp++, fp_err);
}

static void
#if PROTO
usage( int opt)
#else
usage( opt)
    int     opt;
#endif
/*
 * Print usage.
 */
{
    static const char * const   mes[] = {

"Usage:  cpp [-<opts> [-<opts>]] [<infile> [-<opts>] [<outfile>] [-<opts>]]\n",
"    <infile> defaults to stdin and <outfile> defaults to stdout.\n",
"The following options and some more (see manual) are valid:\n",

#if MODE != POST_STANDARD && TOP_SPACE
#if FOLD_CASE
"-A      ",
#else
#if COMPILER == GNUC
"-a, -lang-asm, -x assembler-with-cpp    \n        ",
#else
"-a      ",
#endif
#endif
        "Process \"assembler\" source (the \"traditional\" preprocessing).\n",
#endif  /* MODE != POST_STANDARD && TOP_SPACE   */

#if ! STD_LINE_PREFIX
#if FOLD_CASE
"-B      ",
#else
"-b      ",
#endif
        "Output #line lines in C source style.\n",
#endif  /* ! STD_LINE_PREFIX    */

"-C      Output also comments.           -E      Ignore non-fatal errors.\n",

#if MODE == STANDARD
"-c      Expand recursive macro more than Standard (compatible to GNU C).\n",
#endif

#if COMPILER == LSIC
"-D, -d <macro>[=<value>]    ",
#else
"-D <macro>[=<value>]    ",
#endif
                        "Define <macro> as <value> (default:1).\n",
#if COMPILER == LSIC
"-D, -d <macro(args)>[=<replace>]     ",
#else
"-D <macro(args)>[=<replace>]    ",
#endif
                        "Define <macro(args)> as <replace>.\n",

#if COMPILER == LCC
"-g <n>      Define the macro __LCCDEBUGLEVEL as <n>.\n",
#endif
#if MODE >= STANDARD
"-h <n>      Re-define the pre-defined macro __STDC_HOSTED__ as <n>.\n",
#endif

"-I <directory>  Add <directory> to the #include search list.\n",

#if COMPILER == GNUC
"-nostdinc   ",
#else
#if COMPILER == MSC
"-I-, -X     ",
#else
#if COMPILER == PLAN9_PCC
"-I-, -N     ",
#else
"-I-     ",
#endif
#endif
#endif
        "Unset system or site specific include directories.\n",

#if COMPILER == GNUC || COMPILER == MSC
#if COMPILER == GNUC
"-include <file>     ",
#else
"-Fl <file>  ",
#endif
            "Include the <file> prior to the main input file.\n",
#endif

#if COMPILER == MSC
"-j, -WL     ",
#else
"-j      ",
#endif
            "Do not output the source line in diagnostics.\n",

#if OK_MAKE
"-M, -MM, -MD, -MMD, -MP, -MQ target, -MT target, -MF file\n",
"        Output source file dependency line for makefile.\n",
#endif

#if SYSTEM == SYS_MSDOS
"-m <x>  Specify memory-model as <x> ",
#if COMPILER == LSIC || COMPILER == LATTICEC
                                    "(t,s,d,p,l,h).\n",
#else
                                    "(t,s,c,m,l,h).\n",
#endif
#else   /* SYSTEM != SYS_MSDOS  */
"-m <encoding>   Change the default multi-byte character encoding to one of:\n",
"        euc_jp (euc), gb2312, ksc5601, bigfive (big5), sjis, iso2022_jp, utf8.\n",
#endif  /* SYSTEM == SYS_MSDOS  */

#if COMPILER == GNUC
"-N, -undef  ",
#else
#if COMPILER == MSC
"-N, -u      ",
#else
#if COMPILER == PLAN9_PCC
"-n      ",
#else
"-N      ",
#endif
#endif
#endif
        "Don't predefine any non-standard macros.\n",

#if COMPILER == LCC
"-O      Define the macro __LCCOPTIMLEVEL as 1.\n",
#endif

#if FOLD_CASE
"-O <file>   ",
#else
"-o <file>   ",
#endif
            "Output to <file>.           ",

#if COMPILER == LSIC
"-P, -s  ",
#else
"-P      ",
#endif
        "Don't output #line lines.\n",

"-Q      Output diagnostics to \"cpp.err\" (default:stderr).\n",

#if MODE >= STANDARD
#if COMPILER == GNUC
"-pedantic, -pedantic-errors, -lang-c89      Same as -S1.\n",
"-std=<STANDARD>     Specify the standard to which the code should conform.\n",
"        <STANDARD> may be one of: c90, c99, iso9899:1990, iso14882, etc.\n",
"        iso9899:<n>, iso14882:<n> : Same as -V <n> (long in decimals).\n",
#endif
"-S <n>      Redefine __STDC__ to <n>, undefine old style macros.\n",
#endif  /* MODE >= STANDARD */

"-U <macro>  Undefine <macro>.           -v  Show version of cpp.\n",

#if MODE >= STANDARD
#if FOLD_CASE
"-T <n>      Redefine __STDC_VERSION__ or __cplusplus to <n>.\n",
#else
#if COMPILER == PLAN9_PCC
"-s <n>      Redefine __STDC_VERSION__ or __cplusplus to <n>.\n",
#else
"-V <n>      Redefine __STDC_VERSION__ or __cplusplus to <n>.\n",
#endif
#endif
"        C with -V199901L specifies C99 mode.\n",
"        C++ with -V199901L specifies C99 compatible mode (including _Pragma).\n",
#endif  /* MODE >= STANDARD */

"-W <level>  Set warning level to <level> (OR of {0,1,2,4,8,16}, default:1).\n",

#if COMPILER == LSIC || COMPILER == MSC || COMPILER == GNUC
"-w      Same as -W0.\n",
#endif

"-z      ",
        "Don't output the included file, only defining macros.\n",

#if MODE >= STANDARD && OK_DIGRAPHS
#if COMPILER == GNUC
"-digraphs   Enable digraphs.            ",

#endif
"-2      ",
#if DIGRAPHS_INIT
        "Dis",
#else
        "En",
#endif
           "able digraphs.\n",
#endif  /* MODE >= STANDARD */

#if MODE == STANDARD && OK_TRIGRAPHS
#if COMPILER == GNUC
"-trigraphs  Enable trigraphs.           ",
#endif
"-3      ",
#if TFLAG_INIT
        "Dis",
#else
        "En",
#endif
            "able trigraphs.\n",
#endif  /* MODE == STANDARD && OK_TRIGRAPHS */

#if MODE >= STANDARD
#if COMPILER == GNUC
"-+, -lang-c++, -x c++   ",
#else
#if COMPILER == MSC
"-+, -Tp     ",
#else
"-+      ",
#endif
#endif
        "Process C++ source.\n",
#endif  /* MODE >= STANDARD */

        NULL,
    };

    extern char *   optarg;
    const char *    illegopt = "Incorrect option -%c%s\n";
    const char * const *    mpp = & mes[ 0];

    if (opt != '?')
        fprintf( fp_err, illegopt, opt, optarg ? optarg : "");
    version();
    while (*mpp)
        fputs( *mpp++, fp_err);
    exit( 1);
}

static void
#if PROTO
set_opt_list( char * optlist)
#else
set_opt_list( optlist)
    char *  optlist;
#endif
/*
 * Set list of legal option characters.
 */
{
    const char *    list[] = {
    "e:",   // C2SC, getopt で怒られないようにする
#if MODE == STANDARD && OK_TRIGRAPHS
    "3",
#endif

#if MODE >= STANDARD
#if OK_DIGRAPHS
    "2",
#endif
#if FOLD_CASE
    "t:T:",
#else
#if COMPILER == PLAN9_PCC
    "s:",
#else
    "V:",
#endif
#endif
    "+h:S:",
#endif  /* MODE >= STANDARD */

#if MODE == STANDARD
#if FOLD_CASE
    "f",
#else
    "c",
#endif
#endif

#if OK_MAKE
#if FOLD_CASE
    "m:M:",
#else
    "M:",
#endif
#endif

#if MODE != POST_STANDARD && TOP_SPACE
#if FOLD_CASE
    "aA",
#else
    "a",
#endif
#endif

#if FOLD_CASE
    "cd:ei:npqs:u:w:zH:JO",
#endif

#if ! STD_LINE_PREFIX
#if FOLD_CASE
    "bB",
#else
    "b",
#endif
#endif

#if FOLD_CASE
    "K:"
#else
    "m:",
#endif

#if COMPILER == GNUC
    "$A:d:f:g:i:l:n:r:s:t:u:p:wx:",
#endif
#if COMPILER == MSC
    "F:T:XZ:uw",
#endif
#if COMPILER == LCC
    "g:O",
#endif
#if COMPILER == LSIC
    "d:sw",
#endif
#if COMPILER == PLAN9_PCC
    "ins:V",
#endif
    NULL
    };

    const char * const *    lp = & list[ 0];

    strcpy( optlist, "jo:vzCD:EI:NPQU:W:");
    while (*lp)
        strcat( optlist, *lp++);
#if DEBUG
    if (strlen( optlist) >= OPTLISTLEN)
        cfatal( "Bug: Too long option list", NULLST, 0L, NULLST);   /* _F_  */
#endif
}

static void
#if PROTO
def_a_macro( int opt)
#else
def_a_macro( opt)
    int     opt;
#endif
/*
 * Define a macro specified by -D option.
 * The macro maybe either object-like or function-like (with parameter).
 */
{
    extern char *   optarg;
    DEFBUF *    defp;
    char *      definition;                 /* Argument of -D option*/
    char *      cp;
    int         i;

#if MODE == STANDARD && OK_TRIGRAPHS
    /* Convert trigraphs for the environment which need trigraphs   */
    if (tflag)
        cnv_trigraph( optarg);
#endif
#if MODE == POST_STANDARD && OK_DIGRAPHS
    if (digraphs)           /* Convert prior to installing macro    */
        cnv_digraph( optarg);
#endif
    definition = xmalloc( strlen( optarg) + 4);
    strcpy( definition, optarg);
    if ((cp = strchr( definition, '=')) != NULL) {
        *cp = ' ';                          /* Remove the '='       */
        cp = "\n";                          /* Append <newline>     */
    } else {
        cp = " 1\n";                        /* With definition "1"  */
    }
    strcat( definition, cp);
    cp = definition;
    while ((type[ *cp & UCHARMAX] & SPA) == 0)
        cp++;
    i = *cp;
    *cp = EOS;
    if ((defp = look_id( definition)) != NULL) {    /* Pre-defined  */
        if (defp->nargs == DEF_NOARGS - 1) {
            undef_a_predef( definition);
            /* Remove the name from the table of pre-defined-macros.*/
        }
#if MODE >= STANDARD
        else if (defp->nargs < DEF_NOARGS - 1) {
            if (str_eq( definition, "__STDC_HOSTED__")) {
                defp->nargs = DEF_NOARGS;
                /* Some systems, e.g. GNU C, define this by -D      */
#if COMPILER == PLAN9_PCC
            } else if (str_eq( definition, "__STDC__")) {
                defp->nargs = DEF_NOARGS;
                /* PLAN9 PCC uses '-D__STDC__=1' option */
#endif
            } else {
                fprintf( fp_err, "\"%s\" shouldn't be redefined\n"
                        , definition);      /* Standard predefined  */
                usage( opt);
            }
        }
#endif
        undefine( definition);
    }
    *cp = i;
    /* Now, save the definition.    */
    unget_string( definition, NULLST);
    if (do_define() == FALSE)               /* Define a macro       */
        usage( opt);
    *cp = EOS;
    if (str_eq( definition, "__STDC_HOSTED__")
            || str_eq( definition, "__STDC__")) {
        defp = look_id( definition);
        defp->nargs = DEF_NOARGS - 2;
                                /* Restore Standard-predefinedness  */
    }
    free( definition);
    skip_nl();                      /* Clear the appended <newline> */
}

static char *
#if PROTO
set_files( int argc, char ** argv, char ** in_pp, char ** out_pp)
#else
set_files( argc, argv, in_pp, out_pp)
    int     argc;
    char ** argv;
    char ** in_pp;
    char ** out_pp;
#endif
/*
 * Set input and/or output files.
 */
{
    extern int  optind;
    char *      cp;

    if (*in_pp == NULL) {                           /* Input file   */
        cp = argv[ optind++];
#if SYS_FAMILY == SYS_MSDOS
        cp = bsl2sl( cp, FALSE);
#endif
        *in_pp = cp;
    }
    if (optind < argc && argv[ optind][ 0] != '-' && *out_pp == NULL) {
        cp = argv[ optind++];
#if SYS_FAMILY == SYS_MSDOS
        cp = bsl2sl( cp, FALSE);
#endif
        *out_pp = cp;                               /* Output file  */
    }
    if (optind >= argc)
        return  NULL;           /* Exhausted command line arguments */
    if (argv[ optind][ 0] == '-')
        return  argv[ optind];                      /* More options */
    cfatal( "Excessive file argument \"%s\"", argv[ optind], 0L , NULLST);
    return  NULL;
}

#if MODE >= STANDARD

static void
#if PROTO
set_cplus( char * val, const char * stdc_name , const char * stdc_v_name
        , int nflag)
#else
set_cplus( val, stdc_name, stdc_v_name, nflag)
    char *  val;
    char *  stdc_name;
    char *  stdc_v_name;
    int     nflag;
#endif
/*
 * Set the macro "__cplusplus" and remove "__STDC__", "__STDC_VERSION__".
 */
{
    const char * const  cplus_name = "__cplusplus";
    DEFBUF *    defp;

    if (! cplus) {
#ifdef  COMPILER_CPLUS
        if (! nflag)        /* Undefine pre-defined macro for C++   */
            undefine( COMPILER_CPLUS);
#endif
    } else {
#if COMPILER != GNUC  /* GNU C do not undefine __STDC__ on C++    */
        if ((defp = look_id( stdc_name)) != NULL) {
            defp->nargs = DEF_NOARGS;
            undefine( stdc_name);           /* Remove __STDC__      */
            stdc_val = 0;
        }
#endif
        if ((defp = look_id( stdc_v_name)) != NULL) {
            defp->nargs = DEF_NOARGS;
            undefine( stdc_v_name);     /* Remove __STDC_VERSION__  */
        }
        look_and_install( cplus_name, DEF_NOARGS - 2, "", val);
    }
}

static void
#if PROTO
set_limit( void)
#else
set_limit()
#endif
/*
 * Set the minimum translation limits specified by the Standards.
 */
{
    if (cplus) {                /* Specified by C++ 1998 Standard   */
        str_len_min = SLEN_CPLUS_MIN;
        id_len_min = IDLEN_CPLUS_MIN;
        n_mac_pars_min = NMACPARS_CPLUS_MIN;
        exp_nest_min = EXP_NEST_CPLUS_MIN;
        blk_nest_min = BLK_NEST_CPLUS_MIN;
        inc_nest_min = INCLUDE_NEST_CPLUS_MIN;
        n_macro_min = NMACRO_CPLUS_MIN;
        line_limit = LINE_CPLUS_LIMIT;
    } else if (stdc_ver >= 199901L) {       /* Specified by C 1999 Standard */
        str_len_min = SLEN99MIN;
        id_len_min = IDLEN99MIN;
        n_mac_pars_min = NMACPARS99MIN;
        exp_nest_min = EXP_NEST99MIN;
        blk_nest_min = BLK_NEST99MIN;
        inc_nest_min = INCLUDE_NEST99MIN;
        n_macro_min = NMACRO99MIN;
        line_limit = LINE99LIMIT;
    } else {                        /* Specified by C 1990 Standard */
        str_len_min = SLEN90MIN;
        id_len_min = IDLEN90MIN;
        n_mac_pars_min = NMACPARS90MIN;
        exp_nest_min = EXP_NEST90MIN;
        blk_nest_min = BLK_NEST90MIN;
        inc_nest_min = INCLUDE_NEST90MIN;
        n_macro_min = NMACRO90MIN;
        line_limit = LINE90LIMIT;
    }
}

#if OK_PRAGMA_OP
static void
#if PROTO
set_pragma_op( void)
#else
set_pragma_op()
#endif
/*
 *      #define _Pragma(a)  _Pragma ( a )
 * Define _Pragma() operator as a special macro so as to be searched
 * easily.  The unusual 'DEF_PRAGMA' is a marker of this psuedo
 * macro.
 */
{
    char *  name = "_Pragma";
    char    tmp[ 16];

    sprintf( tmp, "%c%s ( %c%c )", DEF_MAGIC, name, MAC_PARM, 1);
                                                /* Replacement text */
    look_and_install( name, DEF_PRAGMA, "a", tmp);
}
#endif  /* OK_PRAGMA_OP */

#endif  /* MODE => STANDARD */

#if FOLD_CASE
static void
zap_uc(ap)
    register char *     ap;
/*
 * Dec operating systems mangle upper-lower case in command lines.
 * This routine forces the arguments of option to uppercase.
 * It is called only on cpp startup by do_options().
 */
{
    while (*ap != EOS) {
        if (islower( *ap & UCHARMAX))
            *ap = toupper( *ap & UCHARMAX);
        ap++;
    }
}
#endif

#if SYSTEM == SYS_MSDOS

#define LARGE   4
#define SMALL   2

static int
mem_model( int model)
/*
 * Support the selection of different memory models for the i8086 family.
 */
{
#if OK_SIZE
    SIZES *         sizp = size_table;      /* Get start of table   */
#endif
#if COMPILER == MSC
    char            name[ 8] = "M_I86SM";
#else
    const char *    name;
#endif

    switch (model) {
#if ! OK_SIZE
    case 'T' :
    case 'S' :
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :
#endif
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :
#endif
    case 'L' :
    case 'H' :
        break;
#else   /* OK_SIZE  */
    case 'T' :                  /* Tiny model...    */
    case 'S' :                  /* Small model...   */
        while (sizp->bits != T_FPTR)
            (sizp++)->psize = SMALL;
        sizp->psize = SMALL;
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :                  /* Compact model... small code, large data  */
#endif
        while (sizp->bits != T_FPTR)
            (sizp++)->psize = LARGE;
        sizp->psize = SMALL;
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :                  /* Medium model... large code, small data   */
#endif
        while (sizp->bits != T_FPTR )
            (sizp++)->psize = SMALL;    /* All data pointers are short.     */
        sizp->psize = LARGE;    /* Function pointers are long.  */
        break;

    case 'L' :                  /* Large model...   */
    case 'H' :                  /* Huge model... large code, large data     */
        while (sizp->bits)
            (sizp++)->psize = LARGE;
        break;
#endif  /* OK_SIZE  */

    default  :                  /* Invalid model    */
        return  FALSE;
    }

#if COMPILER == MSC

    name[ 5] = model;

#else   /* COMPILER != MSC  */

    switch (model) {
    case 'T' :
        name = "__TINY__";
        break;
    case 'S' :
        name = "__SMALL__";
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'D' :
#else
    case 'C' :
#endif
        name = "__COMPACT__";
        break;
#if COMPILER == LATTICEC || COMPILER == LSIC
    case 'P' :
#else
    case 'M' :
#endif
        name = "__MEDIUM__";
        break;
    case 'L' :
        name = "__LARGE__";
        break;
    case 'H' :
        name = "__HUGE__";
        break;
    }

#endif  /* COMPILER != MSC  */

    look_and_install( name, DEF_NOARGS, "", "1");
    return  TRUE;
}

#endif  /* SYS_MSDOS    */

void
#if PROTO
at_start( void)
#else
at_start()
#endif
/*
 * Do the commands prior to processing main source file.
 */
{
#if SYSTEM != SYS_MSDOS && HOST_HAVE_GETENV
    /*
     * Set multi-byte character encoding according to environment variables
     * LC_ALL, LC_CTYPE and LANG -- with preference in this order.
     */
    char *  env;

    if (mb_changed)
        return;                         /* -m option precedes   */
    if ((env = getenv( "LC_ALL")) != NULL)
        set_encoding( env, "LC_ALL", 0);
    else if ((env = getenv( "LC_CTYPE")) != NULL)
        set_encoding( env, "LC_CTYPE", 0);
    else if ((env = getenv( "LANG")) != NULL)
        set_encoding( env, "LANG", 0);
#endif

#if COMPILER == GNUC || COMPILER == MSC
    /*
     * Do the -include (-Fl for MSC) options in the specified order.
     * Note: This functionality is implemented as nested #includes
     *   which results the same effect as sequential #includes.
     */
    while (preinclude <= --preinc_end)
        open_file( inc_dirp, *preinc_end, TRUE);
#endif
}

#if HOST_HAVE_GETENV

static void
#if PROTO
set_env_dirs( void)
#else
set_env_dirs()
#endif
/*
 * Add to include path those specified by environment variables.
 */
{
    const char *    env;

#if MODE >= STANDARD
    if (cplus) {
        if ((env = getenv( ENV_CPLUS_INCLUDE_DIR)) != NULL)
            parse_env( env);
    }
#endif
    if ((env = getenv( ENV_C_INCLUDE_DIR)) != NULL)
        parse_env( env);
}

static void
#if PROTO
parse_env( const char * env)
#else
parse_env( env)
    char *    env;
#endif
/*
 * Parse environmental variable and append the path to include-dir-list.
 */
{
    char *  save;
    char *  p;
    int     sep;

    save = save_string( env);
    while (*save) {
        p = save;
        while (*p && *p != ENV_SEP)
            p++;
        if (p != save)  {                   /* Variable separator   */
            sep = *p;
            *p = EOS;
            set_a_dir( save);
            if (sep == EOS)
                break;
            save = ++p;
        }
        while (*save == ENV_SEP)
            ++save;
    }
}

#endif  /* HOST_HAVE_GETENV */

static void
#if PROTO
set_sys_dirs( int set_cplus_dir)
#else
set_sys_dirs( set_cplus_dir)
    int     set_cplus_dir;      /* Do not set C++ include-directory */
#endif
/*
 * Set site-specific system-specific directories to the include directory
 * list.
 */
{
#if SYSTEM == SYS_PLAN9
    const char *    env;
    char *          objinc;
#endif

#ifdef  C_INCLUDE_DIR1
    set_a_dir( C_INCLUDE_DIR1);
#endif
#ifdef  C_INCLUDE_DIR2
    set_a_dir( C_INCLUDE_DIR2);
#endif

#if MODE >= STANDARD
#ifdef  CPLUS_INCLUDE_DIR
    if (cplus && set_cplus_dir)
        set_a_dir( CPLUS_INCLUDE_DIR);
#endif
#endif  /* MODE >= STANDARD */

#if SYS_FAMILY == SYS_UNIX
#if SYSTEM == SYS_PLAN9
    if ((env = getenv( "objtype")) != NULL) {
        objinc = xmalloc( 20);
        strcpy( objinc, "/");
        strcat( objinc, env);
        strcat( objinc, "/include/ape");
        set_a_dir( objinc);
    }
    set_a_dir( "/sys/include/ape");
#else
    set_a_dir( "/usr/local/include");
    set_a_dir( "/usr/include");
#endif
#endif

#if SYS_FAMILY == SYS_OS9
    set_a_dir( "/DD/DEFS");
#endif
}

static void
#if PROTO
set_a_dir( const char * dirname)
#else
set_a_dir( dirname)
    char *    dirname;                      /* The path-name        */
#endif
/*
 * Append an include directory, checking array boundary.
 * This routine is called from the following routines (in this order).
 * 1. do_options() by -I option.
 * 2. do_options() by -isystem option (for GNUC).
 * 3. set_env_dirs() by environment variables.
 * 4. set_sys_dirs() by CPLUS_INCLUDE_DIR, C_INCLUDE_DIR and system-
 *    specifics (unless -I- or -nostdinc option is specified).
 * Note: a trailing PATH-DELIM is appended by norm_path().
 */
{
    char *  norm_name;
    const char **   ip;

    norm_name = norm_path( dirname);
                            /* Normalize the pathname to compare    */
    for (ip = incdir; ip < incend; ip++) {
        if (str_eq( *ip, norm_name))
            return;
    }
    if (& incdir[ NINCLUDE - 1] < incend)
        cfatal( "Too many include directories %s"           /* _F_  */
                , norm_name, 0L, NULLST);
    *incend++ = norm_name;
}

static char *
#if PROTO
norm_path( const char * dirname)
#else
norm_path( dirname)
    char *  dirname;
#endif
/*
 * Normalize the pathname removing redundant components such as
 * "foo/../", "./" and trailing "/.".
 * Append trailing "/".
 * Returns a malloc'ed buffer.
 * This routine is called from set_a_dir() only.
 */
{
    char *  norm_name;
    char *  start;
    char *  cp1;
    char *  cp2;
    size_t  len, slen;

    len = slen = strlen( dirname);
    start = norm_name = xmalloc( len + 2);  /* Need a new buffer    */
    strcpy( norm_name, dirname);
    cp1 = norm_name + len;
    if (*(cp1 - 1) != PATH_DELIM) {
        *(norm_name + len++) = PATH_DELIM;  /* Append PATH_DELIM    */
        *(norm_name + len) = EOS;
    }
    if (len <= 1)                                   /* Only "/"     */
        return  norm_name;
#if SYS_FAMILY == SYS_MSDOS
    bsl2sl( norm_name, FALSE);
#endif
#if FNAME_FOLD
    conv_case( norm_name, cp1, LOWER);
#endif
#if SPECIAL_PATH_DELIM                  /* ':' ?    */
    for (cp1 = norm_name; *cp1 != EOS; cp1++) {
        if (*cp1 == PATH_DELIM)
            *cp1 = '/';
    }
#endif
    cp1 = norm_name;

#if SYS_FAMILY == SYS_MSDOS
    if (*(cp1 + 1) == ':')
        start = cp1 += 2;               /* Skip the drive letter    */
#endif

    if (strncmp( cp1, "./", 2) == 0)    /* Remove beginning "./"    */
        memmove( cp1, cp1 + 2, strlen( cp1 + 2) + 1);       /* +1 for EOS   */

    /* Remove redundant "foo/../".  This routine works recursively. */
    while ((cp1 = strstr( cp1, "/../")) != NULL) {
        *cp1 = EOS;
        if ((cp2 = strrchr( start, '/')) != NULL) {
            if (*(cp1 - 1) != '.') {
                memmove( cp2 + 1, cp1 + 4, strlen( cp1 + 4) + 1);
                                        /* Remove "foo/../"         */
                cp1 = cp2;
            } else {                    /* "../../../"              */
                *cp1 = '/';
                cp1 += 3;
            }
        } else if (*(cp1 - 1) != '.') { /* Should be beginning component    */
            memmove( start, cp1 + 4, strlen( cp1 + 4) + 1);
                                    /* Remove beginning "foo/../"   */
            cp1 = start;
        } else {                    /* "../../" at the beginning    */
            *cp1 = '/';
            cp1 += 3;
        }
    }

    cp1 = start;
    while ((cp1 = strstr( cp1, "/./")) != NULL)
        memmove( cp1, cp1 + 2, strlen( cp1 + 2) + 1);
                                        /* Remove "/." of "/./"     */
#if SPECIAL_PATH_DELIM
    for (cp1 = start; *cp1 != EOS; cp1++) {
        if (*cp1 == '/')
            *cp1 = PATH_DELIM;
    }
#endif
    len = strlen( norm_name);
    if (slen > len)                                 /* Truncated    */
        memset( norm_name + len, EOS, slen - len + 1);
        /* Make sure to clear the trailing characters for getopt()  */
    return  norm_name;
}

void
#if PROTO
conv_case( char * name, char * lim, int upper)
#else
conv_case( name, lim, upper)
    char *  name;                       /* (diretory) Name          */
    char *  lim;                        /* End of (directory) name  */
    int     upper;                      /* TRUE if to upper         */
#endif
/* Convert a string to upper-case letters or lower-case letters in-place    */
{
    int     c;
    char *  sp;

    for (sp = name; sp < lim; sp++) {
        c = *sp & UCHARMAX;
#if USE_MBCHAR
        if ((type[ c] & mbstart)) {
            char    tmp[ FILENAMEMAX];
            char *  tp = tmp;
            *tp++ = *sp++;
            mb_read( c, &sp, &tp);
        } else
#endif
        {
            if (upper)
                *sp = toupper( c);
            else
                *sp = tolower( c);
        }
    }
}

#if OK_MAKE

#if COMPILER == GNUC

void
#if PROTO
chk_env( void)
#else
chk_env()
#endif
/*
 * Check the environment variables to specify version of GNU C and
 * output of dependency lines.
 */
{
    char *  env;
    char *  cp;

    /* Version of GNU C */
    if ((env = getenv( ENV_VERSION)) != NULL)
        look_and_install( "__VERSION__", DEF_NOARGS, "", env);

    /* Output of dependency lines   */
    if ((env = getenv( "DEPENDENCIES_OUTPUT")) == NULL) {
        if ((env = getenv( "SUNPRO_DEPENDENCIES")) == NULL)
            return;
        else
            mkdep |= MD_SYSHEADER;
    }
    mkdep |= MD_MKDEP;
    if ((cp = strchr( env, ' ')) != NULL) {
        *cp++ = EOS;
        while (*cp == ' ')
            cp++;
    }
    if (! mkdep_fp)                     /* Command line option precedes */
        mkdep_fp = fopen( env, "a");
    if (! mkdep_target)
        mkdep_target = cp;
}
#endif  /* COMPILER == GNUC   */

void
#if PROTO
put_depend( const char * filename)
#else
put_depend( filename)
    char *  filename;
#endif
/*
 * Append a header name to the source file dependency line.
 */
{
#define MAX_OUT_LEN     76      /* Maximum length of output line    */
#define MKDEP_MAXLEN    (MKDEP_MAX * 0x40)
    static char     output[ MKDEP_MAXLEN];          /* File names   */
    static char *   pos[ MKDEP_MAX];      /* Pointers to filenames  */
    static int      pos_num;              /* Index of pos[]         */
    static char *   out_p;                /* Pointer to output[]    */
    static FILE *   fp;         /* Path to output dependency line   */
    static size_t   llen;       /* Length of current physical output line   */
    char **         pos_pp;               /* Pointer to pos         */
    size_t          fnamlen;              /* Length of filename     */

    if (fp == NULL) {   /* Main source file.  Have to initialize.   */
        out_p = md_init( filename, output);
        fp = mkdep_fp;
        llen = strlen( output);
    } else if (filename == NULL) {              /* End of input     */
        out_p = stpcpy( out_p, "\n\n");
        if (mkdep & MD_PHONY) {
            /* Output the phony target line for each recorded header files. */
            char *  cp;
            int     c;

            if (strlen( output) * 2 + (pos_num * 2) >= MKDEP_MAXLEN) {
                cerror( "Too long dependency line\n"        /* _E_  */
                        , NULLST, 0L, NULLST);
                fputs( output, fp);
                return;
            }
            pos_num--;
            for (pos_pp = &pos[ 0]; pos_pp <= &pos[ pos_num]; pos_pp++) {
                if (pos_pp == &pos[ pos_num]) {
                    for (cp = *pos_pp; *cp != '\n'; cp++)
                        ;
                    c = '\n';
                } else {
                    cp = *(pos_pp + 1) - 1;
                    while( *cp == ' ' || *cp == '\\' || *cp == '\n')
                        cp--;
                    c = *(++cp);
                }
                *cp = EOS;
                out_p = stpcpy( out_p, *pos_pp);
                out_p = stpcpy( out_p, " :\n\n");
                *cp = c;
            }
        }
        fputs( output, fp);
        return;
    }

    fnamlen = strlen( filename);
    /* Check the recorded filename  */
    for (pos_pp = pos; pos_pp < &pos[ pos_num]; pos_pp++) {
        if (memcmp( *pos_pp, filename, fnamlen) == 0)
            return;                 /* Already recorded filename    */
    }
    /* Any new header.  Append its name to output.  */
    if (llen + fnamlen > MAX_OUT_LEN) {         /* Line is long     */
        out_p = stpcpy( out_p, " \\\n ");       /* Fold it          */
        llen = 1;
    }
    llen += fnamlen + 1;
    if (pos_num >= MKDEP_MAX
            || out_p + fnamlen + 1 >= output + MKDEP_MAXLEN)
        cfatal( "Too long dependency line: %s", output, 0L, NULLST);
    *out_p++ = ' ';
    pos[ pos_num++] = out_p;
    out_p = stpcpy( out_p, filename);
}

static char *
#if PROTO
md_init( const char * filename, char * output)
#else
md_init( output, fp_p)
    char *  filename;           /* The source file name             */ 
    char *  output;             /* Output to dependency file        */
#endif
/*
 * Initialize output file and target.
 */
{
    char    prefix[ FILENAMEMAX];
    char *  cp;
    size_t  len;
    char *  out_p;
    const char *    target = filename;
    const char *    cp0;

    if (! mkdep_target || ! mkdep_fp) {         /* Make target name */
#ifdef  PATH_DELIM
        if ((cp0 = strrchr( target, PATH_DELIM)) != NULL)
            target = cp0 + 1;
#endif
        if ((cp0 = strrchr( target, '.')) == NULL)
            len = strlen( target);
        else
            len = (size_t) (cp0 - target);
        memcpy( prefix, target, len);
        cp = prefix + len;
        *cp++ = '.';
    }

    if (! mkdep_fp) {   /* Unless already opened by -MF, -MD, -MMD options  */
        if (mkdep & MD_FILE) {
            strcpy( cp, "d");
            mkdep_fp = fopen( prefix, "w");
        } else {
            mkdep_fp = fp_out;  /* Output dependency line to normal output  */
            no_output++;                /* Without normal output    */
        }
    }

    if (mkdep_target) {         /* -MT or -MQ option is specified   */
        if (mkdep & MD_QUOTE) {         /* 'Quote' $, \t and space  */
            out_p = md_quote( output);
        } else {
            out_p = stpcpy( output, mkdep_target);
        }
    } else {
        strcpy( cp, OBJEXT);
        out_p = stpcpy( output, prefix);
    }

    out_p = stpcpy( out_p, " : ");
    return  out_p;
}

static char *
#if PROTO
md_quote( char * out_p)
#else
md_quote( out_p)
    char *  out_p;
#endif
/*
 * 'Quote' $, tab and space.
 * This part of source code is taken from GNU C V.3.2.
 */
{
    char *  p;
    char *  q;

    for (p = mkdep_target; *p; p++, out_p++) {
        switch (*p) {
        case ' ':
        case '\t':
            /* GNU-make treats backslash-space sequence peculiarly  */
            for (q = p - 1; mkdep_target <= q && *q == '\\'; q--)
                *out_p++ = '\\';
            *out_p++ = '\\';
            break;
        case '$':
            *out_p++ = '$';
            break;
        default:
            break;
        }
        *out_p = *p;
    }
    *out_p = EOS;
    return  out_p;
}

#endif  /* OK_MAKE  */

static char * toolong_fname = "Too long header name \"%s%.0ld%s\""; /* _F_  */
#if ! OK_IF_JUNK
static char * excess_token =
        "Excessive token sequence \"%s\"";          /* _E_, _W1_    */
#endif

int
#if PROTO
do_include( int next)
#else
do_include( next)
    int     next;       /* TRUE if the directive is #include_next   */
#endif
/*
 * Process the #include line.
 * There are three variations:
 *      #include "file"         search somewhere relative to the
 *                              current (or source) directory, if not
 *                              found, treat as #include <file>.
 *      #include <file>         Search in an implementation-dependent
 *                              list of places.
 *      #include macro-call     Expand the macro call, it must be one of
 *                              "file" or <file>, process as such.
 * On success : return TRUE;
 * On failure of syntax : return FALSE;
 * On failure of file opening : return FALSE.
 * do_include() always absorbs the line (including the <newline>).
 */
{
    const char * const  no_name = "No header name";         /* _E_  */
#if MODE >= STANDARD
    char            header[ FILENAMEMAX + 16];
    char            *hp;
    int             c;
    int             token_type;
#endif
    char *          fname;
    register int    delim;                  /* " or <, >            */

    if ((delim = skip_ws()) == '\n') {      /* No argument          */
        cerror( no_name, NULLST, 0L, NULLST);
        return  FALSE;
    }
    fname = infile->bptr - 1;       /* Current token for diagnosis  */

#if MODE >= STANDARD
    if (type[ delim] & LET) {               /* Maybe a macro        */
        if ((token_type = get_unexpandable( delim, FALSE)) == NO_TOKEN) {
            cerror( no_name, NULLST, 0L, NULLST);   /* Expanded to  */
            return  FALSE;                          /*   0 token.   */
        }
#if MODE >= STANDARD
        if (macro_line == MACRO_ERROR)      /* Unterminated macro   */
            return  FALSE;                  /*   already diagnosed. */
#endif
        delim = work[ 0];                   /* Expanded macro       */
        if (token_type == STR) {            /* String literal form  */
            goto found_name;
#if 1 || MODE != POST_STANDARD
        } else if (token_type == OPE && openum == OP_LT) {  /* '<'  */
            hp = header;
            while ((c = skip_ws()) != '\n') {   /* Eliminate spaces */
                token_type = get_unexpandable( c, FALSE);
                if (header + FILENAMEMAX + 15 < hp + (int) (workp - work))
                    cfatal( toolong_fname, header, 0L, work);
                hp = stpcpy( hp, work);     /* Expand any macros    */
                if (token_type == OPE && openum == OP_GT)   /* '>'  */
                    break;
            }
            unget_string( header, NULLST);  /* To re-read           */
            workp = scan_quote( delim, work, work + FILENAMEMAX, TRUE);
                                        /* Re-construct or diagnose */
            goto scanned;
        } else {                            /* Any other token in-  */
            goto not_header;                /*   cluding <=, <<, <% */
#endif
        }
    }
#endif  /* MODE >= STANDARD    */

    if (delim == '"') {                     /* String literal form  */
        workp = scan_quote( delim, work, work + FILENAMEMAX, FALSE);
#if 1 || MODE != POST_STANDARD
    } else if (delim == '<'
            && scan_token( delim, (workp = work, &workp), work_end) == OPE
            && openum == OP_LT) {           /* Token '<'            */
        workp = scan_quote( delim, work, work + FILENAMEMAX, TRUE);
                                            /* Don't decompose      */
#endif
    } else {                                /* Any other token      */
        goto not_header;
    }

scanned:
    if (workp == NULL)                      /* Missing closing '>'  */
        goto  syntax_error;

found_name:
    *--workp = EOS;                     /* Remove the closing and   */
    fname = save_string( &work[ 1]);    /*  the starting delimiter. */

#if OK_IF_JUNK
    skip_nl();
#else   /* ! OK_IF_JUNK */
#if MODE >= STANDARD
    if (get_unexpandable( skip_ws(), FALSE) != NO_TOKEN) {
        cerror( excess_token, work, 0L, NULLST);
        skip_nl();
        goto  error;
    }
    get();                              /* Skip the newline         */
#else   /* MODE == PRE_STANDARD */
    if (skip_ws() != '\n') {
        if (warn_level & 1)
            cwarn( excess_token, infile->bptr-1, 0L, NULLST);
        skip_nl();
    }
#endif  /* MODE == PRE_STANDARD */
#endif  /* ! OK_IF_JUNK */

    if (open_include( fname, (delim == '"'), next)) {
        goto opened;
    }

    cerror( "Can't open include file \"%s\"", fname, 0L, NULLST);   /* _E_  */
    goto error;

not_header:
    cerror( "Not a header name \"%s\"", fname, 0L, NULLST);         /* _E_  */
syntax_error:
    skip_nl();
    return  FALSE;
error:
    free( fname);
    return  FALSE;
opened:
    free( fname);
    errno = 0;      /* Clear errno possibly set by path searching   */
    return  TRUE;
}

static int
#if PROTO
open_include( char * filename, int searchlocal, int next)
#else
open_include( filename, searchlocal, next)
    char *  filename;               /* File name to include         */
    int     searchlocal;            /* TRUE if #include "file"      */
    int     next;                   /* TRUE if #include_next        */
#endif
/*
 * Open an include file.  This routine is only called from do_include()
 * above, but was written as a separate subroutine for portability.
 * It searches the list of directories via search_dir() and opens the file
 * via open_file(), linking it into the list of active files.
 * Returns TRUE if the file was opened, FALSE if open_file() fails.
 */
{
    int     full_path;              /* Filename is full-path-list   */
    int     has_dir = FALSE;        /* Includer has directory part  */
    char    fullname[ FILENAMEMAX + 1] = { EOS, };
                                    /* Directory/filename           */
#if FNAME_FOLD
    char *  dirend;         /* End of directory part (if any) of filename   */
#endif

#if SYS_FAMILY == SYS_MSDOS
    bsl2sl( filename, TRUE);
#endif
#if FNAME_FOLD  /* If O.S. folds upper and lower cases of file-name */
    /* Convert directory name to lower-case-letters */
    if ((dirend = strrchr( filename, PATH_DELIM)) != NULL)
        conv_case( filename, dirend, LOWER);
    else
        dirend = filename;
    /* Convert file-name part to lower-case-letters */
    conv_case( dirend, dirend + strlen( dirend), LOWER);
#endif

#if SYS_FAMILY == SYS_UNIX || SYS_FAMILY == SYS_OS9
    if (filename[0] == PATH_DELIM)
#else
#if SYS_FAMILY == SYS_MSDOS
    if (filename[1] == ':' && filename[2] == PATH_DELIM)
#else
#if SYSTEM == SYS_MAC           /* I don't know.  Write by yourself */
    if (filename[0] != PATH_DELIM && strchr( filename, PATH_DELIM))
#else
/* For other systems you should write code here.    */
    if (0)
#endif
#endif
#endif
        full_path = TRUE;
    else
        full_path = FALSE;

    if (!full_path && searchlocal && (search_rule & SOURCE))
        has_dir = (**(infile->dirp) != EOS)
                || has_directory( infile->filename, fullname);

#if COMPILER == GNUC
    if (!full_path) {
        if ((!searchlocal && incdir < sys_dirp)
                                /* -I- option is specified  */
                || next)                /* #include_next    */
        goto  search_dirs;
    }
#endif

    if ((searchlocal && ((search_rule & CURRENT) || !has_dir)) || full_path) {
        /*
         * Look in local directory first.
         * Try to open filename relative to the "current directory".
         */
        if (open_file( &null, filename, searchlocal && !full_path))
            return  TRUE;
    }
    if (full_path)
        return  FALSE;

    if (searchlocal && (search_rule & SOURCE) && has_dir) {
        /*
         * Look in local directory of source file.
         * Try to open filename relative to the "source directory".
         */
        strcat( fullname, filename);
        if (open_file( infile->dirp, fullname, TRUE))
            return  TRUE;
    }

    /* Search the system include directories    */
#if COMPILER == GNUC
search_dirs:
#endif
    if (search_dir( filename, searchlocal, next))
        return  TRUE;

#if COMPILER == BORLANDC && MODE >= STANDARD
    if ((cplus && !strchr( filename, '.'))
                                    /* <iostream>, <cstdlib>, etc   */
            || strlen( filename) > 10) {    /* Longer than 8 + 1    */
        char *  fname;              /* Mapped file-name             */
        char *  fp;                 /* File-name without directory  */
        char *  cp;                 /* Place of the '.'             */
        size_t  len;        /* Length of file-name without suffix   */

        fname = xmalloc( strlen( filename) + 3);
        strcpy( fname, filename);
        if (cplus && !strchr( filename, '.')) {
            strcat( fname, ".h");           /* Append ".h"          */
            if (search_dir( fname, searchlocal, next))
                goto  success;
        }
        /*
         * If the filename is longer than 8 + 1 (or 8 + 3), truncate it
         * (what a mess !).
         */
        if ((fp = strrchr( fname, PATH_DELIM)) != NULL)
            fp++;
        else
            fp = fname;
        if ((cp = strchr( fp, '.')) != NULL)
            len = cp - fp;
        else
            len = strlen( fp);
        if (len > 8) {
            if (fp[ 8] == '_') {
                if (cp)
                    *cp = EOS;
                fp[ 8] = '.';
            } else if (cp) {
                memmove( fp + 8, cp, strlen( cp) + 1);
            } else {
                strcpy( fp + 8, ".h");
            }
            if (strlen( fp) > 12)
                fp[ 12] = EOS;
            if (search_dir( fname, searchlocal, next))
                goto  success;
        }
        free( fname);
        return  FALSE;
success:
        free( fname);
        return  TRUE;
    }
#endif  /* COMPILER == BORLANDC && MODE >= STANDARD */

    return  FALSE;
}

static int
#if PROTO
has_directory( const char * source, char * directory)
#else
has_directory( source, directory)
    char *  source;                 /* Filename to examine          */
    char *  directory;              /* Put directory stuff here     */
#endif
/*
 * If a directory is found in the 'source' filename string (i.e. "includer"),
 * the directory part of the string is copied to 'directory' and 
 * has_directory() returns TRUE.
 * Else, nothing is copied and it returns FALSE.
 */
{
    char *  sp;
    size_t  len;

    if ((sp = strrchr( source, PATH_DELIM)) == NULL) {
        return  FALSE;
    } else {
        len = (size_t)(sp - source) + 1;
        memcpy( directory, source, len);
        directory[ len] = EOS;
#if FNAME_FOLD
        conv_case( directory, directory + len, LOWER);
#endif
        return  TRUE;
    }
}

static int
#if PROTO
search_dir( char * filename, int searchlocal, int next)
#else
search_dir( filename, searchlocal, next)
    char *  filename;               /* File name to include         */
    int     searchlocal;            /* #include "header.h" for GNUC */
    int     next;                   /* TRUE if #include_next        */
#endif
/*
 * Look in any directories specified by -I command line arguments,
 * specified by environment variable, then in the builtin search list.
 */
{
    register const char **  incptr;         /* -> inlcude directory */

    if (next && **inc_dirp != EOS) {
        incptr = inc_dirp + 1;
        /* In case of include_next search after the includer's directory    */
    } else {
    /* If (next && **inc_dirp == EOS), it should be #include_next "header.h"*/
#if COMPILER == GNUC
        if (searchlocal)
            incptr = incdir;
        else
            incptr = sys_dirp;
#else
        incptr = incdir;
#endif
    }

    for ( ; incptr < incend; incptr++) {
        if (strlen( *incptr) + strlen( filename) >= FILENAMEMAX) {
            char *  cp;
            cp = stpcpy( work, *incptr);
            strcpy( cp, filename);
            cfatal( toolong_fname, work, 0L, "");           /* _F_  */
        }
        if (open_file( incptr, filename, FALSE))
            /* Now infile has been renewed  */
            return  TRUE;
    }

    return  FALSE;
}

static int
#if PROTO
open_file( const char ** dirp, const char * filename, int local)
#else
open_file( dirp, filename, local)
    char **     dirp;
    char *      filename;
    int         local;
#endif
/*
 * Open a file, add it to the linked list of open files, close the includer
 * if nessesary and truncate the includer's buffer.
 * This is called from open_include() and at_start().
 */
{
#if HOST_COMPILER == BORLANDC
    /* Borland's fopen() fails to set errno to EMFILE.  */
    static int  max_open = 15;
#else
    static int  max_open;
#endif
    int         len;
    FILEINFO *  file = infile;
    register FILE *     fp;
    char *      cp;
    char        fullname[ FILENAMEMAX + 1];

#if DEBUG
    if (debug & PATH)
        fprintf( fp_debug, "Searching %s\n", **dirp == EOS ? "." : *dirp);
#endif
    cp = stpcpy( fullname, *dirp);
    strcat( cp, filename);
#if MODE >= STANDARD
    if (included( fullname))                /* Once included        */
        return  TRUE;
#endif

    if ((max_open != 0 && max_open <= include_nest)
                            /* Exceed the known limit of open files */
            || ((fp = fopen( fullname, "r")) == NULL && errno == EMFILE)) {
                            /* Reached the limit for the first time */
        /*
         * Table of open files is full.
         * Remember the file position and close the includer.
         * The state will be restored by get_line() on end of the included.
         */
        file->pos = ftell( file->fp);
        fclose( file->fp);
        /* In case of failure, re-open the includer */
        if ((fp = fopen( fullname, "r")) == NULL) {
            file->fp = fopen( cur_fullname, "r");
            fseek( file->fp, file->pos, SEEK_SET);
            return  FALSE;
        }
        if (max_open == 0)      /* Remember the limit of the system */
            max_open = include_nest;
    } else if (fp == NULL) {    /* No file, illegal path name or so */
        return  FALSE;
    }
    /* Truncate buffer of the includer to save memory   */
    len = (int) (file->bptr - file->buffer);
    if (len) {
        file->buffer = xrealloc( file->buffer, len + 1);
        file->bptr = file->buffer + len;
    }

#if OK_MAKE
    if (mkdep && ((mkdep & MD_SYSHEADER) || local))
        put_depend( fullname);          /* Output dependency line   */
#endif

    add_file( fp, filename);    /* Add file-info to the linked list */
    /*
     * Remember the directory for #include_next.
     * Note: inc_dirp is restored to the parent includer's directory
     *   by get() when the current includer is finished.
     */
    infile->dirp = inc_dirp = dirp;
    strcpy( cur_fullname, fullname);

    if (zflag) {
        no_output++;        /* Don't output the included file       */
    } else {
        line = 1;                       /* Working on line 1 now    */
        sharp();            /* Print out the included file name     */
    }
    line = 0;                           /* To read the first line   */
    return  TRUE;
}

void
#if PROTO
add_file( FILE * fp, const char * filename)
#else
add_file( fp, filename)
    FILE *      fp;                         /* Open file pointer    */
    char *      filename;                   /* Name of the file     */
#endif
/*
 * Initialize tables for this open file.  This is called from open_file()
 * (for #include files), and from the entry to cpp to open the main input
 * file.  It calls a common routine get_file() to build the FILEINFO
 * structure which is used to read characters.
 */
{
    register FILEINFO *     file;

    filename = set_fname( filename);    /* Search or append to fnamelist[]  */
    file = get_file( filename, (size_t) NBUFF); /* file == infile   */
    file->fp = fp;                      /* Better remember FILE *   */
    cur_fname = filename;

#if MODE >= STANDARD
    if ((warn_level & 4) && include_nest == inc_nest_min + 1)
            cwarn( "More than %.0s%ld nesting of #include"  /* _W4_ */
                    , NULLST , (long) inc_nest_min , NULLST);
#endif
    include_nest++;
}

static const char *
#if PROTO
set_fname( const char * filename)
#else
set_fname( filename)
    char *  filename;
#endif
/*
 * Register the source filename to fnamelist[].
 * Search fnamelist[] for filename or append filename to fnamelist[].
 * Returns the pointer.
 */
{
    const char **     fnamep;

    /* Register the filename in fnamelist[] */
    fnamep = fnamelist;
    while (fnamep < fname_end) {
        if (str_eq( *fnamep, filename)) /* Already registered       */
            break;
        fnamep++;
    }
    if (fnamep < fname_end) {
        filename = *fnamep;
    } else {
        if (fname_end - fnamelist >= FNAMELIST)
            cfatal( "Too many include files", NULLST, 0L, NULLST);  /* _F_  */
        *fname_end = xmalloc( strlen( filename) + 1);
        filename = strcpy( *(char **)fname_end++, filename);
                                /* Global pointer for get_file()    */
    }
    return  filename;
}

#if SYS_FAMILY == SYS_MSDOS

static char *
bsl2sl( char * filename, int in_source)
/*
 * Convert '\\' in the path-list to '/'.
 */
{
    static int  converted = FALSE;
    static int  diagnosed = FALSE;
    char *  cp;

    cp = filename;

    while (*cp) {
        if (bsl_in_mbchar) {
            int     c;
            c = *cp & UCHARMAX;
            if (type[ c] & mbstart) {  /* First byte of MBCHAR */
                char    tmp[ FILENAMEMAX];
                char *  tp = tmp;
                *tp++ = *cp++;
                mb_read( c, &cp, &tp);
                            /* Read over the multi-byte characters  */
                continue;
            }
        }
        if (*cp == '\\') {
            *cp++ = PATH_DELIM;
            if (!in_source) {   /* Backslash in command-line or environ     */
                if (!converted) {
                    fprintf( fp_err, "Converted \\ to %c\n", PATH_DELIM);
                    converted = TRUE;
                }
            } else {            /* Backslash in source program      */
                if (!diagnosed && (warn_level & 2)) {
                    cwarn(
        "Converted \\ to %s", "/", 0L, NULLST);             /* _W2_ */
                    diagnosed = TRUE;       /* Diagnose only once   */
                }
            }
        } else {
            cp++;
        }
    }

    return  filename;
}

#endif  /* SYS_FAMILY == SYS_MSDOS  */

static const char * const   unknown_arg =
        "Unknown argument \"%s\"";      /*_W1_*/
#if MODE >= STANDARD || DEBUG || DEBUG_EVAL
static const char * const   not_ident =
        "Not an identifier \"%s\"";     /*_W1_*/
#endif

#if ! OK_IF_JUNK
static int
#if PROTO
is_junk( void)
#else
is_junk()
#endif
/*
 * Check the trailing junk in a directive line.
 */
{
    int     c;

    c = skip_ws();
    unget();
    if (c != '\n') {                        /* Trailing junk        */
        if (warn_level & 1)
            cwarn( unknown_arg, infile->bptr, 0L, NULLST);
        return TRUE;
    } else {
        return FALSE;
    }
}
#endif  /* ! OK_IF_JUNK */

#if MODE >= STANDARD

#define PUSH    1
#define POP    -1

#define __SETLOCALE 1   /* #pragma __setlocale( "encoding") */
#define SETLOCALE   2   /* #pragma setlocale( "encoding")   */

void
#if PROTO
do_pragma( void)
#else
do_pragma()
#endif
/*
 * Process the #pragma lines.
 * 1. Process the sub-directive for cpp.
 * 2. Pass the line to the compiler-proper who understands #pragma.
 *      #pragma __put_defines, #pragma __preprocess, #pragma __preprocessed
 *      and #pragma __once are, however, not put out so as not to duplicate
 *      output when re-preprocessed.
 * 3. Warn and skip the line for the compiler-proper who can't understand
 *      #pragma.
 *    When EXPAND_PRAGMA == TRUE and (__STDC_VERSION__ >= 199901L or
 * __cplusplus >= 199901L), the line is subject to macro expansion unless
 * the next to 'pragma' token is 'STDC'.
 */
{
    register int    c;
    int             warn = FALSE;
                        /* Necessity of warning for ! HAVE_PRAGMA   */
    int             token_type;
    char *          bp;                     /* Pointer to argument  */
    char *          tp;
    FILEINFO *      file;

    c = skip_ws();
    bp = infile->bptr - 1;  /* Remember token to pass to compiler   */
    if (c == '\n') {
        if (warn_level & 1)
            cwarn( "No sub-directive", NULLST, 0L, NULLST); /* _W1_ */
        unget();
        goto  parse_end;
    }
    token_type = scan_token( c, (tp = work, &tp), work_end);
#if EXPAND_PRAGMA
    if (stdc3 && token_type == NAM
            && !str_eq( identifier, "STDC")) {
        DEFBUF *        defp;
        char *          mp;
        char *          mp_end;

        bp = mp = xmalloc( (size_t)(NMACWORK + IDMAX));
                                    /* Buffer for macro expansion   */
        mp_end = mp + NMACWORK;
        tp = stpcpy( mp, identifier);
        do {                /* Expand all the macros in the line    */
            if (token_type == NAM && (defp = is_macro( &tp)) != NULL)
                tp = expand( defp, bp, mp_end);
            token_type = scan_token( c = get(), (bp = tp, &tp), mp_end);
        } while (c != '\n');
        unget_string( mp, NULLST);                  /* To re-read   */
        free( mp);
        c = skip_ws();
        bp = infile->bptr - 1;
        token_type = scan_token( c, (tp = work, &tp), work_end);
    }
#endif
    if (token_type != NAM) {
        if (warn_level & 1)
            cwarn( not_ident, work, 0L, NULLST);
    } else if (str_eq( identifier, "__put_defines")) {
        if (! is_junk()) {
            dump_def( dDflag);              /* #pragma __put_defines*/
            goto  skip_nl;
        }
    } else if (str_eq( identifier, "__preprocess")) {
        if (! is_junk()) {                  /* #pragma __preprocess */
          // <-- C2SC
          if (c2sc_process_mode == 2)
            fputs( "#pragma __preprocessed\n", fp_out);  // original
          else
            fputs ("$(sc::%%pragma \"__preprocessed\")\n", fp_out);
          // C2SC -->
          /* Just putout the directive    */
          goto  skip_nl;
        }
    } else if (str_eq( identifier, "__preprocessed")) {
        if (! is_junk()) {              /* #pragma __preprocessed   */
            skip_nl();
            do_preprocessed();
            return;
        }
    } else if (str_eq( identifier, "__once")) { /* #pragma __once   */
once:   if (! is_junk()) {
            file = infile;
            if (stdc3 && file->fp == NULL)
                file = file->parent;
            tp = stpcpy( work, *(file->dirp));
            strcpy( tp, file->filename);
            do_once( work);
            goto  skip_nl;
        }
    } else if (str_eq( identifier, "__include_next")) {
        do_include( TRUE);              /* #pragma __include_next   */
    } else if (str_eq( identifier, "__warning_cpp")) {
        cwarn( infile->buffer, NULLST, 0L, NULLST);
    } else if (str_eq( identifier, "__push_macro")) {
        push_or_pop( PUSH);
        goto  skip_nl;
    } else if (str_eq( identifier, "__pop_macro")) {
        push_or_pop( POP);
        goto  skip_nl;
#if SYSTEM != SYS_MSDOS && COMPILER != MSC
    } else if (str_eq( identifier, "__setlocale")) {
        if (skip_ws() == '('
                && scan_token( skip_ws(), (tp = work, &tp), work_end) == STR
                && skip_ws() == ')') {
            if (! is_junk()) {
                work[ 0] = *(tp - 1) = '\0';
                set_encoding( work + 1, NULL, __SETLOCALE);
                work[ 0] = *(tp - 1) = '"';
            }   /* else warned by is_junk() */
        } else {
            warn = TRUE;
        }
#endif
#if DEBUG || DEBUG_EVAL
    } else if (str_eq( identifier, "__debug_cpp")) {
        warn = ! do_debug( TRUE);           /* #pragma __debug_cpp  */
    } else if (str_eq( identifier, "__end_debug_cpp")) {
        warn = ! do_debug( FALSE);      /* #pragma __end_debug_cpp  */
#endif

#if COMPILER == GNUC
    /* The #pragma lines for GNU C / cpp is skipped not to confuse cc1. */
    } else if (str_eq( identifier, "GCC")) {
        if ((scan_token( skip_ws(), (tp = work, &tp), work_end) == NAM)
                && (str_eq( identifier, "poison")
                    || str_eq( identifier, "dependency")
                    || str_eq( identifier, "system_header"))) {
            if (warn_level & 2)
                cwarn( "Skipped the #pragma line"           /*_W2_  */
                        , NULLST, 0L, NULLST);
            goto skip_nl;
        }
#endif

#if COMPILER == GNUC || COMPILER == MSC || COMPILER == LCC
    } else if (str_eq( identifier, "once")) {   /* #pragma once     */
        goto once;
#endif

#if COMPILER == MSC
    } else if (str_eq( identifier, "push_macro")) {
        push_or_pop( PUSH);
        goto  skip_nl;
    } else if (str_eq( identifier, "pop_macro")) {
        push_or_pop( POP);
        goto  skip_nl;
#if SYSTEM != SYS_MSDOS
    } else if (str_eq( identifier, "setlocale")) {
        if (skip_ws() == '('
                && scan_token( skip_ws(), (tp = work, &tp), work_end) == STR
                && skip_ws() == ')') {
            if (! is_junk()) {
                work[ 0] = *(tp - 1) = '\0';
                set_encoding( work + 1, NULL, SETLOCALE);
                work[ 0] = *(tp - 1) = '"';
            }   /* else warned by is_junk() */
        } else {
            warn = TRUE;
        }
#endif
#endif

#if COMPILER == LCC
    } else if (str_eq( identifier, "optimize")
                && (skip_ws() == '(')
                && (type[ (c = skip_ws()) & UCHARMAX] == DIG)
                && (skip_ws() == ')')) {
        char    tmp[ 2];

        tmp[ 0] = c;
        tmp[ 1] = EOS;
        look_and_install( optim_name, DEF_NOARGS - 1, null, tmp);
#endif

#if COMPILER == COMPILER_UNKNOWN
    /*
     * Write here any compiler-specific #pragma sub-directive which should
     * be processed by preprocessor.
     */
#endif

    } else {
        warn = TRUE;                        /* Need to warn         */
    }

parse_end:
    /*
     * If HAVE_PRAGMA, #pragma lines are handed to the compiler proper.
     * Else #pragma lines are warned and skipped.
     */
#if HAVE_PRAGMA
#if COMPILER == LSIC
    if (!no_output) {
      // <-- C2SC
      if (c2sc_process_mode == 1 && !strncmp("c2sc_", identifier, 5))
        if (std_line_prefix)
	  fputs( "#pragma ", fp_se2);
        else
	  fputs( "#p ", fp_se2);
      // C2SC -->
      else if (c2sc_process_mode == 2) // C2SC
        if (std_line_prefix)
	  fputs( "#pragma ", fp_out);  // original
        else
	  fputs( "#p ", fp_out);       // original
      else
        fputs( "$(sc::%pragma ", fp_out); // C2SC
    }
#else
    if (!no_output) {
      // <-- C2SC
      if (c2sc_process_mode == 1 && !strncmp("c2sc_", identifier, 5))
        fputs( "#pragma ", fp_se2);
      // C2SC -->      
      else if (c2sc_process_mode == 2) // C2SC
        fputs( "#pragma ", fp_out);  // original
      else
        fputs( "$(sc::%pragma ", fp_out); // C2SC
    }
#endif
    if (!no_output) {
      // <-- C2SC
      if (c2sc_process_mode == 1 && !strncmp("c2sc_", identifier, 5))
        fputs( bp, fp_se2);
      // C2SC -->      
      else if (c2sc_process_mode == 2)  // C2SC
        fputs( bp, fp_out);    /* Line is put out, though  */
      // <-- C2SC
      else
        {
          int i;
          fputc( '"', fp_out );
          for( i=0 ; bp[i]!=EOS && bp[i]!='\n' ; i++ )
            fputc( bp[i], fp_out );
          fputc( '"', fp_out );
          fputs( ")\n", fp_out);
        }
      // C2SC -->
    }
    wrong_line = TRUE;                  /*   it is a directive line */
#else   /* ! HAVE_PRAGMA    */
    if (warn && (warn_level & 1))
        cwarn( unknown_arg, identifier, 0L, NULLST);
    /* Else already warned by is_junk() or do_debug().   */
#endif  /* ! HAVE_PRAGMA    */
skip_nl: /* Don't use skip_nl() which skips to the newline in source file */
    while (get() != '\n')
        ;
}

typedef struct inc_list {               /* List of #pragma once file*/
    struct inc_list *   next;           /* Next file                */
    char            fname[ 1];          /* Filename                 */
} INC_LIST;

static INC_LIST *   start_inc = NULL;   /* The first file in list   */
static INC_LIST *   last_inc;           /* The last file in list    */

static void
#if PROTO
do_once( const char * filename)
#else
do_once( filename)
    char *  filename;
#endif
/*
 * Process #pragma __once so as not to re-include the file in future.
 * This directive has been imported from GNU C V.1.* / cpp as an extension.
 */
{
    register INC_LIST *     inc;
    size_t      fnamlen;

    fnamlen = strlen( filename);
    inc = (INC_LIST *) xmalloc( sizeof (INC_LIST) + fnamlen);
    memcpy( inc->fname, filename, fnamlen + 1);
    inc->next = NULL;
    if (start_inc == NULL)
        start_inc = last_inc = inc;         /* The first file       */
    else
        last_inc = last_inc->next = inc;    /* Append the file to the list  */
}

static int
#if PROTO
included( const char * filename)
#else
included( filename)
    char *  filename;
#endif
/*
 * Has the file been once included ?
 */
{
    register INC_LIST *     inc;

    for (inc = start_inc; inc; inc = inc->next) {
        if (str_eq( inc->fname, filename)) {    /* Already included */
#if DEBUG
            if (debug & PATH)
                fprintf( fp_debug, "Once included \"%s\"\n", filename);
#endif
            return  TRUE;
        }
    }
    return  FALSE;                          /* Not yet included     */
}

static void
#if PROTO
push_or_pop( int direction)
#else
push_or_pop( direction)
    int     direction;
#endif
{
    char *          tp;
    DEFBUF **       prevp;
    DEFBUF *        defp;
    DEFBUF *        dp;
    int             cmp;
    size_t          s_name, s_def;

    if (skip_ws() == '('
            && scan_token( skip_ws(), (tp = work, &tp), work_end) == STR
            && skip_ws() == ')') {

        if (is_junk())
            return;
        s_name = strlen( work) - 2;
        *(work + s_name + 1) = '\0';
        memcpy( identifier, work + 1, s_name + 1);  /* Remove enclosing '"' */
        prevp = look_prev( identifier, &cmp);
        if (cmp == 0) {                     /* Found the macro      */
            defp = *prevp;
            if (direction == PUSH) {    /* #pragma __push_macro( "MACRO")   */
                if (defp->push) {
                    if (warn_level & 1)
                        cwarn( "\"%s\" is already pushed"   /* _W1_ */
                                , identifier, 0L, NULLST);
                    return;
                }
                s_def = sizeof (DEFBUF) + 3 + s_name
#if MODE == STANDARD
                        + strlen( defp->parmnames)
#endif
                        + strlen( defp->repl)
#if DEBUG
                        + strlen( defp->fname)
#endif
                        ;
                dp = (DEFBUF *) xmalloc( s_def);
                memcpy( dp, defp, s_def);   /* Copy the definition  */
                dp->link = *prevp;          /* Insert to linked-list*/
                *prevp = dp;
                prevp = &dp->link;          /* Next link to search  */
            } else {            /* #pragma __pop_macro( "MACRO")    */
                if (defp->push == 0) {
                    if (defp->link == NULL
                            || ! str_eq( identifier, defp->link->name)) {
                        if (warn_level & 1)
                            cwarn( "\"%s\" has not been pushed"     /* _W1_ */
                                    , identifier, 0L, NULLST);
                        return;
                    } else {
                        *prevp = defp->link;
                                /* Link the previous and the next   */
                        free( defp);    /* Delete the definition    */
                    }
                }
            }
            while ((defp = *prevp) != NULL) {
                if ((cmp = memcmp( defp->name, identifier, s_name)) > 0)
                    break;
                defp->push += direction;        /* Increment or decrement   */
                prevp = &defp->link;
            }
        } else {
            if (warn_level & 1)
                cwarn( "\"%s\" has not been defined"        /* _W1_ */
                        , identifier, 0L, NULLST);
        }
    } else {
        if (warn_level & 1)
            cwarn( "Bad %s syntax", direction == PUSH       /* _W1_ */
                    ? "push_macro" : "pop_macro", 0L, NULLST);
    }
}

#else   /* MODE == PRE_STANDARD */

static void
#if PROTO
do_asm( int asm_start)
#else
do_asm( asm_start)
    int     asm_start;                      /* #asm ?               */
#endif
/*
 * #asm, #endasm
 * Originally written for OS-9/09 Microware C.
 */
{
    if (! compiling)
        return;
    if (asm_start == (in_asm != 0L)) {
        if (in_asm)
            cerror( "In #asm block started at line %.0s%ld" /* _E_  */
                    , NULLST, in_asm, NULLST);
        else
            cerror( "Without #asm", NULLST, 0L, NULLST);    /* _E_  */
        skip_nl();
        unget();
        return;
    }
    in_asm = asm_start ? line : 0L;
}

#endif  /* MODE == PRE_STANDARD */

void
#if PROTO
do_old( void)
#else
do_old()
#endif
/*
 * Process the out-of-standard directives.
 * GNU C permits #include_next and #warning even in STANDARD mode.
 */
{
    static const char * const   unknown
            = "Unknown #directive \"%s\"%.0ld%s";       /* _E_ _W8_ */

#if COMPILER == GNUC
    static const char * const   gnu_ext
            = "%s is not allowed by Standard%0d%s";  /* _W2_ _W8_        */

    if (str_eq( identifier, "include_next")) {
        if ((compiling && (warn_level & 2))
                || (! compiling && warn_level & 8))
            cwarn( gnu_ext, "#include_next", 0L
                    , compiling ? NULLST : " (in skipped block)");
        if (! compiling)
            return;
        do_include( TRUE);
        return;
    } else if (str_eq( identifier, "warning")) {
        if ((compiling && (warn_level & 2))
                || (! compiling && warn_level & 8))
            cwarn( gnu_ext, "#warning", 0L
                    , compiling ? NULLST : " (in skipped block)");
        if (! compiling)
            return;
        cwarn( infile->buffer, NULLST, 0L, NULLST);
                                    /* Always output the warning    */
        skip_nl();
        unget();
        return;
    } else if (str_eq( identifier, "ident") || str_eq( identifier, "sccs")) {
        if ((compiling && (warn_level & 1))
                || (! compiling && warn_level & 8)) {
            if (str_eq( identifier, "ident"))
                cwarn(
    compiling ? "Ignored #ident" : "#ident (in skipped block)"  /* _W1_ _W8_*/
                        , NULLST, 0L, NULLST);
            else
                cwarn(
    compiling ? "Ignored #sccs" : "#sccs (in skipped block)"    /* _W1_ _W8_*/
                        , NULLST, 0L, NULLST);
        }
        if (! compiling)
            return;
        skip_nl();
        unget();
        return;
    }
#endif

#if COMPILER == MSC
    if (str_eq( identifier, "using") || str_eq( identifier, "import")) {
                                            /* #using or #import    */
        if (! compiling)
            return;
        //fputs( infile->buffer, fp_out);     /* Putout the line as is*/
        // <-- C2SC
        if (c2sc_process_mode == 1)
          fprintf( fp_out, "$(sc::c-exp \"%s\")\n", infile->buffer ); 
        // C2SC -->
        skip_nl();
        unget();
        return;
    }
#endif

#if MODE == PRE_STANDARD

    if (str_eq( identifier, "assert")) {    /* #assert              */
        if (! compiling)                    /* Only validity check  */
            return;
        if (eval() == 0L) {                 /* Assert expression    */
            cerror( "Preprocessing assertion failed"        /* _E_  */
                    , NULLST, 0L, NULLST);
            skip_nl();
            unget();
        }
        return;
    } else if (str_eq( identifier, "put_defines")) {
        if (! compiling)                    /* Only validity check  */
            return;
#if ! OK_IF_JUNK
        if (! is_junk())                    /* Warn at junk         */
#endif
            dump_def( dDflag);              /* #put_defines         */
        skip_nl();
        unget();
        return;
    } else if (str_eq( identifier, "preprocess")) {
        if (! compiling)                    /* Only validity check  */
            return;
#if ! OK_IF_JUNK
        if (! is_junk())                    /* #preprocess          */
#endif
        /* Just putout the directive for the succeding preprocessor */
	  //fputs( "#preprocessed\n", fp_out);
	  fputs( "$(sc::c-exp \"#preprocessed\")\n", fp_out );
        skip_nl();
        unget();
        return;
    } else if (str_eq( identifier, "preprocessed")) {
        if (! compiling)                    /* Only validity check  */
            return;
#if ! OK_IF_JUNK
        if (! is_junk())                    /* #preprocessed        */
#endif
        {
            skip_nl();
            do_preprocessed();              /* #preprocessed        */
            return;
        }
        skip_nl();
        unget();
        return;
    }

#if DEBUG || DEBUG_EVAL
    if (str_eq( identifier, "debug")) {     /* #debug <args>        */
        if (! compiling)                    /* Only validity check  */
            return;
        do_debug( TRUE);
        return;
    } else if (str_eq( identifier, "end_debug")) {
        if (! compiling)
            return;
        do_debug( FALSE);                   /* #end_debug <args>    */
        return;
    }
#endif

    if (str_eq( identifier, "asm")) {       /* #asm                 */
        do_asm( TRUE);
        return;
    }
    if (str_eq( identifier, "endasm")) {    /* #endasm              */
        do_asm( FALSE);
        skip_nl();                          /* Skip comments, etc.  */
        unget();
        return;
    }

#endif  /* MODE = PRE_STANDARD */

    if (compiling) {
#if MODE != POST_STANDARD && TOP_SPACE
        if (lang_asm) {                     /* "Assembler" source   */
            if (warn_level & 1)
                cwarn( unknown, identifier, 0L, NULLST);
	    //fputs( infile->buffer, fp_out);   /* Putout the line  */
            // <-- C2SC
            if (c2sc_process_mode == 1)
              fprintf( fp_out, "$(sc::c-exp \"%s\")\n", infile->buffer ); 
            // C2SC -->
        } else {
            cerror( unknown, identifier, 0L, NULLST);
        }
#else
        cerror( unknown, identifier, 0L, NULLST);
#endif
    } else if (warn_level & 8) {
        cwarn( unknown, identifier, 0L, " (in skipped block)");
    }
    skip_nl();
    unget();
    return;
}

static void
#if PROTO
do_preprocessed( void)
#else
do_preprocessed()
#endif
/*
 * The source file has been already preprocessed.
 * Copy the lines to output.
 * Install macros according the #define directives.
 */
{
    const char *    corrupted =
            "This preprocessed file is corrupted";          /* _F_  */
    FILEINFO *      file;
    char *          lbuf;
    char *          cp;
#if DEBUG
    const char **   incptr;
    char *          comment;
    char *          colon;
    const char *    dir;
#endif
#if STD_LINE_PREFIX == FALSE
    char            conv[ NBUFF];
    char *          arg;

    /*
     * Compiler cannot accept C source style #line.
     * Convert to the compiler-specific format.
     */
    strcpy( conv, LINE_PREFIX);
    arg = conv + strlen( conv);
#endif
    file = infile;
    lbuf = file->bptr = file->buffer;           /* Reset file->bptr */

    /* Copy the input to output until a comment line appears.       */
    while (fgets( lbuf, NBUFF, file->fp) != NULL
            && memcmp( lbuf, "/*", 2) != 0) {
#if STD_LINE_PREFIX == FALSE
        if (memcmp( lbuf, "#line ", 6) == 0) {
	  strcpy( arg, lbuf + 6);
	  //fputs( conv, fp_out);
          // <-- C2SC
          if (c2sc_process_mode == 1)
            fprintf( fp_out, "$(sc::c-exp \"%s\")\n", conv );
          // C2SC -->
        } else
#endif
        {
	  //fputs( lbuf, fp_out);
          // <-- C2SC
          if (c2sc_process_mode == 1)
            fprintf( fp_out, "$(sc::c-exp \"%s\")\n", lbuf );
          // C2SC -->
        }
    }
    if (! str_eq( lbuf, "/* Currently defined macros. */\n"))
        cfatal( "This is not a preprocessed source"         /* _F_  */
                , NULLST, 0L, NULLST);

    /* Define macros according to the #define lines.    */
    while (fgets( lbuf, NWORK, file->fp) != NULL) {
        if (memcmp( lbuf, "/*", 2) == 0) {
                                    /* Standard predefined macro    */
            continue;
        } else {
            if (memcmp( lbuf, "#define ", 8) != 0)
                 cfatal( corrupted, NULLST, 0L, NULLST);
#if DEBUG   /* Filename and line-number information in comment as:  */
            /* dir/fname:1234\t*/
            cp = lbuf + strlen( lbuf);
            if ((memcmp( cp - 4, "\t*/\n", 4) != 0)
                    || (*(cp - 4) = EOS
                            , (comment = strrchr( lbuf, '*')) == NULL)
                    || (memcmp( --comment, "/* ", 3) != 0)
                    || ((colon = strrchr( comment, ':')) == NULL))
                cfatal( corrupted, NULLST, 0L, NULLST);
            line = atol( colon + 1);        /* Pseudo line number   */
            *colon = EOS;
            dir = comment + 3;
            inc_dirp = &null;
            /* Search the include directory list    */
            for (incptr = incdir ; incptr < incend; incptr++) {
                if (memcmp( *incptr, dir, strlen( *incptr)) == 0) {
                    inc_dirp = incptr;
                    break;
                }
            }
            /* Register the filename to fnamelist[] */
            cur_fname = set_fname( dir + strlen( *inc_dirp));
            strcpy( comment - 2, "\n");     /* Remove the comment   */
#endif
            unget_string( lbuf + 8, NULLST);
            do_define();
            get();      /* '\n' */
            get();      /* Clear the "file" */
            unget();    /* infile == file   */
        }
    }
    file->bptr = file->buffer + strlen( file->buffer);
}

#if DEBUG || DEBUG_EVAL

static int
#if PROTO
do_debug( int set)
#else
do_debug( set)
    int     set;                        /* TRUE to set debugging    */
#endif
/*
 * #pragma __debug_cpp, #pragma __end_debug_cpp, #debug, #end_debug
 * Return TRUE when diagnostic is issued else return FALSE.
 */
{
    struct Debug_arg {
        const char *    arg_name;               /* Name of option   */
        int     arg_num;                        /* Value of 'debug' */
    };
    static struct Debug_arg     debug_args[] = {
#if DEBUG
#if MODE >= STANDARD
        { "__path",     PATH    },
        { "__token",    TOKEN   },
        { "__expand",   EXPAND  },
        { "__if",       IF      },
        { "__getc",     GETC    },
        { "__memory",   MEMORY  },
#else
        { "path",   PATH    },
        { "token",  TOKEN   },
        { "expand", EXPAND  },
        { "if",     IF      },
        { "getc",   GETC    },
        { "memory", MEMORY  },
#endif
#endif
#if DEBUG_EVAL
#if MODE >= STANDARD
        { "__expression",   EXPRESSION  },
#else
        { "expression", EXPRESSION  },
#endif
#endif
        { NULL,     0       },
    };
    struct Debug_arg    *argp;
    int     num;
    int     c;

    c = skip_ws();
    if (c == '\n') {
        unget();
        if (set) {
            if (warn_level & 1)
                cwarn( "No argument", NULLST, 0L, NULLST);  /* _W1_ */
            return TRUE;
        } else {
            debug = 0;                      /* Clear all the flags  */
            return FALSE;
        }
    }
    while (scan_token( c, (workp = work, &workp), work_end) == NAM) {
        argp = debug_args;
        while (argp->arg_name) {
            if (str_eq( argp->arg_name, work))
                break;
            argp++;
        }
        if (argp->arg_name == NULL) {
            if (warn_level & 1)
                cwarn( unknown_arg, work, 0L, NULLST);
            goto  diagnosed;
        } else {
            num = argp->arg_num;
            if (set) {
                debug |= num;
#if DEBUG
                if (num == PATH)
                    dump_path();
                else if (num == MEMORY)
                    print_heap();
#endif
            } else {
                debug &= ~num;
            }
        }
        c = skip_ws();
    }
    if (c != '\n') {
        if (warn_level & 1)
            cwarn( not_ident, work, 0L, NULLST);
        skip_nl();
        unget();
        goto  diagnosed;
    }
    unget();
    return FALSE;
diagnosed:
    return TRUE;
}

#endif  /* DEBUG || DEBUG_EVAL  */

#if MODE == PRE_STANDARD

void
#if PROTO
put_asm( void)
#else
put_asm()
#endif
/*
 * Put out source line as it is.
 */
{
#if 0
  // fputs( "#2\n", fp_out);
  // fputs( infile->buffer, fp_out);
  // <-- C2SC
  if (c2sc_process_mode == 1) {
    fputs( "$(sc::c-exp \"#2\")\n", fp_out );
    fprintf( fp_out, "$(sc::c-exp \"%s\")\n", infile->buffer );
  }
  // C2SC -->
  skip_nl();
#endif
}

#endif  /* MODE == PRE_STANDARD */

#if DEBUG

static void
#if PROTO
dump_path( void)
#else
dump_path()
#endif
/*
 * Show the include directories.
 */
{
    const char **   incptr;

    fputs( "Include paths are as follows --\n", fp_debug);
    for (incptr = incdir; incptr < incend; incptr++)
        fprintf( fp_debug, "    %s\n", *incptr);
}

/*
 * list_heap() is a function to print out information of heap-memory.
 * See "kmmalloc-2.5.lzh" by kmatsui.
 */
#if KMMALLOC
#ifdef  __cplusplus
    extern "C"  int     list_heap( int);
#else
#if PROTO
    int     list_heap( int);
#else
    int     list_heap();
#endif
#endif
#endif
#if BSD_MALLOC
    int     list_heap( char *);
#else
#if DB_MALLOC || DMALLOC
    int     list_heap( FILE *);
#endif
#endif

void
#if PROTO
print_heap( void)
#else
print_heap()
#endif
{
#if KMMALLOC
    list_heap( 1);
#else
#if BSD_MALLOC
    list_heap( ":cpp");
#else
#if DB_MALLOC || DMALLOC || PHK_MALLOC || DLMALLOC
    list_heap( fp_debug);
#endif
#endif
#endif
}

#endif  /* DEBUG    */

void
#if PROTO
at_end( void)
#else
at_end()
#endif
/*
 * Handle the commands to be executed at the end of processing.
 */
{
#if COMPILER == GNUC
    if (dMflag || dDflag) {
        dump_def( dDflag);
    }
#endif

#if DEBUG
    if (debug & MEMORY)
        print_heap();
#endif
}

