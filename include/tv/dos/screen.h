/**[txh]********************************************************************

  Copyright (c) 2001-2002 by Salvador E. Tropea

  Description: 
  Display and Screen `driver' classes for DOS.
  
***************************************************************************/

#ifndef DOS_SCREEN_HEADER_INCLUDED
#define DOS_SCREEN_HEADER_INCLUDED
// virtual to avoid problems with multiple inheritance
class TDisplayDOS : virtual public TDisplay
{
protected:
 TDisplayDOS() {};
 // We will use casts to base classes, destructors must be pointers
 virtual ~TDisplayDOS();
 // This sets the pointers of TDisplay to point to this class
 static void   Init();

 // Behaviors to replace TDisplay
 static void   SetCursorPos(unsigned x, unsigned y);
 static void   GetCursorPos(unsigned &x, unsigned &y);
 static void   GetCursorShape(unsigned &start, unsigned &end);
 static void   SetCursorShape(unsigned start, unsigned end);
 static void   ClearScreen(uchar, uchar);
 static ushort GetRows();
 static ushort GetCols();
 static void   SetCrtMode(ushort mode);
 static ushort GetCrtMode();
 static void   SetCrtModeExt(char *mode);
 // Not available static int    CheckForWindowSize(void);
 static
 const char   *GetWindowTitle(void);
 static int    SetWindowTitle(const char *name);

 // Function members for this driver
 // Calls the BIOS 0x10 interrupt (Video service)
 static void videoInt();
 // Finds the number of lines per char
 static int  getCharLines();
 // Special videoInt to workaround a bug in NT's TXProII driver
 static void setVideoModeInt();
 // Selects a text mode, it detects if the mode is VESA
 static void setTextMode();
 // Select the number of scanlines and font to use.
 // Used to set extended video modes, like 80x50.
 static void setExtendedMode(int mode);
 // Is used to test if a mode that we don't know can be handled by the driver
 // or not. If the mode uses an unknown memory region we just assume that's
 // a graphics mode and revert to 80x25. It could be enhanced.
 static void testForSupport();
 // Selects a VESA or user provided video mode.
 static void setTextMode(int mode);
 // Sets a tweaked video mode
 static int  setTweakedMode(int mode);

 // Data members for this driver
 static __dpmi_regs rDisplay;
 static unsigned    charLines;
 static char        emulateMouse;
};


// With this order the destructor will be called first for TScreenDOS,
// TScreen, TDisplayDOS and finally TDisplay.
class TScreenDOS : public TDisplayDOS, public TScreen
{
public:
 TScreenDOS();
 // We will use casts to base classes, destructors must be pointers
 virtual ~TScreenDOS();

 // Is that really used?
 static int    slowScreen;

protected:
 static void   Resume();
 static void   Suspend();
 static void   setCrtData();
 static void   clearScreen();
 static void   getCharacters(unsigned offset,ushort *buf,unsigned count);
 static ushort getCharacter(unsigned dst);
 static void   setCharacter(unsigned offset,ushort value);
 static void   setCharacters(unsigned dst,ushort *src,unsigned len);
 static int    System(const char *command, pid_t *pidChild);

 // Status before suspending
 static int    wasBlink;
};

#ifdef TSCREEN_DEFINE_REGISTERS
#define r (rDisplay)
#define AL (r.h.al)
#define BL (r.h.bl)
#define CL (r.h.cl)
#define DL (r.h.dl)
#define AH (r.h.ah)
#define BH (r.h.bh)
#define CH (r.h.ch)
#define DH (r.h.dh)
#define AX (r.x.ax)
#define BX (r.x.bx)
#define CX (r.x.cx)
#define DX (r.x.dx)
#define DI (r.x.di)
#define SI (r.x.si)
#define ES (r.x.es)
#endif // TSCREEN_DEFINE_REGISTERS

#endif // DOS_SCREEN_HEADER_INCLUDED
