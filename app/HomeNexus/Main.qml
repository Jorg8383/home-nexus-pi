import QtCore
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

    required property bool embeddedMode

    width: 800
    height: 480
    visible: true

    // ---- Mode-dependent window behaviour ---------------------------
    visibility: window.embeddedMode ? Window.FullScreen : Window.Windowed
    flags: window.embeddedMode ? Qt.FramelessWindowHint : Qt.Window
    title: window.embeddedMode ? "" : "Home-Nexus-Pi"

    // ---- Background ------------------------------------------------

    Settings {
        id: appSettings
        category: "Appearance"

        // non-volatil property via QSettings
        property string backgroundMode: "brushedMetal"
    }

    property alias backgroundMode: appSettings.backgroundMode

    background: Item {
        Rectangle {
            anchors.fill: parent
            color: Style.appColors.contentBackground
        }

        Image {
            anchors.fill: parent
            source: Style.assetsPath + "BrushedMetal.jpg"
            fillMode: Image.PreserveAspectCrop
            visible: window.backgroundMode === "brushedMetal"
        }
    }

    // ----- Header ---------------------------------------------------
    header: AppHeader {
        title: {
            const currentPage = window.activeStack.currentItem as Page
            return currentPage && currentPage.title !== ""
                    ? currentPage.title
                    : qsTr("Home-Nexus-Pi")
        }

        backButtonVisible: window.activeStack.depth > 1

        onBackRequested: window.activeStack.pop()
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
            hasNotification: AppNotificationCenter.hasBannerNotification
            severity: AppNotificationCenter.bannerSeverity
            message: AppNotificationCenter.bannerMessage
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
