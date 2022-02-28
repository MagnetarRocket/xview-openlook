;GIL-3
(
(
	:type                   :base-window
	:name                   entry_window
	:owner                  nil
	:width                  307
	:height                 132
	:background-color       ""
	:foreground-color       ""
	:label                  "Entries"
	:label-type             :string
	:initial-state          :open
	:show-footer            t
	:resizable              t
	:icon-file              ""
	:icon-label             ""
	:icon-mask-file         ""
	:event-handler          nil
	:user-data              ()
	:actions                ()
)
(
	:type                   :control-area
	:name                   entry_display
	:owner                  entry_window
	:help                   "A window for adding and deleting entires
to and from a scrolling list."
	:x                      0
	:y                      0
	:width                  307
	:height                 132
	:background-color       ""
	:foreground-color       ""
	:initial-state          :visible
	:show-border            nil
	:menu                   nil
	:event-handler          nil
	:user-data              ()
	:actions                ()
)
(
	:type                   :button
	:name                   add_button
	:owner                  entry_display
	:help                   "Press this button to add the entry in the
text input field to the scrolling list."
	:x                      8
	:y                      16
	:width                  42
	:height                 19
	:constant-width         nil
	:button-type            :normal
	:foreground-color       ""
	:label                  "Add"
	:label-type             :string
	:initial-state          :active
	:menu                   nil
	:notify-handler         add_button_handler
	:event-handler          nil
	:user-data              ()
	:actions                (
		(
		:from                   (entry_window add_button)
		:when                   (Notify )
		:to                     (entry_window add_button)
		:function_type          CallFunction
		:arg_type               ()
		:action                 (add_button_handler)
		)
	)
)
(
	:type                   :scrolling-list
	:name                   entry_list
	:owner                  entry_display
	:help                   "Entries are displayed here."
	:x                      76
	:y                      16
	:width                  200
	:height                 74
	:value-x                76
	:value-y                16
	:rows                   3
	:foreground-color       ""
	:label                  ""
	:title                  ""
	:label-type             :string
	:layout-type            :horizontal
	:read-only              nil
	:multiple-selections    nil
	:selection-required     nil
	:initial-state          :active
	:droppable              nil
	:default-drop-site      nil
	:menu                   nil
	:notify-handler         entry_list_handler
	:event-handler          nil
	:initial-list-values    ()
	:initial-list-glyphs    ()
	:initial-selections     ()
	:user-data              ()
	:actions                (
		(
		:from                   (entry_window entry_list)
		:when                   (Notify )
		:to                     (entry_window entry_list)
		:function_type          CallFunction
		:arg_type               ()
		:action                 (entry_list_handler)
		)
	)
)
(
	:type                   :button
	:name                   delete_button
	:owner                  entry_display
	:help                   "Press this button to delete the entry in the
text input field to the scrolling list."
	:x                      8
	:y                      48
	:width                  57
	:height                 19
	:constant-width         nil
	:button-type            :normal
	:foreground-color       ""
	:label                  "Delete"
	:label-type             :string
	:initial-state          :active
	:menu                   nil
	:notify-handler         delete_button_handler
	:event-handler          nil
	:user-data              ()
	:actions                (
		(
		:from                   (entry_window delete_button)
		:when                   (Notify )
		:to                     (entry_window delete_button)
		:function_type          CallFunction
		:arg_type               ()
		:action                 (delete_button_handler)
		)
	)
)
(
	:type                   :text-field
	:name                   entry_field
	:owner                  entry_display
	:help                   "Type an entry to be added or deleted."
	:x                      8
	:y                      104
	:width                  287
	:height                 15
	:value-x                55
	:value-y                104
	:value-length           30
	:stored-length          30
	:rows                   3
	:foreground-color       ""
	:text-type              :alphanumeric
	:label                  "Entry:"
	:label-type             :string
	:layout-type            :horizontal
	:value-underlined       t
	:initial-value          ""
	:initial-state          :active
	:read-only              nil
	:notify-handler         nil
	:event-handler          nil
	:user-data              ()
	:actions                ()
)
)
