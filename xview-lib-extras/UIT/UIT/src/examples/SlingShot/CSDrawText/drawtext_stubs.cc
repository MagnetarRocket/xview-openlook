#include "CSDrawText.h"

void text_handler (UIObject *object)
{
  char *value;

  value = object->getValue (value);
  printf ("text_handler: value: %s\n", value);

  delete value;
  return;
}
