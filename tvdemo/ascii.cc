/*----------------------------------------------------------*/
/*                                                          */
/*   Turbo Vision 1.0                                       */
/*   Copyright (c) 1991 by Borland International            */
/*                                                          */
/*   Ascii.cpp: Member functions of following classes:      */
/*                TTable                                    */
/*                TReport                                   */
/*                TAsciiChart                               */
/*----------------------------------------------------------*/

/* Modified by Robert Hoehne to be used with RHIDE */

#define Uses_TRect
#define Uses_TEvent
#define Uses_TKeys
#define Uses_TDrawBuffer
#define Uses_TStreamableClass
#define Uses_TStreamable
#define Uses_TView
#define Uses_TWindow
#include <tv.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <ascii.h>


//
// TTable functions
//

TTable::TTable(TRect & r):
TView(r)
{
}


void
TTable::draw()
{
  TDrawBuffer buf;
  char color = getColor(6);

  for (int y = 0; y <= size.y - 1; y++)
  {
    buf.moveChar(0, ' ', color, size.x);
    for (int x = 0; x <= size.x - 1; x++)
      buf.moveChar(x, 32 * y + x, color, 1);
    writeLine(0, y, size.x, 1, buf);
  }
  showCursor();
}

//
// cmCharFocused is a offset value (basically the ascii code of the
// current selected character) thus should be added, not or'ed, to
// cmAsciiTableCmdBase.
//

void
TTable::charFocused()
{
  message(owner, evBroadcast, cmAsciiTableCmdBase + cmCharFocused,
          (void *) (cursor.x + 32 * cursor.y));
}


void
TTable::handleEvent(TEvent & event)
{
  TView::handleEvent(event);

  if (event.what == evMouseDown)
  {
    do
    {
      if (mouseInView(event.mouse.where))
      {
        TPoint spot = makeLocal(event.mouse.where);

        setCursor(spot.x, spot.y);
        charFocused();
      }
    }
    while (mouseEvent(event, evMouseMove));
    clearEvent(event);
  }
  else
  {
    if (event.what == evKeyboard)
    {
      switch (event.keyDown.keyCode)
      {
        case kbHome:
          setCursor(0, 0);
          break;
        case kbEnd:
          setCursor(size.x - 1, size.y - 1);
          break;
        case kbUp:
          if (cursor.y > 0)
            setCursor(cursor.x, cursor.y - 1);
          break;
        case kbDown:
          if (cursor.y < size.y - 1)
            setCursor(cursor.x, cursor.y + 1);
          break;
        case kbLeft:
          if (cursor.x > 0)
            setCursor(cursor.x - 1, cursor.y);
          break;
        case kbRight:
          if (cursor.x < size.x - 1)
            setCursor(cursor.x + 1, cursor.y);
          break;
        default:
          setCursor(event.keyDown.charScan.charCode % 32,
                    event.keyDown.charScan.charCode / 32);
          break;
      }
      charFocused();
      clearEvent(event);
    }
  }
}


//
// TReport functions
//

TReport::TReport(TRect & r):
TView(r)
{
  asciiChar = 0;
}


void
TReport::draw()
{
  TDrawBuffer buf;
  char color = getColor(6);
  char str[80];

  sprintf(str, "%s%c%s%3d%s%02X",
          _("  Char: "), (asciiChar == 0) ? (char) 0x20 : (char) asciiChar,
          _(" Decimal: "), (int) asciiChar, _(" Hex "), (int) asciiChar);

  buf.moveChar(0, ' ', color, size.x);
  buf.moveStr(0, str, color);
  writeLine(0, 0, 32, 1, buf);
}


void
TReport::handleEvent(TEvent & event)
{
  TView::handleEvent(event);
  if (event.what == evBroadcast)
  {
    if (event.message.command == cmAsciiTableCmdBase + cmCharFocused)
    {
      asciiChar = event.message.infoLong;
      drawView();
      clearEvent(event);
    }
  }
}


//
// TAsciiChart functions
//

TAsciiChart::TAsciiChart():
TWindow(TRect(0, 0, 34, 12), _("ASCII Chart"), wnNoNumber),
TWindowInit(&TAsciiChart::initFrame)
{
  TView *control;

  flags &= ~(wfGrow | wfZoom);
  palette = wpGrayWindow;

  TRect r = getExtent();

  r.grow(-1, -1);
  r.a.y = r.b.y - 1;
  control = new TReport(r);
  control->options |= ofFramed;
  control->eventMask |= evBroadcast;
  insert(control);

  r = getExtent();
  r.grow(-1, -1);
  r.b.y = r.b.y - 2;
  control = new TTable(r);
  control->options |= ofFramed;
  control->blockCursor();
  insert(control);

  control->select();
}
