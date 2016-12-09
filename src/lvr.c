/****************************************************************************
 * defs.h
 * Author Phil Andrews
 * Copyright 1989, Pittsburgh Supercomputing Center, Carnegie Mellon University
 *
 * Permission use, copy, and modify this software and its documentation
 * without fee for personal use or use within your organization is hereby
 * granted, provided that the above copyright notice is preserved in all
 * copies and that that copyright and this permission notice appear in
 * supporting documentation.  Permission to redistribute this software to
 * other organizations or individuals is not granted;  that must be
 * negotiated with the PSC.  Neither the PSC nor Carnegie Mellon
 * University make any representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *****************************************************************************/
/* this file contains the structure definitions necessary for a device 
   driver to successfully communicate with GPLOT */
/* the metafile descriptor elements */
/* sub-definitions */
#include <sys/time.h>

enum 	vdc_enum 	{vdc_int, 	vdc_real};
struct 	r_struct 	{int fixed; 	int exp; 	int fract;};
struct 	c_v_struct 	{int min[3]; 	int max[3];};
/* the structure */
struct mf_d_struct {
	enum vdc_enum 		vdc_type;	/* vdc type 		*/
	int			int_prec;	/* integer precision 	*/
	struct r_struct		real_prec;	/* real precision	*/
	int			ind_prec;	/* index precision	*/
	int			col_prec;	/* colour precision	*/
	int			col_i_prec;	/* colour index prec.	*/
	int			max_c_index;	/* max. colour index 	*/
	struct c_v_struct	c_v_extent;	/* colour value extent	*/
	int			char_c_an;	/* char code announcer	*/
};
/* the picture descriptor elements (class 2) */
/* sub-defs */
struct 	scale_struct 	{int s_mode; 	float m_scaling;};
enum	cs_enum		{i_c_mode,	d_c_mode};
enum 	spec_enum 	{absolute, 	scaled}; 
struct 	vdc_struct 	{float r[4]; 	int i[4];};
struct 	rgb_struct 	{float red; 	float green; 	float blue;};
/* the actual structure */
struct pic_d_struct {
	struct scale_struct	scale_mode;	/* scaling mode		*/
	enum cs_enum		c_s_mode;	/* colour selection mode*/
	enum spec_enum		l_w_s_mode;	/* line width spec mode	*/
	enum spec_enum		m_s_s_mode;	/* marker size spec mode*/
	enum spec_enum		e_w_s_mode;	/* edge width spec mode	*/
	struct vdc_struct	vdc_extent;	/* the vdc extent	*/
	struct rgb_struct	back_col;	/* background colour	*/
};
/* the control elements (class 3) */
/* sub-defs */
struct 	rgbi_struct 	{float red; 	float green; 	float blue;
			int ind;};
enum 	boolean 	{off,		on};
/* the actual structure */
struct control_struct {
	int 			vdc_i_prec;	/* vdc integer precision*/
	struct r_struct		vdc_r_prec;	/* vdc real precision	*/
	struct rgbi_struct	aux_col;	/* auxiliary colour	*/
	enum boolean		transparency;	/* background trans.	*/
	struct vdc_struct	clip_rect;	/* clipping rectangle	*/
	enum boolean		clip_ind;	/* clipping indicator	*/
};
/* now the big one; the attribute elements (class 5) */
/* sub-defs */
enum 	line_enum 	{solid_l = 1, dash, dot_l, dash_dot, dash_d_d};
struct  i_r_struct 	{int i;	float r;};
enum 	txt_enum	{string, character, stroke};
struct 	orient_struct 	{int x_up; int y_up; int x_base; int y_base;};
enum	path_enum	{right, left, up, down};
enum hor_align { normal_h, left_h, center_h, right_h, cont_h };
enum ver_align { normal_v, top_v, cap_v, half_v, base_v, bottom_v, cont_v };
struct 	align_struct 	{enum hor_align hor; enum ver_align ver; 
			float cont_hor; float cont_ver;};
enum 	is_enum		{hollow, solid_i, pattern, hatch, empty};
struct	pt_struct	{int i[2]; float r[2];};
/* the actual structure */
struct attrib_struct {
	int			l_b_index;
	enum line_enum		line_type;
	struct i_r_struct	line_width;
	struct rgbi_struct	line_colour;
	int 			mk_b_index;	/* marker bundle index	*/
	int			mk_type;	/* marker type 		*/
	struct i_r_struct	mk_size;	/* marker size		*/
	struct rgbi_struct	mk_colour;	/* marker colour 	*/
	int 			t_b_index;	/* text bundle index	*/
	int 			t_f_index;	/* text font index	*/
	enum txt_enum		t_prec;		/* text precision	*/
	float			c_exp_fac;	/* character exp. factor*/
	float			c_space;	/* additional spacing	*/
	struct rgbi_struct	text_colour;	
	int			c_height;	/* character height	*/
	struct	orient_struct	c_orient;	/* character orientation*/
	enum path_enum		text_path;
	struct align_struct	text_align;	/* text alignment	*/
	int 			c_set_index;	/* character set index	*/
	int 			a_c_set_index;	/* alt char set index	*/
	int			f_b_index;	/* fill bundle index	*/
	enum is_enum		int_style;	/* interior style	*/
	struct rgbi_struct	fill_colour;	/* for polygons		*/
	int			hatch_index;	
	int			pat_index;	/* pattern index	*/
	int			e_b_index;	/* edge bundle index	*/
	enum line_enum		edge_type;	/* for polygons		*/
	struct i_r_struct	edge_width;	/* edge width		*/
	struct rgbi_struct	edge_colour;	/* edge colour		*/
	enum boolean		edge_vis;	/* edge visibility	*/
	struct pt_struct	fill_ref;	/* fill reference pt	*/
	struct vdc_struct	pat_size;	/* pattern size		*/
	float			*ctab;		/* colour tables	*/
};
/* do the pattern, bundle tables after we've thought about them */

/* now the command line options, use an array of structures */
#define flags_l 20
#define max_str 128
enum optval_enum {onoff, real, integer, str, lst};
struct one_opt {
	char 		flag_str[flags_l];	/* flag string cmd line	*/
	char		flag_char;		/* flag char cmd line	*/
	int		set;			/* was it explicitly set*/
	enum optval_enum 	tag;		/* tag for type 	*/
	union {
		int	i;
		float	r;
		char	str[max_str];
	} val;					/* actual value		*/
};
/* we will use an array of these structure to hold the options */
enum opt_enum {copies, debug, degrees, device, diaquest, lvr,user,frames, 
index_file, included, list, title_string, nindex,
pages, pxl_in, ypxl_in, screen, start, tty, x_offset, x_size, y_offset, 
y_size, text_mag, font_type, scale_output, clear_text, in_name, out_name};
#define opt_size ((int)out_name + 1)


/* now our enumerated classes for the CGM elements */
enum cgmcls0 /* class 0, the delimiter elements */
{No_Op = 0, B_Mf, E_Mf, B_Pic, B_Pic_Body, E_Pic};
#define Delim_Size ((int) E_Pic + 1)

enum cgmcls1 /* class 1, the metafile descriptor elements */
{MfVersion = 1, MfDescrip, vdcType, IntPrec, RealPrec, IndexPrec, ColPrec, 
CIndPrec, MaxCInd, CVExtent, MfElList, MfDefRep, FontList, CharList, 
CharAnnounce};
#define MfDesc_Size ((int) CharAnnounce + 1)

enum cgmcls2 /* class 2, the page descriptor elements */
{ScalMode = 1, ColSelMode, LWidSpecMode, MarkSizSpecMode, EdWidSpecMode, 
vdcExtent, BackCol};
#define PDesc_Size ((int) BackCol + 1)

enum cgmcls3 /* class 3, the control elements */
{vdcIntPrec = 1, vdcRPrec, AuxCol, Transp, ClipRect, ClipIndic};
#define Control_Size ((int) ClipIndic + 1)

enum cgmcls4 /* class 4, the graphical primitives */
{PolyLine = 1, Dis_Poly, PolyMarker, Text, Rex_Text, App_Text, Polygon, 
Poly_Set, Cell_Array, Gen_D_Prim, Rectangle, Cgm_Circle, Circ_3, Circ_3_Close,
Circ_Centre, Circ_C_Close, Ellipse, Ellip_Arc, El_Arc_Close};
#define GPrim_Size ((int) El_Arc_Close + 1)

enum cgmcls5	/* class 5 the attribute elements */
{ LBIndex = 1,  LType,  LWidth,  LColour,  MBIndex,  MType,  MSize,  MColour,
TBIndex, TFIndex, TPrec, CExpFac,  CSpace, TColour, CHeight,  COrient,  
TPath, TAlign, CSetIndex, AltCSetIndex, FillBIndex, IntStyle, FillColour,
HatchIndex,  PatIndex, EdBIndex, EType, EdWidth, EdColour, EdVis, FillRef,
PatTab,  PatSize, ColTab,  AspsFlags};
#define Att_Size ((int) AspsFlags + 1)

enum cgmcls6 	/* the escape element */
{Escape = 1};
#define Esc_Size ((int) Escape + 1)

enum cgmcls7 	/* the external elements */
{Message = 1, Ap_Data};
#define Ext_Size ((int) Ap_Data + 1)

/* we will use these both in the main CGM program and to create arrays 
   of device-specific functions for the main program to call */


/* now define the device info structure and relevant flags */
/* declare the structure which holds the info on PDL cpabilities */
#define port_land 	1	/* portrait/landscape capability */
#define arb_rot 	(1<<1)	/* arbitrary rotations available */
#define arb_trans 	(1<<2)	/* can translate the origin */
#define no_cr	 	(1<<3)	/* needs no carriage return at end of line */
#define need_fix 	(1<<4)	/* needs a fixed_length output record */
#define v_center 	(1<<5)	/* can vertically position text */
#define h_center 	(1<<6)	/* can horizontally position text */
#define pad_rows 	(1<<7)	/* need to pad rows in font desriptions */
#define brk_ok	 	(1<<8)	/* O.K. to break the records (e.g., cr) */
#define stroke_text	(1<<9)	/* Driver can handle stroke prec. text */
#define char_text	(1<<10)	/* Driver can handle character prec. text */
#define string_text	(1<<11)	/* Driver can handle string prec. text */
#define no_def_calls	(1<<12)	/* no calls to driver during mf def. rep. */
#define no_colour	(1<<13)	/* cannot handle colour well */
#define rot1_font	(1<<14)	/* must rotate fonts in default mode */
#define rot2_font	(1<<15)	/* must rotate fonts in rotated mode */
#define can_clip	(1<<16)	/* device can handle clipping */

/* the device info structure */
struct  info_struct {
	float pxl_in;		/* resolution in pixels per inch */
	float ypxl_in;		/* y resolution in pixels per inch */
	float x_size;		/* page size (x, y) */
	float y_size;		
	float x_offset;		/* page origin offset (x, y) */
	float y_offset;		
	int c_height;		/* character height in pixels */
	int c_width;		/* character width in pixels */
	int d_l_width;		/* default line width in pixels */
	int d_e_width;		/* default edge width in pixels */
	int d_m_size;		/* default marker size in pixels */
	char out_name[max_str];	/* default for output file name */
	long int capability;		/* what can it do ?	*/
	int rec_size;		/* size of output record/line size */
};
/* now a TeX processing specific enumerated type */
enum 
{C_Fonts, S_Font, S_Char, S_Abs_H, S_Abs_V, S_Abs_Both, 
 M_Rel_H, S_Rule, St_Font, M_Dev_Char, E_Font, W_Char, 
 Suspend, Restart};
#define Gtex_Size ((int)Restart + 1)

/* and a font info structure */
#define max_chars 128	/* max no of chars in a font */
#define font_name_length 50
#define flag_no (max_chars / sizeof(int))

struct font{
int num;	/* font_number */
int chk_sum;	/* check sum */
int scale;	/* scale factor */
int design;	/* design size */
int a;		/* area length */
int l;		/* name length */
int ino;	/* internal number */
int used;	/* no. of characters used */
int load_flag[flag_no];	/* character use flag */
int want_flag[flag_no];	/* wanted flag */
char name[font_name_length];	/* font name */
char local[font_name_length];	/* local font file name */
int width[max_chars];		/* character widths in sp*/
int pxl[max_chars];		/* character widths in pixels*/
char found;	/* did we actually find this font ? */
char read;	/* and did we try to read it ? */
int local_mag;	/* the mag we will use */
int want_mag;	/* the mag it asked for */
int voff[max_chars];	/* the pixel offset required v direction */
int hoff[max_chars];	/* the pixel offset required h direction */
int w_bytes[max_chars];	/* width in bytes of the raster map */
int h_bits[max_chars];	/* height in bits of the raster map */
int w_bits[max_chars];	/* width in bits of the raster map */
char loaded_once;
int no_bytes[max_chars];		/* stored size */
unsigned char *ptr[max_chars];	/* pointer to stored vectors */
unsigned char *exp[max_chars];	/* pointer to stored expansion */
int exp_size[max_chars];	/* size of info in .exp */
int exp_prec[max_chars];	/* precision of expansion */
int exp_index[max_chars];	/* the colour index stored */
};
/* and a structure for caching the font information */
#define key_str_l 80	/* for character I.D. */
#define cmsize 30000
struct record_type {
	char	c_id[key_str_l];
	int	date, tfm, pxl, bytes;
	char	info[cmsize];
	};
/* now common address structures for indexing */
#define INDEX_FLAG -64
/* basic address structure */
struct ad_struct {
	long int	b_ad;		/* byte address */
	int		r_ad;		/* record address */
	int 		offset;		/* offset in the record */
	};
/* the page info structure */
struct p_struct {
	struct p_struct	*next;
	struct ad_struct ad;		/* address structure */
	char		*str;		/* string associated */
	int		len;		/* string length */
	int		no;		/* page number in the file */
	};
/* flags for text emulation */
#define CA_EMULATION 2	/* emulation by cell arrays */
#define PL_EMULATION 4	/* emulation by polylines */
/* marker array */
static char c_mark[] = {' ', '.', '+', '*', 'o', 'x'};

/* common colour macros */
/* to handle direct colour on an indexed device we use 3 "pivot" indices */
/* which are specific to the colour table on each device */
/* the routine to create these is in emul.c */


/* the macro mapping from real r, g, b to index, aim for speed */
#define int_rgb(r, g, b, inds) ((int) ((r) * (inds[0] - 1) + 0.49) + \
((int) ((g) * (inds[1] - 1) + 0.49)) * inds[0] + \
((int) ((b) * (inds[2] - 1) + 0.49)) * inds[0] * inds[1]);

/* macro to get from integer cgm input to a real direct colour value */
/* i is the rgb index (0, 1 or 2), and ival the integer value */
#define dcind(i, ival, ext) ((float)(ival)-ext.min[i])/(ext.max[i]-ext.min[i])

/* macro to get single index from i, j, k 3-tuple */
#define ind_ijk(inds, i, j, k) (inds[0] * inds[1] * i + inds[0] * j + k)



/****************************************************************************
 * lvr_defines.h
 * Author Grace Giras
 * Copyright 1991, Pittsburgh Supercomputing Center, Carnegie Mellon University
 *
 * Permission to use, copy, and modify this software and its documentation
 * without fee for personal use or use within your organization is hereby
 * granted, provided that the above copyright notice is preserved in all
 * copies and that that copyright and this permission notice appear in
 * supporting documentation.  Permission to redistribute this software to
 * other organizations or individuals is not granted;  that must be
 * negotiated with the PSC.  Neither the PSC nor Carnegie Mellon
 * University make any representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *****************************************************************************/
/* This is included for both lvr.c, the gplot controller module and the
   modules of lvr_do utility program.
 */
#ifdef ardent
#define SYSV
#endif

#ifdef sgi
#define SYSV
#endif

#ifdef vms
#ifndef DEFAULT_PORT
#define DEFAULT_PORT "TTA1:"       /* for laser video */
#endif
#define SYNCHRONIZED 0             /* frame accurate tapeoff */
#define REAL_TIME 1		   /* continuous tapoeoff */
#define GMT_ADJUST 14400	   /* 4 hours in seconds to adjust seconds 
                                      since start 1970 VMS to seconds since
                                      GMT start 1970 */
#ifndef DIAQUEST_PORT
#define DIAQUEST_PORT "TTA0:"	   /* for diaquest used in tapeoff */
#endif
#define START_DIAQUEST 360	   /* New Tape start frame - 12 sec in */
#ifndef TAPEOFF_AUDIT_FILE
#define TAPEOFF_AUDIT_FILE    "lvr_tapeoff.dat"  /* lvr_do tapeoff audit */
#endif
#else  		/* not vms */
#ifdef ultrix   /* decstation 5000*/
#ifndef DEFAULT_PORT
#define DEFAULT_PORT "/dev/tty01"  /* second  serial port */
#endif
#endif

#ifdef sgi	/* silicon graphics?? */
#ifndef DEFAULT_PORT
#define DEFAULT_PORT "/dev/ttyd2"  /* second serial port */
#endif
#endif

#ifndef DEFAULT_PORT
#define DEFAULT_PORT "/dev/tty01"
#endif

#endif          /* if vms; else */

#ifndef AUDIT_FILE
#define AUDIT_FILE "/STUPID/usr/u1/mrvideo/lvr/lvr_gplot.dat"	/* for gplot recording audit */
#endif

enum bool_enum  { NO, YES };
typedef unsigned char byte;
typedef enum bool_enum bool;

#define MAX_STRING 256

#define END_VIDEO_FRAME   43500
#define START_DATA_FRAME  90001
#define END_DATA_FRAME    91024
#define USER_DATA_LEN     64

#define FAILED   NO
#define BOO      NO
#define OK       YES

#define FALSE    NO
#define TRUE     YES

#define NOT_DONE NO
#define DONE     YES

#define OFF      NO
#define ON       YES


#define UNTIL_COMPLETE 0
#define END_INPUT   '!'
#define ABORT_INPUT '#'

/* *** return codes *** */
#define COMPLETION                0X01      /* operation completed */
#define ERROR			  0X02      /* Error condition raised */
#define LID_OPEN                  0x03      /* detection of disk ejection */
#define NOT_TARGET                0X05      /* target frame not found -
					       disk dirty or flawed */
#define NO_FRAME_NUMBER           0x06      /* illegal frame */
#define MARK_RETURN               0x07      /* detection of mark set locatn */
#define ACK                       0X0A      /* effective command received */
#define NAK                       0X0B      /* command execution impossible */

#define TIMED_OUT 	          0         /* Timeout waiting for complete */
#define USER_INTERRUPT            0XFF      /* User interrupted during wait
					       for completion from device */
/* Miscellaneous  Recording Codes */
#define FRAME_REC               0XE6

/* Miscellaneous Playback Codes */
#define FWD_STEP	  	0X3D
#define REPEAT                  0X44
#define FWD_STEP_STILL  	0X2B
#define REV_STEP_STILL  	0X2C
#define CONTINUE                0X61
#define PLAY  			0X3A	/* in forward direction */
#define SLOW_PLAY		0X3C	/* .. */
#define FAST_PLAY		0X3B	/* .. */
#define MENU			0X42
#define STILL			0X4F
#define STOP			0X3f

/* Miscellaneous System Setting Codes */
#define CLEAR			0X56
#define EJECT  			0X2A
#define FRAME_MODE		0X55

/* Miscellaneous Status Inquiry Codes */
#define ADDR_INQ		0X60   /* returns 5 byte frame # */ 
#define CURRENT_BLANK_INQ	0XBC   /* returns 5 byte IN, 5 byte OUT  */

/* Commands which return COMPLETION */
#define SEARCH			0x43
#define REC_STANDBY     	0xe0
#define REC             	0xe9
#define EDGE_RIGHT      	0xb9
#define EDGE_LEFT       	0xba
#define BLANK_AREA_SEARCH 	0xbb
#define READ_DATA 		0xe2
#define SET_DATA 		0xe1
#define WRITE_DATA 		0xe3

#define ENTER	 		0x40

#define NO_OP                   0		/* use for timing */

struct record_data_rec
  {
  unsigned char type[4];        /* organization  */
#define USER_RECORD_DATA  0xff
#ifdef SYSV
  long timestamp;
#else
  time_t timestamp;		/* seconds since 1970 type */
#endif
  unsigned short start;         /* start record 1-43500 */
  unsigned short end;           /*  ... end record   */
  unsigned short copies;        /*  # copies playback speed divisor */
#define MAXUSER 12
  char username[MAXUSER];
#define MAXCGM 38 
  char cgm[MAXCGM];             /* filename */
  char checksum;
  };

struct disk_data_rec
  {
  unsigned char type[4];        /* organization  */
#define DISK_INIT_DATA  0xfe
#ifdef SYSV
  long timestamp;
#else
  time_t timestamp;		/* seconds since 1970 type */
#endif
  char not_used[56];
  char checksum;
  };

/****************************************************************************
 * lvr.c
 * Author Grace Giras
 * Copyright 1990, Pittsburgh Supercomputing Center, Carnegie Mellon University
 *
 * Permission to use, copy, and modify this software and its documentation
 * without fee for personal use or use within your organization is hereby
 * granted, provided that the above copyright notice is preserved in all
 * copies and that that copyright and this permission notice appear in
 * supporting documentation.  Permission to redistribute this software to
 * other organizations or individuals is not granted;  that must be
 * negotiated with the PSC.  Neither the PSC nor Carnegie Mellon
 * University make any representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *****************************************************************************/
/* all rights reserved */
/* lvr.c GPLOT laser video functions module */
/* lvr_ctrl()        - initialize lvr controller delimiter functions 
/* lvr_setup()       - B_Mf (Begin Metafile) I/O open; opts handling; lvr setup
/* lvr_noop()	     - B_Pic_Body (Begin Picture Body) no-op stub
/* lvr_record()	     - E_Pic (End Picture) record lvr framd
/* lvr_end()         - E_Mf (End Metafile) write user data; close I/O
/* current_frame()   - get current frame # - check disk inserted 
/* options_info()    - setup user data struct; dump options to audit file
/* select_blank()    - select largest blank video area or first user data rec 
/* empty_input()      - empty any garbage in lvr input buffer
/* input_bytes()      - either get bytes of input until a COMPLETEION character
/*                      of non-ACK return character OR get specified # of bytes
/* input_byte()       - input 1 byte, returning byte and putting in g_return
/* output_cmds()       - output a commands and parameters to lvr and get back 
                        ACK and any expected COMPLETION codes
/* open_audit()       - open AUDIT_FILE
/* au_dump()          - write to AUDIT_FILE and stderr
/* open_lvr_port()    - open lvr port for read/write
/* close_files()      - close open files
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef vms
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/wait.h>
#endif


int lvr_setup(), lvr_noop(), lvr_record(), lvr_end();


static bool debug_flag = OFF;

static char tmp[MAX_STRING], buf[MAX_STRING];
static struct one_opt *popt;    /* the command line options, in only */
static struct record_data_rec user_data;
static unsigned int frames_recorded = 0;
static unsigned int user_data_rec = 0;
static bool rec_standby = OFF;	



static	int (*ctrl[10])();	/* controller functions, out only */
static	struct one_opt opt[100];	/* the command line options, in only */


/***************************************************************/
/* Modified by Audris Mockus on Mon Apr 13 11:33:29 1992.*******/
/***************************************************************/
/***********************************************************************
  Following functions added:
	initLVR ();
	
	recordLVR ();
	to record a frame that is currently in the frame buffer.
	
	finishLVR();
	to record the information on the data area of the video disk about the
	sequence of recorded frames that started from the call to start_lvr.  
*************************************************************************/

initLVR (char * title)
{
	void lvr_ctrl();	
	char buf [100];
	int framecopies = 1, startframe = 1;
	opt[(int)debug].set = 1;

	opt[(int)lvr].set = 1;
	opt[(int)lvr].tag = str;
	strcpy(opt[lvr].val.str, "/dev/tty00");

	opt[(int)user].set = 1;
	strcpy(opt[(int)user].val.str, cuserid (NULL));
 	

	opt[(int)device].set=1; 
	strcpy(opt[(int)device].val.str,"Usual");

	opt[(int)in_name].set=1; 
	strcpy(opt[(int)in_name].val.str, "empty");

	opt[(int)title_string].set=1; 
	strcpy(opt[(int)title_string].val.str, title);

	
	opt[(int)copies].set=1; 
	opt[(int)copies].val.i = framecopies;

   opt[ (int)start ].set= 1;
	opt[ (int)start ].val.i= startframe;
	
	lvr_ctrl(ctrl,opt);
	lvr_setup();
}

recordLVR ()
{
	printf("Current frame=%d\n", current_frame());
	lvr_record ();
}

finishLVR()
{
	lvr_end ();	
}
/*******************************************************************/


/* ********** lvr_ctrl() ************ */
/* this is called at startup to initialize function pointers */
void lvr_ctrl(ctrl, opt)
int (*ctrl[])();	/* controller functions, out only */
struct one_opt opt[];	/* the command line options, in only */
   {
   /* the controller delimiter functions */
   ctrl[(int) B_Mf] 	      = lvr_setup;
   ctrl[(int) B_Pic_Body]     = lvr_noop;
   ctrl[(int) E_Pic]	      = lvr_record; 
   ctrl[(int) E_Mf]	      = lvr_end;

   /* store the options pointer */
   popt = opt;
   }


/* ********** lvr_setup() ************* */
/* set up for laser_video */

lvr_setup()
   {
   char dev_name[MAX_STRING];
   if ( popt[(int) debug].set) debug_flag = ON; /* debug switch */
   strcpy(dev_name, DEFAULT_PORT);
   if ( popt[(int)lvr].set ) 			/* lvr controller port */
      {
      if (strlen( popt[(int)lvr].val.str) )     /* -Vport option has port */
           strncpy(dev_name, popt[(int)lvr].val.str,MAX_STRING);
      dev_name[MAX_STRING-1] = '\0';	
      }
   setup_tty( dev_name);	      /* open lvr port for I/O */
   open_audit( AUDIT_FILE);	      /* open debug/audit file */
   empty_input();                     /* purge input port of any garbage */
   current_frame();	              /* get current frame - check disk in */
   options_info();	              /* setup user data; dump options info */

   user_data_rec = select_blank(OFF);     /* find next blank user data rec */
   disk_info();			      /* get disk init info */

   if (user_data_rec == START_DATA_FRAME){ /* 90001 for disk not user info */
      au_dump("Disk not Initialized - User Record 90001 blank",YES);
      exit(1);
	}         

   select_blank(ON);	              /* record standby mode for record
                                         on largest video blank area */
   }

/* ********** lvr_noop() ************* */
/* no-op at beginning of picture display */
lvr_noop()
   {
   }

/* *********** lvr_record() *********** */
lvr_record()
   {
   if (rec_standby == ON)
      {
      buf[0] = REC;
      if ( output_cmds(buf,1) != COMPLETION)
	 {
	 au_dump("Frame Not Recorded !!",YES);
         lvr_end();
	 }
      else frames_recorded++;
      }
   }

/* ********** lvr_end() ************ */
/* this is called at end of Metafile */
lvr_end()
   {
   char *pt;
   char set_data   = SET_DATA;
   char write_data = WRITE_DATA;
   char sum = 0;
   int i;

   if (rec_standby == OFF) return;  /* lvr_end already turned off rec standby */

   /* else */
   rec_standby = OFF;
   /* au_dump("Record Standby Mode Off",NO); */
   buf[0] = CLEAR;
   output_cmds(buf,1);

   sprintf(tmp,"Total frames recorded = %d", frames_recorded);
   au_dump(tmp,YES);

   if (frames_recorded) 
      {   
      user_data.end = (unsigned short)current_frame();

      /* get to user data area */
      sprintf(buf,"%c%d%c", SEARCH, user_data_rec, ENTER);
      output_cmds(buf,strlen(buf));

      sprintf(tmp,"End Frame = %d\n", user_data.end);
      au_dump(tmp,YES);

#ifdef vms
      user_data.timestamp  += GMT_ADJUST;	/* add 4 hours to adjust to
                                                  seconds GMT */
      flip_unix( &user_data);
#endif
      for( pt = (char *)&user_data, i=0; i < USER_DATA_LEN; i++) sum += pt[i];
      user_data.checksum = - sum;

      if( output_cmds(&set_data,1) != ACK) 
	 { au_dump("User Data Error: Write Set Data failed",YES); exit(1); }

      for (i =0, pt = (char *)&user_data; i < USER_DATA_LEN + 1; i++)
	      output_byte( (int)*pt++);  /* no ACK's */

      if (input_byte() != COMPLETION)
	 { au_dump("User Data Error: Write of Data failed",YES); exit(1); }

      if (  output_cmds(&write_data,1) != COMPLETION) 
	 { au_dump("User Data Error: Write failed??",YES); exit(1); }
      }
/* go back to the last recorded frame
	Audris Mockus, Mon Aug 17 09:46:26 1992
*/
	{
  		char com [100];
		sprintf(com,"%c%d%c",SEARCH, user_data. end, ENTER);
   	if(output_cmds(com,strlen(com))!=COMPLETION){
	  		au_dump("Search of frame failed\n", YES);
	  		exit (1);
   	}
	}
/* eject the disk 
	Audris Mockus, Fri Jan 29 11:33:34 1993
*/
/*	{
  		char com [100];
		sprintf(com,"%c",EJECT);
   	if(output_cmds(com,1) != ACK){
	  		au_dump("EJECT failed\n", YES);
	  		exit (1);
   	}
	}
*/	
   close_io();
   close_audit();
   }
    

/* *** current_frame() *** */
/* get current frame #  */
current_frame()
   {
   int frame;
   int return_code;
   buf[0] = ADDR_INQ;
   if ((return_code=output_cmds(buf,1)) == NAK) 
      {
      au_dump("No Disk is Inserted or Not in Remote Control Mode!!", YES);
      exit(1);
      }

   input_bytes(buf,5);
   buf[5] = 0;
   sscanf(buf,"%d", &frame);
   return(frame);
   }


/* **** options_info() **** */
options_info()
   {
   register i;
   /* do initial setup of user data record */
   user_data.type[0]   = USER_RECORD_DATA;
   user_data.timestamp  = time(0);
   if ( popt[(int)user].set )
      {   
      strncpy(user_data.username, popt[(int)user].val.str,MAXUSER);
      for (i = 0 ; i < MAXUSER; i++)
	 user_data.username[i] = tolower(user_data.username[i]);
      }
   if ( popt[(int)copies].set )
	 user_data.copies = popt[(int)copies].val.i;
   else  user_data.copies = 1;    /* playback at normal speed */

   /* dump all options stuff to audit/debug file */
   au_dump("\n*********",NO);

   sprintf(tmp,"date     = %s   (%d)",
           (char *)ctime(&user_data.timestamp), user_data.timestamp);
   au_dump(tmp,NO);

   sprintf(tmp,"user     = %.*s",MAXUSER,user_data.username);
   au_dump(tmp,NO);

   sprintf(tmp,"copies   = %d",user_data.copies);
    au_dump(tmp,NO);

   if ( popt[(int)device].set )
      {
      sprintf(tmp,"device   = %s",popt[(int)device].val.str); 
      au_dump(tmp,NO);
      }

   if ( popt[(int)in_name].set )
      {
      strncpy(user_data.cgm, popt[(int)in_name].val.str,MAXCGM);
      for (i = 0 ; i < MAXCGM; i++)
	 user_data.cgm[i] = tolower(user_data.cgm[i]);
      sprintf(tmp,"cgm      = %.*s",MAXCGM, user_data.cgm);
      au_dump(tmp,NO);
      }

   if ( popt[(int)title_string].set )
      {
      sprintf(tmp,"title    = %s",popt[(int)title_string].val.str);
      au_dump(tmp,NO);
      }
   /* etc !!!!!!!!!!!!!!!!!!!! */
   }


static FILE *lvr_info;

/* *** open_audit() **** */
/* Open a file for debug/ record progress */
open_audit(fil) char *fil;
{
#ifdef vms
   lvr_info = fopen( fil,"a","rfm=var", "rat=cr");
#else
   lvr_info = fopen( fil ,"a");
#endif
}
/* ********* close_audit() ********** */
close_audit()
{ 
fclose( lvr_info);
}
/* ***** au_dump() *** */
au_dump(message, output_flag)
char *message;
bool output_flag;	/* if YES output message to stderr regardless of debug_flag */
   {
   if (debug_flag == ON || output_flag == YES ) fprintf(stderr,"%s\n", message);
   if (!lvr_info) return;
   fprintf(lvr_info,"%s\n", message);
   fflush(lvr_info);
   }




/* **** select_blank() *** */
select_blank(video_flag)
bool video_flag;	/* On if for Video Area and Off if for Data Area */
   {

   int first_in, first_out;
   int biggest_area;
   int in, out = 0;

   /* Do minimal Setup and get to center */

   sprintf(buf,"%c%c%c%s%c", CLEAR, FRAME_MODE,SEARCH,
           video_flag == ON ? "1" : "90001" ,ENTER);
   output_cmds(buf,strlen(buf));

   buf[0] = CURRENT_BLANK_INQ;
   if ( output_cmds(buf,1) == NAK)   /* Need to do Blank Area Search */
      {
      if ( video_flag)
         au_dump("Doing Video Blank Area Search - Takes 60 seconds", YES);
      buf[0] = BLANK_AREA_SEARCH;
      if ( output_cmds(buf,1) == COMPLETION)
	 au_dump("Blank Area Search Complete",YES);
      else { au_dump("Blank Area Search failed",YES); exit(1); }
      /* trying again */
      buf[0] = CURRENT_BLANK_INQ;
      output_cmds(buf,1);
      }

   input_bytes(buf,10);
   buf[10] = 0;
   sscanf(buf,"%5d%5d", &first_in, &first_out);
   biggest_area = first_out - first_in;
   sprintf(tmp,"%s Blank Area : %5d - %5d",
              video_flag == ON ? "Video" : "User Data", first_in, first_out);
   au_dump(tmp,NO);

   if (video_flag == OFF)	/* get to first blank user data rec */
      {
      sprintf(tmp,"user data frame          =   %d", first_in);
      au_dump(tmp,YES);
      return(first_in);
      }

   /* video  */
   sprintf(buf,"%c%c", FRAME_REC,REC_STANDBY);
   if (output_cmds(buf,strlen(buf)) != COMPLETION)
     { au_dump("Record Setup Failure",YES); exit(1); }

   /* au_dump("Record Standby Active",NO); */

   for (;;)
      {
      buf[0] = EDGE_RIGHT;
      output_cmds(buf,1);

      buf[0] = CURRENT_BLANK_INQ;
      output_cmds(buf,1);

      input_bytes(buf,10);
      buf[10] = 0;
      sscanf(buf,"%5d%5d", &in, &out);
      if (in == first_in && out == first_out) break;
      else
	 {
	 if ( (out-in) > biggest_area) biggest_area = out -in;
	 sprintf(tmp,"Video Blank Area : %5d - %5d", in, out);
         au_dump(tmp,NO);
	 }   
      }

   while( ( out -in) != biggest_area)   
      {
      buf[0] = EDGE_RIGHT;
      output_cmds(buf,1);
      buf[0] = CURRENT_BLANK_INQ;
      output_cmds(buf,1);
      input_bytes(buf,10);
      buf[10] = 0;
      sscanf(buf,"%5d%5d", &in, &out);
      }
   user_data.start = in;
   user_data.end   = out;		/* for now  */
   sprintf(tmp,"start video record frame =   %d ( %d -%d blank )", in, in, out);
   au_dump(tmp,YES);

 
   sprintf(buf,"%c%c%c", ENTER, ENTER, ENTER);  /* use current blank area */
   output_cmds(buf,strlen(buf));
   if (input_byte() != COMPLETION)
      {
      au_dump("Record Standby IN, OUT Bad",YES);
      exit(1);
      }

   rec_standby = ON;
   /* au_dump("Ready to Record",NO); */
   return(in);
   }

/* ************* disk_info() ****************** */
disk_info()
   {
   static int disk_init_frame[2] = { START_DATA_FRAME, END_DATA_FRAME };
   char read_data = READ_DATA;
   struct disk_data_rec *rec; 
   int i,frame;
   /* get to frame */
  for (frame=0;frame < 2;frame++)
      {
      sprintf(buf,"%c%d%c",SEARCH, disk_init_frame[frame] ,ENTER);
      if (output_cmds(buf,strlen(buf)) != COMPLETION)
         { au_dump("Search for Disk Init Frame Failed??", YES); exit(1); }
/* My stuff For initial record */
/*		{
			char *pt;
			struct disk_data_rec buff;
			char check=0, write_data=WRITE_DATA;
			char command[100];
			buff.type[0]=DISK_INIT_DATA;
			buff.type[1]='\0';
			buff.timestamp=time(0)-842400l;
			strcpy (buff.not_used, "Disk 0, Side A");			
			for( pt = (char *)&buff, i=0; i < USER_DATA_LEN; i++)
				check += pt[i];
			buff.checksum= -check;

			command[0]=SET_DATA;
			for (i=1;i < 66; i++)
				command[i]=((char*)buf)[i-1];
			if( output_cmds(command,1) != ACK){
				au_dump("User Data Error: Write Set Data failed",YES);
				exit(1);
			}
			for (i =0, pt = (char *)&buff; i < USER_DATA_LEN + 1; i++)
	      	output_byte((int)*pt++);
			if (input_byte() != COMPLETION){
				au_dump("User Data Error: Write of Data failed",YES);
				exit(1);
			}
			
			if (  output_cmds(&write_data,1) != COMPLETION){
				au_dump("User Data Error: Write failed??",YES);
				exit(1);
			}
 		}
*/				
      if ( output_cmds(&read_data,1) != COMPLETION)
         { au_dump("Disk Initialization Frame not written", YES); exit(1); }

      for (i = 0; i < USER_DATA_LEN +1; i++) buf[i] = input_byte();

      if ( (unsigned char)buf[0] == DISK_INIT_DATA && buf[1]  == '\0')
         {
	 au_dump("Disk Initialization :",YES);
#ifdef vms
	 flip_unix( rec);
	 rec->timestamp -= GMT_ADJUST;
#endif
	 rec = (struct disk_data_rec *)buf;
	 sprintf(tmp, "disk initialization date     = %s",
                                          (char *)ctime(&rec->timestamp));
	 au_dump(tmp,YES);
	 sprintf(tmp, "disk #                       = %X", rec->timestamp);
	 au_dump(tmp,YES);
	 sprintf(tmp, "Comments:%s", rec->not_used);
	 au_dump(tmp,YES);
	 return;
         }
      }	 
   }         


#ifdef vms
/* ****************** flip_unix() ******************* */
/* flip binary info in user data to/from unix */
flip_unix(rec)
char *rec;
   {
   if ( (unsigned char)rec[0] == USER_RECORD_DATA && rec[1]  == '\0')
	 flip_record(rec );
   else if ( (unsigned char)rec[0] == DISK_INIT_DATA && rec[1]  == '\0')
	 flip_disk( rec );
   }

/* ***************** flip_disk() ************** */
flip_disk(rec)
char *rec;
{
 /* timestamp */
 char tmp;
 tmp = rec[4];
 rec[4] = rec[7];
 rec[7] = tmp;
 tmp = rec[5];
 rec[5] = rec[6];
 rec[6] = tmp;

 /* side */
 tmp = rec[8];
 rec[8] = rec[9];
 rec[9] = tmp;

}

/* *************** flip_record() ************** */
/* put user data record session bianry info into unix format */
flip_record(rec)
char *rec;
 {
 /* timestamp */
 char tmp;
 tmp = rec[4];
 rec[4] = rec[7];
 rec[7] = tmp;
 tmp = rec[5];
 rec[5] = rec[6];
 rec[6] = tmp;

 /* start */
 tmp = rec[8];
 rec[8] = rec[9];
 rec[9] = tmp;

 /* end */
 tmp = rec[10];
 rec[10] = rec[11];
 rec[11] = tmp;

 /* copies */
 tmp = rec[12];
 rec[12] = rec[13];
 rec[13] = tmp;
 }

#endif


        
/****************************************************************************
 * lvr_io.c
 * Author Grace Giras
 * Copyright 1991, Pittsburgh Supercomputing Center, Carnegie Mellon University
 *
 * Permission to use, copy, and modify this software and its documentation
 * without fee for personal use or use within your organization is hereby
 * granted, provided that the above copyright notice is preserved in all
 * copies and that that copyright and this permission notice appear in
 * supporting documentation.  Permission to redistribute this software to
 * other organizations or individuals is not granted;  that must be
 * negotiated with the PSC.  Neither the PSC nor Carnegie Mellon
 * University make any representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *****************************************************************************/
/* ** lvr_io.c ** */
/* #define DEBUG_IO      /* define to display bytes to/from lvr */
/* input_1_byte()     - input 1 byte from lvr, print hex and ascii for byte
/*                      if a return code, print return
/* input_byte()       - input 1 byte, returning byte and putting in g_return
/* empty_input()      - empty any garbage in lvr input buffer
/* input_bytes()      - either get bytes of input until a COMPLETEION character
/*                      of non-ACK return character 
/*                        OR
/*                    - get specified # of bytes
/* ouput_cmds()       - output a commands and parameters to lvr and get back 
                        ACK and any expected COMPLETION codes
/* setup_tty()        - device setup
/* more_setup_tty()   - more device setup 
 */




#ifdef vms
#define OK 1
#define if_fail_die(p1) if (((status = p1) & OK) != OK){ LIB$SIGNAL(status); \
   exit(1); }
#include descrip
#include ssdef
#include iodef
#include ttdef

   static int status;  
   static unsigned short chan;   /* channel of lvr device */
   static struct char_buf        /* original lvr device characteristics buffer */
      {
      unsigned char class;
      unsigned char type;
      unsigned short buffer_size;
      unsigned char charac[3];
      unsigned char page;
      } orig;
   static char ctr_dev[100];
   static $DESCRIPTOR(ctr_dev_desc, ctr_dev);

#else   /* Unix */
   static FILE *ifile, *ofile;
   static int ifile_d;
#endif /* vms else unix */


   static int g_cmd =0;
   static int g_enter=0;
   static int g_return_code;


/* ***  */
   struct code_struct
      {
      int code;
      char *mesg;
      };

   static struct code_struct returns[] =
       { { ACK,"ACK"},
	 { COMPLETION,"COMPLETION"},
	 { ERROR,"ERROR"},
	 { LID_OPEN,"LID_OPEN"},
	 { MARK_RETURN, "MARK RETURN"},
	 { NAK, "NAK"},
	 { NO_FRAME_NUMBER, "NO FRAME NUMBER"},
	 { NOT_TARGET,"NOT TARGET"},
	 { USER_INTERRUPT,"USER INTERRUPT"}  };     

   static num_returns = sizeof(returns)/ sizeof(struct code_struct);

   struct code_struct_2
     {
     int code;
     int num_enter;
     char *mesg;
     };

   static struct code_struct_2 completes[] =
       { { SEARCH,1,"SEARCH"},
	 /* {	REPEAT,2,"REPEAT"},   ...or 3 sometimes */
	 { REC_STANDBY,0,"REC STANDBY"}, /* 2 completes -after cmd & after params*/
	 { REC,0,"REC"},
	 { EDGE_RIGHT,0,"EDGE RIGHT"},
	 { EDGE_LEFT,0,"EDGE LEFT"},
	 { BLANK_AREA_SEARCH,0,"BLANK AREA SEARCH"},
	 { READ_DATA,0,"READ DATA"}, 	
	 { WRITE_DATA,0,"WRITE DATA"} }; 	

   static num_completes = sizeof(completes)/ sizeof(struct code_struct_2);


/* ************** input_1_byte() **************************** */
/* get 1 byte of input */
input_1_byte()
   {
   char moo[1];    
   int i, return_code;
   return_code = input_byte();
   moo[0] = return_code;
   for (i = 0; i < num_returns; i++)
      if (return_code == returns[i].code) fprintf(stderr, "%s\n", returns[i].mesg);
   if (i == num_returns) fprintf(stderr, "%02X %.1s\n", return_code, moo);
   return(return_code);
   }

#ifdef vms
/* *************** empty_input() VMS ************************ */
/* throw out any chars in the input buffer */
empty_input() 
   {
   int temp;
   /* clear input buffer before start */
   if_fail_die(SYS$QIOW(0,chan,IO$_READVBLK | IO$M_PURGE,0,0,0,&temp, 0,0,0,0,0));
   }
#else

/* *************** empty_input() Unix ************************ */
empty_input()
{
   static struct timeval timeout = {0,0};   /* for a select poll */
   int i;
   char buf[512];
   int num = 0;
   int value;
   int readfds;
   readfds = 1 << ifile_d;
   for (;;)
      switch((value = select(32,&readfds,NULL,NULL,&timeout)))
	 {
	 case 0:
	 if (num) 
	    {
	    fprintf(stderr, "\n... garbage in input buffer = ");
	    for (i = 0; i < num; i++) fprintf(stderr, "%02X ", buf[i]);
	    fprintf(stderr, "\n");
	    }
	 return(num);
	 break;

	 case -1:
	 fprintf(stderr,"Error on select\n"); exit(1);

	 case 1:
	 buf[num++] = input_byte(); break;

	 default:
	 return(num); break;
	 }
   }
#endif	/* vms else unix */

/* ************** input_bytes() *********************** */
/* if nb = 0 get input until COMPLETION or non_ACK return character is received
/* if nb != 0 get nb bytes of input */

int input_bytes(buf,nb)
int nb; /* =0 => UNTIL COMPLETE */
char *buf;
   {
   int i;
   int return_code;
   char moo[1];
   /* Note: a select on getting input from the device does not work - the read 
	    must be issued causing possible hangup in program */
   if (nb != UNTIL_COMPLETE) 
      {
      buf[0] = g_return_code;   /* first byte already read and in g_return_code */
      /*    printf("   %02X     %.1s\n", g_return_code, buf);  */
      nb--;
      for ( i =0; i < nb; i++) 
	  {
	  return_code = input_byte();
	  moo[0] = buf[i+1] = return_code;
	  /* printf("   %02X     %.1s\n", return_code, moo); */
	  }
      return(COMPLETION);
      }
   /* else only return codes should be received here  (UNTIL COMPLETE)*/
   for (;;)
      {
      return_code = input_byte();
      for (i = 0; i < num_returns; i++)
	 if (return_code == returns[i].code)
	    {
	    /* printf("%s\n", returns[i].mesg); */
	    if ( return_code == COMPLETION) return( COMPLETION); /* good */
	    if ( return_code != ACK) return(return_code);         /* boo */
	    }	 
      }
   }

/* ************** input_byte() **************************** */
/* get 1 byte of input */
input_byte()
   {
#ifdef vms
   static int no_term[2] = {0,0};   /* for no terminators */

   if_fail_die(SYS$QIOW(0,chan,IO$_READVBLK | IO$M_NOECHO | IO$M_NOFILTR,0,0,0,
				 &g_return_code, 1,0,no_term,0,0));
#else
   g_return_code = getc(ifile);
#endif
#ifdef DEBUG_IO
    fprintf(stderr,"         <== %X\n", g_return_code);
#endif
   return(g_return_code);
   }

   /* *********************** output_byte() ************************* */
   output_byte(outb) int outb;
      {
      int cmd;
      cmd = outb;
#ifdef vms
      if_fail_die(SYS$QIOW(0,chan,IO$_WRITEVBLK,0,0,0,&cmd, 1,0,0,0,0));
#else
     putc( (int)cmd, ofile);
#ifdef ultrix
     fflush(ofile);
#endif
#endif
#ifdef DEBUG_IO
    fprintf(stderr," ==> %X\n", cmd);
#endif
      }

   /* ************************* output_cmds() ************************* */
   /* output command(s) to device */
   output_cmds( cmd,num) 
   byte *cmd;
   int num;
   {
   int return_code;
   register i,j;
   char buf[256];
   return_code = ACK;
   for (i =0; i < num; i++)	/* loop outputting commands and parameters */
      {
      output_byte( (int)cmd[i]);
      for (j = 0; j < num_completes;j++) /* loop seeing if COMPLETION returned */
	 {	    
	 if (cmd[i] == completes[j].code )		/* yes */
	    {       
	    g_enter = completes[j].num_enter;
	    g_cmd = cmd[i];
	    if (g_enter) break;
	    else
	       {
	       if (( return_code = input_bytes(buf,UNTIL_COMPLETE)) == COMPLETION)
		  ; /* printf("%s COMPLETION\n", completes[j].mesg); */
	       else return( return_code);
	       }
	    break;
	    }
	 }   

      if (j == num_completes || g_enter )	/* if looking for ACK */
	 {	 
	 return_code = input_byte();
	 if(return_code  != ACK ) /* boo.. hope not an unexpected COMPLETION ?? */
	    {
	    for (j = 0; j < num_returns; j++)
	       if (return_code == returns[j].code) break;
	    if (j == num_returns);  /*   printf("X'%02X'\n", return_code); */
	    break;   /* break from Command Loop */
	    }	       
	 else		/* got ACK or COMPLETION */
	    { 
	    if (cmd[i] == ENTER && g_enter) /* waiting COMPLETION after params?*/
	       {
	       g_enter--;
	       if (!g_enter) 	 /* enough params input */
		  {
		  if (( return_code = input_bytes(buf,UNTIL_COMPLETE))!=COMPLETION)
							   break;
		  for (j = 0; j < num_completes;j++) 
		      if (g_cmd == completes[j].code )
			; /* printf("%s COMPLETION\n", completes[j].mesg);  */
		  }
	       }
	    }	    	       		  
	 }	 
      }	 /* end of Command Loop */
   return(return_code);
   }



#ifdef vms
/* ****************** setup_tty() VMS *********************** */
setup_tty(dev_name) char *dev_name;
   {
   register i;
   struct char_buf mod;

   int charac;
   static int no_term[2] = {0,0};   /* for no terminators */

   strcpy(ctr_dev, dev_name);
   ctr_dev_desc.dsc$w_length = strlen( ctr_dev);

   if_fail_die(SYS$ALLOC(&ctr_dev_desc,0,0,0,0));
   if_fail_die(SYS$ASSIGN(&ctr_dev_desc, &chan,0,0));
   if_fail_die(SYS$QIOW(0,chan,IO$_SENSEMODE,0,0,0, &orig,8,0,0,0,0));

   /* printf("%X %X\n", ((int *)&orig)[0], ((int *)&orig)[1] ); */
   mod = orig;
   mod.class = TT$_UNKNOWN;
   mod.type  = 0;   
   charac = *( (int *)mod.charac);
   /* set some characteristics */
   charac |= 
     TT$M_EIGHTBIT | TT$M_NOECHO | TT$M_PASSALL | TT$M_HALFDUP | TT$M_MODEM;
   /* reset some characteristics */
   charac &= ~(TT$M_NOTYPEAHD | TT$M_HOSTSYNC | TT$M_TTSYNC | TT$M_READSYNC ) ; 

   for (i = 0; i < 3; i++) mod.charac[i] = ((char *)&charac)[i];
   mod.page  = 0;

   /* printf("%X %X\n", ((int *)&mod)[0], ((int *)&mod)[1] ); */

   if_fail_die(SYS$QIOW(0,chan,IO$_SETMODE,0,0,0, &mod,8,TT$C_BAUD_1200,0,0,0));
   sleep(2);  /* to let set modem take effect */

   }

#else
/* ****************** setup_tty() Unix *********************** */
setup_tty(dev_name) char *dev_name;
   {
   int ofile_d;
   if (!( ofile = fopen(dev_name, "w")))
      { fprintf(stderr,"Could not open %s for write\n", dev_name); exit(1); }

   if (!( ifile = fopen( dev_name, "r")))
      { fprintf(stderr,"Could not open %s for read\n", dev_name); exit(1); }
   ofile_d = fileno(ofile);
   more_setup_tty( ofile_d);

   ifile_d = fileno(ifile);
   more_setup_tty( ifile_d);
   }


/* ***************** more_setup_tty() System V *************************** */
#ifdef SYSV
/* The test of MAKING_DEPEND will cause termio.h to get lost from Makefile
 * dependency list, but because mkdepend can't handle it we have to do so.
 */
#ifndef MAKING_DEPEND
#include <termio.h>
#endif /* MAKING_DEPEND */
   more_setup_tty(fil_d) int fil_d;
   {
   int ret_val;
   struct termio tty;

   ret_val = ioctl(fil_d, TCGETA, &tty);
   if (ret_val !=1) 
      {
      tty.c_cflag = B1200 | CLOCAL | CREAD | CS8;
      tty.c_lflag = 0;
      tty.c_iflag = IGNBRK | IGNPAR; 
      tty.c_oflag = 0; 
      tty.c_cc[VEOL] = 1;    /* TIME = 1 when ICANON not set in c_lflag */
      tty.c_cc[CEOF] = 1;    /* MIN  = 1 when ICANON not set in c_lflag */
      ret_val = ioctl(fil_d, TCSETA, &tty);
      }

#ifdef DEBUG_IO
   ret_val = ioctl(fil_d, TCGETA, &tty);
   if (ret_val !=1) 
      {
      fprintf(stderr,"c_iflag = %x\n", tty.c_iflag);
      fprintf(stderr,"c_oflag = %x\n", tty.c_oflag);
      fprintf(stderr,"c_cflag = %x\n", tty.c_cflag);
      fprintf(stderr,"c_lflag = %x\n", tty.c_lflag);
      fprintf(stderr,"c_line  = %x\n", tty.c_line);
      }
#endif
   }

#else     /* BSD */
/* ***************** more_setup_tty() BSD *************************** */
#include <sgtty.h>
   more_setup_tty(fil_d) int fil_d;
   {
   int ret_val;
   struct sgttyb tty;
   int ldisc = 0; /*OTTYDISC;*/

   ioctl(fil_d, 0/*TIOCSETD*/, &ldisc);  /* standard tty driver */

   ret_val = ioctl(fil_d, TIOCGETP, &tty);
   if (ret_val !=1) 
      {
      tty.sg_ispeed = B9600;
      tty.sg_ospeed = B9600;
      tty.sg_flags  = RAW /*| CBREAK*/;
      ret_val = ioctl(fil_d, TIOCSETP, &tty);
      }
   }

#endif   /* ifdef SYSV else */
#endif   /* ifdef vms else */

   /* ********************** close_io() *************************** */
close_io()
   {
#ifdef vms
   if_fail_die(SYS$QIOW(0,chan,IO$_SETMODE,0,0,0, &orig,8,TT$C_BAUD_1200,0,0,0));
   if_fail_die(SYS$DASSGN(chan));
   SYS$DALLOC(&ctr_dev_desc,0);		/* will fail if allocated twice */
#else
   fclose(ofile);
   fclose(ifile);
#endif
   }
   







