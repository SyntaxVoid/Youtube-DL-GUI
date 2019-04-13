#pragma once

// GUIDefaults.h
// This file serves to define the needed parameters for the GUI application.
// It defines the graphics styles, the identifies used to refer to the various
// edit controls, and the coordinates for all of the GUI components.


// Various graphics styles for the GUI components
#define WS_NORESIZE         WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU
#define BUTTON_STYLE        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
#define CHECKBOX_STYLE      WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP
#define DROPDOWN_STYLE      CBS_DROPDOWNLIST | WS_VISIBLE | CBS_HASSTRINGS | WS_CHILD | \
                            WS_OVERLAPPED | WS_VSCROLL
#define ENTRY_STYLE         WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL

// ID Controls for the buttons. Can get the HWND by GetDlgItem(master_hWnd, IDC_X)
#define IDC_URL_ENTRY       (1100)
#define IDC_PASTE_INTO_URL  (1101)
#define IDC_FORMAT_ENTRY    (1102)
#define IDC_EMBED_CHECKBOX  (1103)
#define IDC_OUTPUT_ENTRY    (1104)
#define IDC_BROWSE_BUTTON   (1105)
#define IDC_CLOSE_BUTTON    (1106)
#define IDC_GO_BUTTON       (1107)

// Padding.. Although I barely used this zzz...
#define padx                (2)
#define pady                (2)

// Main Window Coords
#define MainWindowWidth     (650)
#define MainWindowLength    (260)
#define headerTextHeight    (40)
#define defaultTextHeight   (18)

// Header Coords
#define headerXi            (40)
#define headerYi            (20)
#define headerXf            (MainWindowWidth - headerXi)
#define headerYf            (headerTextHeight + headerYi)

// URL Label Coords
#define URLLabelXi          (0)
#define URLLabelYi          (headerYf + pady)
#define URLLabelXf          (URLLabelXi + 200)
#define URLLabelYf          (URLLabelYi + defaultTextHeight)

// URL Entry Coords
#define URLEntryXi          (URLLabelXf + padx)
#define URLEntryYi          (URLLabelYi - pady / 2)
#define URLEntryWidth       (400)
#define URLEntryHeight      (defaultTextHeight + 2)
#define URLEntryXf          (URLEntryXi + URLEntryWidth)
#define URLEntryYf          (URLLabelYi + URLEntryHeight)

// Paste Button Coords
#define pasteButtonSize     (URLEntryHeight + 2)
#define pasteButtonXi       (URLEntryXf + padx)
#define pasteButtonYi       (URLEntryYi - 1)
#define pasteButtonWidth    (pasteButtonSize)
#define pasteButtonHeight   (pasteButtonSize)
#define pasteButtonXf       (pasteButtonXi + pasteButtonWidth)
#define pasteButtonYf       (pasteButtonYi + pasteButtonHeight)

// Format List Coords
#define formatListXi        (URLEntryXi - 110)
#define formatListYi        (URLLabelYf + 20)
#define formatListWidth     (150)
#define formatListHeight    (200)
#define formatListXf        (formatListXi + formatListWidth)
#define formatListYf        (formatListYi + formatListHeight)

// Embed Album Art Checkbox Coords
#define embedCheckXi        (formatListXf + 50)
#define embedCheckYi        (formatListYi + 3)
#define embedCheckWidth     (300)
#define embedCheckHeight    (defaultTextHeight + 2)
#define embedCheckXf        (embedCheckXi + embedCheckWidth)
#define embedCheckYf        (embedCheckYi + embedCheckHeight)

// Output Label Coords
#define OutputLabelXi       (URLLabelXi)
#define OutputLabelYi       (embedCheckYf + 20)
#define OutputLabelXf       (OutputLabelXi + 200)
#define OutputLabelYf       (OutputLabelYi + defaultTextHeight + 2)

// Output Entry Coords
#define outputEntryXi       (OutputLabelXf + padx)
#define outputEntryYi       (OutputLabelYi - pady / 2)
#define outputEntryWidth    (320)
#define outputEntryHeight   (defaultTextHeight + 2)
#define outputEntryXf       (outputEntryXi + outputEntryWidth)
#define outputEntryYf       (outputEntryYi + outputEntryHeight)

// Browse Button Coords
#define browseButtonXi      (outputEntryXf + padx)
#define browseButtonYi      (outputEntryYi - 1)
#define browseButtonWidth   (pasteButtonSize)
#define browseButtonHeight  (pasteButtonSize)
#define browseButtonXf      (browseButtonXi + browseButtonWidth)
#define browseButtonYf      (browseButtonYi + browseButtonHeight)

// Close button Coords
#define closeButtonXi       (outputEntryXf - 85)
#define closeButtonYi       (outputEntryYf + 10)
#define closeButtonWidth    (120)
#define closeButtonHeight   (headerTextHeight + 1)
#define closeButtonXf       (closeButtonXi + closeButtonWidth)
#define closeButtonYf       (closeButtonYi + closeButtonHeight)

// Go Button Coords
#define goButtonXi          (closeButtonXf + padx)
#define goButtonYi          (closeButtonYi)
#define goButtonWidth       (70)
#define goButtonHeight      (closeButtonHeight)
#define goButtonXf          (goButtonXi + goButtonWidth)
#define goButtonYf          (goButtonYi + goButtonHeight)
