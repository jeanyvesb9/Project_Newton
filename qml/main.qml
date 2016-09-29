import QtQuick 2.6
import QtQuick.Layouts 1.0
import Qt.labs.controls 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 920
    height: 550
    title: qsTr("Checkers")

    Fonts { id: fonts }

    MainView {
        anchors.fill: parent
        fonts: fonts
    }

    /*
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        Page {
            Label {
                text: qsTr("Second page")
                anchors.centerIn: parent
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
    }*/
}
