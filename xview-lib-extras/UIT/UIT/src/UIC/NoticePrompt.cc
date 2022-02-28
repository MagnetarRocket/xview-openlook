/*H+ NoticePrompt.cc
*
*    SCCS ID: @(#)NoticePrompt.cc 1.4 92/05/23 SMI
*
*    Copyright (c) 1990-1992 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the NoticePrompt class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*    MGS       05/10/91   Preparation for code review. 
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)NoticePrompt.cc 1.4 92/05/23 SMI"; 
#endif

// Includes
#ifdef SVR4
#include <poll.h>
#endif

#include "BaseWindow.h"
#include "CharacterFont.h"
#include "NoticePrompt.h"

/*F+ NoticePrompt::NoticePrompt (char *msg, ...)
*
*    PURPOSE:
*
*    NoticePrompt constructor.
*
*    PARAMETERS:
*
*    msg - The first line of a message to be displayed in the NoticePrompt.
*    ... - Additional lines of messages to be displayed in the NoticePrompt.
*
*    NOTES:
*
*    The argument list must be NULL terminated.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
NoticePrompt::NoticePrompt (char *msg, ...)
{
  va_list      ap;
  char        *arg;
  char 	      *message;
  int          size;
  GenericList  temp_list;
  
  // Get the first argument in the arg list
  va_start (ap, msg);
  arg = msg;
  
  // While the arg is not NULL...
  while (arg) {
    // Copy the string argument and put into a list
    message = strcpy (new char [strlen (arg) + 1], arg);
    temp_list.addItem ((Generic *) message);

    // Get the next argument from the  arg list
    arg = va_arg (ap, char *);
  }

  va_end (ap);

  // If the list has strings in it...
  if (size = temp_list.getSize ()) {
    // Create a string array containing the strings in the list
    messages = new char* [size+1];
    for (int i=0; i < size; i++)
      messages [i] = (char *) temp_list [i];
    messages [size] = (char *) NULL;

    // Clear the list
    temp_list.dropItems ();
  }

  // Initialize data members
  beepFlag = TRUE;
  yesLabel = (char *) NULL;
  noLabel = (char *) NULL;
  focusX = UIC_NOT_SET;
  focusY = UIC_NOT_SET;
  triggerEvent = UIC_NOT_SET;
}

/*F+ void NoticePrompt::setButtonLabels (char *button_label, ...)
*
*    PURPOSE:
*
*    Allows specification of labels for the NoticePrompt buttons.
*
*    PARAMETERS:
*
*    button_label - The label for the first NoticePrompt button.
*    ...          - Labels for additional NoticePrompt buttons.
*
*    NOTES:
*
*    The argument list must be NULL terminated.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NoticePrompt::setButtonLabels (char *button_label, ...)
{
  va_list  ap;
  char    *arg;
  char 	  *label_entry;
  
  // Get the first arguemnt in the argument list
  va_start (ap, button_label);
  arg = button_label;
  
  // While the argument is not NULL
  while (arg) {
    // Copy the string argument and put it in a list
    label_entry = strcpy (new char [strlen (arg) + 1], arg);
    labels.addItem ((Generic *) label_entry);

    // Get the next argument
    arg = va_arg (ap, char *);
  }

  va_end (ap);

  // If a "YES" label was previously specified...
  if (yesLabel) {
    // We no longer need it...
    delete yesLabel;
    yesLabel = (char *) NULL;
  }

  // If a "NO" label was previously specified...
  if (noLabel) {
    // We no longer need it...
    delete noLabel;
    noLabel = (char *) NULL;
  }
}

/*F+ int NoticePrompt::addButtonLabel (char *button_label)
*
*    PURPOSE:
*
*    Allows the addition of a button label for a NoticePrompt button.
*    The button containing this label is added to the list of buttons
*    to be displayed in the NoticePrompt.
*
*    PARAMETERS:
*
*    button_label - The label for the NoticePrompt button.
*
*    RETURN VALUES:
*
*    Returns the position of the label in the list of labels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int NoticePrompt::addButtonLabel (char *button_label)
{
  // Copy and add the button label to a list of labels
  labels.addItem ((Generic *)
		  strcpy (new char [strlen (button_label) + 1], button_label));

  // If a "YES" label was previously specified...
  if (yesLabel) {
    // We no longer need it...
    delete yesLabel;
    yesLabel = (char *) NULL;
  }

  // If a "NO" label was previously specified...
  if (noLabel) {
    // We no longer need it...
    delete noLabel;
    noLabel = (char *) NULL;
  }

  // Return the position of the new label
  return (labels.getSize ());
}

/*F+ void NoticePrompt::deleteButtonLabel (int position)
*
*    PURPOSE:
*
*    Allows the deletion of the specified button label from the list
*    of NoticePrompt button labels.
*
*    PARAMETERS:
*
*    position - The position of the label in the list of labels.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NoticePrompt::deleteButtonLabel (int position)
{
  int num_labels = labels.getSize ();
    
  // If the specified label exists...
  if ((position > 0) && (position <= num_labels))
    // Remove it from the list and delete it
    delete ((char *) labels.removeItem (position - 1));
}

/*F+ void NoticePrompt::deleteButtonLabel (char *button_label)
*
*    PURPOSE:
*
*    Allows the deletion of the specified button label from the list
*    of NoticePrompt button labels.
*
*    PARAMETERS:
*
*    button_label - The label of the button to delete.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NoticePrompt::deleteButtonLabel (char *button_label)
{
  int num_labels = labels.getSize ();

  // Loop through all of the labels...
  for (int i=0; i < num_labels; i++) {
    char *label_entry = (char *) labels [i];

    // If this is the specified label...
    if (!strcmp (label_entry, button_label)) {
      // Remove it from the list and delete it
      delete ((char *) labels.removeItem (i));
      break;
    }
  }
}

/*F+ void NoticePrompt::setMessages (char *msg, ...)
*
*    PURPOSE:
*
*    Specifies a list of messages to be displayed by the NoticePrompt.
*
*    PARAMETERS:
*
*    msg - The first line of a message to be displayed in the NoticePrompt.
*    ... - Additional lines of messages to be displayed in the NoticePrompt.
*
*    NOTES:
*
*    The argument list must be NULL terminated.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void NoticePrompt::setMessages (char *msg, ...)
{
  va_list      ap;
  char        *arg;
  char 	      *message;
  int          size;
  GenericList  temp_list;
  
  // Get the first argument from the arg list
  va_start (ap, msg);
  arg = msg;
  
  // While the arg is not NULL
  while (arg) {
    // Copy the string argument and put in a list
    message = strcpy (new char [strlen (arg) + 1], arg);
    temp_list.addItem ((Generic *) message);

    // Get the next argument
    arg = va_arg (ap, char *);
  }

  va_end (ap);

  // If there are items in the list...
  if (size = temp_list.getSize ()) {
    // Delete the old internal message string array
    deleteMessages ();

    // Create a new array containing the strings in the list
    messages = new char* [size+1];
    for (int i=0; i < size; i++)
      messages [i] = (char *) temp_list [i];
    messages [size] = (char *) NULL;

    // Clear the list
    temp_list.dropItems ();
  }
}

/*F+ int NoticePrompt::show (UIObject *parent)
*
*    PURPOSE:
*
*    Displays the NoticePrompt and returns the value of the user interaction
*    with the prompt.
*
*    PARAMETERS:
*
*    parent - The parent object or the object that the NoticePrompt
*             is being displayed for.
*
*    RETURN VALUES:
*
*    Returns a value corresponding to the choice selected:
*
*         NOTICE_YES       - The "yes" button was selected.
*         NOTICE_NO        - The "no" button was selected.
*         NOTICE_TRIGGERED - The trigger event occurred.
*         NOTICE_FAILED    - The NoticePrompt failed.
*         1-5              - The number of the button the user 
*                            selected, if button labels were specified.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int NoticePrompt::show (UIObject *parent)
{
  int         result;

#ifdef SVR4
  /* Sleep for .1 seconds so that event information is clear
     before "notice_prompt" is called. */
  sigset_t sigset;
 
  sigemptyset (&sigset);
  sigaddset (&sigset, SIGIO);
  sigaddset (&sigset, SIGALRM);
 
  sigprocmask (SIG_BLOCK, &sigset, (sigset_t *) 0);
  poll ((struct pollfd *) 0, 0, (int) 100);
  sigprocmask (SIG_UNBLOCK, &sigset, (sigset_t *) 0);
#else
  /* Sleep for .1 seconds so that event information is clear
     before "notice_prompt" is called. */
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000;

  int mask = sigmask (SIGIO) | sigmask (SIGALRM);
  int oldmask = sigblock (mask);

  select (0, 0, 0, 0, &tv);
  sigsetmask (oldmask);
#endif
  
  // If button labels were specified...
  if (labels.getSize ())
    // Create an XView notice prompt with personalized labels
    result = createNoticePromptWithLabels (parent);
  // Else, use the XView default labels or "YES" and "NO" labels...
  else {
    Event      *event = (Event *) NULL;
    UICGeneric  font_pointer = (UICGeneric) NULL;
    InputEvent *input_event;
    UICGeneric  notice_font = (UICGeneric) NULL;
    UICGeneric  object = (UICGeneric) NULL;

    /* If the parent object has event information (the prompt is being
       displayed from a handler)... */
    if (input_event = parent->getEvent ())
      event = input_event->getXViewEvent ();

    // If this object's frame is iconized...
    if (parent->isClosed ()) {
      // The XView parent will be the icon
      object = (UICGeneric) xv_get (parent->getXViewObject (),
				    FRAME_ICON);
    }
    // Else if the frame isn't closed...
    else {
      // Get the the notice parent that is saved in key data
      UIObject *notice_parent = (UIObject *) xv_get (parent->getXViewObject (),
						     XV_KEY_DATA,
						     UIC_NOTICE_KEY);
      if (notice_parent) object = notice_parent->getXViewObject ();
    }

    // If no XView parent has been determined yet...
    if (!object)
      // Use the frame associated with the notice prompt parent
      object = (UICGeneric) xv_get (parent->getXViewObject (),
				    XV_KEY_DATA,
				    UIC_FRAME_KEY);

    // If a font is specified...
    if (font) {
      // Create the XView font object
     ((UIObject *) font)->createObject (parent);

      /* Set the notice font attribute name and value.

	 NOTE: If "notice_font" is not set, it will be NULL and
	       is detected as the end of the argument list when
	       the XView "notice_prompt" function is called. */
      notice_font = (UICGeneric) NOTICE_FONT;
      font_pointer = (UICGeneric) font->getXViewObject ();
    }

    /* This massive if-else statement is handling every combination
       the following attributes having been or not been set:

       "YES" and "NO" labels:                              yesLabel, noLabel
       Trigger Event (see NOTICE_TRIGGER in XView manual): triggerEvent
       User Specified X-Y Focus Location:                  focusX, focusY
       Default X-Y Focus Location:                         event */
    if (yesLabel) {
      if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
	result = notice_prompt (object, &noticeEvent,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, yesLabel,
				NOTICE_BUTTON_NO,  noLabel,
				NOTICE_FOCUS_XY,   focusX, focusY,
				NOTICE_TRIGGER,    triggerEvent,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (focusX != UIC_NOT_SET)
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, yesLabel,
				NOTICE_BUTTON_NO,  noLabel,
				NOTICE_FOCUS_XY,   focusX, focusY,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (triggerEvent != UIC_NOT_SET)
	result = notice_prompt (object, &noticeEvent,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, yesLabel,
				NOTICE_BUTTON_NO,  noLabel,
				NOTICE_FOCUS_XY,   event_x (event),
				                   event_y (event),
				NOTICE_TRIGGER,    triggerEvent,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (event)
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, yesLabel,
				NOTICE_BUTTON_NO,  noLabel,
				NOTICE_FOCUS_XY,   event_x (event),
				                   event_y (event),
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, yesLabel,
				NOTICE_BUTTON_NO,  noLabel,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
    }
    else {
      if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
	result = notice_prompt (object, &noticeEvent,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, "Yes",
				NOTICE_BUTTON_NO,  "No",
				NOTICE_FOCUS_XY,   focusX, focusY,
				NOTICE_TRIGGER,    triggerEvent,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (focusX != UIC_NOT_SET)
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, "Yes",
				NOTICE_BUTTON_NO,  "No",
				NOTICE_FOCUS_XY,   focusX, focusY,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (triggerEvent != UIC_NOT_SET)
	result = notice_prompt (object, &noticeEvent,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, "Yes",
				NOTICE_BUTTON_NO,  "No",
				NOTICE_FOCUS_XY,   event_x (event),
				                   event_y (event),
				NOTICE_TRIGGER,    triggerEvent,
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else if (event)
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, "Yes",
				NOTICE_BUTTON_NO,  "No",
				NOTICE_FOCUS_XY,   event_x (event),
			                           event_y (event),
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
      else
	result = notice_prompt (object, NULL,
				NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
				NOTICE_BUTTON_YES, "Yes",
				NOTICE_BUTTON_NO,  "No",
				NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
				notice_font,       font_pointer,
				NULL);
    }
  }

  lastEvent.setXViewEvent (&noticeEvent);

  // Return the result of the user interaction
  return result;
}

/*F+ int NoticePrompt::createNoticePromptWithLabels (UIObject *parent)
*
*    PURPOSE:
*
*    Private member function used by the NoticePrompt::show member function
*    to create and display a NoticePrompt using the list of user specified
*    labels.
*
*    PARAMETERS:
*
*    parent - The parent object or the object that the NoticePrompt
*             is being displayed for.
*
*    RETURN VALUES:
*
*    Returns a value corresponding to the choice selected:
*
*         NOTICE_YES       - The "yes" button was selected.
*         NOTICE_NO        - The "no" button was selected.
*         NOTICE_TRIGGERED - The trigger event occurred.
*         NOTICE_FAILED    - The NoticePrompt failed.
*         1-5              - The number of the button the user 
*                            selected, if button labels were specified.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int NoticePrompt::createNoticePromptWithLabels (UIObject *parent)
{
  Event      *event = (Event *) NULL;
  UICGeneric  font_pointer = (UICGeneric) NULL;
  InputEvent *input_event;
  UICGeneric  notice_font = (UICGeneric) NULL;
  int         num_labels = labels.getSize ();
  UICGeneric  object = (UICGeneric) NULL;
  int         result;

  event_init (&noticeEvent);

  /* If event information exists for the parent object (it exists
     if the NoticePrompt is being displayed from a handler)... */
  if (input_event = parent->getEvent ())
    event = input_event->getXViewEvent ();

  // If the object's frame is iconized...
  if (parent->isClosed ()) {
    // The parent XView object is the icon
    object = (UICGeneric) xv_get (parent->getXViewObject (),
				  FRAME_ICON);
  }
  // Else if the frame isn't closed...
  else {
    // Get the the notice parent that is saved in key data
    UIObject *notice_parent = (UIObject *) xv_get (parent->getXViewObject (),
						   XV_KEY_DATA,
						   UIC_NOTICE_KEY);
    if (notice_parent) object = notice_parent->getXViewObject ();
  }

  // If no parent XView object has been determined...
  if (!object)
    // Get the frame corresponding to the NoticePrompt parent
    object = (UICGeneric) xv_get (parent->getXViewObject (),
				  XV_KEY_DATA,
				  UIC_FRAME_KEY);

  // If a font is specified...
  if (font) {
    // Create the XView font object
    ((UIObject *) font)->createObject (parent);

    /* Set the notice font attribute name and value.
       
       NOTE: If "notice_font" is not set, it will be NULL and
       is detected as the end of the argument list when
       the XView "notice_prompt" function is called. */
    notice_font = (UICGeneric) NOTICE_FONT;
    font_pointer = (UICGeneric) font->getXViewObject ();
  }

  /* This massive switch and if-else construct does the following:

     The switch chooses the case that specifies the number of user
     specified labels up to 5 labels.  Inside of the specific case,
     the if-else handles every combination the following attributes
     having been or not been set:

     Trigger Event (see NOTICE_TRIGGER in XView manual): triggerEvent
     User Specified X-Y Focus Location:                  focusX, focusY
     Default X-Y Focus Location:                         event */
  switch (num_labels) {
  case 1:
    if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (focusX != UIC_NOT_SET)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (triggerEvent != UIC_NOT_SET)
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (event)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);

    break;
  case 2:
    if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (focusX != UIC_NOT_SET)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (triggerEvent != UIC_NOT_SET)
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (event)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    
    break;
  case 3:
    if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (focusX != UIC_NOT_SET)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (triggerEvent != UIC_NOT_SET)
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (event)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    
    break;
  case 4:
    if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (focusX != UIC_NOT_SET)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (triggerEvent != UIC_NOT_SET)
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (event)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    
    break;
  case 5:
  default:
    if ((focusX != UIC_NOT_SET) && (triggerEvent != UIC_NOT_SET))
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_BUTTON,     (char *) labels [4], 5,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (focusX != UIC_NOT_SET)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_BUTTON,     (char *) labels [4], 5,
			      NOTICE_FOCUS_XY,   focusX, focusY,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (triggerEvent != UIC_NOT_SET)
      result = notice_prompt (object, &noticeEvent,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_BUTTON,     (char *) labels [4], 5,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_TRIGGER,    triggerEvent,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else if (event)
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_BUTTON,     (char *) labels [4], 5,
			      NOTICE_FOCUS_XY,   event_x (event),
			                         event_y (event),
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    else
      result = notice_prompt (object, NULL,
			      NOTICE_MESSAGE_STRINGS_ARRAY_PTR, messages,
			      NOTICE_BUTTON,     (char *) labels [0], 1,
			      NOTICE_BUTTON,     (char *) labels [1], 2,
			      NOTICE_BUTTON,     (char *) labels [2], 3,
			      NOTICE_BUTTON,     (char *) labels [3], 4,
			      NOTICE_BUTTON,     (char *) labels [4], 5,
			      NOTICE_NO_BEEPING, (beepFlag ? FALSE : TRUE),
			      notice_font,       font_pointer,
			      NULL);
    
    break;
  }

  // Return the result of the user interaction
  return result;
}

#ifdef TEST
#endif /* TEST */
