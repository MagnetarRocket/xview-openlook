#include "CSDrawImage.h"

void drag_handler (UIObject *object)
{
  switch (object->getDragStatus ()) {
    case DRAG_NO_DRAG:
      printf ("No drag operation\n");
      break;
    case DRAG_START:
      printf ("Drag start\n");
      break;
    case DRAG_CONVERT:
      printf ("Drag convert\n");
      break;
    case DRAG_DELETE:
      printf ("Drag delete\n");
      break;
    case DRAG_END:
      printf ("Drag end\n");
      break;
    default:
      printf ("Unknown drag status\n");
      break;
  }

  return;
}
