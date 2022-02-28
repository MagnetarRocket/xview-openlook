/*H+ CSArrayTile.h
*
*    SCCS ID: @(#)CSArrayTile.h 1.1 93/02/08 SMI 
*
*    Copyright (c) 1992-1993 Sun Microsystems, Inc.  All rights reserved.
*    See LEGAL_NOTICE file for terms of the license.
*
*    PURPOSE:
*
*    Defines the CSArrayTile class.
*
*    AUTHOR: Mark Soloway
*
*    REVISION HISTORY:
*
*    Who       When       Why                   
*    --------- ---------- -----------------------
*
*H-*/

/*C+ CSArrayTile
*
*    DESCRIPTION:
*
*    The CSArrayTile component is a collection object that positions its
*    children like tiles in an array of tiles.
*
*    INHERITED CLASSES:
*
*    CSComponent - A CSArrayTile object is a CSComponent.
*
*    PUBLIC MEMBER FUNCTIONS:
*
*    void setAlignment (Array_tile_align new_alignment) - Specifies the
*    positioning of the children within the array tile:
*
*         ARRAY_TILE_ALIGN_NORTHWEST
*         ARRAY_TILE_ALIGN_NORTH
*         ARRAY_TILE_ALIGN_NORTHEAST
*         ARRAY_TILE_ALIGN_WEST
*         ARRAY_TILE_ALIGN_CENTER
*         ARRAY_TILE_ALIGN_EAST
*         ARRAY_TILE_ALIGN_SOUTHWEST
*         ARRAY_TILE_ALIGN_SOUTH
*         ARRAY_TILE_ALIGN_SOUTHEAST
*
*    void setAutoPosition (Boolean flag) - Specifies whether or not
*    tile positions for newly added children should be set automatically.
*
*    void setColumnGap (int gap) - The horizontal pixel distance between
*    tiles.
*
*    void setColumnWidth (int width) - The horozontal width of columns.
*
*    void setHorizontalLines (Boolean flag) - Specifies whether or not
*    horizontal lines should be drawn around the children.
*
*    void setLayout (Layout new_layout) - Specifies the direction children
*    should be layed out if automatic positioning is being used:
*
*         ROW_LAYOUT
*         COLUMN_LAYOUT
*
*    void setDisplayColumns (int num_columns) - Specifies the number of
*    columns in the array tile object.
*
*    void setDisplayRows (int num_rows) - Specifies the number of rows
*    in the array tile object.
*
*    void setRowGap (int gap) - Specifies the vertical distance between
*    tiles.
*
*    void setRowHeight (int width) - Specifies the vertical height of tiles.
*
*    void setVerticalLines (Boolean flag) - - Specifies whether or not
*    vertical lines should be drawn around the children.
*
*    void setColumn (CSComponent &child, int column)
*    int  getColumn (CSComponent &child) - Sets or gets the column index
*    of a child object.
*
*    void setRow (CSComponent &child, int row);
*    int  getRow (CSComponent &child) - Sets or gets the row index
*    of a child object.
*  
*    void setPosition (CSComponent &child, int row, int column)
*    void getPosition (CSComponent &child, int &row, int &column) - Sets or
*    gets the row and column position of a child object.
*
*    PROTECTED MEMBER FUNCTIONS:
*
*    virtual void createObject (UIObject *) - Creates the XView
*    ARRAY_TILE encapsulated by this object.
*
*    PRIVATE MEMBER FUNCTIONS:
*
*    void setCSArrayTileDefaults () - Initializes data members.
*
*/

#ifndef _CS_ARRAY_TILE_H
#define _CS_ARRAY_TILE_H

// Includes
#include "CSComponent.h"

#include <sspkg/array.h>

class TilePosition : public Generic {
public:
  TilePosition () { child = (CSComponent *) NULL;
		    row = UIC_NOT_SET;
		    column = UIC_NOT_SET; }
  ~TilePosition () {}

  CSComponent *child;
  int          row;
  int          column;
};

// Class Definition
class CSArrayTile : public CSComponent {
public:
  CSArrayTile ();
  ~CSArrayTile ();

  void setAlignment (Array_tile_align new_alignment)
    {
      alignment = new_alignment;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_ALIGN, alignment, NULL);
    }

  void setAutoPosition (Boolean flag)
    {
      autoPosition = flag;
      autoPositionSet = TRUE;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_AUTO_POSITION, autoPosition, NULL);
    }

  void setColumnGap (int gap)
    {
      columnGap = gap;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_COLUMN_GAP, columnGap, NULL);
    }

  void setColumnWidth (int width)
    {
      columnWidth = width;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_COLUMN_WIDTH, columnWidth, NULL);
    }

  void setHorizontalLines (Boolean flag)
    {
      horizontalLines = flag;
      horizontalLinesSet = TRUE;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_HLINES, horizontalLines, NULL);
    }

  void setLayout (Layout new_layout)
    {
      layout = new_layout;

      if (owObject) {
	if (layout == ROW_LAYOUT)
	  xv_set (owObject,
		  ARRAY_TILE_LAYOUT, ARRAY_TILE_LAYOUT_ROW,
		  NULL);
	else if (layout == COLUMN_LAYOUT)
	  xv_set (owObject,
		  ARRAY_TILE_LAYOUT, ARRAY_TILE_LAYOUT_COLUMN,
		  NULL);
      }
    }

  void setDisplayColumns (int num_columns)
    {
      numColumns = num_columns;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_N_COLUMNS, numColumns, NULL);
    }

  void setDisplayRows (int num_rows)
    {
      numRows = num_rows;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_N_ROWS, numRows, NULL);
    }

  void setRowGap (int gap)
    {
      rowGap = gap;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_ROW_GAP, rowGap, NULL);
    }

  void setRowHeight (int width)
    {
      rowHeight = width;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_ROW_HEIGHT, rowHeight, NULL);
    }

  void setVerticalLines (Boolean flag)
    {
      verticalLines = flag;
      verticalLinesSet = TRUE;

      if (owObject)
	xv_set (owObject, ARRAY_TILE_HLINES, horizontalLines, NULL);
    }

  void setColumn (CSComponent &child, int column);
  int  getColumn (CSComponent &child);

  void setRow (CSComponent &child, int row);
  int  getRow (CSComponent &child);
  
  void setPosition (CSComponent &child, int row, int column);
  void getPosition (CSComponent &child, int &row, int &column);

protected:
  Array_tile_align alignment;
  Boolean          autoPosition;
  Boolean          autoPositionSet;
  int              columnGap;
  int              columnWidth;
  Boolean          horizontalLines;
  Boolean          horizontalLinesSet;
  Layout           layout;
  int              numColumns;
  int              numRows;
  int              rowGap;
  int              rowHeight;
  Boolean          verticalLines;
  Boolean          verticalLinesSet;
  GenericList      positions;

  virtual void createObject (UIObject *parent);

private:    
  void          setCSArrayTileDefaults ();
  TilePosition *findPosition (CSComponent &child,
			      Boolean      create_new_position = TRUE);
};

#endif /* _CS_ARRAY_TILE_H */
