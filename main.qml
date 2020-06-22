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

    ColumnLayout {
        id: loCol

        anchors.fill: parent
        anchors.margins: defaultMargin
        spacing: 10

        Rectangle{
            implicitWidth: 400
            implicitHeight: 400
            color: "lightgray"

            Repeater {
                Layout.fillWidth: true
                Layout.fillHeight: true

                model: LayoutModel {
                    id: layoutModel
                }

                delegate: Rectangle {
                    x: rect.x
                    y: rect.y
                    width: rect.width
                    height: rect.height

                    Text {
                        id: text
                        text: model.id

                        anchors.centerIn: parent
                    }

                    color: "red"
                }
            }
        }

        RowLayout {
            TextField {
                id: textInput
                Layout.fillWidth: true

                validator: IntValidator {
                    bottom: 0
                }
                selectByMouse: true
                font.preferShaping: false

                onFocusChanged: focus ? selectAll() : deselect()
            }

            Button {
                text: "Set"

                onClicked: layoutModel.setUsersNum(parseInt(textInput.text))
            }
        }
    }
}
