import QtQuick
import QtQuick.Controls

Window {
    width: 560
    height: 220
    visible: true
    title: qsTr("Hello World")
    Button{
        text:"单击我"
        id:myButton
        y:60
        x:60
        onClicked: close()
    }
}
