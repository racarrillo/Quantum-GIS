/***************************************************************************
    HoloDark.qml  -  Hola Dark Theme
     --------------------------------------
    Date                 : 07-AuAugg-2012
    Copyright            : (C) 2012 by Ramon Carrillo
    Email                : racarrillo91 at gmail.com
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 1.1

Item {
    // See https://github.com/android/platform_frameworks_base/blob/master/core/res/res/values/colors.xm

    property color screenBackground: "#000000"
    property color screenBackgroundGradStart: "#000000"
    property color screenBackgroundGradEnd: "#101214"

    // Action bar
    property color actionBarBackground: "#282828"
    property color actionBarForeground: "#f4f4f4"
    property int actionBarHeight: 40*dp

    // Menu
    property color menuBackground: "#282828"
    property color menuForeground: "#f4f4f4"
    property color menuSeparator: "#3e3e3e"
    property int menuElementHeight: 48*dp

    // Dialog
    property color dialogHeaderForeground: "#33b5e5"
    property color dialogHeaderBorder: "#33b5e5"
    property int dialogHeaderHeight: 52*dp
    property color dialogBackground: "#282828"
    property color dialogForeground: "#d4d4d4"
    property color dialogButtonForeground: "#d4d4d4"
    property color dialogButtonBorder: "#3e3e3e"

    property color listSeparator: "#3e3e3e"
    property int listHeight: 52*dp

    // Button
    property color buttonBackground: "#999999"
    property color buttonForeground: "#ffffff"
    property int buttonFontSize: 16*dp
    property int buttonHeight: 48*dp
    property int buttonWidth: 192*dp
    property color activatedButtonBackground: "#33b5e5"
    property color activatedButtonForeground: "#ffffff"
    property color deactivatedButtonBackground: "#1c1e1f"
    property color deactivatedButtonForeground: "#616162"

    property color iconForeground: "#cdcdcd"

    property color highlighted: "#33b5e5"

    // Inputs
    property int inputLabelHeight: 24*dp
    property color inputLabelForeground: "#bbbbbb"
    property int inputHeight: 36*dp
    property color inputForeground: "#ffffff"
    property color inputBorder:  "#bbbbbb"

    // Checkboxes
    property string checkboxNormalCheckedIcon: "/themes/holodark/checkbox-normal-checked.png"
    property string checkboxNormalUncheckedIcon: "/themes/holodark/checkbox-normal-unchecked.png"
    property string checkboxDisabledCheckedIcon: "/themes/holodark/checkbox-disabled-checked.png"
    property string checkboxDisabledUncheckedIcon: "/themes/holodark/checkbox-disabled-unchecked.png"

    // Some colors
    property color blueLight: "#33b5e5"
    property color greeLight: "#99cc00"
    property color redLight: "#ff4444"
    property color blueDark: "#0099cc"
    property color greeDark: "#669900"
    property color redDark: "#cc0000"
    property color purple: "#aa66cc"
    property color orangeLight: "#ffbb33"
    property color orangeDark: "#ff8800"
    property color blueBright: "#00ddff"

    property int smallFontSize: 16*dp
    property int mediumFontSize: 24*dp
    property int largeFontSize: 32*dp

    // Toasts
    property color toastBackground: "#272727"
    property color toastForeground: "#ffffff"
    property int toastFontSize: 16*dp

    // Icons
    property string addIcon: "/themes/holodark/add.png"
    property string newIcon: "/themes/holodark/new.png"
    property string removeIcon: "/themes/holodark/remove.png"
    property string moveIcon: "/themes/holodark/move.png"
    property string locationIcon: "/themes/holodark/location.png"
    property string locationAddIcon: "/themes/holodark/location-add.png"
    property string undoIcon: "/themes/holodark/undo.png"
    property string previousIcon: "/themes/holodark/previous.png"
    property string layersIcon: "/themes/holodark/layers.png"
}
