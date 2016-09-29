import QtQuick 2.4
import Qt.labs.controls 1.0

Item {
    id: root

    property color color1: "#4F4F4F"
    property color color2: "#FAFAFA"
    property color borderColor: "#000000"
    property int borderWidth: 3

    Rectangle {
        anchors.fill: parent;
        color: root.borderColor
    }

    Grid {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width - root.borderWidth * 2
        height: parent.height - root.borderWidth * 2

        rows: 8
        columns: 8
        spacing: root.borderWidth

        Repeater {
            model: 64 //cells

            Rectangle {
                width: (parent.width - root.borderWidth * 7) / 8
                height: (parent.height - root.borderWidth * 7) / 8
                color: {
                    var row = Math.floor(index / 8);
                    var column = index % 8;
                    if((row + column) % 2 === 1)
                        return root.color1;
                    else
                        return root.color2;
                }
            }
        }
    }
}
