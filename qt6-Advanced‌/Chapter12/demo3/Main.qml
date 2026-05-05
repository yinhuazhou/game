import QtQuick
import QtQuick.Controls

Window {
    width: 560
    height: 220
    visible: true
    title: qsTr("事件处理")
    Column{
        spacing: 30
        anchors.centerIn: parent
        Label{
            id:myLabel
            text:"这是一个标签"
            font.pixelSize: 22
            font.italic: true
            font.bold: true
            font.underline: true
        }
        Button{
            text: "单击我"
            width: 100
            height: 40
            onClicked: {
                myLabel.text="千山鸟飞绝，万径人踪灭。"
            }
        }
    }
}
