import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import HomeNexus

import "ui/components"
import "ui/pages/home"
import "ui/pages/climate"
import "ui/pages/plants"
import "ui/pages/settings"

ApplicationWindow {
    id: window
    width: 800
    height: 480
    visible: true

    // ---- Mode-dependent window behaviour ---------------------------
    visibility: embeddedMode ? Window.FullScreen : Window.Windowed
    flags: embeddedMode ? Qt.FramelessWindowHint : Qt.Window
    title: embeddedMode ? "" : "Home-Nexus-Pi"

    Component.onCompleted: {
        if (embeddedMode)
            showFullScreen()
        else
            showNormal()
    }

    // ---- Background ------------------------------------------------
    background: Image {
        id: backgroundImage
        source: Style.assetsPath + "BrushedMetal.jpg"
        fillMode: Image.PreserveAspectCrop
    }
    // ----- Header ---------------------------------------------------
    header: AppHeader {
        title: activeStack.currentItem
               && activeStack.currentItem.title !== ""
               ? activeStack.currentItem.title
               : "Home-Nexus-Pi"

        backButtonVisible: activeStack.depth > 1

        onBackRequested: activeStack.pop()
    }

    // ----- Footer ---------------------------------------------------
    footer: TabBar {
        id: footerTabBar
        width: parent.width

        currentIndex: window.currentSectionIndex
        onCurrentIndexChanged: window.currentSectionIndex = currentIndex

        AppTabButton {iconName: "home-icon"}
        AppTabButton {iconName: "climate-icon"}
        AppTabButton {iconName: "plant-icon"}
        AppTabButton {iconName: "setting-icon"}
    }

    // ----- Content---------------------------------------------------

    property int currentSectionIndex: 0

    property StackView activeStack: {
        switch (currentSectionIndex) {
        case 0: return homeStack
        case 1: return climateStack
        case 2: return plantsStack
        case 3: return settingsStack
        default: return homeStack
        }
    }


    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        AppWarningBanner {
            Layout.fillWidth: true
            hasNotification: appNotificationCenter.hasBannerNotification
            severity: appNotificationCenter.bannerSeverity
            message: appNotificationCenter.bannerMessage
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: window.currentSectionIndex

            StackView {
                id: homeStack
                initialItem: HomeRootPage {}
            }

            StackView {
                id: climateStack
                initialItem: ClimateRootPage {
                    stackView: climateStack
                }
            }

            StackView {
                id: plantsStack
                initialItem: PlantsRootPage {
                    stackView: plantsStack
                }
            }

            StackView {
                id: settingsStack
                initialItem: SettingsRootPage {
                    stackView: settingsStack
                }
            }
        }
    }

}
