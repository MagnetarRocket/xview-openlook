/*H+  NoticePrompt.h
*
*    SCCS ID: @(#)NoticePrompt.h 1.1 91/10/24 SMI 
*
*    Copyright (c) 1990,1991 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the NoticePrompt class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*    MGS       09/10/90   Original creation
*    JDZ       05/01/91   Preparation for code review
*
*H-*/

/*C+  NoticePrompt
*
*    DESCRIPTION:
*
*    The NoticePrompt class provides a prompt window for notifying 
*    the user of a situation requiring attention.  The graphics 
*    object points to the area of the user interface that requires 
*    attention.  You can instantiate a NoticePrompt object anywhere, 
*    but you can only show it from a Notify, Event or Done handler.
*
*    INHERITED CLASSES:
*
*    UIObject - In addition to the normal benefits provided by the
*    inheritance mechanism, inheriting the UIObject allows this
*    class to be used as UIObject class inside the Handlers.
*
*    ORDINARY DATA MEMBERS:
*
*    Boolean       beepFlag     - TRUE if a beep signal is to 
*    accompany the NoticePrompt object.
*
*    char        **messages     - The list of all messages that will 
*    be displayed.
*
*    GenericList   labels       - List of button labels.
*
*    char         *yesLabel     - Pointer to the text corresponding 
*    to the "yes" label.
*
*    char         *noLabel      - Pointer to the text corresponding
*    to the "no" label.
*
*    int           focusX       - The X coordinate of the 
*    location to which the prompt is bringing the user's attention.
*    Default: Not set.
*
*    int           focusY       - The Y coordinate of the 
*    location to which the prompt is bringing the user's attention.
*    Default: Not set.
*
*    int           triggerEvent - the event type NoticePrompt object 
*    will use as an accelerator.
*
*    Event	   noticeEvent  - The XView event structure used
*    to hold trigger event information.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void   setButtonLabels (char *, ...) - Indicates the number of 
*    buttons on the NoticePrompt and sets the labels on the buttons.
*    The number of labels dictates the number of buttons.  Maximum 
*    allowed is five. The show member function returns a value 1-5, 
*    based on the button a user selects. The parameter list MUST be 
*    terminated with a NULL.
*
*    int    addButtonLabel (char *) - Allows to add up to five 
*    buttons, one at a time.
*
*    void   deleteButtonLabel (int) - Allows to delete up to five
*    buttons, one at a time. The button number is the parameter 
*    to be supplied.
*
*    void   deleteButtonLabel (char *) -  Allows to delete up to five
*    buttons, one at a time. The button's label is the parameter
*    to be supplied.
*
*    void   setYesAndNoLabels (char *yes, char *no) - Allows to 
*    specify the text of the "yes" and "no" labels.  These are the 
*    labels that respectively correspond to a return of NOTICE_YES 
*    and NOTICE_NO.  The member function that is used last 
*    (setButtonLabels or setYesAndNoLabels) generates the buttons 
*    on the NoticePrompt.
*
*    void   setMessage (char *msg) - Allows to change the existing 
*    prompt message to a new message.
*
*    void   setMessages (char **msgs) - Specifies a NULL terminated array 
*    of strings to be used as the NoticePrompt messages.
*
*    void   setMessages (char *, ...) - Allows to change the existing
*    prompt message to a new message consisting of the specified 
*    strings. The parameter list MUST be terminated with a NULL.
*
*    void   setFocusLocation (int x, int y) - Sets the X and Y 
*    coordinates of the location to which the prompt is bringing the 
*    user's attention.
*
*    void   setTriggerEvent (int trigger_event) - Allows specification
*    of an event that the NoticePrompt object will use as input. An 
*    event trigger for a NoticePrompt that is being displayed from 
*    Event ot Notify Handler is allowed to be specified.
*
*    void   setBeeping (Boolean beep_flag) - Sets the flag to switch 
*    sound on/off.  Default: TRUE.
*
*    int    show (UIObject *event_object) - Displays the NoticePrompt.
*    The show member function requires the pointer to the object 
*    that was passed to the handler.  The show member function 
*    returns the following values:
*
*         NOTICE_YES       - The "yes" button was selected.
*         NOTICE_NO        - The "no" button was selected.
*         NOTICE_TRIGGERED - The trigger event occurred.
*         NOTICE_FAILED    - The NoticePrompt failed.
*         1-5              - The number of the button the user 
*                            selected, if button labels were specified.
*
*    If not otherwise specified, the focus location of the NoticePrompt
*    is that of the X-Y location of the event passed to the show member
*    function.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    void deleteMessages () - Deletes the array containing the NoticePrompt
*    messages (messages data member).
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    int  createNoticePromptWithLabels (UIObject *) - Creates a 
*    NoticePrompt object with previously specified labels.
*
*C-*/
#ifndef _NOTICEPROMPT_H
#define _NOTICEPROMPT_H

// Includes
#include "UIObject.h"

#include <xview/notice.h>

// Class Definition
class NoticePrompt : public UIObject {
public:
  NoticePrompt ()
    { 
      beepFlag = TRUE;
      messages = (char **) NULL;
      yesLabel = (char *) NULL;
      noLabel = (char *) NULL;
      focusX = UIC_NOT_SET;
      focusY = UIC_NOT_SET;
      triggerEvent = UIC_NOT_SET;
    }
  NoticePrompt (char **msgs)
    { 
      beepFlag = TRUE;
      yesLabel = (char *) NULL;
      noLabel = (char *) NULL;
      focusX = UIC_NOT_SET;
      focusY = UIC_NOT_SET;
      triggerEvent = UIC_NOT_SET;
      int size;

      for (size=0; msgs [size]; size++) ;
      messages = new char* [size+1];
      for (int i=0; i < size; i++)
        messages [i] = strcpy (new char [strlen (msgs [i]) + 1], msgs [i]);
      messages [size] = (char *) NULL;
    }
  NoticePrompt (char *, ...);
  ~NoticePrompt ()
    {
      deleteMessages ();

      int num_labels = labels.getSize ();
      for (int i=0; i < num_labels; i++)
	delete (char *) labels.removeItem (0); 

      if (yesLabel) delete yesLabel;
      if (noLabel) delete noLabel;
    }
  void setButtonLabels (char *, ...);
  int  addButtonLabel (char *);
  void deleteButtonLabel (int);
  void deleteButtonLabel (char *);
  void setYesAndNoLabels (char *yes, char *no)
    { 
      if (yes && no) {
	if (yesLabel) delete yesLabel;
	if (noLabel) delete noLabel;
	yesLabel = strcpy (new char [strlen (yes) + 1], yes);
	noLabel = strcpy (new char [strlen (no) + 1], no);

        int num_labels = labels.getSize ();
        for (int i=0; i < num_labels; i++)
	  delete (char *) labels.removeItem (0); 
      }
    }
  void setMessage (char *msg) 
    {
      if (msg) {
        deleteMessages ();

	messages = new char* [2];
        messages [0] = strcpy (new char [strlen (msg) + 1], msg);
	messages [1] = (char *) NULL;
      }
    }
  void setMessages (char **msgs)
    {
      if (msgs) {
	deleteMessages ();

	int size;
	for (size=0; msgs [size]; size++) ;
	messages = new char* [size+1];
	for (int i=0; i < size; i++)
	  messages [i] = strcpy (new char [strlen (msgs [i]) + 1], msgs [i]);
	messages [size] = (char *) NULL;
      }
    }
  void setMessages (char *, ...);
  void setFocusLocation (int x, int y) { focusX = x; focusY = y; }
  void setTriggerEvent (int trigger_event) { triggerEvent = trigger_event; }
  void setBeeping (Boolean beep_flag) { beepFlag = beep_flag; }
  int  show (UIObject *event_object);

protected:
  Boolean       beepFlag;
  char        **messages;
  GenericList   labels;
  char         *yesLabel;
  char         *noLabel;
  int           focusX;
  int           focusY;
  int           triggerEvent;
  Event		noticeEvent;

  void deleteMessages ()
    {
      if (messages) {
	for (int i=0; messages [i]; i++)
	  delete (messages [i]);
	delete messages;
      }
    }

private:
  int  createNoticePromptWithLabels (UIObject *);
};

#endif /* _NOTICEPROMPT_H */
