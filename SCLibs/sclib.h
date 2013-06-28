#pragma once
#ifndef _SCLIB_H_
#define _SCLIB_H_

#ifdef TARGET_CARLSON_GRADE
#  define NOCSCGL
#  define NOCRD
#  define NOSEWER
#  define NOTINSIMPLIFY
#  define NOTMESH
#  define NOCPUTIME
#  define NO_VILABS_CIA
#endif

// TODO: remove me!  I think i am having problems with a mixture of release and dev code,
// but for whatever reason nothing i build is registered. (PAC, 30mar2007)
#if defined(CS_DEBUG)
# define NO_REGISTRATION_CHECK
#endif

#define CHANGEKEY_MODULUS(A) (A%7919)
#ifdef UNDER_CE
	#ifndef NOCSCGL
	#define NOCSCGL
	#endif

	#ifndef NOCRD
	#define NOCRD
	#endif

	#ifndef NOSEWER
	#define NOSEWER
	#endif

	#ifndef NOTINSIMPLIFY
	#define NOTINSIMPLIFY
	#endif

	#ifndef NO_VILABS_CIA
    #define NO_VILABS_CIA
    #endif
#endif

#ifdef _WIN64
  #define NO_SURVEY_MODEL
  #define NO_GC3
  #define NO_LEICA_DBX
  #define NO_NIKON
  #define NO_DAO
#endif

// WORKAROUND: compiler warning 4503 is generated when compiler truncates symbol names.
// It is rather easy to get this warning when using nested template definitions.
// As long as all symbol names (even truncated ones) are unique, this is not a problem.
// If symbol names are no longer unique, a linker error is generated for symbol names
// in question instead. This line of code disables nasty looking warnings across all of
// our projects. It may become unnecessary with the next release of Visual Studio since
// it allows for longer symbol names. - Michael Burenkov
#pragma warning (disable : 4503)
// this is for depreciated string functions warning
#pragma warning (disable : 4996)
#pragma warning (disable : 4786)

//post-XML version
#if (_MSC_VER >= 1400)
  #ifndef _USE_32BIT_TIME_T //to avoid compiler warning when including as preprocessor directive in project
    //#define _USE_32BIT_TIME_T //modifies <time.h>
  #endif
#endif

#include "malloc.h" // making sure it was done before malloc is redefined
/*
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
#define CESCLIB
  #ifndef _JULDEF_ 
    #define _JULDEF_ "juldef.h"
    #include _JULDEF_
  #endif
#endif
*/
#ifdef UNDER_CE
  #include <windef.h>
#endif

#include "scconst.h" // constants and defines

/*
#ifdef WIN32_WINDOWS
#include <winbase.h>
#endif*/
#ifdef __BORLANDC__
#include <time.h>
#endif
#include <commctrl.h>

// for copying ads_name 'b' to ads_name 'a'.
#define COPY_ENTITY_NAME( a, b) a[0] = b[0], a[1] = b[1]

// for copying ads_name 'b' to ads_name 'a' only if 'b' is non-zero.
#define COPY_ENTITY_NAME_IF_NONZERO( a, b) \
  if (b[0] != 0 && b[1] != 0) a[0] = b[0], a[1] = b[1]

// for clearing or zeroing an entity name
#define ZERO_ENTITY_NAME( a) a[0] = 0, a[1] = 0

// in a conditional.  for example, if (IS_ZERO_ENTITY_NAME( name))
#define IS_ZERO_ENTITY_NAME( a) (a[0] == 0 && a[1] == 0)

// for comparing ads_names.  for example, if (IS_SAME_ENTITY_NAME( name1, name2))
#define IS_SAME_ENTITY_NAME( a, b) (a[0]==b[0] && a[1]==b[1])

#ifndef NOCSCGL
#define _USE_MATH_DEFINES 1
#include "CSCGL/MathDefines.h"
#endif

#if /*defined CG_DRAW_API && */defined HAVE_CAD
  #include "DrawAPI\CgPtDrwApi.h"
#endif

#include "AcadEmulator/AcadEmulator.h"

#ifdef HAVE_CAD
#include "scldd.h"
#endif

//#include "./BoxTree/QuadTree.h"
#ifndef NOCSCGL
#pragma warning(push,3)
	#include "CSCGL/CSCGL.h"
  #pragma warning(pop)
#endif

#ifndef MAX
  #define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef MIN
  #define MIN(a,b) (((a)>(b))?(b):(a))
#endif
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x500)
typedef short BOOL;  
#define MAX_PATH FILENAME_MAX
#endif
#ifdef CESCLIB
  #define real double
  #define SIZE_PL2 66
#else
  #define MEMORYLIMIT 36000
#endif
#ifdef CS_DEBUG
  #define SCMALLOC 0  //set to 1 to do some dangerous but very useful debugging
#else
  #define SCMALLOC 0
#endif

#ifdef ACRXAPP
// for all allocations that are to be deleted within SurvCADD, please use
//  the usual malloc/free and new/delete functions.
// For allocations that are intended to be deleted by AutoCADD, please use
//  ads_malloc.  For freeing memory that was allocated by AutoCADD, please use
//  ads_free.
// If you want to override the szFile and iLine arguments, use scmalloc/scfree.
#if SCMALLOC
  void* scmalloc( int iSize, const char *szFile, int iLine);
  void  scfree( void *pToFree, const char *szFile, int iLine);
  void* screalloc( void *pToRealloc, int iNewSize); 
  #define malloc( i)  scmalloc( (i), __FILE__, __LINE__)
  #define free( x)    scfree( (x), __FILE__, __LINE__)
  #define realloc( x, i) screalloc( (x), (i))
  void ScReportUndeletedObjects();
  void ScMallocChange( void *pPreviousScMalloc, const char *szFile, int iLine);
  //#undef acad_relrb                  //too many places use malloc() to allocate, but
  //#define acad_relrb( b) ((void)0)   //  then use acad_relrb() to free.
  /*
  void * __cdecl operator new( unsigned int iSize, const char *szFile, int iLine);
  void __cdecl operator delete( void *pToDelete, const char *szFile, int iLine);
  */
  #define MEM_CHECK_INC(a)  a++ //for incrementing a global or static variable when in debug mode.
  #define MEM_CHECK_DEC(a)  a-- //for decrementing a global or static variable when in debug mode.

  // Use this for when you want to change the file and line # associated with an allocation
  //  (new or malloc).  For example, make_lwpoint() is called from many places and you might
  //  need to know which caller did not free the memory.  After calling make_lwpoint(), you
  //  can SCMALLOC_CHANGE( pNewPoint) so that the file and line # of the call to make_lwpoint()
  //  is used instead.
  #define SCMALLOC_CHANGE( x) ScMallocChange( x, __FILE__, __LINE__)

#else
  #define ScReportUndeletedObjects() ((void)0)
  #define ScMallocChange( p, szFile, iLine) ((void)0)
  #define free(p) ads_free(p) 
  #define malloc(p) ads_malloc(p) 
  #define realloc(p,b) ads_realloc(p,b)
  #define scmalloc( iSize, szFile, iLine)   ads_malloc( iSize)
  #define scfree( pToFree, szFile, iLine)   ads_free( pToFree)
  #define screalloc( pToRealloc, iNewSize)  ads_realloc( pToRealloc, iNewSize)
  #define isalpha scisalpha
  #define new_lwpoint_memcheck( szFile, iLine) new_lwpoint()
  #define MEM_CHECK_INC( a)  (0)
  #define MEM_CHECK_DEC( a)  (0)
  #define SCMALLOC_CHANGE( x) (0)
#endif
#else 
  // not in ACAD
  #define MEM_CHECK_INC( a)  (0)
  #define MEM_CHECK_DEC( a)  (0)
  #ifndef ASSERT
    #define ASSERT( a)  (0)
  #endif
#endif //ACRXAPP

#ifndef NOCRD
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || defined (CE_OTHER))
  #ifndef _CRDFILEH_
    #define _CRDFILEH_ "library/crdapi/CrdFile.h"
    #include _CRDFILEH_
  #endif
#endif
#endif

#ifdef CESCLIB
  #if ((defined(WIN32_WINDOWS)) && (!defined(UNDER_CE)) && (!defined(EMULATOR))) 
    #include <time.h>
    #ifndef ACRXAPP
      #ifdef TELESCOPE
        #define COMPANY "Carlson Software"
        #define DEALER_PHONE "(606) 564 5028"
        #define PROD_NAME "TeleScope"
      #else
        #ifdef SOKIO
          #define COMPANY GetCompanyName()
          #define DEALER_PHONE GetDealerPhone()
          #define CARLSON_XPORT 1
		  #ifdef SURVNET
            #define PROD_NAME "SurvNet"
		  #else
            #ifdef EDITOR_ONLY
			  #define PROD_NAME "Carlson Raw Editor"
			#else
              #ifdef RPROCESS_ONLY
			    #define PROD_NAME "Edit-Process Raw File"
			  #else
			    #define PROD_NAME "Carlson X-Port"
                //#define PROD_NAME "Sokkia I/O"
              #endif
			#endif
		  #endif
        #else
		  #ifdef STANDALONE
            #define COMPANY GetCompanyName()
            #define DEALER_PHONE GetDealerPhone()
            #define PROD_NAME GetProductName()
          #else
		    #ifdef ICAD
              #define COMPANY "Carlson Software"
              #define DEALER_PHONE "(606) 564 5028"
		      #ifdef AGWORKS
                #define PROD_NAME "AgStar"
              #else
                #ifdef AIMS
                  #define PROD_NAME "CSI"
                #else
                  #define PROD_NAME "Carlson"
				#endif
			  #endif
			#else
              #define PROD_NAME "Carlson"
			#endif
          #endif
		#endif
      #endif    
    #else
      #define COMPANY "Carlson Software"
      #define DEALER_PHONE "(606) 564 5028"
      #ifdef CARLSON_CONNECT      
        #define PROD_NAME "Carlson Software"
	  #else
	    #ifdef TSUNAMI
          #define PROD_NAME "Carlson Survey"
        #else
		  #ifdef AGWORKS
            #define PROD_NAME "AgStar"
          #else
		    #ifdef TAKEOFF
              #define PROD_NAME "Takeoff"
            #else
              #ifdef AIMS
                #define PROD_NAME "CSI"
              #else
                #ifdef LDD
                  #define PROD_NAME "Carlson Survey Desktop"
                #else
                  #define PROD_NAME "Carlson Software"
			    #endif
			  #endif
		    #endif
	      #endif
        #endif
      #endif
    #endif
  #else
	#define PROD_NAME "SurvCE"
  #endif
  #define WM_KICKIDLE         0x036A
  #define SCADVER "2014"
  #ifndef DEALER_PHONE 
    #define DEALER_PHONE "(606) 564 5028"
  #endif
  #ifndef COMPANY
    #define COMPANY "Carlson Software"
  #endif
#endif

#if ((defined(WIN32_WINDOWS)) || (defined(UNDER_CE)) || (defined(EMULATOR))) 
  #include "include/scfakedefs.h"
#endif

#define ADS_GET_STRING_MAX 133  //according to documentation, 132 chars plus NULL is most that acad_getstring will get.
#define MAX_SWR_NAME 32
#define MAX_HYDRA_NAME 12
#define MAX_RAIN_ID 256		//maximum # of characters in rain ID, including terminating NULL
#define MAX_INLET_ID 256	//maximum # of characters in inlet ID, including terminating NULL
#define MAX_SWR_STRUCT_ID 256
#define SWRNET_G 32.17			// ft/s^2, grativational constant
#define SWRNET_G_M 9.81			// m/s^2

#include "tools/filesystem/filesystem.h"
#include "tools/md5/md5.h"
#include "cs_string/cs_string.h"
#include "unirecord/unirecord.h"
#include "settings/settingsext.h"
#include "screport.h"
#include "polyline/polyline.h"
#include "storage/storage.h"
#include "surface/surface.h"
#include "tools/formula/formula.h"
#include "tools/dbghelp/dbghelp.h"

#define CIPCSCADARG
#define CIPCSCADPARAM
#define CIPCSCADPARAMDEFAULT
/*
 * Inclusion of the following header enables use of CIP library and CAD Standards
#include "./library/cad/cip/lib/include/CIPCScad.h"
 */

extern void get_main_ini_name(char *);
extern short DateFormat(int Day, int Month, int Year, char *Format, char *Result);
extern short DateParse(char *Format, const char *DateString, int &Day, int &Month, int &Year);
extern void GetCurrentDateFormat(char *UsedFormat);
extern void FixAmp(char *str, char *dest);
extern int draw_order(int nWhere, ads_name enameSrc, ads_name ssSrc, ads_name enameTarg);
extern int draw_order_set(int nWhere, ads_name ssSrc, ads_name enameTarg);
extern void draworder_back(ads_name ename);
extern void draworder_front(ads_name ename);

#if defined(NOCPUTIME)
  #define CPU_TIME__START( x)   (0)
  #define CPU_TIME__END( x)     (0)
  #define CPU_TIME__REPORT( x)  (0)
#endif

// Returns TRUE if the given token is at the beginning of szLine and szArgument
//  will be made to point to the first character after the whitespace after the
//  token.
// Examples:
//  GetToken( "ABC", "abc    def", &szArg) returns TRUE and sets szArg to "def".  Note case-insensitivity.
//  GetToken( "bad", "abc def", &szArg) returns NULL and sets szArg to NULL.
extern BOOL GetToken( const char *szToken, const char *szLine, const char **pszResult);

#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
  extern void change_layer(const char *,ads_name);
  extern void change_color(int,ads_name);
  extern int explode_entity(ads_name);
  extern void add_degree_symbols(char *buf2, char* ang_sep = NULL);
  extern double brg_to_ang(char *buf);
  extern int ang_to_brg(double ang,char *buf,int places=0,int use_symbol=1, char* ang_sep = NULL,
												const char* szNorth=NULL, const char* szSouth=NULL,
												const char* szEast=NULL, const char* szWest=NULL);
  extern void dec_to_display(double ang,char *buf,int is_azi,int do_round,int use_symbol);
  extern void dec_to_deg(double val,char *buf,int do_round,int use_symbol,int prefix_mode=2,int is_lat=1, char* ang_sep = NULL);
  extern void gons_to_dms(char *);
  extern double gon_to_ang(double ang2);
  extern double gon_to_ang(char *input_buf);
  extern double azi_to_ang(double ang2);
  extern double azi_to_ang(char *input_buf);
  extern int azi_to_brg(double ang,char *buf);
  /** \brief converts radian angle to radian azimuth (angle from N)
    * \param angle angle in radian
    * \return azimuth angle in radian
  */
  extern double ang_to_azi(const double& ang);
  extern int is_running;
  extern void double_to_feet_and_inch(double dist,char *buf,int precision=3);
  extern void double_to_string(double,int,char *);
  extern void do_update_dwg_ini(char *);
  extern void URLPrepare(char *buf);
  extern void URLDecode(char *buf);
  extern int is_invalid_layer(char *);
  extern int make_valid_layer(char *);
  extern int get_registry_path(char *);
  extern LONG modRegCreateKeyEx(BOOL IsX64, HKEY hKey, LPCTSTR lpSubKey,  DWORD Reserved,  LPTSTR lpClass,
    DWORD dwOptions,  REGSAM samDesired,  LPSECURITY_ATTRIBUTES lpSecurityAttributes,  PHKEY phkResult, LPDWORD lpdwDisposition);
  extern LONG modRegOpenKeyEx(BOOL IsX64, HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
  extern void get_scjstart_path(char *path);
  extern double scad_get_elevation(double cval,char *prompt,int *get_mode,double *delta_val);
  #ifdef  ACRXAPP
    #include "adsdlg.h"
  #endif
  #if defined(ACRXAPP) || defined(ICAD)
    extern void CALLB help_action(acad_callback_packet *);
  #endif

  // These functions do NOT allocate the string for you.  The buffer must be of size FILENMSZ or FILENAME_MAX.
  // Here are the what the flags mean.  See the documentation on scad_getfiled_acad() for more details on
  //  the ones that are 32 or lower.
  #define FILE_DLG_ALLOW_NEW     (1<<0)  //1. a filename of a file that doesn't exist will be allowed.
  #define FILE_DLG_ANY_EXT       (1<<2)  //4. any extension (or none) may be used.
  #define FILE_DLG_NO_DFLT_NAME  (1<<4)  //16. the given path, 'tmpdir', is used only for directory and not filename
  #define FILE_DLG_NO_WARNING    (1<<5)  //32. if FILE_DLG_ALLOW_NEW is set, then no warning will be displayed if existing file is chosen.
  #define FILE_DLG_NEW_AND_EXIST (FILE_DLG_ALLOW_NEW | FILE_DLG_NO_WARNING) //very confusing, but (1<<5) seems to be used for two different purposes.-Ron
  #define FILE_DLG_APPEND_RECENT (1<<6)  //64. not sure what this means. Please rename and document as you see fit, Leonid. --Ron
  #define FILE_DLG_NO_TPL_GRAPHIC_PREVIEW (1<<7) // 128. needed by C&G because we use a non-graphic "*.tpl" file ending - BMitchell 2/23/09
  extern int scad_getfiled_common(const char *,const char *,const char *,int iFlags,struct acad_resbuf*pResult);
  //'result' is not modified unless RTNORM is returned.
  int scad_getfiled_common(const char *title, const char *tmpdir, const char *ext, int mode, char *result);
#ifdef WIN32_WINDOWS
  extern int scad_getfiled_win32(char *title,char *tmpdir,char *ext,int bFileMayNotExist,struct acad_resbuf *rtype,HWND Parent=NULL);
#endif

  extern double dollar_atof(char *dollarStr);
  extern void FormatPath(const char *orig, char *reform, int len);
  extern void FormatPath (csString& orig, int len);
#endif

extern BOOL FileExists(const _TCHAR *fileName); //checks if file exists
csString make_path_relative(const char *orig_file,const char *ref_path=NULL);
int make_path_local(char *orig_file,const char *ref_path=NULL);
extern FILE *scad_report_fopen(const char *open_mode = "wt");
extern void scad_report_display();
extern char usrdir[];
#ifdef STANDALONE
extern char RegistryRoot[FILENAME_MAX];
extern char ProductName[FILENAME_MAX];
extern char CompanyName[FILENAME_MAX];
extern char DealerPhone[FILENAME_MAX];
char *GetProductName();
char *GetCompanyName();
char *GetDealerPhone();
#endif 
#if defined(STANDALONE) || defined(OEM) || defined(LDD) || defined(ICAD) || defined(UNDER_CE) || defined(EMULATOR)
extern BOOL GetFeatureName(char *Serial, int Module, char *Result);
extern int ProductCode;
#endif
extern BOOL GetTrialVersion(char *Serial, char *Result);
extern BOOL GetLmVersion(char *Serial, char *Result);
extern int ShortWizard;
#ifdef CESCLIB
extern char execdir[];
extern char supdir[];
extern char tmpdir[];
extern char lspdir[];
extern char recpath[1024];
extern char tin_error_log[MAX_PATH];

//#include "scunirec.h"

#ifndef UNDER_CE

// DWF report functions
// extern void StartPrint();
// extern void setDWFValues(char *titleD[], char* sctFilesData[], char* linesD[], int colorD[][20]);
  
#ifdef WIN32_WINDOWS
  extern int get_windows_version_major();
  extern BOOL is_admin();
  class ScMemLoss {
  public:
    ScMemLoss(char *mesg);
    ~ScMemLoss();
    MEMORYSTATUS Mem1, Mem2;
    char Message[128];
  };
  #endif
#endif

typedef struct UserInfo {
  char WinSer[255];
  char AcadSer[50];
  char DiskSer[50];
  char MACAddr[50];
  char Company[255];
  char User[255];
  char ScadSer[80];
  char Email[128];
  char Phone[50];
  char Fax[50];
  int Reason;
} UserInfo;

#if (_MSC_VER >= 1400) && !defined(UNDER_CE) && !defined(_WIN64)
typedef __time32_t cs_time_t;
#else
typedef time_t cs_time_t;
#endif

// the following is guaranteed 32bit time - on any platform
#if (_MSC_VER >= 1400) && !defined(UNDER_CE) 
typedef __time32_t cs32_time_t;
#define cs_localtime32 _localtime32 
#else
typedef time_t cs32_time_t;
#define cs_localtime32 localtime
#endif

class RegInfo {
public:    
  RegInfo();
  // this comes from serial number (get_serial_info)
  char ScadSer[80];
  char Part1[40];
  char Part2[40];
  char Part3[40];
  char Part4[40];
  int ProductId;
  int InstallNumber;
  int DemoNumber;
  int RegMods;
  int IsHW;
  int IsNetwork;
  int IsNFR;
  int IsEDU;
  int IsGoodSerial;
  int CopyID;
  int IsLongStyle;
//this comes from get_reg_info
  int CurrentModule;
  int RegCalls;
  int DemoCalls;
  int DemoDays;
  int RegCode1;
  int RegCode2;
  int RegAnswer;
  int ExtCode1;
  int ExtCode2;
  int ExtAnswer;
  int Unregistered;
  int CanRegister;
  int IsDemo;
  int RegError;
  char sn_scad[32];
  char sn_acad[32];
  cs_time_t ExpirationTime;
  cs_time_t LastUsedTime;
  cs_time_t LicenseEndTime;
  cs_time_t AuthThroughTime;
};

#define ACAD_SN 20
#define SCAD_SN 32
#define ENCRYPT 1
#define DECRYPT 0

struct ident_s {
  char c_data1[4];
  char sn_acad[ACAD_SN];
  cs32_time_t time;
  unsigned short uses;
  char sn_scad[SCAD_SN];
  char c_data2[4];
  cs32_time_t time_last;
  cs32_time_t time_auth;
  cs32_time_t time_end;
}; 

// to be able to use larger font for registration dialogs
extern long RegDialogFontSize;

extern void getsctime(struct sctime *screc);
extern double time_diff(struct sctime t1, struct sctime t2);
extern void ExtrusionCorrection(double *p_norm, double *p_ins);
extern struct acad_resbuf *SkipN(struct acad_resbuf *type, int Number);
extern struct acad_resbuf *FindRBValue(struct acad_resbuf *List, const char *Name);
extern struct acad_resbuf *copylist( struct acad_resbuf *pOrig);//creates a copy. use ads_rbfree() to free results.
extern int FindFreeFieldIDByName(char *szAppName,char *szRecName);
extern int FindMaxFieldIDByName(char *szAppName,char *szRecName);
extern int FindMaxFieldID(char *szAppName);
extern void FindFreeFieldID(char *szAppName, int *pnFieldID, int isCheckGap=1);
extern int handent(char *Handle, ads_name ename);
extern int ssget_by_layer(char *layernm,ads_name ss1);
extern int scad_entsel(char *ent_name,const char *msg,ads_name ename,ads_point pnt);
extern int scad_entlast_marker(ads_name last_ename);
extern int scad_entdel_unlock(ads_name ename1);
extern int scad_entmod_unlock(struct acad_resbuf *rhead);
extern int scad_sssel(struct acad_resbuf *filter, ads_point pnt, ads_name ss);
extern int scad_sscopy( const ads_name ssOrig, ads_name ssNew); ///creates new selection set, ssNew, that is a copy of ssOrig. acad_ssfree() frees ssNew.
//Adds the entities from the given layer to the given selection set.
// The caller can use acad_ssadd and acad_ssfree to create and destroy the selection set.
extern void AddEntitiesFromLayer( ads_name selection, const char *szLayer);
extern void AppendSelectionSet( ads_name ssNew, ads_name ssOrig);//ssOrig = ssOrig + ssNew; no duplicates will be created
//deletes all the entities that are in the selection set from the drawing. Doesn't alter ssToDelete. Returns # successfully deleted.
extern int DeleteEntitiesInSelectionSet( const ads_name ssToDelete);
extern void update_3dpts_surface(const char *ref_surfnm,ScTIN *RefTin);
extern int update_labspot_entity(ads_name ename,double z_pt,double elev_val);
extern void update_labspot_surface(const char *ref_surfnm,ScTIN *RefTin);
extern void update_progrid_surface(const char *ref_surfnm);
extern void update_sctgrid_surface(const char *ref_surfnm);

#define FAKE_SN 987654
extern char *scad_getenv(char *);
extern int CheckRegistrationStatus(char *path,int=1,BOOL quick_check=FALSE, BOOL Silent=FALSE, long *Status=NULL);
extern int check_module_status(int module_num);
extern BOOL FindFeature(char *name,char *feature);
struct FeatureStruct {
  char key[40];
  char alt_key[40];
  char FullName[100];
  char KeyCommands[255];
};
extern struct FeatureStruct Features[17];
extern void get_hardware_serial(char *);
extern void get_platform(char *);
extern int read_block(struct ident_s *ptr,const char *record);
extern int write_block(struct ident_s *ptr,const char *record);
extern void secure(char *buff, int mode,int nbytes);
extern int get_serial_info(const char *Serial, RegInfo *rec, long RegPos=0,int is_check=1);
extern long hextol(char *str);
extern BOOL Disable64Reflecton;
extern void EnsurePath(int=0);
extern void OpenHtml(char *name);
extern int demo_change(short uses,long days,int lock, long RegPos=0);
extern int enter_serial(int);
extern int init_registration();
extern void extend_demo();
extern void change_registration();
extern void edit_serial_numbers();
extern int get_product_id();
extern int call_decode(struct acad_resbuf *args, struct acad_resbuf **rslt);
extern ScUniRecordBase *GetSerialNumberReview();
extern void get_serial_status(long RegPos, char *status);
extern void sn_mark(const char *recpath, int mark, BOOL IsX64);

extern double parse_elev_text(const char *szElevLabel, csString *pStrPrefix=NULL, csString *pStrNumber=NULL,
  csString *pStrSuffix=NULL, int *piDecimals=NULL); ///returns HUGE if no number is found
extern void assign_color_list_rgb(long *,int is_white_daylight);
extern void assign_rgb_list(int *red_list,int *green_list,int *blue_list,int is_white_daylight);
extern long assign_rgb_num(long r,long g,long b);
extern int register_app(const char *name);
#ifdef HAVE_CAD
extern void SendCommandToAutoCAD(const char *original_cmd, BOOL UseMgr = FALSE);
extern void SendCommandToAutoCAD(char *cmd, BOOL UseMgr=FALSE);
extern void scad_save_tiles(ads_hdlg hdlg, char *dcl, char *dialog, int is_temp);
extern void scad_save_tile(ads_hdlg hdlg, char *dcl, char *dialog, char *tile, int is_temp);
extern int scad_load_tiles(ads_hdlg hdlg, char *dcl, char *dialog, int where);
extern int scad_find_tile(char *dcl, char *dialog, int where, char *key, char *copy, int size);
extern void scad_enable_tiles(ads_hdlg hdlg, char *dcl, char *dialog);
extern void ScadSaveTilesUniRec(ads_hdlg hdlg, const char *dcl, const char *dialog, ScUniRecordBase *Record);
extern void ScadLoadTilesUniRec(ads_hdlg hdlg, const char *dcl, const char *dialog, ScUniRecordBase *Record);
extern int scad_ssget2(char *str, const void *pt1, const void *pt2, const struct acad_resbuf *filter,ads_name ss);

extern struct acad_resbuf *GetDictKeys(const char *RootKey, const char *DictName=NULL, BOOL bReturnValues=FALSE);
extern struct acad_resbuf *GetDictValues(const char *RootKey, const char *DictName);
extern BOOL SetDictRecord(const char *RecName, struct acad_resbuf *values, const char *DictName=NULL, ads_name ename=NULL);
extern struct acad_resbuf *GetDictRecord(const char *RecName, const char *DictName=NULL, ads_name ename=NULL, BOOL bSilent=FALSE);
extern BOOL IsEEDAttached(const char *DictName, ads_name ename);
extern void SetDictBinary(const char *RecName, void* buf, int length, const char *DictName = NULL);
extern BOOL GetDictBinary(const char *RecName, void*& buf, int &length, const char *DictName = NULL);
extern void SetDictDouble(const char *RecName, double Value, const char *DictName = NULL);
extern BOOL GetDictDouble(const char *RecName, double *Value, const char *DictName = NULL);
extern void SetDictInt(const char *RecName, int Value, const char *DictName = NULL,int restype=AcDb::kDxfInt32);
extern BOOL GetDictInt(const char *RecName, int *Value, const char *DictName = NULL);
extern void SetDictString(const char *RecName, const char *Value, const char *DictName=NULL,int restype=1,int allow_relative=1);
extern void GetDictString(const char *RecName, char *Value, int Length, const char *DictName=NULL);
extern void GetDictString(const char *RecName, csString &Value, const char *DictName=NULL);
extern void SetDictSS(const char *RecName, ads_name ss, const char *DictName=NULL);
extern void GetDictSS(const char *RecName, ads_name ss, const char *DictName=NULL);
// hasEntriesLeft = -1, cannot retrieve the number of entries left; 1 - has entries; 0 - has no entries left
extern BOOL DelDictRecord(const char *RecName, const char *DictName=NULL, ads_name ename=NULL, int *hasEntriesLeft=NULL);
extern BOOL DelAllDictRecord(const char *DictName=NULL, const char *RecName=NULL, ads_name ename=NULL);
#ifdef ACRXAPP
  AcDbDictionary *GetDictionaryPointer(const char *DictName, int ReadOnly=0);
#endif
extern BOOL RenameKey(const char *RootKey, const char *OldName, const char *NewName, const char *DictName=NULL);
extern void stripdname(char *dname);
extern void read_tk_active_surfaces(char *,int *,int *,char *,char *);
extern void get_tk_surface_name(char *,char *,char *prefix,int pos);
extern void LoadProjectFiles(char *exts, ScUniRecordBase **PathList, ScUniRecordBase **FileList);
extern lwpoint *ReducePolylineEnds(lwpoint *proot);
extern long DensifyPolyline(lwpoint *, double,int use_remain=0);
extern int ValidPathFile(const char *fname);
extern void StoreRecentFile(const char *fname);
#endif
extern void ModifyFavoriteFolder(const char *path, BOOL DoRemove);
extern void ListFolderFiles(const char *exts, const char *Folder, ScUniRecordBase **FileList);
extern void NewFullPath(const char *OrigPath, const char *NewName, char  *FinalPath);
extern ScUniRecordBase *ValidateInstall(const char *ScJStartFolder, BOOL Current, BOOL Status(long ,long ,const char *)=NULL);
extern int convertbkslash(char *in_str);
///Entity extension dictionary data
extern int SetXDictRecord(ads_name ename, const char *pszRecName, struct acad_resbuf *recRb);
extern int SetXDictRecords(ads_name ename, const char *Name, struct acad_resbuf *recRb);
extern struct acad_resbuf* GetXDictRecord(ads_name ename, const char *psxRecName);
extern struct acad_resbuf* GetXDictRecords(ads_name ename, const char *Name);
extern int scad_entdel(const ads_name ename);
extern void UnixTimeToFileTime(time_t t, LPFILETIME pft);
extern int download_file(const char *url, const char *fname);

// good/bad beeping
#include <mmsystem.h> //for Music()
#pragma comment(lib, "winmm.lib")
/// Used by Music () function.
enum MusicID
{
  MUSIC_BAD_TRIPLE         = 0, // Do Do Do -- triple boop, all progressively lower pitch
  MUSIC_GOOD_DOUBLE_SINGLE = 1, // Do Do Ding -- first 2 notes are identical, last note is higher pitch
  MUSIC_2                = 2,
  MUSIC_3                 = 3,
  MUSIC_BAD_SINGLE         = 4, // Boooop -- long low pitched note
  MUSIC_GOOD_TRIPLE        = 5, // Do Do Do -- triple boop, all progressively higher pitch
  MUSIC_BAD_FIVE_SUPER_BAD = 6, // 5 Booops -- all same pitch (5 MUSIC_BAD_SINGLEs)
};
extern void Music (MusicID Kind);



/// Executes an external program. Optionally waits and returns the exit code. Returns 0 otherwise.
extern int WinSystem(char *name,int wait,BOOL HideWindow=FALSE);
extern void WinShellExecute(char *name, char *cmdline,int wait, const char *path); // uses ShellExecute method
extern void strcat_dll_ext(char *str);
extern csString strcat_dll_ext (const char* ArxName);

class FileMatchMaker {
public:
  FileMatchMaker(const char *startFolder, const char *matchPattern, BOOL includeSubfolders) {
    StartFolder=startFolder;
    MatchPattern=matchPattern;
    IncludeSubfolders=includeSubfolders;
    FoldersToScan=new ScUniRecord();
    FoldersToScan->SetId(startFolder);
    FoundMatches=NULL;
    Completed=0;
  }
  ~FileMatchMaker() {
    if (FoldersToScan!=NULL) FoldersToScan->ReleaseAll();
    FoldersToScan=NULL;
    if (FoundMatches!=NULL) FoundMatches->ReleaseAll();
    FoldersToScan=NULL;
  }
  ScUniRecordBase *GetResults() { return FoundMatches; }
  long GetCompleted() { return Completed; }
  long GetQueued() { 
    if (FoldersToScan==NULL) return 0;
    return FoldersToScan->RecordCount();
  }
  long GetFound() { 
    if (FoundMatches==NULL) return 0;
    return FoundMatches->RecordCount();
  }
  BOOL IsFinished() {return (FoldersToScan==NULL); }
  BOOL PerformStep();
  
protected:
  long Completed;
  ScUniRecordBase *FoundMatches;
  ScUniRecordBase *FoldersToScan;
  csString StartFolder;
  csString MatchPattern;
  BOOL IncludeSubfolders;
};
  
// simple text file parsing tools.
// Reads one line of text up to, but not including, a newline character.
// Result will be stored in szResult, whose size is iLength.
// Returns # of characters placed into szResult, not including terminating NULL.
int scad_readline( FILE *fp, char *szResult, int iLength);


#ifdef CE_UPDATE
extern ScUniRecord RegRecord;
#endif

extern int get_charset();
class UnitsRec {
public:
  UnitsRec();
  double Area;
  double Volume;
  char LengthName[10];
  char LengthLongName[50];
  char TonName[50];
  char StationName[50];
  char DateFormat[50];
  double CoalThick;
  double CoalDens;
  double RockThick;
  double RockDens;
  BOOL Init;
};  
extern UnitsRec UnitsSetup();
#ifdef ACRXAPP
extern void attach_finish_xdata(ads_name ename, const char *crdfile, const char *szPointName = NULL,
                          const char *szCodeName = NULL, AcDbObject *pObj = NULL);
#else
extern void attach_finish_xdata(ads_name ename, const char *crdfile, const char *szPointName = NULL,
                          const char *szCodeName = NULL, ads_name eObj = NULL);
#endif
#ifdef HAVE_CAD
///attaches F2F extended entity data. See definition for details.
extern int attach_image(char *fname, ads_point pnt1, ads_point pnt2, int noscale = 0);
// returns TRUE if the entity was found, has Field-to-Finish extended entity data, and the crd file.
BOOL read_finish_xdata( ads_name ename, csString *pCrdFileStr, csString *pPointNamesStr=NULL, csString *pCodeStr=NULL,
                        BOOL *pbFoundCodeXdata=NULL);
#endif

#ifdef ACRXAPP
  #define CAD_HATCH_FORMAT "acad(%s)"
#else
  #define CAD_HATCH_FORMAT "icad(%s)"
#endif

// error-checking macro to help test the return value of Acad ARX function calls.  --Ron
#if defined( ACRXAPP) 
  #ifdef CS_DEBUG
    #define ASSERT_EOK( func) {                                                                                             \
      Acad::ErrorStatus e = func;                                                                                           \
      if (e!=eOk) acad_printf("\n ARX call failed.  Returned %d in file %s on line %d: " #func "\n", e, __FILE__, __LINE__); \
    }
  #else
    #define ASSERT_EOK( func) func
  #endif
#endif //ACRXAPP 

// error-checking macro to ensure that the result is RTNORM
#ifdef CS_DEBUG
  #define ASSERT_RTNORM( func) {                                                          \
    int e = func;                                                                         \
    if (e!=RTNORM) {                                                                      \
      struct acad_resbuf bufErrno;                                                             \
      acad_getvar( "ERRNO", &bufErrno);                                                    \
      acad_printf("\n ADS call failed.  Returned %d in file %s on line %d. ERRNO was %d",  \
                 e, __FILE__, __LINE__, bufErrno.resval.rint);                            \
    }                                                                                     \
  }
#else
  #define ASSERT_RTNORM( func) func
#endif

void GetMACAddress (char *address);

// Replaces the given extension with *.bak or appends it if there is no extension.
// Attempts to delete the file just in case it happened to exist.
void MakeTempFilename( char *szFilename);
extern void get_data_file_subfolder(char *ext_buf,char *subfolder);
extern double get_file_date_time(char *filenm);

extern BOOL get_lisp_string(char *Value, const char *Variable);//converts int types and double to string. returns FALSE on failure, TRUE on success.
extern BOOL get_lisp_string(csString *pStrValue, const char *Variable);//converts int types and double to string. returns FALSE on failure, TRUE on success.
extern BOOL get_lisp_long(long *Value, const char *Variable);//does not convert string or double to long.  returns FALSE on failure, TRUE on success.
extern BOOL get_lisp_int(int *Value, const char *Variable);//does not convert string or double or long to int.  returns FALSE on failure, TRUE on success.
extern BOOL get_lisp_real(double *Value, const char *Variable);//does not convert string to double.  returns FALSE on failure, TRUE on success.
extern BOOL get_lisp_point(double *Value, const char *Variable);
extern BOOL get_lisp_ename(ads_name Value, const char *Variable);
extern void set_lisp_ename(char *Variable, ads_name ename);
extern void set_lisp_sset(char *Variable, ads_name sname);
extern void set_lisp_string(char *Variable, const char *Value);
extern void set_lisp_point(char *Variable, double *Value);
extern void set_lisp_double(char *Variable, double Value);
extern void set_lisp_int(char *Variable, int Value);
extern BOOL get_var(const char *,char *,int=0);
extern BOOL set_var(const char *,const char *,int=0);
extern BOOL del_var(const char *,int=0);
extern BOOL get_var_string( char *szResult, const char *szSystemVar); //gets an AutoCAD system variable that is a string type.
extern BOOL set_var_string( const char *szSystemVar, const char *szValue); //sets an AutoCAD system variable that is a string type.
extern BOOL get_var_real( double *pfResult, const char *szSystemVar); //gets an AutoCAD system variable that is a floating point type.
extern BOOL set_var_real( const char *szSystemVar, double fValue); //sets an AutoCAD system variable that is a floating point type.
extern BOOL get_var_int( int *piResult, const char *szSystemVar); //gets an AutoCAD system variable that is an integer type (RTSHORT).
extern BOOL set_var_int( const char *szSystemVar, int iValue); //for RTSHORT types
extern BOOL get_var_point( ads_point result, const char *szSystemVar); //gets an AutoCAD system variable that is a point type.
extern BOOL set_var_point( const char *szSystemVar, ads_point result); //sets an AutoCAD system variable that is a point type.

extern int scad_get_string(char *, char *,char *,int);
extern double safe_atof(char* buf,int size);
extern int valid_double(char* buf,int size);
extern void print_line(char *line);
extern void scad_alert(const char *);
extern void prepare_layer_filter(const char *layer_in,char *layer_out);
extern int my_ads_wcmatch(const char *szString, const char *szPattern);//returns RTNORM if there's a match, RTERROR if not, and RTNONE if one of the arguments is NULL.
extern void get_user_info(UserInfo *rec, long Pos=0);
extern int get_regist_info(RegInfo *rec, long Pos=0);
extern int apply_change_key(long RegPos);
extern BOOL combine_serials(BOOL Silent=TRUE);
extern int copy_file(const char *from,const char *to);
/// copies file to file, correcting the UNIX \n to Win \r\n along the way
int copy_file_unixtowin(const char *from, const char *to);
extern int copy_file_to_utf8(const char *from,const char *to);
// forces the downgrade of a file from Unicode to Ascii
// returns, 1 - file converted, 0 - no need to convert file not UNICODE, < 0 errors: -1 parameters passed are not correct, -2 file to read/open from failed, -3 file to read/open to failed
extern int copy_file_to_ascii(const char *from,const char *to);
extern int append_file(const char *from,const char *to);
extern BOOL copy_different_file(const char *path_from, const char *path_to, const char *file);
extern BOOL copy_files(const char *path_from, const char *path_to, const char *start_path, const char *excludes_file,
                       BOOL Recursive, BOOL MissingOnly, BOOL NotSameOnly);
extern void do_copy_tin_file(char *,char *);
extern void attach_contour_label_xdata(ads_name txt_ename,ads_name wipeout_ename,ads_name pl_ename,ads_name box_ename);
extern void spt();
extern double clear_angbase();
extern void reset_angbase(double angbase);
extern void clear_osmode();
extern void reset_osmode();
extern void setvar_dimscale(double val);
extern int setvar_color(char *buf);
extern int setvar_color(int num);
extern void prepare_plan_arrow(ads_name);
extern void draw_plan_icon(double *pnt,ads_name ename);
extern void CompleteSymbol(char *dest, const char *name, int=0);
extern void bad_registration();
extern int get_angle_format();
extern void sc_ang_to_azi(double ang,char *buf,int is_sym,int places);
extern int ang_to_display(double ang,char *buf,int is_azi,int angle_prec,int use_symbol,int is_direction=1);
extern double display_to_ang(char *buf,int is_azi);
extern double display_to_dec(double,int is_azimuth);
extern double deg_to_dec(double);
extern int check_dms(double);
#if ((!defined(UNDER_CE)) || (defined(CE_OTHER)))
extern void parsedeg(double,double *,double *,double *);
#endif
void read_part(char *buf,int num1,int num2,char *result);
void fspadder(char *in_str,short num);
void spadder(char *in_str, short num);
// angle, deg utilities
double FixAziUp(double fAng);
double FixAzi(double fAng);
int GetQuadrant(double fAng, BOOL bUseTwist=TRUE);
extern double my_ads_angle(const ads_point pt1, const ads_point pt2);
extern double my_ads_distance(const ads_point pt1, const ads_point pt2, BOOL bIs3D, BOOL bGetSq=0);
//implementation of AutoCAD's acutPolar, a.k.a., acad_polar.
extern void my_acad_polar(const ads_point pt, double angle, double dist, ads_point ptres);
// See acdbRToS() in the AutoCAD ObjectARX help file.
// Only supports 'unit' being 2 which means "engineering (feet and decimal inches)". Returns RTNORM.
// Works slightly better than acdbRToS() for large numbers, like 10^13 and higher.
extern int my_acad_rtos(double val, int unit, int prec, char* str);
extern void my_acad_area(ads_name ename,double *area_val,double *perim_val);
extern int scad_getkword(const char *prompt_buf,char *result_buf,int initget_code,const char *default_buf,const char* fmt, ...);
extern void scad_initget(int initget_code,const char* fmt, ...);

extern int check_key(long seed,long index,long copy_id,long answer,BOOL nfr);
extern void StringToCheckSum(char *);
extern void CheckSum(char *buf);
double get_acadver();
extern void GetCurrentLanguage(csString &Lang, csString &BaseLang);
/**
 * getDictionary attempts to efficiently locate the node corresponding to dictionary entry kpszName.
 * @param kpszName									Name of the entry.
 * @param kpsacad_resbufDictionary	Parent dictionary.
 * @return													Pointer to entry kpszName or NULL
 */
acad_resbuf* getDictionary(const _TCHAR* kpszName, const acad_resbuf* kpsacad_resbufDictionary);

/**
 * createGroup encapsulates the creation of a (nongraphic) GROUP entity and associated owner record in
 * (standard) dictionary ACAD_GROUP.
 * @param kpszName					Name of GROUP. As an optimization, NULL is supported, in which case the name
 *													is set to the (anonymous) name *A<GROUPHandle>, which (theoretically, at least)
 *													should generate a unique name within the drawing.
 * @param kpszDescription		GROUP description. May be NULL.
 * @param kbSelectable			Boolean indicating selectability of resultant GROUP.
 * @param kulMemberCount		Count of ads_name structures which follow.
 * @param ...								variable length list of ads_name structures comprising the target GROUP.
 * @return									RTNORM (success) or RTERROR (check system variable ERRNO)
 *
 * @author Cliff Petersohn  July, 2008
 */
const int createGroup(const _TCHAR* kpszName, const _TCHAR* kpszDescription, const bool kbSelectable,
											const unsigned long kulMemberCount, ...);
#ifdef HAVE_CAD
/**
 * createImage encapsulates the creation of an IMAGE entity and associated 3 owner (nongraphic) objects beginning at
 * (standard) dictionary ACAD_IMAGE_DICT entry, IMAGEDEF, and IMAGEDEF_REACTOR
 * @param rads_nameImage				Name of resultant IMAGE entity if successful.
 * @param kpszImageName					Name of IMAGE as it should appear in the image manager.
 * @param kpszImageURL					URL of image file.
 * @param rads_matrixTransform	Rowmajor image metrics, where
 *															rads_matrixTransform[0] vector of a single pixel (points along the visual bottom of the image)
 *															rads_matrixTransform[1] vector of a single pixel (points along the visual left of the image)
 *															rads_matrixTransform[2] image size in pixels
 *															rads_matrixTransform[3] insertion point
 * @param kpszImageLayerName		Layer in which to instantiate IMAGE entity. Default NULL implies current layer
 * @param kbIsLoaded						load / unload boolean
 * @param knResolutionUnits			Resolution units. 0 None; 2 cm; 5 in
 * @param knProperties					Display properties bitmask. 0x01 Show; 0x02 Show nonaligned; 0x04 Use clip boundary; 0x08 Enable transparency
 * @param kbIsClip							Clip state. 0 Off; 1 On
 * @param knBrightness					Brightness value in range [0..100]
 * @param knContrast						Contrast value in range [0..100]
 * @param knFade								Fade value in range [0..100]
 * @param knClipType						Clip boundary type. 1 Rectangular; 2 Polygonal
 * @param kulClipVertexCount		Count if clip boundary verticies which follow
 * @param ...										variable length list of ads_point arrays comprising the clip boundary
 * @return											RTNORM (success) or RTERROR (check system variable ERRNO)
 * @bug Beware of potetial invalid combinations of parameters on various platforms.
 * @todo Implementation and correct handling of clip boundary. Cleanup of orphan objects if errors are encountered.
 *
 * @version 0.0
 * Initial manuscript
 * @version 0.1
 * Introduced optional argument kpszImageLayerName +CRP [July, 2009]
 *
 * @author Cliff Petersohn  July, 2008
 */
const int createImage(ads_name& rads_nameImage, const _TCHAR* kpszImageName, const _TCHAR* kpszImageURL,
											const ads_matrix& rads_matrixTransform,
											const _TCHAR* kpszImageLayerName = NULL,
											const bool kbIsLoaded = true, const short knResolutionUnits = 0,
											const short knProperties = 7, const bool kbIsClip = false,
											const short knBrightness = 50, const short knContrast = 50, const short knFade = 0,
											const short knClipType = 1,
											const unsigned long kulClipVertexCount = 0, ...);
#endif

void refresh_acad_screen();
void zoom_command(char *buf);
void zoom_redraw();
void zoom_center(double *center_pnt,double size);
void zoom_window(double *pnt1,double *pnt2);
void zoom_extents();
BOOL SetAnnotative(const ads_name ename, const BOOL bAllAnnoScales);
int draw_point(double *pnt1);
int draw_circle(double *pnt1,double radius);
void draw_donut(double *center_pnt,double diameter,double width);
int draw_solid(double *pnt1,double *pnt2,double *pnt3,double *pnt4);
int draw_arc(double *pnt1,double StartAngle, double EndAngle, double Radius);
int draw_line_2pt(double *pnt1,double *pnt2);
int draw_pline_common(struct acad_resbuf *pnt_list,int is_closed);
int draw_arrowhead(double *pnt1,double ang1,double size1);
int draw_circle(double *pnt,double radius);
void draw_leader(ads_point pntTail, ads_point pntArrowhd, double arrow_size,ads_name ssNewLeader=NULL, BOOL bArrow=TRUE,BOOL bTick=FALSE, double tick_width = -1.0, lwpoint *tickPts=NULL);
int draw_text_common(const char *text_buf, double text_size, double *start_pnt,
					 double *to_pnt, const char *justification,
					 int is_3d = 0, BOOL useTextCommand=FALSE,
					 int nAnnotative=-1, int nAllAnnoScales=-1
					 CIPCSCADPARAM CIPCSCADPARAMDEFAULT);
int draw_text_angle(const char *text_buf,double text_size,double *start_pnt,double dir_ang,const char *justification, BOOL useTextCommand=FALSE CIPCSCADPARAM CIPCSCADPARAMDEFAULT);
int draw_text_next_line(const char *text_buf, BOOL useTextCommand=FALSE);
int draw_text_horiz(const char *text_buf,double text_size,double *start_pnt,const char *justification, BOOL useTextCommand=FALSE CIPCSCADPARAM CIPCSCADPARAMDEFAULT);
int draw_text_at_decimal_pt(const char *text_buf, double text_size, ads_point start_pnt, double dir_ang,const char *justification);
int align_text_at_character(const char *text_buf, double text_size, ads_point start_pnt, double dir_ang,const char *justification, const char* substr);
int draw_text_at_character(const char *text_buf, double text_size, ads_point start_pnt, double dir_ang,const char *justification, const char* substr);
extern void draw_mtext(const char *mtext_buf, double text_height,
					   double *start_pnt, double rotation, const char *justification,
					   int max_width=100,int is_3d=0,
					   int nAnnotative=-1, int nAllAnnoScales=-1);
void draw_mleader(double *pnt1,double *pnt2,char *label_buf,double txt_size);
extern void ApplicationByExt(const char *name,char *path);
extern BOOL OpenByExt(const char *name);
extern int create_activate_viewport(ads_point pnt1, ads_point pnt2);
#endif

class ScTopoObject;
class ScTIN;
class ScKdTree;
class ScObject;
class ScPoint;

#ifdef CESCLIB

extern void make_line_common(const lwpoint *,const PolyInfoRec&);
extern int make_polyline_common(lwpoint *root,const PolyInfoRec &info,
                int bulge_flag,int type_change, void* pKeepPLine=NULL CIPCSCADPARAM CIPCSCADPARAMDEFAULT);//modifies 'root'
extern void print_polylinelw(lwpoint *root,char *name);
extern double get_entity_angle(ads_name,ads_point);
extern int my_acad_textbox(ads_name ename,double *txt_pt1,double *txt_pt2);
///Gets the 4 points of an oriented bounding box around a text entity. Returns TRUE if successful. pfWidth & pfHgt are optional.
extern BOOL GetBoundingBox(ads_name ename, ads_point *adsptBox, double *pfWidth, double *pfHgt);
extern BOOL GetBoundingBox(ads_name ename, ScOBB *pResult);
extern int arc_tangentIn_tangentOut_point(double xk1,double yk1,double xk2,double yk2,double xl1,double yl1,double xl2,double yl2,double xp,double yp,int is_draw,double *bulge,double *tang_pnt1,double *tang_pnt2);
extern void ads_point_copy(ads_point dst, ads_point src);
#ifndef NOCSCGL
extern int GetBoundingBox(ads_name ename, ObliqueBBox &acgeBox, BOOL bGetGeomExtent=FALSE);
#endif //NOCSCGL
extern int get_xdata_entities(ads_name inclu_ss,const char *data_name); // ssget by xdata
extern struct acad_resbuf *GetXData(ads_name ename, const char *Xname);
extern BOOL SetXData(ads_name ename, const char *Xname, struct acad_resbuf *data);
extern acad_resbuf *GetAppXData(ads_name ename, const char *AName);
extern BOOL SetAppXData(ads_name ename, const char *AName, acad_resbuf *data);
/*
#if SCMALLOC  //see SCMALLOC above for discussion; used for finding memory leaks and memory corruption.
  lwpoint *new_lwpoint_memcheck( const char *szFile, int iLine);
  #define new_lwpoint() new_lwpoint_memcheck( __FILE__, __LINE__)
#endif
*/
// bArcsOK == 1 means arcs are OK.  bArcsOK 0 means no arcs allowed.
// piIsClosed is an optional pointer to an integer that will be filled in with 0 or 1
//  if the polyline line is closed or not.
// szHandle is an optional pointer to a character buffer to receive the polyline's handle.
//   szHandle should be at least HANDLESZ characters long, including terminating NULL.
// NOTE: the same series of points is returned whether or not polyline is closed.
//  Use ClosePolyline() to duplicate the first point and append it at end of list.
extern lwpoint *extract_polyline(ads_name,int bArcsOK, int *piIsClosed = NULL, char *szHandle = NULL,int IsCheckSpace=0);
extern lwpoint *ReversePolylineList(lwpoint *root);
extern BOOL PolyInfo(ads_name, PolyInfoRec &);
#ifdef HAVE_CAD // my_mlayer is defined in scpoly.cpp and is in an #ifdef HAVE_CAD block
extern void my_mlayer(const char *name,int Color,BOOL bSetAsCurrent=TRUE); //changes 'name' to be valid layer name. Setting as current layer is expensive.
#endif
extern int isLockLayer(char *layernm,int do_unlock=0);
extern void set_layer_color(const char *const_name,int Color); // Allows for changing the color of an existing layer
extern void set_layer_linetype(const char *const_name,const char *LineType);
extern void set_layer_properties(const char *const_name,int Color, const char *LineType, int LineWeight, const char *PlotStyle, const char *description=NULL, BOOL isPlottable=TRUE); // Allows for changing the properties of an existing layer
extern int rename_layer(char *old_name,char *new_name,int is_silent=0);
extern struct acad_resbuf *get_layer(struct acad_resbuf *);
extern int getViewTwist(double& viewTwist);
extern int getCursorSize(int& cursorSize);
extern int getPickBox(int& pickBox);
extern int getAperture(int& aperture);
extern char *GetCurrentSpace();
extern lwpoint *calc_bounding_box(lwpolys *lroot);
extern int tangentPolylineCheck(lwpoint *proot,double dTol,int report_mode,FILE *fp);
extern void get_bbbox (ads_point&, ads_point&, double, ads_point);
///gets the bounding box, in 3D. Returns 1 if successful and 0 otherwise. In WCS units.
extern int get_bbox(ads_name ename, ads_point min, ads_point max);
///gets the bounding box (3D) around a whole selection set. Returns 1 if there was at least one geometric entity. In WCS units.
class ScSSet;
extern long do_get_ssbox(ads_point min, ads_point max, ScSSet& rScSSet);
/// If the given bounding box (in WCS units) is off screen, a Pan command will be issued to center on the middle of the given box.
extern void PanIfOffScreen( ads_point min, ads_point max);
extern BOOL GetEntityLocation( ads_name ent, ads_point pntLocation);///fills in pntLocation w/ 1st dxf 10 code.
extern int set_text_style(char *style_name,char *font_name=NULL,double new_size=0.0,int is_current=1);
extern void set_ltype_style(char *Name);
extern int GetPerimeterLeaderPoints(lwpoint *proot,double *center_pnt,double *perim_pnt,double *leader_pnt,double leader_angle,double txt_size);
extern void set_ltype_style(char *);
/// Limit selection set to the entities in the current work space. 
/// Returns a number of entities remaining in the selection set.
extern long scad_filter_current_space(ads_name ssname);
/// purges all layers which can be purged
extern int scad_purge_layers(char *);
/// Get entity color or layer color if BYLAYER
extern int scad_getcolor(ads_name);
///Set entity color
BOOL scad_setcolor(ads_name ename, int Color);
/// Get entity linetype or layer linetype if BYLAYER
extern void scad_getlinetype(ads_name ename, csString &Linetype);
/// Highlights an entity as if AutoCAD had selected. Be sure that if this entity was recently
///  created or modified, that the graphics have been "flushed". See docs on AcDbEntity::highlight().
/// Call UnhighlightEntity() to restore to normal.
extern void HighlightEntity( ads_name ename);
extern void UnhighlightEntity( ads_name ename);
/// Makes the given entity invisible or visible.
bool setEntityVisibility(ads_name sads_nameEntity, const bool kbVisible);
bool InvisibleEntity(ads_name sads_nameEntity);
bool VisibleEntity(ads_name sads_nameEntity);
/// Get entity visibility
extern int scad_is_visible(ads_name ename);
/// Get entity layer
extern void scad_getlayer(ads_name,char *);
/// Returns TRUE if the layer exists. If it exists, the various states can be optionally returned.
extern BOOL scad_getlayer( const char *szLayer, BOOL *pbOn=NULL, BOOL *pbThawed=NULL, BOOL *pbUnlocked=NULL);
// Return TRUE if layer is found in Layer Library, if it exists, the various properties can be returned
extern BOOL layerlib_getlayer( const char *szLayer, int *color=NULL, char *description=NULL, char *linetype=NULL, char *lineweight=NULL, char *plotstyle=NULL);
/// Get entity handle
extern BOOL scad_gethandle(ads_name,char *);
/// Get insertion point of text by ename or acad_resbuf list
void scad_gettextpoint(ads_name ename, struct acad_resbuf *entity, ads_point pnt);
//returns the slope at the given station or 0.0 if station is not on line.
extern double SlopeAtPoint( lwpoint *pLine, double fDistAlong);
/*  Finds the point along pPoly that is closest to pntOnLine.  Returns -1.0 if the found point
  is farther than fMaxDistFromLine.  Otherwise, returns the distance along the polyline that this
  closest point can be found. Arcs are not supported.
  ppPrev is an optional parameter.  If not NULL, it will be filled in with the lwpoint that
  is immediately before the found point.  If no point is found, NULL will be filled in. */
extern double DistanceAlongPolyline( lwpoint *pPoly, ads_point pntOnLine, double fMaxDistFromLine,
                                     lwpoint **ppPrev = NULL, ads_point *pActualOnLine = NULL);
// make_lwpolyline is now forward defined in lwpoint.h, but implemented in scpoly.cpp
extern int EdgeCheck(ads_point pnt, ads_point top, ads_point bottom);
extern double Find_Side(ads_point pnt,ads_point pntnext,ads_point pntside);
extern int IntersectRay(ads_point s,double angle,lwpoint *bound, ads_point PMinX,
                 ads_point PMaxX, ads_point pminX, ads_point pmaxX);
extern int GetPolylineInsidePoint(lwpoint *polyline, ads_point middle, int slow); // also GetInsidePoint(lwpoint *bound, ads_point center) in polymath.h
extern int polyline_center(lwpoint *list,double *pnt);
extern lwpolys *ReadSelectionSet(ads_name sname);
extern int remove_selection_groups(ads_name setname, BOOL Silent=TRUE);

extern int IsEndpoint2D(lwpoint *polyline, ads_point testpoint);
extern lwpoint *ReorderPolyline(lwpoint *poly, lwpoint *origin);
// pLoop is a polyline whose last point has the same coordinate as the first.
// pntExclude, pnt1st, and pnt2nd must all be on the loop.
// Creates a polyline from pnt1st to pnt2nd whose route excludes pntExclude.
extern lwpoint *CreatePartOfLoop( lwpoint *pLoop, ads_point pntExclude, ads_point pnt1st, ads_point pnt2nd);
extern int CrossesSelf( const lwpoint *pRoot); //returns 1 if polyline intersects itself, 0 otherwise.  Being Ad does not count as self-crossing.
extern void GetRemoveArcsTol(double *,double *,int *);
extern lwpoint *BezierPolyline(lwpoint *root,double delta);
extern lwpoint *FilletPolyline(lwpoint *Root, ads_point Corner, double Radius, BOOL Silent=TRUE, double MaxDist=HUGE);
extern lwpoint *FilletTwoPolylines(lwpoint **Root1, lwpoint **Root2, ads_point Corner, double Radius);
extern lwpoint *get_cross(lwpoint *,lwpoint *,int,int *);
extern lwpoint *add_offset_point(lwpoint *,lwpoint *,double,double,double,double,double,char *,double,int,int,int);
extern double deflection_angle(double *pnt1,double *pnt2,double *pnt3);
extern lwpoint *intersect4(lwpoint *,lwpoint *);
extern lwpolys *process_offset(lwpoint *lister,int closed,lwpoint *bound,int is_inside,int is_scmodel,int is_buffer=0,int is_debug=0);
extern void GrDrawPline(lwpoint *list,int color);
extern int Is3D(lwpoint *Poly);
extern lwpoint *ReadEntity(ads_name ename,int LineWorkOnly=TRUE,int UseText=0);
extern void FixIslands(lwpolys **Polys, char *Layer);
extern lwpoint *GetLowerLeftPoint(lwpoint *poly);
extern int IntersectVecPolyline(ads_point s,double angle,lwpoint *bound, ads_point PMinX,
                 ads_point PMaxX, ads_point pminX, ads_point pmaxX);
extern lwpoint *PolyClipRight(ads_point Point, double Angle, lwpoint *Panel);
/// Select
extern lwpolys* GetInclusions( BOOL bDoubleLines);
/// Creates a set of inclusion/exclusion polyline (doubled up if desired). First selection
/// set includes both inclusion and exclusion, second is just exclusion
extern lwpolys *GenerateInclusionPolys(ads_name combo, ads_name exc, BOOL bDoubleLines);
extern void scad_set_layer_state(const char *Layer, BOOL Thaw);
extern ScUniRecordBase *scad_get_layers();
extern void call_text_import(char *filer);
extern lwpoint* FindNearestCL( lwpolys* plwPolyCL, lwpoint* plwPointLot );
lwpoint *remove_self_crossing_loops(lwpoint *root);
extern long join_nearest(ads_name setname, acad_resbuf *head);
extern int FindLotCorners( lwpolys* plwPolyCL, lwpoint* plwPointLot, ads_point* pPts );
extern void EraseByPolylines(ads_name setname,ads_name ename_protect, lwpoint *inclu_root,lwpoint *exclu_root,int do_poly,int do_solid,int do_3dface,int do_line,int do_arc,
 int do_point,int do_text,int do_insert,int do_dim,int do_leader,int do_inside,int silent);

///checks if entity is of type ARC, CIRCLE, SPLINE or ELLIPSE
extern BOOL IsCurvEntity(ads_name ename);

#ifndef ICAD
  #ifdef ACAD04
    extern lwpolys *ReadMPolygon(ads_name ename,int IsCheckSpace);
    #ifdef ACAD07
      #ifdef ACAD10
        #ifdef ACAD13
          #pragma message("Using acmpolygonObj19.lib")
          #pragma comment(lib, "acmpolygonObj19.lib")
        #else
          #pragma message("Using acmpolygonObj18.lib")
          #pragma comment(lib, "acmpolygonObj18.lib")
        #endif
	  #else
        #pragma message("Using acdbmpolygon17.lib")
	    #pragma comment(lib, "acdbmpolygon17.lib")
	  #endif
    #else
      #pragma message("Using acdbmpolygon16.lib")
      #pragma comment(lib, "acdbmpolygon16.lib")
    #endif
  #endif
#endif

extern ScUniRecordBase *PolylineToUniRec(lwpoint *poly);
extern lwpoint *PolylineFromUniRec(ScUniRecordBase *PntRec);
extern BOOL change_entity_elevation(ads_name ename, double cval, int is_abs, int ignore0, int is_layer, char *layer_nm, int &do_regen);

#endif

#if defined(WIN32_WINDOWS) || defined(_WIN32_WCE) || defined(_CONSOLE)

#ifndef NOCRD
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
typedef struct note_pt {
  char buf[120];
  struct note_pt *next;
} note_pt;

typedef struct note_list {
  char id;
  long pntno;
  char pntno2[CRD_MAX_POINT_NAME];
  char gisfile[FILENMSZ];
  note_pt *list;
  struct note_list *next;
} note_list;

extern note_pt *new_note_pt();
note_pt *add_note_pt(note_pt *nroot,char *buf);
void free_note_pt(note_pt *nroot);
#endif
#endif

#endif

typedef struct dataList {
  double vector[5];
  struct dataList *next;
} dataList;

dataList *new_dataList();
dataList *new_dataList(double fVal0, double fVal1, double fVal2 = 0.0, double fVal3 = 0.0, double fVal4 = 0.0);
void free_dataList(dataList *pRoot);
dataList *copy_dataList(dataList *pData);
dataList *read_dataList(const char *szFile);
void write_dataList(const char *szFile, dataList *pData);
int _GetValueFromDataList(dataList *pList, double fT, double *fQ);
void _GetValueFromDataList(dataList *pList, double x, double &y, double &z);

// corehole definition, moved here RAL 07/30/04. Please make sure when you make changes to these structures and re-build
// corehole or makegrid projects to re-build SCADDB project also

#ifndef _MKGRIDPOINT
#define _MKGRIDPOINT

#ifndef NOCRD
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
typedef struct point_convert {
  int id;
  double vector[3];
  double bulge;
  double side;
  double dist;
  double radius;
  double chord;
  double dang;
  double center[2];
  char pntno2[CRD_MAX_POINT_NAME];
  char rpntno2[CRD_MAX_POINT_NAME];
  char desc[CRD_MAX_POINT_NAME];
  struct point_convert *next;
  struct point_convert *back;
} point_convert;

typedef struct lot_attr {
  char name[64];
  double val;
  int num;
  char buf[255];
  int attr_type;
  struct lot_attr *next;
} lot_attr;

typedef struct lot_pt {
  char pntno2[CRD_MAX_POINT_NAME];
  int is_radius;
  double orig_x;
  double orig_y;
  double orig_z;
  lot_attr *attr_list;
  struct lot_pt *next;
} lot_pt;

typedef struct lot_list {
  char name[64];
  char block[64];
  char type[64];
  char crdfile[FILENMSZ];
  double area;
  double station;
  double end_sta;
  int id;
  lot_pt *list;
  point_convert *list2;
  lot_attr *attr_list;
  lot_list *parent;
  char parent_name[64];
  char parent_block[64];
  struct lot_list *next;
} lot_list;

enum {LOT_REAL, LOT_NUM, LOT_CHAR, LOT_DOC};

extern void free_lot_attr(lot_attr *root);
extern void free_lot_pt(lot_pt *temper);
extern lot_attr *new_lot_attr();
extern lot_pt *new_lot_pt();
extern point_convert *new_point_convert();
extern point_convert *copy_point_convert(point_convert *pSrc);
extern point_convert *free_point_convert(point_convert *root);
#endif
#endif

class Vector3;
class SubEdge;
class MkGridPoint;

typedef struct mkgrid_point_list {
  MkGridPoint *pnt;
  int array_size;
  struct mkgrid_point_list *next;
} mkgrid_point_list;

// triangle
// Be sure to use NewTriangle() to create. 
typedef struct mkgrid_triangle {
  MkGridPoint *a;
  MkGridPoint *b;
  MkGridPoint *c;
  struct mkgrid_triangle *ab;          /* the triangle joined at the edge from */
  struct mkgrid_triangle *bc;          /* point a to point b*/
  struct mkgrid_triangle *ca;
  struct mkgrid_point_list *list;      /* a list of the points within the triangle */
  struct mkgrid_point_list *list2;     /* a list of the points within the triangle */ //makegrid.cpp only
  struct mkgrid_triangle *next;
  class SubDivPoint *pNewCenter;/* the center point of this triangle that will
                         subdivide the triangle.  Used only by DoSubdivision().*/
  SubEdge *m_pEdgeAB, *m_pEdgeBC, *m_pEdgeCA; //for subdivision by bisecting edges.  Search on SUBDIVIDE_BISECT_EDGES_POW2.
  char flags;
} mkgrid_triangle;

/* a point to be triangulated */
// Be sure to use NewPoint() to create.
class MkGridPoint {
public:
  //does not allocate any memory, just does basic initialization
  // by calling NewPoint().  This makes sure that local variables
  // are initialized, too.
  MkGridPoint();
  
  // allocates memory.  Must call DisconnectFromTriangles() to free memory.
  void ConnectToTriangle( mkgrid_triangle *pTri);
  void DisconnectFromTriangles();
  void DisconnectFromTriangle( mkgrid_triangle *pTri);

  int GetRing( MkGridPoint **apFoundPoints, const int iMaxPoints, ScQuadTree *pTree);

public:

  union {
    double vector[3];
    Vector3 v3;
  };
  struct mkgrid_point_list *barriers;  //used in tri4.cpp
  void *list;  // edge* and used as left in duplicates check; can be poly_list* or edge* in makegrid.cpp.  edge* in tri4.cpp.
  MkGridPoint *left;           // left & right ptrs for quick   //makegrid.cpp only
  MkGridPoint *right;          // checking for duplicate points //makegrid.cpp only
  MkGridPoint *next;
  mkgrid_triangle *in_tri;
  mkgrid_triangle **m_apTris;          //which triangles do I belong to?  Use ConnectToTriangle(), etc.
  //INT16 changed to long to get to compile, DSC
  long m_iNumTris, m_iMaxTris; //number of valid entries and maximum length.
  char generation;   /* used by DoSubdivision() */
  char bMarked;
  char id;  //seems to be like a flag and has initial value '0'.  '2' and '5' are two other values I've seen it set to. --Ron
            //used only by MakeGrid.cpp.
};
#endif

#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
#ifndef _CORHOLE_DEF
#define _CORHOLE_DEF

typedef struct desc_val {
  char *name;
  char *value;
  struct desc_val *next;
  struct desc_val *prev;
  long db_id;
} desc_val;

typedef struct attr_val {
  char *buf;
  char *name;
  double val;
  double composite_thk;
  short is_real;
  struct attr_val *next;
  struct attr_val *prev;
  struct lister_corehole *parent_c;	
  struct strata_list *parent_m;
  long db_id;
} attr_val;

typedef struct ch_list {
  ads_name ename;
  char layer[LAYERNMSZ];
  char name[89];
  char desc[89];
  char symbols[9][80];
  double size;
  // Two of these vectors hold the northing and easting entered by the user
  double vector[3];
  // These variables hold northing and easting that exist in the database
  // They will later be used in Update corehole and Insert corehole to ensure that the value
  // entered by the user is >0
  double database_northing;
  double database_easting;
  double key_density;
  int on; /* processing on or off */
  short core_type;
  short xy_qual;
  short z_qual; 
  struct lister_corehole *strata;
  struct ch_list *next;
  struct ch_list *prev;
  struct desc_val *desc_list;
  struct attr_val *key_list;
  struct attr_val *non_list;
  int source; /* 0 = new, 1=from dwg, 2=from db */
  long db_id;
  char db_name[255];
} ch_list;

typedef struct strata_def {
  char name[123];
  char full_name[MAX_PATH];
  char split1[79];
  char split2[79];
  char layer[123];
  char hatch[123];
  char target[241];
  char marker[241];
  int is_done;
  int is_marker;
  int is_target;
  int marker_level;
  int is_model;
  int is_backgnd;
  double scale;
  double density;
  double recovery;
  int is_key;
  int color;
  int is_draw;
  double ang;
  struct attr_val *attr_list;
  struct strata_def *next;
} strata_def;

typedef struct strata_list {
  double vector[3];
  double dip_dx;
  double dip_dy;
  double dip_dz;
  char name[79];
  char bed_name[FILENMSZ];
  char blk_name[FILENMSZ];
  int is_key;
  int is_composite;
  int valid;
  int is_split;
  strata_def *define;
  MkGridPoint *pt_list;
  MkGridPoint *pt_list1;
  MkGridPoint *pt_list2;
  struct ch_point *spot_vals;
  struct ch_point *fence;
  struct ch_point *pnts;
  struct attr_list *list;
  struct attr_val *attr_list;
  struct name_list *above;
  struct name_list *below;
  struct strata_list *next;
} strata_list;

typedef struct mgrid_ch_list {
  char name[89];
  char desc[89];
  int symbols[9];
  int done;
  double size;
  double vector[3];
  double hole_bottom;
  double key_density;
  short core_type;
  short xy_qual;
  short z_qual; 
  strata_list *strata; // for block models
  struct desc_val *desc_list;
  struct attr_val *key_list;
  struct attr_val *non_list;
  struct mgrid_ch_list *next;
} mgrid_ch_list;

typedef struct attr_list {
  char name[80];
  char grd_name[FILENMSZ];
  grid_head *grd_ptr;
  int flag;
  struct ch_point *pnts;
  struct attr_list *next;
} attr_list;

typedef struct name_list {
  struct strata_list *strata;
  struct name_list *back;
  struct name_list *next;
} name_list;

typedef struct point_corehole {
  double vector[3];
  double bulge;
  struct point_corehole *next;
} point_corehole;

typedef struct lister_corehole {
  char name[79];
  char bed_name[79];
  strata_def *define;
  int is_key;
  int is_done;
  int is_composite;
  int valid;
  double bottom_elv;
  double bottom_elv2; // adjusted for dip angle
  double dip;
  double azi;
  struct point_corehole *pt;
  struct attr_val *attr_list;
  struct lister_corehole *next, *prev;
  struct ch_list *parent;
  long db_id;
} lister_corehole;

typedef struct list_id {
  long id;
  struct list_id *next;
} list_id;
#endif
#endif

#ifdef CESCLIB
typedef struct field {
  char  *field_name;
  char  *value;
  double val2;
  int type;
  struct field  *next;
} field;

typedef struct upd_drill {
  char *name_database;
  long id_update;
  struct upd_drill *next;
} upd_drill;

enum {XDATA_HEAD=-3, XDATA_STR=1000, XDATA_APP=1001, XDATA_REAL=1040, XDATA_SHORT=1070, XDATA_LONG=1071};

#ifndef NOCRD
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
//MUST BE KEPT IN SYNC WITH \gislab\sclib.h.
//The hard-coded numbers reflect the LDD *.mdb format (probably). --Ron
typedef struct pctTrs {
  long pctid;
  double nor;
  double eas;
  double elv;
  char des[32];
  char id[10];
} pctTrs;
//MUST BE KEPT IN SYNC WITH \gislab\sclib.h.
//The hard-coded numbers reflect the LDD *.mdb format (probably). --Ron
typedef struct pctLdd {
  long pctid;
  double nor;
  double eas;
  double elv;
  char des[32];
  char id[10];
  struct pctLdd *next;
} pctLdd;

extern int	write_point_ldd(pctTrs *ptr, int typcrd, int protection);
extern int get_name_points_ldd();
extern int read_point_ldd(struct pctTrs *ptr);
extern int writeDrawingSetup(int mode);
extern int get_desc_points_ldd();
extern long get_max_point();
extern int get_size_desc();
extern int open_ldd_db(char *database_name, int create_new);
extern int close_ldd_db();
extern int get_database_name_ldd_project(char *mdb_filenm);
extern int get_database_name_ldd_project_meridian(char *mdb_filenm);
extern void get_no_prf();
extern int write_prf_pro(long prfid, char *pronm);
extern int read_prf_pro(long prfid, char *pronm);
extern int get_alg_cur(char *algnm, double *ststa, double *ensta);
extern int write_alg_cl(char *clnm);
extern int write_xls_cutsheets(char *xlsnm, char *srvpn, char *cut, char *fill, char *prpel, char *extel, char *dsc);
extern int write_cl_alg(char *clnm);
extern int get_low_hig_pct(long *low, long *hig);
extern int get_prj_path(char *prjnm);
extern int get_occ_pct_ldd(char *mdb_filenm);
extern int write_point_ldd_meridian(long pctId, char *pctIdAlf, char *pctDs, double pctEas, double pctNor, double pctElv, int isNew);
extern int get_low_hig_pct_meridian(long *low, long *hig);
extern int get_points_no_meridian(long *lpno);
extern int get_points_no(long *lpno);
extern int read_point_ldd_meridian(struct pctLdd *ptr);
extern int isMetricOrNot();
extern void acad_to_ldd_orientation(double *);
extern void ldd_to_acad_orientation(double *);
extern int wipeout_text(ads_name,double,ads_name,int is_group=0 CIPCSCADPARAM CIPCSCADPARAMDEFAULT);
extern int save_lots_db(char *mdbfile, int typemdb, int lotprotect, struct lot_list *ptr);
extern int read_lots_db(char *mdbfile, struct lot_list **ptr);
extern int free_lots_from_db(struct lot_list *ptr);
extern void get_lot_xdata(ads_name ename,char *id, char* block=NULL);
extern ScUniRecordBase *ReadCRDtoUniRec(CrdFile *pCRD,char *range,char *matchme);
#endif
#endif

#ifdef HAVE_CAD
//loads the DLL, if necessary.  Will give it correct path and extension (*.arx or *.dll).
//For example, int iResult = LoadArx( "tri4");
//Returns 1 if successful, 0 otherwise.
extern int LoadArx( const char *szDLL);
extern int UnloadArx( const char *szDLL);
extern int IsLoadArx( const char *arx_filenm);
#endif

#ifndef NODB
/// "flexible" DB access via DAO. Can use to open multiple databases, multiple tables within that database, and to
/// inspect all of the records of any particular table.
extern long open_db_flex( const char *database_name); //returns ID of open database. 0 if failure.
extern void close_db_flex( long id_database);
extern long open_table_flex( long id_database, const char *table_name, long *p_id_recordset); //returns ID of table and recordset
extern void close_table_flex( long id_table, long id_recordset);
extern void move_first_flex( long id_recordset); //moves record pointer to first record of the record set.
extern void move_next_flex( long id_recordset); //moves record pointer to next record of the record set.
extern BOOL get_record_flex( long id_recordset, const char *field_name, csString &strResult);
extern BOOL get_record_flex( long id_recordset, const char *field_name, double *pfResult);
extern BOOL get_record_flex( long id_recordset, const char *field_name, int *piResult);
extern BOOL get_record_flex( long id_recordset, const char *field_name, long *piResult);
/// New style DB access via ODBC layer
extern int open_db_odbc(char *mdb_filenm);
extern int search_table_db_odbc(char *table_name);
extern int make_table_db_odbc(char *table_name, const struct field *ptr);
extern int add_record_db_odbc(char *table_name, const struct field *ptr);
extern int close_db_odbc();
/// Older style DB access via DAO Arx
//extern int open_db(char *mdb_filenm);
//extern int search_table_db(char *table_name);
//extern int make_table_db(char *table_name, const struct field *ptr, int create_new);
//extern int add_record_db(char *table_name, const struct field *ptr);
//extern int close_db();
extern int exist_dao_install();
#endif

	// to get around AutoCAD's bug of needing a regen in between the user deleting an entity
	//  and us calling acad_entmod. --Ron 5/25/2004
#if defined(ACRXAPP)
  #undef acad_entmod
  #define acad_entmod scad_entmod
  extern int scad_entmod(struct acad_resbuf *);

  #undef acad_entdel
  #define acad_entdel scad_entdel
#endif

#ifdef LDD
  #undef acad_getpoint
  #undef acad_entmake
  #undef acad_entget
  #undef acad_entgetx
  #undef acad_command
  #undef acad_grdraw
  #define acad_getpoint scad_getpoint
  #define acad_entmake scad_entmake
  #define acad_entget scad_entget
  #define acad_entgetx scad_entgetx
  #define acad_command scad_command
  #define acad_grdraw scad_grdraw
  extern int scad_getpoint(const double *,const char *,double *);
  extern struct acad_resbuf *scad_entget(ads_name);
  extern struct acad_resbuf *scad_entgetx(ads_name,struct acad_resbuf *);
  extern int scad_entmake(struct acad_resbuf *);
  extern int scad_command(const int, ...);
  extern int scad_grdraw(const double *,const double *,int,int);
#endif
#if (defined HAVE_CAD)
  //redefine acad_load_dialog as scad_load_dialog for translation
  #undef acad_load_dialog
  #define acad_load_dialog scad_load_dialog
  extern void TranslateDCL(char *dclfile);
  extern int scad_load_dialog(const char* dclfile, int* dcl_id);
  extern void InitTranslationOfficeMnu(); //initializes menu translation
  extern void InitTranslationOfficeMnuHelp(); //initializes menu help string translation
#endif
extern void InitTranslationOffice(); //initializes cpp translation
extern void InitTranslationPointCloud(); //initializes translation for pointcloud
/* globals for crd points */
#define CHKMAXPN (99999+1)  //this is first illegal point number in numeric CRD format.
                            //99999 is highest legal # in SCPoint.cpp.
//#define NO_OLD_CRD  //remove this when the conversion has been completed.
#if !defined(NO_OLD_CRD)
//this group of functions uses the old CRD API
extern int opencrdfile(char *);
extern int makeheader();
extern short putcrdpoint(long,char *);
extern long chkifused(long,char *,int *,int);
extern int do_list_points_dialog(char *);
#endif //!NO_OLD_CRD

class CrdFile;
class CrdPoint;

// Deletes all points whose names are strictly greater than szMaxName.  In some
//  CRD file formats, increasing the maximum point name increases the file size.
// Returns TRUE if successful.
BOOL SetMaxPointName( CrdFile *pCrd, const char *szMaxName);

// Prompts the ACAD user to either pick a location in world coordinates using
//  the cursor or to name a point in the CRD file.  In either case, if successful,
//  pResultPoint will contain the resulting x,y,z and TRUE will be returned.
// If the user cancels, FALSE will be returned.
// pRubberBandStart is optional.  If given, a "rubber band" box will be drawn
//  between the rubber band point and the cursor point.
int PromptForPoint(CrdFile *pCrd, char *crd_filenm,const double* pRubberBandStart, const char* szPrompt, double* pResultPoint, char* szInit=NULL);

// uses acad_printf to display the contents of the given CrdPoint.
void displaypt( const CrdPoint *pCrdPoint);

// Creates a temporary arrow pointing to the given location and (optionally) zooms/pans
//  to include it in view, if is_zoom is 1.
// Call delpreviewpnt() to erase it.
void previewpnt(ads_point loc, int is_zoom = 0, int color = -1);

// Deletes a temporary arrow that was created with previewpnt() at the given location.
void delpreviewpnt( ads_point loc);

// old.  consider using CrdFile::PromptForPoint().
extern int getscpnt(double *rubber_pt,char *prompt,double *res_pt, CrdFile *pCRD=NULL, char *keyword_buf=NULL,char *ptnm=NULL);

extern int do_list_points_dialog(CrdFile *pCrd, char *);
extern int parse_pntno(char *,char *);
extern int do_in_range(const char *szPointName, const char *szRange);
extern int range_in_range( const char *szRange1, const char *szRange2); //does range 1 overlap range 2?
extern void TruncateIntegerDigits( csString &num, int max_integers);
extern void increment_pntno(char *,long); //use CrdFile::IncrementPointName() instead.
extern void get_point_id_at_position(double *pnt,char *name,CrdFile *pCrd);
extern void get_crdfile_dictionary(char *,int);
extern void set_crdfile_dictionary(const char *,int *);
extern void attach_ptnm(const char *pntnm,const char *crdfile,const char *desc,BOOL bNonContourable=FALSE);//attaches to last entity
extern void attach_ptnm2(const char *pntnm,const char *crdfile,ads_name ename,const char *desc,BOOL bNonContourable=FALSE);
#ifdef ACRXAPP
  extern BOOL read_ptnm_xdata( AcDbObject *pObj, csString *pCrdFileStr, csString *pNameStr, csString *pDescStr, BOOL *pbNonContourable);
#endif
extern BOOL read_ptnm_xdata( ads_name ename, csString *pCrdFileStr, csString *pNameStr, csString *pDescStr, BOOL *pbNonContourable);
extern void AppendPointNumberToRange( csString *pRange, long iNewNum);
extern void AppendPointNumberToRange( csString *pRange, const char *szName);
extern int check_pt_group();
// Checks 'szOrigRange' for the "group" and "group[name]" keywords. "group", by itself, will cause
//  a dialog to pop up asking the user to choose a point group. "group[name]" will lookup the group
//  definition. Either way, pStrResultRange will be filled in with the range of point #s.
// szOrigRange and strResultRange can be the same memory.
// Returns TRUE if the range was successfully parsed and pStrResultRange contains valid data.
// Returns FALSE if a DLL is missing, or if the Lisp var "crdfile" wasn't found, or if the user
//  cancelled. pStrResultRange in this case will not have been changed.
// Note: the current crd file must not currently be open!
extern BOOL convert_group_to_range( const char *szOrigRange, csString *pStrResultRange);
// Brings up a dialog box asking the user to choose one or more point groups. The list of point groups will
//  be placed into pStrResultGroup. Returns TRUE if successful and FALSE if the user cancelled or there was
//  a problem. The current crd file, specified by the Lisp variable "crdfile", must not already be open.
extern BOOL pick_group( csString *pStrResultGroup);
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
#if defined (HAVE_CAD) && !defined (NOCRD)
//
// Prints the prompt and then gets a selection set of Crd Points from the drawing and returns them, sorted by point name/id/number.
/*
 * Modified to optionally return an ScSSet of selected entities per request of DSC, to avoid the dubious subsequent
 * use of ads_ssget("P"...) by clients of this function.
 * @author C.Petersohn April, 2008
 */
extern pctLdd *ssget_points(char *prompt, CrdFile *pCRDFile = NULL, ScSSet* pScSSet = NULL);
#endif
extern void get_history_filenm(char *note_file,const char *crd_file);
#ifndef NOCRD
  extern void get_coordinate_index_filenm(char *note_file,const char *crd_file,enCRDType enType);
#endif
extern struct note_list *new_note_list();
extern struct note_list *read_notes(char *);
extern struct note_list *add_note(struct note_list *,char *pt_nm,char *buf,char *gis_file=NULL);
extern void remove_empty_trailing_notes(note_list *nroot);
extern void save_notes(char *,struct note_list *);
extern void free_notes(struct note_list *);
extern void message_out(char *,int); // write_output
extern int OpenNADConverter();
extern void CloseNADConverter();
extern int ConvertNAD27ToNAD83(double,double,double *,double *);
extern int ConvertNAD83ToNAD27(double,double,double *,double *);
extern "C" BOOL SMItoCRD(char *smifile, char *cgfile, int overwrite_flag);
extern "C" BOOL SMIfromCRD(char *cgfile, char *smifile);
#endif
#if (defined(WIN32_WINDOWS) || defined(_WIN32_WCE))
//VC++ 4.2 had 1 byte alignment by default.  VC++ 6.0 has 8 byte alignment by default.
//All these structures are stored in files and must be one-byte aligned.
#pragma pack( push, 1)

#ifndef _CRD_SC_IO_H_ //to avoid conflict with CRD API which defines these same structures
//header of alphanumeric version of SurvCADD CRD file format.
struct header {
  double id;
  char date[32];
  char des[32];
  char format[32];
};
//header as well as point data of numeric version of SurvCADD CRD file format.
struct pntlist {
  double nor;     //header: the last point's ID/name.
  double eas;     //header: job #  (display using %.3f)
  double elv;     //header: job date (display using %4.4f)
  char des[32];   //header: job description
};
//point data of alphanumeric version of SurvCADD CRD file format.
struct pntlist2 { 
  double nor;
  double eas;
  double elv;
  char des[32];
  char id[10];
};
#endif //!_CRD_SC_IO_H_

#pragma pack( pop)

#endif
#endif

#ifndef UNDER_CE
#pragma pack( push, 1)
#endif

class ScTIN;
class TriVolumeOptions;

class ScWireFrame:public ScShape {
public:
  ScWireFrame();
  virtual ~ScWireFrame();
  ScListLong *Tris;
  BOOL DistinctLines;
  void AddPolyline(lwpoint *);
  void AddShape(long);
  void Init(ScModel *Parent, lwpoint *poly, double TopElev, double Thick);
  void Init(ScModel *Parent, lwpoint *poly1, lwpoint *poly2,int is_tin_ends,int is_counter_clockwise);
  void OrderFaces(ScModel *Parent,int RetainStartNormal);
  double Volume(ScModel *Parent);
};

#ifndef UNDER_CE
#pragma pack( pop)
#endif

#ifdef HAVE_CAD
//Classes ScEName and ScSSet replace ads_name and can be used for situations where
//the AutoCAD limit of 255 entity selection sets is unacceptable.
//Michael Burenkov, created 04/05/2004
class ScENameS {
protected:
	CS_LONG_PTR name1, name2;
public:
	ScENameS ();
	ScENameS (const CS_LONG_PTR, const CS_LONG_PTR);
	ScENameS (const ads_name &);
	void getName (ads_name &) const;
	void getName (CS_LONG_PTR &, CS_LONG_PTR &) const;
	void setName (const ads_name &);
	void setName (const CS_LONG_PTR, const CS_LONG_PTR);
	bool operator == (const ScENameS&) const;
	bool operator != (const ScENameS&) const;
	bool operator < (const ScENameS&) const;
	bool operator == (const ads_name&) const;
	bool operator != (const ads_name&) const;
};

class ScEName : public ScENameS {
private:
	ScEName *nname;
public:
	ScEName ();
	ScEName (const CS_LONG_PTR, const CS_LONG_PTR);
	ScEName (const ads_name &);
};

class ScSSet {
private:
	ScEName *sset;
	long items, size;
public:
	ScSSet (const long = 0);
	ScSSet (const ads_name &);
	~ScSSet();
	void reserve (const long);
	long length ();
	void getName (const long, ads_name &);
	void addName (const CS_LONG_PTR, const CS_LONG_PTR);
	void addName (const ads_name &);
	void removeName (const long);
	void removeName (const ads_name &);
	void getSSet(ads_name& rads_nameSet, const bool kbCreateSet = true);		// Modified signature for optional boolean +CRP [March, 2008].
};
#endif

extern void tin_error_guy(char *buf);
extern int is_tin_file(const char *filenm);
BOOL DrawTriangle(ScTIN *Tin, long Tri, int iColor = -1, BOOL bClockwiseTriangles = TRUE);//creates 3d face entity
BOOL DrawTriangle(ScTIN *Tin, long Node1, long Node2, long Node3, int Color);//creates 3d face entity
void DrawTriangle(ads_point pnt1, ads_point pnt2, ads_point pnt3, int iColor = -1, BOOL bInOrder=TRUE);//creates 3d face entity
int Draw3DFaces( ScTIN *Tin, BOOL bClockwiseTriangles = TRUE, ads_name ssTris = NULL, int color = -1);//creates 3d face entities of all triangles
//creates 3d face entity if pnt4=NULL use only 3 points
int Draw3DFace(double* pnt1, double* pnt2, double* pnt3, double* pnt4=NULL, int iColor = -1);

class SurfaceMacro {
public:  
  SurfaceMacro();
  ~SurfaceMacro();
  BOOL RunFileMacro(const char *Fname);
  BOOL RunScriptLine(long Line, const char *Str,ScList *VarList=NULL);
  void AddMessage(int Level,const char *);
  ScList *GetOperands() {return FE->GetOperands();};
  ScList *GetMessages() {return FE->GetMessages();};
  SurfaceFormulaEvaluator *FE;    
#ifdef WIN32
  HWND ParentWindow;
#endif
};
  
void DrawTIN(ScTIN *Tin); //draws as temporary linework (acad_grdraw)
void DrawVoronoi(ScTIN*);
int InsertPoint(lwpoint*, lwpoint*, ads_point);
/* Crust and Skeleton extraction */
enum DP_MODE { DP_AVERAGE, DP_EXACT };
void DensifyPolylines (lwpolys*, double, DP_MODE);
/* Tin densification */
void DensifyTIN (ScTIN*);
enum ECS_MODE { CRUST, SKELETON, SSKELETON };
ScTIN* ExtractCrustSkeleton (ScTIN*, ECS_MODE, BOOL reduced);
typedef struct {
  bool valid;
  int count;
  double length;
  double extend;
} BranchInfo;
void InterpolateRidgeValley(ScTIN*, ScTIN*, BranchInfo);
void InterpolateSummitPit(ScTIN*, ScTIN*);
void MergeDelaunaySkeleton(ScTIN* delaunay, ScTIN* skeleton, BOOL insert = TRUE, BOOL remove = TRUE, double Scaler = 1.0);
extern void get_tin_stats(ScTIN *tin1,double *min_z_val,double *max_z_val,double *min_x_val,
            double *max_x_val,double *min_y_val,double *max_y_val);
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
extern void report_tin_stats(FILE *fp,ScTIN *tin1,char *filer,double min_z,double max_z,double min_x,
            double max_x,double min_y,double max_y);
#endif
void label_slope_pt(double *new_pt,double *norm,double text_size,int is_label,int is_arrow,int is_uphill,char *buf,
 int is_zone,double val,double hscale,double *zone_list,int zone_max,int *color_list,double *scale_list,char layernm_list[70][32]);
int tin_slope_at_point(ScTIN *refTin,ads_point temp,double up_down,double *vec0,double *vec1);
void draw_tin_slopes(ScTIN *refTin,double min_area,int slope_type,double text_size,int is_label,int is_arrow,int is_uphill,
 int is_zone,double hscale,double *zone_list,int zone_max,int *color_list,double *scale_list,char layernm_list[70][32],int places,
 char *pre_buf,char *post_buf);

#ifndef UNDER_CE
typedef struct hatch_type_def {
  int hatch_idb;
  char hatch_type[16];
  int is_acad;
} hatch_type_def;
#ifdef TAKEOFF
  #define NUM_HATCH_TYPE 55
#else
  #ifdef ICAD
    #define NUM_HATCH_TYPE 186
  #else
    #define NUM_HATCH_TYPE 237
  #endif
#endif
extern hatch_type_def hatch_type[NUM_HATCH_TYPE];
extern int SelectSymbol(char *symbol_name);
#endif

extern int GenerateTIN(ads_name ss, ads_name inc, ads_name exc, ads_name reg, char *fname, char *options, ScTIN** ret = 0);
/// Colors TIN by using TRG file with layer colors and region logic
extern BOOL ColorTinByLayerTarget(ScTIN *Tin, char *fname);
extern void AdjustFlatTriangles(ScTIN *surfaceTIN);

typedef struct circle_center {
  double center_sta;
  double center_elev;
  double radius;
  double vc;
  double left_sta;
  double left_elev;
  double right_sta;
  double right_elev;
  int is_up;
} circle_center;

//station equation for centerline file
typedef struct sta_equation {
  double sta_back;
  double sta_ahead;
  struct sta_equation *next;
} sta_equation;

typedef struct duplicate_sta {
  double station;
  struct duplicate_sta *next;
} duplicate_sta;

typedef struct cline {
  double x;
  double y;
  double station;
  double delta_ang;
  double radius;
  double center[2];
  double pi[2];
  double pi_station;
  double tang_in;
  char pntno[11];
  char pntno2[11];
  ads_name line1;
  ads_name line2;
  ads_name line3;
  int is_spiral;
  int spiral_type;
  struct cline *next;
} cline;

typedef struct doubleList {
    double theVal;
    double theVal2;
    double theVal3;
    char szID[MAX_SWR_NAME];
    char szSystem[MAX_SWR_NAME];
    char szPipe[MAX_SWR_NAME];
    struct doubleList *next;
} doubleList;


typedef struct profile_list {
  double sta;
  double elev;
  double elev2;		// used for sewer profile
  double invertInDirection;
  double invertOutDirection;
  doubleList *invertList;
  double vc1;
  double vc2;
  double radius;
  double width;     // pipe size, ft or m
  double width2;    // pipe width, ft or m
  double step;
  double ref_sta;
  double ref_sta2;
  double ref_offset;
  double ref_elev;
  double ref_dist;
  double mh_elev;
  double mh_top;
  double mh_top1;
  double mh_top2;
  double mh_bottom;
  double mh_bottom1;
  double mh_bottom2;
  double mh_offset;
  double mh_sump;
  double mh_drop_invert;    // ft or m
  double taper_hgt;
  double pipe_thickness;    // in or mm, for sewer/pipe profile
  double pipe_thickness2;   // in or mm, for pipe crossing profile
  double pipe_flow;
  double pipe_travel_time;
  double pipe_flow_depth;
  double pipe_velocity;
  double swr_cradle_above;  // cradle depth above the pipe, in or mm
  double swr_cradle_below;  // cradle depth below the pipe, in or mm
  int taper_mode;
  int pipe_shape;           // 0: Cicular, 1: Box, 2: Horizontal Ellipse, 3: Vertical Ellipse
  int crossIndex;
  int manholeIndex;
  char mh_id[MAX_SWR_NAME];
  char pipe_id[MAX_SWR_NAME];
  char pipe_material[MAX_SWR_NAME];
  char szStructID[MAX_SWR_STRUCT_ID];
  char desc[32];
  char desc2[32];   // to store system name for pipe crossings.
  struct profile_list *next;
} profile_list;

// vertical speed table
typedef struct VstData {
  double speed;
  double dist;
  double K;
  VstData *next;
} VstData;

typedef struct VstTable {
  char vstName[123];
  VstData *vstData;
  VstTable *next;
} VstTable;

// profile edit/input
enum {GEN_PROF, ROAD_PROF, SEWER_PROF, PIPE_PROF, XING_PROF, CIRC_PROF};
doubleList *new_doubleList();
void free_doubleList(doubleList *pRoot);
doubleList *copy_doubleList(doubleList *pRoot);
extern profile_list *new_profile_list();
extern profile_list *sc_read_profile(char *filer, int check_sewer, int *profile_type, char *pszProfName = NULL);
extern void sc_write_profile_list(profile_list *pro_head,int pro_type,char *edfil);
double sc_elev_at_sta(double sta,profile_list *plist,double voff,int is_allow_extrap=1,int is_debug=0);
double sc_elev_at_sta(double sta, profile_list *pList, int profile_type, int is_range_check=0);
double sc_slope_at_sta(double sta, profile_list *pList, int profile_type);
void get_road_cl_tin_filenm(char *cl_nm,char *profil,double *date,char *surf_nm,char *speed_nm);
int calc_profile_sag_crest(profile_list *temp,profile_list *proot,double *sta_val,double *elv_val,char *buf);
extern void free_profile_list(profile_list *);
profile_list *ReverseProfileList(profile_list *root);

double sc_calc_vc(double sd, double sta1, double elv1, double sta2, double elv2, double sta3, double elv3,
				  double tob, double eyeht, int use_kval);
double sc_calc_sd_kval(double vc, double sta1, double elv1, double sta2, double elv2, double sta3, double elv3,
					   double tob, double eyeht, int is_kval, int vc_method);

int sc_get_profile_center(circle_center *center, double sta1, double elv1, double sta2, double elv2, 
						  double sta3, double elv3, double radius) ;
double my_round(double value, double interval, int up);
void ptcpy(double *dest, double *src);
bool isNum(char *line);

//centerline edit/input
double arcsin(double);
extern cline *new_cline();
extern cline *copy_cline_list(cline *pRoot);
extern void free_cline_list(cline *pRoot);
extern lwpoint *sc_make_cl_polyline(cline *,int allow_spiral_arc=1);

extern lwpoint *sc_make_spiral_chord(ads_point pt1, ads_point pt2, double delta, double radius, ads_point end_pt,int is_pts);
extern lwpoint *sc_make_spiral(ads_point pt1, ads_point pt2,double delta,double radius,double *end_pt,int is_pts,
                               double *check_pnt=NULL,double *check_radius=NULL,double *check_tang=NULL,int allow_spiral_arc=1);
extern lwpoint *sc_make_spiral2(ads_point start_pt,double tang_in,double len_s,double delta,double radius_in,
 					            double radius_out,double *end_pt,double *tang_out,int is_pts,double *check_pnt=NULL,double *check_radius=NULL,double *check_tang=NULL,int allow_spiral_arc=1);
extern lwpoint *location_at_station(lwpoint *list,double start_sta,double sta,int closed,double *norm,
 int is_3d,int is_rr,int is_insert,double *loc_radius=NULL,int *is_right=NULL,int *is_corner=NULL);
extern int location_sta_offset(lwpoint *list,ads_point pt,int closed,double limit,double *cl_pnt,double *sta_off,int is_rr,
 int is_legacy_offset=1,lwpoint **cl_ptr=NULL,int skip_mode=0,int is_3d=0);
#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
extern cline *read_cl_file (const char *input_filer);
void write_cl_file(char *clfil,cline *cl_list,int side,sta_equation *eroot);
extern double polyline_to_cl_file(lwpoint *root,char *filer,double start_sta,int is_update_dwg=0,int is_screen_report=0);
extern void show_polyline_direction(lwpoint *cut,int is_closed);
#endif
extern void set_centerline_xdata(ads_name cl_ename,char *filenm,int is_erase,int is_attach_reactor=1);
extern void get_centerline_xdata(ads_name ename,char *filenm);
extern sta_equation *read_sta_equation(const char *szCLFile, cline* pCline=NULL);
extern void free_sta_equation(sta_equation *);
void free_cline(cline *);
void remove_trailing_zeroes(char* string);
extern void addstaplus(char *,const char *,double=50.0, int = 0);
extern double sta_atof(const char *);  
extern double latlon_atof(const char *);  
int FindTangentsIntersect(lwpoint *, lwpoint *, ads_point);

// section file (sct) routines
typedef struct offset_rec {
  double offset;
  double elev;
  double old_offset;
  double old_elev;
  int done;
  int id;
  double x;
  double y;
  char name[32];
  struct offset_rec *next;
  struct offset_rec *back;
} offset_rec;

typedef struct ew_area_rec {
  int id;
  double cut;
  double fill;
  double muck;
  double rehandle;
  double left;
  double right;
  double moved;
  double width;
  double height;
  double area;
  double elev;
  ads_name ename;
  lwpoint *list;
  struct ew_area_rec *next;
} ew_area_rec;

typedef struct sct_list {
  double station;
  double overlay;
  double cut;
  double fill;
  offset_rec *list;
  ew_area_rec *alist;
  struct sct_list *next;
} sct_list;

typedef struct sct_list2 {
  double station;
  offset_rec *left_list;
  offset_rec *right_list;
  struct sct_list2 *next;
} sct_list2;

extern int write_sct_file(const char *filer,sct_list *list);
extern void write_sct_file2(const char *filer,sct_list2 *list);
extern sct_list *read_sct_file(char *filenm,int use_subgrade,int use_point=1);
extern sct_list *new_sct_list();
extern offset_rec *new_offset_rec();
extern offset_rec *copy_offset_rec(offset_rec *oroot);
extern ew_area_rec *new_ew_area_rec();
extern void free_sct_list(sct_list *);
extern void free_offset_rec(offset_rec *);
extern double elev_at_offset(offset_rec *list,double offset);
// mode 1=return section elevation, else return delta z
extern double is_above_sct(double sta,double offset,double elev1,sct_list *sroot,int mode);
extern offset_rec *add_offset_list(offset_rec *oroot,offset_rec *onew);
extern sct_list *add_section(double this_sta,sct_list *sroot,int is_desc,profile_list *proot=NULL);
extern offset_rec *adjust_subgrades(offset_rec *oroot);
extern void merge_subgrade(lwpoint *rooter,lwpoint *head,double *cut_data);

// road template data
extern void draw_rotate(double *,double *,double,double,double);
extern double get_vc_elev(double,double,double,double,double,double,double,double,double,int is_debug=0);
extern double get_cir_elev(double,circle_center *);
extern int profile_road_sag_crest(double sta1, double elv1, double sta2, double elv2, double sta3, double elv3,
								  double vc1, double vc2, double *pnt);
	// find the sag/crest point of vertical curve at the PVI(sta2, elv2)
extern int profile_cir_sag_crest(double sta1, double elv1, double sta2, double elv2, double sta3, double elv3,
								  double radius, double *pnt);
	// find the sag/crest point of circular curve at the PVI(sta2, elv2)

csString UnitsToString (double decimal_inches, BOOL f_ShowEmptyArchFields = FALSE, int decimal_places = 3, BOOL f_FormatWithSpace = TRUE);
double StringToUnits(const char *FeetInchString);
BOOL IsDrawingArchitectural ( void );
double to_metric(double,int);
double from_metric(double,int);
double from_usfeet(double,int);
double from_intlfeet(double,int);

/// Returns the units of an autocad drawing.
/// 1 = Scientific
/// 2 = Decimal (NB: This is the default return value if HAVE_CAD isn't defined or some other problem with getting the units exists)
/// 3 = Engineering (feet and decimal inches)
/// 4 = Architectural (feet and fractional inches)
/// 5 = Fractional
int GetDrawingUnits ();

//changed length to 512 to let saving of filenames. LE
typedef struct note_buf_scad {
  char buf[512];
  struct note_buf_scad *next;
} note_buf_scad;
#ifndef EMULATOR
#define note_buf note_buf_scad
#endif

#ifndef NOSEWER
#if (!defined(UNDER_CE)) || defined(CE_OTHER)
#if defined(WIN32_WINDOWS) || defined(_WIN32_WCE) || defined(CE_OTHER)

int update_swrnet_centerline(const char *clfil);
note_buf_scad *read_sewer_dictionary(int FieldID,char *swrfile,char *surfnm);
void read_sewer_config(int &is_link_swr_ref,int &swr_ref_link_mode);

#endif
#endif
#endif

typedef struct tpl_subgrade {
  char hoffset[33];
  char dist[33];
  char material[33];
  char low_pivot[33];
  char high_pivot[33];
  int is_current;
  int low_type;
  int high_type;
  double wrap_hgt;
  double tie_slope;
  double low_slope;
  double high_slope;
  double low_slope3;
  double high_slope3;
  double low_slope2;
  double high_slope2;
  int slope_type;
  int unit_mode;
  double slope_amt;
  double voffset;
  int tie_mode;
  int direction;
  struct tpl_subgrade *next;
} tpl_subgrade;

typedef struct median_pt {
  double hoffset;
  double voffset;
  double old_hoffset;
  double old_voffset;
  double hfactor_dz; // template point profile adjust factors
  double vfactor_dz;
  double hfactor_dx; // template point centerline adjust factors
  double vfactor_dx;
  int grade_in;
  int grade_out;
  struct median_pt *next;
  struct median_pt *back;
} median_pt;

typedef struct tpl_median {
  char name[33];
  median_pt *list;
  int side;
  int is_skip;
  struct tpl_median *next;
} tpl_median;

#define MAX_CURB_TABLE 16

typedef struct tpl_curb {
  double cbtop;
  double cbtapr;
  double cbhgt1;
  double cbhgt2;
  double cbdrop;
  double cbbase;
  double cbbreak;
  double cbwidth;
  double cbradius1;
  double cbradius2;
  double crown_table[MAX_CURB_TABLE];
  double base_table[MAX_CURB_TABLE];
  double gutter_table[MAX_CURB_TABLE];
  char materl[33];
  char name[33];
  int is_left;
  int cbtype;
  int cbunit;
  int round;
  int intgrl;
  int base_only;
  int slope_type;
  double slope_amt;
  struct tpl_curb *next;
} tpl_curb;

typedef struct tpl_surface {
  double hoffset;
  double voffset;
  tpl_curb *curb;
  tpl_median *median;
  int is_current;
  int past_pivot;
  char name[33];
  double dist;
  double slope;
  double old_dist;
  double old_slope;
  int is_ratio;
  int is_parabolic;
  struct tpl_surface *next;
} tpl_surface;

extern tpl_median *new_tpl_median();
extern median_pt *new_median_pt();
extern tpl_surface *new_tpl_surface();
extern tpl_subgrade *new_tpl_subgrade();
extern tpl_surface *copy_tpl_surface(tpl_surface *src);
extern tpl_subgrade *copy_tpl_subgrade(tpl_subgrade *src);

#if (defined(CESCLIB)) && (defined(UNDER_CE))
extern "C" {
#include "cecompat.h"
}
#endif

struct acad_resbuf *add_report_xdata(struct acad_resbuf *xhead,char *fbuf);
///these functions encapsulate two major blocks of Tri Volume calculations.
///see sctopo.cpp and contour4.cpp for example
extern BOOL TriVolumeReport(ScTIN *pTinNew, TriVolumeOptions *TriOpts, char *szFileReport,double *strata_val,char *strata_filer,lwpolys *pInclusion);

/// Creates a list of one or more resbufs that contain the given string.
/// The list of resbufs will be appended to pBuf (unless pBuf is NULL).
/// The head of the whole linked list (which is pBuf, unless pBuf is NULL)
///  is returned.
/// Use acad_relrb to free the acad_resbuf.
struct acad_resbuf* StringToResbuf( const char *szSource, struct acad_resbuf *pBuf = NULL);

/// pBuf must point to the first acad_resbuf that was created by StringToResbuf().  pBuf
///  is not modified in any way, but it can't be const because the return value shouldn't
///  be const.
/// Fills in *pStr with the result of concatenating all of the resbufs that resulted
///  from the original call to StringToResbuf().
/// Returns a pointer to the acad_resbuf after the string.
/// Returns NULL if pBuf was not created by a call to StringToResbuf().
struct acad_resbuf* ResbufToString( struct acad_resbuf *pBuf, csString *pStr);

/// creates a string list out of a resbuf.  It will only return items that of type RTSTR.
csStringList ResbufToStringList (const struct acad_resbuf* in_rb);

#ifdef _WIN32_WCE
  void remove(char *path);
#endif

// the supported formats of the Point Graphic/Drawing API
// NOTE: I moved this here because I needed to use these definitions outside of ACAD - Burke
enum enPointDrawFormat
{
  // PDF == "Point Draw Format" to save 12 characters on each enum's name!
  enPDF_Default    = 0, //means that the "global" PointDrawMgr::SetGlobalDefault() determines the type.
  enPDF_SurvCADD   = 1,
  enPDF_LDD        = 2,
  enPDF_C3D = 3,
  enPDF_CG = 4
};
#if defined(HAVE_CAD ) || defined(NO_ACAD)
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Point Draw API for reading and writing points to an AutoCAD drawing.
//   The name "Point DWG" was used in scadutil.cpp already, so I chose this
//   name instead.
//
// Original author: Ron Little
//
// How to use, simple example:
/*
  //1.  Declare a manager and point.  The CRD filename will be found through Lisp
  //    variable 'crdfile'.  The use of reactors will be read from Lisp's 'linkon1'.
  PointDraw pointDwg;
  PointDrawMgr mgr;

  //2.  Initialize with the minimum information necessary.  All other settings are
  //    read from global defaults.
  pointDwg.SimpleInit( szName, szDesc, loc);

  //3.  Draw it!
  mgr.Draw( &pointDwg);
*/
// How to use, full-featured example:
/*
  //1.  Declare a manager and point.  Use the default format; see POINT_DRAW_DEFAULT.
  PointDraw pointDwg;
  PointDrawMgr mgr;

  //2.  Initialize the manager with additional information, if necessary.
  mgr.SetCrdFile( crdfile, bUseReactor);
  mgr.SetSubFormat( enSubFormatSCF2F);  //Field-to-Finish has special "subordinate" format

  //3.  Set the symbol portion, if necessary.
  pointDwg.SetSymbol(
    def_curr->symbol, // name of symbol (e.g., "spt0")
    pnt,              // insertion point
    sizer,            // x scale factor
    sizer2,           // y scale factor
    twang2);          // rotation angle

  //4.  Set the block attribute portion, if necessary.
  pointDwg.SetBlock(
    buf2,                      //name of block
    pnt,                       //insertion point
    txtsize, txtsize, twang2,  //x-scale, y-scale, twist angle
    buf, szRealName, &buf[200], &buf[100], //display name, real name, elevation, description
    szPointNoLayer, szElevationLayer, szDescLayer, //layers for point name, elevation, description
    szPointEntityLayer);        //for point entity

  //5.  Draw it!
  mgr.Draw( &pointDwg);
*/

// the supported formats of the Point Graphic/Drawing API
// NOTE: I have commented this out and moved a copy of it above the 
//             #if defined(HAVE_CAD ) || defined(NO_ACAD)
//        line above here because I needed to use these definitions outside of ACAD - Burke
/*enum enPointDrawFormat
{
  // PDF == "Point Draw Format" to save 12 characters on each enum's name!
  enPDF_Default    = 0, //means that the "global" PointDrawMgr::SetGlobalDefault() determines the type.
  enPDF_SurvCADD   = 1,
  enPDF_LDD        = 2,
  enPDF_C3D = 3,
  enPDF_CG = 4
};*/

// the supported sub-formats of the Point Graphic/Drawing API.
// For example, SurvCADD's Field-to-Finish has special needs and there
//  doesn't seem to be an easy to generalize this.
enum enPointDrawSubFormat
{
  enSubFormatUnknown    = 0, //means "no special sub-format".  Try to avoid special formats!
  enSubFormatSCF2F      = 1, //SurvCADD's Field to Finish; needed in order to identify F2F-made entities.
};

// what format is used by default if none is specified at run-time?
// The "global"
#ifdef CIVIL3D
#define POINT_DRAW_DEFAULT enPDF_C3D

#else
#ifdef LDD
  #define POINT_DRAW_DEFAULT enPDF_LDD
  #define LDD_COM_ADD_COGO  //enables COM interface, rather than DXF codes, for drawing LDD points.
#else
  #define POINT_DRAW_DEFAULT enPDF_SurvCADD
#endif
#endif

enum enPointDrawErr
{
  enPointDrawErr_OK      = 0,  //operation was successful; no error.
  enPointDrawErr_Unknown = 1,  //operation failed; unknown error
  enPointDrawErr_UnknownFormat = 2,  //selected entity has unknown format or current point draw format is not handled (internal error)

  // errors when querying drawing:
  enPointDrawErr_NoPointInfo =3,  //the named entity had no point information, e.g., no AutoCAD INSERT.
  enPointDrawErr_PointMissing = 4,//the desired point doesn't exist in the drawing
  
  // when making changes:
  enPointDrawErr_NoChange = 5,    //requested change would have no effect, so the command was trivially rejected.

  // errors/warnings when user is selecting from drawing
  enPointDrawErr_Cancel = 6,       //user cancelled
  enPointDrawErr_NoSelect = 7,     //user didn't select anything

  // other errors
  enPointDrawErr_GISUTIL_Missing = 8, //gisutil.dll could not be found.  it's required for drawing or reading LDD/LDT Aecc points.
  enPointDrawErr_Finish_Missing = 9, //finish.dll could not be found.

  // C&G errors
  enPointDrawErr_CgRegappFailed = 10,
  enPointDrawErr_CgBlockInsertFailed = 11,
  enPointDrawErr_CgAttachXdataFailed = 12,
  enPointDrawErr_CgCreateLabelFailed = 13

#ifdef CIVIL3D_NET

	, enPointDrawErr_PointNumberAlreadyExists = 14
	, enPointDrawErr_PointNameAlreadyExists = 15

#endif

};

#ifdef ACRXAPP
class AcDbObject;
class AcDbEntity;
#define acdbCurDwg acdbHostApplicationServices()->workingDatabase
#endif

//helper class for the "leader" feature (line and arrow head to show displacement)
class PointDrawLeader;

//class for quickly looking up a point by name in a selection set of cogo points.
class PointMap;

// for holding a tag and value pair in a linked list of custom attributes to be applied
//  to an AutoCAD block.
class CustomAttribute;

#include "cad_api.h" // for CEntity, the base class

void MakeContinuousLineType(const char *szTempDir, const char *ltname_orig, double ltscale, double spacing_scale,
                            char *szResultLT, BOOL *pbUserSpacingUsed, int *pType, EntityList *pEntityList,const char *ltype_buf);

// structure for point label format used for C&G points
//		  C&G points consist of a block called the symbol and can have up to 3 text labels
//		  (point ID, description and elevation) placed around the symbol in a variety 
//		  of locations or positions.
//        Point label Positions are numbered and arranged around the symbol like the numbers 
//		  on the keypad on the keyboard with the symbol being at the 5.
//          - label position is defined by: 7 = top left, 8 = above pt, 9 = top right, etc
//            0 = do not show label
//          IMPORTANT NOTE: only the elevation can be at position 5 - the point symbol 
//		    replaces the decimal point in the elevation label.
typedef struct PointLabelFormat
{
  BOOL bPlotRadial;
  BOOL bSeparatePtLabelLayers;
  double dLabelSize;
  BOOL bMeters;
  BOOL bSizeScaled;
  int nDescCharsToEliminate;
  int nDescCharsToShow;
  int nDescPosition;
  int nElevDecimalPlaces;
  int nElevPosition;
  int nElevWholeNumberPlaces;
  int nPointPosition;
} PointLabelFormat;

/*
 * Trivial spatial interval booleans which may or may not exist elsewhere in code corpus; remove if redundant,
 * relocate if not. It is assumed that the rectangle is aligned with the axes.
 * @param const ads_point& krads_pointPoint	Point to test
 * @param const ads_point& krads_pointLL		Lower left of rectangle
 * @param const ads_point& krads_pointUR		Upper right of rectangle
 *
 * @author C.Petersohn January, 2008
 */
const bool isPointInRectangle(const ads_point& krads_pointPoint, const ads_point& krads_pointLL, const ads_point& krads_pointUR);
const bool isRectangleIntersect(const ads_point& krads_pointLL0, const ads_point& krads_pointUR0,
																const ads_point& krads_pointLL1, const ads_point& krads_pointUR1);
bool RectangleIntersect(const ads_point& krads_pointLL0, const ads_point& krads_pointUR0,
												const ads_point& krads_pointLL1, const ads_point& krads_pointUR1,
												ads_point& rads_pointLL, ads_point& rads_pointUR);
/*
 * Support function to filter selection set entities based on DXF 10 (insertion point) within a certain range. Introduced to
 * alleviate (temporary?) inability of IntelliCAD [6.4] to perform ssget relational scans on coordinates.
 * @param const ads_name& rads_nameSelection	Selection set to process
 * @param const ads_point& krads_pointLL			Lower left of target rectangle
 * @param const ads_point& krads_pointUR			Upper right of target rectangle
 *
 * @author C.Petersohn January, 2008
 */
ads_name& ssFilterInsertion(ads_name& rads_nameSelection, const ads_point& ads_pointRangeLL, const ads_point& ads_pointRangeUR);
//
// A point to be inserted into a drawing.
//note, PointDwg would have been my first choice as a name, but it's used in scadutil.cpp. --Ron
class PointDraw : public CEntity
{
public:
  PointDraw();
  ~PointDraw();
  PointDraw( const PointDraw *pOrig);
  void Init( const PointDraw *pOrig);

  #ifdef CIVIL3D_NET
    PointDraw(int pointNumberC3D);
    #pragma managed
    void Init(int pointNumberC3D);
    #pragma unmanaged
  #endif

  //-------------------------- SIMPLE POINT DRAWING --------------------------------------
  // Use this method, plus PointDrawMgr::Draw() to create new points.
  // Initializes this point from global SurvCADD and AutoCAD settings like those found
  //  in the SurvCADD Point Defaults dialog and Lisp variable 'crdfile' and AutoCAD's
  //  VIEWTWIST.  Given the description, szDesc, if the global settings indicate that
  //  information should be read from Field-to-Finish, that will happen, also.
  //  'loc' should have as much information as possible, including the real-world elevation.
  //  The zero-elevation setting will be used along with elevation precision setting
  //  to set the elevation attribute correctly and to control the point entity's elevation.
  // Any of the methods in the "full-featured" section can be used to override the
  //  results of this method.
  // Returns enPointDrawErr_OK if successful.
  enPointDrawErr SimpleInit( const char *szName, const char *szDesc, ads_point loc);

  //------------------------ FULL-FEATURED POINT DRAWING ---------------------------------

  // SetBlock()
  // Call this in order to cause the point name, description, and elevation to be displayed
  //  and in order for the optional reactor to be placed and in order for the point entity to
  //  be placed (depending on point format).  Use the empty string for whichever parts aren't
  //  necessary.
  // Parameters:
  //  szBlockName    -- technically a file name, like "srvpno1".
  //  loc            -- location of the point entity.
  //  fXScale        -- scale factor along x-axis
  //  fYScale        -- scale factor along y-axis
  //  fAngle         -- angle, in radians, of rotation counter-clockwise from x-axis towards y-axis.
  //  szDisplayName  -- point name for display purposes
  //  szInternalName -- point name for extended entity data (used by reactors, etc.)
  //  szElev         -- point elevation, as a string
  //  szDesc         -- point description
  //  szNameLay      -- point name layer. If SurvCADD block and it's "PNTNO", then srvpno*.dwg will determine layer.
  //  szElevLay      -- point elevation layer. If SurvCADD block and it's "PNTELEV", then srvpno*.dwg will determine layer.
  //  szDescLay      -- point description layer. If SurvCADD block and it's "PNTDESC", then srvpno*.dwg will determine layer.
  //  szPointEntityLay -- point entity layer
  //  frealZ         -- Z from coordinate file
  PointDraw& SetBlock(const char *szBlockName, const ads_point loc,
											const double fXScale, const double fYScale, const double fAngle,
											const char *szDisplayName, const char *szInternalName, const char *szElev, const char *szDesc,
					            const char *szNameLay, const char *szElevLay, const char *szDescLay, const char *szPointEntityLay,
											const double fRealZ);
/*
 * Layer private data member access. Modified return signature to common practice of object reference for setters,
 * and inlined since they were implemented in this header. Also added heretofore missing SetSymbolLayer() below
 * to complement GetSymbolLayer. These members also test for layer names beginning with the prefix "CLAYER",
 * in which case the current layer name replaces this prefix as requested by DSC.
 *
 * TODO:
 * Implementation still has many (legacy) direct references to these private member variables (m_strPointEntityLay, m_strNameLay,
 * m_strElevLay, m_strDescLay, and m_strSymLay). These should be removed in favor of these member access functions, and
 * additional logic should resist doing this.
 * @author C.Petersohn April, 2008
 */

#ifdef HAVE_CAD
  inline PointDraw& SetPointEntityLayer(const char *szEntityLay) { m_strPointEntityLay = (const char*)ADSCLayer(szEntityLay); return *this; }
#else
  inline PointDraw& SetPointEntityLayer(const char *szEntityLay) { m_strPointEntityLay = szEntityLay; return *this; }
#endif
  const char* GetPointEntityLayer() { return m_strPointEntityLay; }

#ifdef HAVE_CAD
	inline PointDraw& SetNameLayer(const char *szNameLay) { m_strNameLay = (const char*)ADSCLayer(szNameLay); return *this; }
#else
	inline PointDraw& SetNameLayer(const char *szNameLay) { m_strNameLay = szNameLay; return *this; }
#endif

  const char* GetNameLayer() { return m_strNameLay; }

#ifdef HAVE_CAD
  inline PointDraw& SetElevLayer(const char *szElevLay) { m_strElevLay = (const char*)ADSCLayer(szElevLay); return *this; }
#else
  inline PointDraw& SetElevLayer(const char *szElevLay) { m_strElevLay = szElevLay; return *this; }
#endif
  const char* GetElevLayer() { return m_strElevLay; }
#ifdef HAVE_CAD
  inline PointDraw& SetDescLayer(const char *szEntityLay) { m_strDescLay = (const char*)ADSCLayer(szEntityLay); return *this; }
#else
  inline PointDraw& SetDescLayer(const char *szEntityLay) { m_strDescLay =  szEntityLay; return *this; }
#endif
  const char* GetDescLayer() { return m_strDescLay; }
  const char* GetBlockLayer();  //the parent or block layer that the attributes belong to; only valid if this point has been drawn or selected.
  void SetBlockScale( const double fScale) { m_fXScale = m_fYScale = fScale; }
  double GetBlockScale() { return m_fXScale; }
  void SetBlockAngle( const double fAngle) { m_fAngle = fAngle; }
  double GetBlockAngle() { return m_fAngle; }
  void SetBlockName( const char *szBlockName) { m_strBlockName = szBlockName; }//not a substitute for SetBlock()
  const char* GetBlockName() { return m_strBlockName; } //the attribute layout filename (e.g., SRVPNO1)

  // If you want to specify particular colors for the when creating a layer, use this function.
  // Use -1 to specify that the manager should choose a color.
  // If you don't call this method, the manager chooses the colors.
  void SetLayerColors( int iNameColor = -1, int iElevColor = -1, int iDescColor = -1, int iPointEntity = -1, int iSymColor = -1);

  // Change color of a SurvCADD block's attributes on the fly (justin 12-18-03)
  void SetEntityColorsSurvCADD(int iNameColor = -1, int iElevColor = -1, int iDescColor = -1, int iPointEntity = -1, int iSymColor = -1);

  // SetSymbol() gives the information on the symbol.
  // Call SetBlock() and/or SetSymbol() prior to calling PointDrawMgr::Draw().
  // Parameters:
  //   szSymbolName    -- name of symbol (e.g., "spt4")
  //   loc             -- insertion point
  //   fXScale         -- x scale factor
  //   fYScale         -- y scale factor
  //   fAngle / locDir -- rotation angle or point specifying angle (check docs on Acad _.Insert command).
  //   szSymbolLay     -- symbol layer
  PointDraw& SetSymbol(const char *szSymbolName, const ads_point loc,
											 const double fXScale, const double fYScale, const double fAngle, const char *szSymbolLay);
  PointDraw& SetSymbol(const char *szSymbolName, const ads_point loc,
											 const double fXScale, const double fYScale, const ads_point locDir, const char *szSymbolLay);
  void SetSymbolAngle( const double fAngle) { m_fSymAngle = fAngle; m_bSymUseAngle = TRUE; }
  void SetSymbolScale( const double fScale) { m_fSymXScale = m_fSymYScale = fScale; }
  void SetSymbolZScale( const double fScale) { m_fSymZScale = fScale; }
  double GetSymbolAngle() { return m_fSymAngle; }
  double GetSymbolScale() { return m_fSymXScale; }
  const char* GetSymbolName() { return m_strSymName; }
#ifdef HAVE_CAD
  inline PointDraw& SetSymbolLayer(const char *szSymbolLayer) { m_strSymLay = (const char*)ADSCLayer(szSymbolLayer); return *this; }
#else
  inline PointDraw& SetSymbolLayer(const char *szSymbolLayer) { m_strSymLay = szSymbolLayer; return *this; }
#endif
  const char* GetSymbolLayer() { return m_strSymLay; }

  //void SetSymbol(....  take into account twist screen)

  // Causes a copy of the given string to be attached as extended entity data.
  // See attach_ptnm2() in scpoint.cpp.
  void SetAttachedDesc( const char *szAttached) { m_strAttachedDesc = szAttached; }
//
// Sets both the display and internal name.
/*
 * Added optional support to update graphic entities and associated XData if possible (legacy default is not to do so).
 * Also modified return signature. Ignore if indifferent.
 * @author C.Petersohn April, 2008
 */
  PointDraw& SetName(const char* kpszDisplayName, const char* kpszInternalName, const bool kbUpdateGraphics = false);
  const char* GetName() const;  //always the internal or "real" name.
  const char* GetDisplayName() const; //the point name to be displayed.  May be "".
  PointDraw& resetXDataName(const ads_name& krads_nameEntity);
  
  void SetDesc( const char *szDesc);// Point description; same as the description set by SetBlock().
  const char* GetDesc() const;// Point elevation that is displayed in the attribute block; same as szElev in SetBlock().

  // in the case of C&G points, if it is required this will set both m_strElev and m_strElev2
  void SetElev( const char *szElev);
  
  const char* GetElev() const;
  // elev2 is used by C&G when whole and decimal parts of elevation are drawn as 2 entities
  const char* GetElev2() const;
  
  ads_point& GetLoc(ads_point *pResult) const; //location of the point entity which might be different than symbol.
  void SetLoc( ads_point loc);      //location of the point entity which might be different than symbol.
  void SetLoc2( ads_point loc);     //set location of both symbol and point.
  void MoveToLoc( ads_point loc);   //moves this representation of a point in a drawing to the new location.
                                    //  This will appropriately adjust the symbol information as well.
                                    //  To commit the changes to the drawing, call the UpdateDrawing method
                                    //  of the manager.  Handles zero-elevation rules, also.

  const char* GetSymbol() const { return m_strSymName; } //returns the symbol name or "" if it was never set or doesn't exist.
  void SetSymbol( const char* szSymbolName); //if not set already, will set default values for rotation, size, and layer.  use SetSymbol() above if possible.

  // By default, this point will use whatever reactor link settings the PointDrawMgr uses.
  // To override the default behavior, use these two methods.  Make sure that a coordinate
  //  file was given to the manager.
  void DisableLink() { m_iLink = -1; }
  void EnableLink()  { m_iLink =  1; }

  // Override manager's crdfile and reactor setting.  The reactor setting here is exactly as if
  //  EnableLink() had been called if bReactor is TRUE and DisableLink() if bReactor is FALSE.
  void SetCrdFile( const char *szCrdFile, BOOL bReactor);
  void SetCrdFile( const char *szCrdFile);
  //make sure buffer is FILENMSZ long.  If the point has the same crd file as the open crd file
  //  when the point was selected from the drawing, szCrdFile will be empty string, "".
  // Note: for the CrdFile to be set, the PointDrawMgr must be declared with kbAllowCrdFilePerPoint=true (2nd argument)
  char* GetCrdFile(char *pszCrdFile) const;			// Return signature modified +CRP [May, 2008].

  // By default, SurvCADD's triangulate and contour will triangulate the point entity. If contouring
  //  for this point is turned off, extended entity data will be attached notifying Tri & Contour.
  //  Call the Set method on a new point prior to calling the PointDrawMgr's Draw() function.
  //  When reading a drawing using PointDrawMgr, GetContourable() currently always returns TRUE
  //  because the extended entity data isn't 
  void SetContourable( BOOL bContourable);
  BOOL GetContourable() const;
  //odd function; goes right out and adjusts extended entity data. Only works if this PointDraw was
  //  created by querying the drawing so that the entity names of the attribute block, etc., are known.
  //  Updates the internal boolean, too.
  void SetContourableInDwg( BOOL bContourable);

  // The user may be using their own AutoCAD Blocks that have custom attributes. Each custom attribute
  //  consists of a tag and a value. When this PointDraw is drawn, if one of this PointDraw's tags
  //  matches the tag of the user's custom attribute, then the value of that attribute will be
  //  set from the value of the given CustomAttribute.
  // The input is a linked list that will be copied. If this PointDraw already contains any
  //  CustomAttributes, they will be deleted first.
  // Calling with NULL has the effect of deleting whatever internal list already exists.
  void SetCustomAttributes( const CustomAttribute *pCurrOrigAttr);

  // Can be used to clear the state of this point after a call to any of the modifying functions.
  // The state will be restored as if the point had just been created.
  void Clear(); 

  // returns TRUE if the given point could be merged with this.  This is a specialized
  //  function for combining information about a logical point that comes from multiple
  //  entities in the drawing.
  BOOL Merge( const PointDraw *pOrig);

  //Only when running inside LDD/LDT, do we want to preserve the LDT point number.
  //In LDT, the point name is an alias and every point has a point number.
  void SetLDDNumber( long iNumber) {
    #ifdef LDD
    m_iLDDNumber = iNumber;
    #else
	  iNumber; // for compiler warning
    #endif
  }
  long GetLDDNumber() {
    #ifdef LDD
    return m_iLDDNumber;
    #else
    return 0; //is always an invalid LDD/LDT point #.
    #endif
  }

  #ifdef CIVIL3D_NET
  int GetPointNumberC3D()	{ return pointNumberC3D; }
  void SetPointNumberC3D(int pointNumberC3D)	{ this->pointNumberC3D = pointNumberC3D; }
  const char* GetPointCodeC3D() { return pointCodeC3D; }
  void SetPointCodeC3D(const char* pointCodeC3D) { this->pointCodeC3D = pointCodeC3D;	}
  #endif

  #ifdef LDD_COM_ADD_COGO
  // For attaching and getting the symbol handle to or from the Aecc Cogo point.
  void SetLDDSymbolHandle( const char *szHandle) { m_hSymbolHandle.Set( szHandle); }
  const char* GetLDDSymbolHandle() { return m_hSymbolHandle.Get(); }
  #endif

  // Adds the entities of this PointDraw structures to the given selection set.
  // Returns the # of entities added.
	int GetEntities(ScSSet& rScSSet, const bool kbIncludeSubentities = true) const;
	int GetEntitiesOBB( ScOBB *pOBB) const; //gets the OBB around all of the entities. returns # of entities contributing.
  // specialized functions that kind of break the idea of the API.  Some commands want to
  //  individually modify some of the attributes of the Cogo point.
/*
 * Not exactly sure what is implied by the above comment assumed to apply to the following member access functions.
 * Inlined as they were all implemented in this header, changed argument and return signatures to references and
 * return something more useful than void.
 * @author C.Petersohn April, 2008
 */
  inline ads_name& GetEntityName(ads_name& ads_nameName) const { COPY_ENTITY_NAME(ads_nameName, m_entScName); return ads_nameName; }
  inline ads_name& GetEntityElev(ads_name& ads_nameElevation) const { COPY_ENTITY_NAME(ads_nameElevation, m_entScElev); return ads_nameElevation; }
  inline ads_name& GetEntityDesc(ads_name& ads_nameDescription) const { COPY_ENTITY_NAME(ads_nameDescription, m_entScDesc); return ads_nameDescription; }
  inline ads_name& GetEntitySymbol(ads_name& ads_nameSymbol) const { COPY_ENTITY_NAME(ads_nameSymbol, m_entScSymbol); return ads_nameSymbol; }
  inline ads_name& GetEntityBlock(ads_name& ads_nameBlock) const { COPY_ENTITY_NAME(ads_nameBlock, ((IS_ZERO_ENTITY_NAME(m_entScBlock)) ? m_entAeccPoint : m_entScBlock)); return ads_nameBlock; }
  inline ads_name& GetEntityPoint(ads_name& ads_namePoint) const { COPY_ENTITY_NAME(ads_namePoint, m_entScPointEntity); return ads_namePoint; }
	/**
	 * .CRD file path dictionary index accessors
	 * @author C.Petersohn September, 2010
	 */
	inline PointDraw& SetCoordinateFileIndex(const unsigned int	kuiCoordinateFileIndex) { m_uiCoordinateFileIndex = kuiCoordinateFileIndex; return *this; }
	inline const unsigned int GetCoordinateFileIndex() const { return m_uiCoordinateFileIndex; }

  static struct acad_resbuf* GetSymbolNameBuf( struct acad_resbuf *pTop);

  // For attaching and erasing leaders.  Pass in NULL to remove leader data and leader entities from drawing.
  // Attaching a new leader automatically erases an old one.
  void AttachLeader( ads_name ssLeaderEntities);
  // This version moves the leader data from the source PointDraw to this. Thus, pSource is modified. Note, that
  //  this function doesn't actually have any effect on the drawing; we're simply transfering data about the leader
  //  (or lack thereof) from one PointDraw to another.
  void TransferLeader( PointDraw *pSource);
  // If this point has a leader (line with arrow, pointing from attribute block to m_loc), TRUE
  //  will be returned and the oriented bounding box around the line segment will be filled in.
  BOOL GetLeader( ScOBB *pOBB);

  // Special hack-ish way of getting extended entity data to the function attach_finish() and elsewhere.
  // Instead, should maybe have a callback mechanism for attaching special extended entity data.
  void SetF2FCode( const char *szCode) { m_strF2FCode.Set( szCode); }
  const char* GetF2FCode()const { return m_strF2FCode.Get(); } //only read from drawing's xdata if sub-format is enSubFormatSCF2F. Use SetSubFormat().

public:
  ads_point m_loc; //the main location; is always the location of the attribute block, but not necessarily the symbol.
  #ifndef NO_ACAD
  enPointDrawFormat GetPtDrawFormat(void) { return m_enPtDrawFormat; }
  #endif // #ifndef NO_ACAD

private:
  friend class PointDrawMgr;
#ifdef SIMPLICITY
  friend bool F2F_DrawPoint(PointDraw *pPointDraw);
  friend bool DrawF2F(EntityList* pEList);
#endif
  // for the block
  BOOL m_bDrawBlock; //is the attribute block (desc, name, elevation) to be drawn?  Set to true by SetBlock().
  csString m_strBlockName;
  // elev2 is used by C&G when whole and decimal parts of elevation are drawn as 2 entities
  csString	m_strDisplayName, m_strInternalName, m_strElev, m_strElev2, m_strDesc,
						m_strNameLay, 				// Layer on which the point identifier ATTRIB is placed. Comment +CRP [April, 2008]
						m_strElevLay, 				// Layer on which the elevation ATTRIB is placed. Comment +CRP [April, 2008]
						m_strDescLay, 				// Layer on which the description ATTRIB is placed. Comment +CRP [April, 2008]
						m_strPointEntityLay,	// Layer on which the encapsulating INSERT attribute clock and POINT entity is placed. Comment +CRP [April, 2008]
						m_strF2FCode;
  double m_fXScale, m_fYScale, m_fAngle;
  int m_iNameColor, m_iElevColor, m_iDescColor, m_iPointColor;  //layer colors.  -1 means "let the manager choose"
  char m_bZeroElev; //if TRUE, then the entity should be placed at zero elevation even if the elevation attribute is non-zero.
  char m_bNonContourable; //FALSE by default (i.e., points should be contourable by default).
  CustomAttribute *m_pAttrs;// custom attributes linked list

  // for the symbol
  char m_bDrawSymbol; //is the symbol drawn?  Set to true by SetSymbol().
  char m_bSymUseAngle;  //if TRUE, use m_fSymAngle, otherwise use m_symLoc2.
  csString m_strSymName;
  csString m_strSymLay;		// Layer on which the symbol is placed. Comment +CRP [April, 2008]
  ads_point m_symLoc;
  double m_fSymXScale,m_fSymYScale,m_fSymZScale,m_realZ;
  double m_fSymAngle;
  ads_point m_symLocDir;
  int m_iSymColor;//layer colors.  -1 means "let the manager choose"

  // for having an attached string as extended entity data.
  csString m_strAttachedDesc;

  // for modifying SurvCADD points
  // added m_entScElev2 for C&G points since two elevation ents may be produced 
  //   - if the elevation is placed at the point location, one ent is used for the 
  //     whole number and one for the decimal number.  The point symbol then represents
  //     the decimal point in the elevation
  ads_name m_entScBlock, m_entScName, m_entScElev, m_entScElev2, m_entScDesc, m_entScSymbol, m_entScPointEntity;

  // for modifying LDD/LDT Aecc Points
  ads_name m_entAeccPoint;

  // for reactor link.
  // -1 ==> no reactor, 0 ==> manager's setting, 1 ==> use reactor.
  int m_iLink;
	/**
	 * Attempt to efficiently buffer index to .CRD file path
	 * @author C.Petersohn September, 2010
	 */
	unsigned int	m_uiCoordinateFileIndex;

  // for overriding Point Draw Manager's CRD file.  If it's NULL, then use the manager's, otherwise
  //  use this point's.
  csString *m_pStrCrdFile;

  #ifdef LDD
  long m_iLDDNumber;  // in LDD/LDT, there is a distinct point number separate from the point name.
  
  #ifdef LDD_COM_ADD_COGO
  csString m_hSymbolHandle; // so that we can attach the AECC point to the symbol.
  #endif
  #endif
  
  #ifndef NO_ACAD
 // #ifdef CG_DRAW_API

  PointLabelFormat m_ptLabelFormat;
  double m_dDwgScale;
  BOOL m_bFromCgAutoMapping;
  char m_szBlockDir[MAX_PATH];
  char m_szDwgName[MAX_PATH];
public:
  void SetDwgScale(const double dDwgScale) { m_dDwgScale = dDwgScale; }
  const double GetDwgScale(void) { return m_dDwgScale; }
  void SetFromCgAutoMapping(const BOOL bFromCgAutoMapping) { m_bFromCgAutoMapping = bFromCgAutoMapping; }
  const BOOL GetFromCgAutoMapping(void) { return m_bFromCgAutoMapping; }
  void SetBlockDir(const char *szBlockDir) { strcpy(m_szBlockDir, szBlockDir); }
  void SetDwgName(const char *szDwgName) { strcpy(m_szDwgName, szDwgName); }
  const char *GetDwgName(void) { return m_szDwgName; }
  const char *GetBlockDir(void) { return m_szBlockDir; }
  const PointLabelFormat *GetPtLabelFormat(void) { return &m_ptLabelFormat; }
  void SetPtLabelFormat(const PointLabelFormat* pPtLabelFormat);
  void SetDefaultPtLabelFormat(void); // helper for construction
  void GetEntityElev2(ads_name result)const { if (!IS_ZERO_ENTITY_NAME( m_entScElev2)) COPY_ENTITY_NAME( result, m_entScElev2); }
private:

//  #endif // #ifdef CG_DRAW_API
  enPointDrawFormat m_enPtDrawFormat;
  #endif // #ifndef NO_ACAD

  #ifdef CIVIL3D_NET
  int pointNumberC3D;
  csString pointCodeC3D;
  #endif

  //for extra entities that should be read from extended entity data on the attribute block.
  //  No attribute block means m_pExtras is NULL.  "SCLEADER" will be the application name.
  //  It's used for the entities representing the leader when the attribute block is dragged.
  //  See AttachLeader() and RemoveLeader().
  PointDrawLeader *m_pLeader;
  #ifndef NO_ACAD
  // private version. for storing extended entity data that is already on the attribute block.
  void _AttachLeader( struct acad_resbuf *pXData);
  void _MoveLeader( double dx, double dy);
  #endif
                              
  // private member that attempts to call SetBlock() with information from an LDD cogo
  //  point acad_resbuf list.  This point is not initialized.  pResEntityTop should include
  //  the extended entity data, so use PointDrawMgr::GetResbufs() to allocate pResEntityTop.
  BOOL GetFromEntityLDD( struct acad_resbuf *pResEntityTop, const ads_name nameEntity);

public:
  // for singly-linked list of selected points inside PointDrawMgr as well as custom needs of the user of the API.
  PointDraw *m_pNext;

protected:
  // LockStatus support (initially only SQLite .crdb files will support this
  long m_nLockStatus; 

public:
  void SetLockStatus(long nStatus, CrdFile *pCRDFile = NULL);
  long GetLockStatus(CrdFile *pCRDFile = NULL);
};

#ifdef HAVE_CAD 
/**
 * @ingroup SCLIB
 * PointDrawMgr encapsulates some of the functionality of class PointDraw and manages dynamically
 * allocated lists of them created in various ways.
 *
 * @par Requirements
 *
 * @version
 * Added these comments																																			September, 2008 @author CRP
 * Added optional argument to constructor to override exclusion of nonvisble graphics per
 * user request and DSC																																			September, 2008 @author CRP
 *
 * @author Unknown
 * @date Unknown
 * 
 * @todo
 *
 * @bug 
 *
 */
class PointDrawMgr
{
public:
  // Constructor calls acad_regapp("PNTNO") and acad_regapp("SPT") and sets the
  //  internal current CRD file and reactor usage from the Lisp globals
  //  'crdfile' and 'linkon1'.
  // Pass in the format that you want this manager to write points with.
  // By default, the manager will use the global point format POINT_DRAW_DEFAULT
  //  unless that's been overridden by SetGlobalDefault().
  // DO NOT DECLARE AS A GLOBAL VARIABLE because some Lisp settings are cached to improve performance.
  //  If there's a chance that the Lisp settings can change during the life of this object, then
  //  call ReadLispVariables().
/**
 * Constructor modified to allow user initialization of heretofore inaccessible member m_bAllowCrdFilePerPoint.
 * Default behavior remains unchanged
 * @author C.Petersohn May, 2008
 * Constructor modified to allow user initialization of heretofore hardcoded logic to eliminate nonvisible
 * graphics (on layers frozen or off). Default behavior remains unchanged.
 * @author C.Petersohn September, 2008
 *
 * @param enFormat								Graphics(?) format that you want this manager to write points with
 * @param kbAllowCrdFilePerPoint	Allows a per instance .CRD file to be associated with each graphic point representation
 * @param kbIncludeNonVisible			Retain any point graphics found in selections irrespective if visible or not
 */
  PointDrawMgr(enPointDrawFormat enFormat = enPDF_Default,
							 const bool kbAllowCrdFilePerPoint = false,
							 const bool kbRetainNonVisible = false);
  ~PointDrawMgr();
  void Clear(); //Frees the memory; resets this data structure.

  // Use the Get/SetDefaultFormat() to set the global default format for all
  //  PointDrawMgr instances.  This way, if an individual manager's format is
  //  enPDF_Default, that manager will use the global setting instead.
  // GetFormat() returns the _effective_ format.  For example, if the manager's
  //  internal format was enPDF_Default, GetFormat() might return enPDF_LDD if that
  //  was the global default.  GetFormat() will never return enPDF_Default.
  // Get/SetDefaultFormat() can be done without a PointDrawMgr instance!  Use the
  //  following C++ syntax:  PointDrawMgr::SetDefaultFormat( enPDF_SurvCADD);
  void                SetFormat( enPointDrawFormat enFormat)          { m_enFormat = enFormat; }

  // examines the dwg and attempts to identify which type of drawing it is (ie, SurvCAD, C&G, etc.)
  enPointDrawFormat   IdentifyDwgFormat(void);
  
  // reset any static Point Draw Mngr data related to the changing of the dwg name 
  // due to the use of the save or saveas commands
  static void ResetForNewDwgName(const char *szOldDwgName, const char *szNewDwgName);

  enPointDrawFormat   GetFormat();
  void                SetDefaultFormat( enPointDrawFormat enFormat) const; //enPDF_Default is not allowed and will be ignored.
  enPointDrawFormat   GetDefaultFormat() const                        { return static_enDefaultFormat; }
  void                SetSubFormat( enPointDrawSubFormat enSubFormat) { m_enSubFormat = enSubFormat; }
  enPointDrawSubFormat GetSubFormat()                                  { return m_enSubFormat; }

  // If a crd filename should be attached to the point, use this method to let the manager know.
  // If a reactor should be used to keep the crd file up-to-date, set that to true.
  // If you don't call this method, the Manager will look at the global Lisp variables
  //  'crdfile' and 'linkon1'.
  void SetCrdFile( const char *szCrdFile, BOOL bReactor);
  const char* GetCrdFile();

  // after initializing the point, call this function to draw it.
  enPointDrawErr Draw( PointDraw *pPoint);

  // If a point entity and attribute block with the given name is exactly at the given
  //  location, it will be moved to the new location and its description will (optionally)
  //  be updated to a new value.
  // If many queries need to be done in a row, consider calling one of the selection functions
  //  to create a list (like SelectPoints, if you want every cogo point in the drawing) and
  //  then calling GetMap().  Pass in the map and it will be used instead for greatly
  //  improved performance.
  // Returns enPointDrawErr_OK if point was found and modified.  Returns enPointDrawErr_PointMissing
  //  if the point can't be found.  Returns enPointDrawErr_NoChange if the point may exist, but no
  //  change would result even if it did exist because new_loc was same as current_loc, etc.
  enPointDrawErr Modify(ads_point current_loc, const char *szName,ads_point new_loc, const char *szNewDesc=NULL,
                        PointMap *pMap=NULL,ads_name node_ename=NULL);

  // Deletes the given point from the drawing.  pPoint must have been initialized from one of
  //  the selection functions or drawing functions.
  // Returns enPointDrawErr_OK if the point was removed.
  // Returns enPointDrawErr_PointMissing if the point couldn't be found.
  // Returns enPointDrawErr_Unknown if pPoint wasn't initialized correctly or if there was some
  //  other error.
  enPointDrawErr Delete( PointDraw *pPoint);
  enPointDrawErr DeleteAttributeBlock( PointDraw *pPoint);  //just attribute block and point entity/marker.
  enPointDrawErr DeleteSymbol( PointDraw *pPoint); //just the symbol

  #ifdef CIVIL3D_NET
    #pragma managed
    enPointDrawErr DeleteC3D(PointDraw *pPoint);
    #pragma unmanaged
  #endif

  // 1. Call one of the manager selection functions to fill in a PointDraw structure.
  // 2. Modify that PointDraw structure.
  // 3. Call this function to commit the changes to the drawing.
    // Returns enPointDrawErr_OK if point was found and modified.  Returns enPointDrawErr_PointMissing
  //  if the point or some part of the point could not be found.
  // 'bUpdateScreenImage' says whether or not acad_entupd() should be called to update the appearance of
  //  this cogo point in the drawing.  Updating the screen is slow.  If you pass in FALSE, then you should
  //  call acad_command(RTSTR,"_REGEN",RTNONE) after updating all of your points.
  enPointDrawErr UpdateDrawing( PointDraw *pPoint, BOOL bUpdateScreenImage = TRUE);

  #ifdef CIVIL3D_NET
    #pragma managed
    enPointDrawErr UpdateDrawingC3D(PointDraw *pPoint, BOOL bUpdateScreenImage = TRUE);
    #pragma unmanaged
  #endif

  // Builds an internal selection set of potential point entities.
  // Use PointDrawMgr::GetNextSelected() to iterate through the list.
  // bPointsMustHaveName -- if TRUE, all points returned via GetNextSelected
  //  will have a non-empty name.
  // szQueryUser -- if not NULL, the user is presented with the prompt and asked to select the points.
  //  If NULL, all points in the drawing are selected unless the rectangle is used.
  // pCorner1 and pCorner2 -- opposite corners of the rectangle.  If not NULL,
  //  and if szQueryUser is NULL, these will be used to form a "crossing" selection
  //  where any entity that intersects the rectangle will be considered.
  // pCRDFile is for two things:
  //	1) elevations if missing from drawing
  //	2) lock status (if supported by crd file type - currenlty only .crdb format)
  // bAttributeBlockOnly -- if TRUE, only attribute blocks will be selected, not symbols or point entities.
  // Returns enPointDrawErr_OK if at least one item was selected.
  // Returns enPointDrawErr_Cancel if the user cancelled.
  // Returns enPointDrawErr_NoSelect if nothing was selected.
  enPointDrawErr SelectPoints( BOOL bPointsMustHaveName = TRUE, const char* szQueryUser = NULL,
                               ads_point *pCorner1 = NULL, ads_point *pCorner2 = NULL, CrdFile *pCRDFile = NULL,
                               BOOL bAttributeBlockOnly = FALSE, BOOL bAllowCgPts = FALSE);
  // basically the same thing as SelectPoints(), but only the points that are exactly at the
  //  chosen point will be added to the selection set.
  enPointDrawErr SelectAtPoint( const char *szPrompt, BOOL bPointsMustHaveName = TRUE, BOOL bAllowCgPts = FALSE);
  // Same as above, but user isn't queried.  Instead exact point must be provided.
  enPointDrawErr SelectAtPoint( const ads_point loc, BOOL bPointsMustHaveName = TRUE, BOOL bAllowCgPts = FALSE);
	/**
  * SYNOPSIS:
  *		Member SelectAtEntity attempts to completely fill in a PointDraw object given the entity name of ONE of the
	*   three entities representing a Carlson point (attribute INSERT, symbol INSERT, or POINT).
	*   Extracts as much information as possible from the entity into pResult.
	*
  * @param ksads_nameEntity is the name of one of up to three entities comprising a point.
  * @param rnSelectCount indicates the number of main entities used to fetch the point.
  */
  enPointDrawErr SelectAtEntity(const ads_name ksads_nameEntity, short& rnSelectCount, BOOL bInitial = TRUE,
                                                                BOOL bAllowCgPts = FALSE);
  /**
  * SYNOPSIS:
	* Member fillSelection() attempts to "fill in" a selection of PointDraw objects as completely as possible based on the
	* POINT entity comprising a portion thereof by leveraging member SelectAtEntity.
	* Returns the number of selected PointDraw objects "filled".
  */
  unsigned long fillSelection();
  // Same as SelectPoints with all the defaults, except only the point attribute block information
  //  will be chosen.  The symbol and point entity data will be skipped.  This can be an important
  //  speed improvement.
  enPointDrawErr SelectPoints_AttributesOnly();
  // Same as SelectPoints except the given selection set will be used to build up the internal list.
  enPointDrawErr SelectPoints( ads_name ssEntities, BOOL bPointsMustHaveName = TRUE, CrdFile *pCrdFile = NULL,
                                                                BOOL bAttributeBlockOnly = FALSE, 
                                                                BOOL bAllowCgPts = FALSE);

  // Finds all the points within a given circle by doing a crossing selection around the circle
  //  and then discarding selected entities that are outside the circle.  This could potentially
  //  miss certain parts of point entities.  Don't use for finding just a single point.
  //  Use GetNextSelected() to retrieve the selected points.
  enPointDrawErr SelectInCircle(  const ads_point loc, BOOL bPointsMustHaveName, double fRadius, BOOL bAllowCgPts = FALSE);

  /**
   * Builds an internal selection set of potential point entities that fall within the given range
   * @param szRange								Carlson point range specification
   * @param *pCRDFile, if not NULL, is for three things:
	  1) elevations if missing from drawing
	  2) point data if kbIncludeNonGraphic == true
	  3) lock status (if supported by crd file type - currenlty only .crdb format)
   * @param bAttributeBlockOnly		If TRUE, only attribute blocks will be selected, not symbols or point entities.
   * @param kbIncludeNonGraphic		If true and pCRDFile is not NULL, all points from szRange contained in pCRDFile
	 *															will be included in the selection, absent graphic entity information.
   * @return											enPointDrawErr_OK if at least one item was selected.
	 *															enPointDrawErr_NoSelect if nothing was selected.
   *
	 * @remarks Modified signature to incorporate optional argument bIncludeNonGraphic. Default behavior (false)
	 *					remains unchanged.
	 * @author C.Petersohn July, 2008
	*/
  enPointDrawErr SelectPoints(const char* szRange, CrdFile *pCRDFile = NULL,
                                                                BOOL bAttributeBlockOnly = FALSE, 
                                                                const bool kbIncludeNonGraphic = false, 
                                                                BOOL bAllowCgPts = FALSE,
																BOOL kbFileOnly = FALSE);

  #ifdef CIVIL3D_NET
    #pragma managed
    enPointDrawErr SelectPointsC3D(BOOL bPointsMustHaveName = TRUE, 
								    const char* szQueryUser = NULL,
								    ads_point *pCorner1 = NULL, 
								    ads_point *pCorner2 = NULL);
    enPointDrawErr SelectPointsC3D(const char* szRange);
    enPointDrawErr SelectAtPointC3D(const char *szPrompt, BOOL bPointsMustHaveName = TRUE);
    enPointDrawErr SelectAtPointC3D(const ads_point loc, BOOL bPointsMustHaveName = TRUE);
    enPointDrawErr SelectInCircleC3D(const ads_point loc, BOOL bPointsMustHaveName, double fRadius);
    #pragma unmanaged
  #endif

  int GetNumSelected(); //returns the number of internally selected points.  remember, GetNextSelected() decrements the # selected.
  enPointDrawErr GetNextSelected( PointDraw *pResult);  //returns enPointDrawErr_OK if pResult was filled with valid data.  removes point from internal list.
  enPointDrawErr GetNextSelected( CrdPoint *pResult);   //returns enPointDrawErr_OK if pResult was filled with valid data.  removes point from internal list.
  PointDraw* GetSelection(); //returns the linked list resulting from last selection call and clears the internal selection.
                             //you must free the memory yourself.  You can use FreePointList().

  // Consider as an alternative to GetSelection(). Allows for quick look-ups based on point name.
  // Acts like GetSelection() in that ownership of the linked list of points is transferred from
  //  this PointDrawMgr to PointMap.  See PointMap for what methods are offered. The PointMap can
  //  also be passed back to some functions like Modify() to speed-up searches.
  void GetMap( PointMap *pMap);

  // by default, when reading cogo points from the drawing, the extended entity data is not checked
  //  to see if the cogo point originated from a crd file other than the current one.  This check
  //  is expensive, taking about 28.5% of the time of reading the points from the drawing.
  inline void AllowCrdFilePerPoint( BOOL bAllow) { m_bAllowCrdFilePerPoint = bAllow; }
  inline BOOL AllowCrdFilePerPoint() { return m_bAllowCrdFilePerPoint; }
	inline const bool getRetainNonVisible() const { return m_bRetainNonVisible; }
	inline PointDrawMgr& setRetainNonVisible(const bool kbRetainNonVisible)	{ m_bRetainNonVisible = kbRetainNonVisible; return *this; }
  // Initializes the given point structure with as much information as possible from
  //  nameEntity.
  // Returns enPointDrawErr_OK if nameEntity was identified as a point entity.
  enPointDrawErr GetFromEntity( ads_name nameEntity, PointDraw *pResult, const bool kbClearResult = true);
  enPointDrawErr GetFromEntity( ads_name nameEntity, CrdPoint *pResult, const bool kbClearResult = true);
  // Initializes the given PointDraw structure with as much information as possible from
  //  pObj, which must have come from nameEntity.  pObj will not be closed by this function.
  // Returns enPointDrawErr_OK if nameEntity was identified as a cogo point entity.
	/**
  *	@param [ARX version] pObj																AcDbObject pointer to nameEntity opened as AcDb::kForRead.
  *	@param [ADS version] rADSResultBufferMultiMapEntity			Reference to mapped result buffer representing nameEntity, including XData.
	*	@param ads_name nameEntity															Entity name from which to extract data and upon which pObj/rADSResultBufferMultiMapEntity must be based.
  *	@param PointDraw *pResult																Address of PointDraw instance in which to place extracted data.
  *	@param const bool kbClearResult													Flag added to make clearing of existing information in pResult optional.
  */
  #ifdef ACRXAPP
  enPointDrawErr GetFromEntity(AcDbObject *pObj, ads_name nameEntity, PointDraw *pResult, const bool kbClearResult = true);
	#else
	enPointDrawErr GetFromEntity(ADSResultBufferMultiMap& rADSResultBufferMultiMapEntity, ads_name nameEntity, PointDraw *pResult, const bool kbClearResult = true);
  #endif
  // Adds the entities of the current internally selected PointDraw structures to the given selection set.
  // Returns the # of entities added.
/*
 * Modified to use an ScSSet vs. platform selection set as IntelliCAD (6.4) fails when attempting to
 * add subentities to one. Also added option to member signature to get main entities only, consistent with
 * related member in class PointDraw.
 * @author C.Petersohn March, 2008
 */

  int GetEntities(ScSSet& rScSSet, const bool kbIncludeSubentities = true) const;

  // Gets resbufs with the appropriate extended entity data. The result can be passed into GetFromEntity().
  struct acad_resbuf* GetResbufs( ads_name nameEntity);

  // Begins and ends the section of code where crd point reactors need to be disabled.
  // These are included primarily for performance reasons.  If you have a loop that is
  //  inserting/modifying many points in a drawing and if reactors are turned on, you
  //  can use these to disable reactors around your loop.
  // StartReactorSection must always be paired with EndReactorSection.
  // It's OK for the pairs to be nested, i.e., "start start end end".
  void StartReactorSection();
  void EndReactorSection();

	void FreeSelectionSet(PointDraw* pPointDrawHandle);

  #ifdef LDD
  // When running inside LDD/LDT, and when drawing points that already exist in the points.mdb
  //  coordinate file, call this before drawing any points.  This will cause LDT's Insert
  //  Points command to draw the points after the DLL & Lisp files exit.  A call to
  //  (cf:PointDrawAPI "Mgr_PostLDT") will occur after LDT draws the points which will read
  //  the correct description and layer from the LDT_point_fix.tmp file.
  void PreDrawLDD();
  // the range and description filter are only useful if szRange is "all" and szDescFilter
  //  is NULL or "*".
  void PostDrawLDD( const char *szRange = NULL, const char *szDescFilter = NULL);

  #ifdef LDD_COM_ADD_COGO
  // Creates this temporary file used to store the point numbers and descriptions, etc.
  //  This process is necessary because we exit our DLL and Lisp functions in order to call the
  //  LDD command-line tool to insert points into the drawing from the coordinate file.
  // Returns TRUE if successful and FALSE otherwise.
  BOOL CreateLDTPointFile();

  // Returns the # of points in the temporary file if it was successfully opened and 0 otherwise.
  int OpenLDTPointFile();
  
  void CloseLDTPointFile();

  // Appends pPoint to the open file.
  void StoreLDTPoint( PointDraw *pPoint);

  // Gets the next point from the file.
  // Returns TRUE if pPoint was filled in successfully.
  // Returns FALSE if we reached the end of the file or if there was an error.
  BOOL LoadLDTPoint( PointDraw *pPoint);

  // Returns TRUE if point lists can be entered on the command-line which is a requirement for
  //  drawing points if LDD_COM_ADD_COGO is on.  Returns FALSE otherwise.
  BOOL IsLDTCommandLineOn() { return m_iOldCmdLineListOn!=0; }  //let's allow -1 ("unknown") to also count as "OK".
  #endif
  #endif

  // Attaches extended entity data to an existing Aecc (LDD) point in the drawing.
  //  pPoint must have been created by selecting points through PointDrawMgr.
  void AttachDataLDD( PointDraw *pPoint);

  // If the Lisp variables have been updated since this instance was created, call this to update the internal
  //  cache of some Lisp settings, such as elevation prefix and suffix. This function is called by the constructor.
  void ReadLispVariables();
  // Kind of a strange hack-ish function. If the PointDrawMgr is going to be used only for modifying existing
  //  points from a high-level point of view, yet maybe needs to create new point structures, call this to avoid
  //  doing things like adding the elevation prefix and suffix.
  void NoNewPoints();

  // If you need to manually override the elevation prefix and suffix rather than using the global settings,
  //  use these.
  void SetElevPrefix( const char *szPrefix) { m_strElevPrefix.Set( szPrefix); }
  void SetElevSuffix( const char *szSuffix) { m_strElevSuffix.Set( szSuffix); }
  void GetElevPrefix( csString *pStr) { pStr->Set( m_strElevPrefix.Get()); }
  void GetElevSuffix( csString *pStr) { pStr->Set( m_strElevSuffix.Get()); }

  // See 'm_iElevIntegerDigits' below for explanation.
  void SetElevIntegerDigits( int iIntegerDigits = 0) { m_iElevIntegerDigits = iIntegerDigits; }

  // These exist to make sure that object snapping is turned off because otherwise, drawing points may lead to
  //  bizarre behavior where the symbol or attribute block is positioned at the wrong location or wrong angle.
  //  The client of this Point Draw API does not normally need to call these. WritingBegin() is called automatically
  //  the first time a cogo point is drawn. WritingEnd() restores the user's settings and is called, if necessary,
  //  by the deconstructor.
  // So the only time the client code should use these is if you need to restore object snap settings after
  //  drawing points but before the PointDrawMgr is deleted. In that case, go ahead and wrap the drawing section
  //  with a call to WritingBegin() at the beginning and WritingEnd() at the end.
  void WritingBegin();
  void WritingEnd();

private:

  enPointDrawErr DrawSurvCADD( PointDraw *pPoint);
  enPointDrawErr DrawSurvCADDSymbol( PointDraw *pPoint);
  enPointDrawErr DrawLDD( PointDraw *pPoint);

  void fixLayerVisibility( PointDraw *pPoint, int iColor, ads_name blockReference);
//#ifdef CG_DRAW_API
  // draws a C&G point and labels and attaches extended entity data
  //    - A C&G point symbol is most commonly a block without attributes. 
  //        - C&G does have 2 symbol blocks that have attributes that are used for 
  //          compatibility with LDD and are handled with special code)
  //    - The point name, description, and elevation, when present, are text entities
  //        - any of these 3 may or may not be drawn according to the C&G user's settings
  //    - The point symbol and the text labels all have xdata containing information that
  //      allows the point to be redrawn in exactly the same format as it was originally
  //      drawn.
  //    - Xdata also includes the handles that allow you to get the entity names of all
  //      the labels and the symbol
  //    NOTE: Handles MUST be on when a C&G entity is drawn
  enPointDrawErr DrawCG(PointDraw *pPoint);
  enPointDrawErr CgPlacePoint(PointDraw *pPoint);
  BOOL IsCgDwg(void); // helper for IdentifyDwgFormat()

  // member vars for C&G label format
  double m_dDwgScale;
  BOOL m_bFromCgAutoMapping;
  csString m_scBlockDir;
  BOOL m_bDoEntUpdate;  
    
public:
  // public function for initializing label format
  void SetPtLabelFormat(PointLabelFormat* pPtLabelFormat);
  void SetDwgScale(const double dDwgScale) { m_dDwgScale = dDwgScale; }
  void SetFromCgAutoMapping(const BOOL bFromCgAutoMapping) { m_bFromCgAutoMapping = bFromCgAutoMapping; }
  void SetBlockDir(const char *szBlockDir) { m_scBlockDir = szBlockDir; }
  void SetDoEntUpdate(const BOOL entUpdate) { m_bDoEntUpdate = entUpdate; }
  BOOL GetDoEntUpdate() { return m_bDoEntUpdate; }
private:


//#endif // #ifdef CG_DRAW_API

  #ifdef CIVIL3D_NET
  #pragma managed
  enPointDrawErr DrawC3D(PointDraw *pPoint);
  #pragma unmanaged
  #endif

  void IncLayerColor(); //increment layer color

  enPointDrawFormat m_enFormat; //The format that this manager's points are drawn in.
                                //If it's enPDF_Default, then static_enFormat is used.
  static enPointDrawFormat static_enDefaultFormat; //can't be enPDF_Default.  Is set from the #define POINT_DRAW_DEFAULT.
  enPointDrawSubFormat m_enSubFormat;
  int m_iColor; //Next layer color to use if a layer has to be created.
  csString m_strCrdFile;

  BOOL m_bReactor; //attach reactor to new points?
  int m_iInReactorSection;  //have we disabled reactors with Start/EndReactorSection? >0 means yes. 0 means no.
  int m_iPreviousReactorState; //what did CrdUtil.dll think was the reactor state?

  // for SelectPoints(), GetNextSelected(), etc.
  void _FreeSelectionSet();      // private method for freeing the selection set.
  PointDraw *m_pSelection;       // head of linked list of selected points
  BOOL m_bAllowCrdFilePerPoint;  // related to reading points from the drawing. FALSE by default because of performance costs.

	bool m_bRetainNonVisible;			/**< Retain nonvisible entities in graphic selections */
  // should the point entity and attribute block and symbol be put together in an AutoCAD group? Cached to save cpu when
  //  drawing with "Draw Nodes Only" in Draw-Locate Points.
  BOOL m_bCreateEntityGroups;   // read from Lisp variables. Set 

  // elevation suffix and prefix; read from Lisp variables elev_prefix and elev_suffix.
  csString m_strElevPrefix, m_strElevSuffix;
  double m_fAngBase; // system variable "ANGBASE"
  double m_fViewTwist; // 2*PI - sys var "VIEWTWIST"

  //Controls the # of digits to the left of the decimal point on the elevation label. Is read from the Lisp
  //  variable 'lp_digits' which is (currently) only available during draw-locate points. m_iElevIntegerDigits
  //  can also be set from SetElevIntegerDigits. Default value is 0.
  // 0 means no changes or "all". For example, if the elevation string is "210.45",
  // 0 --> "210.45"
  // 1 --> "0.45"
  // 2 --> "10.45"
  // 3 --> "210.45"
  // 4 --> "210.45"
  // 5 --> "210.45" and so on.
  int m_iElevIntegerDigits;

  #ifdef LDD_COM_ADD_COGO
  IDispatch *m_pAeccApp, *m_pCogoPref, *m_pPref, *m_pActiveProject, *m_pCogoPoints, *m_pActiveDocument;
  int m_iOldCreateSequential;
  int m_iOldCmdLineListOn;
  double m_fLDTScaleConvert;  //multiply this by PointDraw/SurvCADD scale factor to result in LDT scale factor.
  double m_fPrevLDTScale;     //the previous LDT "text size" or scale value that was set by either us or the user.
                              //this is an important speed optimization so that we don't update LDT unnecessarily.
  double m_fOldTextSize;      //LDT's text size setting that needs to be restored.
  BOOL m_bUseLDTsInsertPoints; //the default is FALSE. If TRUE, LDT's Insert Points command will be used to create
                              //the cogo points in the drawing based on those that are in the file.  See SetLDDFastMode().
  FILE *m_pTempPointsFile;  //for creating the temporary file to store the data for updating the points in the drawing
  int m_iNumTempFilePoints; // after we use the command-line to insert the points.
  BOOL m_bCreatingTempFile; //
  csString m_strLDDRangeDrawn; //range of point numbers actually drawn
  #endif

  // for use by WritingBegin() and WritingEnd().
  BOOL m_bReadyToEditDwg;
  int m_iOrigOSMODE, m_iOrigATTREQ, m_iOrigATTDIA;

  // private method for building internal selection set of point entities in the drawing.
  // See PointDrawMgr::SelectPoints() for more info.
  // bPointsMustHaveName -- if TRUE, all points returned via GetNextSelected
  //  will have a non-empty name.
  // pCorner1 and pCorner2 -- opposite corners of the rectangle.  If not NULL,
  //  and if bQueryUser is FALSE, these will be used to form a "crossing" selection
  //  where any entity that intersects the rectangle will be considered.
  // bAddToCurrentSelection -- if TRUE, the existing list stored in m_pSelection will be appended
  //  rather than being replaced.
  // pFilterPoint -- if not NULL, will be used in the filter selection to only get entities at that point.
  // pCRDFile, if not NULL, is for two things:
  //	1) elevations if missing from drawing
  //	2) lock status (if supported by crd file type - currenlty only .crdb format)
  //  used to find the elevation.
  // If ssEntities is not NULL, then szOption, pCorner1, pCorner2, and pFilterPoint are ignored and
  //  instead of examining the drawing, ssEntities will be used to build the list of cogo points.
  // Returns enPointDrawErr_OK if at least one item was selected.
  // Returns enPointDrawErr_Cancel if the user cancelled.
  // Returns enPointDrawErr_NoSelect if nothing was selected.
  enPointDrawErr _SelectPoints( const char *szOption, void *pCorner1, void *pCorner2,
                                BOOL bPointsMustHaveName, BOOL bAddToCurrentSelection = FALSE,
                                const ads_point *pFilterPoint = NULL, BOOL bAttributeBlockOnly = FALSE,
                                CrdFile *pCrdFile = NULL, ads_name ssEntities = NULL, BOOL bAllowCgPts = FALSE);

	const ADSCode _SelectPointsInRangeWA(const ads_point& krads_point, const double kdRange,
																			 const acad_resbuf& krsacad_resbufFilter, 
                                                                             ads_name& rads_nameSelection, 
                                                                             BOOL bAllowCgPts = FALSE) const;
	const ADSCode _SelectPointsInRangeWA(const ads_point& ads_pointRangeLL, const ads_point& ads_pointRangeUR,
																			 const acad_resbuf& krsacad_resbufFilter, 
                                                                             ads_name& rads_nameSelection, 
                                                                             BOOL bAllowCgPts = FALSE) const;

	/**
	 * Append a (possibly nongraphic) Carlson point range to a selection from a coordinate file
	 * @param kstrRange		Carlson point range
	 * @param rCrdFile		(Open) Coordinate file
	 * @return						Count of points added to the selection, which are inserted at the beginning
	 * @author CRP [July, 2008].
	 */
	const unsigned long addSelection(const csString& kstrRange, CrdFile& rCrdFile);
};

#endif // #ifdef HAVE_CAD

// Used for storing a (long) list of points, sorted by name, and quickly looking them up.
class PointMap
{
public:
  PointMap();
  ~PointMap();
  void Clear(); //frees all internal memory

  // Initializes the mapping structure. This PointMap object will own the memory.
  void Init( PointDraw *pList);

  // Returns a linked list of cogo points that have the given name and that optionally
  //  are at the precise given location. The returned list is a copy of the internal points
  //  and the caller is responsible for freeing this list (use FreePointList).
  PointDraw* Find( const char *szName, ads_point loc = NULL);
  
  // Efficiently finds all of the points within the given point range. The caller must
  //  delete the returned objects or use FreePointList().
  PointDraw* FindRange( const char *szRange);

  long GetNumPoints() const { return m_iNumPoints; }
  const PointDraw* GetPoint( long i) const;

private:
  PointDraw *m_pList;     //the linked list of points that we own
  PointDraw **m_apPoints; //array of pointers to the above points
  long m_iNumPoints;      //# of points in the list, # of elements in array
};

void FreePointList( PointDraw *pList); //traverses list, freeing every point.

//for holding a linked list of custom attributes on an AutoCAD block.
class CustomAttribute
{
public:
  CustomAttribute() {
    decimals = 3;
    next=NULL;
  }
  CustomAttribute( const char *set_tag, const char *set_value) : tag(set_tag), value(set_value) {
    decimals = 3;
    next=NULL;
  }
  csString tag,value,holder;
  int decimals;
  CustomAttribute *next;
};

// Finds the given AutoCAD block definition and returns information about it. The search is done in this order:
// 1. current drawing
// 2. If a path is supplied with szBlockName, then the block will be looked for there.
//  Otherwise, the block will be looked for in the \SUP directory (as specified by the Lisp variable "supdir").
// 3. in the AutoCAD search path.
// Retruns TRUE if the block was found and FALSE otherwise.
//  ppCustomAttributes -- will be set to NULL if the block is not found. If it is found, then
//    all attributes that are not "constant" and not "preset", as AutoCAD defines those terms,
//    will be returned. The Carlson Software custom tags will not be included in this list.
//	bForceReadCarlsonTags
//    forces the read of Carlson Software custom tags; 1 reads them also and adds them to the list
BOOL ReadBlock( const char *szBlockName, CustomAttribute **ppCustomAttributes, int bForceReadCarlsonTags=0);

// Quickly inserts a block reference into the current AutoCAD drawing database.
// Returns TRUE if successful and FALSE otherwise.
// Parameters:
//  szBlockName -- technically a file name, like "spt0", or if it has a path, it doesn't have ".dwg" extension.
//  adsInsertPoint -- location of the point entity.
//  fXScale        -- scale factor along x-axis
//  fYScale        -- scale factor along y-axis
//  pntDir         -- point at which the block's local x-axis should be rotated towards.
//  szPointName    -- point name (optional). Applied to attribute with "PT#" tag.
//  szElevation    -- point elevation, as a string (optional). Applied to attribute with "ELEV2" tag.
//  szDesc         -- point description (optional). Applied to attribute with "DESC2" tag.
//  pCustomAttributes -- an optional linked list of tag and value pairs to try to apply to this block.
//                       If a tag in a CustomAttribute matches a tag on the block, then the value from
//                       the CustomAttribute will be used as the value.
BOOL InsertBlock( const char *szBlockName, const ads_point adsInsertPoint,
                  const double fXScale, const double fYScale, const ads_point pntDir,
                  const char *szPointName = NULL, const char *szElevation = NULL,
                  const char *szDesc = NULL, CustomAttribute *pCustomAttributes = NULL, double fZScale = 0.0,
				  BOOL bAnnotative=FALSE CIPCSCADPARAM CIPCSCADPARAMDEFAULT);

int draw_block_common(const char *blk_nm,double scaler,double *insert_pnt,double *to_pnt, CustomAttribute *pCustomAttr=NULL);

// Point DWG API for reading and writing points to an AutoCAD drawing.
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#endif

#ifdef HAVE_CAD
////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS FOR SCLDD.cpp    ....NEED TO DEVELOP #ifdef's for
////////////////////////////////////////////////////////////////////////////
extern int ComFailed1;

extern HRESULT SCAutoWrap(int autoType, VARIANT *pvResult, IDispatch *pDisp, LPOLESTR ptName, int cArgs, ...);
extern IDispatch * GetActiveInstance();
extern IDispatch * GetC3DActiveInstance();
extern IDispatch * GetItem(IDispatch *pInObject,int number);
extern IDispatch * GetItem(IDispatch *pInObject,long number);
extern IDispatch * GetItem(IDispatch *pInObject,char *name);
extern IDispatch * PropertyGet(IDispatch *pInObject,char * buf);
extern void PropertyGet(IDispatch *pInObject,char *buf,int *returnvalue);
extern void PropertyGet(IDispatch *pInObject,char *buf,long *returnvalue);
extern void PropertyGet(IDispatch *pInObject,char *buf,double *returnvalue);
extern void PropertyGet(IDispatch *pInObject,char *buf,char *returnvalue);
extern void PropertyGet(IDispatch *pInObject,char *buf,bool *returnvalue);
extern int PropertyPut(IDispatch *pInObject,char *buf,int invalue);
extern int PropertyPut(IDispatch *pInObject,char *buf,long invalue);
extern int PropertyPut(IDispatch *pInObject,char *buf,double invalue);
extern int PropertyPut(IDispatch *pInObject,char *buf,char *invalue);
extern int PropertyPut(IDispatch *pInObject,char *buf,bool invalue);
extern ScUniRecordBase *GetLDDProjectSettings();
//extern ScUniRecordBase *GetC3DDocSettings();
extern void SetLDDNextPoint(int);
extern note_buf_scad *GetAvailableTINs();
extern note_buf_scad *GetAvailableAligns();
extern note_buf_scad *GetCivil3DTINs();
extern note_buf_scad *GetCivil3DAligns();
extern int GetAvailablePointGroups(note_buf_scad *PointGroupNamesList);
extern int GetPointList(char *PointGroupName, char *PointList);
extern int Get_XY_StationOffsetCurrent(double station, double offset, double *eas, double *nor, double *direction);
extern int Get_XY_StationOffsetNamed(double station, double offset, double *eas, double *nor, double *direction, char *align_nm);
extern int Get_StationOffsetCurrent(double eas, double nor, double *station, double *offset, double *direction);
extern int Get_StationOffsetNamed(double eas, double nor, double *station, double *offset, double *direction, char *align_nm);
extern double Get_Tin_Z_Current(double eas, double nor);
extern double Get_Tin_Z_Named(double eas, double nor, char *tin_nm);
extern note_buf_scad * GetAvailableParcels();
extern ScUniRecordBase * GetParcelDef(char *name);
extern int CreateCogoAeccPoint( IDispatch *pCogoPref, IDispatch *pCogoPoints, double eas, double nor, double elv, int number, const char *name, const char *desc, const char *szLayer, double fScale, double fPrevScale);
extern BOOL DeleteCogoAeccPoint( IDispatch *pAeccApp, long iPointNumber);
extern int CreateAeccPoint(double eas, double nor, double elv, int number, const char *szName, char *szDesc, const char *szLayer, double rotation, double textsize, short pdmode, double pdsize);
extern int ConvertTINsToFLTFile(char *surfaceName, char *filenm);
extern int ConvertC3DTINsToFLTFile(char *surfaceName, char *filenm);
extern int ConvertAlignsToCL(char *alignName, char *filenm);
extern int ConvertC3DAlignsToCL(char *alignName, char *filenm);
extern void *GetActiveAcadInstance(BOOL Silent);
extern void PolylineAddTinPoints(ScTIN *tin1,lwpoint *first);
#endif

#if (((!defined(UNDER_CE)) && (!defined(EMULATOR))) || (defined(CE_OTHER)))
//VarMapper: (justin 6-22-04)
//This class is used to map variable definitions to and from objectarx.
//Can be used for the following purposed:
//1) To read and write large ini files without having to do the knitty gritty
//2) To map variables to and from lisp
//3) To map variables to and from the dictionary (not yet implemented)
//For examples, see label.cpp in cogoutil.

class VarMapNode {
public:
  VarMapNode() {
    m_nType = 0;
    m_szVarName=NULL;
    m_vVar = NULL;
    m_nSize = 0;
    m_szDictSubNm=NULL;
    m_bSkip = FALSE;
    next=NULL;
  }

  ~VarMapNode() 
  { 
	Clear();
  };

  void Clear()
  {
	if (m_szVarName != NULL)
	{
	  free(m_szVarName);
	  m_szVarName = NULL;
	}
  }

  VarMapNode(const VarMapNode& vmp);
  VarMapNode& operator =(const VarMapNode& vmp);

  char *m_szVarName;
  void *m_vVar;
  int m_nType;
  int m_nSize;
  char *m_szDictSubNm;
  BOOL m_bSkip;
  VarMapNode *next;
};

class VarMapper {
public:
  VarMapper() {
    varList=NULL; 
    strcpy(m_szAppNm, ""); 
    strcpy(m_szFileNm, ""); 
    strcpy(m_szDictNm, ""); 
    Clear();
  };
  ~VarMapper() {Clear();};

  VarMapper(const VarMapper& vm);
  VarMapper& operator =(const VarMapper& vm);

  void Clear();
  void ReverseList(); //By defaults, adds are backwards, so call this when you're done adding
  int Add(char *szVarName, long *plVarVal, char *szSubNm=NULL);//longs
  int Add(char *szVarName, int *pnVarVal, char *szSubNm=NULL); //ints
  int Add(char *szVarName, double *pfVarVal, char *szSubNm=NULL); //doubles
  int Add(char *szVarName, const char *szVarVal, int nSize, char *szSubNm=NULL); //constant char array ptrs (preferred)
  int SetSkip(char *szVarName, BOOL bSkip);
  int RemoveAllSkips();
  int ReadIni( FILE** fpIn = NULL );    // Use and return (leave open) passed in fp if non-NULL
  int WriteIni( FILE** fpIn = NULL );   // Use and return (leave open) passed in fp if non-NULL
  int ReadLispVars();
  int WriteLispVars();
  int ReadResbuf(struct acad_resbuf *pRb, BOOL bReadAppNm=TRUE, struct acad_resbuf **pprbResRb=NULL); //read/write XData buffer
  int WriteResbuf(struct acad_resbuf *pRb, int nSize, BOOL bWriteHeader, char *szFilter=NULL, BOOL bWriteAppName=TRUE);
  int ReadDictionary();
  int WriteDictionary();
  int GetSize(void);

  char m_szFileNm[FILENMSZ];
  char m_szAppNm[200];
  char m_szDictNm[200];
private:
  VarMapNode *varList;
};

//End VarMapper (justin 6-22-04)


#endif

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// BasisChange
// A class for easily creating a new coordinate system and for converting to
//  and from it.  Uses the math library in simplify.h.  \scmath was missing vector tools
//  and the STL-based matrix felt awkward to use (too much typing) plus has a scary
//  compile warning.
//
// Original author: Ron Little
//
class BasisChange
{
public:
  BasisChange();
  ~BasisChange();

  // Uses the points in pLine to create a best plane and then form a coordinate system
  //  in that plane such that the line goes clockwise around the z-axis when looking
  //  down at the origin and pLine going around the origin (if bClockwise is TRUE).
  // TRUE is returned if a valid best plane is found. FALSE otherwise. In the case of
  //  failure, this class will be initialized with the identity matrix so that the
  //  conversion routines below will have no affect on the coordinates.
  BOOL InitBestPlane( const lwpoint *pLine, BOOL bClockwise);

  // Initializes this class so that the transformation is to the new origin, with no
  //  rotation.
  void InitTranslate( ads_point new_origin);

  // Creates the matrix for transforming to and from the given basis, including the translation.
  // v1, v2, and v3 must be linearly independent vectors.
  // Returns TRUE if successful (i.e., the desired target basis vectors are linearly independent).
  // If unsuccessful, the internal transformation matrices will be identity matrices.
  BOOL Init( const ads_point v1, const ads_point v2, const ads_point v3, const ads_point new_origin);

  void TransformTo( lwpoint *pLine);
  void TransformFrom( lwpoint *pLine);

  void TransformTo( lwpolys *pLines);
  void TransformFrom( lwpolys *pLines);

  void TransformTo( ScTIN *pTIN);
  void TransformFrom( ScTIN *pTIN);

private:
  class Implementation;
  Implementation *m_pImp; //to avoid polluting this header file, let's hide the details.
};

// end of BasisChange
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// PushSetting
// Allows you to change a variable, typically a global, and then have that variable restored
//  to its original value when the PushSetting object goes out of scope.  Was meant to work
//  with integers but it will work with any object for which the assignment operator makes sense.
// Useful for working with older code that uses global variables to pass parameters around or
//  if you have a complicated function with many return points and you need to restore some
//  global variable when the function returns.  (Ron Little, 7/18/05)
// Example:
//  /*globals*/
//  int i0, i1, i2, i3, i4; //some values from a dialog box, maybe
//  int bigfoo()
//  {
//    PushSetting<int> setting_i0( &i0, 1); //set it to 1 and then restore it to its original value when returning.
//
//    if (some_error) return 1; //i0 will be restored to its original value here
//    ...  lots of code with many returns. Every return will cause i0 to be restored.
//    return 0; //i0 will be restored to its original value here
//  }
template <typename SettingType>
class PushSetting
{
public:
  PushSetting( SettingType *pSetting, SettingType newValue)
  {
    m_oldValue = *pSetting;
    *pSetting = newValue;
    m_pSetting = pSetting;
  }
  inline ~PushSetting()
  {
    *m_pSetting = m_oldValue;
  }
private:
  SettingType *m_pSetting;
  SettingType m_oldValue;
};
// end of PushSetting
////////////////////////////////////////////////////////////////////////////

#ifdef HAVE_CAD
#ifdef ACRXAPP
  char* getMTextContents(AcDbMText* mText);
#endif
  char* getMTextContents(ads_name entityName, BOOL bKeepBreaks=FALSE);
  csString GetCurrentLayer ();

/// Simple wrapper class that handles the loading and unloading of arx files.  Basically, if the
/// arx file is already loaded then we don't want to re-load and unload it for fear of breaking
/// any other functions.  However, if the arx file is NOT loaded then we should load it and 
/// unload it when this variable goes out of scope.
class ARXLoadHandler
{
public:
  ARXLoadHandler (const csString& ARXNameNoExtension)
    : m_arxname (ARXNameNoExtension)
    , m_we_loaded (false)
    , m_valid (false)
  {
    if (IsLoadArx (m_arxname) == 0)
	{
      if (LoadArx (m_arxname) == 1)
      {
        m_we_loaded = true;
        m_valid = true;
      }
	}
    else
    {      
      m_valid = true;
    }    
  }

  ~ARXLoadHandler ()
  {
    // unload the file if we had to load it
    if (m_we_loaded)
      UnloadArx (m_arxname);
  }

  /// Returns whether the arx file is loaded or not
  bool IsValid () const
  {
    return m_valid;
  }

private:
  /// Hide the assignment operator
  ARXLoadHandler& operator = (ARXLoadHandler& lhs);

  /// Hide the copy constructor
  ARXLoadHandler (const ARXLoadHandler& c);

private:
  csString m_arxname;
  bool     m_we_loaded;
  bool     m_valid;
};
#endif

#define POINT_DWG_NAME 150  //warning, some uses of 'name' involve hard-coded lengths.
typedef struct PointDwg {
  char name[POINT_DWG_NAME];
  char desc[150];
  char category[100];
  HTREEITEM TreeItem;
  long Index;
  struct PointDwg *Next;
} PointDwg;
extern PointDwg *PointDwgRoot;
extern PointDwg *NewPointDwg();
extern void LoadPointDwgs(char *FileName);

class openglViewSettings
{
public:
    openglViewSettings()
    {
        is_first = 1;
        mode = 1;
        x = y = z = 0.0;
        maxRad = 1;
    };

public:
    int is_first;
    int mode;
    double x;
    double y;
    double z;
    double maxRad;
};

#ifndef NOCSCGL
/* Calculate arc bulge from start point, end point, and direction. */
double getBulge(const double* startPoint, const double* endPoint, double direction);
/* Calculate arc bulge from start point, end point, and a third point anywhere on the arc. */
double getBulge(const double* startPoint, const double*, const double* thirdPoint);
/* Calculate arc bulge from start point, end point, center point, and orientation. */
/* Precondition: the two radii specified by these points must be equal. */
double getBulge(const double* startPoint, const double* endPoint, const double* centerPoint, bool orientation);

// ======= centerln ======= //
// The following centerln classes are used for centerline grips.
class centerlnPt;
class centerlnCurve;
class centerlnSpiral;
class centerlnElement;
class centerln;

class centerlnPt
{
public:
	centerlnPt();
	centerlnPt(double fStation, double fX, double fY, char *pszPtnum);
	void Reset(double fStation, double fX, double fY, char *pszPtnum = NULL);
	double GetStation() { return m_fStation; };
	double GetX() { return m_fX; };
	double GetY() { return m_fY; };
	char* GetPtnum() { return m_szPtnum; };
	int GetFlag() { return m_nFlag; };
	void SetStation(double fStation) { m_fStation = fStation; };
	void SetX(double fX) { m_fX = fX; };
	void SetY(double fY) { m_fY = fY; };
	void SetPtnum(char *pszPtnum) { if (pszPtnum) strncpy(m_szPtnum, pszPtnum, 31); };
	void SetFlag(int nFlag) { if (nFlag >= 1) m_nFlag = 1; else m_nFlag = 0; };

private:
	double m_fStation;
	double m_fX;
	double m_fY;
	char m_szPtnum[32];
	int m_nFlag;
};

class centerlnCurve
{
public:
	centerlnCurve();
	~centerlnCurve();
		
	bool Update(centerlnPt *pPrevPI, centerlnPt *pPI, centerlnPt *pNextPI, centerlnPt *pPt, 
                unsigned int nMode, unsigned int isFirst = 0);
		// update radius and other points
		// all input points are not changed
		// pPt is the fixed point, is either PC or PT or curve center point
		// nMode: 0 fix PC, 1 fix PT, 2, fix curve center point

    bool Update(centerlnPt *pPrevPI, centerlnPt *pPI, centerlnPt *pNextPI, unsigned int isFirst = 0);
        // all input points are not changed
		// update PC, PT, center point and curve center point, radius doesn't change
        // isFirst: 0 the curve is between two elements
        //          1 the curve is the first element, pPrevPI is a faked PI, fix PC's station
        
	bool Process(centerlnElement *pElePrev, centerlnElement *pEleNext, centerlnPt *pPI);
		// radius and PI are not changed

	centerlnPt *GetPC() { return m_pPC; };
	centerlnPt *GetPT() { return m_pPT; };
	centerlnPt *GetCenter() { return m_pCenter; };
	centerlnPt *GetCurveCenter() { return m_pCurveCenter; };
    centerlnPt *GetPrevPI() { return m_pPrevPI; };
    centerlnPt *GetNextPI() { return m_pNextPI; };
	double GetRadius() { return m_fRadius; };
	double GetDeltaAng() { return m_fDeltaAng; };
	double GetInBearing() { return m_fInBearing; };
	double GetOutBearing() { return m_fOutBearing; };
	void SetPC(centerlnPt *p) { m_pPC = p; };
	void SetPT(centerlnPt *p) { m_pPT = p; };
	void SetCenter(centerlnPt *p) { m_pCenter = p; };
	void SetCurveCenter(centerlnPt *p) { m_pCurveCenter = p; };
    void SetPrevPI(centerlnPt *p) { m_pPrevPI = p; };
    void SetNextPI(centerlnPt *p) { m_pNextPI = p; };
	void SetRadius(double fRadius) { m_fRadius = fRadius; };
	void SetDeltaAng(double fAng) { m_fDeltaAng = fAng; };
	void SetInBearing(double fBearing) { m_fInBearing = fBearing; };
	void SetOutBearing(double fBearing) { m_fOutBearing = fBearing; };

private:
	centerlnPt *m_pPC;
	centerlnPt *m_pPT;
	centerlnPt *m_pCenter;
	centerlnPt *m_pCurveCenter;
    centerlnPt *m_pPrevPI;      // faked PI, used when centerline starts with a curve
    centerlnPt *m_pNextPI;      // faked PI, used when centerline ends with a curve
	double m_fRadius;
	double m_fDeltaAng;
	double m_fInBearing;
	double m_fOutBearing;
};

class centerlnSpiral
{
public:
	centerlnSpiral();
	~centerlnSpiral();
	bool Update(centerlnPt *pPrevPI, centerlnPt *pPI, centerlnPt *pNextPI);
		// update TS, SC, CS, ST, center point and curve center point, radius, spiral-in and spiral-out don't change

	bool Update(centerlnPt *pPrevPI, centerlnPt *pPI, centerlnPt *pNextPI, centerlnPt *pPt);
		// update radius and other points
		// all input points are not changed
		// pPt is the curve center point

	bool Process(centerlnElement *pElePrev, centerlnElement *pEleNext, centerlnPt *pPI);
		// radius, spiral length and PI are not changed

	centerlnPt *GetTS() { return m_pTS; };
	centerlnPt *GetSC() { return m_pSC; };
	centerlnPt *GetCS() { return m_pCS; };
	centerlnPt *GetST() { return m_pST; };
	centerlnPt *GetCenter() { return m_pCenter; };
	centerlnPt *GetCurveCenter() { return m_pCurveCenter; };
	double GetRadius() { return m_fRadius; };
	double GetDeltaAng() { return m_fDeltaAng; };
	double GetCirDeltaAng() { return m_fCirDeltaAng; };
	double GetSpiDeltaAngIn() { return m_fSpiDeltaAngIn; };
	double GetSpiDeltaAngOut() { return m_fSpiDeltaAngOut; };
	double GetInBearing() { return m_fInBearing; };
	double GetOutBearing() { return m_fOutBearing; };
	double GetSpiralIn() { return m_fSpiralIn; };
	double GetSpiralOut() { return m_fSpiralOut; };
	void SetTS(centerlnPt *p) { m_pTS = p; };
	void SetSC(centerlnPt *p) { m_pSC = p; };
	void SetCS(centerlnPt *p) { m_pCS = p; };
	void SetST(centerlnPt *p) { m_pST = p; };
	void SetCenter(centerlnPt *p) { m_pCenter = p; };
	void SetCurveCenter(centerlnPt *p) { m_pCurveCenter = p; };
	void SetRadius(double fRadius) { m_fRadius = fRadius; };
	void SetDeltaAng(double fAng) { m_fDeltaAng = fAng; };
	void SetCirDeltaAng(double fVal) { m_fCirDeltaAng = fVal; };
	void SetSpiDeltaAngIn(double fVal) { m_fSpiDeltaAngIn = fVal; };
	void SetSpiDeltaAngOut(double fVal) { m_fSpiDeltaAngOut = fVal; };
	void SetInBearing(double fBearing) { m_fInBearing = fBearing; };
	void SetOutBearing(double fBearing) { m_fOutBearing = fBearing; };
	void SetSpiralIn(double fVal) { m_fSpiralIn = fVal; };
	void SetSpiralOut(double fVal) { m_fSpiralOut = fVal; };

private:
	centerlnPt *m_pTS;
	centerlnPt *m_pSC;
	centerlnPt *m_pCS;
	centerlnPt *m_pST;
	centerlnPt *m_pCenter;
	centerlnPt *m_pCurveCenter;
	double m_fRadius;
	double m_fDeltaAng;
	double m_fCirDeltaAng;
	double m_fSpiDeltaAngIn;
	double m_fSpiDeltaAngOut;
	double m_fInBearing;
	double m_fOutBearing;
	double m_fSpiralIn;
	double m_fSpiralOut;
};

class centerlnElement
{
public:
	centerlnElement();
	centerlnElement(double x, double y, double radius, double spiral1, double spiral2);
	~centerlnElement();
	centerlnElement *GetNext() { return m_pNext; };
	centerlnPt *GetPI() { return m_pPI; };
	centerlnCurve *GetCurve() { return m_pCurve; };
	centerlnSpiral *GetSpiral() { return m_pSpiral; };
	void SetPI(centerlnPt *p) { m_pPI = p; };
	void SetCurve(centerlnCurve *p) { m_pCurve = p; };
	void SetSpiral(centerlnSpiral *p) { m_pSpiral = p; };

private:
	// PI information
	centerlnPt *m_pPI;
	centerlnCurve *m_pCurve;
	centerlnSpiral *m_pSpiral;
	centerlnElement *m_pNext;

friend class centerln;
};

class centerln
{
public:
	centerln();
	centerln(cline *pclRoot, sta_equation *pEquation = NULL);
	~centerln();
	centerlnElement *GetCenterlnRoot() { return m_pRoot; };
	centerlnPt *GetCenterlnPt(int nIndex);
	sta_equation *GetStaEquation() { return m_pEquation; };
	int GetIndex(centerlnPt *p);
	int GetClinkIndex(centerlnPt *p);
	void Append(centerlnElement *p);
	void Process(double fStartStation = 0.0);
	void ClearFlag();
	void Update(centerlnPt *pEdit);
		// update whole centerline data based on the input centerline point
		// pEdit:	Line: PI
		//			Curve: PI, PC, PT, Curve Center
		//			Spiral-Curve-Spiral: PI, Curve Center
	cline *MakeCline();
	void Write(const char *szFile);
	BOOL IsChanged() { return m_bChanged; };
	void SetChanged(BOOL bChanged) { m_bChanged = bChanged; };
    void CheckCurve();  // Check if the centerline starts or ends with a curve. If so, make the faked PI points.
    void Rotate(ads_point basePt, double angle);

private:
	centerlnElement *m_pRoot;
	sta_equation *m_pEquation;
	BOOL m_bChanged;
};


// ======= end of centerln ======= //

void calc_ctr(double *pt1,double *pt2,double radius,double ang,double *ctr);

#endif

// ===================================================================================
// Super elevation read/write method (moved from EWORKS.CPP) to use else where - Dinesh
// ===================================================================================
typedef struct super_rec {
  double speed;
  double norm_slope;
  double beg_tan;
  double beg_tan_vc;
  double beg_run;
  double beg_run_vc;
  double norm_in;
  double norm_in_vc;
  double max_slope;
  double beg_full;
  double beg_full_vc;
  double end_full;
  double end_full_vc;
  double norm_out;
  double norm_out_vc;
  int is_reverse;
  double end_run;
  double end_run_vc;
  double end_tan;
  double end_tan_vc;
  int is_compound;
  double max_slope2;
  double beg_full2;
  double beg_full2_vc;
  double end_full2;
  double end_full2_vc;
  struct super_rec *next;
} super_rec;
super_rec *new_super_rec();
void write_super_file(const char *pszSupFile, super_rec *pSuperRecRoot, double fMaxSlope, double fGlobalDesignSpeed);
super_rec* read_super_file(const char *pszSupFile, double& fMaxSlope, double& fGlobalDesignSpeed);

#endif // _SCLIB_H_ : Do not put any code below this line
