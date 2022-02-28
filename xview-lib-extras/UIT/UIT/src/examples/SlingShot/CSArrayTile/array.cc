#include "BaseWindow.h"
#include "CanvasShellDisplay.h"
#include "CSArrayTile.h"
#include "CSBox.h"
#include "CSClock.h"
#include "CSDrawText.h"
#include "Notifier.h"

void selection_handler (UIObject *object)
{
  char *value;

  value = object->getValue (value);
  printf ("selection_handler: %s %s\n",
	  value,
	  (((CSComponent *) object)->isSelected () ?
	   "selected" : "deselected"));

  delete value;
}

int main (int argc, char **argv)
{
  BaseWindow bw;
  bw.initUI (argc, argv);
  bw.setLabel ("CSArrayTile: Select a tile, any tile");
 
  CanvasShellDisplay shell;
  shell.setDestroyXViewObject (FALSE);
  bw.addDisplay (shell);

  CSBox b1;
  b1.setDestroyXViewObject (FALSE);
  b1.setBorder (10);
  b1.setLayout (VERTICAL);
  shell.addComponent (b1);

  CSDrawText title;
  title.setDestroyXViewObject (FALSE);
  title.setValue ("Demonstration of using CSDrawText for a title");
  title.setJustification (JUSTIFY_CENTER);
  title.setSelectable (FALSE);
  title.setWidth (200);
  b1.addChild (title);

  CSBox b2;
  b2.setDestroyXViewObject (FALSE);
  b2.setBorder (10);
  b2.setGap (0);
  b2.setLayout (HORIZONTAL);
  b1.addChild (b2);

  CSArrayTile *at;
  CSClock     *cl;
  CSDrawText  *dt;

  for (int i=0; i < 10; i++) {
    at = new CSArrayTile;
    at->setDestroyXViewObject (FALSE);
    at->setDisplayColumns (1);
    at->setDisplayRows (10);
    at->setColumnGap (0);
    at->setColumnWidth (0);
    at->setHorizontalLines (TRUE);
    at->setAlignment (ARRAY_TILE_ALIGN_WEST);
    b2.addChild (*at);

    if (i > 0) {
      for (int j=0; j < 10; j++) {
	char temp_string [40];

	sprintf (temp_string, "(%d, %d)", j, i-1);

	dt = new CSDrawText;
        dt->setDestroyXViewObject (FALSE);
	dt->setValue (temp_string);
	dt->setDisplayLength (i);
	dt->setHeight (20);
	dt->setWidth (40 + i * 4);
	dt->setSelectable (TRUE);
	dt->setExclusiveSelect (TRUE);
	dt->setSelectionHandler (selection_handler);
	at->addChild (*dt);
      }
    }
    else {
      for (int j=0; j < 10; j++) {
        cl = new CSClock;
        cl->setDestroyXViewObject (FALSE);
        cl->setHeight (20);
	cl->setWidth (20);
	cl->setMinute (j * 5);
	cl->setHour (10);
	at->addChild (*cl);
      }
    }
  }

  shell.createXViewObjects ();
  shell.setWidth (b1.getWidth ());
  shell.setHeight (b1.getHeight ());

  bw.setFit (WIDTH);
  bw.setFit (HEIGHT);

  Notifier notifier;
  notifier.start ();

  exit (0);
} 
