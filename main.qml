import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

//import MyModel 1.0


/*Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("My File Io")

    //    File {
    //        id: file
    //        //        source: "/home/gr/Documents/log.txt"
    //        source: "/home/gr/Documents/Repo/company_repo/build-e600-Desktop_Qt_5_15_2_GCC_64bit-Debug/UI/2022-08-17T13:46:55.log"
    //        onSourceChanged: {
    //            console.log("onSourceChanged")
    //        }
    //        onTextChanged: {
    //            console.log("onTextChanged")
    //        }
    //    }

    //    Window {
    //        visible: false
    //        id: windowView
    //        width: 500
    //        height: 400
    //        modality: Qt.WindowModal
    //        flags: Qt.Window

    //                TextArea {
    //                    id: textarea
    //                    width: parent.width
    //                    height: parent.height
    //                    //            anchors.fill: parent
    //                    font.pixelSize: 16
    //                    // File::text() will be called if use "file.txt"
    //                    text: file.text

    //                    // the content of TextArea is modified
    //                    onTextChanged: file.text = text
    //                }
    //    }

    TextArea {
        id: area
        width: 500
        height: 500
        text: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
              + "sed do eiusmod tempor incididunt ut labore et dolore magna "
              + "aliqua. Ut enim ad minim veniam, quis nostrud exercitation "
              + "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. "
    }
    Rectangle {
        width: 50
        height: 20
        color: "green"

        MouseArea {
            anchors.fill: parent
            //            onClicked: windowView.visible = true
        }
    }
}*/
Window {
    visible: true
    id: windowView
    width: 300
    height: 100
    x: 700
    y: 200

    modality:     Qt.WindowModal
//            flags: Qt.Window | Qt.ScrollBarAlwaysOn
    flags: Qt.Window

    color: "#666666"
    ScrollView {
        id: view
        anchors.fill: parent
        TextArea {
            id: textArea
            cursorVisible: true
            //            focusReason: Qt.MouseFocusReason
            //            wrapMode: TextArea.Wrap //换行
            //            font.pixelSize: 20
            //            font.weight: Font.Light
            //            focus: true
            //            textFormat: TextArea.AutoText
            //            selectByMouse: true
            //            selectByKeyboard: true
            color: "red"
            text: "侠客行\n赵客缦胡缨，吴钩霜雪明。\n银鞍照白马，飒沓如流星。\n十步杀一人，千里不留行。\n事了拂衣去，深藏身与名。\n闲过信陵饮⑹，脱剑膝前横。"
        }
    }
} //Rectangle {//    id: textRect//    //    anchors.top: parent.top//    //    anchors.topMargin: 100//    //    anchors.left: parent.left
//    //    anchors.leftMargin: 150
//    width: 300
//    height: 100
//    color: "#666666"
//    ScrollView {
//        id: view
//        anchors.fill: parent
//        TextArea {
//            id: textArea
//            cursorVisible: true
//            //            focusReason: Qt.MouseFocusReason
//            //            wrapMode: TextArea.Wrap //换行
//            //            font.pixelSize: 20
//            //            font.weight: Font.Light
//            //            focus: true
//            //            textFormat: TextArea.AutoText
//            //            selectByMouse: true
//            //            selectByKeyboard: true
//            color: "red"
//            text: "侠客行\n赵客缦胡缨，吴钩霜雪明。\n银鞍照白马，飒沓如流星。\n十步杀一人，千里不留行。\n事了拂衣去，深藏身与名。\n闲过信陵饮⑹，脱剑膝前横。"
//        }
//    }
//}

