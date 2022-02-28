/*
 *	(c) Copyright 1991 Sun Microsystems, Inc.  All rights reserved.
 *	See LEGAL_NOTICE file for terms and restrictions.
 */

#ifndef lint
#ifdef sccs
static char     sccsid[] = "@(#)selection.c 1.20 93/05/25";
#endif
#endif

#include <xview/sel_pkg.h>
#include <sspkg/rectobj.h>
#include "rectobj_impl.h"
#include <assert.h>

static	Rectobj_list	*selected_list;
static	int		num_selected;
static	Selection_owner	curr_owner;

#define call_selection_proc(rectobj, add, event)			\
		/*  Rectobj rectobj; int add; */			\
		{							\
		Proc_ptr	p;					\
		p = (Proc_ptr) xv_get(rectobj, RECTOBJ_SELECTION_PROC);	\
		if(p)							\
			(p)(rectobj, add, event);       		\
		}

#define HIGHLIGHT_KEY   XV_KEY_DATA, RECTOBJ_HIGHLIGHT_LIST


Rectobj_list *
rectobj_get_selected_list()
{
	/* 1.0 & 2.0.x style: always returns global list */
	return(selected_list);
}

Rectobj_list *
rectobj_get_selected_list2(rectobj)
	Rectobj rectobj;
{
	Rectobj group;
	Xv_opaque value;
	/*
	 * 2.0+ function for getting selected list.
	 * This function does what rectobj_get_selected_list
	 * does, but also returns a group list if it is a member
	 * of a group.
	 */
	if(rectobj == NULL)
		return(selected_list);

	group = rectobj_upsearch(rectobj, &value, RECTOBJ_HIGHLIGHT_GROUP, 0);
	if(group == NULL)
		return(selected_list);
	return((Rectobj_list*) xv_get(group, HIGHLIGHT_KEY));
}


static void
rectobj_set_selected_list(rectobj, new_list)
	Rectobj_list *new_list;
{
	Rectobj group;
	Xv_opaque value; /* unused */

	if(rectobj == NULL) {
		selected_list = new_list;
		return;
	}

	group = rectobj_upsearch(rectobj, &value, RECTOBJ_HIGHLIGHT_GROUP, 0);
	if(group == NULL) {
		selected_list = new_list;
	} else {
		xv_set(group,
			HIGHLIGHT_KEY, new_list,
			NULL);
	}
}

static void
rectobj_selection_own(sel_owner, event, own)
	Selection_owner	sel_owner;
	Event		*event;
	int		own;
{
	if(sel_owner) {
		/*
		 * Don't release if already released, or if already owned.
		 *
		 * Beware of recursion problem with selection lose proc, 
		 * which is called before SEL_OWN is set to TRUE.
		 */
		if(xv_get(sel_owner, SEL_OWN) == own) {
			return;
		}

		/*
		 * printf("%s selection\n", (own?"owning":"disowning"));
		 */
		if(event) {
			xv_set(sel_owner,
				SEL_OWN, own,
				SEL_TIME, &event_time(event),
				NULL);
		} else {
			xv_set(sel_owner,
				SEL_OWN, own,
				NULL);
		}
	}
}


static void
clear_selected(new, event)
	Rectobj	new;
	Event	*event;
{
	/* clear all but new from selected list */
	Rectobj		tmp;
	Rectobj_list	*this, *next;

	while(this = rectobj_get_selected_list2(new)) {
		tmp = RECTOBJ_LIST_HANDLE(this);
		next = (Rectobj_list*) list_first(
				list_delete_node(this));
		rectobj_set_selected_list(new, next);
		if(tmp != new) {
			rectobj_set_paint_style(tmp, event, RECTOBJ_NORMAL);
			num_selected--;
			RF_UNSET(RECTOBJ_PRIVATE(tmp), RF_SELECTED);
			call_selection_proc(tmp, FALSE, event);
		}
	}
}


void
rectobj_add_to_selected_list(new, exclusive, event)
	Rectobj         new;
	int             exclusive;
	Event		*event;
{
	Rectobj_list   *new_node;
	Rectobj_list   *this, *next;
	Rectobj_info   *rinfo;
	Selection_owner sel_owner;
	int		previously_selected;

	if(!new) {
		/* preserve obsolute behavior */
		if(exclusive) {
			clear_selected(new, event);
			rectobj_selection_own(curr_owner, event, FALSE);
			curr_owner = NULL;
		}
		return;
	}

	rinfo = RECTOBJ_PRIVATE(new);

	if(!RF_IS_SET(rinfo, RF_SELECTABLE))
		return;

	previously_selected = RF_IS_SET(rinfo, RF_SELECTED);

	if(exclusive) {
		clear_selected(new, event);

		if(rinfo->parent &&
                   xv_get(rinfo->parent, RECTOBJ_RESTACK_CHILDREN))
			rectobj_set_stacking_position(new, 0x7FFFFFFF);
	}

	if(!previously_selected) {
		new_node = xv_alloc(Rectobj_list);
		new_node->handle = (void *) new;
		this = rectobj_get_selected_list2(new);
		next = (Rectobj_list *) list_concat(this, new_node);
		rectobj_set_selected_list(new, next);
		RF_SET(rinfo, RF_SELECTED);
		call_selection_proc(new, TRUE, event);

		/*
		 * The object was not previously selected, own the
		 * selection and repaint in new state.
		 */
		if(rectobj_upsearch(new, &sel_owner, RECTOBJ_SELECTION_OWNER)) {
			/*
			 * Own the selection if application has created and
			 * attached a selection owner, and the previously
			 * selected list was empty.
			 */
			rectobj_selection_own(sel_owner, event, TRUE);
			curr_owner = sel_owner;
		}
		num_selected++;
	}
}


void
rectobj_clear_selected_list(reference, event)
	Rectobj	reference;
	Event   *event;
{
	/*
	 * clear the global selection list by:
	 * 	rectobj_clear_selected_list(NULL, event);
	 * clear a group by:
	 * 	rectobj_clear_selected_list(group_member, event);
	 */
	clear_selected(reference, event);
	if(reference == NULL) {
		rectobj_selection_own(curr_owner, event, FALSE);
		curr_owner = NULL;
	}
}

void
rectobj_del_from_selected_list(rectobj, event)
	Rectobj rectobj;
	Event	*event;
{
	Rectobj_list *node;
	Rectobj_list *this, *next;
	Rectobj_info *rinfo = RECTOBJ_PRIVATE(rectobj);

	if(RF_IS_SET(rinfo, RF_SELECTED)) {
		RF_UNSET(rinfo, RF_SELECTED);
		this = rectobj_get_selected_list2(rectobj);
		if(node = list_find(this, (void*)rectobj)) {
			next = (Rectobj_list*) 
				list_first( list_delete_node(node) );
			rectobj_set_selected_list(rectobj, next);
			num_selected--;
			call_selection_proc(rectobj, FALSE, event);
		}
		/*assert(node); should never be RF_SELECTED and not on list */
	}

	if(num_selected == 0) {
		rectobj_selection_own(curr_owner, event, FALSE);
		curr_owner = NULL;
	}
}


void
rectobj_lose_selection()
{
	/*
	 * Intended to be called at a SEL_CLEAR event on the canvas window.
	 * This does not disown selection because the selection package is 
	 * supposed to do this before it gets here.
	 */
	curr_owner = NULL;
	clear_selected(NULL, NULL);
}

