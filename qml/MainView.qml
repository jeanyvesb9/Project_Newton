import QtQuick 2.5
import Qt.labs.controls 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: root
    property var fonts

    color: "#222222"

    RowLayout {
        id: mainHorizontalLayout
        anchors.bottomMargin: 40
        anchors.rightMargin: 40
        anchors.leftMargin: 40
        anchors.topMargin: 40
        spacing: 40
        anchors.fill: parent

        Board {
            id: board
            Layout.preferredWidth: height
            Layout.fillHeight: true

            borderWidth: 2
        }

        ColumnLayout {
            id: uiData
            width: 100
            height: 100
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            Text {
                id: text1
                color: "#ffffff"
                text: "Checkers"
                Layout.fillWidth: true
                font.family: fonts.adobeClean.name
                font.pixelSize: 55

            }

            Text {
                id: text2
                Layout.fillWidth: true
                color: "#eeeeee"
                text: "Genetic Learning Neural Network Algorithm"
                font.family: fonts.adobeClean.name
                wrapMode: Text.WordWrap
                font.letterSpacing: 3
                font.pixelSize: 21
            }

            Text {
                id: text3
                color: "#eeeeee"
                text: "Jean Yves Beaucamp  -  6ºB"
                font.family: fonts.adobeClean.name
                font.pixelSize: 15
            }

            Item {
                Layout.fillHeight: true
            }

            Text {
                id: clock
                text: "2:07:24  -  47 moves"
                font.family: fonts.segoeUi.name
                font.pixelSize: 25
                color: "#FFFFFF"
            }

            Text {
                id: turnIndicator
                text: "It's your turn!"
                font.family: fonts.segoeUi.name
                font.pixelSize: 20
                color: "#FFFFFF"
            }

            Item {
                Layout.preferredHeight: 40
            }


            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#000000"

                Rectangle {
                    id: redPlayerTitle
                    color: "#222222"
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width / 2 - 1
                    height: redPlayerTitleText.height

                    Text {
                        id: redPlayerTitleText
                        anchors.top: parent.top
                        anchors.topMargin: 3
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        height: font.pixelSize + 3
                        text: "Reds"
                        font.family: fonts.adobeClean.name
                        font.pixelSize: 18
                        color: "#FFFFFF"
                    }
                }

                Rectangle {
                    color: "#222222"
                    anchors.left: parent.left
                    anchors.top: redPlayerTitle.bottom
                    anchors.topMargin: 2
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1

                    Text {
                        id: redPlayerDataName
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        text: "Captured Pieces"
                        font.family: fonts.segoeUi.name
                        font.pixelSize: 15
                        color: "#FFFFFF"
                    }

                    Text {
                        id: redPlayerData
                        anchors.right: parent.right
                        anchors.rightMargin: 3
                        anchors.top: redPlayerDataName.top
                        text: "7"
                        font.family: fonts.segoeUi.name
                        font.pixelSize: 15
                        color: "#FFFFFF"
                    }
                }

                Rectangle {
                    id: blackPlayerTitle
                    color: "#222222"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    width: parent.width / 2 - 1
                    height: blackPlayerTitleText.height

                    Text {
                        id: blackPlayerTitleText
                        anchors.top: parent.top
                        anchors.topMargin: 3
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        height: font.pixelSize + 3
                        text: "Blacks"
                        font.family: fonts.adobeClean.name
                        font.pixelSize: 18
                        color: "#FFFFFF"
                    }
                }

                Rectangle {
                    color: "#222222"
                    anchors.right: parent.right
                    anchors.top: blackPlayerTitle.bottom
                    anchors.topMargin: 2
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1

                    Text {
                        id: blackPlayerDataName
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 3
                        text: "Captured Pieces"
                        font.family: fonts.segoeUi.name
                        font.pixelSize: 15
                        color: "#FFFFFF"
                    }

                    Text {
                        id: blackPlayerData
                        anchors.right: parent.right
                        anchors.rightMargin: 3
                        anchors.top: blackPlayerDataName.top
                        text: "2"
                        font.family: fonts.segoeUi.name
                        font.pixelSize: 15
                        color: "#FFFFFF"
                    }
                }

            }

            Item {
                Layout.preferredHeight: 30
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                Button {
                    text: "Reset"
                    onClicked: console.log(window.height + 'x' + window.width)
                }

                Button {
                    text: "Help"
                }
            }
        }
    }

}
