/*H+ CSArrayTile.cc
*
*    SCCS ID: @(#)CSArrayTile.cc 1.1 93/02/08 SMI
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the member functions of the CSArrayTile class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                              
*    --------- ---------- ------------------------
*
*H-*/
#ifndef lint
static char sccsid [] = "@(#)CSArrayTile.cc 1.1 93/02/08 SMI"; 
#endif

#ifdef SLINGSHOT

// Includes
#include "CSArrayTile.h"

/*F+ CSArrayTile::CSArrayTile ()
*
*    PURPOSE:
*
*    Constructor of the CSArrayTile class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSArrayTile::CSArrayTile ()
{
  setCSArrayTileDefaults ();
}

/*F+ CSArrayTile::setCSArrayTileDefaults ()
*
*    PURPOSE:
*
*    Initializes the data members of the CSArrayTile class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::setCSArrayTileDefaults ()
{
  alignment = (Array_tile_align) UIC_NOT_SET;
  autoPosition = FALSE;
  autoPositionSet = FALSE;
  columnGap = UIC_NOT_SET;
  columnWidth = UIC_NOT_SET;
  horizontalLines = FALSE;
  horizontalLinesSet = FALSE;
  layout = (Layout) UIC_NOT_SET;
  numColumns = UIC_NOT_SET;
  numRows = UIC_NOT_SET;
  rowGap = UIC_NOT_SET;
  rowHeight = UIC_NOT_SET;
  verticalLines = FALSE;
  verticalLinesSet = FALSE;
}

/*F+ CSArrayTile::~CSArrayTile ()
*
*    PURPOSE:
*
*    Destructor of the CSArrayTile class.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
CSArrayTile::~CSArrayTile ()
{
}

/*F+ void CSArrayTile::setColumn (CSComponent &child, int column)
*
*    PURPOSE:
*
*    Sets the column of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*    column - The column
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::setColumn (CSComponent &child, int column)
{
  addChild (child);

  TilePosition *position = findPosition (child);
  position->column = column;

  if (owObject) {
    xv_set (owObject,
	    ARRAY_TILE_COLUMN, child.getXViewObject (), column,
	    NULL);
  }
}

/*F+ int CSArrayTile::getColumn (CSComponent &child)
*
*    PURPOSE:
*
*    Gets the column of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*
*    RETURN VALUES:
*
*    Returns the column index of the child.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int CSArrayTile::getColumn (CSComponent &child)
{
  int column = UIC_NOT_SET;

  if (owObject) {
    column = (int) xv_get (owObject,
			   ARRAY_TILE_COLUMN, child.getXViewObject (),
			   NULL);
  }
  else {
    TilePosition *position = findPosition (child, FALSE);

    if (position)
      column = position->column;
  }

  return column;
}

/*F+ void CSArrayTile::setRow (CSComponent &child, int row)
*
*    PURPOSE:
*
*    Sets the row of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*    row - The row
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::setRow (CSComponent &child, int row)
{
  addChild (child);

  TilePosition *position = findPosition (child);
  position->row = row;

  if (owObject) {
    xv_set (owObject,
	    ARRAY_TILE_ROW, child.getXViewObject (), row,
	    NULL);
  }
}

/*F+ int CSArrayTile::getRow (CSComponent &child)
*
*    PURPOSE:
*
*    Gets the row of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*
*    RETURN VALUES:
*
*    Returns the row index of the child.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
int CSArrayTile::getRow (CSComponent &child)
{
  int row = UIC_NOT_SET;

  if (owObject) {
    row = (int) xv_get (owObject,
			ARRAY_TILE_ROW, child.getXViewObject (),
			NULL);
  }
  else {
    TilePosition *position = findPosition (child, FALSE);

    if (position)
      row = position->row;
  }

  return row;
}

/*F+ void CSArrayTile::setPosition (CSComponent &child, int row, int column)
*
*    PURPOSE:
*
*    Sets the position of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*    position - The position
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::setPosition (CSComponent &child, int row, int column)
{
  addChild (child);

  TilePosition *position = findPosition (child);
  position->row = row;
  position->column = column;

  if (owObject) {
    xv_set (owObject,
	    ARRAY_TILE_POSITION, child.getXViewObject (), row, column,
	    NULL);
  }
}

/*F+ void CSArrayTile::getPosition (CSComponent &child, int &row, int &column)
*
*    PURPOSE:
*
*    Gets the position of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*    row - The row of the child
*    column - The column of the child
*
*    RETURN VALUES:
*
*    Returns the row and column indexes of the child through
*    the arguments.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::getPosition (CSComponent &child, int &row, int &column)
{
  row = UIC_NOT_SET;
  column = UIC_NOT_SET;

  if (owObject) {
    row = (int) xv_get (owObject,
			ARRAY_TILE_ROW, child.getXViewObject (),
			NULL);

    column = (int) xv_get (owObject,
			   ARRAY_TILE_COLUMN, child.getXViewObject (),
			   NULL);
  }
  else {
    TilePosition *position = findPosition (child, FALSE);

    if (position) {
      row = position->row;
      column = position->column;
    }
  }
}

/*F+ void CSArrayTile::createObject (UIObject *parent)
*
*    PURPOSE:
*
*    Creates the XView object.
*
*    PARAMETERS:
*
*    parent - The parent of the CSArrayTile object.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
void CSArrayTile::createObject (UIObject *parent)
{
  owObject = xv_create (parent->getXViewObject (), ARRAY_TILE, NULL);

  if (alignment != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_ALIGN, alignment, NULL);

  if (autoPositionSet)
    xv_set (owObject, ARRAY_TILE_AUTO_POSITION, autoPosition, NULL);

  if (columnGap != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_COLUMN_GAP, columnGap, NULL);

  if (columnWidth != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_COLUMN_WIDTH, columnWidth, NULL);

  if (horizontalLinesSet)
    xv_set (owObject, ARRAY_TILE_HLINES, horizontalLines, NULL);

  if (layout == ROW_LAYOUT)
    xv_set (owObject,
	    ARRAY_TILE_LAYOUT, ARRAY_TILE_LAYOUT_ROW,
	    NULL);
  else if (layout == COLUMN_LAYOUT)
    xv_set (owObject,
	    ARRAY_TILE_LAYOUT, ARRAY_TILE_LAYOUT_COLUMN,
	    NULL);

  if (numColumns != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_N_COLUMNS, numColumns, NULL);

  if (numRows != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_N_ROWS, numRows, NULL);

  if (rowGap != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_ROW_GAP, rowGap, NULL);

  if (rowHeight != UIC_NOT_SET)
    xv_set (owObject, ARRAY_TILE_ROW_HEIGHT, rowHeight, NULL);

  if (verticalLinesSet)
    xv_set (owObject, ARRAY_TILE_VLINES, verticalLines, NULL);

  setCSComponentAttributes (parent);

  int num_positions = positions.getSize ();
  for (int i=0; i < num_positions; i++) {
    TilePosition *position = (TilePosition *) positions [i];
    addChild (*position->child);

    if ((position->row != UIC_NOT_SET) &&
	(position->column != UIC_NOT_SET)) {
      xv_set (owObject,
	      ARRAY_TILE_POSITION, position->child->getXViewObject (),
	                           position->row,
	                           position->column,
	      NULL);
    }
    else if (position->row != UIC_NOT_SET) {
      xv_set (owObject,
	      ARRAY_TILE_ROW, position->child->getXViewObject (),
	                      position->row,
	      NULL);
    }
    else if (position->column != UIC_NOT_SET) {
      xv_set (owObject,
	      ARRAY_TILE_COLUMN, position->child->getXViewObject (),
	                         position->column,
	      NULL);
    }
  }
}

/*F+ TilePosition *CSArrayTile::findPosition (CSComponent &child,
*                                             Boolean      create_new_position)
*
*    PURPOSE:
*
*    Returns the position information of the specified child.
*
*    PARAMETERS:
*
*    child - The child component
*    create_new_position - If TRUE, a new position information structure 
*                          is created if the specified child was not found.
*
*    RETURN VALUES:
*
*    Returns a pointer to a tile position information structure.  Returns
*    NULL if no position information was found or created.
*
*    REVISION HISTORY:
*
*    Who       When       Why                    
*    --------- ---------- ------------------------
*
*F-*/
TilePosition *CSArrayTile::findPosition (CSComponent &child,
                                         Boolean      create_new_position)
{
  int           num_positions = positions.getSize ();
  TilePosition *position = (TilePosition *) NULL;

  for (int i=0; i < num_positions; i++) {
    if (((TilePosition *) positions [i])->child == &child) {
      position = (TilePosition *) positions [i];
      break;
    }
  }

  if (!position && create_new_position) {
    position = new TilePosition;
    position->child = &child;

    positions.addItem ((Generic *) position);
  }

  return position;
}

#ifdef TEST
#endif /* TEST */

#endif /* SLINGSHOT */
