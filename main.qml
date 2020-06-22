import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import LayoutModel 1.0

Window {
    visible: true
    title: qsTr("Hello World")

    property int defaultMargin: 5

    minimumWidth: loCol.implicitWidth + 2 * defaultMargin
    minimumHeight: loCol.implicitHeight + 2 * defaultMargin

    LayoutModel {
        id: layoutModel
        screenSize: "400 x 400"
    }

    ColumnLayout {
        id: loCol

        implicitWidth: screenRect.implicitWidth

        anchors.fill: parent
        anchors.margins: defaultMargin
        spacing: 10

        Rectangle{
            id: screenRect
            color: "lightgray"

            implicitWidth: layoutModel.screenSize.width
            implicitHeight: layoutModel.screenSize.height

            Layout.alignment: Qt.AlignCenter

            Repeater {
                Layout.fillWidth: true
                Layout.fillHeight: true

                model: layoutModel

                delegate: Rectangle {
                    color: "dodgerblue"

                    x: rect.x
                    y: rect.y
                    width: rect.width
                    height: rect.height

                    Text {
                        id: text
                        text: model.id

                        anchors.centerIn: parent
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            TextField {
                id: widthInput

                Layout.fillWidth: true
                Layout.preferredWidth: 40

                validator: IntValidator { bottom: 0 }
                selectByMouse: true
                font.preferShaping: false

                onFocusChanged: focus ? selectAll() : deselect()
            }

            Text {
                text: qsTr("x")
            }

            TextField {
                id: heigthInput

                Layout.fillWidth: true
                Layout.preferredWidth: 40

                validator: IntValidator { bottom: 0 }
                selectByMouse: true
                font.preferShaping: false

                onFocusChanged: focus ? selectAll() : deselect()
            }

            Button {
                text: "Set width"

                onClicked: layoutModel.screenSize = Qt.size(widthInput.text, heigthInput.text)
            }
        }

        RowLayout {
            TextField {
                id: userNumInput

                Layout.fillWidth: true
                Layout.preferredWidth: 40

                validator: IntValidator {
                    bottom: 0
                }
                selectByMouse: true
                font.preferShaping: false

                onFocusChanged: focus ? selectAll() : deselect()
            }

            Button {
                text: "Set num windows"

                onClicked: layoutModel.setUsersNum(userNumInput.text)
            }
        }
    }
}
